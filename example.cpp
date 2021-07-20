#include <pybind11/pybind11.h>
#include <vector>
#include <map>
#include <iostream>
#include <limits>
#include <ctime>

namespace py = pybind11;


class PyMCTS {
private:
	double exploration_weight;
	double cpu_time;
	int rollout_depth;
	int max_turns;

	std::map<py::object, py::list> children;

public:
	PyMCTS(double _exploration_weight, double _cpu_time, int _rollout_depth, int _max_turns) {
		exploration_weight = _exploration_weight;
		cpu_time = _cpu_time;
		rollout_depth = _rollout_depth;
		max_turns = _max_turns;
	}

	void play(py::object& initial_node) {
		py::object node = initial_node;

		for (int turn_no = 0; turn_no < max_turns; turn_no++) {
			if (node.attr("is_terminal")().cast<bool>())
				return;
			std::cout << turn_no << std::endl;
			clock_t start = std::clock();
			clock_t end;
			double elapsed_seconds;

			while (true) {
				rollout(node);
				end = std::clock();
				elapsed_seconds = ((float)end - start)/CLOCKS_PER_SEC;
				std::cout << elapsed_seconds << std::endl;
				if (elapsed_seconds > cpu_time) break;
			}


			auto new_node = choose(node);
			node = new_node;

		}
	}

	py::object choose(py::object& node)  {
		if (!children.count(node)) return node.attr("random_child")();

		auto nodes = children[node];

		double max = -std::numeric_limits<double>::max();
		int idx = 0;
		int max_idx = 0;

		for (auto node : nodes) {
			double node_score = node.attr("avg_reward")().cast<double>();
			if (node_score > max) {
				max = node_score;
				max_idx = idx;
			}
			idx++;
		}
		return nodes[max_idx];
	}

	void rollout(py::object& node) {
		auto path = select(node);
		auto leaf = path.back();
		expand(leaf);
		int reward = simulate(leaf);
		backpropagate(path, reward);
	}

	std::vector<py::object> select(py::object& node) {
		std::vector<py::object> path;

		while (true) {
			path.push_back(node);
			if (!children.count(node) || node.attr("is_terminal")().cast<bool>())
				return path;

			for (auto handle : children[node]) {
				py::object child = py::reinterpret_borrow<py::object>(handle);
				if (!children.count(child)) {
					path.push_back(child);
					return path;
				}
			}

			node = uct_select(node);
		}
	}

	py::object uct_select(py::object node) {
		std::vector<double> ucts;

		auto local_children = children[node];
		int max_idx = 0;
		double top_score = -std::numeric_limits<double>::max();
		double score;

		for (int i = 0; i < local_children.size(); i++) {
			score = uct(local_children[i]);
			if (score > top_score) {
				top_score = score;
				max_idx = i;
			}
		}

		return local_children[max_idx];
	}

	double uct(py::object node) {
		double avg_reward = node.attr("avg_reward")().cast<double>();
		int visits = node.attr("visits").cast<int>();
		return avg_reward + exploration_weight * sqrt(std::log(visits)/visits);
	}

	void expand(py::object node) {
		if (children.count(node)) return;

		children[node] = node.attr("find_children")();
	}


	int simulate(py::object start) {
		py::object node = start;
		for (int i = 0; i < rollout_depth; i++) {
			if (node.attr("is_terminal")().cast<bool>()) 
				return node.attr("value").cast<int>();

			node = node.attr("random_child")();
		}
		return node.attr("value").cast<int>();
	}

	void backpropagate(std::vector<py::object>& path, double value) {
		for (py::object node : path) {
			node.attr("visits") = node.attr("visits").cast<int>() + 1;
			node.attr("backprop_value") = node.attr("backprop_value").cast<double>() + value;
		}
	}

};
