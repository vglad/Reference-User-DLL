#pragma once

class IBridge {
    public:
        virtual ~IBridge() = default;
        
        virtual double      read_symbol(std::string const& name, symbol_type const& symbol_type) = 0;
        virtual bool        write_symbol(std::string const& name, std::string const& value) = 0;
        virtual std::string read_region(std::string const& name) = 0;
        virtual void        write_log(std::string const& caller, std::string const& message) = 0;
        
        
};







