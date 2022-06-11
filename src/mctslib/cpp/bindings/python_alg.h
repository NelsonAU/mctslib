#include <pybind11/pybind11.h>
#include <string>

namespace mctslib {

// Wraps a mctslib Algorithm such that it can be easily used from Python. Expects that each
// algorithm will require a new Settings object in order to move, and that we are using the
// associated PythonNode template found in this directory.
template <class Alg>
class PyAlg {
    Alg alg;

public:
    template <typename... Args>
    PyAlg(Args... args)
        : alg(args...)
    {
    }

    template <typename... Args>
    pybind11::tuple search(std::string iters_or_cpu_time, Args... args)
    {
        if (iters_or_cpu_time == "iters") {

        } else if (iters_or_cpu_time == "cpu_time") {

        } else {

        }
        auto node = alg.move(typename Alg::Settings(args...));
        return pybind11::make_tuple(node, alg.get_global_stats());
    }

    // Used to get the string that will be used to identify the algorithm in the outputted Python
    // module. Each instantiated algorithm should have a unique string based on the algorithm and
    // all compile-time known settings (e.g. using_iters, randomize_ties, etc.).
    static const char* str_id() requires requires(Alg alg) { Alg::str_id; }
    {
        return Alg::str_id.c_str();
    }
};
}
