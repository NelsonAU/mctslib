
auto mcts_backpropagate = [](auto alg, const auto& path, const double value) -> void {
    for (auto node : path) {
        node.stats.visits++;
        node.stats.backprop_value += value;
    }
};
