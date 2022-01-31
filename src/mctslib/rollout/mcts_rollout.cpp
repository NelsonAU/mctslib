#include <type_traits>

namespace mctslib {
auto mcts_rollout = [](auto& alg, auto& node) -> void {
    using Node = typename std::remove_reference<decltype(alg)>::type::Node;

    auto path = alg.select(alg, node);
    Node& leaf = path.back(); //auto defaults to interpreting this as a std::ref_wrapper
    alg.expand(alg, leaf);
    double reward = alg.simulate(alg, leaf);
    alg.backpropagate(alg, path, reward);
};
}
