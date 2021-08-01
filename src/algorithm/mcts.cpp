#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <limits>
#include <ctime>

#pragma once

struct MCTSSettings {
	double exploration_weight;
	int rollout_depth;
	int iters;
	double cpu_time;
	bool invert_reward;
};

template<typename Node>
class MCTS {
public:
	std::map<Node, std::vector<Node>> children_map;
	MCTSSettings settings;
	Node current_node;

	MCTS(const Node root) : current_node(root) {
		//small constructor til we have alternative selection functions etc
		//parameters previously supplied to the constructor will be given to play instead
		//this makes it easier to have rollout depth scheduling, things like saving PNG of each 
		//gamestate in the ALE case, etc
	}

	const Node move(const MCTSSettings& _settings) {
		settings = _settings;
		if (current_node.is_terminal()) {
			return current_node;
		}
		
		if (settings.cpu_time) {
			clock_t start = std::clock();
			while (true) {
				rollout(current_node);
				clock_t end = std::clock();
				double elapsed_seconds = ((float)end - start)/CLOCKS_PER_SEC;
				if (elapsed_seconds > settings.cpu_time) break;
			}
		} else {
			for (int i = 0; i < settings.iters; i++) {
				rollout(current_node);
			}
		}
		current_node = choose(current_node);
		return current_node;
	}

	const Node choose(const Node& node)  {
		if (!children_map.count(node)) {
			return node.random_child();
		}

		auto nodes = children_map[node];

		double max = -std::numeric_limits<double>::max();
		int idx = 0;
		int max_idx = 0;

		for (auto node : nodes) {
			double avg_reward = node.stats->avg_reward();
			double node_score = settings.invert_reward ? -avg_reward : avg_reward;
			if (node_score > max) {
				max = node_score;
				max_idx = idx;
			}
			idx++;
		}
		return nodes[max_idx];
	}

	const void rollout(const Node& node) {
		auto path = select(node);
		auto leaf = path.back();
		expand(leaf);
		int reward = simulate(leaf);
		backpropagate(path, reward);
	}

	const std::vector<Node> select(const Node& node) {
		std::vector<Node> path;
		Node local_node = node;
		while (true) {
			path.push_back(local_node);
			if (!children_map.count(local_node) || local_node.is_terminal())
				return path;

			for (auto child : children_map[local_node]) {
				if (!children_map.count(child)) {
					path.push_back(child);
					return path;
				}
			}

			local_node = uct_select(local_node);
		}
	}

	Node uct_select(const Node& node) {
		std::vector<double> ucts;

		auto children = children_map[node];
		int max_idx = 0;
		double top_score = -std::numeric_limits<double>::max();
		double score;

		for (int i = 0; i < children.size(); i++) {
			score = uct(*children[i].stats);
			if (score > top_score) {
				top_score = score;
				max_idx = i;
			}
		}

		return children[max_idx];
	}

	double uct(const NodeStats& stats) {
		double avg_reward = stats.avg_reward();

		return avg_reward + settings.exploration_weight * sqrt(std::log(stats.visits)/stats.visits);
	}

	void expand(const Node& node) {
		if (children_map.count(node)) return;

		children_map[node] = node.find_children();
	}


	int simulate(const Node& start) {
		Node node = start;
		for (int i = 0; i < settings.rollout_depth; i++) {
			if (node.is_terminal()) 
				return node.stats->evaluation;

			node = node.random_child();
		}
		return node.stats->evaluation;
	}

	void backpropagate(const std::vector<Node>& path, const double value) {
		for (Node node : path) {
			node.stats->visits++; //TODO: this is probably not the most efficient way to write this
			node.stats->backprop_value += value;
		}
	}

};

