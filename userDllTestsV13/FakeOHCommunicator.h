#pragma once
#ifndef USER_FakeOHComm_H
#define USER_FakeOHComm_H

class OHCommunicator : public IBridge {
public:
    OHCommunicator();
    ~OHCommunicator();

    double      read_symbol(std::string const& name, symbol_type const& symbol_type) override;
    bool        write_symbol(std::string const& name, std::string const& value) override;
    std::string read_region(std::string const& name) override;
    void        write_log(std::string const& caller, std::string const& message) override;

};

OHCommunicator _ohCommunicator;

#include "FakeOHCommunicator.inl"

#endif
