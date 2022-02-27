
struct CPUMCTSSettings {
    uint rollout_depth;
    double cpu_time;
    double exploration_weight;
};

struct IterMCTSSettings {
    uint rollout_depth;
    uint iters;
    double exploration_weight;
};
