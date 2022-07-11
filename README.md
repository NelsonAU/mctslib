# mctslib

## Development setup
To do development on this project
```sh
$ python3 setup.py develop # setup.py will build and install development version
```

To run tests:
```sh
$ tox -e py39,flake8 # substitute 39 for desired python version
```

## Defining an environment in Python

The primary feature of mctslib is the ability to define an environment in Python and be able to run
any algorithm mctslib provides in that environment. Some algorithms have extra requirements (for
example, that the node store the action that led to the state, or that the node is hashable). Here 
is a minimal example of an environment in which the agent can only move up or right. The score of any
state in this environment is the product of the agent's x and y position.

```python
# up_or_right.py
import random


class State:
    def __init__(self, x, y, action_id=0):
        self.x = x
        self.y = y
        self._evaluation = (self.x * self.y)/((self.x/2 + self.y/2)**2)
        self.action_id = action_id

    def find_children(self):
        return [State(self.x + 1, self.y, action_id=0), State(self.x, self.y + 1, action_id=1)]

    def apply_action(self, action_id):
        if action_id == 0:
            return State(self.x + 1, self.y, action_id=0)
        else:
            return State(self.x, self.y + 1, action_id=1)

    def get_legal_actions(self):
        return [0, 1]

    def default_policy(self):
        return random.choice(self.find_children())

    def evaluation(self):
        return self._evaluation

    def is_terminal(self):
        return False

```




## Using mctslib defined algorithms


mctslib implements many variations of algorithms with different options and performance characteristics.
To make choosing an algorithm as simple as possible, mctslib exposes helper functions that will choose
the correct algorithm implementation based on the settings you provide.

### MCTS

To get an instance of MCTS using mctslib, use the MCTS helper function.

```python

def MCTS(root, *, max_action_value: int, backprop_decay: float = 1, structure: str = "tree",
        randomize_ties: bool = True, constant_action_space: bool = True):
    ...
```

MCTS gives you several options to choose from:

- `structure: {"tree", "dag"}`: Controls whether MCTS will take advantage of the environment being
a DAG instead of a tree. This comes with some overhead - if `"dag"` is chosen, then a map is used to
track down the 'canonical' reference to a node. In environments with many transpositions, this can 
improve performance.
- `randomize_ties: bool`: If `True`, then nodes which have the same evaluation will be 
equally likely to be chosen. Otherwise, the node with the lower index is chosen. Choosing the node with
the lowest index can be helpful in some circumstances. For example, in the Atari Learning Environment,
the node with the lowest index is always the result of the agent taking no action. This makes it visually
clear when the agent cannot figure out a path forward.
- `backprop_decay: float`: Defaults to 1, discount factor applied to rewards when backpropagating up
the tree.
- `constant_action_space: bool`: Defaults to `True`, if `True`, will only call `get_legal_actions`
once and save the result.

The return value of MCTS is an algorithm initialized with a root node. Each algorithm in mctslib
exposes five methods: `search_using_iters`, `search_using_cpu_time`, `choose`, `choose_best_node`,
and `get_global_stats`.

```python
# Signature of methods on MCTS as it appears from Python

class MCTS:
    def search_using_iters(self, *, rollout_depth: int, iters: int, exploration_weight: float) -> Node:
        ...

    def search_using_cpu_time(self, *, rollout_depth: int, cpu_time: float, exploration_weight: float) -> Node:
        ...

    def choose_node(self, node) -> Node:
        ...

    def choose_best_node(self) -> Node:
        ...

    def get_global_stats(self) -> dict:
        ...
```









