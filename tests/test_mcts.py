import pytest
from mctslib import MCTS
from mctslib.envs.up_or_right import Node

@pytest.mark.parametrize("randomize_ties", [True, False])
def test_tree_iter_mcts(randomize_ties):
    alg = MCTS(Node(0, 0), structure="tree", iter_stop="iters", randomize_ties=randomize_ties)
    alg.move(rollout_depth=10, iters=10, exploration_weight=2)


@pytest.mark.parametrize("randomize_ties", [True, False])
def test_tree_cpu_time_mcts(randomize_ties):
    alg = MCTS(Node(0, 0), structure="tree", iter_stop="cpu_time", randomize_ties=randomize_ties)
    alg.move(rollout_depth=10, cpu_time=.2, exploration_weight=2)
