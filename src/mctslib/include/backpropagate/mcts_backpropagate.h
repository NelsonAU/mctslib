#include <type_traits>

namespace mctslib {
inline auto mcts_backpropagate = []<class Alg>(Alg& alg, const auto& path, const double value) -> void {
    using Node = typename Alg::Node;
    for (Node& node : path) {
        node.stats.visits++;
        node.stats.backprop_value += value;
    }
};
}
