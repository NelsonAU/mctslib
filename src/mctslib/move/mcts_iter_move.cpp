#include <algorithm>
#include <iostream>
namespace mctslib {
auto mcts_iter_move = []<class Alg>(Alg& alg, typename Alg::Settings settings) -> auto {
    alg.body.settings = settings;
    
    for (uint i = 0; i < settings.iters; i++) {
        alg.rollout(alg, *alg.body.current_node);
    }

        std::cout << "NODE VALUE" << __FILE__ << __LINE__ << std::endl; alg.body.current_node->print();
    alg.body.current_node = *std::max_element(
        alg.body.current_node->children.begin(),
        alg.body.current_node->children.end(),
        [](const auto& left, const auto& right) {
            return left->stats.avg_value() < right->stats.avg_value();
        }
    );

        std::cout << "NODE VALUE" << __FILE__ << __LINE__ << std::endl; alg.body.current_node->print();
    auto out = *alg.body.current_node;
    return out;
};
}
