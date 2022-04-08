#include <iostream>
#include <memory>
#include <pybind11/pybind11.h>
#include <random>
#include <type_traits>
#include <vector>

#pragma once

namespace mctslib {
// Node Class responsible for calling out to an underlying Python object which contains methods
// for interacting with an environment. Accepts a Stats type as a template paramter which is used
// to hold all statistics relevant to the algorithm being used.
template <class Stats>
class PythonNode {
    bool expanded = false;

public:
    pybind11::object object;
    Stats stats;
    // Important - we can't depend on the child nodes being in any particular order, or that
    // all available actions will have a corresponding child node.
    std::vector<std::shared_ptr<PythonNode>> children;

    PythonNode(pybind11::object obj, Stats stats)
        : object(obj)
        , stats(stats)
    {
    }

    // Used for all algorithms which require knowledge of which action was taken in the prior state
    // to produce this one. Used by all RAVE variants. Variadic template is required because the
    // signature of the Stats constructor will vary by algorithm.
    template <typename... Args>
    PythonNode(pybind11::object obj, Args... args) requires requires(Stats stats) { stats.action_id; }
        : object(obj),
          stats(obj.attr("evaluation")().cast<double>(), obj.attr("action_id").cast<uint>(), args...)
    {
    }

    // Used for algorithms which may not require an action to inform the stats they collect, so the
    // user won't need to provide an action_id on the Python object. Used by MCTS. Variadic template
    // is required because the signature of the Stats constructor will vary by algorithm.
    template <typename... Args>
    PythonNode(pybind11::object obj, Args... args)
        : object(obj)
        , stats(obj.attr("evaluation")().cast<double>(), args...)
    {
    }

    template <typename... Args>
    void create_children(Args... args)
    {
        pybind11::list list = object.attr("find_children")();
        size_t length = pybind11::len(list);

        children.reserve(length);

        for (pybind11::handle child : list) {
            children.push_back(
                std::make_shared<PythonNode>(pybind11::reinterpret_borrow<pybind11::object>(child), args...));
        }
        expanded = true;
    }

    PythonNode default_policy() const
    {
        return PythonNode(object.attr("default_policy")(), Stats());
    }

    bool is_terminal() const
    {
        return object.attr("is_terminal")().template cast<bool>();
    }

    bool is_expanded() const
    {
        return expanded;
    }

    void print() const
    {
        pybind11::print(object);
    }

    friend bool operator<(const PythonNode& lhs, const PythonNode& rhs)
    {
        return lhs.object < rhs.object;
    }

    friend bool operator==(const PythonNode& lhs, const PythonNode& rhs)
    {
        return lhs.object.equal(rhs.object);
    }
};
}

namespace std {
template <typename NodeStats>
struct hash<mctslib::PythonNode<NodeStats>> {
    std::size_t operator()(mctslib::PythonNode<NodeStats> const& node) const
    {
        return (std::size_t)pybind11::hash(node.object);
    }
};
}
