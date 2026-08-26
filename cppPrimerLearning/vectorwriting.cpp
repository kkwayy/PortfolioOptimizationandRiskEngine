//
// Created by ethan on 15-Aug-26.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

int main() {
    std::ifstream file("data.csv");
    if (!file) {
        std::cerr << "could not open data.csv\n";
        return 1;
    }

    std::vector<std::vector<double>> rows;   // the whole table: rows of doubles




    // 1. read and discard the header line (one getline BEFORE the loop)

    std::string header;
    std::getline(file, header);


    // 2. outer loop: for each remaining line...
    //      - make an empty std::vector<double> for this row
    //      - inner loop: getline each field on the comma, stod it, push_back into the row vector
    //      - after the inner loop, push_back the completed row into `rows`

    std::string line;



    while (std::getline(file,line)) {
        std::istringstream ss(line);
        std::string field;

        std::vector<double> data;

        while (std::getline(ss,field,',')) {
            double value = std::stod(field);
            data.push_back(value);
        }

        rows.push_back(data);

    }

    // 3. after reading: print rows.size() to confirm how many rows you loaded

    std::cout<< rows.size()<< '\n';

    return 0;
}