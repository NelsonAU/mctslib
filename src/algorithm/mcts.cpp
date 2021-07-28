#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <limits>
#include <ctime>


template<typename NodeClass>
class MCTS {
public:
	double exploration_weight;
	double cpu_time;
	int rollout_depth;
	int max_turns;

	std::map<NodeClass, std::vector<NodeClass>> children_map;

	MCTS(double _exploration_weight, double _cpu_time, int _rollout_depth, int _max_turns) {
		exploration_weight = _exploration_weight;
		cpu_time = _cpu_time;
		rollout_depth = _rollout_depth;
		max_turns = _max_turns;
	}

	NodeClass play(NodeClass& initial_node) {
		NodeClass node = initial_node;

		for (int turn_no = 0; turn_no < max_turns; turn_no++) {
			if (node.is_terminal()) {
				return node;
			}
			clock_t start = std::clock();
			clock_t end;
			double elapsed_seconds;
			int i = 0;
			while (true) {
				rollout(node);
				end = std::clock();
				elapsed_seconds = ((float)end - start)/CLOCKS_PER_SEC;
				if (elapsed_seconds > cpu_time) break;
			}


			auto new_node = choose(node);
			py::print(node.object, new_node.object, "new node chosen");
			node = new_node;

		}

		return node;
	}

	NodeClass choose(NodeClass node)  {
		py::print("Enter choose");
		if (!children_map.count(node)) {
			std::cout << "node not in map" << std::endl;
			return node.random_child();
		}

		auto nodes = children_map[node];

		double max = -std::numeric_limits<double>::max();
		int idx = 0;
		int max_idx = 0;

		for (auto node : nodes) {
			double node_score = node.stats->avg_reward();
			if (node_score > max) {
				max = node_score;
				max_idx = idx;
			}
			idx++;
		}
		return nodes[max_idx];
	}

	void rollout(const NodeClass node) {
		auto path = select(node);
		auto leaf = path.back();
		expand(leaf);
		int reward = simulate(leaf);
		backpropagate(path, reward);
	}

	std::vector<NodeClass> select(NodeClass node) {
		std::vector<NodeClass> path;
		NodeClass local_node = node;
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

	NodeClass uct_select(const NodeClass& node) {
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

	double uct(NodeStats& stats) {
		double avg_reward = stats.avg_reward();

		return avg_reward + exploration_weight * sqrt(std::log(stats.visits)/stats.visits);
	}

	void expand(NodeClass node) {
		if (children_map.count(node)) return;

		children_map[node] = node.find_children();
	}


	int simulate(NodeClass start) {
		NodeClass node = start;
		for (int i = 0; i < rollout_depth; i++) {
			if (node.is_terminal()) 
				return node.stats->evaluation;

			node = node.random_child();
		}
		return node.stats->evaluation;
	}

	void backpropagate(const std::vector<NodeClass>& path, const double value) {
		for (NodeClass node : path) {
			node.stats->visits++; //TODO: this is probably not the most efficient way to write this
			node.stats->backprop_value += value;
		}
	}

};

class PyMCTS : MCTS<PyNode> {
public:
	using MCTS<PyNode>::MCTS;
	py::object pyPlay(py::object object) {
		auto node = PyNode(object);
		return play(node).object;
	}
};
