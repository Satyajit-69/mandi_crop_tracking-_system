#ifndef CROPMANAGER_H
#define CROPMANAGER_H

#include "Crop.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

// Thrown when standard input ends (Ctrl+Z / Ctrl+D or piped input runs out).
class InputEnded : public std::runtime_error
{
public:
    InputEnded() : std::runtime_error("Input ended!") {}
};

// Line-based input helpers. They return false if the line is not a valid
// number (or has extra characters) and throw InputEnded at end of input.
bool readInt(int &value);
bool readDouble(double &value);

// Function declarations
void loadPreviousData(std::vector<Crop *> &crops);
void createCrop(std::vector<Crop *> &crops);
void readAll(std::vector<Crop *> &crops);
void searchCrop(std::vector<Crop *> &crops);
void updateCrop(std::vector<Crop *> &crops);
void deleteCrop(std::vector<Crop *> &crops);
void summaryReport(std::vector<Crop *> &crops);
void compareCrops(std::vector<Crop *> &crops);
void saveAll(const std::vector<Crop *> &crops);
void clearCrops(std::vector<Crop *> &crops);

// Function template
template <typename T>
T *bestPrice(const std::vector<T *> &crops)
{
    if (crops.empty())
        throw std::runtime_error("No crops available!");

    return *std::max_element(crops.begin(), crops.end(), [](T *a, T *b)
                             { return a->getPrice() < b->getPrice(); });
}

#endif
