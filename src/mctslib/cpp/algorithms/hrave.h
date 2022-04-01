#include "algorithms/mcts.h"


namespace mctslib {

struct HRAVEStats : MCTSStats {
    uint action;

    explicit HRAVEStats() = default;
    HRAVEStats(uint action, double reward) : MCTSStats(reward), action(action) {}
};


template <class Node, bool using_iters, bool using_dag, bool randomize_ties>
class HRAVEBase : public MCTSBase<Node, using_iters, using_dag, randomize_ties> {
    std::vector<MCTSStats> global_amafs;

    template<typename... Args>
    HRAVEBase(uint action_space_size, Args... args) 
        : global_amafs(action_space_size, MCTSStats()),
        MCTSBase<Node, using_iters, using_dag, randomize_ties>(args...) {}
};


template <class Node, bool using_iters, bool using_dag, bool randomized_ties>
class HRAVE final : public HRAVEBase<Node, using_iters, using_dag, randomized_ties> {
    using HRAVEBase<Node, using_iters, using_dag, randomized_ties>::HRAVEBase;
};

}
