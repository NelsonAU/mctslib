#include <limits>
#include <type_traits>

#pragma once
namespace mctslib {

template <typename Action>
struct MCTSStats {
    double evaluation;
    double backprop_value;
    uint visits;
    Action action;

    MCTSStats() = default;

    MCTSStats(double eval, Action action)
        : visits(0)
        , evaluation(eval)
        , action(action)
        , backprop_value(0)
    {
    }

    double avg_value() const
    {

        return visits ? backprop_value / visits : std::numeric_limits<double>::min();
    }
};

}
