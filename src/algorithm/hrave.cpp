#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <limits>
#include <ctime>


#pragma once



template<
	template<class> class NodeTemplate,
	template<class> class NodeStatsTemplate,
	typename Action,
	template <class, class> class MapTemplate = std::map
>
class HRAVE : public MCTS<NodeTemplate, NodeStatsTemplate, Action, MapTemplate> {
public:
	using NodeStats = NodeStatsTemplate<Action>;
	using Node = NodeTemplate<NodeStats>;

	uint equiv_param; //referred to as k in the literature

	/* This contains MCTSStats rather than the template type paramater because */
	/* we only need the things that MCTS stores, because the action is the key */
	/* into the container that stores the amaf stats. In some way this makes sense, */
	/* the AMAF values are not nodes, but something else. Possibly there should be */
	/* another template paramater for the AMAF stats, but that seems overkill */
	/* for now. */
	std::vector<MCTSStats<>> global_amafs; //TODO: implement for sparse action spaces

	HRAVE(Node* root, int k, size_t action_space_size) 
		: MCTS<NodeTemplate, NodeStatsTemplate, Action, MapTemplate>(root), equiv_param(k) {
		global_amafs.reserve(action_space_size);
		for (uint i = 0; i < action_space_size; i++) global_amafs[i] = MCTSStats();
	}

	double uct(const NodeStats& stats) const {
		/* TODO: does the compiler know that log_N and beta can be computed once per rollout? */
		/* current set up makes me have to override fewer methods, but could be slower, will */
		/* need to test */
		double log_N = std::log(this->current_node->stats.visits);
		double beta = sqrt(equiv_param/(3*this->current_node->stats.visits + equiv_param));
		double avg_amaf_value = global_amafs.at(stats.action).avg_value();
		double avg_value = stats.avg_value();
		
		return (1 - beta) * avg_value + beta * avg_amaf_value + 
			this->settings.exploration_weight * log_N;
	}

	void backpropagate(const std::vector<std::reference_wrapper<Node>> path, const double value) {

		for (Node& node : path) {
			node.stats.backprop(value);
			global_amafs[node.stats.action].backprop(value);
		}
	}
};
