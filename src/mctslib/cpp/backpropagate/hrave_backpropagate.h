#include <memory>
#include <type_traits>

namespace mctslib {
inline auto hrave_backpropagate = []<class Alg>(Alg& alg, const auto& path, const double value) -> void {
    for (std::shared_ptr<typename Alg::Node> node : path) {
        node->stats.visits++;
        node->stats.backprop_value += value;

        alg.body.global_amafs.at(node->stats.action).visits++;
        alg.body.global_amafs.at(node->stats.action).backprop_value += value;
    }
};
}
