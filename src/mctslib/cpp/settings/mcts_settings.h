
struct CPUMCTSSettings {
    uint rollout_depth;
    double cpu_time;
    double exploration_weight;
    uint thread_count;
};

struct IterMCTSSettings {
    uint rollout_depth;
    uint iters;
    double exploration_weight;
    uint thread_count;
};
