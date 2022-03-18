#include <algorithm>
#include <memory>
#include <random>
#include <vector>

namespace mctslib {

inline auto hrave_value = []<class Alg>(Alg & alg, const typename Alg::NodeStats& stats) -> double
{
    double beta = sqrt(alg.body.equivalence_param / (3 * alg.body.current_node->stats.visits + alg.body.equivalence_param));
    double avg_amaf_value = alg.body.global_amafs.at(stats.action).avg_value();
    double avg_value = stats.avg_value();

    return (1 - beta) * avg_value + beta * avg_amaf_value;
};

inline auto hrave_iter_move_no_rng = []<class Alg>(Alg & alg, typename Alg::Settings settings) -> auto
{
    alg.body.settings = settings;

    for (uint i = 0; i < settings.iters; i++) {
        alg.rollout(alg, alg.body.current_node);
    }

    alg.body.current_node = *std::max_element(
        alg.body.current_node->children.begin(),
        alg.body.current_node->children.end(),
        [&alg](const auto& left, const auto& right) {
            return hrave_value(alg, left->stats) < hrave_value(alg, right->stats);
        });

    return *alg.body.current_node;
};

inline auto hrave_iter_move_rng = []<class Alg>(Alg & alg, typename Alg::Settings settings) -> auto
{
    alg.body.settings = settings;

    for (uint i = 0; i < settings.iters; i++) {
        alg.rollout(alg, alg.body.current_node);
    }

    std::vector<std::shared_ptr<typename Alg::Node>> ties {};
    double max = hrave_value(alg, alg.body.current_node->children[0]->stats);

    for (std::shared_ptr<typename Alg::Node> node_ptr : alg.body.current_node->children) {
        if (hrave_value(alg, node_ptr->stats) > max) {
            ties.clear();
            ties.push_back(node_ptr);
        } else if (hrave_value(alg, node_ptr->stats) == max) {
            ties.push_back(node_ptr);
        }
    }

    std::uniform_int_distribution<size_t> dist { 0, ties.size() - 1 };
    size_t idx = dist(Alg::rng);

    alg.body.current_node = ties[idx];

    return *alg.body.current_node;
};

inline auto hrave_cpu_move_no_rng = []<class Alg>(Alg & alg, typename Alg::Settings settings) -> auto
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
        [&alg](const auto& left, const auto& right) {
            return hrave_value(alg, left->stats) < hrave_value(alg, right->stats);
        });

    return *alg.body.current_node;
};

inline auto hrave_cpu_move_rng = []<class Alg>(Alg & alg, typename Alg::Settings settings) -> auto
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
    double max = hrave_value(alg, alg.body.current_node->children[0]->stats);

    for (std::shared_ptr<typename Alg::Node> node_ptr : alg.body.current_node->children) {
        if (hrave_value(alg, node_ptr->stats) > max) {
            ties.clear();
            ties.push_back(node_ptr);
        } else if (hrave_value(alg, node_ptr->stats) == max) {
            ties.push_back(node_ptr);
        }
    }

    std::uniform_int_distribution<size_t> dist { 0, ties.size() - 1 };
    size_t idx = dist(Alg::rng);

    alg.body.current_node = ties[idx];

    return *alg.body.current_node;
};
}
