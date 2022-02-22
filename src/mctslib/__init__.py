from . import _mctslib


def MCTS(root, *args, iter_stop=None, structure="tree", randomize_ties=True, **kwargs):
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "randomized_ties_" if randomize_ties else ""

    cls = getattr(_mctslib, f"{structure}_{iter_stop}_{randomized_str}MCTS")

    return cls(root, *args, **kwargs)


__all__ = ("MCTS")
