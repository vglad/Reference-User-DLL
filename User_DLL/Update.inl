#pragma once
#include "Update.h"

inline Update::Update() = default;
inline Update::~Update() = default;

//in reverse order because when we insert values the previous item moves to next position
const std::vector<std::string> Update::tablemap_names = {
    "t4p3_pka", "t4p1_pka", "t4p0_pka", "t3p3_pka", "t3p1_pka", "t3p0_pka",
    "t2p3_pka", "t2p1_pka", "t2p0_pka", "t1p3_pka", "t1p1_pka", "t1p0_pka"
};

inline void Update::create_default_players_names_file(const int table_number) {
    std::string text;
    const auto file_path = "..\\" + std::to_string(table_number).append(PLAYERS_FILE_PATH);
    for (auto i = 0; i < OPPONENTS_PER_TABLE; ++i) {
        const auto table_index = PLAYERS_NAMES_COUNT - OPPONENTS_PER_TABLE * table_number + (OPPONENTS_PER_TABLE - 1) - i;
        text.append(tablemap_names[table_index]).append("\n");
    }
    std::ofstream players_names_file(file_path, std::ios::trunc);
    players_names_file << text;
}

inline bool Update::correct_players_names_file_format(const int table_number) const {
    const auto file_path = "..\\" + std::to_string(table_number).append(PLAYERS_FILE_PATH);
    std::ifstream check_if_file_exist(file_path);
    if (!check_if_file_exist.good()) {
        return false;
    }
    std::ifstream check_correct_number_of_lines(file_path);
    // new lines will be skipped unless we stop it from happening:    
    check_correct_number_of_lines.unsetf(std::ios_base::skipws);
    // count the newlines with an algorithm specialized for counting:
    const unsigned line_count = std::count(
        std::istream_iterator<char>(check_correct_number_of_lines),
        std::istream_iterator<char>(),
        '\n');
    return line_count == OPPONENTS_PER_TABLE;
}

inline void Update::update_table(const int table_number, std::vector<std::string> const& new_players_names) const {
    const auto file_path = "..\\" + std::to_string(table_number).append(PLAYERS_FILE_PATH);
    std::ifstream in_players_names_file(file_path);
    std::string line, text;
    auto need_rewrite = 0;
    auto index = OPPONENTS_PER_TABLE;
    while (getline(in_players_names_file, line) && !(index == 0)) {
        auto current_p_name = new_players_names[table_number * OPPONENTS_PER_TABLE - index];
        if (line.find(current_p_name, 0) != std::string::npos && strlen(line.c_str()) == strlen(current_p_name.c_str())) {
            text.append(line).append("\n");
        }
        else {
            text.append(current_p_name).append("\n");
            need_rewrite = 1;
        }
        index--;
    }
    if (need_rewrite) {
        std::ofstream out_players_names_file(file_path, std::ios::trunc);
        out_players_names_file << text;
    }
}

inline std::vector<std::string> Update::get_current_names() {
    std::vector<std::string> new_players_names;
    for (auto i = 0; i < PLAYERS_NAMES_COUNT; ++i) {
        auto player_name = OHComm::read_region(tablemap_names[i]);
        if (player_name.length() > 0) {
            player_name.append("_pka");
        }
        else {
            player_name = (tablemap_names[i]);
        }
        new_players_names.insert(new_players_names.begin(), player_name);
    }
    return new_players_names;
}
