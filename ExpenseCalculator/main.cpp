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

int main(int argc, const char * argv[]) {
    ExpenseSheet::Entry e, ex;
    e.label = "Test Test Test";
    e.value = 99.23123;
    std::ofstream fileOut("./entry.dat", std::ios::out | std::ios::binary);
    e.Serialize(fileOut);
    fileOut.close();
    
    std::ifstream fileIn("./entry.dat", std::ios::in | std::ios::binary);
    ex.Deserialize(fileIn);
    fileIn.close();
    
    ExpenseSheet expenseSheet;
    
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
                    std::cout << "Usage: add <label> <optional: +/-> <value>" << '\n';
                }
            } else {
                std::cout << "Usage: add <label> <optional: +/-> <value>" << '\n';
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
        } else {
            std::cout << "Command " << cmd << " does not exist" << '\n';
        }
    }
    
    return 0;
}
