#include <pybind11/pybind11.h>

#pragma once
namespace mctslib {
struct NoAction {
    /* this is very hacky. Not sure what else to do as the compiler seems to give me no choice about */
    /* including an action type for MCTS. This at least makes it so the user can not specify an */
    /* action and get no errors when the algorithm doesn't need the action */
    NoAction() {}
    NoAction(pybind11::object obj) {}
};

template<typename Action>
struct MCTSStats {
    int visits;
    double evaluation; //TODO: make names more clear
    double backprop_value;
    Action action;


    MCTSStats() = default;
    MCTSStats(double evaluation, Action action) : 
        visits(1), evaluation(evaluation), backprop_value(evaluation), action(action) {}

    double avg_value () const {
        if (!visits) return -std::numeric_limits<double>::max();
        return backprop_value/visits;
    }

    void backprop (double value) {
        visits++;
        backprop_value += value;
    }
};

struct RAVEStats {
//TODO
};

struct GRAVEStats {
//TODO
};

}
