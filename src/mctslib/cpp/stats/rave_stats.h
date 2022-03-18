#include "stats/stats.h"
#include <limits>
#include <vector>


namespace mctslib {
struct RAVEStats {
    double evaluation;
    double backprop_value;
    uint visits;
    uint action;
    std::vector<MCTSStats> amaf_stats;

    RAVEStats() = default;

    RAVEStats(double eval, uint action)
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
