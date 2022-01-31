#include <algorithm>
#include <iostream>

auto mcts_iter_move = [] (auto& alg, auto settings) -> auto {
    alg.body.settings = settings;
    
        std::cout << __LINE__ << std::endl;
    for (uint i = 0; i < settings.iters; i++) {
        alg.rollout(alg, *alg.body.current_node);
        std::cout << __LINE__ << std::endl;
    }

        std::cout << __LINE__ << alg.body.current_node->children.size() << std::endl;
    alg.body.current_node = *std::max_element(
        alg.body.current_node->children.begin(),
        alg.body.current_node->children.end(),
        [](const auto& left, const auto& right) {
            return left->stats.avg_value() < right->stats.avg_value();
        }
    );

        std::cout << __LINE__ << std::endl;
    auto out = *alg.body.current_node;
        std::cout << __LINE__ << std::endl;
    return out;
};
