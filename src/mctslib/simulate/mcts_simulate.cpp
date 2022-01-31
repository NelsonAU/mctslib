
auto mcts_simulate = [](auto& alg, auto& start) -> double {
    auto& node = start;
    for (uint i = 0; i < alg.body.settings.rollout_depth; i++) {
        if (node.is_terminal()) break;

        node = node.random_child();
    }

    return node.stats.evaluation;
};
