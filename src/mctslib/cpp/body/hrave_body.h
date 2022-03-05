#include "util/empty.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include "stats/mcts_stats.h"

namespace mctslib {
template <
    class Node_,
    class Settings,
    bool sparse_action_space>
class TreeHRAVEBody {
    using Node = Node_;

    Settings settings;
    uint equivalence_param;
    std::shared_ptr<Node> current_node;
    std::conditional<
        sparse_action_space,
        std::unordered_map<decltype(decltype(Node::stats)::action), MCTSStats>,
        std::vector<MCTSStats>> global_amafs;

    TreeHRAVEBody(std::shared_ptr<Node> cur, uint equivalence_param, uint action_space_size)
        : equivalence_param(equivalence_param)
        , current_node(cur)
    {
        if constexpr (sparse_action_space) {
            global_amafs(action_space_size);
        }
    }
};
}
