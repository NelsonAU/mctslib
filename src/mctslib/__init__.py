from . import _mctslib

def MCTS(root, *args, **kwargs):
    """TODO: for now, this is simple, but adding versions of MCTS that store a map of canonical
    nodes make things more complicated"""
    return _mctslib.MCTS(root, *args, **kwargs)

__all__ = ("MCTS")
