
auto mcts_rollout = [](auto& alg, auto& node) -> void {
    auto path = alg.select(alg, node);
    auto leaf = path.back();
    expand(leaf);
    double reward = simulate(leaf);
    alg.backpropagate(path, reward);
};
