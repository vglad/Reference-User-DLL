#pragma once
#include "BotLogic.h"

inline BotLogic::BotLogic() = default;
inline BotLogic::~BotLogic() = default;

inline int BotLogic::solution(const int allin_situation) {
    if (!(allin_situation == FIRST_TO_ACT) && !(allin_situation == ONE_ALLIN) && 
        !(allin_situation == TWO_ALLINS)   && !(allin_situation == THREE_ALLINS)) {
        return 0;
    }
    if (_table.positions_info(allin_situation) == 0) {
        return 0;
    }
    auto stacks_vector = _table.stacks_info(allin_situation);
    if (stacks_vector.empty()) {
        return 0;
    }

    std::string exploit_file_full_path;
    auto play_exploit = false;
    const auto stat_opp_info = _pt.stat_opportunities_info(allin_situation, _player.hero_position(), stacks_vector);
    if (stat_opp_info != LESS_THAN_MIN_SAMPLE) {
        const auto stat_name_info = _pt.stat_name_info(allin_situation, _player.hero_position(), stacks_vector, stat_opp_info);
        if (stat_name_info != UNKNOWN_STAT_TYPE) {
            exploit_file_full_path = _range.exploit_file(stacks_vector, allin_situation, stat_name_info);
            if (_range.check_strategy_file(exploit_file_full_path, false) != 0) {
                play_exploit = true;
            }
        }
    }

    const auto gto_file_full_path = _range.gto_file(stacks_vector, allin_situation);
    if (_range.check_strategy_file(gto_file_full_path, true) == 0) {
        return 0;
    }

    auto combo = _player.hero_combo();
    if (combo.empty()) {
        return 0;
    }

    std::vector<std::string> exploit_combo_allin_info;
    if (play_exploit) {
        exploit_combo_allin_info = _range.combo_allin_info(exploit_file_full_path, combo, false);
        if (exploit_combo_allin_info.empty()) {
            play_exploit = false;
        }
        else if (exploit_combo_allin_info[PLAYING_PERCENTAGE] == ZERO_PERCENTAGE) {
            return 0;
        }
    }

    auto gto_combo_allin_info = _range.combo_allin_info(gto_file_full_path, combo, true);
    if (gto_combo_allin_info.empty() || gto_combo_allin_info[PLAYING_PERCENTAGE] == ZERO_PERCENTAGE) {
        return 0;
    }
    
    const auto hero_eff_stack = _player.hero_eff_stack(stacks_vector, allin_situation);
    _player.log_eff_stack(hero_eff_stack);

    if (!check_combo_for_allin(gto_combo_allin_info, exploit_combo_allin_info, hero_eff_stack, allin_situation, play_exploit)) {
        return 0;    
    }
    return 1;
}

inline bool BotLogic::check_combo_for_allin(const std::vector<std::string>& gto_combo_allin_info, const std::vector<std::string>& exploit_combo_allin_info, const int my_stack_size, const int allin_situation, const bool play_exploit) {
    auto exploit = play_exploit;
    if (exploit && exploit_combo_allin_info.empty()) {
        _oh.write_log("check_combo_for_allin", "ERROR: Empty exploit vector.");
        _oh.write_log("check_combo_for_allin", "ERROR: Playing GTO.");
        exploit = false;
    }

    double exploit_playing_percent = 0;
    if (exploit) {
        exploit_playing_percent = std::stod(exploit_combo_allin_info[PLAYING_PERCENTAGE]);
        if (exploit_playing_percent == 0) {
            _oh.write_log("check_combo_for_allin", "Exploit playing percent is 0");
            return false;
        }
        if (!k_play_combos_with_mixed_percent && exploit_playing_percent < 1) {
            _oh.write_log("check_combo_for_allin", "Not playing combos with mixed percent");
            return false;
        }
    }

    const auto gto_playing_percent = std::stod(gto_combo_allin_info[PLAYING_PERCENTAGE]);
    if (gto_playing_percent == 0) {
        _oh.write_log("check_combo_for_allin", "GTO playing percent is 0");
        return false;
    }

    if (!k_play_combos_with_mixed_percent && gto_playing_percent < 1 && !exploit) {
        _oh.write_log("check_combo_for_allin", "Not playing combos with mixed percent");
        return false;
    }

    const auto ev_correction = combo_ev_correction(my_stack_size, allin_situation, exploit);
    const auto ev = std::stod((exploit ? exploit_combo_allin_info : gto_combo_allin_info)[COMBO_EV]) - ev_correction;
    _oh.write_log("check_combo_for_allin", "Corrected combo ev: [" + _conv.to_3_digits_after_dot(ev) + "]");
    return ev > 0;
}

inline double BotLogic::combo_ev_correction(const int my_stack_size, const int allin_situation, const bool play_exploit) {
    auto ev_correction = 0.0;
    if (k_use_stack_ev_correction) {
        if (!play_exploit && allin_situation == FIRST_TO_ACT) {
            ev_correction = ev_correction_big(my_stack_size);
            _oh.write_log("combo_ev_correction", "EV correction for FIRST_TO_ACT: [" + _conv.to_3_digits_after_dot(ev_correction) + "]");
        }
        else if (play_exploit && (allin_situation == FIRST_TO_ACT || allin_situation == ONE_ALLIN)){
            ev_correction = ev_correction_small(my_stack_size);
            _oh.write_log("combo_ev_correction", "Small EV correction for exploit: [" + _conv.to_3_digits_after_dot(ev_correction) + "]");
        }
        else {
            ev_correction = ev_correction_small(my_stack_size);
            _oh.write_log("combo_ev_correction", "Small EV correction: [" + _conv.to_3_digits_after_dot(ev_correction) + "]");
        }
    }
    //else {
    //    _oh.write_log("combo_ev_correction", "Combo ev correction not used");
    //}
    return ev_correction;
}

inline double BotLogic::ev_correction_small(const int my_stack_size) {
    auto ev_correction = 0.0;
    switch (my_stack_size) {
        case 8:  {ev_correction = CORRECTION_8;  break;}
        case 9:  {ev_correction = CORRECTION_9;  break;}
        case 10: {ev_correction = CORRECTION_10; break;}
        case 11: {ev_correction = CORRECTION_11; break;}
        case 12: {ev_correction = CORRECTION_12; break;}
        case 13: {ev_correction = CORRECTION_13; break;}
        case 14: {ev_correction = CORRECTION_14; break;}
        case 15: {ev_correction = CORRECTION_15; break;}
        case 16: {ev_correction = CORRECTION_16; break;}
        default: {
            _oh.write_log("combo_ev_correction", "ERROR: Unknown stack size: [" + std::to_string(my_stack_size) + "]");
            _oh.write_log("combo_ev_correction", "ERROR: Setting combo ev correction to: [" + std::to_string(CORRECTION_8) + "]");
            ev_correction = CORRECTION_8;
            break;
        }
    }
    return ev_correction;
}

inline double BotLogic::ev_correction_big(const int my_stack_size) {
    auto ev_correction = 0.0;
    switch (my_stack_size) {
        case 8:  {ev_correction = OR_CORRECTION_8;  break;}
        case 9:  {ev_correction = OR_CORRECTION_9;  break;}
        case 10: {ev_correction = OR_CORRECTION_10; break;}
        case 11: {ev_correction = OR_CORRECTION_11; break;}
        case 12: {ev_correction = OR_CORRECTION_12; break;}
        case 13: {ev_correction = OR_CORRECTION_13; break;}
        case 14: {ev_correction = OR_CORRECTION_14; break;}
        case 15: {ev_correction = OR_CORRECTION_15; break;}
        case 16: {ev_correction = OR_CORRECTION_16; break;}
        default: {
            _oh.write_log("combo_ev_correction", "ERROR: Unknown stack size: [" + std::to_string(my_stack_size) + "]");
            _oh.write_log("combo_ev_correction", "ERROR: Setting combo ev correction to: [" + std::to_string(OR_CORRECTION_8) + "]");
            ev_correction = OR_CORRECTION_8;
            break;
        }
    }
    return ev_correction;
}

inline int BotLogic::fold() {
    _oh.write_log("fold", "ALLIN: FALSE");
    _oh.write_log(EMPTY_STRING, LOG_FOOTER);
    return 0;
}

inline int BotLogic::allin() {
    _oh.write_log("allin", "ALLIN: TRUE");
    _oh.write_log(EMPTY_STRING, LOG_FOOTER);
    return 1;
}

