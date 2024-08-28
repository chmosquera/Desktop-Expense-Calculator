//
//  main.cpp
//  ExpenseCalculator
//
//  Created by Chanelle Mosquera on 8/28/24.
//

#include "InputArguments.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main(int argc, const char * argv[]) {
    
    while (true) {
        // get input
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        
        InputArguments inArgs(line);
        std::string cmd = inArgs[0];
        auto args = inArgs.Subset(1);
        
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char c) {return std::tolower(c);});

        // handle command
        if (cmd == "add") {
            std::cout << "Command " << cmd << '\n';
        } else if (cmd == "del") {
            std::cout << "Command " << cmd << '\n';
        } else if (cmd == "list") {
            std::cout << "Command " << cmd << '\n';
        } else if (cmd == "eval") {
            std::cout << "Command " << cmd << '\n';
        } else if (cmd == "exit") {
            return 0;
        } else {
            std::cout << "Command " << cmd << " does not exist" << '\n';
        }
    }
    
    return 0;
}
