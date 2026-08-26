#ifndef DATALOADER_H
#define DATALOADER_H
#include "Matrix.h"

#include <vector>
#include <string>

std::vector<std::vector<double>> loadCSV(const std::string& filename);

Matrix buildMatrix(const std::vector<std::vector<double>>& data);

Matrix computeReturns(const Matrix& prices);

#endif