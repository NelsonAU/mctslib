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

#pragma once

namespace mctslib {

// Contains all the stats needed for a node in MCTS.
struct MCTSStats {
    double evaluation;
    uint action_id;
    double backprop_reward;
    uint visits;

    // In this case we discard action_space_size as MCTSStats doesn't need to store information
    // about all available actions
    MCTSStats(double eval, uint action_id, uint max_action_value)
        : evaluation(eval)
        , action_id(action_id)
        , backprop_reward(0)
        , visits(0)
    {
    }
    // In this case both constructors are the same, but may not be for more complicated Stats
    static MCTSStats eval_only(double eval, uint action_id)
    {
        return MCTSStats(eval, action_id, 0);
    }

    double average_reward() const
    {
        return visits ? backprop_reward / visits : std::numeric_limits<double>::min();
    }
};

// Implements the MCTS Base class. This class is designed to be inherited from, but should never
// be instantiated independently. All algorithms that are designed to be used should be marked
// final. To use MCTS, use the template found below this one.
template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool constant_action_space>
class MCTSBase {
public:
    inline const static std::string opts_str = std::string(using_iters ? "iters" : "cpu_time") + "_" + (using_dag ? "dag" : "tree") + "_" + (randomize_ties ? "rng_ties" : "no_rng_ties") + "_" + (constant_action_space ? "const_action_space" : "non_const_action_space");
    inline const static std::string alg_str = "MCTS";
    inline const static std::string str_id = opts_str + "_" + alg_str;

    // Used to store all options of the algorithm that can vary per iteration. Every call to move
    // will overwrite the previous Settings object.
    struct Settings {
        uint rollout_depth;
        typename std::conditional<using_iters, uint, std::monostate>::type iters;
        typename std::conditional<using_iters, std::monostate, double>::type cpu_time;
        double exploration_weight;

        Settings(uint rollout_depth, uint iters, double exploration_weight) requires(std::is_same_v<decltype(iters), uint>)
            : rollout_depth(rollout_depth)
            , iters(iters)
            , exploration_weight(exploration_weight)
        {
        }

        Settings(uint rollout_depth, double cpu_time, double exploration_weight) requires(std::is_same_v<decltype(cpu_time), double>)
            : rollout_depth(rollout_depth)
            , cpu_time(cpu_time)
            , exploration_weight(exploration_weight)
        {
        }

        // Used to give default values on construction of the algorithm, but should never be used
        // otherwise.
        explicit Settings() = default;
    };

    std::mt19937 rng;
    const double backprop_decay;
    const uint max_action_value;
    std::shared_ptr<Node> current_node_ptr;
    typename std::conditional<using_dag, std::unordered_map<Node, std::shared_ptr<Node>>, std::monostate>::type transposition_table;
    typename std::conditional<constant_action_space, std::vector<uint>, std::monostate>::type action_space;
    Settings settings = Settings();

    // Args... will be forwarded to the Node class
    template <typename... Args>
    MCTSBase(double backprop_decay, uint max_action_value, Args... args)
        : backprop_decay(backprop_decay)
        , max_action_value(max_action_value)
        , current_node_ptr(std::make_shared<Node>(args..., max_action_value))

    {
        if constexpr (constant_action_space) {
            // fill vector with 0...action_space_size - 1
            action_space = std::vector<uint>(max_action_value + 1);
            std::iota(std::begin(action_space), std::end(action_space), 0);
        }
    }

    // Used to choose the next node to progress to after all rollouts have been completed.
    virtual std::shared_ptr<Node> choose(std::shared_ptr<Node> node_ptr)
    {
        return *std::max_element(
            node_ptr->children().begin(),
            node_ptr->children().end(),
            [](const std::shared_ptr<Node> left, const std::shared_ptr<Node> right) {
                return left->stats.average_reward() < right->stats.average_reward();
            });
    }

    // Entrypoint to the algorithm. Takes in all the settings that might vary per iteration, and
    // performs rollouts until an iteration count or time limit is reached, then returns a new node.
    //
    // Of note: if using CPU time, the algorithm will always use an extra bit more time than it is
    // given, and will never preemtively stop itself from starting a new iteration even if it has
    // very close to no time left. If a usecase requires that the algorithm make a choice before
    // some strict deadline, then the value of cpu_time should be chosen conservatively.
    virtual Node move(const Settings new_settings)
    {
        settings = new_settings;

        if constexpr (using_iters) {
            for (uint i = 0; i < settings.iters; i++)
                rollout(current_node_ptr);
        } else {
            auto start = std::chrono::high_resolution_clock::now();

            for (;;) {
                rollout(current_node_ptr);

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end - start;

                if (diff.count() > settings.cpu_time)
                    break;
            }
        }

        current_node_ptr = choose(current_node_ptr);
        return *current_node_ptr;
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
        std::vector<uint> node_action_space;

        // get appropriate legal action set
        if constexpr (constant_action_space) {
            node_action_space = action_space;
        } else {
            node_action_space = node_ptr->get_legal_actions();
        }

        // This solves multiple problems: in choose, we would prefer ties be broken randomly,
        // but that doesn't account for biases in select where we iterate over child nodes to find
        // a non-expanded one. Shuffling here means that we won't have to worry about either.
        // Of course it is still toggleable based on whether the user likes ties to be randomized,
        // but in my testing this is quite important.
        if constexpr (randomize_ties) {
            std::shuffle(std::begin(node_action_space), std::end(node_action_space), rng);
        }

        std::vector<std::shared_ptr<Node>> children;
        children.reserve(node_action_space.size());

        for (uint action_id : node_action_space) {
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

    virtual void rollout(std::shared_ptr<Node> node_ptr)
    {
        std::vector<std::shared_ptr<Node>> path = select(node_ptr);
        std::shared_ptr<Node> leaf = path.back();
        this->expand(leaf);
        double reward = simulate(leaf);
        backpropagate(path, reward);
    }

    virtual double simulate(std::shared_ptr<Node> node_ptr)
    {
        Node node = *node_ptr;
        std::vector<uint> legal_action_space;

        if constexpr (constant_action_space) {
            legal_action_space = action_space;
        } else {
            legal_action_space = node.get_legal_actions();
        }

        for (uint i = 0; i < settings.rollout_depth; i++) {
            if (node.is_terminal())
                break;

            std::uniform_int_distribution<uint> dist { 0, (uint)legal_action_space.size() - 1};
            uint next_action = dist(rng);
            node = node.apply_action_eval_only(next_action);

            if constexpr (!constant_action_space) {
                legal_action_space = node.get_legal_actions();
            }
        }

        return node.stats.evaluation;
    }

    // Traverses already expanded nodes in the tree/dag based on the tree_policy_metric. In the case
    // of MCTS, this is UCT, but could differ for other algorithms.
    virtual std::vector<std::shared_ptr<Node>> select(std::shared_ptr<Node> node_ptr)
    {
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
                });

            path.push_back(node_ptr);
        }
    }

    // For MCTS, this is UCT
    virtual double tree_policy_metric(const std::shared_ptr<Node> node_ptr)
    {
        double avg_reward = node_ptr->stats.average_reward();
        double log_N = std::log(current_node_ptr->stats.visits);
        return avg_reward + settings.exploration_weight * sqrt(log_N / node_ptr->stats.visits);
    }
};

// The purpose of this class is to create a completely concrete class which the compiler will then
// hopefully devirtualize and optimize better. This may be unnecessary - but makes intent clear and
// keeps me from worrying about the compiler not doing the right thing :-).
template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool constant_action_space>
class MCTS final : public MCTSBase<Node, using_iters, using_dag, randomize_ties, constant_action_space> {
    using MCTSBase<Node, using_iters, using_dag, randomize_ties, constant_action_space>::MCTSBase;
};
}
