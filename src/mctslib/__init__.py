from . import _mctslib_mcts
from . import _mctslib_hrave


def MCTS(root, *, structure="tree", iter_stop=None, randomize_ties=True):
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "randomized_ties_" if randomize_ties else ""

    cls = getattr(_mctslib_mcts, f"{structure}_{iter_stop}_{randomized_str}MCTS")

    return cls(root)

def HRAVE(root, *, structure="tree", iter_stop=None, randomize_ties=True,
        equivalence_param, action_space_size):
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "randomized_ties_" if randomize_ties else ""

    cls = getattr(_mctslib_hrave, f"{structure}_{iter_stop}_{randomized_str}HRAVE")

    return cls(root, equivalence_param, action_space_size)

__all__ = ("MCTS", "HRAVE")
