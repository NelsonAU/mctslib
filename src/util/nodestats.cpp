
class NodeStats {
public:
	double evaluation;
	double backprop_value;
	int visits;

	NodeStats(double _evaluation) {
		visits = 1;
		evaluation = _evaluation;
		backprop_value = evaluation;
	}

	double avg_reward () const {
		if (!visits) return -std::numeric_limits<double>::max();
		return backprop_value/visits;
	}
};
