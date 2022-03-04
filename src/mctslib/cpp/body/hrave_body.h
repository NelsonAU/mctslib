#include <memory>
#include <vector>
#include "util/empty.h"



namespace mctslib {
template<
    template <class> class NodeTemplate,
    template <class> class NodeStatsTemplate,
    typename Action,
    class Settings_>
class DenseHRAVEBody {
    using NodeStats = NodeStatsTemplate<Action>;
    using Node = NodeTemplate<NodeStats>;
    using Settings = Settings_;
    using Action = typename NodeStats::Action;

    Settings_ settings;
    uint equivalence_param;
    std::vector<NodeStats> global_amafs;
    std::shared_ptr<Node> current_node;

    DenseHRAVEBody(std::shared_ptr<Node> cur, uint equivalence_param, uint action_space_size) 
        : equivalence_param(equivalence_param), current_node(cur), global_amafs(action_space_size) {}
};
}
