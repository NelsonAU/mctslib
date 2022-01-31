#include "mctslib/algorithm/algorithm.h"
#include "mctslib/backpropagate/backpropagate.h"
#include "mctslib/body/body.h"
#include "mctslib/expand/expand.h"
#include "mctslib/rollout/rollout.h"
#include "mctslib/move/move.h"
#include "mctslib/node/python_node.h"
#include "mctslib/select/select.h"
#include "mctslib/settings/mcts_settings.h"
#include "mctslib/simulate/simulate.h"
#include "mctslib/stats/stats.h"
#include "mctslib/uct/uct.h"
#include "mctslib/util/empty.h"

namespace mctslib {
template<
    template<template<class> class, template<class> class, class, class> class BodyTemplate,
    template<class> class NodeTemplate,
    class Move
>
using MCTS = Algorithm<
    BodyTemplate,
    NodeTemplate,
    MCTSStats,
    MCTSSettings,
    Empty,
    Move,
    decltype(mcts_rollout),
    decltype(mcts_select),
    decltype(mcts_uct),
    decltype(mcts_expand),
    decltype(mcts_simulate),
    decltype(mcts_backpropagate)
>;
}
