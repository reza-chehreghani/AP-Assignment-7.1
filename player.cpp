#include "player.hpp"

Player::Player(string name_)
    : name(name_), /*score(0.0),*/ injured(false, -1), red_card(false), num_yellow_card(0) {}

void Player::POST_pass_week(const PlayersStatus &players_status) {
    last_week_played = true;

    set_week_score(players_status.players_score);

    set_injured(players_status.injureds);

    set_yellow_card(players_status.yellow_cards);

    set_red_card(players_status.red_cards);
}

bool Player::is_bitween(const vector<string> &names) {
    for (string name_ : names)
        if (name == name_)
            return true;

    return false;
}

void Player::set_week_score(const vector<PlayerScore> &players_score) {
    for (auto player_score : players_score)
        if (player_score.first == name) {
            // score += player_score.second;
            weeks_score.push_back(player_score.second);
            return;
        }

    weeks_score.push_back(0.f);
}

void Player::set_injured(const vector<string> &injureds) {
    if (injured.first == true) {
        last_week_played = false;
        if (weeks_score.size() - injured.second == NUM_OF_INJURED_WEEKS) {
            injured.first = false;
            return;
        }
    }

    if (this->is_bitween(injureds))
        injured = {true, weeks_score.size()};
}

void Player::set_yellow_card(const vector<string> &yellow_cards) {
    if (num_yellow_card == NUM_OF_YELLOW_CARD_FOR_SUSPENSION) {
        last_week_played = false;
        num_yellow_card = 0;
        return;
    }

    if (this->is_bitween(yellow_cards))
        num_yellow_card++;
}

void Player::set_red_card(const vector<string> &red_cards) {
    if (red_card == true) {
        last_week_played = false;
        red_card = false;
        return;
    }

    if (this->is_bitween(red_cards)) {
        red_card = true;
        num_yellow_card = 0;
    }
}

void GoalKeeper::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::goalkeeper)
        output_players.push_back(GET_player_info("gk"));
}

void Defender::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::defender)
        output_players.push_back(GET_player_info("df")); 
}

void Midfielder::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::midfielder)
        output_players.push_back(GET_player_info("md"));
}

void Forward::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::forward)
        output_players.push_back(GET_player_info("fw"));
}

PlayerInfo Player::GET_player_info(string post) {
    return {name, post, get_avarage_score()};
}

float Player::get_avarage_score() {
    float total_score = 0.f;
    int num_of_played_week = 0;
    for (auto week_score : weeks_score)
        if (week_score != 0.f) {
            total_score += week_score;
            num_of_played_week++;
        }

    if (total_score == 0.f)
        return total_score;

    // return Tools::round(total_score / num_of_played_week);
    return total_score / num_of_played_week; //*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
}

void GoalKeeper::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(team_of_the_week.goal_keeper, week_num);
}

void Defender::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(find_fewer(team_of_the_week.defender1, team_of_the_week.defender2), week_num);

    sort(team_of_the_week.defender1, team_of_the_week.defender2);
}

void Midfielder::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(team_of_the_week.mid_fielder, week_num);
}

void Forward::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(team_of_the_week.forward, week_num);
}

void Player::check_then_replace(PlayerScore &player_score, int week_num) {
    if (player_score.second < weeks_score[week_num] ||
        (player_score.second == weeks_score[week_num] && name < player_score.first)) {

        player_score.first = name;
        player_score.second = weeks_score[week_num];
    }
}

PlayerScore& Defender::find_fewer(PlayerScore& first, PlayerScore& second) {
    if (first.second < second.second)
        return first;

    return second;
}

void Defender::sort(PlayerScore& first, PlayerScore& second) {
    if (first.first > second.first) {
        PlayerScore first_copy(first);
        first = second;
        second = first_copy;
    }
}

bool Player::can_next_week_play() {
    if (red_card)
        return false;

    if (yellow_card == NUM_OF_YELLOW_CARD_FOR_SUSPENSION)
        return false;

    if (injured.first)
        return false;

    return true;
}

ostringstream GoalKeeper::if_can_buy(vector<Player*>& fantasy_players) {
    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Goalkeeper]);
}

ostringstream Defender::if_can_buy(vector<Player*>& fantasy_players) {
    try {
        return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Defender1]);
    } catch (...) {}

    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Defender2]);
}

ostringstream Midfielder::if_can_buy(vector<Player*>& fantasy_players) {
    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Midfielder]);
}

ostringstream Forward::if_can_buy(vector<Player*>& fantasy_players) {
    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Striker]);
}

ostringstream Player::if_can_buy(Player* &post_fantasy_player) {
    if (post_fantasy_player != nullptr)
        throw BadRequest("can't buy because this post is full");
    
    if (!can_next_week_play())
        return ostringstream(ANSWER_CAN_NOT_BUY_PLAYER_BECAUSE_CAN_NOT_PLAY_NEXT_WEEK);

    post_fantasy_player = this;
    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}