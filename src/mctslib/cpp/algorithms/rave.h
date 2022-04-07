#include "algorithms/mcts.h"
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace mctslib {
struct RAVEStats : public MCTSStats {
    uint action_id;
    std::vector<MCTSStats> amaf_stats;

    explicit RAVEStats() = default;
    RAVEStats(double reward, uint action_id, uint action_space_size)
        : MCTSStats(reward)
        , action_id(action_id)
        , amaf_stats(action_space_size, MCTSStats())
    {
    }

    template <typename... Args>
    RAVEStats(Args... args)
    {
        throw std::invalid_argument("Invalid construction of RAVEStats!");
    }

    double amaf_average_reward(uint action)
    {
        return amaf_stats.at(action).average_reward();
    }
};

template <class Node, bool using_iters, bool using_dag, bool randomize_ties>
class RAVEBase : public MCTSBase<Node, using_iters, using_dag, randomize_ties> {
public:
    inline const static std::string alg_str = "RAVE";
    inline const static std::string str_id = MCTSBase<Node, using_iters, using_dag, randomize_ties>::opts_str + "_" + alg_str;

    const uint equivalence_param;
    const uint action_space_size;

    template <typename... Args>
    RAVEBase(uint action_space_size, uint equivalence_param, Args... args)
        : MCTSBase<Node, using_iters, using_dag, randomize_ties>(args..., action_space_size)
        , equivalence_param(equivalence_param)
        , action_space_size(action_space_size)
    {
    }

    double tree_policy_metric(const std::shared_ptr<Node> node_ptr) override
    {
        auto current_node_stats = this->current_node_ptr->stats;
        double log_N = std::log(current_node_stats.visits);
        double beta = sqrt(equivalence_param / (3 * current_node_stats.visits + equivalence_param));
        double avg_amaf_reward = current_node_stats.amaf_average_reward(node_ptr->stats.action_id);
        double avg_reward = node_ptr->stats.average_reward();

        return (1 - beta) * avg_reward + beta * avg_amaf_reward + this->settings.exploration_weight * log_N;
    }

    void backpropagate(std::vector<std::shared_ptr<Node>> path, const double reward) override
    {
        std::vector<bool> seen_action_ids(action_space_size, false);

        for (auto it = path.rbegin(); it != path.rend(); it++) {
            std::shared_ptr<Node> node_ptr = *it;

            seen_action_ids.at(node_ptr->stats.action_id) = true;

            node_ptr->stats.visits++;
            node_ptr->stats.backprop_reward += reward;

            for (uint action_id = 0; action_id < action_space_size; action_id++) {
                if (seen_action_ids.at(action_id)) {
                    node_ptr->stats.amaf_stats.at(action_id).visits++;
                    node_ptr->stats.amaf_stats.at(action_id).backprop_reward += reward;
                }
            }
        }
    }

    void expand(std::shared_ptr<Node> node_ptr) override
    {
        node_ptr->create_children(action_space_size);

        if constexpr (using_dag) {

            for (uint i = 0; i < node_ptr->children.size(); i++) {
                if (this->transposition_table.count(*(node_ptr->children[i]))) {
                    node_ptr->children[i] = this->transposition_table[*(node_ptr->children[i])];
                } else {
                    this->transposition_table[*(node_ptr->children[i])] = node_ptr->children[i];
                }
            }
        }
    }
};

template <class Node, bool using_iters, bool using_dag, bool randomized_ties>
class RAVE final : public RAVEBase<Node, using_iters, using_dag, randomized_ties> {
    using RAVEBase<Node, using_iters, using_dag, randomized_ties>::RAVEBase;
};

}
