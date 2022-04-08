#include <algorithm>
#include <chrono>
#include <limits>
#include <memory>
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
    double backprop_reward;
    uint visits;

    explicit MCTSStats() = default;
    MCTSStats(double eval)
        : evaluation(eval)
        , backprop_reward(0)
        , visits(0)
    {
    }
    double average_reward() const
    {
        return visits ? backprop_reward / visits : std::numeric_limits<double>::min();
    }
};

// Implements the MCTS Base class. This class is designed to be inherited from, but should never
// be instantiated independently. All algorithms that are designed to be used should be marked
// final. To use MCTS, use the template found below this one.
template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool use_mcts_expand>
class MCTSBase {
public:
    inline const static std::string opts_str = std::string(using_iters ? "iters" : "cpu_time") + "_" + std::string(using_dag ? "dag" : "tree") + "_" + std::string(randomize_ties ? "rng_ties" : "no_rng_ties");
    inline const static std::string alg_str = "MCTS";
    inline const static std::string str_id = opts_str + "_" + alg_str;

    // Used to store all options of the algorithm that can vary per iteration. Every call to move
    // will overwrite the previous Settings object.
    struct Settings {
        uint rollout_depth;
        [[no_unique_address]] typename std::conditional<using_iters, uint, std::monostate>::type iters;
        [[no_unique_address]] typename std::conditional<using_iters, std::monostate, double>::type cpu_time;
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

    static inline std::mt19937 rng;
    std::shared_ptr<Node> current_node_ptr;
    typename std::conditional<using_dag, std::unordered_map<Node, std::shared_ptr<Node>>, std::monostate>::type transposition_table;
    Settings settings = Settings();

    template <typename... Args>
    MCTSBase(Args... args)
        : current_node_ptr(std::make_shared<Node>(args...))
    {
    }

    // Used to choose the next node to progress to after all rollouts have been completed.
    virtual std::shared_ptr<Node> choose(std::shared_ptr<Node> node_ptr)
    {
        if constexpr (randomize_ties) {
            // Doing this is fine as algorithms should not depend on the order of any given
            // set of child nodes. Ensures that ties are broken randomly.
            std::shuffle(std::begin(node_ptr->children), std::end(node_ptr->children), rng);
        }

        return *std::max_element(
            node_ptr->children.begin(),
            node_ptr->children.end(),
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
    virtual void backpropagate(std::vector<std::shared_ptr<Node>> path, const double reward)
    {
        for (std::shared_ptr<Node> node_ptr : path) {
            node_ptr->stats.visits++;
            node_ptr->stats.backprop_reward += reward;
        }
    }

    // Creates the children of a leaf node, and, if using_dag is enabled, checks the transposition
    // table and replaces the appropriate child nodes if necessary.
    //
    // Note on use_mcts_expand: This is done to tell the compiler that no code besides the exception
    // should be generated in the case that use_mcts_expand is false. This lets subclasses of this
    // class override expand without being bound to the fact that this version of expand assumes
    // that constructing Node::Stats requires no additional arguments. This is not true for all
    // algorithms, and for algorithms need arguments passed to create children, this presents a
    // problem because inheriting from this class will make the program fail to compile. See the
    // implementation of expand in rave.h for an example of an algorithm where this is useful.
    virtual void expand(std::shared_ptr<Node> node_ptr)
    {
        if constexpr (use_mcts_expand) {
            node_ptr->create_children();
            if constexpr (using_dag) {
                for (uint i = 0; i < node_ptr->children.size(); i++) {
                    if (transposition_table.count(*(node_ptr->children[i]))) {
                        node_ptr->children[i] = transposition_table[*(node_ptr->children[i])];
                    } else {
                        transposition_table[*(node_ptr->children[i])] = node_ptr->children[i];
                    }
                }
            }
        } else {
            std::logic_error("Never call base version of expand when use_mcts_expand is false!");
        }
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
        for (uint i = 0; i < settings.rollout_depth; i++) {
            if (node.is_terminal())
                break;

            node = node.default_policy();
        }

        return node.stats.evaluation;
    }

    // Traverses already expanded nodes in the tree/dag based on the tree_policy_metric. In the case
    // of MCTS, this is UCT, but could differ for other algorithms.
    virtual std::vector<std::shared_ptr<Node>> select(std::shared_ptr<Node> node_ptr)
    {
        std::vector<std::shared_ptr<Node>> path { node_ptr }; // NOTE: slight difference here, check to make sure it works

        for (;;) {
            if (!node_ptr->is_expanded() || node_ptr->is_terminal())
                return path;

            for (std::shared_ptr<Node> child : node_ptr->children) {
                if (!child->is_expanded()) {
                    path.push_back(child);
                    return path;
                }
            }

            node_ptr = *std::max_element(
                node_ptr->children.begin(),
                node_ptr->children.end(),
                [this](const std::shared_ptr<Node> left, const std::shared_ptr<Node> right) {
                    return tree_policy_metric(left) > tree_policy_metric(right);
                });
        }
    }

    virtual double tree_policy_metric(const std::shared_ptr<Node> node_ptr)
    {
        return uct(node_ptr);
    }

    virtual double uct(const std::shared_ptr<Node> node_ptr)
    {
        double avg_reward = node_ptr->stats.average_reward();
        double log_N = std::log(current_node_ptr->stats.visits);
        return avg_reward + settings.exploration_weight * sqrt(log_N / node_ptr->stats.visits);
    }
};

// The purpose of this class is to create a completely concrete class which the compiler will then
// hopefully devirtualize and optimize better. This may be unnecessary - but makes intent clear and
// keeps me from worrying about the compiler not doing the right thing :-).
template <class Node, bool using_iters, bool using_dag, bool randomized_ties>
class MCTS final : public MCTSBase<Node, using_iters, using_dag, randomized_ties, true> {
    using MCTSBase<Node, using_iters, using_dag, randomized_ties, true>::MCTSBase;
};
}
