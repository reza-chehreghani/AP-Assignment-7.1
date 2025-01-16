#include "futballFantasy.hpp"

FutballFantasy::FutballFantasy()
    : is_admin_registered(false), is_transfer_window_open(TRASFER_WINDOW_BEGINING_STATUS) {}

ostringstream FutballFantasy::do_order(istringstream &input) {
    string order;
    input >> order;
    if (order == "POST_pass_week") //*/*/*/*/*/*/*/*/*/
        return premier_league.POST_pass_week();
    else if (order == "POST")
        return do_POST_order(input);
    else if (order == "GET")
        return do_GET_order(input);
    else if (order == "PUT")
        return do_PUT_order(input);
    else if (order == "DELETE")
        return do_DELETE_order(input);
    else
        throw BadRequest("the first part of the input is invalid.");
}

ostringstream FutballFantasy::do_POST_order(istringstream &input) {
    string order;
    input >> order;
    
    if (order == "signup")
        return fantasy_league.POST_signup(input, is_admin_registered);
    else if (order == "login")
        return fantasy_league.POST_login(input, is_admin_registered);
    else if (order == "register_admin")
        return POST_register_admin(input);
    else if (order == "logout")
        return POST_logout();
    else if (order == "sell_player")
        return fantasy_league.POST_sell_player(input, is_transfer_window_open);
    else if (order == "buy_player")
        return fantasy_league.POST_buy_player(input, is_transfer_window_open, &premier_league);
    else if (order == "close_transfer_window")
        return POST_close_transfer_window();
    else if (order == "open_transfer_window")
        return POST_open_transfer_window();
    else if (order == "pass_week")
        return POST_pass_week();
    else
        throw NotFound("POST order not found.");

    abort();
}

ostringstream FutballFantasy::do_GET_order(istringstream &input) {
    string order;
    input >> order;

    if (order == "team_of_the_week")
        return premier_league.GET_team_of_the_week(input);
    else if (order == "players")
        return premier_league.GET_players(input);
    else if (order == "league_standings")
        return premier_league.GET_league_standings();
    else if (order == "users_ranking")
        return fantasy_league.GET_users_ranking();
    else if (order == "matches_result_league")
        return premier_league.GET_matches_result_league(input);
    else if (order == "squad")
        return fantasy_league.GET_squad(input);
    else
        throw NotFound("GET order not found.");
    abort();
}

ostringstream FutballFantasy::do_PUT_order(istringstream &input) {
    abort();
}

ostringstream FutballFantasy::do_DELETE_order(istringstream &input) {
    abort();
}

ostringstream FutballFantasy::POST_register_admin(istringstream &input) {
    if (is_admin_registered || fantasy_league.is_anyone_logged())
        throw BadRequest("admin can't register because another user logged");    

    Tools::check_question_mark(input);

    if (Tools::get_input_word(input, "username") != ADMIN_USERNAME)
        throw BadRequest("wrong admin username");

    if (Tools::get_input_word(input, "password") != ADMIN_PASSWORD)
        throw BadRequest("wrong admin password");

    is_admin_registered = true;

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

ostringstream FutballFantasy::POST_logout() {
    if (is_admin_registered) {
        is_admin_registered = false;
        return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
    }

    return fantasy_league.POST_logout();
}

ostringstream FutballFantasy::POST_pass_week() {
    if (!is_admin_registered)
        throw PermissionDenied("admin don't registered so can't pass week");

    premier_league.POST_pass_week();
    fantasy_league.POST_pass_week();

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

ostringstream FutballFantasy::POST_close_transfer_window() {
    if (!is_admin_registered)
        throw PermissionDenied("admin isn't registered");

    is_transfer_window_open = false;

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

ostringstream FutballFantasy::POST_open_transfer_window() {
    if (!is_admin_registered)
        throw PermissionDenied("admin isn't registered");

    is_transfer_window_open = true;

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}