#include <type_traits>

namespace mctslib {
auto mcts_backpropagate = [](auto& alg, const auto& path, const double value) -> void {
    using Node = typename std::remove_reference<decltype(alg)>::type::Node;
    for (Node& node : path) {
        node.stats.visits++;
        node.stats.backprop_value += value;
    }
};
}
