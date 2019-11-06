#pragma once
#include "OHComm.h"

inline OHComm::OHComm() = default;
inline OHComm::~OHComm() = default;

inline double OHComm::read_symbol(std::string const& name, symbol_type const& symbol_type) {
    std::string symbol;
    if (symbol_type == MEMORY_SYMBOL) {
        symbol.append("me_re_");
    }
    symbol.append(name);
    return GetSymbol(symbol.c_str());
}

inline bool OHComm::write_symbol(std::string const& name, std::string const& value) {
    std::string symbol;
    symbol.append("me_st_").append(name).append("_").append(value);
    GetSymbol(symbol.c_str());
    return true;
}

inline std::string OHComm::read_region(std::string const& name) {
    int result_length;
    std::string result;
    const auto scraped_result = ScrapeTableMapRegion(_strdup(name.c_str()), result_length);
    if (scraped_result != nullptr) {
        result = scraped_result;
    }
    LocalFree(scraped_result);
    return result;
}


inline std::string OHComm::read_hand_number() {
    return GetHandnumber();
}

inline void OHComm::save_list(std::string const& list_name, std::string const& list_combos) {
    ParseHandList(const_cast<char *>(list_name.c_str()), const_cast<char *>(list_combos.c_str()));
}

inline void OHComm::write_log(std::string const& caller, std::string const& message) {
    if (k_debug) {
        std:: string text;
        text.append(" [").append(caller).append("] ").append(message).append("\n");
        if (read_symbol(ACTIVE_FLAG_BUTTON_FOR_LOG,BUILT_IN_SYMBOL) == 1) {
            const auto str = const_cast<char *>(text.c_str());
            WriteLog(str);
        }
        else {
            char now_time[26];
            get_time(now_time);
            std::ofstream log_file(LOG_FILE_PATH, std::ios::app);
            log_file << now_time << text;
        }
    }
}

inline char *OHComm::get_time(char * time_buf) {
    // returns current system time in WH format
    time_t	ltime;
    char tmptime[30];
    time(&ltime);
    ctime_s(tmptime, 26, &ltime);
    tmptime[24] = '\0';
    memcpy(time_buf, tmptime + 20, 4); //yyyy
    *(time_buf + 4) = '-';
    // mm
    if (memcmp(tmptime + 4, "Jan", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '1';
    }
    else if (memcmp(tmptime + 4, "Feb", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '2';
    }
    else if (memcmp(tmptime + 4, "Mar", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '3';
    }
    else if (memcmp(tmptime + 4, "Apr", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '4';
    }
    else if (memcmp(tmptime + 4, "May", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '5';
    }
    else if (memcmp(tmptime + 4, "Jun", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '6';
    }
    else if (memcmp(tmptime + 4, "Jul", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '7';
    }
    else if (memcmp(tmptime + 4, "Aug", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '8';
    }
    else if (memcmp(tmptime + 4, "Sep", 3) == 0) {
        *(time_buf + 5) = '0';
        *(time_buf + 6) = '9';
    }
    else if (memcmp(tmptime + 4, "Oct", 3) == 0) {
        *(time_buf + 5) = '1';
        *(time_buf + 6) = '0';
    }
    else if (memcmp(tmptime + 4, "Nov", 3) == 0) {
        *(time_buf + 5) = '1';
        *(time_buf + 6) = '1';
    }
    else if (memcmp(tmptime + 4, "Dec", 3) == 0) {
        *(time_buf + 5) = '1';
        *(time_buf + 6) = '2';
    }
    *(time_buf + 7) = '-';
    memcpy(time_buf + 8, tmptime + 8, 2); //dd
    *(time_buf + 10) = ' ';
    memcpy(time_buf + 11, tmptime + 11, 8); //HH:mm:ss
    *(time_buf + 19) = '\0';
    return time_buf;
}
