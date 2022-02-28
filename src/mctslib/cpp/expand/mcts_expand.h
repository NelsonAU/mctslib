#include <iostream>
#include <memory>

namespace mctslib {
inline auto tree_mcts_expand = []<class Alg>(Alg& alg, std::shared_ptr<typename Alg::Node> node) -> void {
    node->create_children();
};


inline auto dag_mcts_expand = []<class Alg>(Alg& alg, std::shared_ptr<typename Alg::Node> node) -> void {
    node->create_children();

    for (uint i = 0; i < node->children.size(); i++) {
        if (alg.body.canonical_map.count(*(node->children[i]))) {
            node->children[i] = alg.body.canonical_map[*(node->children[i])];
        } else {
            alg.body.canonical_map[*(node->children[i])] = node->children[i];
        }
    }
};
}
