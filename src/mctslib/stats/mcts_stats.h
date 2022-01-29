#include "mctslib/util/no_action.h"

namespace mctslib {

template<class Action = NoAction>
struct MCTSStats {
    uint visits;
    double evaluation;
    double backprop_value;
    Action action;

    MCTSStats(double eval) : visits(1), evaluation(eval), backprop_value(eval) {}

    double avg_value() const {
        return backprop_value/visits;
    }
};

}
