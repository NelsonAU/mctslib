
#pragma once

class NoAction {};

template<typename Action = uint>
struct MCTSStats {
	int visits;
	double evaluation; //TODO: make names more clear
	double backprop_value;
	Action action;


	MCTSStats() = default;
	MCTSStats(double evaluation, Action action) : 
		visits(1), evaluation(evaluation), backprop_value(evaluation), action(action) {}

	double avg_value () const {
		if (!visits) return -std::numeric_limits<double>::max();
		return backprop_value/visits;
	}

	void backprop (double value) {
		visits++;
		backprop_value += value;
	}
};

struct RAVEStats {
//TODO
};

struct GRAVEStats {
//TODO
};
