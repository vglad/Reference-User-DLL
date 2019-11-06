#pragma once
#include "Table.h"

inline Table::Table() = default;
inline Table::~Table() = default;



//inline std::string Table::evaluate_equity_result(const double hand_equity, const double break_even_equity) {
//    std::string message;
//    if (hand_equity > break_even_equity) {
//        message.append("..........................[Table] Enough equity to continue. Returning TRUE.\n");
//    }
//    else {
//        message.append("..........................[Table] Not enough equity to continue. Returning FALSE.\n");
//    }
//    return message;
//}
//
//inline std::string Table::evaluate_sample(const int stat_opportunities_sample, const int min_samples_needed) {
//    std::string sample_massage;
//    if (stat_opportunities_sample == -1 || stat_opportunities_sample == 0) {
//        sample_massage.append(".........................[Table] No samples at all. Skipping exploit bot...\n");
//    }
//    else if (stat_opportunities_sample < min_samples_needed) {
//        sample_massage.append(".........................[Table] No enough samples. Skipping exploit bot...\n");
//    }
//    else {
//        sample_massage.append(".........................[Table] Sample size is OK. Starting exploit bot...\n");
//    }
//    return sample_massage;
//}
//
//inline std::string Table::get_stat_name(const int sit_position, const int stack_range,
//                                        const int one_all_in_position, const int one_allin_position_chair) {
//    std::string stat_name = "pt_";
//    switch (one_all_in_position) {
//        case POSITION_CO: {
//            stat_name.append("co");
//            break;
//        }
//        case POSITION_BU:  {
//            stat_name.append("bu");
//            break;
//        }
//        case POSITION_SB:  {
//            stat_name.append("sb");
//            break;
//        }
//        default: return EMPTY_STRING;
//    }
//    switch (stack_range) {
//        case STACK_RANGE_105: {
//            stat_name.append("105_rfi");
//            break;
//        }
//        case STACK_RANGE_135: {
//            stat_name.append("135_rfi");
//            break;
//        }
//        case STACK_RANGE_160:  {
//            stat_name.append("160_rfi");
//            break;
//        }
//        default: return EMPTY_STRING;
//    }
//    switch (one_allin_position_chair) {
//        case CHAIR_0:  {
//            stat_name.append("0");
//            break;
//        }
//        case CHAIR_1:  {
//            stat_name.append("1");
//            break;
//        }
//        case CHAIR_2:  {
//            stat_name.append("2");
//            break;
//        }
//        case CHAIR_3:  {
//            stat_name.append("3");
//            break;
//        }
//        default: return EMPTY_STRING;
//    }
//    return stat_name;
//}

inline int Table::positions_info(const int allin_situation) {
    switch (allin_situation) {
        case FIRST_TO_ACT: {
            return first_to_act_positions_info();        
        }
        case ONE_ALLIN: {
            return one_allin_positions_info();
        }
        case TWO_ALLINS: {
            return two_allin_positions_info();
        }
        case THREE_ALLINS: {
            return three_allin_positions_info();
        }
        default: {
            _oh.write_log("positions_info", "ERROR: Unknown allin situation: [" + std::to_string(allin_situation) + "]");
            return 0;
        }
    }
}

inline std::vector<int> Table::stacks_info(const int allin_situation) {
    switch (allin_situation) {
        case FIRST_TO_ACT:  return stacks_info_for_first_to_act();
        case ONE_ALLIN:     return stacks_info_for_one_allin();
        case TWO_ALLINS:    return stacks_info_for_two_allins();
        case THREE_ALLINS:  return stacks_info_for_three_allins();
        default: {
            _oh.write_log("stacks_info", "ERROR: Unknown allin situation: [" + std::to_string(allin_situation) + "]");
            std::vector<int> empty_vector;
            return empty_vector;
        }
    }
}

inline int Table::player_stack(std::string const& bet_or_balance_symbol, std::string const& position) {
    const auto chair_bit_number = _player.chair_bit_number(position);
    if (chair_bit_number == CHAIR_UNKNOWN) {
        return 0;
    }
    return _player.stack_in_bb(bet_or_balance_symbol, position, _conv.chair_bit_number_to_int(chair_bit_number));
}

inline void Table::log_hand_number() {
    _oh.write_log(EMPTY_STRING, LOG_HEADER);
    _oh.write_log("log_hand_number", "Hand number: " + _oh.read_hand_number());    
}

inline void Table::log_allin_situation_name(const int allin_situation) {
    _oh.write_log("log_allin_situation_name", _conv.allin_situation_to_text(allin_situation));
}

inline int Table::first_to_act_positions_info() {
    const auto hero_position = _oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL);
    if (!(hero_position == POSITION_CO) && !(hero_position == POSITION_BU) && !(hero_position == POSITION_SB)) {
        _oh.write_log("one_allin_positions_info", "ERROR: Wrong hero position: [" + std::to_string(hero_position) + "]");
        return 0;            
    }

    _oh.write_log("first_to_act_positions_info", "Hero position: [" + _conv.position_to_text(hero_position) + "]");
    return 1;
}

inline int Table::one_allin_positions_info() {
    const auto one_allin_position = _oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL);
    if (!(one_allin_position == POSITION_CO) && !(one_allin_position == POSITION_BU)
                                             && !(one_allin_position == POSITION_SB)) {
        _oh.write_log("one_allin_positions_info", "ERROR: Unknown one allin position: [" + std::to_string(one_allin_position) + "]");
        return 0;
    }
    const auto hero_position = _oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL);
    if (!(hero_position == POSITION_BU) && !(hero_position == POSITION_SB) && !(hero_position == POSITION_BB)) {
        _oh.write_log("one_allin_positions_info", "ERROR: Wrong hero position: [" + std::to_string(hero_position) + "]");
        return 0;            
    }
    if (hero_position == POSITION_BU && !(one_allin_position == POSITION_CO)) {
        _oh.write_log("one_allin_positions_info", "ERROR: Wrong one allin position: [" + std::to_string(one_allin_position) + "]");
        _oh.write_log("one_allin_positions_info", "ERROR: Hero on BU, one allin position must be CO");
        return 0;            
    }
    if (hero_position == POSITION_SB && (one_allin_position != POSITION_CO && one_allin_position != POSITION_BU)) {
        _oh.write_log("one_allin_positions_info", "ERROR: Wrong one allin position: [" + std::to_string(one_allin_position) + "]");
        _oh.write_log("one_allin_positions_info", "ERROR: Hero on SB, one allin position must be CO or BU");
        return 0;            
    }
    if (hero_position == POSITION_BB && (one_allin_position != POSITION_CO && one_allin_position != POSITION_BU && one_allin_position != POSITION_SB)) {
        _oh.write_log("one_allin_positions_info", "ERROR: Wrong one allin position: [" + std::to_string(one_allin_position) + "]");
        _oh.write_log("one_allin_positions_info", "ERROR: Hero on BB, one allin position must be CO, BU or SB");
        return 0;            
    }

    _oh.write_log("one_allin_positions_info", _conv.allin_positions_to_text(one_allin_position));
    _oh.write_log("one_allin_positions_info", "Hero position: [" + _conv.position_to_text(hero_position) + "]");
    return 1;
}

inline int Table::two_allin_positions_info() {
    const auto two_allin_positions = _oh.read_symbol(TWO_ALLIN_POSITIONS, MEMORY_SYMBOL);
    if (!(two_allin_positions == POSITION_CO_BU) && !(two_allin_positions == POSITION_CO_SB)
                                                 && !(two_allin_positions == POSITION_BU_SB)) {
        _oh.write_log("two_allin_positions_info", "ERROR: Unknown two allin positions: [" + std::to_string(two_allin_positions) + "]");
        return 0;
    }
    const auto hero_position = _oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL);
    if (!(hero_position == POSITION_SB) && !(hero_position == POSITION_BB)) {
        _oh.write_log("two_allin_positions_info", "ERROR: Wrong hero position: [" + std::to_string(hero_position) + "]");
        return 0;            
    }
    if (hero_position == POSITION_SB && !(two_allin_positions == POSITION_CO_BU)) {
        _oh.write_log("two_allin_positions_info", "ERROR: Wrong two allin positions: [" + std::to_string(two_allin_positions) + "]");
        _oh.write_log("two_allin_positions_info", "ERROR: Hero on SB, two allin positions must be CO and BU");
        return 0;             
    }

    _oh.write_log("two_allin_positions_info", _conv.allin_positions_to_text(two_allin_positions));
    _oh.write_log("two_allin_positions_info", "Hero position: [" + _conv.position_to_text(hero_position) + "]");
    return 1;
}

inline int Table::three_allin_positions_info() {
    const auto three_allin_positions = _oh.read_symbol(THREE_ALLIN_POSITIONS, MEMORY_SYMBOL);
    if (three_allin_positions != POSITION_CO_BU_SB) {
        _oh.write_log("three_allin_positions_info", "ERROR: Unknown three allin positions: [" + std::to_string(three_allin_positions) + "]");
        return 0;
    }
    const auto hero_position = _oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL);
    if (hero_position != POSITION_BB) {
        _oh.write_log("three_allin_positions_info", "ERROR: Wrong hero position: [" + std::to_string(hero_position) + "]");
        return 0;            
    }

    _oh.write_log("three_allin_positions_info", _conv.allin_positions_to_text(three_allin_positions));
    _oh.write_log("three_allin_positions_info", "Hero position: [" + _conv.position_to_text(hero_position) + "]");
    return 1;
}

inline std::vector<int> Table::stacks_info_for_first_to_act() {
    std::vector<int> stacks = {0, 0, 0, 0}, empty_vector;
    const auto hero_position = static_cast<int>(_oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL));
    if (hero_position == POSITION_CO) {
        if ((stacks[0] = player_stack(BALANCE, CUTOFF)) == 0) {
            return empty_vector;
        }
    }
    if (hero_position == POSITION_CO || hero_position == POSITION_BU) {
        if ((stacks[1] = player_stack(BALANCE, BUTTON)) == 0) {
            return empty_vector;                }
    }
    if (hero_position == POSITION_CO || hero_position == POSITION_BU || hero_position == POSITION_SB) {
        if ((stacks[2] = player_stack(BALANCE, SMALLBLIND)) == 0 ||
            (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
            return empty_vector;
        }
    }            
    return stacks;
}

inline std::vector<int> Table::stacks_info_for_one_allin() {
    std::vector<int> stacks = {0, 0, 0, 0}, empty_vector;
    const auto one_allin_position = _oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL);
    if (one_allin_position == POSITION_CO) {
        if ((stacks[0] = player_stack(BET, CUTOFF)) == 0 ||
            (stacks[1] = player_stack(BALANCE, BUTTON)) == 0 ||
            (stacks[2] = player_stack(BALANCE, SMALLBLIND)) == 0 ||
            (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
            return empty_vector;
        }
    }
    else if (one_allin_position == POSITION_BU) {
        if ((stacks[1] = player_stack(BET, BUTTON)) == 0 ||
            (stacks[2] = player_stack(BALANCE, SMALLBLIND)) == 0 ||
            (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
            return empty_vector;
        }            
    }
    else if (one_allin_position == POSITION_SB) {
        if ((stacks[2] = player_stack(BET, SMALLBLIND)) == 0 ||
            (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
            return empty_vector;
        }            
    }            
    return stacks;
}

inline std::vector<int> Table::stacks_info_for_two_allins() {
    std::vector<int> stacks = {0, 0, 0, 0}, empty_vector;
    const auto two_allin_positions = _oh.read_symbol(TWO_ALLIN_POSITIONS, MEMORY_SYMBOL);
    if (two_allin_positions == POSITION_CO_BU) {
        if ((stacks[0] = player_stack(BET, CUTOFF)) == 0 ||
            (stacks[1] = player_stack(BET, BUTTON)) == 0 ||
            (stacks[2] = player_stack(BALANCE, SMALLBLIND)) == 0 ||
            (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
            return empty_vector;
        }
    }
    else if (two_allin_positions == POSITION_CO_SB) {
        if ((stacks[0] = player_stack(BET, CUTOFF)) == 0 ||
            (stacks[1] = player_stack(BALANCE, BUTTON)) == 0 ||
            (stacks[2] = player_stack(BET, SMALLBLIND)) == 0 ||
            (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
            return empty_vector;
        }
    }
    else if (two_allin_positions == POSITION_BU_SB) {
        if ((stacks[1] = player_stack(BET, BUTTON)) == 0 ||
            (stacks[2] = player_stack(BET, SMALLBLIND)) == 0 ||
            (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
            return empty_vector;
        }
    }
    return stacks;
}

inline std::vector<int> Table::stacks_info_for_three_allins() {
    std::vector<int> stacks = {0, 0, 0, 0}, empty_vector;
    if ((stacks[0] = player_stack(BET, CUTOFF)) == 0 ||
        (stacks[1] = player_stack(BET, BUTTON)) == 0 ||
        (stacks[2] = player_stack(BET, SMALLBLIND)) == 0 ||
        (stacks[3] = player_stack(BALANCE, BIGBLIND)) == 0) {
        return empty_vector;
    }
    return stacks;
}