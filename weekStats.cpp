#include "weekStats.hpp"

ostringstream WeekStats::GET_matches_result_league() {
    if (matches.empty())
        throw NotFound("there is no match in this week");

    ostringstream output;
    for (auto match : matches)
        output << match.team1 << ' ' << match.team1_goals << " | "
               << match.team2 << ' ' << match.team2_goals << '\n';

    return output;
}