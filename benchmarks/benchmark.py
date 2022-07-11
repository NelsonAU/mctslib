from mctslib import MCTS
from mctslib.envs.up_or_right import State
import sys

if __name__ == "__main__":
    iters = int(sys.argv[1])
    root = State(1, 1)
    mcts = MCTS(root, max_action_value=1)
    mcts.search_using_iters(iters=iters, rollout_depth=50, exploration_weight=2)





