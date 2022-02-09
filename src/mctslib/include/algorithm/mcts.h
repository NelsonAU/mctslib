#include "algorithm/algorithm.h"
#include "backpropagate/backpropagate.h"
#include "body/body.h"
#include "expand/expand.h"
#include "rollout/rollout.h"
#include "move/move.h"
#include "select/select.h"
#include "settings/settings.h"
#include "simulate/simulate.h"
#include "stats/stats.h"
#include "uct/uct.h"
#include "util/empty.h"

namespace mctslib {
template<
    template<class Stats> class NodeTemplate,
    class Move
>
using TreeMCTS = Algorithm<
    MCTSBody<NodeTemplate, MCTSStats, MCTSSettings>,
    Move,
    decltype(mcts_rollout),
    decltype(mcts_select),
    decltype(mcts_uct),
    decltype(mcts_expand),
    decltype(mcts_simulate),
    decltype(mcts_backpropagate)
>;
}
