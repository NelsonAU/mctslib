import pytest
from .envs.up_or_right import Node
from mctslib import MCTS

@pytest.mark.parametrize("turn_limit", [50, 100])
@pytest.mark.parametrize("hashable", [True, False])
@pytest.mark.parametrize("rollout_depth", [10, 50])
def test_mcts_iters(*, turn_limit, hashable, rollout_depth):

	mcts = MCTS(Node(0, 0), hashable=hashable)

	for i in range(turn_limit):
		node = mcts.move(rollout_depth=rollout_depth, iters=5)
		if i == turn_limit - 1:
			return node




