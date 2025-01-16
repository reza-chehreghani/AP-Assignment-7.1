#include "fantasyLeague.hpp"

FantasyLeague::FantasyLeague() : logged_team(nullptr) {}

ostringstream FantasyLeague::POST_signup(istringstream &input, bool is_admin_registered) {
    if (logged_team != nullptr || is_admin_registered)
        throw BadRequest("Another user is currently logged");

    Tools::check_question_mark(input);

    string team_name = Tools::get_input_word(input, "team_name");
    check_team_name_duplication(team_name);

    fantasy_teams.push_back(FantasyTeam(team_name, Tools::get_input_word(input, "password")));

    logged_team = &(*(fantasy_teams.end() - 1));
    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

void FantasyLeague::check_team_name_duplication(string team_name) {
    for (auto fantasy_team : fantasy_teams)
        if (fantasy_team.is_name(team_name))
            throw BadRequest("this team name is currently exist");
}

ostringstream FantasyLeague::POST_login(istringstream &input, bool is_admin_registered) {
    if (logged_team != nullptr || is_admin_registered)
        throw BadRequest("Another user is currently logged");

    Tools::check_question_mark(input);
    
    string team_name = Tools::get_input_word(input, "team_name");

    FantasyTeam* fantasy_team = find_fantasy_team(team_name);
    fantasy_team->POST_login(Tools::get_input_word(input, "password"));

    logged_team = fantasy_team;
    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

FantasyTeam* FantasyLeague::find_fantasy_team(string team_name) {
    for (auto& fantasy_team : fantasy_teams)
        if (fantasy_team.is_name(team_name))
            return &fantasy_team;

    throw NotFound("team name not found");
}

ostringstream FantasyLeague::POST_logout() {
    check_login();

    logged_team = nullptr;
    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

void FantasyLeague::check_login() {
    if (logged_team == nullptr)
        throw PermissionDenied("there is no user looged");
}

ostringstream FantasyLeague::GET_users_ranking() {
    vector<pair<Name, Point>> users_rank = get_users_ranking();
    
    sort(users_rank.begin(), users_rank.end(), &FantasyLeague::compare_users);

    return make_users_ranking(users_rank);
}

vector<pair<Name, Point>> FantasyLeague::get_users_ranking() {
    vector<pair<Name, Point>> users_rank;

    for (auto& fantasy_team : fantasy_teams) {
        pair<Name, Point> user_ranking = fantasy_team.GET_user_ranking();
        
        if (user_ranking.first != NULL_STRING)
            users_rank.push_back(user_ranking);
    }

    return users_rank;
}

bool FantasyLeague::compare_users(const pair<Name, Point>& left, const pair<Name, Point>& right) {
    if (left.second != right.second)
        return left.second > right.second;

    return left.first < right.first;
}

ostringstream FantasyLeague::make_users_ranking(vector<pair<Name, Point>> users_rank) {
    if (users_rank.size() == 0)
        return ostringstream(RESPONSE_TO_EMPTY_LIST);

    ostringstream output;
    for (int i = 0; i < users_rank.size(); i++)
        output << to_string(i + 1) << ". team_name: " << users_rank[i].first << " | point: "
               << fixed << setprecision(NUM_OF_DECIMAL_PLACE) << users_rank[i].second << '\n';

    return output;
}

ostringstream FantasyLeague::POST_pass_week() {
    for (auto& fantasy_team : fantasy_teams)
        fantasy_team.POST_pass_week();

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

ostringstream FantasyLeague::GET_squad(istringstream& input) {
    check_login();

    Tools::check_question_mark(input);

    return get_fantasy_team(input)->GET_squad();
}

FantasyTeam* FantasyLeague::get_fantasy_team(istringstream& input) {
    string FANTASY_TEAM, fantasy_team_name;
    
    if (!(input >> FANTASY_TEAM))
        return logged_team;

    if (FANTASY_TEAM != "fantasy_team" || !(input >> fantasy_team_name))
        throw BadRequest("invalid input");

    return find_fantasy_team(fantasy_team_name);
}

ostringstream FantasyLeague::POST_sell_player(istringstream &input, bool is_transfer_window_open) {
    check_login();

    if (!is_transfer_window_open)
        throw PermissionDenied("transfer window is closed");

    return logged_team->POST_sell_player(input);
}

ostringstream FantasyLeague::POST_buy_player(istringstream &input, bool is_transfer_window_open, const PremierLeague* premier_league) {
    check_login();
    
    if (!is_transfer_window_open)
        throw PermissionDenied("transfer window is closed");

    return logged_team->POST_buy_player(input, premier_league);
}