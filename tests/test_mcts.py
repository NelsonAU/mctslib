import pytest
from mctslib import MCTS
from mctslib.envs.up_or_right import Node


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("rng", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_iter_mcts(structure, rng, cas):
    alg = MCTS(Node(1, 1), structure=structure, iter_stop="iters", randomize_ties=rng,
            max_action_value=1, constant_action_space=cas)
    for _ in range(1000):
        node = alg.move(rollout_depth=2, iters=10, exploration_weight=1)


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("rng", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_cpu_time_mcts(structure, rng, cas):
    alg = MCTS(Node(1, 1), structure=structure, iter_stop="cpu_time", randomize_ties=rng,
            max_action_value=1, constant_action_space=cas)
    for _ in range(1000):
        node = alg.move(rollout_depth=2, cpu_time=.005, exploration_weight=1)
