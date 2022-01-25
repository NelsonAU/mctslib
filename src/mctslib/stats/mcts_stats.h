#include "util/no_action.h"

struct MCTSStats {
    uint visits;
    double evaluation;
    double backprop_value;
    NoAction action;

    MCTSStats(double eval) : visits(1), evaluation(eval), backprop_value(eval) {}

    double avg_value() const {
        return backprop_value/visits;
    }
};
