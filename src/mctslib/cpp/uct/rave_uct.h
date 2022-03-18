#include <cmath>
namespace mctslib {
inline auto rave_uct = []<class Alg>(Alg& alg, const typename Alg::NodeStats& stats) -> double {
    double log_N = std::log(alg.body.current_node->stats.visits);
    double beta = sqrt(alg.body.equivalence_param / (3 * alg.body.current_node->stats.visits + alg.body.equivalence_param));
    double avg_amaf_value = alg.body.current_node->stats.amaf_stats.at(stats.action).avg_value();
    double avg_value = stats.avg_value();

    return (1 - beta) * avg_value + beta * avg_amaf_value + alg.body.settings.exploration_weight * log_N;
};
}
