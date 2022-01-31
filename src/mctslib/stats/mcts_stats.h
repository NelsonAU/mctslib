#include "mctslib/util/empty.h"

namespace mctslib {

template<class Action = Empty>
struct MCTSStats {
    uint visits;
    double evaluation;
    double backprop_value;
    Action action;
    
    MCTSStats() = default;
    MCTSStats(double eval) : visits(1), evaluation(eval), backprop_value(eval) {}

    double avg_value() const {
        return backprop_value/visits;
    }
};

}
