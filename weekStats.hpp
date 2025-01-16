#pragma once

#include "config.hpp"

struct Match {
    string team1;
    string team2;
    int team1_goals;
    int team2_goals;
};

class WeekStats {
    vector<Match> matches;

public:
    WeekStats(vector<Match> matches_) : matches(matches_) {}

    ostringstream GET_matches_result_league();
};