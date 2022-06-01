#include "algorithms/mcts.h"
#include "util/amaf_stats.h"
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace mctslib {
struct RAVEStats : public MCTSStats {
    std::vector<AMAFStats> amaf_stats;

    RAVEStats(double eval, int action_id, int max_action_value)
        : MCTSStats(eval, action_id, max_action_value)
        , amaf_stats(max_action_value + 1, AMAFStats())
    {
    }

    static RAVEStats eval_only(double eval, int action_id)
    {
        return RAVEStats(eval, action_id, 0);
    }

    // gets the average reward of a given action according to the AMAF stats on this node
    double amaf_average_reward(int action)
    {
        return amaf_stats.at(action).average_reward();
    }
};

template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool constant_action_space>
class RAVEBase : public MCTSBase<Node, using_iters, using_dag, randomize_ties, constant_action_space> {
public:
    using MCTSBaseCls = MCTSBase<Node, using_iters, using_dag, randomize_ties, constant_action_space>;

    inline const static std::string alg_str = "RAVE";
    inline const static std::string str_id = MCTSBaseCls::opts_str + "_" + alg_str;

    const int equivalence_param;

    template <typename... Args>
    RAVEBase(double backprop_decay, int max_action_value, int equivalence_param, Args... args)
        : MCTSBaseCls(backprop_decay, max_action_value, args...)
        , equivalence_param(equivalence_param)
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

    void backpropagate(std::vector<std::shared_ptr<Node>> path, double reward) override
    {
        std::vector<bool> seen_action_ids(this->max_action_value + 1, false);
        double discounted_reward = reward;

        for (auto it = path.rbegin(); it != path.rend(); it++) {
            std::shared_ptr<Node> node_ptr = *it;

            seen_action_ids.at(node_ptr->stats.action_id) = true;

            node_ptr->stats.visits++;
            node_ptr->stats.backprop_reward += discounted_reward;
            discounted_reward *= this->backprop_decay;

            for (int action_id = 0; action_id <= this->max_action_value; action_id++) {
                if (seen_action_ids.at(action_id)) {
                    node_ptr->stats.amaf_stats.at(action_id).visits++;
                    node_ptr->stats.amaf_stats.at(action_id).backprop_reward += reward;
                }
            }
        }
    }
};

template <class Node, bool using_iters, bool using_dag, bool randomized_ties, bool constant_action_space>
class RAVE final : public RAVEBase<Node, using_iters, using_dag, randomized_ties, constant_action_space> {
    using RAVEBase<Node, using_iters, using_dag, randomized_ties, constant_action_space>::RAVEBase;
};

}
