#include <cmath>

auto mcts_uct = [](auto& alg, const auto& stats) -> double {
    double avg_value = stats.avg_value();
    double log_N = std::log(alg.body.current_node->stats.visits);
    return avg_value + alg.body.settings.exploration_weight * sqrt(log_N/stats.visits);
};

