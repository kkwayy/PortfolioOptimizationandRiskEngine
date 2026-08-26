//
// Created by ethan on 24-Aug-26.
//

// DataLoader.cpp
#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>

std::vector<std::vector<double>> loadCSV(const std::string& filename) {
    std::vector<std::vector<double>> rows;

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open " << filename << '\n';
        return rows;   // return empty on failure
    }


    std::string header;
    std::getline(file, header);

    std::string line;


    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;                        // skip blank lines
        }

        std::istringstream ss(line);         // wrap the line in a STREAM
        std::string field;
        std::vector<double> row;             // plain vector<double>

        std::getline(ss, field, ',');        // consume + DISCARD the date field

        while (std::getline(ss, field, ',')) {   // read from ss (the stream), not line
            double value = std::stod(field);     // stod, not strtod
            row.push_back(value);
        }

        rows.push_back(row);                  // don't forget to store the row!
    }


    return rows;
}

Matrix buildMatrix(const std::vector<std::vector<double>>& data) {
    // 1. rows = data.size(), cols = data[0].size()
    // 2. Matrix result(rows, cols, 0.0);
    // 3. nested loop: result(r, c) = data[r][c];
    // 4. return result;
    assert(!data.empty());

    size_t rows= data.size();
    size_t cols = data[0].size();

    Matrix result(rows,cols,0.0);

    for (size_t r =0 ; r <rows;  r++) {
        for (size_t c = 0 ; c<cols; c++) {
            result(r,c)= data[r][c];
        }
    }

    return result;

}

Matrix computeReturns(const Matrix& prices) {
    // result is (prices.numRows() - 1) x prices.numCols()
    // for t = 1 to prices.numRows():
    //     for j = 0 to prices.numCols():
    //         result(t-1, j) = (prices(t,j) - prices(t-1,j)) / prices(t-1,j);
    // return result

    Matrix result(prices.numRows()-1,prices.numCols(),0.0);

    for (size_t t= 1; t < prices.numRows(); t++ ) {
        for (size_t j = 0; j < prices.numCols();j++) {

            result(t-1,j)= ((prices(t,j)- prices(t-1,j))/ prices(t-1,j));

        }
    }

    return result;
}