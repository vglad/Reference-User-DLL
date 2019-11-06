#pragma once
#ifndef USER_HELPERFUNCTIONS_H 
#define USER_HELPERFUNCTIONS_H

namespace Helper {
    
    // trim from start (in place)
    static void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from both ends (copying)
    static std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }


    inline bool FileExist(const std::string& filename) {
        struct stat buffer{};
        return (stat(filename.c_str(), &buffer) == 0);
    }

    inline std::string check_number_for_dots(std::string number_to_check) {
        const auto number = atoi(number_to_check.c_str());
        //const auto number = atoi(number_to_check);
        if (number < 100) {
            return number_to_check;
        }
        if (number < 1000) { //3 digits
            const auto first_digit = static_cast<int>(number / 100);
            switch (first_digit) {
                //for 2/4
                case 3: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 4: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 5: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 6: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 7: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 8: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 9: return std::to_string(static_cast<int>(number / 10)).c_str();
                //for 5/10
                //case 7: return std::to_string(static_cast<int>(number / 10)).c_str();
                //case 8: return std::to_string(static_cast<int>(number / 10)).c_str();
                //case 9: return std::to_string(static_cast<int>(number / 10)).c_str();
                default: return number_to_check;
            }
        }
        if (number < 10000) { //4 digits
            const auto first_digit = static_cast<int>(number / 1000);
            switch (first_digit) {
                //for 2/4
                case 1: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 2: return std::to_string(static_cast<int>(number / 10)).c_str();
                case 3: return std::to_string(static_cast<int>(number / 100)).c_str();
                case 4: return std::to_string(static_cast<int>(number / 100)).c_str();
                case 5: return std::to_string(static_cast<int>(number / 100)).c_str();
                case 6: return std::to_string(static_cast<int>(number / 100)).c_str();
                case 7: return std::to_string(static_cast<int>(number / 100)).c_str();
                case 8: return std::to_string(static_cast<int>(number / 100)).c_str();
                case 9: return std::to_string(static_cast<int>(number / 100)).c_str();
                //for 5/10
                //case 1: return std::to_string(static_cast<int>(number / 10)).c_str();
                //case 2: return std::to_string(static_cast<int>(number / 10)).c_str();
                //case 3: return std::to_string(static_cast<int>(number / 10)).c_str();
                //case 4: return std::to_string(static_cast<int>(number / 10)).c_str();
                //case 7: return std::to_string(static_cast<int>(number / 100)).c_str();
                //case 8: return std::to_string(static_cast<int>(number / 100)).c_str();
                //case 9: return std::to_string(static_cast<int>(number / 100)).c_str();
                default: return std::to_string(static_cast<int>(number / 10)).c_str();
            }
        }
        if (number < 100000) { //5 digits
            return std::to_string(static_cast<int>(number / 100)).c_str();
        }

        return number_to_check;
    }

    //#include <filesystem>
    //inline void create_percentage_file_in_directories() {
    //    for (auto i = 8; i < 17; ++i) {
    //        std::string files_dir;
    //        files_dir.append(R"(c:\Poker\soft\toUse\PKA\profile\exploit\sb_bb\sb_exploit\)").append(std::to_string(i)).append(R"(\)");
    //        std::cout << files_dir << "\n";
    //        std::string percentage_file;
    //        percentage_file = percentage_file.append(R"(c:\Poker\soft\toUse\PKA\profile\exploit\sb_bb\sb_exploit\)").append(std::to_string(i)).append(R"(\bb_ai.csv)");
    //        std::cout << percentage_file << "\n";
    //        std::ofstream percent_file_stream(percentage_file);
    //        std::vector<double> perc_vector;
    //        for (const auto & entry : std::experimental::filesystem::directory_iterator(files_dir)) {
    //            auto filename = entry.path().filename().generic_string();
    //            if (filename.find("ex", 0) != std::string::npos && filename != "bb_ai.csv") {
    //                std::string percent;
    //                percent = filename.substr(0, filename.length() - 4);
    //                percent = percent.substr(percent.find_last_of('_') + 1);
    //                std::cout << percent << "\n";
    //                perc_vector.insert(perc_vector.begin(), std::stod(percent));
    //            }
    //        }
    //        std::sort(perc_vector.begin(), perc_vector.end());
    //        std::string sorted_text;
    //        for (const auto& entry : perc_vector) {
    //            percent_file_stream << entry << "\n";
    //        }    
    //    }
    //    std::cout << "Done.\n"; 
    //}

          
}

#endif
