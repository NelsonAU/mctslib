#include "util/empty.h"
#include <iostream>
#include <memory>
#include <pybind11/pybind11.h>
#include <random>
#include <type_traits>
#include <vector>

#pragma once

namespace mctslib {
template <class NodeStats>
class PythonNode {
    bool _been_expanded = false;
    static inline std::mt19937 rng;

public:
    pybind11::object object;
    NodeStats stats;
    std::vector<std::shared_ptr<PythonNode>> children;

    PythonNode(pybind11::object obj)
        : object(obj)
    {
        if constexpr (std::is_same_v<decltype(stats.action), Empty>) {
            stats = NodeStats { object.attr("evaluation")().cast<double>() };
        } else {
            stats = NodeStats { object.attr("evaluation")().cast<double>(), object.attr("action_id").cast<uint>() };
        }
    }

    template <typename... Args>
    PythonNode(pybind11::object obj, Args... args)
        : object(obj), stats(object.attr("evaluation")().cast<double>(), object.attr("action_id").cast<uint>(), args...)
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
        _been_expanded = true;
    }

    PythonNode default_policy() const
    {
        return PythonNode(object.attr("default_policy")());
    }

    bool is_terminal() const
    {
        return object.attr("is_terminal")().template cast<bool>();
    }

    bool been_expanded() const
    {
        return _been_expanded;
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
