#include "algorithms/mcts.h"
#include "util/amaf_stats.h"
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#ifdef MCTSLIB_USING_PYBIND11
#include <pybind11/pybind11.h>
#endif

namespace mctslib {

// Implements History Rapid Action Value Estimation. Like GRAVE with the ref_threshold set to
// infinity.
template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool constant_action_space>
class HRAVEBase : public MCTSBase<Node, using_iters, using_dag, randomize_ties, constant_action_space> {
public:
    using MCTSBaseCls = MCTSBase<Node, using_iters, using_dag, randomize_ties, constant_action_space>;
    inline const static std::string alg_str = "HRAVE";
    inline const static std::string str_id = MCTSBaseCls::opts_str + "_" + alg_str;
    // Stores the all moves as first (AMAF) statistics for each action. The GRAVE analogy taken
    // literally would have us store this on the root node, but in practice this lets us keep the in
    // memory size of the tree down by freeing nodes above the current_node_ptr.
    std::vector<AMAFStats> global_amafs;
    const int equivalence_param;

    template <typename... Args>
    HRAVEBase(double backprop_decay, int max_action_value, int equivalence_param, Args... args)
        : MCTSBaseCls(backprop_decay, max_action_value, args...)
        , global_amafs(max_action_value + 1, AMAFStats())
        , equivalence_param(equivalence_param)
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
        std::vector<bool> seen_action_ids(this->max_action_value + 1, false);
        double discounted_reward = reward;

        for (std::shared_ptr<Node> node_ptr : path) {
            seen_action_ids.at(node_ptr->stats.action_id) = true;
            node_ptr->stats.visits++;
            node_ptr->stats.backprop_reward += discounted_reward;

            discounted_reward *= this->backprop_decay;
        }

        for (int i = 0; i < this->max_action_value + 1; i++) {
            if (seen_action_ids.at(i)) {
                global_amafs.at(i).visits++;
                global_amafs.at(i).backprop_reward += reward;
            }
        }
    }

#ifdef MCTSLIB_USING_PYBIND11
    pybind11::dict get_global_stats() {
        return MCTSBaseCls::get_global_stats();
    }
#endif

};

template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool constant_action_space>
class HRAVE final : public HRAVEBase<Node, using_iters, using_dag, randomize_ties, constant_action_space> {
    using HRAVEBase<Node, using_iters, using_dag, randomize_ties, constant_action_space>::HRAVEBase;
};

}
