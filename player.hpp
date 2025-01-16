#pragma once

#include "config.hpp"
#include "tools.hpp"

enum PlayerPost {
    goalkeeper,
    defender,
    midfielder,
    forward,
    all
};

enum PlayerStatus {
    injured,
    yellow_card,
    red_card,
    score
};

typedef int WeekNum;

struct PlayersStatus {
    vector<string> injureds;
    vector<string> yellow_cards;
    vector<string> red_cards;
    vector<PlayerScore> players_score;
};

struct PlayerInfo {
    Name name;
    string role;
    float avg_score;
};

class Player {
    string name;
    // float score;
    vector<float> weeks_score;
    pair<bool, WeekNum> injured;
    int num_yellow_card;
    bool red_card;
    bool last_week_played;

    bool is_bitween(const vector<string> &names);

    void set_injured(const vector<string> &injureds);
    void set_yellow_card(const vector<string> &yellow_cards);
    void set_red_card(const vector<string> &red_cards);
    void set_week_score(const vector<PlayerScore> &players_score);

    float get_avarage_score();

protected:
    Player(string name_);

    PlayerInfo GET_player_info(string post);
    void check_then_replace(PlayerScore &player_score, int week_num);

    ostringstream if_can_buy(Player* &post_fantasy_player);

public:
    void POST_pass_week(const PlayersStatus &players_status);

    virtual void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) = 0;

    virtual void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) = 0;

    float get_last_week_score() { return *(weeks_score.end() - 1); }
    string get_name() { return name; }

    bool is_name(string name_) { return name == name_; }

    bool did_last_week_play() { return last_week_played; }
    bool can_next_week_play();

    virtual ostringstream if_can_buy(vector<Player*>& fantasy_players) = 0;
};

class GoalKeeper : public Player {


public:
    GoalKeeper(string name) : Player(name) {}

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players);
};

class Defender : public Player {
    PlayerScore& find_fewer(PlayerScore& first, PlayerScore& second);
    void sort(PlayerScore& first, PlayerScore& second);

public:
    Defender(string name) : Player(name) {}

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players);
};

class Midfielder : public Player {


public:
    Midfielder(string name) : Player(name) {}

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players);
};

class Forward : public Player {


public:
    Forward(string name) : Player(name) {}

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players);
};