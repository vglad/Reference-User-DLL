#pragma once
#include "PT.h"

inline PT::PT() = default;
inline PT::~PT() = default;

inline std::string PT::generate_stat_name(const int allin_situation, const int hero_position, const std::vector<int>& stacks_vector, bool opportunities) {
    const auto one_allin_position = _oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL);
    std::string stat_name;
    if (allin_situation == FIRST_TO_ACT && hero_position == POSITION_SB) {
        std::string stack_range;
        if (!stacks_vector.empty()) {
            stack_range = std::to_string(_conv.stack_to_stack_range(_player.hero_eff_stack(stacks_vector, allin_situation)));    
        }
        stat_name.append("pt_bb").append(stack_range).append("_fold_to_sb_open");
        if (opportunities) {
            stat_name.append("_opp");
        }
        const auto chair_number = std::to_string(_conv.chair_bit_number_to_int(_player.chair_bit_number(_conv.position_to_text(POSITION_BB))));
        stat_name.append(chair_number);
    }
    else if (allin_situation == ONE_ALLIN && hero_position == POSITION_BB && one_allin_position == POSITION_SB) {
        std::string stack_range;
        if (!stacks_vector.empty()) {
            stack_range = std::to_string(_conv.stack_to_stack_range(_player.hero_eff_stack(stacks_vector, allin_situation)));    
        }
        stat_name.append("pt_sb").append(stack_range).append("_rfi");
        if (opportunities) {
            stat_name.append("_opp");
        }
        const auto chair_number = std::to_string(_conv.chair_bit_number_to_int(_player.chair_bit_number(_conv.position_to_text(POSITION_SB))));
        stat_name.append(chair_number);
    }

    //std::string stat_name = "pt_";
    //switch (one_all_in_position) {
    //    case POSITION_CO: {
    //        stat_name.append("co");
    //        break;
    //    }
    //    case POSITION_BU:  {
    //        stat_name.append("bu");
    //        break;
    //    }
    //    case POSITION_SB:  {
    //        stat_name.append("sb");
    //        break;
    //    }
    //    default: return EMPTY_STRING;
    //}
    //switch (stack_range) {
    //    case STACK_RANGE_105: {
    //        stat_name.append("105_rfi");
    //        break;
    //    }
    //    case STACK_RANGE_135: {
    //        stat_name.append("135_rfi");
    //        break;
    //    }
    //    case STACK_RANGE_160:  {
    //        stat_name.append("160_rfi");
    //        break;
    //    }
    //    default: return EMPTY_STRING;
    //}
    //switch (one_allin_position_chair) {
    //    case CHAIR_0:  {
    //        stat_name.append("0");
    //        break;
    //    }
    //    case CHAIR_1:  {
    //        stat_name.append("1");
    //        break;
    //    }
    //    case CHAIR_2:  {
    //        stat_name.append("2");
    //        break;
    //    }
    //    case CHAIR_3:  {
    //        stat_name.append("3");
    //        break;
    //    }
    //    default: return EMPTY_STRING;
    //}
    return stat_name;    
}

inline double PT::stat_value(const std::string& stat_name) {
    //const auto value = _oh.read_symbol(stat_name, BUILT_IN_SYMBOL);
    //_oh.write_log("stat_value", "Reading stat: [" + stat_name + "] : [" + std::to_string(value) + "]");
    //return value;
    return _oh.read_symbol(stat_name, BUILT_IN_SYMBOL);
}

inline bool PT::enough_opportunities(const double stat_value) {
    return stat_value >= MIN_STAT_SAMPLE;
}

inline int PT::stat_opportunities_info(const int allin_situation, const int hero_position, const std::vector<int>& stacks_vector) {
    const auto one_allin_position = _oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL);
    if (!(allin_situation == FIRST_TO_ACT && hero_position == POSITION_SB) &&
        !(allin_situation == ONE_ALLIN && hero_position == POSITION_BB && one_allin_position == POSITION_SB)) {
        _oh.write_log("stat_opportunities_info", "No exploit for this situation");
        return LESS_THAN_MIN_SAMPLE;
    }
    const std::vector<int> empty_vector;
    const auto stat_opp_by_eff_stack_name = _pt.generate_stat_name(allin_situation, hero_position, stacks_vector, true);
    const auto stat_opp_by_eff_stack_value = _pt.stat_value(stat_opp_by_eff_stack_name);
    if (stat_opp_by_eff_stack_value >= MIN_STAT_SAMPLE) {
        _oh.write_log("stat_opportunities_info", "Enough opportunities by eff stack");
        _oh.write_log("stat_opportunities_info", stat_opp_by_eff_stack_name + ": [" + _conv.stat_opp_value_to_text(stat_opp_by_eff_stack_value) + "]");
        return BY_EFF_STACK_SITUATIONS;
    }
    const auto stat_opp_all_name = _pt.generate_stat_name(allin_situation, hero_position, empty_vector, true);
    const auto stat_opp_all_value = _pt.stat_value(stat_opp_all_name);
    if (stat_opp_all_value >= MIN_STAT_SAMPLE) {
        _oh.write_log("stat_opportunities_info", "Enough opportunities by all situations");
        _oh.write_log("stat_opportunities_info",  stat_opp_all_name + ": [" + _conv.stat_opp_value_to_text(stat_opp_all_value) + "]");
        return BY_ALL_SITUATIONS;
    }
    _oh.write_log("stat_opportunities_info", "Not enough opportunities by all situations");
    _oh.write_log("stat_opportunities_info", stat_opp_by_eff_stack_name + ": [" + _conv.stat_opp_value_to_text(stat_opp_by_eff_stack_value) + "]");
    _oh.write_log("stat_opportunities_info", stat_opp_all_name + ": [" + _conv.stat_opp_value_to_text(stat_opp_all_value) + "]");
    return LESS_THAN_MIN_SAMPLE;    
}

inline double PT::stat_name_info(const int allin_situation, const int hero_position, const std::vector<int>& stacks_vector, const int stat_type) {
    const auto one_allin_position = _oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL);
    if (!(allin_situation == FIRST_TO_ACT && hero_position == POSITION_SB) &&
        !(allin_situation == ONE_ALLIN && hero_position == POSITION_BB && one_allin_position == POSITION_SB)) {
        _oh.write_log("stat_name_info", "ERROR: Unknown all in situation and hero position: [" + std::to_string(allin_situation) + "] [" + std::to_string(hero_position) + "]");
        return UNKNOWN_STAT_TYPE;
    }
    if (!(stat_type == BY_EFF_STACK_SITUATIONS || stat_type == BY_ALL_SITUATIONS)) {
        _oh.write_log("stat_name_info", "ERROR: Unknown stat type: [" + std::to_string(stat_type) + "]");
        return UNKNOWN_STAT_TYPE;
    }
    const std::vector<int> empty_vector;
    std::string stat_name;
    if (stat_type == BY_EFF_STACK_SITUATIONS) {
        stat_name = _pt.generate_stat_name(allin_situation, _player.hero_position(), stacks_vector, false);
    }
    else if (stat_type == BY_ALL_SITUATIONS) {
        stat_name = _pt.generate_stat_name(allin_situation, _player.hero_position(), empty_vector, false);
    }

    if (allin_situation == FIRST_TO_ACT && hero_position == POSITION_SB) {
        const auto stat_value_fold = _pt.stat_value(stat_name);
        _oh.write_log("stat_name_info", stat_name + ": [" + _conv.value_in_percent_to_text(stat_value_fold) + "]");
        const auto stat_name_call = _conv.fold_to_call_stat_name(stat_name);
        const auto stat_value_call = _conv.fold_to_call_stat_value(stat_value_fold);
        _oh.write_log("stat_name_info", stat_name_call + ": [" + _conv.value_in_percent_to_text(stat_value_call) + "]");
        return stat_value_call;
    }
    if (allin_situation == ONE_ALLIN && hero_position == POSITION_BB && one_allin_position == POSITION_SB) {
        const auto stat_value_allin = _pt.stat_value(stat_name);
        _oh.write_log("stat_name_info", stat_name + ": [" + _conv.value_in_percent_to_text(stat_value_allin) + "]");
        return stat_value_allin;
    }

    //
    //return UNKNOWN_STAT_TYPE;
}


