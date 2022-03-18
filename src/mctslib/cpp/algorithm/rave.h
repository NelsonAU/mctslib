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
#include <type_traits>

namespace mctslib {
template <
    class Node,
    class Move,
    class Settings,
    bool is_dag>
using RAVE = Algorithm<
    RAVEBody<Node, Settings, is_dag>,
    Move,
    decltype(mcts_rollout),
    decltype(mcts_select),
    decltype(rave_uct),
    typename std::conditional<is_dag, decltype(dag_rave_expand), decltype(tree_rave_expand)>::type,
    decltype(mcts_simulate),
    decltype(rave_backpropagate)>;
}
