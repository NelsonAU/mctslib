from mctslib import MCTS
from mctslib.envs import Node
import random


def test_tree_iter_mcts():
    alg = MCTS(Node(0, 0), structure="tree", iter_stop="iters")
    alg.move(2, 10, 5, 0)
    alg.move(2, 10, 5, 0)
