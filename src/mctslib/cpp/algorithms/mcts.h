#include <algorithm>
#include <chrono>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace mctslib {

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

template <class Node, bool using_iters, bool using_dag, bool randomize_ties>
class MCTSBase {
public:
    inline const static std::string opts_str = std::string(using_iters ? "iters" : "cpu_time") + "_" + std::string(using_dag ? "dag" : "tree") + "_" + std::string(randomize_ties ? "rng_ties" : "no_rng_ties");
    inline const static std::string alg_str = "MCTS";
    inline const static std::string str_id = opts_str + "_" + alg_str;

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

    virtual std::shared_ptr<Node> choose(std::shared_ptr<Node> node_ptr)
    {
        if constexpr (randomize_ties) {
            std::shuffle(std::begin(node_ptr->children), std::end(node_ptr->children), rng);
        }

        return *std::max_element(
            node_ptr->children.begin(),
            node_ptr->children.end(),
            [](const std::shared_ptr<Node> left, const std::shared_ptr<Node> right) {
                return left->stats.average_reward() < right->stats.average_reward();
            });
    }

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

    virtual void backpropagate(std::vector<std::shared_ptr<Node>> path, const double reward)
    {
        for (std::shared_ptr<Node> node_ptr : path) {
            node_ptr->stats.visits++;
            node_ptr->stats.backprop_reward += reward;
        }
    }

    virtual void expand(std::shared_ptr<Node> node_ptr)
    {
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

template <class Node, bool using_iters, bool using_dag, bool randomized_ties>
class MCTS final : public MCTSBase<Node, using_iters, using_dag, randomized_ties> {
    using MCTSBase<Node, using_iters, using_dag, randomized_ties>::MCTSBase;
};
}
