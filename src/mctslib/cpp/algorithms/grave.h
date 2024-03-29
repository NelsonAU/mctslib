#include "algorithms/mcts.h"
#include "algorithms/rave.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <deque>
#include <memory>
#include <string>
#include <vector>

#ifdef MCTSLIB_USING_PYBIND11
#include <pybind11/pybind11.h>
#endif

namespace mctslib {
template <class Node, bool using_dag, bool randomize_ties, bool constant_action_space>
class GRAVEBase : public MCTSBase<Node, using_dag, randomize_ties, constant_action_space> {
public:
    using MCTSBaseCls = MCTSBase<Node, using_dag, randomize_ties, constant_action_space>;
#ifdef MCTSLIB_USING_PYBIND11
    inline const static std::string alg_str = "GRAVE";
    inline const static std::string str_id = MCTSBaseCls::opts_str + "_" + alg_str;
#endif

    const int equivalence_param;
    const int ref_threshold;
    std::deque<std::shared_ptr<Node>> ref_nodes; // newest nodes at the front

    template <typename... Args>
    GRAVEBase(double backprop_decay, int max_action_value, int equivalence_param, int ref_threshold, Args... args)
        : MCTSBaseCls(backprop_decay, max_action_value, args...)
        , equivalence_param(equivalence_param)
        , ref_threshold(ref_threshold)
        , ref_nodes { this->current_node_ptr }
    {
    }

    // Gets the node lowest in the tree which has the requisite number of visits to be used as the
    // ref_node. In the case that there are none that meet the threshold, the root node is used.
    virtual std::shared_ptr<Node> get_ref_node()
    {
        // possible to make this more efficient by starting at the back
        for (size_t i = 0; i < ref_nodes.size(); i++) {
            if (ref_nodes.at(i)->stats.visits >= ref_threshold)
                return ref_nodes.at(i);
        }

        return ref_nodes.back();
    }

    // Trims the deque holding the ref_nodes so we can free nodes no longer required for their
    // AMAF stats.
    virtual void trim_ref_nodes()
    {
        for (size_t i = 0; i < ref_nodes.size(); i++) {
            if (ref_nodes.at(i)->stats.visits >= ref_threshold) {
                ref_nodes.resize(i + 1);
            }
        }
    }

    // Changes tree policy metric to use the ref_node's AMAF stats. Otherwise should be similar to
    // RAVE, HRAVE tree_policy_metric
    double tree_policy_metric(const std::shared_ptr<Node> node_ptr) override
    {
        auto ref_node_stats = get_ref_node()->stats;
        double log_N = std::log(ref_node_stats.visits);
        double beta = sqrt(equivalence_param / (3 * ref_node_stats.visits + equivalence_param));
        double avg_amaf_reward = ref_node_stats.amaf_average_reward(node_ptr->stats.action_id);
        double avg_reward = node_ptr->stats.average_reward();

        return (1 - beta) * avg_reward + beta * avg_amaf_reward + this->exploration_weight * log_N;
    }

    void backpropagate(std::vector<std::shared_ptr<Node>> path, const double reward) override
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

        // GRAVE requires us to potentially backpropagate above the current node in the game tree
        for (std::shared_ptr<Node> node_ptr : ref_nodes) {
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
        // Possible that this is the wrong way to do this - but this is immediately after ref
        // nodes have new visit counts, so it seems the most appropriate
        trim_ref_nodes();
    }

    // Selects node and updates ref_nodes
    virtual std::shared_ptr<Node> choose_node(std::shared_ptr<Node> chosen_node) override
    {
        MCTSBaseCls::choose_node(chosen_node);
        ref_nodes.push_front(this->current_node_ptr);
        return this->current_node_ptr;
    }

#ifdef MCTSLIB_USING_PYBIND11
    pybind11::dict get_global_stats() {
        return MCTSBaseCls::get_global_stats();
    }
#endif

};

template <class Node, bool using_dag, bool randomized_ties, bool constant_action_space>
class GRAVE final : public GRAVEBase<Node, using_dag, randomized_ties, constant_action_space> {
    using GRAVEBase<Node, using_dag, randomized_ties, constant_action_space>::GRAVEBase;
};
}
