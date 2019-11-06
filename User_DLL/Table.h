#pragma once
#ifndef USER_TABLE_H
#define USER_TABLE_H

class Table final {
    public:
        Table();
        ~Table();


        //static std::string evaluate_equity_result(double hand_equity, double break_even_equity);
        //static std::string evaluate_sample(int stat_opportunities_sample, int min_samples_needed);
        //static std::string get_stat_name(int sit_position, int stack_range, int one_all_in_position,
        //                                 int one_allin_position_chair);
        //

        
        static int              positions_info(int allin_situation);
        static std::vector<int> stacks_info(int allin_situation);
        static int              player_stack (std::string const& bet_or_balance_symbol, std::string const& position);
        static void             log_hand_number();
        static void             log_allin_situation_name(int allin_situation);

    private:
        static int first_to_act_positions_info();
        static int one_allin_positions_info();
        static int two_allin_positions_info();
        static int three_allin_positions_info();
        static std::vector<int> stacks_info_for_first_to_act();
        static std::vector<int> stacks_info_for_one_allin();
        static std::vector<int> stacks_info_for_two_allins();
        static std::vector<int> stacks_info_for_three_allins();
};


Table _table;

#include "Table.inl"

#endif
