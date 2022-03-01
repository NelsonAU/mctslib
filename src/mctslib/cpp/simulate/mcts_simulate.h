#include <functional>
#include <memory>
#include <numeric>
#include <thread>
#include <vector>

namespace mctslib {
inline auto mcts_simulate = []<class Alg>(Alg& alg, std::shared_ptr<typename Alg::Node> start) -> double {
    typename Alg::Node node = *start;
    for (uint i = 0; i < alg.body.settings.rollout_depth; i++) {
        if (node.is_terminal())
            break;

        node = node.random_child();
    }

    return node.stats.evaluation;
};

/* inline auto threaded_mcts_simulate = []<class Alg>(Alg& alg, std::shared_ptr<typename Alg::Node> start) -> double { */
/*     // used for leaf parallelization */
/*     typename Alg::Node node = *start; */
/*     uint num_threads = alg.body.settings.thread_count; */
/*     std::vector<double> evals(num_threads, 0); */
/*     std::vector<std::thread> threads; */

/*     threads.reserve(alg.body.settings.thread_count); */

/*     for (double& eval : evals) { */
/*         threads.push_back( */
/*             std::thread( */
/*                 [node](uint t_rollout_depth, double& t_eval) { */
/*                     for (uint i = 0; i < t_rollout_depth; i++) { */
/*                         if (node.is_terminal()) */
/*                             break; */

/*                         node = node.random_child(); */
/*                     } */

/*                     return node.stats.evaluation; */
/*                 })); */
/*     } */

/*     for (std::thread& thread : threads) */
/*         thread.join(); */

/*     return std::reduce(evals.begin(), evals.end()) / num_threads; */
/* }; */
}
