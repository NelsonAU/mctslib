# mctslib

## Development setup
To do development on this project, do
```sh
$ python3 setup.py develop # setup.py will build and install development version
```

To run tests and flake8, do
```sh
$ tox -e py39,flake8 # substitute 39 for desired python version
```

## Defining an environment

### Python
The primary feature of mctslib is the ability to define an environment in Python and be able to run
any algorithm mctslib provides in that environment. Some algorithms have extra requirements (for
example, that the node store the action that led to the state, or that the node is hashable). Here 
is a minimal example of an environment in which the agent can only move up or right. The score of any
state in this environment is the product of the agent's x and y position.

```python
# up_or_right.py
import random


class Node:
    __slots__ = ("x", "y", "_evaluation", "action")

    def __init__(self, x, y):
        self.x = x
        self.y = y
        self._evaluation = self.x*self.y

    def find_children(self):
        return [
                Node(self.x + 1, self.y),
                Node(self.x, self.y + 1),
            ]

    def random_child(self):
        return random.choice(self.find_children())

    def evaluation(self):
        return self._evaluation

    def is_terminal(self):
        return False
```


### C++
Environments can be defined in C++ as well. Here is the above environment defined in C++:
```cpp
// up_or_right.h
#include <memory>
#include<random>
#include <vector>

template <class NodeStats>
class Node {
private:
    bool _been_expanded = false;
    int x;
    int y;
public:
    static inline std::mt19937 rng;
    NodeStats stats;
    std::vector<std::shared_ptr<Node>> children;

    Node(int x, int y) : x(x), y(y), stats(x*y) {}

    void create_children() {
        children.push_back(std::make_shared<Node>(x + 1, y));
        children.push_back(std::make_shared<Node>(x, y + 1));
        _been_expanded = true;
    }

    Node random_child() const {
        std::uniform_int_distribution<int> dist { 0, 1 };
        int result = dist(rng);
        
        if (result) return Node(x + 1, y);
        return Node(x, y + 1);
    }

    bool is_terminal() const { return false; }

    bool been_expanded const { return _been_expanded; }
}
```

There are a few things that mctslib will assume about the class that may not be obvious from
the above snippet:

- The constructor of NodeStats expects a value of type double that will be used as the evaluation of the node.
- children must be public, this is necessary for algorithms which leverage environments being DAGs rather than trees.
- children does not necessarily need to be a vector, but must be iterable, and must use std::shared\_ptr.










