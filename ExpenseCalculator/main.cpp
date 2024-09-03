//
//  main.cpp
//  ExpenseCalculator
//
//  Created by Chanelle Mosquera on 8/28/24.
//

#include "InputArguments.hpp"
#include "ExpenseSheet.hpp"

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <filesystem>

int main(int argc, const char * argv[]) {
    
    ExpenseSheet expenseSheet;
    
    // If program initiates with a filepath arg, try to open expense sheet.
    if (argc == 2) {
        std::filesystem::path path = argv[1];
        if (!expenseSheet.Open(path)) {
            std::cout << "Could not open the file at path " << (char*)&path << '\n';
        }
    }
    
    while (true) {
        // get input
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        
        InputArguments inArgs(line);
        std::string cmd = inArgs[0];
        auto args = inArgs.Subset(1);
        
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char c) {return std::tolower(c);});

        // Handle commands
        if (cmd == "add") {
            if (args.Count() == 2 || args.Count() == 3) {
                const auto& label = args[0];
                double value;
                
                if (args.Count() == 2) {
                    value = atof(args[1].c_str()) * -1.0;
                } else {
                    value = atof(args[2].c_str());
                    if (args[1] != "+") {
                        value *= -1.;
                    }
                }
                
                if (expenseSheet.Add(label, value) == false) {
                    std::cout << "Could not add '" << label << "'. Make sure that you enter the right values and try again." << '\n';
                    std::cout << "Usage: add <label> <optional: +|-> <value>" << '\n';
                }
            } else {
                std::cout << "Usage: add <label> <optional: +|-> <value>" << '\n';
            }
        } else if (cmd == "del") {
            if (args.Count() == 1) {
                if (expenseSheet.Del(args[0]) == false) {
                    std::cout << "Could not find '" << args[0] << "'. Make sure that you enter the right label and try again. Use command 'list' to see all entries." << '\n';
                    std::cout << "Usage: del <label>" << '\n';
                }
            } else {
                std::cout << "Usage: del <label>" << '\n';
            }
        } else if (cmd == "list") {
            expenseSheet.List(std::cout);
        } else if (cmd == "eval") {
            double total = expenseSheet.Eval();
            std::cout << "Total: " << std::setprecision(3) << total << '\n';
        } else if (cmd == "exit") {
            return 0;
        } else if (cmd == "open") {
            if (args.Count() == 1) {
                std::filesystem::path p = args[0];
                p.replace_extension(".dat");
                if (!expenseSheet.Open(p)) {
                    std::cout << "Could not open the file." << '\n';
                }
            } else {
                std::cout << "Usage: open <path>" << '\n';
            }
        } else if (cmd == "save") {
            if (args.Count() == 1) {
                std::filesystem::path p = args[0];
                p.replace_extension(".dat");
                if (!expenseSheet.Save(p)) {
                    std::cout << "Could not save file at path " << (char*)&p << '\n';
                    std::cout << "Usage: save <path>" << '\n';
                }
            } else if (args.Count() == 0) {
                if (!expenseSheet.Save()) {
                    std::cout << "Could not save file." << '\n';
                    std::cout << "Usage: save <path>" << '\n';
                }
            } else {
                std::cout << "Usage: save <path>" << '\n';
            }
            
        } else if (cmd == "export") {
            if (args.Count() == 2) {
                if (args[0] == "csv") {
                    std::filesystem::path p = args[1];
                    expenseSheet.ExportCsv(p);
                } else if (args[0] == "html") {
                    std::filesystem::path p = args[1];
                    expenseSheet.ExportHtml(p);
                } else {
                    std::cout << "Usage: export <csv|html>" << '\n';
                }
                
            } else {
                std::cout << "Usage: export <csv|html>" << '\n';
            }
        }
        else {
            std::cout << "Command " << cmd << " does not exist" << '\n';
        }
    }
    
    return 0;
}
