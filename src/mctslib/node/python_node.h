#include <iostream>
#include <random>

#include <pybind11/pybind11.h>

#include "mctslib/util/empty.h"

#pragma once

namespace mctslib {
template<class NodeStats>
class PythonNode {
    static inline std::mt19937 rng;
public:
    pybind11::object object;
    NodeStats stats;
    std::vector<PythonNode*> children;
    bool been_expanded = false;
   
    PythonNode(pybind11::object obj) : object(obj) {
        using Action = decltype(stats.action);
        if constexpr (std::is_same_v<Action, Empty>) {
            stats = NodeStats{object.attr("evaluation")().cast<double>()};
        } else {
            stats = NodeStats{object.attr("evaluation")().cast<double>(), object.attr("action").cast<Action>()};
        }
    }

    void create_children() {
        pybind11::list list = object.attr("find_children")();
        size_t length = pybind11::len(list);

        children.reserve(length);

        for (pybind11::handle child : list) {
            children.push_back(
                new PythonNode(pybind11::reinterpret_borrow<pybind11::object>(child))
            );
        }
        been_expanded = true;
    }

    PythonNode random_child() const {
        if (been_expanded) {
            std::uniform_int_distribution<size_t> dist {0, children.size() - 1};
            size_t idx = dist(rng);
            return *children[idx];
        }

        return PythonNode(object.attr("random_child")());
    }

    bool is_terminal() const {
        return object.attr("is_terminal")().template cast<bool>();
    }

    void print() const {
        pybind11::print(object);
    }

    friend bool operator< (const PythonNode& lhs, const PythonNode& rhs) {
        return lhs.object < rhs.object;
    }

    friend bool operator== (const PythonNode& lhs, const PythonNode& rhs) {
        return lhs.object.equal(rhs.object);
    }


};
}

namespace std {
    template<typename NodeStats>
    struct hash<mctslib::PythonNode<NodeStats>> {
        std::size_t operator()(mctslib::PythonNode<NodeStats> const& node) const {
            return (std::size_t) pybind11::hash(node.object);
        }
    };
}

