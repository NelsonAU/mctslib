#include <iostream>
#include <memory>

namespace mctslib {
inline auto mcts_expand = []<class Alg>(Alg& alg, std::shared_ptr<typename Alg::Node> node) -> void {
    node->create_children();
};
}
