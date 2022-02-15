#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace mctslib {
inline auto mcts_select = []<class Alg>(Alg & alg, typename Alg::Node& initial_node) -> auto
{
    // assumptions: graph of states is a tree
    // TODO: Can make this (and other things) cleaner now that body.current_node is
    // std::shared_ptr<Node>. No more reference wrapper soon!
    std::reference_wrapper<typename Alg::Node> ref = initial_node;
    std::vector<std::reference_wrapper<typename Alg::Node>> path { ref };

    for (;;) {
        if (!ref.get().been_expanded() || !ref.get().children.size())
            return path;

        for (std::shared_ptr<typename Alg::Node> child : ref.get().children) {
            if (!child->been_expanded()) {
                path.push_back(std::ref(*child));
                return path;
            }
        }

        ref = **std::max_element( // replacing uct_select
            ref.get().children.begin(),
            ref.get().children.end(),
            [&](const auto& left, const auto& right) {
                return alg.uct(alg, left->stats) < alg.uct(alg, right->stats);
            });

        path.push_back(ref);
    }
};
}
