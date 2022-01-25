#include <random>

#include <pybind11/pybind11.h>

namespace mctslib {
template<class NodeStats>
class PythonNode {
    static inline std::mt19937 rng;
public:
    pybind11::object object;
    NodeStats stats;
    std::vector<PythonNode*> children;
    bool been_expanded = false;
   
    PyNode(pybind11::object obj) : object(obj) {
        using Action = decltype(stats.action);
        if constexpr (std::is_same_v<Action, NoAction>) {
            stats {object.attr("evaluation")().cast<double>(), NoAction()};
        } else {
            stats {object.attr("evaluation")().cast<double>(), object.attr("action").cast<Action>()};
        }
    }

    void create_children() {
        pybind11::list list = object.attr("find_children")();
        size_t length = py::len(list);

        children.reserve(length);

        for (pybind11::handle child : list)
            children.push_back(new PythonNode(pybind11::reinterpret_borrow<pybind11::object(child)));
    }

    bool is_terminal() const {
        return object.attr("is_terminal")().template cast<bool>();
    }

    friend bool operator< (const PythonNode& lhs, const PythonNode& rhs) {
        return lhs.object < rhs.object;
    }

    friend bool operator== (const PythonNode& lhs, const PythonNode& rhs) {
        return lhs.object.equals(rhs.object);
    }

};
}

namespace std {
    template<typename NodeStats>
    struct hash<mctslib::PythonNode<NodeStats>> {
        std::size_t oeprator()(mctslib::PythonNode<NodeStats const& node) const {
            return (std::size_t) pybind11::hash(node.object);
        }
    }
}


