
template<
    class Body,
    class Move,
    class Rollout,
    class Select,
    class UCT,
    class Expand,
    class Simulate,
    class Backpropagate
>
class Algorithm {
    using Node = typename Body::Node;
    using NodeStats = typename Body::NodeStats;
    using Settings = typename Body::Settings;

    Body body;
    Move move;
    Rollout rollout;
    Select select;
    UCT uct;
    Expand expand;
    Simulate simulate;
    Backpropagate backpropagate;
};
