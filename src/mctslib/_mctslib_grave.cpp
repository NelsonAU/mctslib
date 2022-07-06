#include <pybind11/pybind11.h>

#include "algorithms/grave.h"
#include "bindings/python_alg.h"
#include "bindings/python_node.h"

using namespace mctslib;
namespace py = pybind11;

template<typename cls>
auto register_GRAVE(auto module) {
    return pybind11::class_<cls>(module, cls::str_id.c_str())
        .def(pybind11::init<double, int, int, int, pybind11::object>())
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
        .def("choose", &cls::choose)
        .def("get_global_stats", &cls::get_global_stats)
        .def_readonly("current_node", &cls::current_node_ptr);
}

PYBIND11_MODULE(_mctslib_grave, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    /* py::class_<RAVEStats>(m, "GRAVEStats") */
    /*     .def_readonly("evaluation", &RAVEStats::evaluation) */
    /*     .def_readonly("action_id", &RAVEStats::action_id) */
    /*     .def_readonly("backprop_reward", &RAVEStats::backprop_reward) */
    /*     .def_readonly("visits", &RAVEStats::visits) */
    /*     .def_readonly("amaf_stats", &RAVEStats::amaf_stats) */
    /*     .def("average_reward", &RAVEStats::average_reward) */
    /*     .def("amaf_average_reward", &RAVEStats::amaf_average_reward); */

    /* py::class_<PythonNode<RAVEStats>, std::shared_ptr<PythonNode<RAVEStats>>>(m, "GRAVENode") */
    /*     .def("children", &PythonNode<RAVEStats>::children, "") */
    /*     .def_readonly("state", &PythonNode<RAVEStats>::state) */
    /*     .def_readonly("stats", &PythonNode<RAVEStats>::stats); */


    register_GRAVE<GRAVE<PythonNode<RAVEStats>, false, false, false>>(m);
    register_GRAVE<GRAVE<PythonNode<RAVEStats>, false, false, true>>(m);
    register_GRAVE<GRAVE<PythonNode<RAVEStats>, false, true, false>>(m);
    register_GRAVE<GRAVE<PythonNode<RAVEStats>, false, true, true>>(m);

    register_GRAVE<GRAVE<PythonNode<RAVEStats>, true, false, false>>(m);
    register_GRAVE<GRAVE<PythonNode<RAVEStats>, true, false, true>>(m);
    register_GRAVE<GRAVE<PythonNode<RAVEStats>, true, true, false>>(m);
    register_GRAVE<GRAVE<PythonNode<RAVEStats>, true, true, true>>(m);

}
