
auto mcts_simulate = [](auto& alg, const auto& start) -> double {
    auto& node = start;
    for (uint i = 0; i < alg.settings.rollout_depth; i++) {
        if (node.is_terminal()) break;

        node = node.random_child();
    }

    return node.stats.evaluation;
};
