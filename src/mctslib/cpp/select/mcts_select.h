#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace mctslib {
inline auto mcts_select = []<class Alg>(Alg& alg, std::shared_ptr<typename Alg::Node> initial_node)
    -> std::vector<std::shared_ptr<typename Alg::Node>> {
    // assumptions: graph of states is a tree
    std::vector<std::shared_ptr<typename Alg::Node>> path { initial_node };
    std::shared_ptr<typename Alg::Node> node = initial_node;

    for (;;) {
        if (!node->been_expanded() || !node->children.size()) // TODO: make this more clear
            return path;

        for (std::shared_ptr<typename Alg::Node> child : node->children) { // find unexpanded node
            if (!child->been_expanded()) {
                path.push_back(child);
                return path;
            }
        }

        node = *std::max_element( // replacing uct_select
            node->children.begin(),
            node->children.end(),
            [&](const auto& left, const auto& right) {
                return alg.uct(alg, left->stats) < alg.uct(alg, right->stats);
            });

        path.push_back(node);
    }
};
}
