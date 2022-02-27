#include <memory>
#include <type_traits>

namespace mctslib {
inline auto mcts_backpropagate = []<class Alg>(Alg& alg, const auto& path, const double value) -> void {
    for (std::shared_ptr<typename Alg::Node> node : path) {
        node->stats.visits++;
        node->stats.backprop_value += value;
    }
};
}
