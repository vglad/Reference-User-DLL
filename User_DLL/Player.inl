#pragma once
#include "Player.h"
#include <stack>

inline Player::Player() = default;
inline Player::~Player() = default;

inline int Player::stack_in_bb(std::string const& bet_or_balance_symbol, std::string const& position, const int chair_number) {
    // get "currentbetX" or "balanceX" of the player in needed position
    const auto chips_in_bet_or_balance = _oh.read_symbol(bet_or_balance_symbol + std::to_string(chair_number), BUILT_IN_SYMBOL);
    const auto bblind = _oh.read_symbol(BIG_BLIND_SYMBOL, BUILT_IN_SYMBOL);
    double full_stack_size;
    if (position == SMALLBLIND && bet_or_balance_symbol == BALANCE) {
        full_stack_size = chips_in_bet_or_balance + bblind / 2;
    }
    else if (position == BIGBLIND && bet_or_balance_symbol == BALANCE) {
        full_stack_size = chips_in_bet_or_balance + bblind;
    }
    else {
        full_stack_size = chips_in_bet_or_balance;
    }
    const auto full_stack_size_in_bb = _conv.stack_to_blinds(full_stack_size, bblind);
    if (full_stack_size_in_bb < MIN_STACK_SIZE || full_stack_size_in_bb > MAX_STACK_SIZE) {
        _oh.write_log("stack_in_bb", "ERROR: Unknown " + position + " stack: [" + std::to_string(full_stack_size_in_bb) + "]");
        _oh.write_log("stack_in_bb", "ERROR: Setting " + position + " stack to 16bb");
        return MAX_STACK_SIZE;
    }
    return full_stack_size_in_bb;
}

inline int Player::hero_eff_stack(const std::vector<int>& stacks_vector, const int allin_situation) {
    const auto hero_position = static_cast<int>(_oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL));
    int allin_positions;
    switch (allin_situation) {
        case FIRST_TO_ACT: {
            return eff_stack_for_first_to_act(stacks_vector, hero_position);
        }
        case ONE_ALLIN: {
            allin_positions = static_cast<int>(_oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL));
            return eff_stack_for_one_allin(stacks_vector, hero_position, allin_positions);
        }
        case TWO_ALLINS: {
            allin_positions = static_cast<int>(_oh.read_symbol(TWO_ALLIN_POSITIONS, MEMORY_SYMBOL));        
            return eff_stack_for_two_allins(stacks_vector, hero_position, allin_positions);        
        }
        case THREE_ALLINS: {
            allin_positions = static_cast<int>(_oh.read_symbol(THREE_ALLIN_POSITIONS, MEMORY_SYMBOL));
            return eff_stack_for_three_allins(stacks_vector, hero_position, allin_positions);        
        }
        default: {
            _oh.write_log("hero_eff_stack", "ERROR: Unknown allin situation: [" + std::to_string(allin_situation) + "]");
            _oh.write_log("hero_eff_stack", "ERROR: Setting hero eff stack same as balance");
            return stack_in_bb(BALANCE, _conv.position_to_text(hero_position), chair_bit_number(_conv.position_to_text(hero_position)));
        }
    }
}

inline int Player::eff_stack_for_first_to_act(const std::vector<int>& stacks_vector, const int player_position) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    auto eff_stack = 0;
    switch (player_position) {
        case POSITION_CO: {
            if (co_stack <= bu_stack) {
                eff_stack = co_stack;
            }
            else {
                if (co_stack <= sb_stack) {
                    eff_stack = co_stack;
                }
                else {
                    if (co_stack <= bb_stack) {
                        eff_stack = co_stack;
                    }
                    else {
                        if (bu_stack <= sb_stack) {
                            if (sb_stack <= bb_stack) {
                                eff_stack = bb_stack;
                            }
                            else {
                                eff_stack = sb_stack;
                            }
                        }
                        else {
                            if (bu_stack <= bb_stack) {
                                eff_stack = bb_stack;
                            }
                            else {
                                eff_stack = bu_stack;
                            }
                        }
                    }
                }
            }
            break;
        }
        case POSITION_BU: {
            if (bu_stack <= sb_stack) {
                eff_stack = bu_stack;
            }
            else {
                if (bu_stack <= bb_stack) {
                    eff_stack = bu_stack;
                }
                else {
                    if (sb_stack <= bb_stack) {
                        eff_stack = bb_stack;
                    }
                    else {
                        eff_stack = sb_stack;
                    }
                }
            }
            break;
        }
        case POSITION_SB: {
            if (sb_stack <= bb_stack) {
                eff_stack = sb_stack;
            }
            else {
                eff_stack = bb_stack;
            }
            break;
        }
        default: {
            _oh.write_log("eff_stack_for_first_to_act", "ERROR: Unknown player position: [" + std::to_string(player_position) + "]");
            _oh.write_log("eff_stack_for_first_to_act", "ERROR: Setting player eff stack to 16bb");
            eff_stack = MAX_STACK_SIZE;
            break;
        }
    }
    return eff_stack;
}

inline int Player::eff_stack_for_one_allin(const std::vector<int>& stacks_vector, const int player_position, const int one_allin_position) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    auto eff_stack = 0;
    switch (one_allin_position) {
        case POSITION_CO: {
            switch (player_position) {
                case POSITION_BU: {
                    if (bu_stack <= co_stack || bu_stack <= sb_stack || bu_stack <= bb_stack){
                        eff_stack = bu_stack;
                    }
                    else if (co_stack >= sb_stack && co_stack >= bb_stack) {
                        eff_stack = co_stack;
                    }
                    else if (sb_stack >= bb_stack) {
                        eff_stack = sb_stack;
                    }
                    else {
                        eff_stack = bb_stack;
                    }
                    break;
                }
                case POSITION_SB: {
                    if (sb_stack <= co_stack || sb_stack <= bb_stack){
                        eff_stack = sb_stack;
                    }
                    else if (co_stack >= bb_stack) {
                        eff_stack = co_stack;
                    }
                    else {
                        eff_stack = bb_stack;
                    }
                    break;
                }
                case POSITION_BB: {
                    if (bb_stack <= co_stack){
                        eff_stack = bb_stack;
                    }
                    else {
                        eff_stack = co_stack;
                    }
                    break;
                }
                default: {
                    _oh.write_log("eff_stack_for_one_allin", "ERROR: Unknown player position: [" + std::to_string(player_position) + "]");
                    _oh.write_log("eff_stack_for_one_allin", "ERROR: Setting player eff stack to 16bb");
                    eff_stack = MAX_STACK_SIZE;
                    break;
                }
            }
            break;
        }
        case POSITION_BU: {
            switch (player_position) {
                case POSITION_SB: {
                    if (sb_stack <= bu_stack || sb_stack <= bb_stack){
                        eff_stack = sb_stack;
                    }
                    else if (bu_stack >= bb_stack) {
                        eff_stack = bu_stack;
                    }
                    else {
                        eff_stack = bb_stack;
                    }
                    break;
                }
                case POSITION_BB: {
                    if (bb_stack <= bu_stack){
                        eff_stack = bb_stack;
                    }
                    else {
                        eff_stack = bu_stack;
                    }
                    break;
                }
                default: {
                    _oh.write_log("eff_stack_for_one_allin", "ERROR: Unknown player position: [" + std::to_string(player_position) + "]");
                    _oh.write_log("eff_stack_for_one_allin", "ERROR: Setting player eff stack to 16bb");
                    eff_stack = MAX_STACK_SIZE;
                    break;
                }
            }
            break;
        }
        case POSITION_SB: {
            if (player_position == POSITION_BB) {
                if (bb_stack <= sb_stack){
                    eff_stack = bb_stack;
                }
                else {
                    eff_stack = sb_stack;
                }
            }
            else {
                _oh.write_log("eff_stack_for_one_allin", "ERROR: Unknown player position: [" + std::to_string(player_position) + "]");
                _oh.write_log("eff_stack_for_one_allin", "ERROR: Setting player eff stack to 16bb");
                eff_stack = MAX_STACK_SIZE;
            }
            break;
        }
        default: {
            _oh.write_log("eff_stack_for_one_allin", "ERROR: Unknown allin position: [" + std::to_string(one_allin_position) + "]");
            _oh.write_log("eff_stack_for_one_allin", "ERROR: Setting player eff stack to 16bb");
            eff_stack = MAX_STACK_SIZE;
            break;
        }
    }
    return eff_stack;
}

inline int Player::eff_stack_for_two_allins(const std::vector<int>& stacks_vector, const int player_position, const int two_allin_positions) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    auto eff_stack = 0;
    switch (two_allin_positions) {
        case POSITION_CO_BU: {
            switch (player_position) {
                case POSITION_SB: {
                    if (sb_stack <= co_stack || sb_stack <= bu_stack || sb_stack <= bb_stack){
                        eff_stack = sb_stack;
                    }
                    else if (co_stack >= bu_stack && co_stack >= bb_stack) {
                        eff_stack = co_stack;
                    }
                    else if (bu_stack >= bb_stack) {
                        eff_stack = bu_stack;
                    }
                    else {
                        eff_stack = bb_stack;
                    }
                    break;
                }
                case POSITION_BB: {
                    if (bb_stack <= co_stack || bb_stack <= bu_stack){
                        eff_stack = bb_stack;
                    }
                    else if (co_stack >= bu_stack) {
                        eff_stack = co_stack;
                    }
                    else {
                        eff_stack = bu_stack;
                    }
                    break;
                }
                default: {
                    _oh.write_log("eff_stack_for_two_allins", "ERROR: Unknown player position: [" + std::to_string(player_position) + "]");
                    _oh.write_log("eff_stack_for_two_allins", "ERROR: Setting player eff stack to 16bb");
                    eff_stack = MAX_STACK_SIZE;
                    break;
                }
            }
            break;
        }
        case POSITION_CO_SB: {
            if (bb_stack <= co_stack || bb_stack <= sb_stack){
                eff_stack = bb_stack;
            }
            else if (co_stack >= sb_stack) {
                eff_stack = co_stack;
            }
            else {
                eff_stack = sb_stack;
            }
            break;
        }
        case POSITION_BU_SB: {
            if (bb_stack <= bu_stack || bb_stack <= sb_stack){
                eff_stack = bb_stack;
            }
            else if (bu_stack >= sb_stack) {
                eff_stack = bu_stack;
            }
            else {
                eff_stack = sb_stack;
            }
            break;
        }
        default: {
            _oh.write_log("eff_stack_for_two_allins", "ERROR: Unknown allin positions: [" + std::to_string(two_allin_positions) + "]");
            _oh.write_log("eff_stack_for_two_allins", "ERROR: Setting player eff stack to 16bb");
            eff_stack = MAX_STACK_SIZE;
            break;
        }
    }
    return eff_stack;
}

inline int Player::eff_stack_for_three_allins(const std::vector<int>& stacks_vector, const int player_position, const int three_allin_positions) {
    const auto co_stack = stacks_vector[CO_STACK];
    const auto bu_stack = stacks_vector[BU_STACK];
    const auto sb_stack = stacks_vector[SB_STACK];
    const auto bb_stack = stacks_vector[BB_STACK];
    auto eff_stack = 0;
    if (three_allin_positions == POSITION_CO_BU_SB) {
        if (player_position == POSITION_BB) {
            _oh.write_log("eff_stack_for_three_allins", "ERROR: Not implemented");
            _oh.write_log("eff_stack_for_three_allins", "ERROR: Setting player eff stack to 16bb");
            eff_stack = MAX_STACK_SIZE;
        }
        else {
            _oh.write_log("eff_stack_for_two_allins", "ERROR: Unknown player position: [" + std::to_string(player_position) + "]");
            _oh.write_log("eff_stack_for_two_allins", "ERROR: Setting player eff stack to 16bb");
            eff_stack = MAX_STACK_SIZE;
        }
    }
    else {
        _oh.write_log("eff_stack_for_three_allins", "ERROR: Unknown allin positions: [" + std::to_string(three_allin_positions) + "]");
        _oh.write_log("eff_stack_for_three_allins", "ERROR: Setting player eff stack to 16bb");
        eff_stack = MAX_STACK_SIZE;
    }
    return eff_stack;
}

inline void Player::log_eff_stack(const int stack_size) {
    _oh.write_log("log_eff_stack", "Effective stack: [" + std::to_string(stack_size) + "]");
}

inline int Player::chair_bit_number(std::string const& position) {
    const auto chair_number = static_cast<int>(_oh.read_symbol(_conv.position_to_chair_symbol(position), MEMORY_SYMBOL));
    if (chair_number == CHAIR_UNKNOWN) {
        _oh.write_log("chair_bit_number", "ERROR: Unknown " + position + "chair: [" + std::to_string(chair_number) + "]");
        return CHAIR_UNKNOWN;
    }
    return chair_number;
}

inline int Player::hero_position() {
    const auto hero_position = _oh.read_symbol(HERO_POSITION, MEMORY_SYMBOL);
    if (!(hero_position == POSITION_CO) && !(hero_position == POSITION_BU) && !(hero_position == POSITION_SB) && !(hero_position == POSITION_BB)) {
        _oh.write_log("hero_positions", "ERROR: Wrong hero position: [" + std::to_string(hero_position) + "]");
        return 0;
    }
    return static_cast<int>(hero_position);
}

inline std::string Player::hero_combo() {
    const auto card1 = _oh.read_symbol(CARD_ONE, BUILT_IN_SYMBOL);
    const auto card2 = _oh.read_symbol(CARD_TWO, BUILT_IN_SYMBOL);
    const auto converted_card1 = _conv.card_index_to_text(card1);
    const auto converted_card2 = _conv.card_index_to_text(card2);
    if (converted_card1.empty()) {
        _oh.write_log("hero_combo", "ERROR: Unknown card1 index: [" + std::to_string(card1) + "]");
        return EMPTY_STRING;
    }
    if (converted_card2.empty()) {
        _oh.write_log("hero_combo", "ERROR: Unknown card2 index: [" + std::to_string(card2) + "]");
        return EMPTY_STRING;
    }
    const auto combo = _conv.hand_to_combo(converted_card1, converted_card2);
    _oh.write_log("hero_combo", "Hero combo: [" + combo + "]");
    return combo;
}
