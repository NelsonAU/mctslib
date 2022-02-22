#include "util/empty.h"
#include <limits>
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
        : visits(0)
        , evaluation(eval)
        , backprop_value(0)
    {
    }

    double avg_value() const
    {
        
        return visits ? backprop_value / visits : std::numeric_limits<double>::min();
    }
};

}
