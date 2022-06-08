from mctslib import MCTS
from mctslib.envs.up_or_right import Node


alg = MCTS(Node(1, 1), structure="tree", iter_stop="iters", randomize_ties=True,
            max_action_value=1, constant_action_space=True)


for _ in range(10):
    node, global_stats = alg.move(rollout_depth=2, iters=10, exploration_weight=1)
    print(node, global_stats, node.state, node.stats, node.children())

