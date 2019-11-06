// Class OHComm implements dll exported functions.
#pragma once
#ifndef USER_OHCOMM_H
#define USER_OHCOMM_H

class OHComm final{
public:
    OHComm();
    ~OHComm();

    static double       read_symbol(std::string const& name, symbol_type const& symbol_type);
    static bool         write_symbol(std::string const& name, std::string const& value);
  
    static std::string  read_region(std::string const& name);
    static std::string  read_hand_number();
    static void         save_list(std::string const& list_name, std::string const& list_combos);
    static void         write_log(std::string const& caller, std::string const& message);

    static char *       get_time(char * time_buf);
};

OHComm _oh;

#include "OHComm.inl"

#endif
