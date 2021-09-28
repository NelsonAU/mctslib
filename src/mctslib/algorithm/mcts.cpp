#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <limits>
#include <ctime>

#include "util/nodestats.cpp"

#pragma once


struct MCTSSettings {
	double exploration_weight;
	int rollout_depth;
	uint iters;
	double cpu_time;
	bool invert_reward;
};


template<
	template<class> class NodeTemplate, 
	template<class> class NodeStatsTemplate, 
	typename Action,
	template <class, class> typename MapTemplate = std::map
>
class MCTS {
public:
	using NodeStats = NodeStatsTemplate<Action>;
	using Node = NodeTemplate<NodeStats>;

	MapTemplate<Node, Node*> node_ptr_map; //TODO make this unordered_map?
	MCTSSettings settings;
	Node* current_node;

	MCTS(Node* root) : current_node(root) {
		/* small constructor til we have alternative selection functions etc */
		/* parameters previously supplied to the constructor will be given to play instead */
		/* this makes it easier to have rollout depth scheduling, things like saving PNG of each */ 
		/* gamestate in the ALE case, etc */
	}

	~MCTS() {
		for (auto [_, node_ptr] : node_ptr_map) {
			delete node_ptr;
		}
	}

	Node move(const MCTSSettings& _settings) {
		settings = _settings;
		if (current_node->been_expanded && !current_node->children.size()) {
			return *current_node;
		}
		
		if (settings.cpu_time) {
			clock_t start = std::clock();
			while (true) {
				rollout(*current_node);
				clock_t end = std::clock();
				double elapsed_seconds = ((float)end - start)/CLOCKS_PER_SEC;
				if (elapsed_seconds > settings.cpu_time) break;
			}
		} else {
			for (uint i = 0; i < settings.iters; i++) {
				rollout(*current_node);
			}
		}
		current_node = choose(*current_node);
		return *current_node;
	}

	Node* choose(const Node& node) {
		if (!node.been_expanded) {
			throw std::invalid_argument("Node must be expanded before choose is called");
		}


		double max = -std::numeric_limits<double>::max();
		int idx = 0;
		int max_idx = 0;

		for (Node* child : node.children) {
			double avg_value = child->stats.avg_value();
			double node_score = settings.invert_reward ? -avg_value : avg_value;
			if (node_score > max) {
				max = node_score;
				max_idx = idx;
			}
			idx++;
		}
		return node.children[max_idx];
	}

	void rollout(Node& node) {
		auto path = select(node);
		auto leaf = path.back();
		expand(leaf);
		double reward = simulate(leaf);
		backpropagate(path, reward);
	}

	const std::vector<std::reference_wrapper<Node>> select(Node& initial_node) {
		std::vector<std::reference_wrapper<Node>> path {std::ref(initial_node)};
		Node node = initial_node;
		while (true) {
			if (!node.been_expanded || !node.children.size())
				return path;

			for (Node* child : node.children) {
				if (!child->been_expanded) {
					path.push_back(std::ref(*child));
					return path;
				}
			}
			//confused about how to get this to not happen...
			Node& temp = uct_select(node);
			path.push_back(std::ref(temp));
			node = temp;
		}
	}

	Node& uct_select(const Node& node) {
		std::vector<double> ucts;
		ucts.reserve(node.children.size());

		uint max_idx = 0;
		double top_score = -std::numeric_limits<double>::max();
		double score;

		for (uint i = 0; i < node.children.size(); i++) {
			score = uct(node.children[i]->stats);
			if (score > top_score) {
				top_score = score;
				max_idx = i;
			}
		}

		return *node.children[max_idx];
	}

	double uct(const NodeStats& stats) const {
		double avg_value = stats.avg_value();
		double log_N = std::log(current_node->stats.visits);
		return avg_value + settings.exploration_weight * sqrt(log_N/stats.visits);
	}

	void expand(Node& node) {
		if (node.been_expanded) return;

		node.create_children();
		node.been_expanded = true;
		for (size_t i = 0; i < node.children.size(); i++) {
			if (node_ptr_map.count(*node.children[i])) {
				/* TODO: tried to make this work with smart pointers to avoid explicit delete, */
				/* 		 that version seeemd more complicated though. Something to revisit */

				//if we've seen this node before we should get the canonical version
				//and free the newly created one

				Node* duplicate_node_ptr = node.children[i];
				node.children[i] = node_ptr_map[*node.children[i]];
				delete duplicate_node_ptr;
			} else {
				//otherwise, we should insert it into the map
				node_ptr_map[*node.children[i]] = node.children[i];
			}
		}
	}


	double simulate(const Node& start) const {
		Node node = start;
		for (int i = 0; i < settings.rollout_depth; i++) {
			if (!node.children.size()) 
				return node.stats.evaluation;

			node = node.random_child();
		}
		return node.stats.evaluation;
	}

	void backpropagate(const std::vector<std::reference_wrapper<Node>> path, const double value) {
		for (Node& node : path) {
			node.stats.visits++;
			node.stats.backprop_value += value;
		}
	}


};

