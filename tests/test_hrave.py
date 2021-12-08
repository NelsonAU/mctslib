import pytest
from mctslib.envs.up_or_right import Node
from mctslib import HRAVE


@pytest.mark.parametrize("k", [10, 50])
@pytest.mark.parametrize("turn_limit", [50, 100])
@pytest.mark.parametrize("action_space", ["sparse", "dense"])
@pytest.mark.parametrize("hashable", [True, False])
@pytest.mark.parametrize("rollout_depth", [10, 50])
def test_hrave_iters(*, k, turn_limit, action_space, hashable, rollout_depth):

    action_space_size = Node.get_action_space_size() if action_space == "dense" else -1
    hrave = HRAVE(Node(0, 0), k, action_space_size, action_space=action_space, hashable=hashable)

    for i in range(turn_limit):
        node = hrave.move(rollout_depth=rollout_depth, iters=5)
        if i == turn_limit - 1:
            return node
