
#pragma once
namespace mctslib {

struct HRAVEStats {
    double evaluation;
    double backprop_value;
    uint visits;
    uint action;

    HRAVEStats() = default;

    HRAVEStats(double eval, uint action)
        : visits(0)
        , evaluation(eval)
        , action(action)
        , backprop_value(0)
    {
    }

    double avg_value() const
    {

        return visits ? backprop_value / visits : std::numeric_limits<double>::min();
    }
};

}