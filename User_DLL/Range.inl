#pragma once
#include "Range.h"
//#include "RangeOrder.h"


inline Range::Range() = default;
inline Range::~Range() = default;


//inline double Range::evaluate_stat_percent(const double stat_name_percent) {
//    if (stat_name_percent == 1) {
//        return 0.99999;
//    }
//    else {
//        return stat_name_percent;
//    }
//}
//
//inline std::string Range::get_string_percent(const double percent) {
//    if (percent < 0) {
//        return "-1";
//    }
//    const auto unmodified_int = static_cast<int>(round(10000 * percent));
//    if (percent >= 1 || unmodified_int >= 10000) {
//        return "100.00";
//    }
//    std::string percent_with_double_digit;
//    //if percent was less then 10, add one 0 in front, if less then 1, add two zeros in front ...
//    const int needed_zeros_in_front = MIN_LENGTH_IN_PERCENTAGE_TEXT - std::to_string(unmodified_int).length();
//    for (auto i = 0; i < needed_zeros_in_front; ++i) {
//        percent_with_double_digit.append("0");
//    }
//    percent_with_double_digit.append(std::to_string(unmodified_int)).insert(2, ".");
//    return percent_with_double_digit;
//}
//
//inline std::string Range::get_range_name(std::string const& percent) {
//    if (percent == "-1") {
//        return "-1";
//    }
//    std::string range_name = "list";
//    range_name.append(percent).append("X");
//    range_name.erase(range_name.length() - 4, 1);
//    return range_name;
//}
//
//inline std::string Range::get_range_combos(const double percent, std::vector<std::string> const& full_range_order) {
//    std::string range_combos;
//    auto counter = 0;
//    auto top_percent = percent * 100;
//    for (const auto& combo : full_range_order) {
//        const auto diff = get_combo_percent(combo.substr(2, 1));
//        if (top_percent < fabs(top_percent - diff)) {
//            break;
//        }
//        add_combo(range_combos,combo);
//        top_percent -= diff;
//
//        //can display range in several rows in OH function editor if needed
//        if (counter++ == 14) {
//            counter = 0;
//            range_combos = range_combos.append("\n");
//        }
//    }
//    return range_combos;
//}
//
//inline double Range::get_combo_percent(std::string const& combo_type) {
//    if (combo_type == COMBO_TYPE_PAIR) {
//        return DIFF_PAIR;
//    }
//    if (combo_type == COMBO_TYPE_SUITED) {
//        return DIFF_SUITED;
//    }
//    if (combo_type == COMBO_TYPE_OFFSUITED) {
//        return DIFF_OFFSUITED;
//    }
//    return 0;
//}
//
//inline bool Range::add_combo(std::string& range, std::string const& combo) {
//    if (!(Helper::trim_copy(combo).length() == 3)) {
//        return false;
//    }
//    if (combo.substr(2, 1) == COMBO_TYPE_PAIR) {
//            //if pair, we have to remove letter "p" at the end
//            range.append(combo.substr(0, 2)).append(" ");
//    }
//    else {
//        range.append(combo).append(" ");
//    }
//    return true;
//}
//
//inline std::string Range::get_full_range_order_name(std::string const& stat_name) {
//    std::string list_name = "list";
//    const auto original_stat = stat_name.substr(3, stat_name.length() - 4);
//    //appent 105/135/160
//    list_name.append(original_stat.substr(2, 3)).append("_");
//    //append CO/BU/SB/BB
//    auto to_upper = original_stat.substr(0, 2);
//    std::transform(to_upper.begin(), to_upper.end(),to_upper.begin(), ::toupper);
//    list_name.append(to_upper).append("__");
//    //append CO_AI/CO_F_BU_AI/CO_F_BU_F_SB_AI
//    if (to_upper == "CO") {
//        list_name.append("CO_AI");
//    }
//    else if(to_upper == "BU") {
//        list_name.append("CO_F_BU_AI");
//    }
//    else if (to_upper == "SB") {
//        list_name.append("CO_F_BU_F_SB_AI");
//    }
//
//    return list_name;
//}
//
//inline std::vector<std::string> Range::get_full_range_order(const std::string& full_range_order_name) {
//    std::vector<std::string> range;
//    if (full_range_order_name == "list105_CO__CO_AI") {
//        range = list105_CO__CO_AI;
//    }
//    else if(full_range_order_name == "list105_BU__CO_F_BU_AI") {
//        range = list105_BU__CO_F_BU_AI;
//    }
//    else if(full_range_order_name == "list105_SB__CO_F_BU_F_SB_AI") {
//        range = list105_SB__CO_F_BU_F_SB_AI;
//    }
//    else if (full_range_order_name == "list135_CO__CO_AI") {
//        range = list135_CO__CO_AI;
//    }
//    else if(full_range_order_name == "list135_BU__CO_F_BU_AI") {
//        range = list135_BU__CO_F_BU_AI;
//    }
//    else if(full_range_order_name == "list135_SB__CO_F_BU_F_SB_AI") {
//        range = list135_SB__CO_F_BU_F_SB_AI;
//    }
//    else if (full_range_order_name == "list160_CO__CO_AI") {
//        range = list160_CO__CO_AI;
//    }
//    else if(full_range_order_name == "list160_BU__CO_F_BU_AI") {
//        range = list160_BU__CO_F_BU_AI;
//    }
//    else if(full_range_order_name == "list160_SB__CO_F_BU_F_SB_AI") {
//        range = list160_SB__CO_F_BU_F_SB_AI;
//    }
//
//    return range;
//}

inline std::string Range::gto_file(const std::vector<int>& stacks_vector, const int allin_situation) {
    const auto hero_position = _oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL);
    double allin_positions;
    switch (allin_situation) {
        case FIRST_TO_ACT: {
            return gto_file_for_first_to_act(stacks_vector, hero_position);        
        }
        case ONE_ALLIN: {
            allin_positions = _oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL);
            return gto_file_for_one_allin(stacks_vector, hero_position, allin_positions);
        }
        case TWO_ALLINS: {
            allin_positions = _oh.read_symbol(TWO_ALLIN_POSITIONS, MEMORY_SYMBOL);        
            return gto_file_for_two_allins(stacks_vector, hero_position, allin_positions);        
        }
        case THREE_ALLINS: {
            allin_positions = _oh.read_symbol(THREE_ALLIN_POSITIONS, MEMORY_SYMBOL);        
            return gto_file_for_three_allins(stacks_vector, hero_position, allin_positions);        
        }
        default: {
            _oh.write_log("gto_file", "ERROR: Unknown allin situation: [" + std::to_string(allin_situation) + "]");
            return EMPTY_STRING;
        }
    }
}

inline int Range::check_strategy_file(std::string const& file, const bool gto_file) {
    const std::string prefix = gto_file ? "GTO " : "Exploit ";
    if (file.empty()) {
        _oh.write_log("check_strategy_file", "ERROR: " + prefix + " file name is empty");
        return 0;
    }
    if (!Helper::FileExist(file)) {
        _oh.write_log("check_strategy_file", "ERROR: " + prefix + " file [" + file + "] not exist");
        return 0;
    }
    const auto offset = gto_file ? 15 : 25;;
    _oh.write_log("check_strategy_file", prefix + " file: [" + file.substr(offset, file.length()) + "]");
    return 1;
}

inline std::string Range::exploit_file(const std::vector<int>& stacks_vector, const int allin_situation, const double stat_percent) {
    const auto hero_position = _oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL);
    //double allin_positions;
    switch (allin_situation) {
        case FIRST_TO_ACT: {
            return exploit_file_for_first_to_act(stacks_vector, hero_position, stat_percent);
        }
        case ONE_ALLIN: {
            return exploit_file_for_one_allin(stacks_vector, hero_position, stat_percent);
        }
        default: {
            _oh.write_log("exploit_file", "ERROR: Unknown allin situation: [" + std::to_string(allin_situation) + "]");
            return EMPTY_STRING;
        }
    }
}

inline std::vector<std::string> Range::combo_allin_info(const std::string& strategy_file_full_path, const std::string& combo, const bool gto_info) {
    std::vector<std::string> combo_info;
    if (!Helper::FileExist(strategy_file_full_path)) {
        _oh.write_log("combo_allin_info", "ERROR: File [" + strategy_file_full_path + "] not exist");
        return combo_info;
    }
    const std::string prefix = gto_info ? "GTO " : "Exploit ";
    std::ifstream in_file(strategy_file_full_path);
    std::string line;
    while (getline(in_file, line)) {
        if (line.substr(0,3).find(combo, 0) != std::string::npos) {
            const auto played_percent = line.substr(line.find_first_of(',') + 1, NUM_PERCENTS_DIGITS);
            const auto ev = line.substr(line.find_last_of(',') + 1);
            //_oh.write_log("combo_allin_info", "Combo: [" + combo + "]");
            _oh.write_log("combo_allin_info", prefix + "percent: [" + _conv.value_in_percent_to_text(std::stod(played_percent)) + "]");
            _oh.write_log("combo_allin_info", prefix + "EV: [" + ev + "]");
            combo_info.insert(combo_info.begin(), ev);
            combo_info.insert(combo_info.begin(), played_percent);
            combo_info.insert(combo_info.begin(), combo);
            return combo_info;
        }
    }
    _oh.write_log("combo_allin_info", "ERROR: Combo [" + combo + "] not found in " + prefix + "file [" + strategy_file_full_path + "]");
    return combo_info;
}

inline std::string Range::gto_file_for_first_to_act(const std::vector<int>& stacks_vector, const double hero_position) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    std::string path;
    if (hero_position == POSITION_CO) {
        path.append(GTO_FOLDER_PATH).append("\\co\\").append(std::to_string(co_stack)).append("\\")
        .append(std::to_string(bu_stack)).append("\\").append(std::to_string(sb_stack)).append("\\")
        .append(std::to_string(bb_stack)).append("\\co_ai.csv");
    }
    if (hero_position == POSITION_BU) {
        path.append(GTO_FOLDER_PATH).append("\\bu\\").append(std::to_string(bu_stack)).append("\\")
        .append(std::to_string(sb_stack)).append("\\").append(std::to_string(bb_stack)).append("\\co_f_bu_ai.csv");
    }
    if (hero_position == POSITION_SB) {
        if (sb_stack <= bb_stack) {
            path.append(GTO_FOLDER_PATH).append("\\sb\\").append(std::to_string(sb_stack)).append("\\co_f_bu_f_sb_ai.csv");
        }
        else {
            path.append(GTO_FOLDER_PATH).append("\\sb\\").append(std::to_string(bb_stack)).append("\\co_f_bu_f_sb_ai.csv");    
        }
    }
    return path;
}

inline std::string Range::gto_file_for_one_allin(const std::vector<int>& stacks_vector, const double hero_position, const double one_allin_position) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    std::string path;
    if (hero_position == POSITION_CO || one_allin_position == POSITION_BB) {
        return path;
    }
    if (hero_position == POSITION_BU) {
        path.append(GTO_FOLDER_PATH).append("\\co\\").append(std::to_string(co_stack)).append("\\")
        .append(std::to_string(bu_stack)).append("\\").append(std::to_string(sb_stack)).append("\\")
        .append(std::to_string(bb_stack)).append("\\co_ai_bu_ai.csv");
    }
    if (hero_position == POSITION_SB && one_allin_position == POSITION_CO) {
        path.append(GTO_FOLDER_PATH).append("\\co\\").append(std::to_string(co_stack)).append("\\")
        .append(std::to_string(bu_stack)).append("\\").append(std::to_string(sb_stack)).append("\\")
        .append(std::to_string(bb_stack)).append("\\co_ai_bu_f_sb_ai.csv");
    }
    if (hero_position == POSITION_SB && one_allin_position == POSITION_BU) {
        path.append(GTO_FOLDER_PATH).append("\\bu\\").append(std::to_string(bu_stack)).append("\\")
        .append(std::to_string(sb_stack)).append("\\").append(std::to_string(bb_stack)).append("\\co_f_bu_ai_sb_ai.csv");
    }
    if (hero_position == POSITION_BB && one_allin_position == POSITION_CO) {
        path.append(GTO_FOLDER_PATH).append("\\co\\").append(std::to_string(co_stack)).append("\\")
        .append(std::to_string(bu_stack)).append("\\").append(std::to_string(sb_stack)).append("\\")
        .append(std::to_string(bb_stack)).append("\\co_ai_bu_f_sb_f_bb_ai.csv");
    }
    if (hero_position == POSITION_BB && one_allin_position == POSITION_BU) {
        path.append(GTO_FOLDER_PATH).append("\\bu\\").append(std::to_string(bu_stack)).append("\\")
        .append(std::to_string(sb_stack)).append("\\").append(std::to_string(bb_stack)).append("\\co_f_bu_ai_sb_f_bb_ai.csv");
    }
    if (hero_position == POSITION_BB && one_allin_position == POSITION_SB) {
        if (sb_stack <= bb_stack) {
            path.append(GTO_FOLDER_PATH).append("\\sb\\").append(std::to_string(sb_stack)).append("\\co_f_bu_f_sb_ai_bb_ai.csv");
        }
        else {
            path.append(GTO_FOLDER_PATH).append("\\sb\\").append(std::to_string(bb_stack)).append("\\co_f_bu_f_sb_ai_bb_ai.csv");    
        }
    }
    return path;
}

inline std::string Range::gto_file_for_two_allins(const std::vector<int>& stacks_vector, const double hero_position, const double two_allin_positions) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    std::string path;
    if (!(hero_position == POSITION_SB && two_allin_positions == POSITION_CO_BU || 
          hero_position == POSITION_BB && two_allin_positions == POSITION_CO_BU ||
          hero_position == POSITION_BB && two_allin_positions == POSITION_CO_SB ||
          hero_position == POSITION_BB && two_allin_positions == POSITION_BU_SB)) {
        return path;
    }
    if (hero_position == POSITION_SB) {
        path.append(GTO_FOLDER_PATH).append("\\co\\").append(std::to_string(co_stack)).append("\\")
        .append(std::to_string(bu_stack)).append("\\").append(std::to_string(sb_stack)).append("\\")
        .append(std::to_string(bb_stack)).append("\\co_ai_bu_ai_sb_ai.csv");
    }
    else if (hero_position == POSITION_BB && two_allin_positions == POSITION_CO_BU) {
        path.append(GTO_FOLDER_PATH).append("\\co\\").append(std::to_string(co_stack)).append("\\")
        .append(std::to_string(bu_stack)).append("\\").append(std::to_string(sb_stack)).append("\\")
        .append(std::to_string(bb_stack)).append("\\co_ai_bu_ai_sb_f_bb_ai.csv");
    }
    else if (hero_position == POSITION_BB && two_allin_positions == POSITION_CO_SB) {
        path.append(GTO_FOLDER_PATH).append("\\co\\").append(std::to_string(co_stack)).append("\\")
        .append(std::to_string(bu_stack)).append("\\").append(std::to_string(sb_stack)).append("\\")
        .append(std::to_string(bb_stack)).append("\\co_ai_bu_f_sb_ai_bb_ai.csv");
    }
    else if (hero_position == POSITION_BB && two_allin_positions == POSITION_BU_SB) {
        path.append(GTO_FOLDER_PATH).append("\\bu\\").append(std::to_string(bu_stack)).append("\\")
        .append(std::to_string(sb_stack)).append("\\").append(std::to_string(bb_stack)).append("\\co_f_bu_ai_sb_ai_bb_ai.csv");
    }
    return path;
}

inline std::string Range::gto_file_for_three_allins(const std::vector<int>& stacks_vector, const double hero_position, const double three_allin_positions) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    std::string path;
    if (!(hero_position == POSITION_BB && three_allin_positions == POSITION_CO_BU_SB)) {
        return path;
    }
    path.append(GTO_FOLDER_PATH).append("\\co_ai_bu_ai_sb_ai_bb_ai.csv");
    return path;
}

inline std::string Range::exploit_file_for_first_to_act(const std::vector<int>& stacks_vector, const double hero_position, const double stat_percent) {
    std::string path;
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];

    if (hero_position == POSITION_SB) {
        std::string file_name = "\\co_f_bu_f_sb_ex_ai_bb_ai_";
        std::string file_ext = ".csv";
        auto eff_stack = std::to_string(sb_stack <= bb_stack ? sb_stack : bb_stack);
        std::string percentage_file_path;
        percentage_file_path.append(EXPLOIT_FOLDER_PATH).append(R"(\sb_bb\sb_exploit\)").append(eff_stack).append("\\bb_ai.csv");
        if (Helper::FileExist(percentage_file_path)) {
            std::ifstream percentage_file(percentage_file_path);
            std::string line;
            std::string previous_value = "1.8";
            std::string percent_as_text = "1.8"; //min value for AA, KK, QQ
            auto percent = _conv.percent_as_double_to_numeric(stat_percent);
            if (percent > 1.8) {
                while (getline(percentage_file, line)) {
                    if (percent - std::stod(line) <= 0) {
                        break;
                    }
                    previous_value = line;
                }
                percent_as_text = fabs(stat_percent - std::stod(line)) < stat_percent - std::stod(previous_value) ? line : previous_value;
            }
            auto percent_as_double = std::stod(percent_as_text);
            if (percent_as_double - static_cast<int>(percent_as_double) == 0) {
                percent_as_text.append(".0");
            }
            path.append(EXPLOIT_FOLDER_PATH).append(R"(\sb_bb\sb_exploit\)").append(eff_stack).append(file_name).append(percent_as_text).append(file_ext);
        }
        else {
            _oh.write_log("exploit_file_for_first_to_act", "Percentage file not exist: [" + percentage_file_path + "]");
        }
    }
    return path;
}

inline std::string Range::exploit_file_for_one_allin(const std::vector<int>& stacks_vector, const double hero_position, const double stat_percent) {
    std::string path;
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];

    if (hero_position == POSITION_BB) {
        std::string file_name = "\\co_f_bu_f_sb_ai_";
        std::string file_ext = "_bb_ex_ai.csv";
        auto eff_stack = std::to_string(sb_stack <= bb_stack ? sb_stack : bb_stack);
        std::string percentage_file_path;
        percentage_file_path.append(EXPLOIT_FOLDER_PATH).append(R"(\sb_bb\bb_exploit\)").append(eff_stack).append("\\sb_ai.csv");
        if (Helper::FileExist(percentage_file_path)) {
            std::ifstream percentage_file(percentage_file_path);
            std::string line;
            std::string previous_value = "1.8";
            std::string percent_as_text = "1.8"; //min value for AA, KK, QQ
            auto percent = _conv.percent_as_double_to_numeric(stat_percent);
            if (percent > 1.8) {
                while (getline(percentage_file, line)) {
                    if (percent - std::stod(line) <= 0) {
                        break;
                    }
                    previous_value = line;
                }
                percent_as_text = fabs(stat_percent - std::stod(line)) < stat_percent - std::stod(previous_value) ? line : previous_value;
            }
            auto percent_as_double = std::stod(percent_as_text);
            if (percent_as_double - static_cast<int>(percent_as_double) == 0) {
                percent_as_text.append(".0");
            }
            path.append(EXPLOIT_FOLDER_PATH).append(R"(\sb_bb\bb_exploit\)").append(eff_stack).append(file_name).append(percent_as_text).append(file_ext);
        }
        else {
            _oh.write_log("exploit_file_for_one_allin", "Percentage file not exist: [" + percentage_file_path + "]");
        }
    }
    return path;

}
