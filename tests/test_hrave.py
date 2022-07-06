import pytest
from mctslib import HRAVE
from mctslib.envs.up_or_right import Node


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("randomize_ties", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_iter_hrave(structure, randomize_ties, cas):
    alg = HRAVE(Node(1, 1), equivalence_param=10, structure=structure,
                   randomize_ties=randomize_ties, max_action_value=1, constant_action_space=cas)
    for _ in range(10):
        node, children, global_stats = alg.search_using_iters(rollout_depth=10, iters=10, exploration_weight=2)
        alg.choose_best_node()


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("randomize_ties", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_cpu_hrave(structure, randomize_ties, cas):
    alg = HRAVE(Node(1, 1), equivalence_param=10, structure=structure,
                   randomize_ties=randomize_ties, max_action_value=1, constant_action_space=cas)
    for _ in range(10):
        node, children, global_stats = alg.search_using_cpu_time(rollout_depth=10, cpu_time=.05, exploration_weight=2)
        alg.choose_best_node()
