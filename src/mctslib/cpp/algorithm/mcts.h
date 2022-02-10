#include "algorithm/algorithm.h"
#include "backpropagate/backpropagate.h"
#include "body/body.h"
#include "expand/expand.h"
#include "move/move.h"
#include "rollout/rollout.h"
#include "select/select.h"
#include "settings/settings.h"
#include "simulate/simulate.h"
#include "stats/stats.h"
#include "uct/uct.h"
#include "util/empty.h"

namespace mctslib {
template <
    template <class Stats> class NodeTemplate,
    class Move,
    class Settings>
using TreeMCTS = Algorithm<
    MCTSBody<NodeTemplate, MCTSStats, Settings>,
    Move,
    decltype(mcts_rollout),
    decltype(mcts_select),
    decltype(mcts_uct),
    decltype(mcts_expand),
    decltype(mcts_simulate),
    decltype(mcts_backpropagate)>;
}
