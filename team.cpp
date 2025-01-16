#include "team.hpp"

Team::Team(string name_, vector<vector<string>> players_name)
    : name(name_), score(0), num_goals_for(0), num_goals_against(0) {
    init_players(players_name);
}

void Team::init_players(vector<vector<string>> posts_players_name) {
    init_posted_players<class::GoalKeeper>(posts_players_name[PlayerPost::goalkeeper]);
    init_posted_players<class::Defender>(posts_players_name[PlayerPost::defender]);
    init_posted_players<class::Midfielder>(posts_players_name[PlayerPost::midfielder]);
    init_posted_players<class::Forward>(posts_players_name[PlayerPost::forward]);
}

template<typename PlayerPost> void Team::init_posted_players(vector<string> posted_players_name) {
    for (auto posted_player_name : posted_players_name)
        players.push_back(make_shared<PlayerPost>(PlayerPost(posted_player_name)));
}

void Team::POST_pass_week(MatchResult match_result) {
    num_goals_for += match_result.num_goals_for;
    num_goals_against += match_result.num_goals_against;

    if (match_result.num_goals_for > match_result.num_goals_against)
        score += WIN_POINTS;
    else if (match_result.num_goals_for == match_result.num_goals_against)
        score += EQUAL_POINTS;

    for (auto& player : players)
        player->POST_pass_week({match_result.injureds, match_result.yellow_cards, match_result.red_cards, match_result.players_score});
}

void Team::check_then_add_player(TeamOfTheWeek& team_of_the_week, int week_num) {
    for (auto player : players)
        player->check_then_add(team_of_the_week, week_num);
}

ostringstream Team::GET_players(Post_Ranks post_and_ranks) {
    vector<PlayerInfo> output_players = GET_post_players(post_and_ranks.first);

    if (post_and_ranks.second == true)
        sort(output_players.begin(), output_players.end(), &Team::compare_players_by_score);
    else
        sort(output_players.begin(), output_players.end(), &Team::compare_players_by_name);

    return make_list_of_players(output_players);
}

vector<PlayerInfo> Team::GET_post_players(PlayerPost player_post) {
    vector<PlayerInfo> output_players;

    for (auto& player : players)
        player->GET_player(output_players, player_post);

    return output_players;
}

bool Team::compare_players_by_score(const PlayerInfo &left, const PlayerInfo &right) {
    if (left.avg_score != right.avg_score)
        return left.avg_score > right.avg_score;

    return left.name < right.name;
}

bool Team::compare_players_by_name(const PlayerInfo &left, const PlayerInfo &right) {
    return left.name < right.name;
}

// string Team::extract_score_from_player_info(const string &player_info) {
//     string SCORE = "score: ";
//     size_t score_pos = player_info.find(SCORE) + SCORE.length();
//     size_t score_length = player_info.find_last_of(DOT) + NUM_OF_DECIMAL_PLACE - score_pos + 2;
//     string score = player_info.substr(score_pos, score_length);
//     score.pop_back();
//     return score;
// }

ostringstream Team::make_list_of_players(const vector<PlayerInfo>& players) {
    ostringstream output;
    output << "list of players:\n";
    for (int i = 0; i < players.size(); i++)
        output << to_string(i + 1) << ". name: " << players[i].name
               << " | role: " << players[i].role << " | score: "
               << fixed << setprecision(NUM_OF_DECIMAL_PLACE) << players[i].avg_score << '\n';
    
    return output;
}

TeamInfo Team::get_team_info() {
    return {name, score, num_goals_for, num_goals_against};
}

Player* Team::find_player(string player_name) const {
    for (auto player : players)
        if (player->is_name(player_name))
            return &*player;

    return nullptr;
}