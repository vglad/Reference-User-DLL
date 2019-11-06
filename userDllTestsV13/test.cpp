#include "pch.h"

#include <memory>
#include <iterator>
#include <string>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <iostream>
#include <chrono>


//#include "..\User_DLL\user.h"
#include "..\User_DLL\MagicNumbers.h"
#include "..\User_DLL\HelperFunctions.h"
#include "..\User_DLL\Converter.h"
#include "..\User_DLL\IBridge.h"

//#include "..\User_DLL\OHCommunicator.h"
#include "FakeOHCommunicator.h"

#include "..\User_DLL\TableInfo.h"
//#include "..\User_DLL\Player.h"
//#include "..\User_DLL\Range.h"
//#include "..\User_DLL\RangeOrder.h"
//#include "..\User_DLL\Update.h"
//#include "..\User_DLL\Table.h"
//#include "..\User_DLL\BotLogic.h"



TEST(TableInfo_set_big_blind, SetProperBlindValues) {
    TableInfo table_info;

    table_info.set_big_blind(4);
    EXPECT_EQ(4, table_info.bblind);
    table_info.set_big_blind(10);
    EXPECT_EQ(10, table_info.bblind);
    table_info.set_big_blind(50);
    EXPECT_EQ(50, table_info.bblind);
    table_info.set_big_blind(200);
    EXPECT_EQ(200, table_info.bblind);
    table_info.set_big_blind(20);
    EXPECT_EQ(0, table_info.bblind);
    table_info.set_big_blind(0);
    EXPECT_EQ(0, table_info.bblind);

}

TEST(TableInfo_set_player_seated, SetProperSeatedValues) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;

    p_info[P_SEATED] = 0;
    table_info.set_player_seated(p_info);
    EXPECT_EQ(0, p_info[P_SEATED]);

    p_info[P_SEATED] = 1;
    table_info.set_player_seated(p_info);
    EXPECT_EQ(1, p_info[P_SEATED]);

    p_info[P_SEATED] = -1;
    table_info.set_player_seated(p_info);
    EXPECT_EQ(0, p_info[P_SEATED]);

    p_info[P_SEATED] = 10;
    table_info.set_player_seated(p_info);
    EXPECT_EQ(0, p_info[P_SEATED]);

}

TEST(TableInfo_check_enough_players_seated, ReturnProperValuesWhenCheckSeated) {
    TableInfo table_info;
    
    table_info.table_info[0][P_SEATED] = 0;
    table_info.table_info[1][P_SEATED] = 0;
    table_info.table_info[2][P_SEATED] = 0;
    table_info.table_info[3][P_SEATED] = 1;
    EXPECT_EQ(false, table_info.check_enough_players_seated());
    table_info.clear_table_info();

    table_info.table_info[0][P_SEATED] = 0;
    table_info.table_info[1][P_SEATED] = 0;
    table_info.table_info[2][P_SEATED] = 0;
    table_info.table_info[3][P_SEATED] = 0;
    EXPECT_EQ(false, table_info.check_enough_players_seated());
    table_info.clear_table_info();

    table_info.table_info[0][P_SEATED] = 0;
    table_info.table_info[1][P_SEATED] = 0;
    table_info.table_info[2][P_SEATED] = 1;
    table_info.table_info[3][P_SEATED] = 1;
    EXPECT_EQ(true, table_info.check_enough_players_seated());
    table_info.clear_table_info();
}

TEST(TableInfo_set_player_active, SetProperActiveValues) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;

    p_info[P_ACTIVE] = 0;
    table_info.set_player_active(p_info);
    EXPECT_EQ(0, p_info[P_ACTIVE]);

    p_info[P_ACTIVE] = -1;
    table_info.set_player_active(p_info);
    EXPECT_EQ(0, p_info[P_ACTIVE]);

    p_info[P_ACTIVE] = 10;
    table_info.set_player_active(p_info);
    EXPECT_EQ(0, p_info[P_ACTIVE]);

    p_info[P_SEATED] = 0;
    p_info[P_ACTIVE] = 1;
    table_info.set_player_active(p_info);
    EXPECT_EQ(0, p_info[P_ACTIVE]);

    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    table_info.set_player_active(p_info);
    EXPECT_EQ(1, p_info[P_ACTIVE]);
}

TEST(TableInfo_check_enough_players_active, ReturnProperValuesWhenCheckActive) {
    TableInfo table_info;
    
    table_info.table_info[0][P_ACTIVE] = 0;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.table_info[3][P_ACTIVE] = 1;
    EXPECT_EQ(false, table_info.check_enough_players_active());
    table_info.clear_table_info();

    table_info.table_info[0][P_ACTIVE] = 0;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.table_info[3][P_ACTIVE] = 0;
    EXPECT_EQ(false, table_info.check_enough_players_active());
    table_info.clear_table_info();

    table_info.table_info[0][P_ACTIVE] = 0;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.table_info[3][P_ACTIVE] = 1;
    EXPECT_EQ(true, table_info.check_enough_players_active());
    table_info.clear_table_info();
}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesIfNotSeated) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;

    p_info[P_SEATED] = 0;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 10;
    p_info[P_BALANCE] = 100;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0, p_info[P_ACTIVE]);
    EXPECT_EQ(0, p_info[P_BET]);
    EXPECT_EQ(0, p_info[P_BALANCE]);
}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesIfNotActive) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;

    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 0;
    p_info[P_BET] = 10;
    p_info[P_BALANCE] = 100;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0, p_info[P_ACTIVE]);
    EXPECT_EQ(0, p_info[P_BET]);
    EXPECT_EQ(0, p_info[P_BALANCE]);
}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesIfBetAndBalanceZero) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    
    p_info[P_BET] = 0;
    p_info[P_BALANCE] = 0;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BET]);
    EXPECT_EQ(0, p_info[P_BALANCE]);

    p_info[P_BET] = -1;
    p_info[P_BALANCE] = 0;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BET]);
    EXPECT_EQ(0, p_info[P_BALANCE]);

    p_info[P_BET] = -1;
    p_info[P_BALANCE] = -1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BET]);
    EXPECT_EQ(0, p_info[P_BALANCE]);

    p_info[P_BET] = 0;
    p_info[P_BALANCE] = -1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BET]);
    EXPECT_EQ(0, p_info[P_BALANCE]);

}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesForBalanceIfBetZero) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    table_info.bblind = 10;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 0;
    
    p_info[P_BALANCE] = 1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BALANCE]);

    p_info[P_BALANCE] = 79;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BALANCE]);

    p_info[P_BALANCE] = 161;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BALANCE]);

    p_info[P_BALANCE] = 80;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(8, p_info[P_BALANCE]);

    p_info[P_BALANCE] = 160;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BALANCE]);

    p_info[P_BALANCE] = 122;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(12, p_info[P_BALANCE]);

    p_info[P_BALANCE] = 99;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(10, p_info[P_BALANCE]);

}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesForBetIfBalanceZero) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    table_info.bblind = 10;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BALANCE] = 0;

    p_info[P_BET] = 1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BET]);

    p_info[P_BET] = 79;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BET]);

    p_info[P_BET] = 188;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(16, p_info[P_BET]);

    p_info[P_BET] = 136;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(14, p_info[P_BET]);

    p_info[P_BET] = 84;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(8, p_info[P_BET]);

    p_info[P_BALANCE] = -1;
    p_info[P_BET] = 84;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0, p_info[P_BALANCE]);
    EXPECT_EQ(8, p_info[P_BET]);

}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesForBetAndBalanceOnSB) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    table_info.bblind = 10;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    
    p_info[P_BET] = 5;
    p_info[P_BALANCE] = 1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0.5, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 5;
    p_info[P_BALANCE] = 75;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0.5, p_info[P_BET]);
    EXPECT_EQ(7.5, p_info[P_BALANCE]);

    p_info[P_BET] = 5;
    p_info[P_BALANCE] = 155;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0.5, p_info[P_BET]);
    EXPECT_EQ(15.5, p_info[P_BALANCE]);

    p_info[P_BET] = 5;
    p_info[P_BALANCE] = 156;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0.5, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 5;
    p_info[P_BALANCE] = 101;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(0.5, p_info[P_BET]);
    EXPECT_EQ(10, p_info[P_BALANCE]);

}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesForBetAndBalanceOnBB) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    table_info.bblind = 10;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    
    p_info[P_BET] = 10;
    p_info[P_BALANCE] = 1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 10;
    p_info[P_BALANCE] = 75;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(8, p_info[P_BALANCE]);

    p_info[P_BET] = 10;
    p_info[P_BALANCE] = 155;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 10;
    p_info[P_BALANCE] = 145;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 10;
    p_info[P_BALANCE] = 104;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(10, p_info[P_BALANCE]);

    table_info.bblind = 4;
    p_info[P_BET] = 4;
    p_info[P_BALANCE] = 28;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(7, p_info[P_BALANCE]);


}

TEST(TableInfo_set_player_bet_and_balance, SetProperValuesForUnknownBet) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    table_info.bblind = 10;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    
    p_info[P_BET] = 3;
    p_info[P_BALANCE] = 1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 45;
    p_info[P_BALANCE] = 18;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 100;
    p_info[P_BALANCE] = 1;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(15, p_info[P_BALANCE]);

    p_info[P_BET] = 100;
    p_info[P_BALANCE] = 109;
    table_info.set_player_bet_and_balance(p_info);
    EXPECT_EQ(1, p_info[P_BET]);
    EXPECT_EQ(11, p_info[P_BALANCE]);
}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfTwoBBFoundSB0_1) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    //if big blinds next to each other
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[3][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.bblind = 10;
    table_info.table_info[3] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[2][P_BALANCE] = 10;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(10.5, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[0]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[1]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[2]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[3]);
}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfTwoBBFoundSB0_2) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    //if big blinds opposite each other
    table_info.table_info[3] = p_info;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[1] = p_info;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[0][P_ACTIVE] = 0;
    table_info.table_info[1] = p_info;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2] = p_info;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[2] = p_info;
    table_info.table_info[3][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(15.5, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2] = p_info;
    table_info.table_info[3][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[0]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[1]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[2]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[3]);

    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[2] = p_info;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[0]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[1]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[2]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[3]);
}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfTwoBBFoundSB1_1) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    //if big blinds next to each other
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();


    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();


    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();


}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfTwoBBFoundSB1_2) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    //if big blinds opposite to each other
    table_info.table_info[0] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(16, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfNoBBFoundSB0_3_4) {
    TableInfo table_info;
    table_info.bblind = 10;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 0;
    p_info[P_BALANCE] = 15;
    
    //sb_chairs.length = 0
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[0]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[1]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[2]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[3]);

    //sb_chairs.length = 3
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[3] = p_info;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[0]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[1]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[2]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[3]);

    //sb_chairs.length = 4
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[0]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[1]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[2]);
    EXPECT_EQ(p_info_empty_vector, table_info.table_info[3]);
}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfNoBBFoundSB1) {
    TableInfo table_info;
    table_info.bblind = 10;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 0;
    p_info[P_BALANCE] = 15;
    
    //sb_chairs.length = 1
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[1][P_BET] = 0;
    table_info.table_info[1][P_BALANCE] = 16;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[1][P_BET]);
    EXPECT_EQ(15, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[0][P_BET] = 0;
    table_info.table_info[0][P_BALANCE] = 14;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[0][P_BET]);
    EXPECT_EQ(14, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.table_info[3][P_BET] = 0;
    table_info.table_info[3][P_BALANCE] = 16;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[3][P_BET]);
    EXPECT_EQ(15, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.table_info[2][P_BET] = 0;
    table_info.table_info[2][P_BALANCE] = 8;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[2][P_BET]);
    EXPECT_EQ(8, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.table_info[3][P_BET] = 0;
    table_info.table_info[3][P_BALANCE] = 8;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[3][P_BET]);
    EXPECT_EQ(8, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[0][P_BET] = 0;
    table_info.table_info[0][P_BALANCE] = 8;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[0][P_BET]);
    EXPECT_EQ(8, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    //sb_chairs.length = 1 and not enough active players
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[0][P_BET] = 0;
    table_info.table_info[0][P_BALANCE] = 10;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_ACTIVE]);
    EXPECT_EQ(0, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfNoBBFoundSB2) {
    TableInfo table_info;
    table_info.bblind = 10;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 0.5;
    p_info[P_BALANCE] = 15.5;
    
    //sb_chairs.length = 2
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[1][P_BET]);
    EXPECT_EQ(15, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BALANCE] = 10;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[2][P_BET]);
    EXPECT_EQ(9.5, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BALANCE] = 14;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[3][P_BET]);
    EXPECT_EQ(13.5, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BALANCE] = 7.5;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[0][P_BET]);
    EXPECT_EQ(7, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[2] = p_info;
    table_info.table_info[3][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[2][P_BET]);
    EXPECT_EQ(15, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[1][P_ACTIVE] = 1;
    table_info.table_info[2] = p_info;
    table_info.table_info[3][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[0][P_BET]);
    EXPECT_EQ(15, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BALANCE] = 10;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[3][P_BET]);
    EXPECT_EQ(9.5, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BALANCE] = 7.5;
    table_info.table_info[2][P_ACTIVE] = 1;
    table_info.table_info[3] = p_info;
    table_info.table_info[0][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(1, table_info.table_info[1][P_BET]);
    EXPECT_EQ(7, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfOneBBFoundSB0_1) {
    TableInfo table_info;
    table_info.bblind = 10;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;
    
    //previous player active
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(15, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(15, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(15, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(15, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    //previous player not active, but before previous is active
    table_info.table_info[3] = p_info;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(15, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[3][P_ACTIVE] = 0;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(15, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    //2 previous players not active
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    table_info.table_info[3][P_ACTIVE] = 0;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(15, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();


    //not enough active players
    table_info.table_info[1][P_ACTIVE] = 0;
    table_info.table_info[1][P_BET] = 10;
    table_info.table_info[0][P_ACTIVE] = 1;
    table_info.table_info[0][P_BET] = 1;
    table_info.table_info[0][P_BALANCE] = 10;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_ACTIVE]);
    EXPECT_EQ(0, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfOneBBFoundSB0_2) {
    TableInfo table_info;
    table_info.bblind = 4;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;
    
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 32;
    table_info.table_info[0][P_BALANCE] = 0;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 4;
    table_info.table_info[2][P_BALANCE] = 28;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0;
    table_info.table_info[3][P_BALANCE] = 64;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(8, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    EXPECT_EQ(POSITION_SB, table_info.table_info[0][P_POSITION]);
    EXPECT_EQ(1, table_info.table_info[0][P_ISALLIN]);
    EXPECT_EQ(1, table_info.table_info[2][P_BET]);
    EXPECT_EQ(7, table_info.table_info[2][P_BALANCE]);
    EXPECT_EQ(POSITION_BB, table_info.table_info[2][P_POSITION]);
    EXPECT_EQ(0, table_info.table_info[2][P_ISALLIN]);
    EXPECT_EQ(0, table_info.table_info[3][P_BET]);
    EXPECT_EQ(16, table_info.table_info[3][P_BALANCE]);
    EXPECT_EQ(POSITION_BU, table_info.table_info[3][P_POSITION]);
    table_info.clear_table_info();

}


TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfOneBBFoundSB2) {
    TableInfo table_info;
    table_info.bblind = 10;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;
    
    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();


    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    //unknown sb_chairs value
    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_blinds_when_one_bb_found("2", "31");
    EXPECT_EQ(0, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_ACTIVE]);
    EXPECT_EQ(0, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfOneBBFoundSB3) {
    TableInfo table_info;
    table_info.bblind = 10;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;
    
    table_info.table_info[3] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    EXPECT_EQ(15.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    EXPECT_EQ(15.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    EXPECT_EQ(15.5, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(15.5, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    EXPECT_EQ(15.5, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfThreeBBFound) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    table_info.clear_table_info();
}

TEST(TableInfo_set_bet_values_on_blinds, SetProperValuesIfFourBBFound) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    table_info.table_info[1] = p_info;
    table_info.table_info[2] = p_info;
    table_info.table_info[3] = p_info;
    table_info.set_bet_values_on_blinds();
    EXPECT_EQ(0, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();
}

TEST(TableInfo_set_all_chairs_positions, SetBBPosition) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    EXPECT_EQ(true, table_info.set_bb_position());
    EXPECT_EQ(POSITION_BB, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    EXPECT_EQ(false, table_info.set_bb_position());
    EXPECT_EQ(0, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_ACTIVE] = 0;
    EXPECT_EQ(false, table_info.set_bb_position());
    EXPECT_EQ(0, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_all_chairs_positions, SetSBPosition) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 0.5;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    EXPECT_EQ(true, table_info.set_sb_position());
    EXPECT_EQ(POSITION_SB, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 1;
    EXPECT_EQ(false, table_info.set_sb_position());
    EXPECT_EQ(0, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_ACTIVE] = 0;
    EXPECT_EQ(false, table_info.set_sb_position());
    EXPECT_EQ(0, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_all_chairs_positions, SetBUPositionIf3or4Players) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_POSITION] = POSITION_BB;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[3][P_POSITION] = POSITION_SB;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[2][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_POSITION] = POSITION_BB;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[3][P_POSITION] = POSITION_SB;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[1][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_POSITION] = POSITION_BB;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[0][P_POSITION] = POSITION_SB;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[3][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_POSITION] = POSITION_BB;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[0][P_POSITION] = POSITION_SB;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[2][P_POSITION]);
    table_info.clear_table_info();


    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_POSITION] = POSITION_BB;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[1][P_POSITION] = POSITION_SB;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_POSITION] = POSITION_BB;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0.5;
    table_info.table_info[1][P_POSITION] = POSITION_SB;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[3][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_POSITION] = POSITION_BB;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[2][P_POSITION] = POSITION_SB;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[1][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_POSITION] = POSITION_BB;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[2][P_POSITION] = POSITION_SB;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_BU, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();


}

TEST(TableInfo_set_all_chairs_positions, SkipBUPositionIf2Players) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_POSITION] = POSITION_BB;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[3][P_POSITION] = POSITION_SB;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.table_info[2][P_BET] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(0, table_info.table_info[2][P_POSITION]);
    table_info.clear_table_info();
}

TEST(TableInfo_set_all_chairs_positions, SetCOPositionIf4Players) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_CHAIR] = 0;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0.5;
    table_info.table_info[3][P_CHAIR] = 3;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0;
    table_info.table_info[2][P_CHAIR] = 2;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    table_info.table_info[1][P_CHAIR] = 1;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_CO, table_info.table_info[1][P_POSITION]);
    table_info.clear_table_info();

    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_CHAIR] = 3;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0.5;
    table_info.table_info[2][P_CHAIR] = 2;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    table_info.table_info[1][P_CHAIR] = 1;
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0;
    table_info.table_info[0][P_CHAIR] = 0;
    EXPECT_EQ(true, table_info.set_all_chairs_positions());
    EXPECT_EQ(POSITION_CO, table_info.table_info[0][P_POSITION]);
    table_info.clear_table_info();

}

TEST(TableInfo_set_all_chairs_positions, SetCOPositionIfWrongInfo) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    // no sb test
    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_CHAIR] = 0;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 0;
    table_info.table_info[3][P_CHAIR] = 3;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 0;
    table_info.table_info[2][P_CHAIR] = 2;
    table_info.table_info[2][P_ACTIVE] = 0;
    table_info.table_info[1] = p_info;
    table_info.table_info[1][P_BET] = 0;
    table_info.table_info[1][P_CHAIR] = 1;
    table_info.table_info[1][P_ACTIVE] = 0;
    EXPECT_EQ(false, table_info.set_all_chairs_positions());
    EXPECT_EQ(0, table_info.table_info[0][P_BET]);
    EXPECT_EQ(0, table_info.table_info[0][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[2][P_BET]);
    EXPECT_EQ(0, table_info.table_info[2][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[3][P_BET]);
    EXPECT_EQ(0, table_info.table_info[3][P_BALANCE]);
    EXPECT_EQ(0, table_info.table_info[1][P_BET]);
    EXPECT_EQ(0, table_info.table_info[1][P_BALANCE]);
    table_info.clear_table_info();
}

TEST(TableInfo_set_allin_info, SetProperAllinValue) {
    TableInfo table_info;
    auto p_info = p_info_empty_vector;
    p_info[P_SEATED] = 1;
    p_info[P_ACTIVE] = 1;
    p_info[P_BET] = 1;
    p_info[P_BALANCE] = 15;

    table_info.table_info[0] = p_info;
    table_info.table_info[0][P_BET] = 0.5;
    table_info.table_info[3] = p_info;
    table_info.table_info[3][P_BET] = 8;
    table_info.table_info[3][P_BALANCE] = 0;
    table_info.table_info[3][P_POSITION] = POSITION_CO;
    table_info.table_info[2] = p_info;
    table_info.table_info[2][P_BET] = 16;
    table_info.table_info[2][P_BALANCE] = 0;
    table_info.table_info[2][P_POSITION] = POSITION_BU;
    table_info.table_info[1] = p_info;
    table_info.set_allin_info();
    EXPECT_EQ(1, table_info.table_info[3][P_ISALLIN]);
    EXPECT_EQ(1, table_info.table_info[2][P_ISALLIN]);
    EXPECT_EQ(0, table_info.table_info[1][P_ISALLIN]);
    EXPECT_EQ(0, table_info.table_info[0][P_ISALLIN]);
    table_info.clear_table_info();
}



TEST(Converter_stack_int_to_blinds, ReturnProperValuesInBB) {
    Converter conv;

    EXPECT_EQ(0, conv.stack_int_to_blinds(0, 4));
    EXPECT_EQ(0, conv.stack_int_to_blinds(-1, 4));
    EXPECT_EQ(0, conv.stack_int_to_blinds(-1, 50));
    EXPECT_EQ(1, conv.stack_int_to_blinds(4, 4));
    EXPECT_EQ(2, conv.stack_int_to_blinds(8, 4));
    EXPECT_EQ(2, conv.stack_int_to_blinds(9, 4));
    EXPECT_EQ(3, conv.stack_int_to_blinds(10, 4));
    EXPECT_EQ(3, conv.stack_int_to_blinds(11, 4));
    EXPECT_EQ(15, conv.stack_int_to_blinds(150, 10));
    EXPECT_EQ(15, conv.stack_int_to_blinds(145, 10));
    EXPECT_EQ(14, conv.stack_int_to_blinds(144, 10));
}

TEST(HelperFunctions_check_number_for_dots, ReturnProperNumberWithoutDotsFor2_4) {
    EXPECT_EQ("32", Helper::check_number_for_dots("32"));
    EXPECT_EQ("64", Helper::check_number_for_dots("64"));
    EXPECT_EQ("156", Helper::check_number_for_dots("15698"));
    EXPECT_EQ("101", Helper::check_number_for_dots("10144"));
    EXPECT_EQ("201", Helper::check_number_for_dots("20199"));
    EXPECT_EQ("102", Helper::check_number_for_dots("10256"));
    EXPECT_EQ("70", Helper::check_number_for_dots("7048"));
    EXPECT_EQ("79", Helper::check_number_for_dots("7924"));
    EXPECT_EQ("89", Helper::check_number_for_dots("8948"));
    EXPECT_EQ("99", Helper::check_number_for_dots("9948"));
    EXPECT_EQ("80", Helper::check_number_for_dots("8025"));
    EXPECT_EQ("100", Helper::check_number_for_dots("1009"));
    EXPECT_EQ("190", Helper::check_number_for_dots("1902"));
    EXPECT_EQ("256", Helper::check_number_for_dots("2569"));
    EXPECT_EQ("38", Helper::check_number_for_dots("3897"));
    EXPECT_EQ("48", Helper::check_number_for_dots("4897"));
    EXPECT_EQ("70", Helper::check_number_for_dots("709"));
    EXPECT_EQ("78", Helper::check_number_for_dots("785"));
    EXPECT_EQ("88", Helper::check_number_for_dots("885"));
    EXPECT_EQ("92", Helper::check_number_for_dots("925"));
    EXPECT_EQ("115", Helper::check_number_for_dots("115"));
    EXPECT_EQ("230", Helper::check_number_for_dots("230"));
    EXPECT_EQ("33", Helper::check_number_for_dots("330"));
    EXPECT_EQ("46", Helper::check_number_for_dots("460"));

}

//TEST(TableInfo_set_player_currentbet_and_balance, SetProperBetValues) {
//    TableInfo table_info;
//    auto p_info = p_info_empty_vector;
//
//
//    table_info.set_player_currentbet_and_balance(p_info,0);
//    EXPECT_EQ(0, table_info.table_info[0][0]);
//    EXPECT_EQ(0, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({0, -1},0);
//    EXPECT_EQ(0, table_info.table_info[0][0]);
//    EXPECT_EQ(0, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({-1, -1},0);
//    EXPECT_EQ(0, table_info.table_info[0][0]);
//    EXPECT_EQ(0, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({-1, 0},0);
//    EXPECT_EQ(0, table_info.table_info[0][0]);
//    EXPECT_EQ(0, table_info.table_info[0][1]);
//
//    table_info.set_big_blind(10);
//    table_info.set_player_currentbet_and_balance({10, 70},0);
//    EXPECT_EQ (1, table_info.table_info[0][0]);
//    EXPECT_EQ (7, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({10, 69},0);
//    EXPECT_EQ (1, table_info.table_info[0][0]);
//    EXPECT_EQ (15, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({10, 115},0);
//    EXPECT_EQ (1, table_info.table_info[0][0]);
//    EXPECT_EQ (11.5, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({10, 150},0);
//    EXPECT_EQ (1, table_info.table_info[0][0]);
//    EXPECT_EQ (15, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({10, 154},0);
//    EXPECT_EQ (1, table_info.table_info[0][0]);
//    EXPECT_EQ (15, table_info.table_info[0][1]);
//
//    table_info.set_player_currentbet_and_balance({5, 1},0);
//    EXPECT_EQ (0.5, table_info.table_info[0][0]);
//    EXPECT_EQ (15.5, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({5, 75},0);
//    EXPECT_EQ (0.5, table_info.table_info[0][0]);
//    EXPECT_EQ (7.5, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({5, 95},0);
//    EXPECT_EQ (0.5, table_info.table_info[0][0]);
//    EXPECT_EQ (9.5, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({5, 154},0);
//    EXPECT_EQ (0.5, table_info.table_info[0][0]);
//    EXPECT_EQ (15.4, table_info.table_info[0][1]);
//    table_info.set_player_currentbet_and_balance({5, 160},0);
//    EXPECT_EQ (0.5, table_info.table_info[0][0]);
//    EXPECT_EQ (15.5, table_info.table_info[0][1]);
//
//
//    //table_info.set_player_currentbet_and_balance({0, 160},0);
//    //EXPECT_EQ (0.5, table_info.table_info[0][0]);
//    //EXPECT_EQ (15.5, table_info.table_info[0][1]);
//
//
//
//}

//TEST(Table_get_raiser_eff_stack, ReturnProperEffStackForCORaiser) {
//    Table table;
//    EXPECT_EQ(80, table.player_eff_stack_for_first_to_act(POSITION_CO, 80, 129, 90, 110));
//    EXPECT_EQ(80, table.player_eff_stack_for_first_to_act(POSITION_CO, 80, 129, 90, 70));
//    EXPECT_EQ(80, table.player_eff_stack_for_first_to_act(POSITION_CO, 80, 129, 79, 70));
//    EXPECT_EQ(129, table.player_eff_stack_for_first_to_act(POSITION_CO, 160, 129, 79, 70));
//    EXPECT_EQ(129, table.player_eff_stack_for_first_to_act(POSITION_CO, 129, 129, 79, 70));
//    EXPECT_EQ(110, table.player_eff_stack_for_first_to_act(POSITION_CO, 160, 100, 110, 70));
//    EXPECT_EQ(120, table.player_eff_stack_for_first_to_act(POSITION_CO, 160, 100, 110, 120));
//    EXPECT_EQ(110, table.player_eff_stack_for_first_to_act(POSITION_CO, 160, 100, 110, 105));
//    EXPECT_EQ(110, table.player_eff_stack_for_first_to_act(POSITION_CO, 160, 100, 110, 110));
//    EXPECT_EQ(160, table.player_eff_stack_for_first_to_act(POSITION_CO, 160, 160, 110, 110));
//    EXPECT_EQ(135, table.player_eff_stack_for_first_to_act(POSITION_CO, 160, 125, 135, 125));
//    EXPECT_EQ(110, table.player_eff_stack_for_first_to_act(POSITION_CO, 110, 80, 105, 125));
//    EXPECT_EQ(110, table.player_eff_stack_for_first_to_act(POSITION_CO, 110, 80, 135, 125));
//    EXPECT_EQ(135, table.player_eff_stack_for_first_to_act(POSITION_CO, 140, 125, 135, 125));
//    EXPECT_EQ(125, table.player_eff_stack_for_first_to_act(POSITION_CO, 125, 125, 135, 125));
//    EXPECT_EQ(135, table.player_eff_stack_for_first_to_act(POSITION_CO, 135, 125, 135, 125));
//    EXPECT_EQ(135, table.player_eff_stack_for_first_to_act(POSITION_CO, 135, 135, 125, 135));
//    EXPECT_EQ(135, table.player_eff_stack_for_first_to_act(POSITION_CO, 135, 135, 145, 135));
//};
//
//TEST(Table_get_raiser_eff_stack, ReturnProperEffStackForBURaiser) {
//    Table table;
//    EXPECT_EQ(80, table.player_eff_stack_for_first_to_act(POSITION_BU, 140, 80, 90, 110));
//    EXPECT_EQ(120, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 90, 130));
//    EXPECT_EQ(110, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 90, 110));
//    EXPECT_EQ(115, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 115, 110));
//    EXPECT_EQ(120, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 140, 130));
//    EXPECT_EQ(120, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 130, 130));
//    EXPECT_EQ(120, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 130, 140));
//    EXPECT_EQ(120, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 120, 120));
//    EXPECT_EQ(120, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 105, 120));
//    EXPECT_EQ(105, table.player_eff_stack_for_first_to_act(POSITION_BU, 80, 120, 105, 105));
//    EXPECT_EQ(80, table.player_eff_stack_for_first_to_act(POSITION_BU, 0, 80, 79, 148));
//}
//
//TEST(Table_get_raiser_eff_stack, ReturnProperEffStackForSBRaiser) {
//    Table table;
//    EXPECT_EQ(80, table.player_eff_stack_for_first_to_act(POSITION_SB, 140, 80, 80, 110));
//    EXPECT_EQ(110, table.player_eff_stack_for_first_to_act(POSITION_SB, 140, 80, 115, 110));
//    EXPECT_EQ(115, table.player_eff_stack_for_first_to_act(POSITION_SB, 140, 80, 115, 115));
//}
//
//TEST(Table_get_my_eff_stack_for_one_allin, ReturnEffStackForOneAllinFromCO) {
//    Table table;
//    EXPECT_EQ(80, table.hero_eff_stack_for_one_allin(140, 80, 80, 110,POSITION_BU,POSITION_CO));
//    EXPECT_EQ(120, table.hero_eff_stack_for_one_allin(120, 140, 80, 110,POSITION_BU,POSITION_CO));
//    EXPECT_EQ(100, table.hero_eff_stack_for_one_allin(90, 140, 100, 80,POSITION_BU,POSITION_CO));
//    EXPECT_EQ(100, table.hero_eff_stack_for_one_allin(90, 140, 80, 100,POSITION_BU,POSITION_CO));
//    EXPECT_EQ(130, table.hero_eff_stack_for_one_allin(110, 140, 120, 130,POSITION_BU,POSITION_CO));
//    EXPECT_EQ(125, table.hero_eff_stack_for_one_allin(110, 140, 125, 120,POSITION_BU,POSITION_CO));
//
//    EXPECT_EQ(125, table.hero_eff_stack_for_one_allin(160, 140, 125, 120,POSITION_SB,POSITION_CO));
//    EXPECT_EQ(140, table.hero_eff_stack_for_one_allin(80, 130, 140, 160,POSITION_SB,POSITION_CO));
//    EXPECT_EQ(130, table.hero_eff_stack_for_one_allin(130, 140, 145, 120,POSITION_SB,POSITION_CO));
//    EXPECT_EQ(120, table.hero_eff_stack_for_one_allin(80, 130, 145, 120,POSITION_SB,POSITION_CO));
//    EXPECT_EQ(80, table.hero_eff_stack_for_one_allin(100, 130, 80, 120,POSITION_SB,POSITION_CO));
//
//    EXPECT_EQ(80, table.hero_eff_stack_for_one_allin(100, 130, 80, 80,POSITION_BB,POSITION_CO));
//    EXPECT_EQ(100, table.hero_eff_stack_for_one_allin(100, 130, 130, 120,POSITION_BB,POSITION_CO));
//    EXPECT_EQ(120, table.hero_eff_stack_for_one_allin(120, 130, 80, 120,POSITION_BB,POSITION_CO));
//}
//
//TEST(Table_get_my_eff_stack_for_one_allin, ReturnEffStackForOneAllinFromBU) {
//    Table table;
//
//    EXPECT_EQ(80, table.hero_eff_stack_for_one_allin(160, 80, 160, 80,POSITION_SB,POSITION_BU));
//    EXPECT_EQ(140, table.hero_eff_stack_for_one_allin(80, 130, 140, 160,POSITION_SB,POSITION_BU));
//    EXPECT_EQ(140, table.hero_eff_stack_for_one_allin(130, 140, 145, 120,POSITION_SB,POSITION_BU));
//    EXPECT_EQ(145, table.hero_eff_stack_for_one_allin(80, 130, 145, 160,POSITION_SB,POSITION_BU));
//    EXPECT_EQ(80, table.hero_eff_stack_for_one_allin(100, 130, 80, 120,POSITION_SB,POSITION_BU));
//
//    EXPECT_EQ(80, table.hero_eff_stack_for_one_allin(100, 130, 80, 80,POSITION_BB,POSITION_BU));
//    EXPECT_EQ(100, table.hero_eff_stack_for_one_allin(100, 100, 130, 120,POSITION_BB,POSITION_BU));
//    EXPECT_EQ(120, table.hero_eff_stack_for_one_allin(120, 120, 80, 120,POSITION_BB,POSITION_BU));
//}
//
//TEST(Table_get_my_eff_stack_for_one_allin, ReturnEffStackForOneAllinFromSB) {
//    Table table;
//
//    EXPECT_EQ(80, table.hero_eff_stack_for_one_allin(100, 130, 80, 80,POSITION_BB,POSITION_SB));
//    EXPECT_EQ(120, table.hero_eff_stack_for_one_allin(100, 100, 130, 120,POSITION_BB,POSITION_SB));
//    EXPECT_EQ(90, table.hero_eff_stack_for_one_allin(120, 120, 90, 120,POSITION_BB,POSITION_SB));
//}
//
//TEST(Table_get_my_eff_stack_for_two_allins, ReturnEffStackForTwoAllinFromCOandBU) {
//    Table table;
//
//    EXPECT_EQ(80, table.hero_eff_stack_for_two_allins(100, 130, 80, 80,POSITION_SB,POSITION_CO_BU));
//    EXPECT_EQ(140, table.hero_eff_stack_for_two_allins(140, 130, 160, 80,POSITION_SB,POSITION_CO_BU));
//    EXPECT_EQ(145, table.hero_eff_stack_for_two_allins(130, 145, 160, 80,POSITION_SB,POSITION_CO_BU));
//    EXPECT_EQ(100, table.hero_eff_stack_for_two_allins(90, 100, 160, 100,POSITION_SB,POSITION_CO_BU));
//    EXPECT_EQ(110, table.hero_eff_stack_for_two_allins(90, 100, 160, 110,POSITION_SB,POSITION_CO_BU));
//}
//
//TEST(Table_get_my_eff_stack_for_two_allins, ReturnEffStackForTwoAllinFromCOandSB) {
//    Table table;
//
//    EXPECT_EQ(80, table.hero_eff_stack_for_two_allins(100, 130, 80, 80,POSITION_BB,POSITION_CO_SB));
//    EXPECT_EQ(80, table.hero_eff_stack_for_two_allins(140, 130, 160, 80,POSITION_BB,POSITION_CO_SB));
//    EXPECT_EQ(140, table.hero_eff_stack_for_two_allins(130, 145, 160, 140,POSITION_BB,POSITION_CO_SB));
//    EXPECT_EQ(120, table.hero_eff_stack_for_two_allins(120, 80, 80, 140,POSITION_BB,POSITION_CO_SB));
//    EXPECT_EQ(160, table.hero_eff_stack_for_two_allins(90, 100, 160, 160,POSITION_BB,POSITION_CO_SB));
//    EXPECT_EQ(130, table.hero_eff_stack_for_two_allins(90, 100, 130, 160,POSITION_BB,POSITION_CO_SB));
//}
//
//TEST(Table_get_my_eff_stack_for_two_allins, ReturnEffStackForTwoAllinFromBUandSB) {
//    Table table;
//
//    EXPECT_EQ(80, table.hero_eff_stack_for_two_allins(100, 130, 80, 80,POSITION_BB,POSITION_BU_SB));
//    EXPECT_EQ(80, table.hero_eff_stack_for_two_allins(140, 130, 160, 80,POSITION_BB,POSITION_BU_SB));
//    EXPECT_EQ(140, table.hero_eff_stack_for_two_allins(130, 145, 160, 140,POSITION_BB,POSITION_BU_SB));
//    EXPECT_EQ(80, table.hero_eff_stack_for_two_allins(120, 80, 80, 140,POSITION_BB,POSITION_BU_SB));
//    EXPECT_EQ(160, table.hero_eff_stack_for_two_allins(90, 100, 160, 160,POSITION_BB,POSITION_BU_SB));
//    EXPECT_EQ(130, table.hero_eff_stack_for_two_allins(90, 100, 130, 160,POSITION_BB,POSITION_BU_SB));
//}
//
//
//TEST(Table_get_stat_name, ReturnProperStatName) {
//    Table table;
//    EXPECT_EQ("pt_sb105_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_SB, CHAIR_0));
//    EXPECT_EQ("pt_sb105_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_SB, CHAIR_1));
//    EXPECT_EQ("pt_sb105_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_SB, CHAIR_2));
//    EXPECT_EQ("pt_sb105_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_SB, CHAIR_3));
//    EXPECT_EQ("pt_sb135_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_SB, CHAIR_0));
//    EXPECT_EQ("pt_sb135_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_SB, CHAIR_1));
//    EXPECT_EQ("pt_sb135_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_SB, CHAIR_2));
//    EXPECT_EQ("pt_sb135_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_SB, CHAIR_3));
//    EXPECT_EQ("pt_sb160_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_SB, CHAIR_0));
//    EXPECT_EQ("pt_sb160_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_SB, CHAIR_1));
//    EXPECT_EQ("pt_sb160_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_SB, CHAIR_2));
//    EXPECT_EQ("pt_sb160_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_SB, CHAIR_3));
//    EXPECT_EQ("pt_bu105_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_BU, CHAIR_0));
//    EXPECT_EQ("pt_bu105_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_BU, CHAIR_1));
//    EXPECT_EQ("pt_bu105_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_BU, CHAIR_2));
//    EXPECT_EQ("pt_bu105_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_BU, CHAIR_3));
//    EXPECT_EQ("pt_bu135_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_BU, CHAIR_0));
//    EXPECT_EQ("pt_bu135_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_BU, CHAIR_1));
//    EXPECT_EQ("pt_bu135_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_BU, CHAIR_2));
//    EXPECT_EQ("pt_bu135_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_BU, CHAIR_3));
//    EXPECT_EQ("pt_bu160_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_BU, CHAIR_0));
//    EXPECT_EQ("pt_bu160_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_BU, CHAIR_1));
//    EXPECT_EQ("pt_bu160_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_BU, CHAIR_2));
//    EXPECT_EQ("pt_bu160_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_BU, CHAIR_3));
//    EXPECT_EQ("pt_co105_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_CO, CHAIR_0));
//    EXPECT_EQ("pt_co105_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_CO, CHAIR_1));
//    EXPECT_EQ("pt_co105_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_CO, CHAIR_2));
//    EXPECT_EQ("pt_co105_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_105, POSITION_CO, CHAIR_3));
//    EXPECT_EQ("pt_co135_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_CO, CHAIR_0));
//    EXPECT_EQ("pt_co135_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_CO, CHAIR_1));
//    EXPECT_EQ("pt_co135_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_CO, CHAIR_2));
//    EXPECT_EQ("pt_co135_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_135, POSITION_CO, CHAIR_3));
//    EXPECT_EQ("pt_co160_rfi0", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_CO, CHAIR_0));
//    EXPECT_EQ("pt_co160_rfi1", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_CO, CHAIR_1));
//    EXPECT_EQ("pt_co160_rfi2", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_CO, CHAIR_2));
//    EXPECT_EQ("pt_co160_rfi3", table.get_stat_name(POSITION_BB, STACK_RANGE_160, POSITION_CO, CHAIR_3));
//}
//
//TEST(Table_my_position, ReturnProperTextForAllUsedStackRanges) {
//    Table table;
//    EXPECT_EQ(".........................[Table] Eff stack: [<10.5bb]\n", table.stack_range(STACK_RANGE_105));
//    EXPECT_EQ(".........................[Table] Eff stack: [10.5bb <= X < 13.5bb]\n", table.stack_range(STACK_RANGE_135));
//    EXPECT_EQ(".........................[Table] Eff stack: [>= 13.5bb]\n", table.stack_range(STACK_RANGE_160));
//    EXPECT_EQ(EMPTY_STRING, table.stack_range(0));
//    EXPECT_EQ(EMPTY_STRING, table.stack_range(-1));
//}
//
//TEST(Table_stack_range, ReturnProperTextForAllMinePositions) {
//    Table table;
//    EXPECT_EQ(".........................[Table] My position: [CO]\n", table.my_position(POSITION_CO));
//    EXPECT_EQ(".........................[Table] My position: [BU]\n", table.my_position(POSITION_BU));
//    EXPECT_EQ(".........................[Table] My position: [SB]\n", table.my_position(POSITION_SB));
//    EXPECT_EQ(".........................[Table] My position: [BB]\n", table.my_position(POSITION_BB));
//    EXPECT_EQ(EMPTY_STRING, table.my_position(99));
//    EXPECT_EQ(EMPTY_STRING, table.my_position(-1));
//}
//
//TEST(Table_one_allin_position, ReturnProperTextForAllOneAllInPositions) {
//    Table table;
//    EXPECT_EQ(".........................[Table] One AllIn position: [CO]\n", table.one_allin_position(POSITION_CO));
//    EXPECT_EQ(".........................[Table] One AllIn position: [BU]\n", table.one_allin_position(POSITION_BU));
//    EXPECT_EQ(".........................[Table] One AllIn position: [SB]\n", table.one_allin_position(POSITION_SB));
//    EXPECT_EQ(EMPTY_STRING, table.one_allin_position(99));
//    EXPECT_EQ(EMPTY_STRING, table.one_allin_position(-1));
//}
//
//TEST(Table_get_one_all_in_position_chair_symbol, ReturnProperChairSymbolForOneAllInPositions) {
//    Table table;
//    EXPECT_EQ("SBchair", table.get_one_allin_position_chair_symbol(POSITION_SB));
//    EXPECT_EQ("BUchair", table.get_one_allin_position_chair_symbol(POSITION_BU));
//    EXPECT_EQ("COchair", table.get_one_allin_position_chair_symbol(POSITION_CO));
//    EXPECT_EQ(EMPTY_STRING, table.get_one_allin_position_chair_symbol(99));
//    EXPECT_EQ(EMPTY_STRING, table.get_one_allin_position_chair_symbol(-1));
//}
//
//TEST(Table_one_all_in_position_chair, ReturnProperChairNumberForOneAllInPositions) {
//    Table table;
//    EXPECT_EQ(".........................[Table] One AllIn position table chair: [0]\n", table.one_allin_position_chair(CHAIR_0));
//    EXPECT_EQ(".........................[Table] One AllIn position table chair: [1]\n", table.one_allin_position_chair(CHAIR_1));
//    EXPECT_EQ(".........................[Table] One AllIn position table chair: [2]\n", table.one_allin_position_chair(CHAIR_2));
//    EXPECT_EQ(".........................[Table] One AllIn position table chair: [3]\n", table.one_allin_position_chair(CHAIR_3));
//    EXPECT_EQ(EMPTY_STRING, table.one_allin_position_chair(99));
//    EXPECT_EQ(EMPTY_STRING, table.one_allin_position_chair(-1));
//}
//
//TEST(Table_evaluate_sample, ReturnProperInfoAboutSample) {
//    Table table;
//    const auto min_samples_needed = 10;
//    EXPECT_EQ(".........................[Table] No samples at all. Skipping exploit bot...\n", table.evaluate_sample(-1, min_samples_needed));
//    EXPECT_EQ(".........................[Table] No samples at all. Skipping exploit bot...\n", table.evaluate_sample(0, min_samples_needed));
//    EXPECT_EQ(".........................[Table] No enough samples. Skipping exploit bot...\n", table.evaluate_sample(1, min_samples_needed));
//    EXPECT_EQ(".........................[Table] No enough samples. Skipping exploit bot...\n", table.evaluate_sample(9, min_samples_needed));
//    EXPECT_EQ(".........................[Table] Sample size is OK. Starting exploit bot...\n", table.evaluate_sample(10, min_samples_needed));
//    EXPECT_EQ(".........................[Table] Sample size is OK. Starting exploit bot...\n", table.evaluate_sample(12, min_samples_needed));
//}
//
//TEST(Table_evaluate_equity_result, ReturnProperInfoAboutEquityEvaluation) {
//    Table table;
//    EXPECT_EQ("..........................[Table] Enough equity to continue. Returning TRUE.\n", table.evaluate_equity_result(0.32564, 0.312666));
//    EXPECT_EQ("..........................[Table] Not enough equity to continue. Returning FALSE.\n", table.evaluate_equity_result(0.32564, 0.32564));
//    EXPECT_EQ("..........................[Table] Not enough equity to continue. Returning FALSE.\n", table.evaluate_equity_result(0.32564, 0.33564));
//}
//
//
//
//TEST(Range_evaluate_stat_percent, Returen9999IfStatPercentIs100) {
//    Range range;
//    EXPECT_EQ(0.99999, range.evaluate_stat_percent(1));
//    EXPECT_EQ(0.99999, range.evaluate_stat_percent(0.99999));
//    EXPECT_EQ(0.3564189, range.evaluate_stat_percent(0.3564189));
//}
//
//TEST(Range_get_full_range_order, ReturenProperFullRange) {
//    Range range;
//    EXPECT_EQ(list105_CO__CO_AI, range.get_full_range_order("list105_CO__CO_AI"));
//    EXPECT_EQ(list105_BU__CO_F_BU_AI, range.get_full_range_order("list105_BU__CO_F_BU_AI"));
//    EXPECT_EQ(list105_SB__CO_F_BU_F_SB_AI, range.get_full_range_order("list105_SB__CO_F_BU_F_SB_AI"));
//    EXPECT_EQ(list135_CO__CO_AI, range.get_full_range_order("list135_CO__CO_AI"));
//    EXPECT_EQ(list135_BU__CO_F_BU_AI, range.get_full_range_order("list135_BU__CO_F_BU_AI"));
//    EXPECT_EQ(list135_SB__CO_F_BU_F_SB_AI, range.get_full_range_order("list135_SB__CO_F_BU_F_SB_AI"));
//    EXPECT_EQ(list160_CO__CO_AI, range.get_full_range_order("list160_CO__CO_AI"));
//    EXPECT_EQ(list160_BU__CO_F_BU_AI, range.get_full_range_order("list160_BU__CO_F_BU_AI"));
//    EXPECT_EQ(list160_SB__CO_F_BU_F_SB_AI, range.get_full_range_order("list160_SB__CO_F_BU_F_SB_AI"));
//
//}
//
//TEST(Range_get_string_percent, ReturnProperString) {
//    Range range;
//    EXPECT_EQ("-1", range.get_string_percent(-1));
//    EXPECT_EQ("100.00", range.get_string_percent(1.01));
//    EXPECT_EQ("100.00", range.get_string_percent(1));
//    EXPECT_EQ("100.00", range.get_string_percent(0.99996));
//    EXPECT_EQ("99.97", range.get_string_percent(0.99966));
//    EXPECT_EQ("55.97", range.get_string_percent(0.55969544));
//    EXPECT_EQ("01.97", range.get_string_percent(0.01967584));
//    EXPECT_EQ("00.45", range.get_string_percent(0.004511584));
//    EXPECT_EQ("00.01", range.get_string_percent(0.000141584));
//    EXPECT_EQ("00.01", range.get_string_percent(0.00005));
//    EXPECT_EQ("00.00", range.get_string_percent(0.00000));
//}
//
//TEST(Range_get_range_name, ReturnProperString) {
//    Range range;
//    EXPECT_EQ("-1", range.get_range_name("-1"));
//    EXPECT_EQ("list10000X", range.get_range_name("100.00"));
//    EXPECT_EQ("list0000X", range.get_range_name("00.00"));
//    EXPECT_EQ("list9997X", range.get_range_name("99.97"));
//    EXPECT_EQ("list5597X", range.get_range_name("55.97"));
//    EXPECT_EQ("list0197X", range.get_range_name("01.97"));
//    EXPECT_EQ("list0045X", range.get_range_name("00.45"));
//    EXPECT_EQ("list0001X", range.get_range_name("00.01"));
//}
//
//TEST(Range_get_combo_percent, ReturnProperPercent) {
//    Range range;
//    EXPECT_EQ(0.452, DIFF_PAIR);
//    EXPECT_EQ(0.301, DIFF_SUITED);
//    EXPECT_EQ(0.904, DIFF_OFFSUITED);
//    EXPECT_EQ(DIFF_PAIR, range.get_combo_percent(COMBO_TYPE_PAIR));
//    EXPECT_EQ(DIFF_SUITED, range.get_combo_percent(COMBO_TYPE_SUITED));
//    EXPECT_EQ(DIFF_OFFSUITED, range.get_combo_percent(COMBO_TYPE_OFFSUITED));
//    EXPECT_EQ(0, range.get_combo_percent(""));
//    EXPECT_EQ(0, range.get_combo_percent("A"));
//    EXPECT_EQ(0, range.get_combo_percent("fff"));
//}
//
//TEST(Range_get_range_combos, ReturnProperRangeCombos) {
//    Range range;
//    std::string list =  "AA KK QQ JJ TT AKs AKo AQs 99 AQo AJs ATs 88 AJo ATo \n"
//                        "77 A9s KQs A8s 66 A9o KJs KQo A7s 55 A8o KTs A6s A5s KJo \n"
//                        "A4s A7o 44 A3s KTo A2s K9s A6o A5o A4o 33 QJs A3o K8s K9o \n"
//                        "A2o QTs 22 K7s QJo K6s K8o JTs Q9s QTo K5s K7o K4s K3s J9s \n"
//                        "K6o Q8s K2s JTo T9s Q9o K5o 98s T8s J8s Q7s K4o 87s Q6s K3o \n"
//                        "97s 76s Q5s J9o T7s Q8o K2o J7s T9o 86s Q4s 65s 96s Q3s 75s \n"
//                        "54s T6s Q2s J6s 98o 85s J5s T8o 64s J8o Q7o 95s 87o J4s Q6o \n"
//                        "74s T5s 53s 97o 76o J3s Q5o T4s T7o 84s 43s J2s J7o Q4o 63s \n"
//                        "T3s 86o 65o 94s T2s 96o 93s 52s 73s Q3o 75o 54o T6o 92s Q2o \n"
//                        "42s J6o 83s 62s 85o 82s J5o 64o 32s 95o 72s J4o 74o T5o 53o \n"
//                        "J3o T4o 84o 43o J2o 63o T3o 94o T2o 93o 52o 73o 92o 42o 83o \n"
//                        "62o 82o 32o 72o ";
//    EXPECT_EQ(EMPTY_STRING, range.get_range_combos(0, list105_SB__CO_F_BU_F_SB_AI));
//    EXPECT_EQ(list, range.get_range_combos(1, list105_SB__CO_F_BU_F_SB_AI));
//}
//
//TEST(Range_add_combo, ReturnProperRangeWithNewCombo) {
//    Range range;
//    std::string range_combos;
//    range.add_combo(range_combos, "AAp");
//    EXPECT_EQ("AA ", range_combos);
//    range.add_combo(range_combos, "KKp");
//    EXPECT_EQ("AA KK ", range_combos);
//    range.add_combo(range_combos, "AKs");
//    EXPECT_EQ("AA KK AKs ", range_combos);
//    range.add_combo(range_combos, "AKo");
//    EXPECT_EQ("AA KK AKs AKo ", range_combos);
//}
//
//TEST(Range_add_combo, ReturnTrueIfProperCombo) {
//    Range range;
//    std::string range_combos;
//    auto result = range.add_combo(range_combos, "AAp");
//    EXPECT_EQ(true, result);
//    result = range.add_combo(range_combos, "AKs");
//    EXPECT_EQ(true, result);
//    result = range.add_combo(range_combos, "AKo");
//    EXPECT_EQ(true, result);
//    result = range.add_combo(range_combos, " AKs");
//    EXPECT_EQ(true, result);
//    result = range.add_combo(range_combos, " AKo ");
//    EXPECT_EQ(true, result);
//}
//
//TEST(Range_add_combo, ReturnFalseIfWrongCombo) {
//    Range range;
//    std::string range_combos;
//    const auto result = range.add_combo(range_combos, "    ");
//    EXPECT_EQ(false, result);
//}
//
//TEST(Range_get_range_correction, ReturnCorrectionsForRange) {
//    Range range;
//    std::string range_combos;
//    const auto result = range.add_combo(range_combos, "    ");
//    EXPECT_EQ(false, result);
//}
//
//TEST(Range_evaluate_hand_for_allin, ReturnTrueIfComboFound) {
//    Range range;
//    std::vector<std::string> allin_info = {"A2o" , "1.000", "0.781"};
//    EXPECT_EQ(true, range.check_combo_for_allin(allin_info, 8));
//    allin_info = {"95o" , "0.000", "-0.182"};
//    EXPECT_EQ(false, range.check_combo_for_allin(allin_info, 8));
//    allin_info = {"T4s" , "1.000", "0.028"};
//    EXPECT_EQ(true, range.check_combo_for_allin(allin_info, 8));
//    EXPECT_EQ(false, range.check_combo_for_allin(allin_info, 10));
//    allin_info = {"74s" , "1.000", "0.059"};
//    EXPECT_EQ(true, range.check_combo_for_allin(allin_info, 15));
//    EXPECT_EQ(false, range.check_combo_for_allin(allin_info, 16));
//    allin_info = {"95o" , "1.000", "0.025"};
//    EXPECT_EQ(false, range.check_combo_for_allin(allin_info, 8));
//
//}
//
//TEST(Range_get_combo_allin_info, ReturnFullRowWithAllInInfo) {
//    Range range;
//    const std::string file_name = R"(C:\Poker\soft\toUse\PKA\profile\gto\sb\10\co_f_bu_f_sb_ai.csv)";
//    std::vector<std::string> line_vector = {"AA" , "1.000", "3.353"};
//    EXPECT_EQ(line_vector, range.combo_allin_info(file_name, "AA"));
//    line_vector = {"A2o" , "1.000", "0.781"};
//    EXPECT_EQ(line_vector, range.combo_allin_info(file_name, "A2o"));
//    line_vector = {"K7s" , "1.000", "0.632"};
//    EXPECT_EQ(line_vector, range.combo_allin_info(file_name, "K7s"));
//    line_vector = {"95o" , "0.000", "-0.182"};
//    EXPECT_EQ(line_vector, range.combo_allin_info(file_name, "95o"));
//    line_vector = {"" , "0.000", "-100"};
//    EXPECT_EQ(line_vector, range.combo_allin_info(file_name, "zzz"));
//
//}
//
//TEST(Range_get_full_range_order_name, ReturnProperOneAllinRangeOrderName) {
//    Range range;
//    //EXPECT_EQ("list105_CO_", range.get_full_range_order_name("pt_co105_rfi0"));
//    EXPECT_EQ("list105_CO__CO_AI", range.get_full_range_order_name("pt_co105_rfi0"));
//    EXPECT_EQ("list105_CO__CO_AI", range.get_full_range_order_name("pt_co105_rfi1"));
//    EXPECT_EQ("list105_CO__CO_AI", range.get_full_range_order_name("pt_co105_rfi2"));
//    EXPECT_EQ("list105_CO__CO_AI", range.get_full_range_order_name("pt_co105_rfi3"));
//    EXPECT_EQ("list135_CO__CO_AI", range.get_full_range_order_name("pt_co135_rfi0"));
//    EXPECT_EQ("list135_CO__CO_AI", range.get_full_range_order_name("pt_co135_rfi1"));
//    EXPECT_EQ("list135_CO__CO_AI", range.get_full_range_order_name("pt_co135_rfi2"));
//    EXPECT_EQ("list135_CO__CO_AI", range.get_full_range_order_name("pt_co135_rfi3"));
//    EXPECT_EQ("list160_CO__CO_AI", range.get_full_range_order_name("pt_co160_rfi0"));
//    EXPECT_EQ("list160_CO__CO_AI", range.get_full_range_order_name("pt_co160_rfi1"));
//    EXPECT_EQ("list160_CO__CO_AI", range.get_full_range_order_name("pt_co160_rfi2"));
//    EXPECT_EQ("list160_CO__CO_AI", range.get_full_range_order_name("pt_co160_rfi3"));
//    EXPECT_EQ("list105_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu105_rfi0"));
//    EXPECT_EQ("list105_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu105_rfi1"));
//    EXPECT_EQ("list105_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu105_rfi2"));
//    EXPECT_EQ("list105_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu105_rfi3"));
//    EXPECT_EQ("list135_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu135_rfi0"));
//    EXPECT_EQ("list135_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu135_rfi1"));
//    EXPECT_EQ("list135_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu135_rfi2"));
//    EXPECT_EQ("list135_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu135_rfi3"));
//    EXPECT_EQ("list160_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu160_rfi0"));
//    EXPECT_EQ("list160_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu160_rfi1"));
//    EXPECT_EQ("list160_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu160_rfi2"));
//    EXPECT_EQ("list160_BU__CO_F_BU_AI", range.get_full_range_order_name("pt_bu160_rfi3"));
//    EXPECT_EQ("list105_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb105_rfi0"));
//    EXPECT_EQ("list105_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb105_rfi1"));
//    EXPECT_EQ("list105_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb105_rfi2"));
//    EXPECT_EQ("list105_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb105_rfi3"));
//    EXPECT_EQ("list135_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb135_rfi0"));
//    EXPECT_EQ("list135_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb135_rfi1"));
//    EXPECT_EQ("list135_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb135_rfi2"));
//    EXPECT_EQ("list135_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb135_rfi3"));
//    EXPECT_EQ("list160_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb160_rfi0"));
//    EXPECT_EQ("list160_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb160_rfi1"));
//    EXPECT_EQ("list160_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb160_rfi2"));
//    EXPECT_EQ("list160_SB__CO_F_BU_F_SB_AI", range.get_full_range_order_name("pt_sb160_rfi3"));
//
//}
//
//TEST(Range_get_gto_file_for_first_allin, ReturnProperGTOFilePath) {
//    Range range;
//    
//    auto result = range.gto_file_for_first_to_act(8, 8, 8, 8, POSITION_CO);
//    std::string path = GTO_FOLDER_PATH;
//    path.append(R"(\co\8\8\8\8\co_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(8,9,15,16,POSITION_CO);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\co\8\9\15\16\co_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(16,8,8,10,POSITION_CO);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\co\16\8\8\10\co_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(0,8,8,10,POSITION_BU);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\bu\8\8\10\co_f_bu_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(16,16,16,10,POSITION_BU);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\bu\16\16\10\co_f_bu_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(8,10,8,14,POSITION_BU);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\bu\10\8\14\co_f_bu_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(8,10,8,14,POSITION_SB);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\sb\8\co_f_bu_f_sb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(8,10,16,16,POSITION_SB);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\sb\16\co_f_bu_f_sb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_first_to_act(9,10,10,9,POSITION_SB);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\sb\9\co_f_bu_f_sb_ai.csv)");
//    EXPECT_EQ(path, result);
//}
//
//TEST(Range_get_gto_file_for_one_allin, ReturnProperGTOFilePath) {
//    Range range;
//    
//    auto result = range.gto_file_for_one_allin(8, 8, 8, 8, POSITION_BU, POSITION_CO);
//    std::string path = GTO_FOLDER_PATH;
//    path.append(R"(\co\8\8\8\8\co_ai_bu_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_one_allin(10, 16, 8, 16, POSITION_BU, POSITION_CO);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\co\10\16\8\16\co_ai_bu_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_one_allin(10, 16, 8, 16, POSITION_SB, POSITION_BU);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\bu\16\8\16\co_f_bu_ai_sb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_one_allin(10, 16, 8, 16, POSITION_SB, POSITION_CO);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\co\10\16\8\16\co_ai_bu_f_sb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_one_allin(10, 16, 8, 16, POSITION_BB, POSITION_CO);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\co\10\16\8\16\co_ai_bu_f_sb_f_bb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_one_allin(12, 12, 10, 16, POSITION_BB, POSITION_BU);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\bu\12\10\16\co_f_bu_ai_sb_f_bb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_one_allin(12, 12, 10, 16, POSITION_BB, POSITION_SB);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\sb\10\co_f_bu_f_sb_ai_bb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_one_allin(12, 12, 14, 11, POSITION_BB, POSITION_SB);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\sb\11\co_f_bu_f_sb_ai_bb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//}
//
//TEST(Range_get_gto_file_for_two_allins, ReturnProperGTOFilePath) {
//    Range range;
//    
//    auto result = range.gto_file_for_two_allins(8, 8, 8, 8, POSITION_SB, POSITION_CO_BU);
//    std::string path = GTO_FOLDER_PATH;
//    path.append(R"(\co\8\8\8\8\co_ai_bu_ai_sb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_two_allins(12, 10, 8, 16, POSITION_BB, POSITION_CO_BU);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\co\12\10\8\16\co_ai_bu_ai_sb_f_bb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_two_allins(16, 16, 8, 8, POSITION_BB, POSITION_CO_SB);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\co\16\16\8\8\co_ai_bu_f_sb_ai_bb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_two_allins(16, 16, 8, 16, POSITION_BB, POSITION_BU_SB);
//    path = GTO_FOLDER_PATH;
//    path.append(R"(\bu\16\8\16\co_f_bu_ai_sb_ai_bb_ai.csv)");
//    EXPECT_EQ(path, result);
//
//    result = range.gto_file_for_two_allins(16, 16, 8, 16, POSITION_CO, POSITION_CO_SB);
//    path = EMPTY_STRING;
//    EXPECT_EQ(path, result);
//
//
//}
//
//TEST(HelperFunctions_check_number_for_dots, ReturnProperNumberWithoutDotsFor5_10) {
//    EXPECT_EQ("99", Helper::check_number_for_dots("99"));
//    EXPECT_EQ("80", Helper::check_number_for_dots("80"));
//    EXPECT_EQ("156", Helper::check_number_for_dots("15698"));
//    EXPECT_EQ("101", Helper::check_number_for_dots("10144"));
//    EXPECT_EQ("205", Helper::check_number_for_dots("20599"));
//    EXPECT_EQ("498", Helper::check_number_for_dots("49899"));
//    EXPECT_EQ("70", Helper::check_number_for_dots("7048"));
//    EXPECT_EQ("79", Helper::check_number_for_dots("7924"));
//    EXPECT_EQ("89", Helper::check_number_for_dots("8948"));
//    EXPECT_EQ("99", Helper::check_number_for_dots("9948"));
//    EXPECT_EQ("80", Helper::check_number_for_dots("8025"));
//    EXPECT_EQ("100", Helper::check_number_for_dots("1009"));
//    EXPECT_EQ("190", Helper::check_number_for_dots("1902"));
//    EXPECT_EQ("256", Helper::check_number_for_dots("2569"));
//    EXPECT_EQ("389", Helper::check_number_for_dots("3897"));
//    EXPECT_EQ("489", Helper::check_number_for_dots("4897"));
//    EXPECT_EQ("70", Helper::check_number_for_dots("709"));
//    EXPECT_EQ("78", Helper::check_number_for_dots("785"));
//    EXPECT_EQ("88", Helper::check_number_for_dots("885"));
//    EXPECT_EQ("92", Helper::check_number_for_dots("925"));
//    EXPECT_EQ("115", Helper::check_number_for_dots("115"));
//    EXPECT_EQ("230", Helper::check_number_for_dots("230"));
//    EXPECT_EQ("330", Helper::check_number_for_dots("330"));
//    EXPECT_EQ("460", Helper::check_number_for_dots("460"));
//}


//TEST(HelperFunctions_round_stack_to_one_bb, ReturnRoundedStack) {
//    EXPECT_EQ(8, Helper::Round_Stack_To_One_BB(79, 10));
//    EXPECT_EQ(8, Helper::Round_Stack_To_One_BB(75, 10));
//    EXPECT_EQ(7, Helper::Round_Stack_To_One_BB(70, 10));
//    EXPECT_EQ(7, Helper::Round_Stack_To_One_BB(74, 10));
//    EXPECT_EQ(14, Helper::Round_Stack_To_One_BB(144, 10));
//    EXPECT_EQ(16, Helper::Round_Stack_To_One_BB(155, 10));
//    EXPECT_EQ(0, Helper::Round_Stack_To_One_BB(0, 10));
//    EXPECT_EQ(0, Helper::Round_Stack_To_One_BB(-1, 10));
//
//}
//
//TEST(HelperFunctions_evaluate_stack, ReturnPossibleStackSize) {
//    EXPECT_EQ(8, Helper::Evaluate_Stack(8));
//    EXPECT_EQ(16, Helper::Evaluate_Stack(16));
//    EXPECT_EQ(12, Helper::Evaluate_Stack(12));
//    EXPECT_EQ(16, Helper::Evaluate_Stack(7));
//    EXPECT_EQ(16, Helper::Evaluate_Stack(0));
//    EXPECT_EQ(16, Helper::Evaluate_Stack(-1));
//    EXPECT_EQ(16, Helper::Evaluate_Stack(17));
//
//}
//
//TEST(HelperFunctions_get_combo_by_highest_card, ReturnProperComboWithFirstCardHighest) {
//    EXPECT_EQ("Ac2d", Helper::GetComboByHighestCard("2d","Ac"));
//    EXPECT_EQ("Ac3c", Helper::GetComboByHighestCard("3c","Ac"));
//    EXPECT_EQ("9c2c", Helper::GetComboByHighestCard("9c","2c"));
//}
//
//TEST(HelperFunctions_get_combo, ReturnProperCardCombo) {
//    EXPECT_EQ("A2o", Helper::GetCombo("2d","Ac"));
//    EXPECT_EQ("K7o", Helper::GetCombo("Kd","7c"));
//    EXPECT_EQ("K7s", Helper::GetCombo("Kd","7d"));
//    EXPECT_EQ("QJs", Helper::GetCombo("Jd","Qd"));
//    EXPECT_EQ("22", Helper::GetCombo("2d","2c"));
//    EXPECT_EQ("AA", Helper::GetCombo("Ac","As"));
//    EXPECT_EQ("T6o", Helper::GetCombo("Tc","6s"));
//    EXPECT_EQ("T2o", Helper::GetCombo("2c","Ts"));
//}


//// ContinueOnMyTurn tests
//#pragma region ContinueOnMyTurn
//
//struct ContinueOnMyTurn : testing::Test {
//    unique_ptr<BotLogic> bot_logic;
//    ContinueOnMyTurn() = default;
//};
//
//TEST_F(ContinueOnMyTurn, SkipIfFinalAnswerIsZeroOrUnknown) {
//    EXPECT_EQ(false, bot_logic->IsFinalAnswer(0));
//    EXPECT_EQ(false, bot_logic->IsFinalAnswer(-1));
//    EXPECT_EQ(false, bot_logic->IsFinalAnswer(23));
//}
//
//TEST_F(ContinueOnMyTurn, ContinueIfFinalAnswerIsOne) {
//    EXPECT_EQ(true, bot_logic->IsFinalAnswer(1));
//}
//
//
//TEST_F(ContinueOnMyTurn, SkipIfMTBisZeroOrUnknown) {
//    EXPECT_EQ(false, bot_logic->MyTurnBitsIsCorrect(0));
//    EXPECT_EQ(false, bot_logic->MyTurnBitsIsCorrect(-1));
//    EXPECT_EQ(false, bot_logic->MyTurnBitsIsCorrect(99));
//}
//
//TEST_F(ContinueOnMyTurn, ContinueIfMTBisCorrect) {
//    EXPECT_EQ(true, bot_logic->MyTurnBitsIsCorrect(BTN_FOLD_CHECK_BET_AI));
//    EXPECT_EQ(true, bot_logic->MyTurnBitsIsCorrect(BTN_FOLD_CALL_RAISE_AI));
//}
//
//
//TEST_F(ContinueOnMyTurn, SkipIfBetroundUnknown) {
//    EXPECT_EQ(false, bot_logic->BetRoundIsCorrect(0));
//    EXPECT_EQ(false, bot_logic->BetRoundIsCorrect(-1));
//    EXPECT_EQ(false, bot_logic->BetRoundIsCorrect(5));
//    
//}
//
//
//TEST_F(ContinueOnMyTurn, ContinueIfBetroundCorrect) {
//    EXPECT_EQ(true, bot_logic->BetRoundIsCorrect(ROUND_PREFLOP));
//    EXPECT_EQ(true, bot_logic->BetRoundIsCorrect(ROUND_FLOP));
//    EXPECT_EQ(true, bot_logic->BetRoundIsCorrect(ROUND_TURN));
//    EXPECT_EQ(true, bot_logic->BetRoundIsCorrect(ROUND_RIVER));
//}
//
//
//TEST_F(ContinueOnMyTurn, SkipIfHeroCardsUnknown) {
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(0, 62, 0, 0, 0, 0, 0, ROUND_FLOP));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(0, -1, 0, 0, 0, 0, 0, ROUND_RIVER));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(23, -1, 0, 0, 0, 0, 0, ROUND_TURN));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(52, 2, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//    
//
//}
//
//
//TEST_F(ContinueOnMyTurn, DoNotCheckCardsIfBetroundNotCorrect) {
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(0, 0, 0, 0, 0, 0, 0, 0));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(22, 23, 5, 7, 3, 44, 0, ROUND_TURN));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(0, 0, 0, 0, 0, 0, 0, -1));
//
//}
//
//TEST_F(ContinueOnMyTurn, SkipIfAnyCardsUnknownPreflop) {
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(51, 52, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(55, 13, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(25, -1, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//
//}
//
//
//TEST_F(ContinueOnMyTurn, SkipIfAnyCardsUnknownOnFlop) {
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(0, 0, 0, 0, 52, 0, 0, ROUND_FLOP));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(42, 0, 18, -1, 40, 0, 0, ROUND_FLOP));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(22, 23, 19, -1, 0, 0, 0, ROUND_FLOP));
//}
//
//TEST_F(ContinueOnMyTurn, SkipIfAnyCardsUnknownOnTurn) {
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(0, 0, 0, 0, 0, -1, 0, ROUND_TURN));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(42, 0, 18, -1, 40, 12, 0, ROUND_TURN));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(22, -2, 19, 6, 7, 8, 0, ROUND_TURN));
//}
//
//
//TEST_F(ContinueOnMyTurn, SkipIfAnyCardsUnknownOnRiver) {
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(0, 999, 0, 0, 0, 0, 0, ROUND_RIVER));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(42, 19, 18, 7, 40, 12, 60, ROUND_RIVER));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(22, -2, 19, 6, 7, 8, 9, ROUND_RIVER));
//}
//
//
//
//TEST_F(ContinueOnMyTurn, ContinueIfHeroCardsKnownPreflop) {
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(15, 10, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(51, 13, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(25, 2, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(25, -1, 0, 0, 0, 0, 0, ROUND_PREFLOP));
//
//}
//
//
//TEST_F(ContinueOnMyTurn, ContinueIfAllCardsKnownOnFlop) {
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(15, 10, 5, 7, 8, 0, 0, ROUND_FLOP));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(51, 13, 23, 12, 7, 0, 0, ROUND_FLOP));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(25, 2, 3, 9, 7, 0, 0, ROUND_FLOP));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(25, 2, -1, 9, 7, 0, 0, ROUND_FLOP));
//
//}
//
//
//TEST_F(ContinueOnMyTurn, ContinueIfAllCardsKnownOnTurn) {
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(15, 10, 5, 7, 8, 45, -1, ROUND_TURN));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(51, 13, 23, 12, 7, 51, 0, ROUND_TURN));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(25, 2, 3, 9, 7, 37, 0, ROUND_TURN));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(42, 0, 18, 0, 40, 52, 0, ROUND_TURN));
//}
//
//
//TEST_F(ContinueOnMyTurn, ContinueIfAllCardsKnownOnRiver) {
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(15, 10, 5, 7, 8, 45, 51, ROUND_RIVER));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(41, 13, 23, 12, 7, 49, 51, ROUND_RIVER));
//    EXPECT_EQ(true, bot_logic->AllCardsKnown(25, 2, 3, 9, 7, 37, 47, ROUND_RIVER));
//    EXPECT_EQ(false, bot_logic->AllCardsKnown(42, 10, 18, 2, 40, 12, -1, ROUND_RIVER));
//}
//
//#pragma endregion
//
//
////HandParameters tests
//#pragma region HandParameters
//
//struct HandParamsChecks : testing::Test {
//    unique_ptr<BotLogic> bot_logic;
//    HandParamsChecks() = default;
//};
//
////Handnumber
//TEST_F(HandParamsChecks, HandNumberReturnUnknownIfEmpty) {
//    EXPECT_EQ("Unknown - []", bot_logic->HandNumber(""));
//    EXPECT_EQ("Unknown - [-]", bot_logic->HandNumber("-"));
//    EXPECT_EQ("Unknown - [-1]", bot_logic->HandNumber("-1"));
//}
//
//TEST_F(HandParamsChecks, HandNumberReturnItselfIfNotEmpty) {
//    EXPECT_EQ("123456789", bot_logic->HandNumber("123456789"));
//    EXPECT_EQ("222", bot_logic->HandNumber("222"));
//}
//
////Bet round
//TEST_F(HandParamsChecks, BetRoundReturnsUnknownIfNotInScope) {
//    EXPECT_EQ("Unknown - [0]", bot_logic->BetRound(0));
//    EXPECT_EQ("Unknown - [5]", bot_logic->BetRound(5));
//    EXPECT_EQ("Unknown - [-1]", bot_logic->BetRound(-1));
//}
//
//TEST_F(HandParamsChecks, BetRoundReturnsItsNameIfInScope) {
//    EXPECT_EQ("Preflop", bot_logic->BetRound(1));
//    EXPECT_EQ("Flop", bot_logic->BetRound(2));
//    EXPECT_EQ("Turn", bot_logic->BetRound(3));
//    EXPECT_EQ("River", bot_logic->BetRound(4));
//}
//
////BigBlindSize
//TEST_F(HandParamsChecks, BBsizeReturnZeroIfNotFound) {
//    EXPECT_EQ(0, bot_logic->BigBlindSize(0.11));
//    EXPECT_EQ(0, bot_logic->BigBlindSize(-4.01));
//}
//
//TEST_F(HandParamsChecks, BBsizeReturnItselfIfFound) {
//    EXPECT_EQ(0.1, bot_logic->BigBlindSize(0.1));
//    EXPECT_EQ(0.25, bot_logic->BigBlindSize(0.25));
//    EXPECT_EQ(0.5, bot_logic->BigBlindSize(0.5));
//    EXPECT_EQ(1, bot_logic->BigBlindSize(1));
//    EXPECT_EQ(2, bot_logic->BigBlindSize(2));
//    EXPECT_EQ(4, bot_logic->BigBlindSize(4));
//    EXPECT_EQ(6, bot_logic->BigBlindSize(6));
//    EXPECT_EQ(10, bot_logic->BigBlindSize(10));
//}
//
//
////Number of players dealt
//TEST_F(HandParamsChecks, PlayersDealtReturnsZeroIfNotInScope) {
//    EXPECT_EQ(0, bot_logic->PlayersDealt(0));
//    EXPECT_EQ(0, bot_logic->PlayersDealt(1));
//    EXPECT_EQ(0, bot_logic->PlayersDealt(-1));
//    EXPECT_EQ(0, bot_logic->PlayersDealt(10));
//}
//
//TEST_F(HandParamsChecks, PlayersDealtReturnsItselfIfInScope) {
//    for (auto i = MIN_PLAYERS_DEALT; i <= MAX_PLAYERS_DEALT; ++i) {
//        EXPECT_EQ(i, bot_logic->PlayersDealt(i));
//    }
//}
//
//
////Number of opponents playing
//TEST_F(HandParamsChecks, OppsPlayingReturnsZeroIfNotInScope) {
//    EXPECT_EQ(0, bot_logic->OppsPlaying(0));
//    EXPECT_EQ(0, bot_logic->OppsPlaying(-1));
//    EXPECT_EQ(0, bot_logic->OppsPlaying(9));
//    EXPECT_EQ(0, bot_logic->OppsPlaying(10));
//}
//
//TEST_F(HandParamsChecks, OppsPlayingReturnsItselfIfInScope) {
//    for (auto i = MIN_OPPS_PLAYING; i <= MAX_OPPS_PLAYING; ++i) {
//        EXPECT_EQ(i, bot_logic->OppsPlaying(i));
//    }
//}
//
////Hero position
//TEST_F(HandParamsChecks, HeroPositionReturn9maxPositionIfDealerUnknown) {
//    EXPECT_EQ("BTN", bot_logic->HeroPosition(0, 9));
//    EXPECT_EQ("CO", bot_logic->HeroPosition(0, 8));
//    EXPECT_EQ("MP+1", bot_logic->HeroPosition(0, 7));
//    EXPECT_EQ("MP", bot_logic->HeroPosition(0, 6));
//    EXPECT_EQ("UTG+2", bot_logic->HeroPosition(0, 5));
//    EXPECT_EQ("UTG+1", bot_logic->HeroPosition(0, 4));
//    EXPECT_EQ("UTG", bot_logic->HeroPosition(0, 3));
//    EXPECT_EQ("BB", bot_logic->HeroPosition(0, 2));
//    EXPECT_EQ("SB", bot_logic->HeroPosition(0, 1));
//}
//
//
//TEST_F(HandParamsChecks, HeroPositionReturnUTGWhen6maxPlus) {
//    EXPECT_EQ("UTG", bot_logic->HeroPosition(9, 3));
//    EXPECT_EQ("UTG", bot_logic->HeroPosition(8, 3));
//    EXPECT_EQ("UTG", bot_logic->HeroPosition(7, 3));
//    EXPECT_EQ("UTG", bot_logic->HeroPosition(6, 3));
//}
//
//TEST_F(HandParamsChecks, HeroPositionReturnUTGPlus1When7maxPlus) {
//    EXPECT_EQ("UTG+1", bot_logic->HeroPosition(9, 4));
//    EXPECT_EQ("UTG+1", bot_logic->HeroPosition(8, 4));
//    EXPECT_EQ("UTG+1", bot_logic->HeroPosition(7, 4));
//}
//
//
//TEST_F(HandParamsChecks, HeroPositionReturnUTGPlus2When9max) {
//    EXPECT_EQ("UTG+2", bot_logic->HeroPosition(9, 5));
//}
//
//TEST_F(HandParamsChecks, HeroPositionReturnMPWhen5maxPlus) {
//    EXPECT_EQ("MP", bot_logic->HeroPosition(9, 6));
//    EXPECT_EQ("MP", bot_logic->HeroPosition(8, 5));
//    EXPECT_EQ("MP", bot_logic->HeroPosition(7, 5));
//    EXPECT_EQ("MP", bot_logic->HeroPosition(6, 4));
//    EXPECT_EQ("MP", bot_logic->HeroPosition(5, 3));
//}
//
//
//TEST_F(HandParamsChecks, HeroPositionReturnMPPlus1When8maxPlus) {
//    EXPECT_EQ("MP+1", bot_logic->HeroPosition(9, 7));
//    EXPECT_EQ("MP+1", bot_logic->HeroPosition(8, 6));
//}
//
//
//TEST_F(HandParamsChecks, HeroPositionReturnCOWhen4maxPlus) {
//    for (auto i = 4; i <= MAX_PLAYERS_DEALT; ++i) {
//        EXPECT_EQ("CO", bot_logic->HeroPosition(i, i-1));
//    }
//}
//
//TEST_F(HandParamsChecks, HeroPositionReturnBTNWhen3MaxPlus) {
//    for (auto i = 3; i <= MAX_PLAYERS_DEALT; ++i) {
//        EXPECT_EQ("BTN", bot_logic->HeroPosition(i, i));
//    }
//}
//
//
//TEST_F(HandParamsChecks, HeroPositionReturnBBWhen3MaxPlus) {
//    for (auto i = 3; i <= MAX_PLAYERS_DEALT; ++i) {
//        EXPECT_EQ("BB", bot_logic->HeroPosition(i, 2));
//    }
//}
//
//
//TEST_F(HandParamsChecks, HeroPositionReturnSBWhen3MaxPlus) {
//    for (auto i = 3; i <= MAX_PLAYERS_DEALT; ++i) {
//        EXPECT_EQ("SB", bot_logic->HeroPosition(i, 1));
//    }
//}
//
//TEST_F(HandParamsChecks, HeroPositionReturnSBWhenHU) {
//    EXPECT_EQ("SB", bot_logic->HeroPosition(2, 2));
//}
//
//TEST_F(HandParamsChecks, HeroPositionReturnBBWhenHU) {
//    EXPECT_EQ("BB", bot_logic->HeroPosition(2, 1));
//}
//
//
////Hero pocket cards
//TEST_F(HandParamsChecks, HeroCardsReturnProperValuesIfInScope) {
//    EXPECT_EQ("As Ad", bot_logic->HeroCards(51, 25));
//    EXPECT_EQ("2h 9d", bot_logic->HeroCards(0, 20));
//    EXPECT_EQ("Ks Td", bot_logic->HeroCards(50, 21));
//}
//
//
//TEST_F(HandParamsChecks, HeroCardsReturnProperValuesIfUnknown) {
//    EXPECT_EQ("Unknown - [-1] Td", bot_logic->HeroCards(-1, 21));
//    EXPECT_EQ("Ks Unknown - [999]", bot_logic->HeroCards(50, 999));
//}
//
//
////Action
//TEST_F(HandParamsChecks, ActionReturnFAIfActFirst) {
//    EXPECT_EQ("FA", bot_logic->Action(1, 0, 0, 0));
//    EXPECT_EQ("0", bot_logic->Action(0, 0, 0, 0));
//    EXPECT_EQ("0", bot_logic->Action(-1, 0, 0, 0));
//    EXPECT_EQ("0", bot_logic->Action(2, 0, 0, 0));
//}
//
//TEST_F(HandParamsChecks, ActionReturnORIfFacedOR) {
//    EXPECT_EQ("OR", bot_logic->Action(0, 1, 0, 0));
//    EXPECT_EQ("OR", bot_logic->Action(-1, 1, 0, 0));
//}
//
//TEST_F(HandParamsChecks, ActionReturnISOLIMPIfFacedIsoLimpers) {
//    EXPECT_EQ("ISO_LIMP", bot_logic->Action(0, 1, 1, 0));
//    EXPECT_EQ("ISO_LIMP", bot_logic->Action(0, 1, 2, 0));
//}
//
////Flop cards
//TEST_F(HandParamsChecks, FlopCardsReturnProperValuesIfInScope) {
//    EXPECT_EQ("As Ad Ac", bot_logic->FlopCards(51, 25, 38));
//    EXPECT_EQ("2h 9d 2s", bot_logic->FlopCards(0, 20, 39));
//    EXPECT_EQ("Ks Td 9d", bot_logic->FlopCards(50, 21, 20));
//}
//
//TEST_F(HandParamsChecks, FlopCardsReturnProperValuesIfUnknown) {
//    EXPECT_EQ("As Ad Unknown - [-1]", bot_logic->FlopCards(51, 25, -1));
//    EXPECT_EQ("2h Unknown - [52] 2s", bot_logic->FlopCards(0, 52, 39));
//    EXPECT_EQ("Unknown - [-12358] Td 9d", bot_logic->FlopCards(-12358, 21, 20));
//}
//
//
////Hero stack size
//TEST_F(HandParamsChecks, HeroStackSizeReturnZeroIfNegative) {
//    EXPECT_EQ(0, bot_logic->HeroStackSize(-1));
//    EXPECT_EQ(0, bot_logic->HeroStackSize(-100));
//    EXPECT_EQ(0, bot_logic->HeroStackSize(-0.56));
//}
//
//TEST_F(HandParamsChecks, HeroStackSizeReturnItselfIfPositive) {
//    EXPECT_EQ(0.1, bot_logic->HeroStackSize(0.1));
//    EXPECT_EQ(100, bot_logic->HeroStackSize(100));
//    EXPECT_EQ(100.56, bot_logic->HeroStackSize(100.56));
//}
//
//
////Pot size
//TEST_F(HandParamsChecks, PotSizeReturnZeroIfNegative) {
//    EXPECT_EQ(0, bot_logic->PotSize(-1));
//    EXPECT_EQ(0, bot_logic->PotSize(-100));
//    EXPECT_EQ(0, bot_logic->PotSize(-0.56));
//}
//
//TEST_F(HandParamsChecks, PotSizeReturnItselfIfPositive) {
//    EXPECT_EQ(0.1, bot_logic->PotSize(0.1));
//    EXPECT_EQ(100, bot_logic->PotSize(100));
//    EXPECT_EQ(100.56, bot_logic->PotSize(100.56));
//}
//
//
//#pragma endregion
//
//
//
//#pragma region Helperfunctions
//
////FindDoubleInDoubleVector
//TEST(HelperFunctions, FindBBsizeInVector_False) {
//    const auto PossibleBBsize = POSSIBLE_BB_SIZE;
//    EXPECT_EQ(false, Helper::FindDoubleInDoubleVector(0, PossibleBBsize));
//    EXPECT_EQ(false, Helper::FindDoubleInDoubleVector(0.21, PossibleBBsize));
//    EXPECT_EQ(false, Helper::FindDoubleInDoubleVector(3, PossibleBBsize));
//    EXPECT_EQ(false, Helper::FindDoubleInDoubleVector(25, PossibleBBsize));
//    EXPECT_EQ(false, Helper::FindDoubleInDoubleVector(1.25, PossibleBBsize));
//}
//
//
//TEST(HelperFunctions, FindBBsizeInVector_True) {
//    const auto PossibleBBsize = POSSIBLE_BB_SIZE;
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(0.10, PossibleBBsize));
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(0.25, PossibleBBsize));
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(0.5, PossibleBBsize));
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(1, PossibleBBsize));
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(2, PossibleBBsize));
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(4, PossibleBBsize));
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(6, PossibleBBsize));
//    EXPECT_EQ(true, Helper::FindDoubleInDoubleVector(10, PossibleBBsize));
//    
//}
//
//
////CreateUnknownParameterString
//TEST(HelperFunctions, CreateUnknownParameterStringWorks) {
//    EXPECT_EQ("Unknown - []", Helper::CreateUnknownParameterString(""));
//    EXPECT_EQ("Unknown - [-]", Helper::CreateUnknownParameterString("-"));
//    EXPECT_EQ("Unknown - [-1]", Helper::CreateUnknownParameterString("-1"));
//    EXPECT_EQ("Unknown - [qwer]", Helper::CreateUnknownParameterString("qwer"));
//    EXPECT_EQ("Unknown - [12_&?34r t]", Helper::CreateUnknownParameterString("12_&?34r t"));
//}
//
//
////Convert card index
//TEST(HelperFunctions, ConvertCardReturnUnknownWhenIndexNotInScope) {
//    EXPECT_EQ("Unknown - [-1]", Helper::ConvertCardToText(-1));
//    EXPECT_EQ("Unknown - [53]", Helper::ConvertCardToText(53));
//    EXPECT_EQ("Unknown - [52]", Helper::ConvertCardToText(52));
//    EXPECT_EQ("Unknown - [1345]", Helper::ConvertCardToText(1345));
//    EXPECT_EQ("Unknown - [1345]", Helper::ConvertCardToText(1345.5899));
//}
//
//TEST(HelperFunctions, ConvertCardReturnCardNameWhenIndexInScope) {
//    EXPECT_EQ("2h", Helper::ConvertCardToText(0));
//    EXPECT_EQ("3h", Helper::ConvertCardToText(1));
//    EXPECT_EQ("4h", Helper::ConvertCardToText(2));
//    EXPECT_EQ("5h", Helper::ConvertCardToText(3));
//    EXPECT_EQ("6h", Helper::ConvertCardToText(4));
//    EXPECT_EQ("7h", Helper::ConvertCardToText(5));
//    EXPECT_EQ("8h", Helper::ConvertCardToText(6));
//    EXPECT_EQ("9h", Helper::ConvertCardToText(7));
//    EXPECT_EQ("Th", Helper::ConvertCardToText(8));
//    EXPECT_EQ("Jh", Helper::ConvertCardToText(9));
//    EXPECT_EQ("Qh", Helper::ConvertCardToText(10));
//    EXPECT_EQ("Kh", Helper::ConvertCardToText(11));
//    EXPECT_EQ("Ah", Helper::ConvertCardToText(12));
//    EXPECT_EQ("2d", Helper::ConvertCardToText(13));
//    EXPECT_EQ("3d", Helper::ConvertCardToText(14));
//    EXPECT_EQ("4d", Helper::ConvertCardToText(15));
//    EXPECT_EQ("5d", Helper::ConvertCardToText(16));
//    EXPECT_EQ("6d", Helper::ConvertCardToText(17));
//    EXPECT_EQ("7d", Helper::ConvertCardToText(18));
//    EXPECT_EQ("8d", Helper::ConvertCardToText(19));
//    EXPECT_EQ("9d", Helper::ConvertCardToText(20));
//    EXPECT_EQ("Td", Helper::ConvertCardToText(21));
//    EXPECT_EQ("Jd", Helper::ConvertCardToText(22));
//    EXPECT_EQ("Qd", Helper::ConvertCardToText(23));
//    EXPECT_EQ("Kd", Helper::ConvertCardToText(24));
//    EXPECT_EQ("Ad", Helper::ConvertCardToText(25));
//    EXPECT_EQ("2c", Helper::ConvertCardToText(26));
//    EXPECT_EQ("3c", Helper::ConvertCardToText(27));
//    EXPECT_EQ("4c", Helper::ConvertCardToText(28));
//    EXPECT_EQ("5c", Helper::ConvertCardToText(29));
//    EXPECT_EQ("6c", Helper::ConvertCardToText(30));
//    EXPECT_EQ("7c", Helper::ConvertCardToText(31));
//    EXPECT_EQ("8c", Helper::ConvertCardToText(32));
//    EXPECT_EQ("9c", Helper::ConvertCardToText(33));
//    EXPECT_EQ("Tc", Helper::ConvertCardToText(34));
//    EXPECT_EQ("Jc", Helper::ConvertCardToText(35));
//    EXPECT_EQ("Qc", Helper::ConvertCardToText(36));
//    EXPECT_EQ("Kc", Helper::ConvertCardToText(37));
//    EXPECT_EQ("Ac", Helper::ConvertCardToText(38));
//    EXPECT_EQ("2s", Helper::ConvertCardToText(39));
//    EXPECT_EQ("3s", Helper::ConvertCardToText(40));
//    EXPECT_EQ("4s", Helper::ConvertCardToText(41));
//    EXPECT_EQ("5s", Helper::ConvertCardToText(42));
//    EXPECT_EQ("6s", Helper::ConvertCardToText(43));
//    EXPECT_EQ("7s", Helper::ConvertCardToText(44));
//    EXPECT_EQ("8s", Helper::ConvertCardToText(45));
//    EXPECT_EQ("9s", Helper::ConvertCardToText(46));
//    EXPECT_EQ("Ts", Helper::ConvertCardToText(47));
//    EXPECT_EQ("Js", Helper::ConvertCardToText(48));
//    EXPECT_EQ("Qs", Helper::ConvertCardToText(49));
//    EXPECT_EQ("Ks", Helper::ConvertCardToText(50));
//    EXPECT_EQ("As", Helper::ConvertCardToText(51));
//
//}
//
//#pragma endregion
