from . import _mctslib_mcts
from . import _mctslib_hrave
from . import _mctslib_rave
from . import _mctslib_grave


def MCTS(root, *, max_action_value: int, iter_stop: str, backprop_decay: float = 1,
        structure: str = "tree", randomize_ties: bool = True, constant_action_space: bool = True):
    """
    Used to fetch and initialize the appropriate MCTS implementation from the shared object.
    """

    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"
    action_space_str = "const_action_space" if constant_action_space else "non_const_action_space"

    cls = getattr(_mctslib_mcts, f"{iter_stop}_{structure}_{randomized_str}_{action_space_str}_MCTS")

    return cls(backprop_decay, max_action_value, root)


def HRAVE(root, *, max_action_value: int, iter_stop: str, equivalence_param: int, 
            backprop_decay: float = 1, structure: str = "tree", randomize_ties: bool = True,
            constant_action_space: bool = True):
    """
    Used to fetch and initialize the appropriate HRAVE implementation from the shared object.
    """
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"
    action_space_str = "const_action_space" if constant_action_space else "non_const_action_space"

    cls = getattr(_mctslib_hrave, f"{iter_stop}_{structure}_{randomized_str}_{action_space_str}_HRAVE")

    return cls(backprop_decay, max_action_value, equivalence_param, root)


def RAVE(root, *, max_action_value: int, iter_stop: str, equivalence_param: int, 
            backprop_decay: float = 1, structure: str = "tree", randomize_ties: bool = True,
            constant_action_space: bool = True):
    """
    Used to fetch and initialize the appropriate RAVE implementation from the shared object.
    """
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"
    action_space_str = "const_action_space" if constant_action_space else "non_const_action_space"

    cls = getattr(_mctslib_rave, f"{iter_stop}_{structure}_{randomized_str}_{action_space_str}_RAVE")

    return cls(backprop_decay, max_action_value, equivalence_param, root)


def GRAVE(root, *, max_action_value: int, iter_stop: str, equivalence_param: int, ref_threshold: int,
            backprop_decay: float = 1, structure: str = "tree", randomize_ties: bool = True,
            constant_action_space: bool = True):
    """
    Used to fetch and initialize the appropriate GRAVE implementation from the shared object.
    """
    if iter_stop not in ("iters", "cpu_time"):
        raise ValueError(f"Argument iter_stop must be 'iters' or 'cpu_time', not {iter_stop}")

    if structure not in ("tree", "dag"):
        raise ValueError(f"Argument structure must be 'tree' or 'dag', not {structure}")

    randomized_str = "rng_ties" if randomize_ties else "no_rng_ties"
    action_space_str = "const_action_space" if constant_action_space else "non_const_action_space"

    cls = getattr(_mctslib_grave, f"{iter_stop}_{structure}_{randomized_str}_{action_space_str}_GRAVE")

    return cls(backprop_decay, max_action_value, equivalence_param, ref_threshold, root)


__all__ = ("MCTS", "HRAVE", "RAVE", "GRAVE")
