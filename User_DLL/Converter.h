#pragma once
#ifndef USER_CONVERTER_H
#define USER_CONVERTER_H

class Converter final {
    public:
    Converter();
    ~Converter();
    


    static int          stack_to_blinds(double stack, double bblind);
    static int          stack_int_to_blinds(int stack, int bblind);
    static int          stack_to_stack_range(int stack);
    static int          chair_bit_number_to_int(int chair_bit_value);

    static std::string  position_to_chair_symbol(std::string const& position);
    static std::string  position_to_text(double position);
    
    static std::string  allin_positions_to_text(double positions);
    static std::string  allin_situation_to_text(int allin_situation);

    static std::string  card_index_to_text(double card_index);
    static int          card_rank_to_int(const std::string& card_rank);

    static std::string  hand_to_hand_by_highest_card(const std::string& card1, const std::string& card2);
    static std::string  hand_to_combo(const std::string& card1, const std::string& card2);

    static std::string  value_in_percent_to_text(double value);
    static std::string  stat_opp_value_to_text(double stat_value);
    static double       percent_as_double_to_numeric(double value);

    static std::string  to_3_digits_after_dot(double value);
    static std::string  to_1_digits_after_dot(double value);

    static std::string  fold_to_call_stat_name(std::string stat_fold_name);
    static double       fold_to_call_stat_value(double stat_fold_value);

};

Converter _conv;

#include "Converter.inl"

#endif
