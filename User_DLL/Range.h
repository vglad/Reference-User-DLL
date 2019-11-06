#pragma once
#ifndef USER_RANGE_H
#define USER_RANGE_H

class Range final {
    public:
    Range();
    ~Range();
    

    //static double evaluate_stat_percent(double stat_name_percent);
    //static std::string get_string_percent(double percent);
    //static std::vector<std::string> get_full_range_order(const std::string& full_range_order_name);
    //static std::string get_full_range_order_name(std::string const& stat_name);
    //static std::string get_range_name(std::string const& percent);
    //static std::string get_range_combos(double percent, std::vector<std::string> const& full_range_order);
    //static double get_combo_percent(std::string const& combo_type);
    //static bool add_combo(std::string &range, std::string const& combo);
    
    static std::string      gto_file(const std::vector<int>& stacks_vector, int allin_situation);
    static int              check_strategy_file(std::string const& file, bool gto_file);
    static std::string      exploit_file(const std::vector<int>& stacks_vector, int allin_situation, double stat_percent);
    static std::vector<std::string> combo_allin_info(const std::string& strategy_file_full_path, const std::string& combo, bool gto_info);


    private:
        static std::string gto_file_for_first_to_act(const std::vector<int>& stacks_vector, double hero_position);
        static std::string gto_file_for_one_allin(const std::vector<int>& stacks_vector, double hero_position, double one_allin_position);
        static std::string gto_file_for_two_allins(const std::vector<int>& stacks_vector, double hero_position, double two_allin_positions);
        static std::string gto_file_for_three_allins(const std::vector<int>& stacks_vector, double hero_position, double three_allin_positions);

        static std::string exploit_file_for_first_to_act(const std::vector<int>& stacks_vector, double hero_position, double stat_percent);
        static std::string exploit_file_for_one_allin(const std::vector<int>& stacks_vector, double hero_position, double stat_percent);
        
};

Range _range;

#include "Range.inl"

#endif
