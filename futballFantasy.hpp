#pragma once

#include "config.hpp"
#include "premierLeague.hpp"
#include "fantasyLeague.hpp"

class FutballFantasy {
    PremierLeague premier_league;
    FantasyLeague fantasy_league;
    bool is_admin_registered;
    bool is_transfer_window_open;

    ostringstream do_POST_order(istringstream &input);
    ostringstream do_GET_order(istringstream &input);
    ostringstream do_PUT_order(istringstream &input);
    ostringstream do_DELETE_order(istringstream &input);

    ostringstream POST_register_admin(istringstream &input);
    ostringstream POST_logout();

    ostringstream POST_pass_week();

    ostringstream POST_close_transfer_window();
    ostringstream POST_open_transfer_window();

public:
    FutballFantasy();

    ostringstream do_order(istringstream &input);
};