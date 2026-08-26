#include "Matrix.h"
#include "DataLoader.h"
#include <iostream>

int main() {
    std::vector<std::vector<double>> data = loadCSV("prices.csv");

    std::cout << "rows loaded: " << data.size() << '\n';



    Matrix prices= buildMatrix(data);

    Matrix returns = computeReturns(prices);

   std::cout << returns;


    return 0;
}