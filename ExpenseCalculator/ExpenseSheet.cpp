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

double ExpenseSheet::Eval() const {
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

bool ExpenseSheet::ExportCsv(const std::filesystem::path& file) const {
    auto xpath = file;
    xpath.replace_extension(".csv");
    
    auto path = xpath;
    if (path.remove_filename().empty() == false) {
        std::filesystem::create_directories(path);
    }
    
    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc);
    if (fileOut.is_open()) {
        fileOut << "Pos,Expense,Value" << '\n';
        size_t i = 1;
        for (const ExpenseSheet::Entry& e : m_entries) {
            fileOut << i++ << "," << e.label << "," << e.value << '\n';
        }
        return true;
    }
    
    return false;
}

bool ExpenseSheet::ExportHtml(const std::filesystem::path& file) const {
    auto xpath = file;
    xpath.replace_extension(".html");
    
    auto path = xpath;
    if (path.remove_filename().empty() == false) {
        std::filesystem::create_directories(path);
    }
    
    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc);
    if (fileOut.is_open()) {
        fileOut <<
            R"(<!doctype html>
            <html lang="en">
              <head>
                <!-- Required meta tags -->
                <meta charset="utf-8">
                <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

                <!-- Bootstrap CSS -->
                <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">

                <title>)" << m_path.filename().string() << R"(</title>
              </head>
              <body>
                <!-- As a heading -->
                <nav class="navbar navbar-light bg-light">
                  <span class="navbar-brand mb-0 h1">Expenses: )" << m_path.filename().string() << R"(</span>
                </nav>
                <div class="container">
                  <div class="card">
                    <div class="card-body">
                      <table class="table">
                        <thead>
                          <tr>
                            <th scope="col">Pos</th>
                            <th scope="col">Expese</th>
                            <th scope="col">Value</th>
                          </tr>
                        </thead>
                        <tbody>)";
        
        size_t i = 0;
        for (const ExpenseSheet::Entry& e : m_entries) {
            fileOut << R"(
                        <tr>
                          <th scope="row">)" << ++i << R"(</th>
                          <td>)" << e.label << R"(</td>
                          <td>)" << e.value << R"(</td>
                        </tr>)";
        }
    
        fileOut << R"(
                      </tbody>
                    </table>
                    <p class=".fs-3">Total: )" << this->Eval() << R"(</p>
                  </div>
                </div>
              </div>
            <!-- Optional JavaScript -->
            <!-- jQuery first, then Popper.js, then Bootstrap JS -->
            <script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
            <script src="https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
            <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
          </body>
        </html>)";
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
        if (c != '\0') {
            ss << c;
        }
    } while (c != '\0');
    
    label = ss.str();
    in.read((char*)&value, sizeof(double));
}
