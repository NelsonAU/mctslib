#include "util/empty.h"
#include <memory>
#include <unordered_map>
namespace mctslib {
template <
    template <class> class NodeTemplate,
    template <class> class NodeStatsTemplate,
    class Settings_>
struct TreeMCTSBody {
    using NodeStats = NodeStatsTemplate<Empty>;
    using Node = NodeTemplate<NodeStats>;
    using Settings = Settings_;

    Settings settings;
    std::shared_ptr<Node> current_node;

    TreeMCTSBody(std::shared_ptr<Node> cur)
        : current_node(cur) {};
};

template <
    template <class> class NodeTemplate,
    template <class> class NodeStatsTemplate,
    class Settings_>
struct DAGMCTSBody {
    using NodeStats = NodeStatsTemplate<Empty>;
    using Node = NodeTemplate<NodeStats>;
    using Settings = Settings_;

    Settings settings;
    std::shared_ptr<Node> current_node;
    std::unordered_map<Node, std::shared_ptr<Node>> canonical_map;

    DAGMCTSBody(std::shared_ptr<Node> cur)
        : current_node(cur) {};
};
}
