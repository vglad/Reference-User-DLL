#pragma once
#include "FakeOHCommunicator.h"

inline OHCommunicator::OHCommunicator() = default;
inline OHCommunicator::~OHCommunicator() = default;

inline double OHCommunicator::read_symbol(std::string const& name, symbol_type const& symbol_type) {
    //if (name == "HeroPosition" && symbol_type == MEMORY_SYMBOL) {
    //    return 2;
    //}
    return 0;
}

inline bool OHCommunicator::write_symbol(std::string const& name, std::string const& value) {
    return true;
}

inline void OHCommunicator::write_log(std::string const& caller, std::string const& message) {
}

inline std::string OHCommunicator::read_region(std::string const& name) {
    return "";
}

