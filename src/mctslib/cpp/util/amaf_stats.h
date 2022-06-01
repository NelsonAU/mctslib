#include <limits>

namespace mctslib {
struct AMAFStats {
    double evaluation;
    double backprop_reward;
    int visits;

    double average_reward() const
    {
        return visits ? backprop_reward / visits : std::numeric_limits<double>::min();
    }
};
}
