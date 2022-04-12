import pytest
from mctslib import GRAVE
from mctslib.envs.up_or_right import Node


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("randomize_ties", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_iter_grave(structure, randomize_ties, cas):
    alg = GRAVE(Node(1, 1), equivalence_param=10, structure=structure, iter_stop="iters",
                randomize_ties=randomize_ties, action_space_size=2, ref_threshold=50,
                constant_action_space=cas)
    for _ in range(10):
        alg.move(rollout_depth=10, iters=10, exploration_weight=2)


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("randomize_ties", [True, False])
@pytest.mark.parametrize("cas", [True, False])
def test_cpu_time_grave(structure, randomize_ties, cas):
    alg = GRAVE(Node(1, 1), equivalence_param=10, structure=structure, iter_stop="cpu_time",
                randomize_ties=randomize_ties, action_space_size=2, ref_threshold=50,
                constant_action_space=cas)

    for _ in range(10):
        alg.move(rollout_depth=10, cpu_time=.05, exploration_weight=2)
