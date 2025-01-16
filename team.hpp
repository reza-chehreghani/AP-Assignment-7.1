#pragma once

#include "config.hpp"
#include "player.hpp"

typedef void (Player:: *UpdateStatus)();
typedef void (Player:: *UpdateScore)(int);

typedef bool Ranks;
typedef pair<PlayerPost, Ranks> Post_Ranks;

struct MatchResult {
    int num_goals_for;
    int num_goals_against;
    vector<string> injureds;
    vector<string> yellow_cards;
    vector<string> red_cards;
    vector<PlayerScore> players_score;
};

struct TeamInfo {
    string name;
    int score;
    int num_goals_for;
    int num_goals_against;
};

class Team {
    string name;
    int score;
    int num_goals_for;
    int num_goals_against;
    vector<shared_ptr<Player>> players;

    void init_players(vector<vector<string>> posts_players_name);
    template<typename PlayerPost> void init_posted_players(vector<string> posted_players_name);

    vector<PlayerInfo> GET_post_players(PlayerPost player_post);
    static bool compare_players_by_score(const PlayerInfo &left, const PlayerInfo &right);
    static bool compare_players_by_name(const PlayerInfo &left, const PlayerInfo &right);
    // static string extract_score_from_player_info(const string &player_info);
    ostringstream make_list_of_players(const vector<PlayerInfo>& players);

public:
    Team(string name_, vector<vector<string>> players_name);
  
    void POST_pass_week(MatchResult match_result);

    bool is_name(string name_) { return name == name_; }

    void check_then_add_player(TeamOfTheWeek& team_of_the_week, int week_num);

    ostringstream GET_players(Post_Ranks post_and_ranks);

    TeamInfo get_team_info();

    Player* find_player(string player_name) const;
};