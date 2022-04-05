from . import _mctslib_mcts
from . import _mctslib_hrave


def MCTS(root, *, structure="tree", iter_stop=None, randomize_ties=True):

    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"

    cls = getattr(_mctslib_mcts, f"{iter_stop}_{structure}_{randomized_str}_MCTS")

    return cls(root)


def HRAVE(root, *, structure="tree", iter_stop=None, randomize_ties=True,
            equivalence_param, action_space_size):

    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"

    cls = getattr(_mctslib_hrave, f"{iter_stop}_{structure}_{randomized_str}_HRAVE")

    return cls(action_space_size, equivalence_param, root)


__all__ = ("MCTS")
