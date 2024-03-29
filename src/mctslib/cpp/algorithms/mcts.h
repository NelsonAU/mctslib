#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#ifdef MCTSLIB_USING_PYBIND11
#include <pybind11/pybind11.h>
#endif

#pragma once

namespace mctslib {

struct Empty {};

// Contains all the stats needed for a node in MCTS.
struct MCTSStats {
    double evaluation;
    int action_id;
    double backprop_reward;
    int visits;

    // In this case we discard max_action_value as MCTSStats doesn't need to store information
    // about all available actions
    MCTSStats(double eval, int action_id, int max_action_value)
        : evaluation(eval)
        , action_id(action_id)
        , backprop_reward(0)
        , visits(0)
    {
    }

    // In this case both constructors are the same, but may not be for more complicated Stats.
    // eval_only is used when stats will be discared (like in simulations).
    static MCTSStats eval_only(double eval, int action_id)
    {
        return MCTSStats(eval, action_id, 0);
    }

    double average_reward() const
    {
        return visits ? backprop_reward / visits : std::numeric_limits<double>::min();
    }
};

// Implements the MCTS Base class. This class is designed to be inherited from, but should never
// be instantiated. All algorithms that are intended to be used should be marked final.
// For MCTS, that means the class defined below this one.
template <class Node, bool using_dag, bool randomize_ties, bool constant_action_space>
class MCTSBase {
public:

#ifdef MCTSLIB_USING_PYBIND11
    // Used for naming different algorithm implementations for Python bindings
    inline const static std::string opts_str =  std::string(using_dag ? "dag" : "tree") +
        "_" + (randomize_ties ? "rng_ties" : "no_rng_ties") +
        "_" + (constant_action_space ? "const_action_space" : "non_const_action_space");

    inline const static std::string alg_str = "MCTS";
    inline const static std::string str_id = opts_str + "_" + alg_str;
#endif

    double exploration_weight;
    int rollout_depth;
    std::mt19937 rng;
    const double backprop_decay;
    const int max_action_value;
    std::shared_ptr<Node> current_node_ptr;
    typename std::conditional<using_dag, std::unordered_map<Node, std::shared_ptr<Node>>, Empty>::type transposition_table;
    typename std::conditional<constant_action_space, std::vector<int>, Empty>::type action_space;

    // global stats
    int total_iters = 0;
    double total_cpu_time = 0;


    // Args... will be forwarded to the Node class
    template <typename... Args>
    MCTSBase(double backprop_decay, int max_action_value, Args... args)
        : backprop_decay(backprop_decay)
        , max_action_value(max_action_value)
        , current_node_ptr(std::make_shared<Node>(args..., max_action_value))

    {
        if constexpr (constant_action_space) {
            // fill vector with 0...action_space_size - 1
            action_space = std::vector<int>(max_action_value + 1);
            std::iota(std::begin(action_space), std::end(action_space), 0);
        }
    }

    // Used to choose the next node to progress to after all rollouts have been completed.
    std::shared_ptr<Node> choose_best_node()
    {
        std::shared_ptr<Node> best_node = *std::max_element(
            current_node_ptr->children().begin(),
            current_node_ptr->children().end(),
            [](const std::shared_ptr<Node> left, const std::shared_ptr<Node> right) {
                return left->stats.average_reward() < right->stats.average_reward();
            });
        return choose_node(best_node);
    }

    // Used to choose a node manually. This is helpful when you may want to explore choosing nodes
    // based on metrics besides their average reward. choose_best_node should always invoke
    // choose_node because there may be algorithm specific bookkeeping that needs to be done
    // immediately before/after a node is chosen. E.g. updating ref_nodes list in GRAVE.
    virtual std::shared_ptr<Node> choose_node(std::shared_ptr<Node> chosen_node) {
        current_node_ptr = chosen_node;
        return chosen_node;
    }


    // Searches for cpu_time seconds and returns the current node with updated statistics.
    std::shared_ptr<Node> search_using_cpu_time(double cpu_time, int rollout_depth, double exploration_weight) {
        this->exploration_weight = exploration_weight;
        this->rollout_depth = rollout_depth;

        auto start = std::chrono::high_resolution_clock::now();

        for (;;) {
            rollout();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;

            if (diff.count() > cpu_time) {
                total_cpu_time += diff.count();
                break;
            }
        }

        return current_node_ptr;
    }


    // Searches for iters iterations and returns the current node with updated statistics.
    std::shared_ptr<Node> search_using_iters(int iters, int rollout_depth, double exploration_weight) {
        this->exploration_weight = exploration_weight;
        this->rollout_depth = rollout_depth;

        for (int i = 0; i < iters; i++) {
            rollout();
        }

        return current_node_ptr;
    }

    // Backpropagates the result of a simulation back up the path taken during the selection phase.
    virtual void backpropagate(std::vector<std::shared_ptr<Node>> path, double reward)
    {
        for (std::shared_ptr<Node> node_ptr : path) {
            node_ptr->stats.visits++;
            node_ptr->stats.backprop_reward += reward;
            reward *= backprop_decay;
        }
    }

    // Creates the children of a leaf node, and, if using_dag is enabled, checks the transposition
    // table and replaces the appropriate child nodes if necessary. If randomize_ties is enabled,
    // then we shuffle the legal action set for the node so that it's children are in a random
    // order.
    virtual void expand(std::shared_ptr<Node> node_ptr)
    {
        std::vector<int> node_action_space;

        if constexpr (constant_action_space) {
            node_action_space = action_space;
        } else {
            node_action_space = node_ptr->get_legal_actions();
        }

        // This solves multiple problems: in choose, we would prefer ties be broken randomly,
        // but that doesn't account for biases in select where we iterate over child nodes to find
        // a non-expanded one. Shuffling here means that we won't have to worry about either.
        // Of course it is still toggleable based on whether the user likes ties to be randomized,
        // but in my testing this is quite important. Could be problematic for large action spaces.
        if constexpr (randomize_ties) {
            std::shuffle(std::begin(node_action_space), std::end(node_action_space), rng);
        }

        std::vector<std::shared_ptr<Node>> children;
        children.reserve(node_action_space.size());

        for (int action_id : node_action_space) {
            std::shared_ptr<Node> child_ptr = node_ptr->apply_action(action_id, max_action_value);

            if constexpr (using_dag) {
                if (transposition_table.contains(*child_ptr)) {
                    child_ptr = transposition_table.at(*child_ptr);
                } else {
                    transposition_table[*child_ptr] = child_ptr;
                }
            }

            children.push_back(child_ptr);
        }

        node_ptr->set_children(children);
    }

    virtual void rollout()
    {
        std::vector<std::shared_ptr<Node>> path = select();
        std::shared_ptr<Node> leaf = path.back();
        this->expand(leaf);
        double reward = simulate(leaf);
        backpropagate(path, reward);
        total_iters += 1;
    }

    virtual double simulate(std::shared_ptr<Node> node_ptr)
    {
        Node node = *node_ptr;
        std::vector<int> legal_action_space;

        if constexpr (constant_action_space) {
            legal_action_space = action_space;
        } else {
            legal_action_space = node.get_legal_actions();
        }

        for (int i = 0; i < rollout_depth; i++) {
            if (node.is_terminal())
                break;

            std::uniform_int_distribution<int> dist { 0, (int)legal_action_space.size() - 1};
            int next_action = dist(rng);
            node = node.apply_action_eval_only(next_action);

            if constexpr (!constant_action_space) {
                legal_action_space = node.get_legal_actions();
            }
        }

        return node.stats.evaluation;
    }

    // Traverses already expanded nodes in the tree/dag based on the tree_policy_metric. In the case
    // of MCTS, this is UCT, but could differ for other algorithms.
    virtual std::vector<std::shared_ptr<Node>> select()
    {
        std::shared_ptr<Node> node_ptr = current_node_ptr;
        std::vector<std::shared_ptr<Node>> path { node_ptr };

        for (;;) {
            if (!node_ptr->is_expanded() || node_ptr->is_terminal())
                return path;

            for (std::shared_ptr<Node> child : node_ptr->children()) {
                if (!child->is_expanded()) {
                    path.push_back(child);
                    return path;
                }
            }

            node_ptr = *std::max_element(
                node_ptr->children().begin(),
                node_ptr->children().end(),
                [this](const std::shared_ptr<Node> left, const std::shared_ptr<Node> right) {
                    return tree_policy_metric(left) > tree_policy_metric(right);
                }
            );

            path.push_back(node_ptr);
        }
    }

    // For MCTS, this is UCT
    virtual double tree_policy_metric(const std::shared_ptr<Node> node_ptr)
    {
        double avg_reward = node_ptr->stats.average_reward();
        double log_N = std::log(current_node_ptr->stats.visits);
        return avg_reward + exploration_weight * sqrt(log_N / node_ptr->stats.visits);
    }

#ifdef MCTSLIB_USING_PYBIND11
    pybind11::dict get_global_stats() {
        pybind11::dict dict;
        dict["total_iters"] = total_iters;
        dict["total_cpu_time"] = total_cpu_time;
        return dict;
    }
#endif

};

// The purpose of this class is to create a completely concrete class which the compiler will then
// hopefully devirtualize and optimize better. This may be unnecessary - but makes intent clear and
// keeps me from worrying about the compiler not doing the right thing :-). Most importantly,
// ensures that the transposition table won't be used when it isn't enabled.
template <class Node, bool using_dag, bool randomize_ties, bool constant_action_space>
class MCTS final : public MCTSBase<Node, using_dag, randomize_ties, constant_action_space> {
    using MCTSBase<Node, using_dag, randomize_ties, constant_action_space>::MCTSBase;
};
}
