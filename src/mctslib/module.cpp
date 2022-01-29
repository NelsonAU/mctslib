#include <pybind11/pybind11.h>

#include "mctslib/algorithm/mcts.h"
#include "mctslib/body/body.h"
#include "mctslib/move/mcts_iter_move.cpp"
#include "mctslib/node/python_node.h"

using namespace mctslib;

PYBIND11_MODULE(mctslibcpp, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    using IterMCTS = MCTS<MCTSBody, PythonNode, decltype(mcts_iter_move)>;

}
