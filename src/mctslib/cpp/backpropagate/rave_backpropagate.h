#include <memory>
#include <vector>

namespace mctslib {

inline auto rave_backpropagate = []<class Alg>(Alg& alg, const auto& path, const double value) -> void {
    std::vector<bool> seen_actions {alg.body.action_space_size};

    for (auto it = path.rbegin(); it != path.rend(); it++) {
        std::shared_ptr<typename Alg::Node> node_ptr = *it;
        seen_actions.at(node_ptr->stats.action) = true;

        for (uint i = 0; i < seen_actions.size(); i++) { //backprop for AMAF values
            if (seen_actions.at(i)) {
                node_ptr->stats.amaf_stats.at(i).visits++;
                node_ptr->stats.amaf_stats.at(i).backprop_value += value;
            }
        }

        node_ptr->stats.visits++;
        node_ptr->stats.backprop_value += value;
    }
};
}
