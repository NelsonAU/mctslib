from . import mctslib
from .mctslib import MCTS



def HRAVE(root, *args, action_space=None, hashable=False, **kwargs):
    if action_space not in ("dense", "sparse"):
        raise ValueError("Must provide action_space argument to HRAVE")

    hashable_str = "Hashable" if hashable else ""
    cls = getattr(mctslib, f"{hashable_str}{action_space.capitalize()}HRAVE")
    return cls(root, *args, **kwargs)


__all__ = [HRAVE, MCTS]
