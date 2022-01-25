#include <algorithm>

auto mcts_iter_move = [] (auto& alg, auto settings) -> auto {
    alg.body.settings = settings;
    
    for (uint i = 0; i < settings.iters; i++) {
        alg.rollout(alg.current_node);
    }

    alg.body.current_node = std::max_element(
        alg.body.current_node.children.begin(),
        alg.body.current_node.children.end(),
        [](const auto& left, const auto& right) {
            return left.avg_value() < right.avg_value();
        }
    );
    return *alg.body.current_node;

};
