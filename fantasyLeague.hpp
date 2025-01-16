#pragma once

#include "config.hpp"
#include "fantasyTeam.hpp"

class FantasyLeague {
    vector<FantasyTeam> fantasy_teams;
    FantasyTeam* logged_team;

    void check_team_name_duplication(string team_name);

    FantasyTeam* find_fantasy_team(string team_name);

    vector<pair<Name, Point>> get_users_ranking();
    static bool compare_users(const pair<Name, Point>& left, const pair<Name, Point>& right);
    ostringstream make_users_ranking(vector<pair<Name, Point>> users_rank);

    FantasyTeam* get_fantasy_team(istringstream& input);
    
public:
    FantasyLeague();

    ostringstream POST_signup(istringstream &input, bool is_admin_registered);

    ostringstream POST_login(istringstream &input, bool is_admin_registered);

    bool is_anyone_logged() { return logged_team; }

    ostringstream POST_logout();
    void check_login();

    ostringstream GET_users_ranking();

    ostringstream POST_pass_week();

    ostringstream GET_squad(istringstream& input);

    ostringstream POST_sell_player(istringstream &input, bool is_transfer_window_open);

    ostringstream POST_buy_player(istringstream &input, bool is_transfer_window_open, const PremierLeague* premier_league);
};