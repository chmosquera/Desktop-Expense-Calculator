//
//  main.cpp
//  ExpenseCalculator
//
//  Created by Chanelle Mosquera on 8/28/24.
//

#include <iostream>
#include <string>

int main(int argc, const char * argv[]) {
    
    while (true) {
        // get input
        std::cout << "> ";
        std::string cmd;
        std::getline(std::cin, cmd);

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
