#include <pybind11/pybind11.h>

namespace mctslib {

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
    pybind11::object move(Args... args)
    {
        return alg.move(typename Alg::Settings(args...)).object;
    }

    static const char* str_id() requires requires(Alg alg) { Alg::str_id; }
    {
        return Alg::str_id.c_str();
    }
};
}
