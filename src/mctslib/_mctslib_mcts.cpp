#include <memory>
#include <pybind11/pybind11.h>


#include "algorithms/mcts.h"
#include "bindings/python_alg.h"
#include "bindings/python_node.h"

using namespace mctslib;
namespace py = pybind11;


template<typename cls>
auto register_MCTS(auto module) {
    return pybind11::class_<cls>(module, cls::str_id.c_str())
        .def(pybind11::init<double, int, pybind11::object>())
        .def("search_using_cpu_time", 
            [](cls& self, double cpu_time, int rollout_depth, double exp_weight) {
                auto node = self.search_using_cpu_time(cpu_time, rollout_depth, exp_weight);
                return pybind11::make_tuple(node, self.get_global_stats());
            }, "", pybind11::kw_only(),
            pybind11::arg("cpu_time"), pybind11::arg("rollout_depth"),
            pybind11::arg("exploration_weight")
        )
        .def("search_using_iters", 
            [](cls& self, int iters, int rollout_depth, double exp_weight) {
                auto node = self.search_using_iters(iters, rollout_depth, exp_weight);
                return pybind11::make_tuple(node, self.get_global_stats());
            }, "", pybind11::kw_only(),
            pybind11::arg("iters"), pybind11::arg("rollout_depth"),
            pybind11::arg("exploration_weight")
        )
        .def("get_global_stats", &cls::get_global_stats)
        .def_readonly("current_node", &cls::current_node_ptr);
}


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

    register_MCTS<MCTS<PythonNode<MCTSStats>, false, false, false>>(m);
    register_MCTS<MCTS<PythonNode<MCTSStats>, false, false, true>>(m);
    register_MCTS<MCTS<PythonNode<MCTSStats>, false, true, false>>(m);
    register_MCTS<MCTS<PythonNode<MCTSStats>, false, true, true>>(m);

    register_MCTS<MCTS<PythonNode<MCTSStats>, true, false, false>>(m);
    register_MCTS<MCTS<PythonNode<MCTSStats>, true, false, true>>(m);
    register_MCTS<MCTS<PythonNode<MCTSStats>, true, true, false>>(m);
    register_MCTS<MCTS<PythonNode<MCTSStats>, true, true, true>>(m);
}
