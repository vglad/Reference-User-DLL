#pragma once
#include "Converter.h"

inline Converter::Converter() = default;
inline Converter::~Converter() = default;

inline std::string Converter::allin_positions_to_text(const double positions) {
    switch (static_cast<int>(positions)) {
        case POSITION_CO:  return "One Allin from CO";
        case POSITION_BU:  return "One Allin from BU";
        case POSITION_SB:  return "One Allin from SB";
        case POSITION_CO_BU: return "Two allins from CO and BU";
        case POSITION_CO_SB: return "Two allins from CO and SB";
        case POSITION_BU_SB: return "Two allins from BU and SB";
        case POSITION_CO_BU_SB: return "Three allins from CO, BU and SB";
        default: return EMPTY_STRING;
    }

}

inline std::string Converter::allin_situation_to_text(const int allin_situation) {
    switch (allin_situation) {
        case FIRST_TO_ACT: return "FIRST TO ACT";
        case ONE_ALLIN: return "FACING ONE ALLIN";
        case TWO_ALLINS: return "FACING TWO ALLINS";
        case THREE_ALLINS: return "FACING THREE ALLINS";
        default: return EMPTY_STRING;
    }
}

inline std::string Converter::card_index_to_text(const double card_index) {
        switch (static_cast<int>(card_index)) {
            case 0: return "2h";
            case 1: return "3h";
            case 2: return "4h";
            case 3: return "5h";
            case 4: return "6h";
            case 5: return "7h";
            case 6: return "8h";
            case 7: return "9h";
            case 8: return "Th";
            case 9: return "Jh";
            case 10: return "Qh";
            case 11: return "Kh";
            case 12: return "Ah";
            case 13: return "2d";
            case 14: return "3d";
            case 15: return "4d";
            case 16: return "5d";
            case 17: return "6d";
            case 18: return "7d";
            case 19: return "8d";
            case 20: return "9d";
            case 21: return "Td";
            case 22: return "Jd";
            case 23: return "Qd";
            case 24: return "Kd";
            case 25: return "Ad";
            case 26: return "2c";
            case 27: return "3c";
            case 28: return "4c";
            case 29: return "5c";
            case 30: return "6c";
            case 31: return "7c";
            case 32: return "8c";
            case 33: return "9c";
            case 34: return "Tc";
            case 35: return "Jc";
            case 36: return "Qc";
            case 37: return "Kc";
            case 38: return "Ac";
            case 39: return "2s";
            case 40: return "3s";
            case 41: return "4s";
            case 42: return "5s";
            case 43: return "6s";
            case 44: return "7s";
            case 45: return "8s";
            case 46: return "9s";
            case 47: return "Ts";
            case 48: return "Js";
            case 49: return "Qs";
            case 50: return "Ks";
            case 51: return "As";
            default: return EMPTY_STRING;
        }
}

inline int Converter::card_rank_to_int(const std::string& card_rank) {
    if (card_rank == "2") {
        return 2;
    }
    if (card_rank == "3") {
        return 3;
    }
    if (card_rank == "4") {
        return 4;
    }
    if (card_rank == "5") {
        return 5;
    }
    if (card_rank == "6") {
        return 6;
    }
    if (card_rank == "7") {
        return 7;
    }
    if (card_rank == "8") {
        return 8;
    }
    if (card_rank == "9") {
        return 9;
    }
    if (card_rank == "T") {
        return 10;
    }
    if (card_rank == "J") {
        return 11;
    }
    if (card_rank == "Q") {
        return 12;
    }
    if (card_rank == "K") {
        return 13;
    }
    if (card_rank == "A") {
        return 14;
    }
    return 0;
}

inline std::string Converter::position_to_chair_symbol(std::string const& position) {
    return position + "chair";
}

inline std::string Converter::position_to_text(const double position) {
    switch (static_cast<int>(position)) {
        case POSITION_CO:  return CUTOFF;
        case POSITION_BU:  return BUTTON;
        case POSITION_SB:  return SMALLBLIND;
        case POSITION_BB:  return BIGBLIND;
        default: return EMPTY_STRING;
    }
}

inline std::string Converter::hand_to_hand_by_highest_card(const std::string& card1, const std::string& card2) {
    if (card_rank_to_int(card1.substr(0,1)) < card_rank_to_int(card2.substr(0,1))) {
      return card2 + card1;  
    }
    return card1 + card2;
}

inline std::string Converter::hand_to_combo(const std::string& card1, const std::string& card2) {
    auto combo = _conv.hand_to_hand_by_highest_card(card1, card2);
    //check if pair
    if (card1.substr(0,1) == card2.substr(0,1)) {
        return card1.substr(0,1).append(card2.substr(0,1));
    }
    //check if suited
    if (card1.substr(1,1) == card2.substr(1,1)) {
        return combo.substr(0,1).append(combo.substr(2,1)).append(COMBO_TYPE_SUITED);
    }
    //offsuit
    return combo.substr(0,1).append(combo.substr(2,1)).append(COMBO_TYPE_OFFSUITED);
}

inline int Converter::stack_to_blinds(const double stack, const double bblind) {
    return static_cast<int>(round(stack/bblind));
}

inline int Converter::stack_int_to_blinds(const int stack, const int bblind) {
    return static_cast<int>(round(static_cast<double>(stack) / bblind));
}

inline int Converter::stack_to_stack_range(const int stack) {
    if (stack >= 8 && stack <= 10) {
        return STACK_RANGE_105;
    }
    if (stack >= 11 && stack <= 13) {
        return STACK_RANGE_135;
    }
    if (stack >= 14 && stack <= 16) {
        return STACK_RANGE_160;
    }
    return 0;
}

inline int Converter::chair_bit_number_to_int(const int chair_bit_value) {
    switch (chair_bit_value) {
        case CHAIR_0: return 0;
        case CHAIR_1: return 1;
        case CHAIR_2: return 2;
        case CHAIR_3: return 3;
        default: return CHAIR_UNKNOWN;
    }
}

inline std::string Converter::value_in_percent_to_text(const double value) {
    if (value == -1) {
        return "-1";
    }
    if (value == 0) {
        return "0.0%";
    }
    if (value == 1) {
        return "100.0%";
    }
    if (value - static_cast<int>(value) != 0) {
        auto percent = std::to_string(round(value * 1000) / 10);
        return percent.substr(0, percent.length() - 5).append("%");
    }
    return std::to_string(static_cast<int>(value)).append(".0%");
}

inline std::string Converter::stat_opp_value_to_text(const double stat_value) {
    if (stat_value == -1) {
        return "-1";
    }
    if (stat_value == 0) {
        return "0";
    }
    if (stat_value - static_cast<int>(stat_value) != 0) {
        return std::to_string(stat_value);            
    }
    return std::to_string(static_cast<int>(stat_value));
}

inline std::string Converter::to_3_digits_after_dot(const double value) {
    return std::to_string(value).substr(0, std::to_string(value).find('.') + 4);
}

inline std::string Converter::to_1_digits_after_dot(const double value) {
    return std::to_string(value).substr(0, std::to_string(value).find('.') + 2);
}

inline double Converter::percent_as_double_to_numeric(const double value) {
    return round(value * 1000) / 10;
}

inline std::string Converter::fold_to_call_stat_name(std::string stat_fold_name) {
    std::string from = "fold";
    const std::string to = "call";
    const auto start_pos = stat_fold_name.find(from);
    if(start_pos == std::string::npos)
        return stat_fold_name;
    auto stat_call_name = stat_fold_name.replace(start_pos, from.length(), to);
    return stat_call_name;
}

inline double Converter::fold_to_call_stat_value(const double stat_fold_value) {
    return  1 - stat_fold_value;
}
