from .mctslib import *


def HRAVE(root, *args, action_space=None, **kwargs):
	if action_space == "sparse":
		return SparseHRAVE(root, *args, **kwargs)

	if action_space == "dense":
		return DenseHRAVE(root, *args, **kwargs)

	raise ValueError()


__all__ = [HRAVE, MCTS]
