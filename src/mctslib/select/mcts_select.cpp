#include <algorithm>
#include <functional>
#include <vector>

auto mcts_select = [](auto alg, auto& initial_node) -> auto& {
    // assumptions: graph of states is a tree
    using Node = typename decltype(alg)::Node;
    std::vector<std::reference_wrapper<Node>> path {std::ref(initial_node)};
    Node node = initial_node;

    for (;;) {
       if (!node.been_expanded || !node.children.size()) return path;

       for (Node* child : node.children) {
           if (!child->been_expanded) {
               path.push_back(std::ref(*child));
               return path;
           }
       }
    }
    
    return std::max_element( //replacing uct_select
        node.children.begin(), 
        node.children.end(), 
        [&](const auto& left, const auto& right) {
            return alg.uct(alg, left.stats) < alg.uct(alg, right.stats);
        }
    );
    
};
