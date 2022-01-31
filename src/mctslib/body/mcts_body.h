


template<
    template<class> class NodeTemplate,
    template<class> class NodeStatsTemplate,
    typename Action,
    class Settings_
>
struct MCTSBody { //TODO: implement DAGMCTSBody
    using NodeStats = NodeStatsTemplate<Action>;
    using Node = NodeTemplate<NodeStats>;
    using Settings = Settings_;
    Settings settings;
    Node* current_node;

    MCTSBody (Node* cur) : current_node(cur) {};
};


