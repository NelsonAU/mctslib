#include <algorithm>
#include <chrono>

inline auto mcts_cpu_move = []<class Alg>(Alg& alg, typename Alg::Settings settings) -> auto {
    alg.body.settings = settings;
    
    auto start = std::chrono::high_resolution_clock::now();

    for (;;) {
        alg.rollout(alg, *alg.body.current_node);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;

        if (diff.count() > settings.cpu_time) break;
    }

    alg.body.current_node = *std::max_element(
        alg.body.current_node->children.begin(),
        alg.body.current_node->children.end(),
        [](const auto& left, const auto& right) {
            return left->stats.avg_value() < right->stats.avg_value();
        }
    );

    return *alg.body.current_node;
};
