import random, math
from .envs.up_or_right import Node
from mctslib import HRAVE


def test_hrave():
	turn_limit = 100
	hrave = HRAVE(Node(0, 0), 50, 2)
	for i in range(turn_limit):
		node = hrave.move(rollout_depth=3, iters=5)
		if i == turn_limit - 1:
			print(node)


if __name__ == "__main__":
	test_hrave()
