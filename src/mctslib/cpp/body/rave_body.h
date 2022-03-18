#include "settings/settings.h"
#include "stats/mcts_stats.h"
#include "util/empty.h"
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace mctslib {
template <
    class Node_,
    class Settings,
    bool is_dag>
class RAVEBody {
public:
    using Node = Node_;

    Settings settings;
    uint equivalence_param;
    std::shared_ptr<Node> current_node;

    [[no_unique_address]] typename std::conditional<
        is_dag,
        std::unordered_map<Node, std::shared_ptr<Node>>,
        Empty>::type canonical_map;

    RAVEBody(std::shared_ptr<Node> cur, uint equivalence_param, uint action_space_size)
        : equivalence_param(equivalence_param)
        , current_node(cur)
        , global_amafs(action_space_size)
    {
    }
};
}
