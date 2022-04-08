#include "algorithms/mcts.h"
#include "algorithms/rave.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <deque>
#include <memory>
#include <string>
#include <vector>

namespace mctslib {
template <class Node, bool using_iters, bool using_dag, bool randomize_ties, bool use_mcts_expand = false>
class GRAVEBase : public MCTSBase<Node, using_iters, using_dag, randomize_ties, use_mcts_expand> {
public:
    using MCTSBaseCls = MCTSBase<Node, using_iters, using_dag, randomize_ties, use_mcts_expand>;

    inline const static std::string alg_str = "GRAVE";
    inline const static std::string str_id = MCTSBaseCls::opts_str + "_" + alg_str;

    const uint equivalence_param;
    const uint action_space_size;
    const uint ref_threshold;
    std::deque<std::shared_ptr<Node>> ref_nodes; // newest nodes at the front

    template <typename... Args>
    GRAVEBase(uint equivalence_param, uint action_space_size, uint ref_threshold, Args... args)
        : MCTSBaseCls(args..., action_space_size)
        , equivalence_param(equivalence_param)
        , action_space_size(action_space_size)
        , ref_threshold(ref_threshold)
        , ref_nodes { this->current_node_ptr }
    {
    }

    // Gets the node lowest in the tree which has the requisite number of visits to be used as the
    // ref_node. In the case that there are none that meet the threshold, the root node is used.
    virtual std::shared_ptr<Node> get_ref_node()
    {
        // possible to make this more efficient by starting at the back
        for (uint i = 0; i < ref_nodes.size(); i++) {
            if (ref_nodes.at(i)->stats.visits >= ref_threshold)
                return ref_nodes.at(i);
        }

        return ref_nodes.back();
    }

    // Trims the deque holding the ref_nodes so we can free nodes no longer required for their
    // AMAF stats.
    virtual void trim_ref_nodes()
    {
        for (uint i = 0; i < ref_nodes.size(); i++) {
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

        // GRAVE requires us to potentially backpropagate above the current node in the game tree
        for (std::shared_ptr<Node> node_ptr : ref_nodes) {
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
        // Possible that this is the wrong way to do this - but this is immediately after ref
        // nodes have new visit counts, so it seems the most appropriate
        trim_ref_nodes();
    }

    // expand should have the same implementation for both RAVE and GRAVE
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

    // Changes move to update ref_nodes. Otherwise should be the same as MCTS::move
    Node move(const typename MCTSBaseCls::Settings new_settings) override
    {
        this->settings = new_settings;

        if constexpr (using_iters) {
            for (uint i = 0; i < this->settings.iters; i++)
                this->rollout(this->current_node_ptr);
        } else {
            auto start = std::chrono::high_resolution_clock::now();

            for (;;) {
                this->rollout(this->current_node_ptr);

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end - start;

                if (diff.count() > this->settings.cpu_time)
                    break;
            }
        }

        this->current_node_ptr = this->choose(this->current_node_ptr);
        ref_nodes.push_front(this->current_node_ptr);
        return *(this->current_node_ptr);
    }
};

template <class Node, bool using_iters, bool using_dag, bool randomized_ties>
class GRAVE final : public GRAVEBase<Node, using_iters, using_dag, randomized_ties> {
    using GRAVEBase<Node, using_iters, using_dag, randomized_ties>::GRAVEBase;
};
}
