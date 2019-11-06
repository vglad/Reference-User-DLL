#pragma once
#ifndef USER_OHCommunicator_H
#define USER_OHCommunicator_H

class OHCommunicator final : public IBridge {
public:
    OHCommunicator();
    ~OHCommunicator();

    double      read_symbol(std::string const& name, symbol_type const& symbol_type) override;
    bool        write_symbol(std::string const& name, std::string const& value) override;
    std::string read_region(std::string const& name) override;    
    void        write_log(std::string const& caller, std::string const& message) override;
        
    static char * get_time(char * time_buf);
};

OHCommunicator _ohCommunicator;

#include "OHCommunicator.inl"

#endif
