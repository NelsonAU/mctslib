#include <algorithm>
#include <functional>
#include <vector>

namespace mctslib {
auto mcts_select = [](auto alg, auto& initial_node) -> auto {
    // assumptions: graph of states is a tree
    using Node = typename std::remove_reference<decltype(alg)>::type::Node;
    std::reference_wrapper<Node> ref = initial_node;
    std::vector<std::reference_wrapper<Node>> path {ref};

    for (;;) {
        if (!ref.get().been_expanded || !ref.get().children.size()) return path;

        for (Node* child : ref.get().children) {
            if (!child->been_expanded) {
               path.push_back(std::ref(*child));
               return path;
            }
        }

        ref = **std::max_element( //replacing uct_select
            ref.get().children.begin(), 
            ref.get().children.end(), 
            [&](const auto& left, const auto& right) {
                return alg.uct(alg, left->stats) < alg.uct(alg, right->stats);
            }
        );

        path.push_back(ref);
    }
};
}
