#include <functional>
#include <type_traits>
#include <vector>

namespace mctslib {
inline auto mcts_rollout = []<class Alg>(Alg& alg, typename Alg::Node& node) -> void {
    std::vector<std::reference_wrapper<typename Alg::Node>> path = alg.select(alg, node);
    typename Alg::Node& leaf = path.back();
    alg.expand(alg, leaf);
    double reward = alg.simulate(alg, leaf);
    alg.backpropagate(alg, path, reward);
};
}
