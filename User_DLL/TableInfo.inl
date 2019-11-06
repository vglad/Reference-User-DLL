#pragma once
#include "TableInfo.h"

using namespace std;

inline TableInfo::TableInfo() = default;
inline TableInfo::~TableInfo() = default;

inline int TableInfo::set_table_info() {
    set_big_blind(get_big_blind());
    for (auto i = 3; i >= 0; --i) {
        table_info[i] = get_player_default_info(i);
        set_player_seated(table_info[i]);
    }
    if (!check_enough_players_seated()) {
        clear_table_info();
        return 0;
    }
    for (auto i = 3; i >= 0; --i) {
        set_player_active(table_info[i]);
    }
    if (!check_enough_players_active()) {
        clear_table_info();
        return 0;
    }
    for (auto i = 3; i >= 0; --i) {
        set_player_bet_and_balance(table_info[i]);
    }
    if (!set_bet_values_on_blinds()) {
        return 0;
    }
    if (!set_all_chairs_positions()) {
        return 0;
    }
    set_allin_info();
    return 1;
}

inline vector<double> TableInfo::get_player_default_info(const int chair_number) const {
    auto p_info = p_info_empty_vector;
    p_info[P_CHAIR] = chair_number;
    const auto seated = _ohCommunicator.read_region("p" + to_string(chair_number) + "seated");
    if (seated == "true") {
        p_info[P_SEATED] = 1;
        const auto active = _ohCommunicator.read_region("p" + to_string(chair_number) + "active");
        if (active == "true") {
            p_info[P_ACTIVE] = 1;
            p_info[P_BET] = _ohCommunicator.read_symbol(BET + to_string(chair_number), BUILT_IN_SYMBOL);
            p_info[P_BALANCE] = _ohCommunicator.read_symbol(BALANCE + to_string(chair_number), BUILT_IN_SYMBOL);
        }
    }
    return p_info;
}

inline void TableInfo::set_player_seated(vector<double>& p_info) const {
    if (p_info[P_SEATED] != 0 && p_info[P_SEATED] != 1) {
        _ohCommunicator.write_log("set_player_seated", "ERROR: Unknown SEATED value [" + to_string(p_info[P_SEATED]) + "] for chair [" + to_string(p_info[P_CHAIR]) + "]");
        _ohCommunicator.write_log("set_player_seated", "ERROR: Changing SEATED to 0");
        p_info[P_SEATED] = 0;
    }
}

inline void TableInfo::set_player_active(vector<double>& p_info) const {
    if (p_info[P_ACTIVE] != 0 && p_info[P_ACTIVE] != 1) {
        _ohCommunicator.write_log("set_player_active", "ERROR: Unknown ACTIVE value [" + to_string(p_info[P_ACTIVE]) + "] for chair [" + to_string(p_info[P_CHAIR]) + "]");
        _ohCommunicator.write_log("set_player_active", "ERROR: Changing ACTIVE to 0");
        p_info[P_ACTIVE] = 0;
    }
    else if (p_info[P_ACTIVE] == 1 && p_info[P_SEATED] != 1) {
        _ohCommunicator.write_log("set_player_active", "ERROR: Chair [" + to_string(p_info[P_CHAIR]) + "] has ACTIVE = 1 and SEATED = [" + to_string(p_info[P_SEATED]) + "]");
        _ohCommunicator.write_log("set_player_active", "ERROR: Changing ACTIVE to 0");
        p_info[P_ACTIVE] = 0;
    }
}

inline void TableInfo::set_player_bet_and_balance(vector<double>& p_info) const {
    const auto chair   = to_string(p_info[P_CHAIR]);
    const auto seated  = p_info[P_SEATED];
    const auto active  = p_info[P_ACTIVE];;
    const auto bet     = p_info[P_BET];
    const auto balance = p_info[P_BALANCE];
    if (seated != 1) {
        if (active != 0) {
            p_info[P_ACTIVE] = 0;
        }
        if (bet < 0) {
            p_info[P_BET] = 0;
        }
        else if (bet > 0) {
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has BET > 0 and SEATED = 0");
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BET to 0");
            p_info[P_BET] = 0;
        }
        if (balance < 0) {
            p_info[P_BALANCE] = 0;
        }
        else if (balance > 0) {
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has BALANCE > 0 and SEATED = 0");
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BALANCE to 0");
            p_info[P_BALANCE] = 0;
        }
    }
    else if (active != 1) {
        p_info[P_ACTIVE] = 0;
        p_info[P_BET] = 0;
        p_info[P_BALANCE] = 0;
    }
    else if (bet <= 0 && balance <= 0) {
        _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has BALANCE and BET <= 0 but SEATED and ACTIVE");
        _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: BALANCE = [" + to_string(balance) + "]; BET = [" + to_string(bet) + "];");
        _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BALANCE to 0");
        _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BET to 16bb");
        p_info[P_BET] = 16;
        p_info[P_BALANCE] = 0;
    }
    else if (bet == 0) {
        if (balance / bblind < MIN_STACK_SIZE || balance / bblind > MAX_STACK_SIZE) {
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has unknown BALANCE = [" + to_string(balance) + "]");
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BALANCE to 16bb");
            p_info[P_BALANCE] = 16;
        }
        else {
            p_info[P_BALANCE] = _conv.stack_to_blinds(balance, bblind);
        }
    }
    else if (bet > 0 && balance <= 0) {
        if (balance < 0) {
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has negative BALANCE = [" + to_string(balance) + "]");
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BALANCE to 0");
            p_info[P_BALANCE] = 0;
        }
        if (bet / bblind < MIN_STACK_SIZE || bet / bblind > MAX_STACK_SIZE) {
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has unknown BET = [" + to_string(bet) + "]");
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BET to 16bb");
            p_info[P_BET] = 16;
        }
        else {
            p_info[P_BET] = _conv.stack_to_blinds(bet, bblind);
        }
    }
    else {
        if (bet == static_cast<double>(bblind) / 2 || bet == bblind) {
            bet == bblind ? p_info[P_BET] = 1 : p_info[P_BET] = 0.5;
            if (balance / bblind < MIN_STACK_SIZE - p_info[P_BET] || balance / bblind > MAX_STACK_SIZE - p_info[P_BET]) {
                _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has unknown BALANCE = [" + to_string(balance) + "]");
                _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BALANCE to 15bb");
                p_info[P_BALANCE] = 15;
            }
            else if (balance / bblind == MIN_STACK_SIZE - p_info[P_BET] || balance / bblind == MAX_STACK_SIZE - p_info[P_BET]) {
                p_info[P_BALANCE] = p_info[P_BET] == 1 ? _conv.stack_to_blinds(balance, bblind) : _conv.stack_to_blinds(balance, bblind) - p_info[P_BET];
            }
            else {
                p_info[P_BALANCE] = _conv.stack_to_blinds(balance, bblind);
            }
        }
        else {
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has unknown BET = [" + to_string(bet) + "]");
            _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BET to 1bb");
            p_info[P_BET] = 1;
            if (balance / bblind < MIN_STACK_SIZE || balance / bblind > MAX_STACK_SIZE) {
                _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Chair [" + chair + "] has unknown BALANCE = [" + to_string(balance) + "]");
                _ohCommunicator.write_log("set_player_bet_and_balance", "ERROR: Changing BALANCE to 15bb");
                p_info[P_BALANCE] = 15;
            }
            else {
                p_info[P_BALANCE] = _conv.stack_to_blinds(balance, bblind);
            }
        }
    }
}

inline void TableInfo::set_big_blind(const int value) {
    if (value != 4 && value != 10 && value != 50 && value != 200) {
        _ohCommunicator.write_log("set_big_blind", "ERROR: Unknown big blind size: [" + to_string(value) + "]");
        _ohCommunicator.write_log("set_big_blind", "ERROR: Setting big blind to 0");
        bblind = 0;
        return;
    }
    bblind = value;
}

inline int TableInfo::get_big_blind() {
    return static_cast<int>(_ohCommunicator.read_symbol(BIG_BLIND_SYMBOL, BUILT_IN_SYMBOL));
}




inline bool TableInfo::set_bet_values_on_blinds() {
    string bb_chairs, sb_chairs;
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_BET] == 1) {
            bb_chairs.append(to_string(i));
        }
        else if (table_info[i][P_BET] == 0.5) {
            sb_chairs.append(to_string(i));
        }
    }
    switch (bb_chairs.length()) {
        case 0: return set_blinds_when_no_bb_found(sb_chairs);
        case 1: return set_blinds_when_one_bb_found(bb_chairs, sb_chairs);
        case 2: return set_blinds_when_two_bb_found(bb_chairs, sb_chairs);
        case 3: return set_blinds_when_three_bb_found(bb_chairs);
        case 4: return set_blinds_when_four_bb_found();
        default: {
            _ohCommunicator.write_log("set_bet_values_on_blinds", "ERROR: Unknown chairs format: bb_chairs = [" + bb_chairs + "]");
            break;
        }
    }
    return false;
}

inline bool TableInfo::set_blinds_when_no_bb_found(const string& sb_chairs) {
    _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: No big blind found");
    auto chair = -1;
    switch (sb_chairs.length()) {
        case 0: {
            _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: No small blind found");
            break;
        }
        case 1: {
            _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: 1 small blind found. Chair [" + sb_chairs + "]");
            if (!check_enough_players_active()) {
                break;
            }
            for (auto i = 0; i <= 2; ++i) {
                chair = atoi(sb_chairs.c_str()) + i + 1;
                if (chair > 3) {
                    chair = chair - 4;
                }
                if (table_info[chair][P_ACTIVE] == 1) {
                    _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: Changing chair [" + to_string(chair) + "] bet to big blind");
                    table_info[chair][P_BET] = 1;
                    if (table_info[chair][P_BALANCE] > MAX_STACK_SIZE - 1) {
                        table_info[chair][P_BALANCE] = MAX_STACK_SIZE - 1;
                    }
                }
            }
            break;
        }
        case 2: {
            const auto first = sb_chairs.substr(0, 1);
            const auto second = sb_chairs.substr(1, 1);
            _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: 2 small blinds found. Chairs: [" + first + "] and [" + second + "]");
            if (first == "0" && second == "1" && !(table_info[2][P_ACTIVE] == 0 && table_info[3][P_ACTIVE] == 0)) {
                chair = 1;
            }
            else if (first == "1" && second == "2" && !(table_info[3][P_ACTIVE] == 0 && table_info[0][P_ACTIVE] == 0)) {
                chair = 2;
            }
            else if (first == "2" && second == "3" && !(table_info[0][P_ACTIVE] == 0 && table_info[1][P_ACTIVE] == 0)) {
                chair = 3;
            }
            else if (first == "0" && second == "3" && !(table_info[1][P_ACTIVE] == 0 && table_info[2][P_ACTIVE] == 0)) {
                chair = 0;
            }
            else if (first == "0" && second == "2") {
                if (table_info[1][P_ACTIVE] == 0 && table_info[3][P_ACTIVE] == 1) {
                    chair = 2;
                }
                else if (table_info[1][P_ACTIVE] == 1 && table_info[3][P_ACTIVE] == 0) {
                    chair = 0;
                }
            }
            else if (first == "1" && second == "3") {
                if (table_info[2][P_ACTIVE] == 0 && table_info[0][P_ACTIVE] == 1) {
                    chair = 3;
                }
                else if (table_info[2][P_ACTIVE] == 1 && table_info[0][P_ACTIVE] == 0) {
                    chair = 1;
                }
            }
            if (chair != -1) {
                _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: Changing chair [" + to_string(chair) + "] bet to big blind");
                table_info[chair][P_BET] = 1;
                if (table_info[chair][P_BALANCE] < MIN_STACK_SIZE - 0.5 && table_info[chair][P_BALANCE] > MAX_STACK_SIZE - 0.5) {
                    table_info[chair][P_BALANCE] = 15;
                }
                else {
                    table_info[chair][P_BALANCE] = table_info[chair][P_BALANCE] - 0.5;        
                }
            }
            else {
                _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: Unknown small blinds information: sb_chairs = [" + sb_chairs + "]");
            }
            break;
        }
        case 3: {
            _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: 3 small blinds found. Chairs: [" + sb_chairs.substr(0,1) + "], [" + sb_chairs.substr(1,1) + "], [" + sb_chairs.substr(2,1) + "]");
            break;
        }
        case 4: {
            _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: 4 small blinds found");
            break;
        }
        default: {
            _ohCommunicator.write_log("set_blinds_when_no_bb_found", "ERROR: Unknown small blinds information format: [" + sb_chairs + "]");
            break;
        }
    }
    if (chair == -1) {
        log_table_info();
        clear_table_info();
        return false;
    }
    return true;
}

inline bool TableInfo::set_blinds_when_one_bb_found(const string& bb_chairs, const string& sb_chairs) {
    auto chair = -1;
    auto chair1 = -1;
    auto chair2 = -1;
    switch (sb_chairs.length()) {
        case 0: {
            _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: No small blind information found");
            if (!check_enough_players_active()) {
                break;
            }
            for (auto i = 0; i <= 2; ++i) {
                chair = atoi(bb_chairs.c_str()) - i - 1;
                if (chair < 0) {
                    chair = chair + 4;
                }
                if (table_info[chair][P_ACTIVE] == 1) {
                    _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Changing chair [" + to_string(chair) + "] bet to small blind");
                    table_info[chair][P_BET] = 0.5;
                    if (table_info[chair][P_BALANCE] > MAX_STACK_SIZE - 0.5) {
                        table_info[chair][P_BALANCE] = MAX_STACK_SIZE - 0.5;
                    }
                }
            }
            break;
        }
        case 1: {
            chair  = 0;
            chair1 = 0;
            chair2 = 0;
            break;
        }
        case 2: {
            _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: 2 small blinds found. Chairs: [" + sb_chairs.substr(0,1) + "], [" + sb_chairs.substr(1,1) + "]");
            switch (atoi(bb_chairs.c_str())) {
                case 0: {
                    if (sb_chairs.substr(0, 1) == "1" && sb_chairs.substr(1, 1) == "2" ||
                        sb_chairs.substr(0, 1) == "1" && sb_chairs.substr(1, 1) == "3") {
                        chair = 1;
                    }
                    else if (sb_chairs.substr(0, 1) == "2" && sb_chairs.substr(1, 1) == "3") {
                        chair = 2;
                    }
                    break;
                }
                case 1: {
                    if (sb_chairs.substr(0, 1) == "2" && sb_chairs.substr(1, 1) == "3" ||
                        sb_chairs.substr(0, 1) == "0" && sb_chairs.substr(1, 1) == "2") {
                        chair = 2;
                    }
                    else if (sb_chairs.substr(0, 1) == "0" && sb_chairs.substr(1, 1) == "3") {
                        chair = 3;
                    }
                    break;
                }       
                case 2: {
                    if (sb_chairs.substr(0, 1) == "0" && sb_chairs.substr(1, 1) == "3" ||
                        sb_chairs.substr(0, 1) == "1" && sb_chairs.substr(1, 1) == "3") {
                        chair = 3;
                    }
                    else if (sb_chairs.substr(0, 1) == "0" && sb_chairs.substr(1, 1) == "1") {
                        chair = 0;
                    }
                    break;
                }         
                case 3: {
                    if (sb_chairs.substr(0, 1) == "0" && sb_chairs.substr(1, 1) == "1" ||
                        sb_chairs.substr(0, 1) == "0" && sb_chairs.substr(1, 1) == "2") {
                        chair = 0;
                    }
                    else if (sb_chairs.substr(0, 1) == "1" && sb_chairs.substr(1, 1) == "2") {
                        chair = 1;
                    }
                    break;
                }                         
                default: {
                    _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Unknown big blind chair: [" + bb_chairs + "]");
                    break;
                }

            }
            if (chair != -1) {
                _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Changing chair [" + to_string(chair) + "] bet to allin");
                table_info[chair][P_BET] = 0.5 + table_info[chair][P_BALANCE];
                table_info[chair][P_BALANCE] = 0;                
            }
            else {
                _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Unknown small blinds chairs: [" + sb_chairs.substr(0, 1) + "] [" + sb_chairs.substr(1, 1) + "]");
            }
            break;
        }
        case 3: {
            _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: 3 small blinds found. Chairs: [" + sb_chairs.substr(0,1) + "], [" + sb_chairs.substr(1,1) + "], [" + sb_chairs.substr(2,1) + "]");
            if (sb_chairs == "012") {
                chair1 = 0;
                chair2 = 1;
            }
            else if (sb_chairs == "013") {
                chair1 = 3;
                chair2 = 0;
            }
            else if (sb_chairs == "023") {
                chair1 = 2;
                chair2 = 3;
            }
            else if (sb_chairs == "123") {
                chair1 = 1;
                chair2 = 2;
            }
            if (chair1 != -1 && chair2 != -1) {
                _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Changing chair [" + to_string(chair1) + "] bet to allin");
                table_info[chair1][P_BET] = 0.5 + table_info[chair1][P_BALANCE];
                table_info[chair1][P_BALANCE] = 0;                
                _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Changing chair [" + to_string(chair2) + "] bet to allin");
                table_info[chair2][P_BET] = 0.5 + table_info[chair2][P_BALANCE];
                table_info[chair2][P_BALANCE] = 0;                
            }
            else {
                _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Unknown small blinds chairs: [" + sb_chairs.substr(0, 1) + "] [" + sb_chairs.substr(1, 1) + "] [" + sb_chairs.substr(2, 1) + "]");
            }
            break;
        }
        default: {
            _ohCommunicator.write_log("set_blinds_when_one_bb_found", "ERROR: Unknown small blinds information format: [" + sb_chairs + "]");
            break;
        }
    }
    if (chair == -1 && chair1 == -1 && chair2 == -1) {
        log_table_info();
        clear_table_info();
        return false;
    }
    return true;
}

inline bool TableInfo::set_blinds_when_two_bb_found(const string& bb_chairs, const string& sb_chairs) {
    const auto first = bb_chairs.substr(0, 1);
    const auto second = bb_chairs.substr(1, 1);
    auto chair = -1;
    _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: Two big blinds found. Chairs: [" + first + "] and [" + second + "]");
    switch (sb_chairs.length()) {
        case 0: {
            _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: No small blind found");
            if (first == "0" && second == "1" && !(table_info[2][P_ACTIVE] == 0 && table_info[3][P_ACTIVE] == 0)) {
                chair = 0;
            }
            else if (first == "1" && second == "2" && !(table_info[3][P_ACTIVE] == 0 && table_info[0][P_ACTIVE] == 0)) {
                chair = 1;
            }
            else if (first == "2" && second == "3" && !(table_info[0][P_ACTIVE] == 0 && table_info[1][P_ACTIVE] == 0)) {
                chair = 2;
            }
            else if (first == "0" && second == "3" && !(table_info[1][P_ACTIVE] == 0 && table_info[2][P_ACTIVE] == 0)) {
                chair = 3;
            }
            else if (first == "1" && second == "3") {
                if (table_info[0][P_ACTIVE] == 1 && table_info[2][P_ACTIVE] == 0) {
                    chair = 1;
                }
                else if (table_info[0][P_ACTIVE] == 0 && table_info[2][P_ACTIVE] == 1) {
                    chair = 3;
                }
            }
            else if (first == "0" && second == "2") {
                if (table_info[3][P_ACTIVE] == 1 && table_info[1][P_ACTIVE] == 0) {
                    chair = 0;
                }
                else if (table_info[3][P_ACTIVE] == 0 && table_info[1][P_ACTIVE] == 1) {
                    chair = 2;
                }
            }
            if (chair != -1) {
                _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: Changing chair [" + to_string(chair) + "] bet to small blind");
                table_info[chair][P_BET] = 0.5;
                if (table_info[chair][P_BALANCE] < MIN_STACK_SIZE - 1 && table_info[chair][P_BALANCE] > MAX_STACK_SIZE - 1) {
                    table_info[chair][P_BALANCE] = 15.5;
                }
                else {
                    table_info[chair][P_BALANCE] = table_info[chair][P_BALANCE] + 0.5;        
                }
            }
            else {
                _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: Unknown big blinds information: bb_chairs = [" + bb_chairs + "]");
            }
            break;
        }
        case 1: {
            _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: One small blind found. Chair [" + sb_chairs + "]");
            if (bb_chairs == "01") {
                if (sb_chairs == "2" && table_info[3][P_ACTIVE] == 0 || sb_chairs == "3") {
                    chair = 1;
                }
            }
            else if (bb_chairs == "12") {
                if (sb_chairs == "3" && table_info[0][P_ACTIVE] == 0 || sb_chairs == "0") {
                    chair = 2;
                }
            }
            else if (bb_chairs == "23") {
                if (sb_chairs == "0" && table_info[1][P_ACTIVE] == 0 || sb_chairs == "1") {
                    chair = 3;
                }
            }
            else if (bb_chairs == "03") {
                if (sb_chairs == "1" && table_info[2][P_ACTIVE] == 0 || sb_chairs == "2") {
                    chair = 0;
                }
            }            
            else if (bb_chairs == "02") {
                if (sb_chairs == "3") {
                    chair = 2;
                }
                else if(sb_chairs == "1") {
                    chair = 0;
                }
            }        
            else if (bb_chairs == "13") {
                if (sb_chairs == "2") {
                    chair = 1;
                }
                else if(sb_chairs == "0") {
                    chair = 3;
                }
            }        
            if (chair != -1) {
                _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: Changing chair [" + to_string(chair) + "] bet to allin");
                table_info[chair][P_BET] = 1 + table_info[chair][P_BALANCE];
                table_info[chair][P_BALANCE] = 0;                
            }
            else {
                _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: Unknown blinds information: bb_chairs = [" + bb_chairs + "]; sb_chairs = [" + sb_chairs + "]");
            }
            break;
        }
        case 2: {
            _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: Unknown blinds information: bb_chairs = [" + bb_chairs + "]; sb_chairs = [" + sb_chairs + "]");
            break;
        }
        default: {
            _ohCommunicator.write_log("set_blinds_when_two_bb_found", "ERROR: Unknown small blind information: [" + sb_chairs + "]");
            break;
        }
    }
    if (chair == -1) {
        log_table_info();
        clear_table_info();
        return false;        
    }
    return true;
}

inline bool TableInfo::set_blinds_when_three_bb_found(const string& bb_chairs) {
    _ohCommunicator.write_log("set_blinds_when_three_bb_found", "ERROR: Three big blinds found. Chairs: [" + bb_chairs.substr(0,1) + "], [" + bb_chairs.substr(1,1) + "], [" + bb_chairs.substr(2,1) + "]");
    log_table_info();
    clear_table_info();
    return false;        
}

inline bool TableInfo::set_blinds_when_four_bb_found() {
    _ohCommunicator.write_log("set_blinds_when_three_bb_found", "ERROR: Four big blinds found");
    log_table_info();
    clear_table_info();
    return false;        
}



inline void TableInfo::log_table_info() {
    for (auto i = 0; i >= 3; ++i) {
        _ohCommunicator.write_log("log_table_info", "ERROR: P" + to_string(i).append(": ACTIVE = [").append(to_string(table_info[i][P_ACTIVE])).append("]; BET = [").append(to_string(table_info[i][P_BET])).append("]; BALANCE = [").append(to_string(table_info[i][P_BALANCE])).append("]"));
    }
}

inline bool TableInfo::check_enough_players_seated() {
    auto counter = 0;
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_SEATED] == 1) {
            ++counter;
        }
    }
    if (counter <= 1) {
        _ohCommunicator.write_log("check_enough_players_seated", "ERROR: Not enough players seated");
        return false;    
    }
    return true;
}

inline bool TableInfo::check_enough_players_active() {
    auto counter = 0;
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_ACTIVE] == 1) {
            ++counter;
        }
    }
    if (counter <= 1) {
        _ohCommunicator.write_log("check_enough_players_active", "ERROR: Not enough players active");
        return false;
    }
    return true;
}

inline void TableInfo::clear_table_info() {
    for (auto& player_info : table_info) {
        player_info = p_info_empty_vector;
    }
}


inline bool TableInfo::set_all_chairs_positions() {
    if (!set_bb_position()) {
        log_table_info();
        clear_table_info();
        return false;
    }
    if (!set_sb_position()) {
        log_table_info();
        clear_table_info();
        return false;
    }
    if (!set_bu_position()) {
        log_table_info();
        clear_table_info();
        return false;
    }
    if (!set_co_position()) {
        log_table_info();
        clear_table_info();
        return false;
    }

    return true;
}

inline bool TableInfo::set_bb_position() {
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_BET] == 1 && table_info[i][P_ACTIVE] == 1) {
            table_info[i][P_POSITION] = POSITION_BB;
            return true;
        }
    }
    _ohCommunicator.write_log("set_bb_position", "ERROR: Unknown big blind position.");
    return false;
}

inline bool TableInfo::set_sb_position() {
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_BET] == 0.5 && table_info[i][P_ACTIVE] == 1) {
            table_info[i][P_POSITION] = POSITION_SB;
            return true;
        }
    }
    _ohCommunicator.write_log("set_sb_position", "ERROR: Unknown small blind position.");
    return false;
}

inline bool TableInfo::set_bu_position() {
    double bb_chair = -1, sb_chair = -1;
    auto active_players_counter = 0;
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_SEATED] == 1 && table_info[i][P_ACTIVE] == 1) {
            active_players_counter ++;
        }
        if (table_info[i][P_POSITION] == POSITION_BB) {
            bb_chair = table_info[i][P_CHAIR];
        }
        else if (table_info[i][P_POSITION] == POSITION_SB) {
            sb_chair = table_info[i][P_CHAIR];
        }
    }
    if (sb_chair == -1 || bb_chair == -1) {
        _ohCommunicator.write_log("set_bu_position", "ERROR: Unknown BB or SB position: bb_chair = [" + to_string(bb_chair)+ "]; sb_chair = [" + to_string(sb_chair) + "]");
        return false;
    }
    switch (active_players_counter) {
        case 2: return true;
        case 3: {
            for (auto i = 0; i <= 3; ++i) {
                if (table_info[i][P_POSITION] == 0 && table_info[i][P_SEATED] == 1 && table_info[i][P_ACTIVE] == 1) {
                    table_info[i][P_POSITION] = POSITION_BU;
                    return true;
                }
            }
            break;
        }
        case 4: {
            if (sb_chair == 0 && bb_chair == 1) {
                table_info[3][P_POSITION] = POSITION_BU;
            }
            else if (sb_chair == 1 && bb_chair == 2) {
                table_info[0][P_POSITION] = POSITION_BU;
            }
            else if (sb_chair == 2 && bb_chair == 3) {
                table_info[1][P_POSITION] = POSITION_BU;
            }
            else if (sb_chair == 3 && bb_chair == 0) {
                table_info[2][P_POSITION] = POSITION_BU;
            }
            else {
                _ohCommunicator.write_log("set_bu_position", "ERROR: Unknown BB or SB position: bb_chair = [" + to_string(bb_chair) + "]; sb_chair = [" + to_string(sb_chair) + "]");
                break;
            }
            return true;
        }
        default: {
            _ohCommunicator.write_log("set_bu_position", "ERROR: Unknown active players count: [" + to_string(active_players_counter) + "]");
            break;
        }
    }
    _ohCommunicator.write_log("set_button_position", "ERROR: Unknown button position.");
    log_table_info();
    clear_table_info();
    return false;
}

inline bool TableInfo::set_co_position() {
    double bb_chair = -1, sb_chair = -1, bu_chair = -1;
    auto active_players_counter = 0;
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_SEATED] == 1 && table_info[i][P_ACTIVE] == 1) {
            active_players_counter ++;
        }
        if (table_info[i][P_POSITION] == POSITION_BB) {
            bb_chair = table_info[i][P_CHAIR];
        }
        else if (table_info[i][P_POSITION] == POSITION_SB) {
            sb_chair = table_info[i][P_CHAIR];
        }
        else if (table_info[i][P_POSITION] == POSITION_BU) {
            bu_chair = table_info[i][P_CHAIR];
        }
    }
    if (sb_chair == -1 || bb_chair == -1) {
        _ohCommunicator.write_log("set_bu_position", "ERROR: Unknown BB or SB position: bb_chair = [" + to_string(bb_chair) + "]; sb_chair = [" + to_string(sb_chair) + "]");
        return false;
    }
    if (bu_chair == -1 && active_players_counter >= 3) {
        _ohCommunicator.write_log("set_bu_position", "ERROR: Unknown BU position: bu_chair = [" + to_string(bu_chair) + "]");
        return false;
    }
    switch (active_players_counter) {
        case 2: return true;
        case 3: return true;
        case 4: {
            if (bu_chair == 3 && sb_chair == 0 && bb_chair == 1) {
                table_info[2][P_POSITION] = POSITION_CO;
            }
            else if (bu_chair == 0 && sb_chair == 1 && bb_chair == 2) {
                table_info[3][P_POSITION] = POSITION_CO;
            }
            else if (bu_chair == 1 && sb_chair == 2 && bb_chair == 3) {
                table_info[0][P_POSITION] = POSITION_CO;
            }
            else if (bu_chair == 2 && sb_chair == 3 && bb_chair == 0) {
                table_info[1][P_POSITION] = POSITION_CO;
            }
            else {
                _ohCommunicator.write_log("set_bu_position", "ERROR: Unknown BB, SB or BU position: bb_chair = [" + to_string(bb_chair) + "]; sb_chair = [" + to_string(sb_chair) + "]; bu_chair = [" + to_string(bu_chair) + "]");
                break;
            }
            return true;
        }
        default: {
            _ohCommunicator.write_log("set_bu_position", "ERROR: Unknown active players count: [" + to_string(active_players_counter)+ "]");
            break;
        }
    }
    _ohCommunicator.write_log("set_button_position", "ERROR: Unknown CO position.");
    log_table_info();
    clear_table_info();
    return false;
}

inline void TableInfo::set_allin_info() {
    for (auto i = 0; i <= 3; ++i) {
        if (table_info[i][P_SEATED] == 1 && table_info[i][P_ACTIVE] == 1 && table_info[i][P_BALANCE] == 0 && 
            table_info[i][P_BET] >= MIN_STACK_SIZE && table_info[i][P_BET] <= MAX_STACK_SIZE && 
            (table_info[i][P_POSITION] == POSITION_CO || table_info[i][P_POSITION] == POSITION_BU || table_info[i][P_POSITION] == POSITION_SB)) {
            table_info[i][P_ISALLIN] = 1;
        }
    }
}
