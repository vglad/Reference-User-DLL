#pragma once
#ifndef USER_PT_H
#define USER_PT_H

class PT final {
    public:
    PT();
    ~PT();

    static std::string  generate_stat_name(int allin_situation, int hero_position, const std::vector<int>& stacks_vector, bool opportunities);
    static double       stat_value(const std::string& stat_name);
    static bool         enough_opportunities(double stat_value);
    static int          stat_opportunities_info(int allin_situation, int hero_position, const std::vector<int>& stacks_vector);
    static double       stat_name_info(int allin_situation, int hero_position, const std::vector<int>& stacks_vector, int stat_type);
};

PT _pt;

#include "PT.inl"

#endif
