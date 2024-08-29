//
//  ExpenseSheet.cpp
//  ExpenseCalculator
//
//  Created by Chanelle Mosquera on 8/29/24.
//

#include "ExpenseSheet.hpp"

bool ExpenseSheet::Add(std::string label, double value) {
    Entry entry;
    entry.label = label;
    entry.value = value;
    
    auto element = std::find(m_entries.begin(), m_entries.end(), entry);
    if (element == m_entries.end()) {
        m_entries.push_back(std::move(entry));
        return true;
    }
    return false;
}

bool ExpenseSheet::Del(std::string_view label) {
    Entry entry;
    entry.label = label;
    auto element = std::find(m_entries.begin(), m_entries.end(), entry);
    if (element != m_entries.end()) {
        m_entries.erase(element);
        return true;
    }
    return false;
}

void ExpenseSheet::List(std::ostream& os) const {
    size_t idx = 0;    
    for (auto entry : m_entries) {
        std::cout << "(" << idx << ") " << entry.label << " " << entry.value <<'\n';
    }
}

double ExpenseSheet::Eval() {
    double sum = 0;
    for (const Entry& entry : m_entries) {
        sum += entry.value;
    }
    return sum;
}
