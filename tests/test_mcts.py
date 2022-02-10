from mctslib import MCTS
from mctslib.envs.up_or_right import Node


def test_tree_iter_mcts():
    alg = MCTS(Node(0, 0), structure="tree", iter_stop="iters")
    alg.move(rollout_depth=10, iters=10, exploration_weight=2)


def test_tree_cpu_time_mcts():
    alg = MCTS(Node(0, 0), structure="tree", iter_stop="cpu_time")
    alg.move(rollout_depth=10, cpu_time=.2, exploration_weight=2)
