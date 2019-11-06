#pragma once
#ifndef USER_UPDATE_H
#define USER_UPDATE_H

class Update final {
    public:
    Update();
    ~Update();

    static void create_default_players_names_file(const int table_number);
    bool correct_players_names_file_format(const int table_number) const;
    void update_table(const int table_number, std::vector<std::string> const& new_players_names) const;
    static std::vector<std::string> get_current_names();

    static const std::vector<std::string> tablemap_names;

};

Update _updater;

#include "Update.inl"

#endif
