#include "premierLeague.hpp"

PremierLeague::PremierLeague() {
    init_teams();
}

void PremierLeague::init_teams() {
    vector<vector<string>> teams_elements = Tools::read_csv(PREMIER_LEAGUE_FILE_DIRECTORY);

    for (auto team_elements : teams_elements)
        teams.push_back(make_team(team_elements));
}

Team PremierLeague::make_team(vector<string> team_elements) {
    vector<vector<string>> posts_players_name;

    for (int post = PlayerPost::goalkeeper; post <= PlayerPost::forward; post++)
        posts_players_name.push_back(Tools::split_words(team_elements[post + CSV_OFFSET_PLAYERS], CSV_DELIMITER_FOR_COLUMN_WITH_MORE_THAN_ONE_ELEMENTS));

    return Team(team_elements[CSV_NUM_COL_TEAM_NAME], posts_players_name);
}

ostringstream PremierLeague::POST_pass_week() {
    vector<vector<string>> week_stats_elements = Tools::read_csv(Tools::get_file_directory_week(weeks_stats.size()));
    vector<Match> matches;

    for (auto match_elemets : week_stats_elements) {
        Match match = make_match(match_elemets[CSV_NUM_COL_TEAMS], match_elemets[CSV_NUM_COL_RESULTS]);
        update_match_teams(match, vector<string>(match_elemets.begin() + CSV_NUM_COL_INJURED, match_elemets.end()));
        matches.push_back(match);
    }

    weeks_stats.push_back(WeekStats(matches));

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

Match PremierLeague::make_match(string teams_name_, string result) {
    vector<string> teams_name = Tools::split_words(teams_name_, CSV_DELIMITER_FOR_MATCH_TEAMS_NAME);
    vector<string> teams_result = Tools::split_words(result, CSV_DELIMITER_FOR_MATCH_TEAMS_RESULT);

    return {teams_name[first], teams_name[second], stoi(teams_result[first]), stoi(teams_result[second])};
}

void PremierLeague::update_match_teams(Match match, vector<string> match_players_elemets) {
    vector<string> injureds = Tools::split_words(match_players_elemets[PlayerStatus::injured], CSV_DELIMITER_FOR_COLUMN_WITH_MORE_THAN_ONE_ELEMENTS);
    vector<string> yellow_cards = Tools::split_words(match_players_elemets[PlayerStatus::yellow_card], CSV_DELIMITER_FOR_COLUMN_WITH_MORE_THAN_ONE_ELEMENTS);
    vector<string> red_cards = Tools::split_words(match_players_elemets[PlayerStatus::red_card], CSV_DELIMITER_FOR_COLUMN_WITH_MORE_THAN_ONE_ELEMENTS);
    vector<PlayerScore> players_score = make_players_score(Tools::split_words(match_players_elemets[PlayerStatus::score], CSV_DELIMITER_FOR_COLUMN_WITH_MORE_THAN_ONE_ELEMENTS));

    find_team(match.team1).POST_pass_week({match.team1_goals, match.team2_goals, injureds, yellow_cards, red_cards, players_score});
    find_team(match.team2).POST_pass_week({match.team2_goals, match.team1_goals, injureds, yellow_cards, red_cards, players_score});
}

vector<PlayerScore> PremierLeague::make_players_score(vector<string> players_score_elements) {
    vector<PlayerScore> players_scores;
    for (auto player_score_elements : players_score_elements) {
        vector<string> elements = Tools::split_two_word(player_score_elements, CSV_DELIMITER_FOR_PLAYER_SCORE);
        players_scores.push_back({elements[first], stof(elements[second])});
    }
    return players_scores;
}

Team& PremierLeague::find_team(string name) {
    for (auto &team : teams)
        if (team.is_name(name))
            return team;
    
    throw NotFound("team_name not found");
}

ostringstream PremierLeague::GET_team_of_the_week(istringstream &input) {
    Tools::check_question_mark(input);

    int week_num = get_week_num(input);

    TeamOfTheWeek team_of_the_week;
    for (auto &team : teams)
        team.check_then_add_player(team_of_the_week, week_num);

    return make_team_of_week(team_of_the_week);
}

int PremierLeague::get_week_num(istringstream &input) {
    string WEEK_NUM;
    int week_num;

    if (!(input >> WEEK_NUM)) {
        if (weeks_stats.size() == 0)
            throw BadRequest("first pass week");
        else
            return weeks_stats.size() - 1;
    }
    
    if (WEEK_NUM != "week_num")
        throw BadRequest("there is must week_num word");

    if (!(input >> week_num))
        throw BadRequest("there is must week_num integer");

    if (week_num <= 0)
        throw NotFound("the number of week is invalid");

    if (week_num > weeks_stats.size())
        throw BadRequest("the number of week is inavailable");

    return week_num - 1;
}

ostringstream PremierLeague::make_team_of_week(TeamOfTheWeek team_of_the_week) {
    ostringstream output;
    output << "team of the week:\n"
           << "GoalKeeper: " << team_of_the_week.goal_keeper.first 
           << " | score: " << fixed << setprecision(NUM_OF_DECIMAL_PLACE) 
           << team_of_the_week.goal_keeper.second << '\n'
           << "Defender 1: " << team_of_the_week.defender1.first
           << " | score: " << fixed << setprecision(NUM_OF_DECIMAL_PLACE) 
           << team_of_the_week.defender1.second << '\n'
           << "Defender 2: " << team_of_the_week.defender2.first
           << " | score: " << fixed << setprecision(NUM_OF_DECIMAL_PLACE) 
           << team_of_the_week.defender2.second << '\n'
           << "Midfielder: " << team_of_the_week.mid_fielder.first
           << " | score: " << fixed << setprecision(NUM_OF_DECIMAL_PLACE) 
           << team_of_the_week.mid_fielder.second << '\n'
           << "Forward: " << team_of_the_week.forward.first
           << " | score: " << fixed << setprecision(NUM_OF_DECIMAL_PLACE) 
           << team_of_the_week.forward.second << '\n';

    return output;
}

ostringstream PremierLeague::GET_players(istringstream &input) {
    Tools::check_question_mark(input);

    Team& team = get_team(input);
    Post_Ranks post_ranks = get_post_and_ranks(input);

    return team.GET_players(post_ranks);
}

Team& PremierLeague::get_team(istringstream &input) {
    string TEAM_NAME, team_name;

    if (!(input >> TEAM_NAME >> team_name) || TEAM_NAME != "team_name")
        throw BadRequest("invalid team_name arguments");

    replace(team_name.begin(), team_name.end(), TEAM_NAME_DELEMETER, ' ');

    return find_team(team_name);
}

Post_Ranks PremierLeague::get_post_and_ranks(istringstream &input) {
    string post, RANKS;
    
    if (!(input >> post))
        return Post_Ranks(all, false);

    if (input >> RANKS)
        if (RANKS == "ranks")
            return Post_Ranks(find_post(post), true);
        else
            throw BadRequest("this word must equal to 'ranks'");

    if (post == "ranks")
        return Post_Ranks(all, true);

    return Post_Ranks(find_post(post), false);
}

PlayerPost PremierLeague::find_post(string post) {
    if (post == "gk")
        return PlayerPost::goalkeeper;
    else if (post == "df")
        return PlayerPost::defender;
    else if (post == "md")
        return PlayerPost::midfielder;
    else if (post == "fw")
        return PlayerPost::forward;
    else
        throw BadRequest("player post not found");
}

ostringstream PremierLeague::GET_league_standings() {
    vector<TeamInfo> teams_info;
    for (auto& team : teams)
        teams_info.push_back(team.get_team_info());

    sort(teams_info.begin(), teams_info.end(), &PremierLeague::compare_teams);

    return make_league_standings(teams_info);
}

bool PremierLeague::compare_teams(const TeamInfo& left, const TeamInfo& right) {
    if (left.score != right.score)
        return left.score > right.score;

    if ((left.num_goals_for - left.num_goals_against) != (right.num_goals_for - right.num_goals_against))
        return (left.num_goals_for - left.num_goals_against) > (right.num_goals_for - right.num_goals_against);

    if (left.num_goals_for != right.num_goals_for)
        return left.num_goals_for > right.num_goals_for;

    return left.name < right.name;
}

ostringstream PremierLeague::make_league_standings(const vector<TeamInfo>& teams_info) {
    ostringstream output;
    output << "league standings:\n";
    for (int i = 0; i < teams_info.size(); i++)
        output << to_string(i + 1) << ". " << teams_info[i].name << ": "
               << "score: " << teams_info[i].score << " | "
               << "GF: " << teams_info[i].num_goals_for << " | "
               << "GA: " << teams_info[i].num_goals_against << '\n';

    return output;
}

ostringstream PremierLeague::GET_matches_result_league(istringstream &input) {
    Tools::check_question_mark(input);

    return weeks_stats[get_week_num(input)].GET_matches_result_league();
}

Player* PremierLeague::find_player(string player_name) const {
    for (auto& team : teams) {
        Player* player = team.find_player(player_name);
        if (player != nullptr)
            return player;
    }

    throw NotFound("this player doesn't exist to buy");
}