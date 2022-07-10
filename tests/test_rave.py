import pytest
from mctslib import RAVE
from mctslib.envs.up_or_right import State


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("randomize_ties", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_iter_rave(structure, randomize_ties, cas):
    alg = RAVE(State(1, 1), equivalence_param=10, structure=structure,
            randomize_ties=randomize_ties, max_action_value=1, constant_action_space=cas)
    for _ in range(10):
        node, children, global_stats = alg.search_using_iters(rollout_depth=10, iters=10, exploration_weight=2)
        alg.choose_best_node()


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("randomize_ties", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_cpu_time_rave(structure, randomize_ties, cas):
    alg = RAVE(State(1, 1), equivalence_param=10, structure=structure,
            randomize_ties=randomize_ties, max_action_value=1, constant_action_space=cas)
    for _ in range(10):
        node, children, global_stats = alg.search_using_cpu_time(rollout_depth=10, cpu_time=.05, exploration_weight=2)
        alg.choose_best_node()
