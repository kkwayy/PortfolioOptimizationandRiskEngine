//
// Created by ethan on 15-Aug-26.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>

int main() {
    std::map<std::string, std::vector<double>> priceSeries;
    priceSeries["AAPL"] = {150.0, 152.5, 151.0};
    priceSeries["MSFT"] = {300.0, 305.5, 302.0};

    std::cout << priceSeries.size() << '\n';

    if (priceSeries.count("AAPL")) {
        std::cout << priceSeries.at("AAPL")[0] << '\n';   // .at() to read, [0] for first price
    }

    for (const auto& [ticker, prices] : priceSeries) {
        std::cout << ticker << " -> ";
        for (double p : prices) {
            std::cout << p << ' ';
        }
        std::cout << '\n';
    }
}