#pragma once

#include "config.hpp"
#include "team.hpp"
#include "weekStats.hpp"

class PremierLeague {
    vector<Team> teams;
    vector<WeekStats> weeks_stats;
    
    void init_teams();
    Team make_team(vector<string> team_elements);

    Match make_match(string teams_name, string result);
    void update_match_teams(Match match, vector<string> match_players_elemets);
    vector<PlayerScore> make_players_score(vector<string> players_score_elements);
    Team& find_team(string name);

    int get_week_num(istringstream &input);
    ostringstream make_team_of_week(TeamOfTheWeek team_of_the_week);

    Team& get_team(istringstream &input);
    Post_Ranks get_post_and_ranks(istringstream &input);
    PlayerPost find_post(string post);

    static bool compare_teams(const TeamInfo& left, const TeamInfo& right);
    ostringstream make_league_standings(const vector<TeamInfo>& teams_info);

public:
    PremierLeague();

    ostringstream GET_team_of_the_week(istringstream &input);

    ostringstream GET_players(istringstream &input);

    ostringstream GET_league_standings();

    ostringstream GET_matches_result_league(istringstream &input);

    ostringstream POST_pass_week();

    Player* find_player(string player_name) const;
};