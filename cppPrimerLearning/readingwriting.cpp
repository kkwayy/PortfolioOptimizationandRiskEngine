//
// Created by ethan on 14-Aug-26.
//


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    // 1. open data.csv for reading
    // 2. if it didn't open, print an error to std::cerr and return 1
    // 3. read line by line with getline, print each line
    // 4. return 0
    // (no need to close — why?)

    std::ifstream file("data.csv");
    if (!file) {
        std::cerr<<"could not open data.csv \n";

        return 1;

    }

    std::string line;

    while (std::getline(file,line)) {
       std::istringstream ss(line);
        std::string field;

        while (std::getline(ss,field,',')){
        std::cout<<field<<'\n';
        }

    }
    return 0;

}