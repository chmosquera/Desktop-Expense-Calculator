//
//  ExpenseSheet.hpp
//  ExpenseCalculator
//
//  Created by Chanelle Mosquera on 8/29/24.
//

#ifndef ExpenseSheet_hpp
#define ExpenseSheet_hpp

#include <stdio.h>
#include <string>
#include <string_view>
#include <iostream>
#include <ostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <filesystem>

class ExpenseSheet {
    public:
        
        struct Entry {
            double value = 0.;
            std::string label;
            
            inline bool operator==(const Entry& rhs) const {
                return label == rhs.label;
            }
            
            void Serialize(std::ostream& out) const;
            void Deserialize(std::istream& in);
        };
        
        ExpenseSheet() = default;
        ExpenseSheet(const ExpenseSheet&) = default;
        ExpenseSheet& operator=(const ExpenseSheet&) = default;
        
        bool Add(std::string label, double value);
        bool Del(std::string_view label);
        void List(std::ostream& os) const;
        double Eval();
    
        bool Open(const std::filesystem::path& datafile);
        bool Save(const std::filesystem::path& datafile) const;

        // store in a vector m_entries;
        // Default constructor, copy constructor, and copyable = operator
        // Add function to add a label and value, return true for success
        // Del function, given a label, return true for success
        // List writes to an outsstream
    private:
        std::vector<Entry> m_entries;
};

#endif /* ExpenseSheet_hpp */
