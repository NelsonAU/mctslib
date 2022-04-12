#include <iostream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
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
    bool expanded_ = false;
    // Important - we can't depend on the child nodes being in any particular order, or that
    // all available actions will have a corresponding child node.
    std::vector<std::shared_ptr<PythonNode>> children_;

public:
    pybind11::object object;
    Stats stats;

    PythonNode(pybind11::object obj, Stats stats)
        : object(obj)
        , stats(stats)
    {
    }

    // Used for all algorithms which require knowledge of which action was taken in the prior state
    // to produce this one. Used by all RAVE variants. Variadic template is required because the
    // signature of the Stats constructor will vary by algorithm.
    template <typename... Args>
    PythonNode(pybind11::object obj, Args... args)
        : object(obj),
          stats(obj.attr("evaluation")().cast<double>(), obj.attr("action_id").cast<uint>(), args...)
    {
    }

    template <typename... Args>
    std::shared_ptr<PythonNode> apply_action(uint action_id, Args... args) {
        pybind11::object resultant_obj = object.attr("apply_action")(action_id);
        return std::make_shared<PythonNode>(resultant_obj, args...);
    }

    // Used for simulations where the node will be discarded immediately, we only care about getting
    // the eval so we can backpropagate the result
    PythonNode apply_action_eval_only(uint action_id) {
        pybind11::object resultant_obj = object.attr("apply_action")(action_id);
        double eval = resultant_obj.attr("evaluation")().cast<double>();
        return PythonNode(resultant_obj, Stats::eval_only(eval, action_id));
    }

    std::vector<uint> get_legal_actions() {
        pybind11::list legal_action_list = object.attr("get_legal_actions")();

        return legal_action_list.cast<std::vector<uint>>();
    }

    void set_children(std::vector<std::shared_ptr<PythonNode>> children) {
        children_ = children;
        expanded_ = true;
    }

    std::vector<std::shared_ptr<PythonNode>>& children() {
        return children_;
    }

    bool is_terminal() const
    {
        return object.attr("is_terminal")().template cast<bool>();
    }

    bool is_expanded() const
    {
        return expanded_;
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
