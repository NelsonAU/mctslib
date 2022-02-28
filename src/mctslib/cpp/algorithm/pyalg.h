#include "algorithm/algorithm.h"
#include <iostream>
#include <memory>
#include <pybind11/pybind11.h>

namespace mctslib {

template <
    class Alg>
class PyAlg {
private:
    Alg alg;

public:
    template <class... Args>
    PyAlg(pybind11::object root, Args... args)
        : alg(std::make_shared<typename Alg::Node>(root), args...)
    {
    }

    template <class... Args>
    pybind11::object move(Args... args)
    {
        static_assert(sizeof...(args) > 0, "Settings should not be default constructed in move!");
        using Settings = typename Alg::Settings;
        Settings settings { args... };
        return alg.move(alg, settings).object;
    }
};

}
