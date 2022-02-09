#include <functional>

namespace mctslib {
inline auto mcts_simulate = []<class Alg>(Alg& alg, typename Alg::Node& start) -> double {
    typename Alg::Node node = start;
    for (uint i = 0; i < alg.body.settings.rollout_depth; i++) {
        if (node.is_terminal()) break;

        node = node.random_child();
    }

    return node.stats.evaluation;
};
}
