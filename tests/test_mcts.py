import pytest
from mctslib import MCTS
from mctslib.envs.up_or_right import State


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("rng", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_iter_mcts(structure, rng, cas):
    alg = MCTS(State(1, 1), structure=structure, randomize_ties=rng,
            max_action_value=1, constant_action_space=cas)
    for _ in range(10):
        node, children, global_stats = alg.search_using_iters(rollout_depth=2, iters=10, exploration_weight=1)
        alg.choose_best_node()


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("rng", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_cpu_time_mcts(structure, rng, cas):
    alg = MCTS(State(1, 1), structure=structure, randomize_ties=rng,
            max_action_value=1, constant_action_space=cas)
    for _ in range(10):
        node, children, global_stats = alg.search_using_cpu_time(rollout_depth=2, cpu_time=.05, exploration_weight=1)
        alg.choose_best_node()
