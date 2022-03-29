#include <algorithm>
#include <iterator>
#include <type_traits>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <variant>
#include <vector>

namespace mctslib {

template<class Alg>
class PyAlg {
    Alg alg;

    template <typename ...Args>
    PyAlg (Args ...args) : alg(args...) {}

    template <typename ...Args>
    pybind11::object move (Args ...args) {
        return alg.move(Alg::Settings(args...)).object;
    }

};

class MCTSStats {
    double evaluation;
    double backprop_reward;
    uint visits;

    explicit MCTSStats() = default;
    MCTSStats(double eval) : visits(0), evaluation(eval), backprop_reward(0) {}
};

template<class Node, bool using_iters, bool using_dag, bool randomized_ties>
class MCTSBase {
    struct Settings {
        int rollout_depth;
        double exploration_weight;
        [[no_unique_address]] std::conditional<using_iters, int, std::monostate> iters;
        [[no_unique_address]] std::conditional<!using_iters, double, std::monostate> cpu_time;
    };


    std::shared_ptr<Node> current_node_ptr;
    std::conditional<using_dag, std::unordered_map<Node, std::shared_ptr<Node>>, std::monostate>::type transposition_table;
    Settings settings;
    

    template <typename ...Args>
    MCTSBase(Args ...args) : current_node_ptr(std::make_shared(args...)) {}

    virtual Node move(const Settings new_settings) {
        settings = new_settings;

        if constexpr (using_iters) {
            for (int i = 0; i < settings.iters; i++) rollout(current_node_ptr);
        } else {
            auto start = std::chrono::high_resolution_clock::now();

            for (;;) {
                rollout(current_node_ptr);

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end - start;

                if (diff.count() > settings.cpu_time) break;

            }
        }

        choose();
        return *current_node_ptr;
    }

    virtual std::shared_ptr<Node> choose(std::shared_ptr<Node> node_ptr) requires(!randomized_ties) {
        return *std::max_element(
                node_ptr->children.begin(),
                node_ptr->children.end(),
                [](const std::shared_ptr<Node> left, const shared_ptr<Node> right) {
                    return left->stats.avg_value() < right->stats.avg_value();
                });
    }

    virtual void backpropagate(std::vector<std::shared_ptr<Node>> path, const double reward) {
        for (std::shared_ptr<Node> node_ptr : path) {
            node_ptr->stats.visits++;
            node_ptr->stats.backprop_value += reward;
        }
    }


    virtual void expand(std::shared_ptr<Node> node_ptr) {
        node_ptr->create_children();

        if constexpr (using_dag) {
            for (int i = 0; i < node_ptr->children.size(); i++) {
                if (transposition_table.count(*(node_ptr->children[i]))) {
                    node_ptr->children[i] = transposition_table[*(node_ptr->children[i])];
                } else {
                    transposition_table[*(node_ptr->children[i])] = node_ptr->children[i];
                }
            }
        }
    }

    virtual void rollout(std::shared_ptr<Node> node_ptr) {
        std::vector<std::shared_ptr<Node>> path = select(node_ptr);
        std::shared_ptr<Node> leaf = path.back();
        expand(leaf);
        double reward = simulate(leaf);
        backpropagate(path, reward);
    }

    virtual std::vector<std::shared_ptr<Node>> select(std::shared_ptr<Node> node_ptr) {
        std::vector<std::shared_ptr<Node>> path { node_ptr }; // NOTE: slight difference here, check to make sure it works
        
        for (;;) {
            if (!node_ptr->is_expanded() || node_ptr->is_terminal()) return path;

            for (std::shared_ptr<Node> child : node_ptr->children) {
                if (!child->is_expanded()) {
                    path.push_back(child);
                    return path;
                }
            }

            node_ptr = *std::max_element(
                    node_ptr->children.begin(),
                    node_ptr->children.end(),
                    [](const std::shared_ptr<Node> left, const std::shared_ptr<Node> right) {
                        return tree_policy_metric(left->stats) > tree_policy_metric(right->stats);
                    });
        }
    }

    virtual double tree_policy_metric(std::shared_ptr<Node> node_ptr) {
        return uct(node_ptr);
    }

    virtual double uct(std::shared_ptr<Node> node_ptr) {
        double avg_reward = node_ptr->stats.average_reward();
        double log_N = std::log(current_node_ptr->stats.visits);
        return avg_reward + settings.exploration_weight * sqrt(log_N/node_ptr->stats.visits);
    }

};

template<class Node, bool using_iters, bool using_dag, bool randomized_ties>
class MCTS final : public MCTSBase<Node, using_iters, using_dag, randomized_ties> {};


}


