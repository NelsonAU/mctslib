import random, math
from .envs.up_or_right import Node
from mctslib import MCTS


def test_mcts():
	turn_limit = 100
	mcts = MCTS(Node(0, 0))
	for i in range(turn_limit):
		node = mcts.move(rollout_depth=3, iters=5)
		if i == turn_limit - 1:
			print(node)


if __name__ == "__main__":
	test_mcts()
