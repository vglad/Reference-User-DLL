// Needs to be defined here, before #include "user.h"
// to generate proper export- and inport-definitions
#define USER_DLL

#include "user.h"
#include <conio.h>
#include <Windows.h>
#include "OpenHoldemFunctions.h"

#include <iterator>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>
#include <iostream>
#include <chrono>


#include "MagicNumbers.h"
#include "HelperFunctions.h"
#include "Converter.h"
#include "OHComm.h"
#include "IBridge.h"
#include "OHCommunicator.h"
#include "Player.h"
#include "Table.h"
#include "TableInfo.h"
#include "Range.h"
#include "PT.h"
#include "BotLogic.h"
#include "Update.h"


#pragma region Dll Functions
//******************************************************************************
//
// Place all your initalizations in the functions below
// DLLOnLoad() amd DLLOnUnLoad() get called by the DLL,
// the other functions get called by OpenHoldem
//
//******************************************************************************

void DLLOnLoad() {
}

void DLLOnUnLoad() {
}

void __stdcall DLLUpdateOnNewFormula() {
}

void __stdcall DLLUpdateOnConnection() {
}

void __stdcall DLLUpdateOnHandreset() {
}

void __stdcall DLLUpdateOnNewRound() {
}

void __stdcall DLLUpdateOnMyTurn() {
}

void __stdcall DLLUpdateOnHeartbeat() {
}
#pragma endregion

std::chrono::steady_clock::time_point _t1;
std::chrono::steady_clock::time_point _t2;
std::string _stat_name;
double _stat_percent;
std::string _list_name;


DLL_IMPLEMENTS double __stdcall ProcessQuery(const char* pquery) {
    if (pquery == NULL) {
        return 0;
    }

    if (strncmp(pquery, "dll$LogHandNumber", 18) == 0) {
        _table.log_hand_number();
        return 1;
    }

    if (strncmp(pquery, "dll$GTO_FirstToAct", 19) == 0) {
        _table.log_allin_situation_name(FIRST_TO_ACT);
        if (_bot.solution(FIRST_TO_ACT) == 0) {
            return _bot.fold();    
        }
        return _bot.allin();
    }

    if (strncmp(pquery, "dll$GTO_OneAllIn", 18) == 0) {
        _table.log_allin_situation_name(ONE_ALLIN);
        if (_bot.solution(ONE_ALLIN) == 0) {
            return _bot.fold();    
        }
        return _bot.allin();
    }

    if (strncmp(pquery, "dll$GTO_TwoAllIns", 18) == 0) {
        _table.log_allin_situation_name(TWO_ALLINS);
        if (_bot.solution(TWO_ALLINS) == 0) {
            return _bot.fold();    
        }
        return _bot.allin();
    }

    if (strncmp(pquery, "dll$GTO_ThreeAllIns", 20) == 0) {
        _table.log_allin_situation_name(THREE_ALLINS);
        if (_bot.solution(THREE_ALLINS) == 0) {
            return _bot.fold();    
        }
        return _bot.allin();        
    }

    //if (strncmp(pquery, "dll$CORaiserEffStack", 21) == 0) {
    //    const auto bb_chair = std::to_string(static_cast<int>(_oh.read_symbol("BBchair", MEMORY_SYMBOL)));
    //    const auto sb_chair = std::to_string(static_cast<int>(_oh.read_symbol("SBchair", MEMORY_SYMBOL)));
    //    const auto bu_chair = std::to_string(static_cast<int>(_oh.read_symbol("BUchair", MEMORY_SYMBOL)));
    //    const auto co_chair = std::to_string(static_cast<int>(_oh.read_symbol("COchair", MEMORY_SYMBOL)));
    //    const auto bblind = _oh.read_symbol(BIG_BLIND_SYMBOL, BUILT_IN_SYMBOL);
    //    const auto bb_stack = static_cast<int>(_oh.read_symbol("balance" + bb_chair, BUILT_IN_SYMBOL) + bblind);
    //    const auto sb_stack = static_cast<int>(_oh.read_symbol("balance" + sb_chair, BUILT_IN_SYMBOL) + bblind / 2);
    //    const auto bu_stack = static_cast<int>(_oh.read_symbol("balance" + bu_chair, BUILT_IN_SYMBOL));
    //    const auto co_stack = static_cast<int>(_oh.read_symbol("currentbet" + co_chair, BUILT_IN_SYMBOL));
    //    auto eff_stack = _table.player_eff_stack_for_first_to_act(POSITION_CO, co_stack, bu_stack, sb_stack, bb_stack) / bblind;
    //    _oh.write_log("[dll$CORaiserEffStack]"," CO raiser Eff Stack: [" + std::to_string(eff_stack) + "]\n");
    //    return eff_stack;
    //} 

    //if (strncmp(pquery, "dll$BURaiserEffStack", 21) == 0) {
    //    const auto bb_chair = std::to_string(static_cast<int>(_oh.read_symbol("BBchair", MEMORY_SYMBOL)));
    //    const auto sb_chair = std::to_string(static_cast<int>(_oh.read_symbol("SBchair", MEMORY_SYMBOL)));
    //    const auto bu_chair = std::to_string(static_cast<int>(_oh.read_symbol("BUchair", MEMORY_SYMBOL)));
    //    const auto bblind = _oh.read_symbol("bblind", BUILT_IN_SYMBOL);
    //    const auto bb_stack = static_cast<int>(_oh.read_symbol("balance" + bb_chair, BUILT_IN_SYMBOL) + bblind);
    //    const auto sb_stack = static_cast<int>(_oh.read_symbol("balance" + sb_chair, BUILT_IN_SYMBOL) + bblind / 2);
    //    const auto bu_stack = static_cast<int>(_oh.read_symbol("currentbet" + bu_chair, BUILT_IN_SYMBOL));
    //    auto eff_stack = _table.player_eff_stack_for_first_to_act(POSITION_BU, 0, bu_stack, sb_stack, bb_stack) / bblind;
    //    _oh.write_log("[dll$BURaiserEffStack]"," BU raiser Eff Stack: [" + std::to_string(eff_stack) + "]\n");
    //    return eff_stack;
    //}

    //if (strncmp(pquery, "dll$SBRaiserEffStack", 21) == 0) {
    //    const auto sb_chair = std::to_string(static_cast<int>(_oh.read_symbol("SBchair", MEMORY_SYMBOL)));
    //    const auto bblind = _oh.read_symbol("bblind", BUILT_IN_SYMBOL);
    //    const auto bb_stack = static_cast<int>(_oh.read_symbol("balance2", BUILT_IN_SYMBOL) + bblind);
    //    const auto sb_stack = static_cast<int>(_oh.read_symbol("currentbet" + sb_chair, BUILT_IN_SYMBOL));
    //    auto eff_stack = _table.player_eff_stack_for_first_to_act(POSITION_SB, 0, 0, sb_stack, bb_stack) / bblind;
    //    _oh.write_log("[dll$SBRaiserEffStack]"," SB raiser Eff Stack: [" + std::to_string(eff_stack) + "]\n");
    //    return eff_stack;
    //}

    //if (strncmp(pquery, "dll$GetPTStatSample", 20) == 0) {
    //    _oh.write_log("[dll$GetPTStatSample]"," Getting samples for exploit bot...\n");
    //    const auto sit_position = static_cast<int>(_oh.read_symbol("SitPosition", MEMORY_SYMBOL));
    //    _oh.write_log("",_table.my_position(sit_position));
    //    const auto stack_range = static_cast<int>(_oh.read_symbol("StackRange", MEMORY_SYMBOL));
    //    _oh.write_log("",_table.stack_range(stack_range));
    //    const auto one_all_in_position = static_cast<int>(_oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL));
    //    //_oh.write_log("",_table.one_allin_position(one_all_in_position));
    //
    //    const auto one_allin_position_chair_symbol = _table.get_one_allin_position_chair_symbol(one_all_in_position);
    //    const auto one_allin_position_chair = static_cast<int>(_oh.read_symbol(one_allin_position_chair_symbol, MEMORY_SYMBOL));
    //    //_oh.write_log("",_table.one_allin_position_chair(one_allin_position_chair));
    //
    //    _stat_name = _table.get_stat_name(sit_position, stack_range, one_all_in_position, one_allin_position_chair);;
    //    const auto stat_name_percent = _oh.read_symbol(_stat_name, BUILT_IN_SYMBOL);
    //    _stat_percent = _range.evaluate_stat_percent(stat_name_percent) * 100;
    //    _oh.write_log("[dll$GetPTStatSample]"," Stat name: [" + _stat_name + "] = ["
    //                          + std::to_string(_stat_percent) + "]\n");
    //
    //    const auto stat_chair = _stat_name.substr(_stat_name.length() - 1, 1);
    //    const auto stat_opportunities_name = _stat_name.substr(0, _stat_name.length() - 1).append("_opp").append(stat_chair);
    //    const auto db_stat_opportunities_sample = static_cast<int>(_oh.read_symbol(stat_opportunities_name, BUILT_IN_SYMBOL));
    //    _oh.write_log("[dll$GetPTStatSample]"," DB stat opportunities sample: [" + stat_opportunities_name + "] = ["
    //                          + std::to_string(db_stat_opportunities_sample) + "]\n");
    //
    //    if (db_stat_opportunities_sample > 0) {
    //        const auto db_stat_action_sample = (db_stat_opportunities_sample * _stat_percent / 100);
    //        _oh.write_log("[dll$GetPTStatSample]"," DB stat action sample: [" + std::to_string(db_stat_action_sample) + "]\n");
    //
    //        _oh.write_log("[dll$GetPTStatSample]"," Real time opportunities sample: [" + std::to_string(db_stat_opportunities_sample + 1) + "]\n");
    //        _oh.write_log("[dll$GetPTStatSample]"," Real time action sample: [" + std::to_string(db_stat_action_sample + 1) + "]\n");
    //        _stat_percent = ((db_stat_action_sample + 1) / (db_stat_opportunities_sample + 1)) * 100;
    //        _oh.write_log("[dll$GetPTStatSample]"," Real time stat percent: [" + std::to_string(_stat_percent) + "]\n");
    //    }
    //
    //    const auto min_sample_needed = static_cast<int>(_oh.read_symbol(OH_MIN_SAMPLE_FUNC, BUILT_IN_SYMBOL));
    //    _oh.write_log("", _table.evaluate_sample(db_stat_opportunities_sample, min_sample_needed));
    //
    //    //std::string log;
    //    //log.append(_table.my_position(sit_position));
    //    //log.append(_table.stack_range(stack_range));
    //    //log.append(_table.one_allin_position(one_all_in_position));
    //    //log.append(_table.one_allin_position_chair(one_allin_position_chair));
    //    //log.append("[dll$GetPTStatSample] Stat name: [").append(stat_name).append("] = [").append(std::to_string(stat_name_percent)).append("]\n");
    //    //log.append("[dll$GetPTStatSample] Stat opportunities: [").append(stat_opportunities_name).append("] = [").append(std::to_string(stat_opportunities_sample)).append("]\n\n");
    //    //_oh.write_log(log);
    //
    //    return db_stat_opportunities_sample;
    //}

    //if (strncmp(pquery, "dll$CreateRange", 16) == 0) {
    //    if (_stat_name.empty() || _stat_percent == 0) {
    //        const auto sit_position = static_cast<int>(_oh.read_symbol("SitPosition", MEMORY_SYMBOL));
    //        const auto stack_range = static_cast<int>(_oh.read_symbol("StackRange", MEMORY_SYMBOL));
    //        const auto one_all_in_position = static_cast<int>(_oh.read_symbol(ONE_ALLIN_POSITION, MEMORY_SYMBOL));
    //        const auto one_allin_position_chair_symbol = _table.get_one_allin_position_chair_symbol(one_all_in_position);
    //        const auto one_allin_position_chair = static_cast<int>(_oh.read_symbol(one_allin_position_chair_symbol, MEMORY_SYMBOL));
    //        _stat_name = _table.get_stat_name(sit_position, stack_range, one_all_in_position, one_allin_position_chair);
    //        _stat_percent = _range.evaluate_stat_percent(_oh.read_symbol(_stat_name, BUILT_IN_SYMBOL)) * 100;
    //    }
    //
    //    _oh.write_log("[dll$CreateRange]"," Evaluation started with exploit bot ...\n");
    //    _oh.write_log("[dll$CreateRange]"," Stat percent : [" + std::to_string(_stat_percent) + "]\n");
    //    const auto str_range_percent = _range.get_string_percent(_stat_percent / 100);
    //    _oh.write_log("[dll$CreateRange]"," Opponent range: [" + str_range_percent + "]\n");
    //
    //    _list_name = _range.get_range_name(str_range_percent);
    //    _oh.write_log("[dll$CreateRange]"," List name: [" + _list_name + "]\n");
    //    const auto full_range_order_name = _range.get_full_range_order_name(_stat_name);
    //    const auto full_range_order = _range.get_full_range_order(full_range_order_name);
    //    _oh.write_log("[dll$CreateRange]"," Full range order: [" + full_range_order_name + "]\n");
    //
    //
    //    const auto list_combos = _range.get_range_combos(_stat_percent / 100, full_range_order);
    //    _oh.save_list(_list_name, list_combos);
    //    _oh.write_log("[dll$CreateRange]"," List saved to OpenHoldem.\n");
    //
    //    //std::string log;
    //    //log.append("[dll$CreateRange] Opponent range: [").append(str_range_percent).append("]\n");
    //    //log.append("[dll$CreateRange] List name: [").append(_list_name).append("]\n");
    //    //log.append("[dll$CreateRange] Full range order: [").append(full_range_order_name).append("]\n");
    //    //log.append("[dll$CreateRange] List saved to OpenHoldem.").append("\n");
    //    //_oh.write_log(log);
    //
    //    _stat_percent = 0;
    //    _stat_name = EMPTY_STRING;
    //    return 1;
    //}

    //if (strncmp(pquery, "dll$GetEquityVsRange", 21) == 0) {
    //    const auto card1 = _conv.card_index_to_text(_oh.read_symbol(CARD_ONE,BUILT_IN_SYMBOL));
    //    const auto card2 = _conv.card_index_to_text(_oh.read_symbol(CARD_TWO,BUILT_IN_SYMBOL));
    //    _oh.write_log("[dll$GetEquityVsRange]"," My hand: [" + card1 + card2 + "]\n");
    //
    //    const auto prwin = _oh.read_symbol("vs$" + _list_name + "$prwin", BUILT_IN_SYMBOL);
    //    const auto prtie = _oh.read_symbol("vs$" + _list_name + "$prtie", BUILT_IN_SYMBOL);
    //    const auto hand_equity = prwin + prtie * 0.5;
    //    _oh.write_log("[dll$GetEquityVsRange]"," Hand equity vs " + _list_name + ": [" + std::to_string(hand_equity * 100) + "]\n");
    //
    //    const auto break_even_equity = _oh.read_symbol("f$Calc_BreakEvenEquity", BUILT_IN_SYMBOL);
    //    _oh.write_log("[dll$GetEquityVsRange]"," Break even equity: [" + std::to_string((break_even_equity * 100)) + "]\n");
    //    _oh.write_log("", _table.evaluate_equity_result(hand_equity, break_even_equity));
    //
    //    _list_name = EMPTY_STRING;
    //    return hand_equity;
    //}

    if (strncmp(pquery, "dll$GetTableInfo", 23) == 0) {
        auto result = _tableInfo.set_table_info();
        //auto tab_info = _tableInfo.table_info;
        //for (const auto& vector : tab_info) {
        //    for (auto value : vector) {
        //        _ohCommunicator.write_log("dll$GetTableInfo", "[" + to_string(value) + "]");
        //    }
            _ohCommunicator.write_log("dll$GetTableInfo", "****");
        //}
        return 1;
    }

    if (strncmp(pquery, "dll$UpdateTables", 17) == 0) {
        const auto new_players_names = _updater.get_current_names();
        for (auto i = 1; i < TABLE_COUNT + 1; ++i) {
            if (!_updater.correct_players_names_file_format(i)) {
                _updater.create_default_players_names_file(i);
            }
            _updater.update_table(i, new_players_names);
        }
        return 1;
    }

    if (strncmp(pquery, "dll$test", 9) == 0) {
        return 999;
    }

    if (strncmp(pquery, "dll$TStart", 11) == 0) {
        k_current_debug_value = k_debug;
        k_debug = true;
        _oh.write_log("dll$TStart","Time counter started...\n");
        k_debug = k_current_debug_value;
        _t1 = std::chrono::high_resolution_clock::now();
        return 1;
    }

    if (strncmp(pquery, "dll$TStop", 10) == 0) {
        _t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(_t2 - _t1).count();
        k_current_debug_value = k_debug;
        k_debug = true;
        _oh.write_log("dll$TStop", "Time counter stopped. Duration (microseconds): " + std::to_string(duration) + "\n");
        k_debug = k_current_debug_value;
        return 1;
    }

    return 0;
}

#pragma region DLL Entry point
//******************************************************************************
//
// DLL entry point
//
//******************************************************************************

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
            AllocConsole();
#endif _DEBUG
            InitializeOpenHoldemFunctionInterface();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
#ifdef _DEBUG
            FreeConsole();
#endif _DEBUG
            break;
    }
    return TRUE;
}
#pragma endregion 