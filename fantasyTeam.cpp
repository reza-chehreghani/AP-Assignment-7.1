#include "fantasyTeam.hpp"

FantasyTeam::FantasyTeam(string name_, string password_) : name(name_), password(password_),
    point(0.0), players(NUM_OF_FANTASY_TEAM_PLAYERS, nullptr), num_of_sells(0), num_of_buys(0),
    has_played_any_game(false), has_passed_week(false) {}

void FantasyTeam::POST_login(string password_) {
    if (password != password_)
        throw PermissionDenied("incorrect password");
}

void FantasyTeam::POST_pass_week() {
    if (is_complete() && did_all_players_last_week_played()) {
        for (auto player : players)
            point += player->get_last_week_score();
    }

    if (is_complete() && did_all_players_last_week_played())
        has_played_any_game = true;

    num_of_sells = 0;
    num_of_buys = 0;
    has_passed_week = true;
}

bool FantasyTeam::is_complete() {
    return find(players.begin(), players.end(), nullptr) == players.end();
}

bool FantasyTeam::did_all_players_last_week_played() {
    for (auto player : players)
        if (!player->did_last_week_play())
            return false;

    return true;
}

pair<Name, Point> FantasyTeam::GET_user_ranking() {
    if (has_passed_week)
        return {name, point};
    
    return {NULL_STRING, 0.f};
}

ostringstream FantasyTeam::GET_squad() {
    if (!is_complete())
        return ostringstream(RESPONSE_TO_EMPTY_LIST);

    sort(players.begin() + Defender1, players.begin() + Defender2 + 1, [](Player* left, Player* right) {
        return left->get_name() < right->get_name();
    }); 

    return make_squad();    
}

ostringstream FantasyTeam::make_squad() {
    ostringstream output;
    output << "fantasy_team: " << name << '\n'
           << "Goalkeeper: " << players[goalkeeper]->get_name() << '\n'
           << "Defender1: " << players[Defender1]->get_name() << '\n'
           << "Defender2: " << players[Defender2]->get_name() << '\n'
           << "Midfielder: " << players[Midfielder]->get_name() << '\n'
           << "Striker: " << players[Striker]->get_name() << '\n'
           << "Total Points: " << fixed << setprecision(NUM_OF_DECIMAL_PLACE) << point << '\n';

    return output; 
}

ostringstream FantasyTeam::POST_sell_player(istringstream& input) {
    if (num_of_sells == MAX_PLAYER_TRANSFER && has_played_any_game)
        throw PermissionDenied("this week you can't sell more player");

    Tools::check_question_mark(input);

    find_fantasy_player(get_player_name(input)) = nullptr;

    num_of_sells++;

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

string FantasyTeam::get_player_name(istringstream& input) {
    string NAME, player_name = NULL_STRING;

    if (!(input >> NAME) || NAME != "name")
        throw BadRequest("invalid input");

    while(getline(input, player_name))
        ;

    if (player_name == NULL_STRING || player_name == " ")
        throw BadRequest("invalid input");

    player_name = player_name.substr(1);

    return player_name;
}

Player*& FantasyTeam::find_fantasy_player(string player_name) {
    for (auto& player : players)
        if (player != nullptr && player->is_name(player_name))
            return player;

    throw NotFound("this player doesn't exist to sell");
}

ostringstream FantasyTeam::POST_buy_player(istringstream &input, const PremierLeague* premier_league) {
    if (num_of_buys == MAX_PLAYER_TRANSFER && has_played_any_game)
        throw PermissionDenied("this week you can't buy more player");

    Tools::check_question_mark(input);

    return premier_league->find_player(get_player_name(input))->if_can_buy(players);
}