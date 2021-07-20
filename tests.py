from example import MCTS
import random
import pdb, traceback, sys

class Node:
	def __init__(self, x, y, visits=0):
		self.x = x
		self.y = y
		self.visits = visits
		self.backprop_value = 0
		self.value = 100 if x == 10 and y == 10 else 0

	def find_children(self):
		return [
				Node(self.x + 1, self.y, visits=self.visits + 1),
				Node(self.x - 1, self.y, visits=self.visits + 1),
				Node(self.x, self.y + 1, visits=self.visits + 1),
				Node(self.x, self.y - 1, visits=self.visits + 1)
			]


	def is_terminal(self):
		if self.x == 10 and self.y == 10:
			return True
		return False
	
	def random_child(self):
		return random.choice(self.find_children())

	def avg_reward(self):
		return self.backprop_value/self.visits

	def __lt__(self, other):
		return self.value < other.value

	def __repr__(self):
		return f"<Node({self.x=}, {self.y=})>"


if __name__ == "__main__":
	mcts = MCTS(1, 1, 10, 100)
	try:
		mcts.play(Node(0, 0))
	except:
		extype, value, tb = sys.exc_info()
		traceback.print_exc()
		pdb.post_mortem(tb)

