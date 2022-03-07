import pytest
from mctslib import HRAVE
from mctslib.envs.up_or_right import Node

# @pytest.mark.parametrize("structure", ["tree", "dag"])
# @pytest.mark.parametrize("randomize_ties", [True, False])
# def test_iter_hrave(structure, randomize_ties):
#     alg = HRAVE(Node(0, 0), equivalence_param=10, structure=structure, iter_stop="iters",
#            randomize_ties=randomize_ties)
#     alg.move(rollout_depth=10, iters=10, exploration_weight=2)


@pytest.mark.parametrize("structure", ["tree", "dag"])
@pytest.mark.parametrize("randomize_ties", [True, False])
def test_cpu_time_hrave(structure, randomize_ties):
    alg = HRAVE(Node(0, 0), equivalence_param=10, structure=structure, iter_stop="cpu_time",
                randomize_ties=randomize_ties, action_space_size=2)
    alg.move(rollout_depth=10, cpu_time=.2, exploration_weight=2)


def test_hrave():
    alg = HRAVE(Node(0, 0), equivalence_param=10, structure="tree", iter_stop="iters",
                randomize_ties=False, action_space_size=2)
    alg.move(rollout_depth=10, iters=10, exploration_weight=2)
