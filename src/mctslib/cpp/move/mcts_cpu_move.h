#include <algorithm>
#include <chrono>
#include <memory>
#include <random>
#include <vector>

namespace mctslib {
inline auto mcts_cpu_move_no_rng = []<class Alg>(Alg & alg, typename Alg::Settings settings) -> auto
{
    alg.body.settings = settings;

    auto start = std::chrono::high_resolution_clock::now();

    for (;;) {
        alg.rollout(alg, alg.body.current_node);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;

        if (diff.count() > settings.cpu_time)
            break;
    }

    alg.body.current_node = *std::max_element(
        alg.body.current_node->children.begin(),
        alg.body.current_node->children.end(),
        [](const auto& left, const auto& right) {
            return left->stats.avg_value() < right->stats.avg_value();
        });

    return *alg.body.current_node;
};

inline auto mcts_cpu_move_rng = []<class Alg>(Alg & alg, typename Alg::Settings settings) -> auto
{
    alg.body.settings = settings;

    auto start = std::chrono::high_resolution_clock::now();

    for (;;) {
        alg.rollout(alg, alg.body.current_node);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;

        if (diff.count() > settings.cpu_time)
            break;
    }

    std::vector<std::shared_ptr<typename Alg::Node>> ties {};
    double max = alg.body.current_node->children[0]->stats.avg_value();

    for (std::shared_ptr<typename Alg::Node> node_ptr : alg.body.current_node->children) {
        if (node_ptr->stats.avg_value() > max) {
            ties.clear();
            ties.push_back(node_ptr);
        } else if (node_ptr->stats.avg_value() == max) {
            ties.push_back(node_ptr);
        }
    }

    std::uniform_int_distribution<size_t> dist { 0, ties.size() - 1 };
    size_t idx = dist(Alg::rng);

    alg.body.current_node = ties[idx];

    return *alg.body.current_node;
};
}
