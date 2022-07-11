import random


class State:
    __slots__ = ("x", "y", "_evaluation", "action_id")

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

    def __eq__(self, other):
        return (self.x, self.y) == (other.x, other.y)

    def __hash__(self):
        return hash((self.x, self.y))

    def __repr__(self):
        return f"State<self.x={self.x}, self.y={self.y}, self.eval={self._evaluation} self.action={self.action_id}>"
