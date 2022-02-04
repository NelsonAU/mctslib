#include <cmath>
namespace mctslib {
auto mcts_uct = []<class Alg>(Alg& alg, const typename Alg::NodeStats& stats) -> double {
    double avg_value = stats.avg_value();
    double log_N = std::log(alg.body.current_node->stats.visits);
    return avg_value + alg.body.settings.exploration_weight * sqrt(log_N/stats.visits);
};
}

