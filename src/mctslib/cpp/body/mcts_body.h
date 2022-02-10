#include "util/empty.h"


template<
    template<class> class NodeTemplate,
    template<class> class NodeStatsTemplate,
    class Settings_
>
struct MCTSBody { //TODO: implement DAGMCTSBody
    using NodeStats = NodeStatsTemplate<Empty>;
    using Node = NodeTemplate<NodeStats>;
    using Settings = Settings_;
    using Action = Empty;

    Settings settings;
    Node* current_node;

    MCTSBody (Node* cur) : current_node(cur) {};
};


