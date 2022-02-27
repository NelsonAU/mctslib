#include <functional>
#include <memory>

namespace mctslib {
inline auto mcts_simulate = []<class Alg>(Alg& alg, std::shared_ptr<typename Alg::Node> start) -> double {
    typename Alg::Node node = *start;
    for (uint i = 0; i < alg.body.settings.rollout_depth; i++) {
        if (node.is_terminal())
            break;

        node = node.random_child();
    }

    return node.stats.evaluation;
};
}
