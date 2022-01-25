#include <pybind11/pybind11.h>

#include "mctslib/algorithm/mcts.h"
#include "mctslib/body/body.h"


PYBIND11_MODULE(mctslibcpp, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    using IterMCTS = MCTS<TreeMCTSBody, 

}
