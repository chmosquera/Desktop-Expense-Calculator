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

bool ExpenseSheet::Open(const std::filesystem::path& datafile) {
    
    std::ifstream fileIn(datafile, std::ios::in | std::ios::binary);
    if (fileIn.is_open()) {
        m_path = datafile;
        size_t elementCount = 0;
        fileIn.read((char*)&elementCount, sizeof(size_t));
        
        for (size_t i = 0; i < elementCount; i++) {
            ExpenseSheet::Entry e;
            e.Deserialize(fileIn);
            m_entries.push_back(std::move(e));
        }
        return true;
    }
    
    return false;
}

bool ExpenseSheet::Save(const std::filesystem::path& datafile) const {
    auto xpath = datafile;
    
    if (datafile.empty()) {
        if (m_path.empty()) {
            return false;
        } else {
            xpath = m_path;
        }
    }
    
    // Create the directories by passing in only directories, not filename
    auto path = xpath;
    path.remove_filename();
    
    if (path.empty() == false) {
        std::filesystem::create_directories(path);
    }
    
    // Tip: trunc overwrites the file if one exists
    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc | std::ios::binary);
    if (fileOut.is_open()) {
        // Write number of elements for easy processing when opening
        size_t elementCount = m_entries.size();
        fileOut.write((char*)&elementCount, sizeof(size_t));
                
        for (const ExpenseSheet::Entry& e : m_entries) {
            e.Serialize(fileOut);
        }
        
        return true;
    }
    return false;
    
}


void ExpenseSheet::Entry::Serialize(std::ostream& out) const {
    out.write(label.c_str(), label.length() + 1);
    out.write((char*)&value, sizeof(double));
}

void ExpenseSheet::Entry::Deserialize(std::istream& in) {
    std::stringstream ss;
    char c;
    do {
        in.read(&c, 1);
        ss << c;
    } while (c!= '\0');
    
    label = ss.str();
    in.read((char*)&value, sizeof(double));
}
