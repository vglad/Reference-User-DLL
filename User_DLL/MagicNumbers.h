#ifndef USER_MAGICNUMBERS_H
#define USER_MAGICNUMBERS_H
#pragma once

#define MIN_LENGTH_IN_PERCENTAGE_TEXT   4

#define COMBO_TYPE_PAIR         "p"
#define COMBO_TYPE_SUITED       "s"
#define COMBO_TYPE_OFFSUITED    "o"
#define DIFF_PAIR               0.452   //percent from all 169 combos (100% range)
#define DIFF_SUITED             0.301
#define DIFF_OFFSUITED          0.904

#define TABLE_COUNT         4
#define OPPONENTS_PER_TABLE 3
#define PLAYERS_NAMES_COUNT 12  //(TABLE_COUNT * OPPONENTS_PER_TABLE)

#define LOG_FILE_PATH       "tools\\bot_log.txt"
#define PLAYERS_FILE_PATH   "\\tools\\players.txt"
#define GTO_FOLDER_PATH     "..\\profile\\gto"
#define EXPLOIT_FOLDER_PATH "..\\profile\\exploit"

#define EMPTY_STRING    ""
#define ZERO_PERCENTAGE "0.000"

#define POSITION_CO         11000
#define POSITION_BU         10100
#define POSITION_SB         10010
#define POSITION_BB         10001
#define POSITION_CO_BU      11100
#define POSITION_CO_SB      11010
#define POSITION_BU_SB      10110
#define POSITION_CO_BU_SB   11110
#define POSITION_UNKNOWN    11111


#define CUTOFF                      "CO"
#define BUTTON                      "BU"
#define SMALLBLIND                  "SB"
#define BIGBLIND                    "BB"

//OH logic Symbols
#define CO_CHAIR    "COchair"
#define BU_CHAIR    "BUchair"
#define SB_CHAIR    "SBchair"
#define BB_CHAIR    "BBchair"

#define HERO_POSITION            "HeroPosition"
#define ONE_ALLIN_POSITION       "OneAllInPosition"
#define TWO_ALLIN_POSITIONS      "TwoAllInPositions"
#define THREE_ALLIN_POSITIONS    "ThreeAllInPositions"

#define BET                 "currentbet"
#define BALANCE             "balance"
#define BIG_BLIND_SYMBOL    "bblind"

#define CARD_ONE    "$$pc0"
#define CARD_TWO    "$$pc1"

#define ACTIVE_FLAG_BUTTON_FOR_LOG  "f19"

#define CHAIR_0         11000
#define CHAIR_1         10100
#define CHAIR_2         10010
#define CHAIR_3         10001
#define CHAIR_UNKNOWN   11111

#define OH_MIN_SAMPLE_FUNC "f$Gen_MinSample"
//end of OH logic symbols

#define MIN_SAMPLE_BB_FOLD_TO_SB 15
#define MIN_STAT_SAMPLE 15

#define UNKNOWN_STAT_TYPE       -1
#define LESS_THAN_MIN_SAMPLE    10
#define BY_EFF_STACK_SITUATIONS 11
#define BY_ALL_SITUATIONS       12


#define STACK_RANGE_105 105
#define STACK_RANGE_135 135
#define STACK_RANGE_160 160

#define CO_STACK    0
#define BU_STACK    1
#define SB_STACK    2
#define BB_STACK    3

#define FIRST_TO_ACT        100
#define ONE_ALLIN           101
#define TWO_ALLINS          102
#define THREE_ALLINS        103
#define UNKNOWN_SITUATION   111

#define MAX_STACK_SIZE  16
#define MIN_STACK_SIZE  8

#define NUM_PERCENTS_DIGITS 5

//cell numbers in p_info vector
#define P_CHAIR     0
#define P_SEATED    1
#define P_ACTIVE    2
#define P_POSITION  3
#define P_BET       4
#define P_BALANCE   5
#define P_ISALLIN   6
const std::vector<double> p_info_empty_vector {0, 0, 0, 0, 0, 0, 0};


//cell numbers in vector with combo allin info
#define COMBO_NAME          0
#define PLAYING_PERCENTAGE  1
#define COMBO_EV            2

#define CORRECTION_8    0.025
#define CORRECTION_9    0.025
#define CORRECTION_10   0.035
#define CORRECTION_11   0.035
#define CORRECTION_12   0.045
#define CORRECTION_13   0.045
#define CORRECTION_14   0.055
#define CORRECTION_15   0.055
#define CORRECTION_16   0.065

#define OR_CORRECTION_8    0.105
#define OR_CORRECTION_9    0.105
#define OR_CORRECTION_10   0.105
#define OR_CORRECTION_11   0.105
#define OR_CORRECTION_12   0.115
#define OR_CORRECTION_13   0.115
#define OR_CORRECTION_14   0.115
#define OR_CORRECTION_15   0.125
#define OR_CORRECTION_16   0.125

#define LOG_HEADER  "**********NEW HAND EVALUATION********************"
#define LOG_FOOTER  "***************************************************************"

enum symbol_type {
    BUILT_IN_SYMBOL,
    MEMORY_SYMBOL
};

bool k_debug = true;
bool k_current_debug_value = true;
const bool k_play_combos_with_mixed_percent = false;
const bool k_use_stack_ev_correction = true;
int k_critical_error = 0;






#endif
