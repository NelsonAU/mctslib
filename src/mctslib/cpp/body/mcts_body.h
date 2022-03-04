#include "util/empty.h"
#include <memory>
#include <unordered_map>
namespace mctslib {
template <
    class Node_,
    class Settings>
struct TreeMCTSBody {
    using Node = Node_;

    Settings settings;
    std::shared_ptr<Node> current_node;

    TreeMCTSBody(std::shared_ptr<Node> cur)
        : current_node(cur) {};
};

template <
    class Node_,
    class Settings>
struct DAGMCTSBody {
    using Node = Node_;

    Settings settings;
    std::shared_ptr<Node> current_node;
    std::unordered_map<Node, std::shared_ptr<Node>> canonical_map;

    DAGMCTSBody(std::shared_ptr<Node> cur)
        : current_node(cur) {};
};
}
