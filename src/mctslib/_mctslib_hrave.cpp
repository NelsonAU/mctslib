#include <pybind11/pybind11.h>

#include "algorithms/hrave.h"
#include "nodes/python_node.h"

using namespace mctslib;

template<typename cls>
auto register_HRAVE(auto module) {
    return pybind11::class_<cls>(module, cls::str_id.c_str())
        .def(pybind11::init<double, int, int, pybind11::object>())
        .def("search_using_cpu_time", 
            [](cls& self, double cpu_time, int rollout_depth, double exp_weight) {
                auto node = self.search_using_cpu_time(cpu_time, rollout_depth, exp_weight);
                return pybind11::make_tuple(node, node->children(), self.get_global_stats());
            }, "", pybind11::kw_only(),
            pybind11::arg("cpu_time"), pybind11::arg("rollout_depth"),
            pybind11::arg("exploration_weight")
        )
        .def("search_using_iters", 
            [](cls& self, int iters, int rollout_depth, double exp_weight) {
                auto node = self.search_using_iters(iters, rollout_depth, exp_weight);
                return pybind11::make_tuple(node, node->children(), self.get_global_stats());
            }, "", pybind11::kw_only(),
            pybind11::arg("iters"), pybind11::arg("rollout_depth"),
            pybind11::arg("exploration_weight")
        )
        .def("choose_best_node", &cls::choose_best_node)
        .def("choose_node", &cls::choose_node)
        .def("get_global_stats", &cls::get_global_stats)
        .def_readonly("current_node", &cls::current_node_ptr);
}

PYBIND11_MODULE(_mctslib_hrave, m)
{
    m.doc() = "This module contains implementations of HRAVE for mctslib.";


    pybind11::class_<AMAFStats>(m, "AMAFStats")
        .def_readonly("evaluation", &AMAFStats::evaluation)
        .def_readonly("backprop_reward", &AMAFStats::backprop_reward)
        .def_readonly("visits", &AMAFStats::visits)
        .def("average_reward", &AMAFStats::average_reward);


    register_HRAVE<HRAVE<PythonNode<MCTSStats>, false, false, false>>(m);
    register_HRAVE<HRAVE<PythonNode<MCTSStats>, false, false, true>>(m);
    register_HRAVE<HRAVE<PythonNode<MCTSStats>, false, true, false>>(m);
    register_HRAVE<HRAVE<PythonNode<MCTSStats>, false, true, true>>(m);

    register_HRAVE<HRAVE<PythonNode<MCTSStats>, true, false, false>>(m);
    register_HRAVE<HRAVE<PythonNode<MCTSStats>, true, false, true>>(m);
    register_HRAVE<HRAVE<PythonNode<MCTSStats>, true, true, false>>(m);
    register_HRAVE<HRAVE<PythonNode<MCTSStats>, true, true, true>>(m);
}
