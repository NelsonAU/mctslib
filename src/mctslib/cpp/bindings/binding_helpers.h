#include <pybind11/pybind11.h>

namespace mctslib {

template<typename cls>
auto register_mcts(auto module) {
    return pybind11::class_<cls>(module, cls::str_id.c_str())
        .def(pybind11::init<double, int, pybind11::object>())
        .def("search_using_cpu_time", &cls::search_using_cpu_time)
        .def("search_using_iters", &cls::search_using_iters)
        .def("get_global_stats", &cls::get_global_stats)
        .def_readonly("current_node", &cls::current_node_ptr);
}

}
