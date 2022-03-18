#include "stats/mcts_stats.h"
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

    RAVEStats(double eval, uint action, uint action_space_size)
        : visits(0)
        , evaluation(eval)
        , action(action)
        , backprop_value(0)
        , amaf_stats(action_space_size)
    {
    }

    double avg_value() const
    {
        return visits ? backprop_value / visits : std::numeric_limits<double>::min();
    }
};
}
