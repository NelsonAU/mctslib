#include <iostream>
#include <memory>
#include <pybind11/pybind11.h>
#include <random>
#include <type_traits>
#include <vector>

#pragma once

namespace mctslib {
template <class Stats>
class PythonNode {
    bool expanded = false;
    static inline std::mt19937 rng;

public:
    pybind11::object object;
    Stats stats;
    std::vector<std::shared_ptr<PythonNode>> children;

    PythonNode(pybind11::object obj, Stats stats)
        : object(obj)
        , stats(stats)
    {
    }

    template <typename... Args>
    PythonNode(pybind11::object obj, Args... args) requires requires(Stats stats) { stats.action_id; }
        : object(obj),
          stats(obj.attr("evaluation")().cast<double>(), obj.attr("action_id").cast<uint>(), args...)
    {
    }

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
