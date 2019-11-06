#pragma once
#ifndef USER_TableInfo_H
#define USER_TableInfo_H

using namespace std;

class TableInfo {
public:
    TableInfo();
    ~TableInfo();

    int             set_table_info();

    static int      get_big_blind();    
    void            set_big_blind(int value);

    vector<double>  get_player_default_info(int chair_number) const;
    void            set_player_seated(vector<double>& p_info) const;
    bool            check_enough_players_seated();
    void            set_player_active(vector<double>& p_info) const;
    bool            check_enough_players_active();
    void            set_player_bet_and_balance(vector<double>& p_info) const;
    bool            set_bet_values_on_blinds();
    bool            set_all_chairs_positions();
    void            set_allin_info();

    void            clear_table_info();
    void            log_table_info();
    
    bool            set_blinds_when_no_bb_found(const string& sb_chairs);
    bool            set_blinds_when_one_bb_found(const string& bb_chairs, const string& sb_chairs);
    bool            set_blinds_when_two_bb_found(const string& bb_chairs, const string& sb_chairs);
    bool            set_blinds_when_three_bb_found(const string& bb_chairs);
    bool            set_blinds_when_four_bb_found();

    bool            set_bb_position();
    bool            set_sb_position();
    bool            set_bu_position();
    bool            set_co_position();


    int bblind = 0;
    vector< vector<double> > table_info = { p_info_empty_vector, p_info_empty_vector, p_info_empty_vector, p_info_empty_vector };

};



TableInfo _tableInfo;


#include "TableInfo.inl"

#endif
