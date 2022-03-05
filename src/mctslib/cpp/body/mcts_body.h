#include "util/empty.h"
#include <memory>
#include <unordered_map>
#include <type_traits>

namespace mctslib {
template <
    class Node_,
    class Settings,
    bool is_dag>
struct MCTSBody {
    using Node = Node_;

    Settings settings;
    std::shared_ptr<Node> current_node;
    [[no_unique_address]] std::conditional<is_dag, std::unordered_map<Node, std::shared_ptr<Node>>, Empty> canonical_map;

    MCTSBody(std::shared_ptr<Node> cur)
        : current_node(cur) {};
};
}
