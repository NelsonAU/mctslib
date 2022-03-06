
#include <cmath>

#include <pybind11/pybind11.h>

#include "algorithm/hrave.h"
#include "algorithm/pyalg.h"
#include "body/body.h"
#include "move/move.h"
#include "node/python_node.h"
#include "settings/settings.h"
#include "stats/stats.h"
#include "util/empty.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib_hrave, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.
    //
    //
    using PyTreeIterDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, false>>;
    py::class_<PyTreeIterDenseHrave>(m, "tree_iters_dense_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyTreeIterDenseHrave::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    /* using PyTreeCPUDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, false, false>>; */
    /* py::class_<PyTreeCPUDenseHrave>(m, "tree_cpu_time_dense_HRAVE") */
    /*     .def(py::init<py::object, uint, uint>()) */
    /*     .def("move", &PyTreeCPUDenseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyTreeIterRNGDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_iter_move_rng), IterMCTSSettings, false, false>>; */
    /* py::class_<PyTreeIterRNGDenseHrave>(m, "tree_iters_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint, uint>()) */
    /*     .def("move", &PyTreeIterRNGDenseHrave::move<uint, uint, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyTreeCPURNGDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, false, false>>; */
    /* py::class_<PyTreeCPURNGDenseHrave>(m, "tree_cpu_time_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint, uint>()) */
    /*     .def("move", &PyTreeCPURNGDenseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGIterDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, true, false>>; */
    /* py::class_<PyDAGIterDenseHrave>(m, "dag_iters_dense_HRAVE") */
    /*     .def(py::init<py::object, uint, uint>()) */
    /*     .def("move", &PyDAGIterDenseHrave::move<uint, uint, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGCPUDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, true, false>>; */
    /* py::class_<PyDAGCPUDenseHrave>(m, "dag_cpu_time_dense_HRAVE") */
    /*     .def(py::init<py::object, uint, uint>()) */
    /*     .def("move", &PyDAGCPUDenseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGIterRNGDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_iter_move_rng), IterMCTSSettings, true, false>>; */
    /* py::class_<PyDAGIterRNGDenseHrave>(m, "dag_iters_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint, uint>()) */
    /*     .def("move", &PyDAGIterRNGDenseHrave::move<uint, uint, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGCPURNGDenseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, true, false>>; */
    /* py::class_<PyDAGCPURNGDenseHrave>(m, "dag_cpu_time_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint, uint>()) */
    /*     .def("move", &PyDAGCPURNGDenseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyTreeIterSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, false, true>>; */
    /* py::class_<PyTreeIterSparseHrave>(m, "tree_iters_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyTreeIterSparseHrave::move<uint, uint, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyTreeCPUSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, false, true>>; */
    /* py::class_<PyTreeCPUSparseHrave>(m, "tree_cpu_time_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyTreeCPUSparseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyTreeIterRNGSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_iter_move_rng), IterMCTSSettings, false, true>>; */
    /* py::class_<PyTreeIterRNGSparseHrave>(m, "tree_iters_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyTreeIterRNGSparseHrave::move<uint, uint, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyTreeCPURNGSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, false, true>>; */
    /* py::class_<PyTreeCPURNGSparseHrave>(m, "tree_cpu_time_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyTreeCPURNGSparseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGIterSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, true, true>>; */
    /* py::class_<PyDAGIterSparseHrave>(m, "dag_iters_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyDAGIterSparseHrave::move<uint, uint, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGCPUSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, true, true>>; */
    /* py::class_<PyDAGCPUSparseHrave>(m, "dag_cpu_time_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyDAGCPUSparseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGIterRNGSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_iter_move_rng), IterMCTSSettings, true, true>>; */
    /* py::class_<PyDAGIterRNGSparseHrave>(m, "dag_iters_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyDAGIterRNGSparseHrave::move<uint, uint, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2)); */

    /* using PyDAGCPURNGSparseHrave = PyAlg<HRAVE<PythonNode<MCTSStats<Empty>>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, true, true>>; */
    /* py::class_<PyDAGCPURNGSparseHrave>(m, "dag_cpu_time_randomized_ties_dense_HRAVE") */
    /*     .def(py::init<py::object, uint>()) */
    /*     .def("move", &PyDAGCPURNGSparseHrave::move<uint, double, double>, "", py::kw_only(), */
    /*         py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2)); */
}
