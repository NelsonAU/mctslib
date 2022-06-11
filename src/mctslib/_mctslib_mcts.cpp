#include <memory>
#include <pybind11/pybind11.h>


#include "algorithms/mcts.h"
#include "bindings/python_alg.h"
#include "bindings/python_node.h"

using namespace mctslib;
namespace py = pybind11;

#define MCTSLIB_REGISTER_MCTS_CLASS(cls, module)                    \
    py::class_<cls>(module, cls::str_id.c_str())                    \
        .def(py::init<double, int, py::object>())                   \
        .def("search_using_cpu_time", &cls::search_using_cpu_time)  \
        .def("search_using_iters", &cls::search_using_iters)        \
        .def("get_global_stats", &cls::get_global_stats)            \
        .def_readonly("current_node", &cls::current_node_ptr);      \


PYBIND11_MODULE(_mctslib_mcts, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring


    py::class_<MCTSStats>(m, "MCTSStats")
        .def_readonly("evaluation", &MCTSStats::evaluation)
        .def_readonly("action_id", &MCTSStats::action_id)
        .def_readonly("backprop_reward", &MCTSStats::backprop_reward)
        .def_readonly("visits", &MCTSStats::visits)
        .def("average_reward", &MCTSStats::average_reward);

    py::class_<PythonNode<MCTSStats>, std::shared_ptr<PythonNode<MCTSStats>>>(m, "MCTSNode")
        .def("children", &PythonNode<MCTSStats>::children, "")
        .def_readonly("state", &PythonNode<MCTSStats>::state)
        .def_readonly("stats", &PythonNode<MCTSStats>::stats);

    using MCTS_FFF = MCTS<PythonNode<MCTSStats>, false, false, false>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_FFF, m)

    using MCTS_FFT = MCTS<PythonNode<MCTSStats>, false, false, true>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_FFT, m)

    using MCTS_FTF = MCTS<PythonNode<MCTSStats>, false, true, false>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_FTF, m)

    using MCTS_FTT = MCTS<PythonNode<MCTSStats>, false, true, true>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_FTT, m)

    using MCTS_TFF = MCTS<PythonNode<MCTSStats>, true, false, false>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_TFF, m)

    using MCTS_TFT = MCTS<PythonNode<MCTSStats>, true, false, true>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_TFT, m)

    using MCTS_TTF = MCTS<PythonNode<MCTSStats>, true, true, false>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_TTF, m)

    using MCTS_TTT = MCTS<PythonNode<MCTSStats>, true, true, true>;
    MCTSLIB_REGISTER_MCTS_CLASS(MCTS_TTT, m)
}
