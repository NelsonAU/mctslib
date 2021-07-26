from mctslib import MCTS
import random

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
	resultant_node = MCTS(1, .1, 10, 100).play(Node(0, 0))
	print(resultant_node)
