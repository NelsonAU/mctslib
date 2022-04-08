from . import _mctslib_mcts
from . import _mctslib_hrave
from . import _mctslib_rave
from . import _mctslib_grave


def MCTS(root, *, structure: str = "tree", iter_stop: str, randomize_ties: bool = True):
    """
    Used to fetch and initialize the appropriate MCTS implementation from the shared object.
    """
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"

    cls = getattr(_mctslib_mcts, f"{iter_stop}_{structure}_{randomized_str}_MCTS")

    return cls(root)


def HRAVE(root, *, structure: str = "tree", iter_stop: str, randomize_ties: bool = True,
            equivalence_param: int, action_space_size: int):
    """
    Used to fetch and initialize the appropriate HRAVE implementation from the shared object.
    """
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"

    cls = getattr(_mctslib_hrave, f"{iter_stop}_{structure}_{randomized_str}_HRAVE")

    return cls(action_space_size, equivalence_param, root)


def RAVE(root, *, structure: str = "tree", iter_stop: str, randomize_ties: bool = True,
            equivalence_param: int, action_space_size: int):
    """
    Used to fetch and initialize the appropriate RAVE implementation from the shared object.
    """
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"

    cls = getattr(_mctslib_rave, f"{iter_stop}_{structure}_{randomized_str}_RAVE")

    return cls(action_space_size, equivalence_param, root)


def GRAVE(root, *, structure: str = "tree", iter_stop: str, randomize_ties: bool = True,
            equivalence_param: int, action_space_size: int, ref_threshold: int):
    """
    Used to fetch and initialize the appropriate GRAVE implementation from the shared object.
    """
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"

    cls = getattr(_mctslib_grave, f"{iter_stop}_{structure}_{randomized_str}_GRAVE")

    return cls(action_space_size, equivalence_param, ref_threshold, root)


__all__ = ("MCTS", "HRAVE", "RAVE", "GRAVE")
