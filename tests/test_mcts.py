import pytest
from mctslib import MCTS
from mctslib.envs.up_or_right import Node


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("rng", [True, False])
def test_iter_mcts(structure, rng):
    alg = MCTS(Node(0, 0), structure=structure, iter_stop="iters", randomize_ties=rng)
    alg.move(rollout_depth=10, iters=10, exploration_weight=2)


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("rng", [True, False])
def test_cpu_time_mcts(structure, rng):
    alg = MCTS(Node(0, 0), structure=structure, iter_stop="cpu_time", randomize_ties=rng)
    alg.move(rollout_depth=10, cpu_time=.2, exploration_weight=2)
