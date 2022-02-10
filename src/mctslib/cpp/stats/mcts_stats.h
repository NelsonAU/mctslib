#include "util/empty.h"
#include <type_traits>

namespace mctslib {

template <class T = Empty>
struct MCTSStats {
    static_assert(std::is_same_v<T, Empty>);
    uint visits;
    double evaluation;
    double backprop_value;
    Empty action;

    MCTSStats() = default;
    MCTSStats(double eval)
        : visits(1)
        , evaluation(eval)
        , backprop_value(eval)
    {
    }

    double avg_value() const
    {
        return backprop_value / visits;
    }
};

}
