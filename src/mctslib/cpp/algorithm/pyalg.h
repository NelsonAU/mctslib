#include <pybind11/pybind11.h>
#include "algorithm/algorithm.h"
#include <iostream>
namespace mctslib {

template<
    class Alg
>
class PyAlg {
private:
    Alg alg;

public:
    template<class... Args>
    PyAlg (pybind11::object root, Args... args) : alg(new typename Alg::Node(root), args...) {}

    template<class... Args>
    pybind11::object move(Args... args) {
        static_assert(sizeof...(args) > 0, "Settings should not be default constructed in move!");
        using Settings = typename Alg::Settings;
        std::cout << __LINE__ << std::endl;
        Settings settings {args...};
        return alg.move(alg, settings).object;
    }
};

}
