import random, math
from mctslib import MCTS

class Node:
	__slots__ = ("x", "y", "evaluation")
	def __init__(self, x, y):
		self.x = x
		self.y = y
		self.evaluation = self.x*self.y
	
	def find_children(self):
		return [
				Node(self.x + 1, self.y),
				Node(self.x, self.y + 1),
			]
	
	def random_child(self):
		return random.choice(self.find_children())

	def is_terminal(self):
		return False

	def __lt__(self, other):
		return (self.x, self.y) < (other.x, other.y)
	def __repr__(self):
		return f"Node<{self.x=}, {self.y=}>"


if __name__ == "__main__":
	turn_limit = 100
	mcts = MCTS(Node(0, 0))
	for i in range(turn_limit):
		node = mcts.move(rollout_depth=3, iters=5)
		if i == turn_limit - 1:
			print(node)
