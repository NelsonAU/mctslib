#include "algorithms/mcts.h"
#include <cmath>
#include <memory>
#include <string>
#include <vector>

namespace mctslib {

struct HRAVEStats : MCTSStats {
    uint action_id;

    explicit HRAVEStats() = default;
    HRAVEStats(double reward, uint action_id)
        : MCTSStats(reward)
        , action_id(action_id)
    {
    }
};

// Implements History Rapid Action Value Estimation. Like GRAVE with the ref_threshold set to
// infinity.
template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool use_mcts_expand = true>
class HRAVEBase : public MCTSBase<Node, using_iters, using_dag, randomize_ties, use_mcts_expand> {
public:
    using MCTSBaseCls = MCTSBase<Node, using_iters, using_dag, randomize_ties, use_mcts_expand>;
    inline const static std::string alg_str = "HRAVE";
    inline const static std::string str_id = MCTSBaseCls::opts_str + "_" + alg_str;
    // Stores the all moves as first (AMAF) statistics for each action. The GRAVE analogy taken 
    // literally would have us store this on the root node, but in practice this lets us keep the in
    // memory size of the tree down by freeing nodes above the current_node_ptr.
    std::vector<MCTSStats> global_amafs;
    const uint equivalence_param;
    const uint action_space_size;

    template <typename... Args>
    HRAVEBase(uint action_space_size, uint equivalence_param, Args... args)
        : MCTSBaseCls(args...)
        , global_amafs(action_space_size, MCTSStats())
        , equivalence_param(equivalence_param)
        , action_space_size(action_space_size)
    {
    }

    // Changes the tree policy to use AMAF stats.
    // TODO: add a setting for how we combine the AMAF estimation and UCT. Multiple different
    // ways to do this in the literature.
    double tree_policy_metric(const std::shared_ptr<Node> node_ptr) override
    {
        double log_N = std::log(this->current_node_ptr->stats.visits);
        double beta = sqrt(equivalence_param / (3 * this->current_node_ptr->stats.visits + equivalence_param));
        double avg_amaf_reward = global_amafs.at(node_ptr->stats.action_id).average_reward();
        double avg_reward = node_ptr->stats.average_reward();

        return (1 - beta) * avg_reward + beta * avg_amaf_reward + this->settings.exploration_weight * log_N;
    }

    // Changes the backpropagation method to update the global AMAF stats as well as the stats
    // of the nodes along the path taken by the tree policy.
    void backpropagate(std::vector<std::shared_ptr<Node>> path, const double reward) override
    {
        std::vector<bool> seen_action_ids(action_space_size, false);

        for (std::shared_ptr<Node> node_ptr : path) {
            seen_action_ids.at(node_ptr->stats.action_id) = true;
            node_ptr->stats.visits++;
            node_ptr->stats.backprop_reward += reward;
        }

        for (uint i = 0; i < action_space_size; i++) {
            if (seen_action_ids.at(i)) {
                global_amafs.at(i).visits++;
                global_amafs.at(i).backprop_reward += reward;
            }
        }
    }
};

template <class Node, bool using_iters, bool using_dag, bool randomized_ties>
class HRAVE final : public HRAVEBase<Node, using_iters, using_dag, randomized_ties> {
    using HRAVEBase<Node, using_iters, using_dag, randomized_ties>::HRAVEBase;
};

}
