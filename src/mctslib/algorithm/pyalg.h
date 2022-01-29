#include <pybind11/pybind11.h>
#include "mctslib/algorithm/algorithm.h"

namespace mctslib {

template<
    class Alg
>
class PyAlg {
private:
    Alg alg;

public:
    PyAlg (pybind11::object root) : alg(root) {}

    template<class... Args>
    pybind11::object pyMove(Args... args) {
        using Settings = typename Alg::Settings;

        Settings settings {args...};
        return alg.move(settings).object;
    }
};

}
