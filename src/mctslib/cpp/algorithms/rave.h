#include "algorithms/mcts.h"
#include <memory>
#include <string>
#include <vector>


namespace mctslib {
    struct RAVEStats : public MCTSStats {
        uint action_id;
        std::vector<MCTSStats> amaf_stats;

        explicit RAVEStats() = default;
        RAVEStats(double reward, uint action_id, uint action_space_size) : MCTSStats(reward), action_id(action_id), amaf_stats(action_space_size, MCTSStats()){}
        
        double amaf_backprop_reward(uint action) {
            return amaf_stats.at(action).backprop_reward();
        }
    };

template<class Node, bool using_iters, bool using_dag, bool randomize_ties>
class RAVEBase : public MCTSBase<Node, using_iters, using_dag, randomize_ties> {
public:
    inline const static std::string alg_str = "RAVE";
    inline const static std::string str_id = MCTSBase<Node, using_iters, using_dag, randomize_ties>::opts_str + "_" + alg_str;

    const uint equivalence_param;
    const uint action_space_size;

    template <typename... Args>
    RAVEBase(uint action_space_size, uint equivalence_param, Args... args)
        : MCTSBase<Node, using_iters, using_dag, randomize_ties>(args..., action_space_size), 
        action_space_size(action_space_size), equivalence_param(equivalence_param)

}

}
