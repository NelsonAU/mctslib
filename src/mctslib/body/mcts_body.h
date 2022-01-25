

template<
    template<class> class NodeTemplate,
    class _NodeStats,
    class Settings
>
class TreeMCTSBody { //TODO: implement DAGMCTSBody
public:
    using NodeStats = _NodeStats;
    using Node = NodeTemplate<NodeStats>;
    Settings settings;
    Node* current_node;
};


