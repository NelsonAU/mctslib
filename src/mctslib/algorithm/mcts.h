#include "mctslib/algorithm/algorithm.h"
#include "mctslib/backpropagate/backpropagate.h"
#include "mctslib/body/body.h"
#include "mctslib/expand/expand.h"
#include "mctslib/move/move.h"
#include "mctslib/rollout/rollout.h"
#include "mctslib/select/select.h"
#include "mctslib/simulate/simulate.h"
#include "mctslib/stats/stats.h"
#include "mctslib/uct/uct.h"


template<template<template<class> class, class, class> class BodyTemplate, class Move>
using MCTS = Algorithm<
    BodyTemplate<MCTS
    Move,
    decltype(mcts_rollout),
    decltype(mcts_select),
    decltype(mcts_uct),
    decltype(mcts_expand),
    decltype(mcts_simulate),
    decltype(mcts_backpropagate)
>;
