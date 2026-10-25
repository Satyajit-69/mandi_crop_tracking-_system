
#ifndef CROPMANAGER_H
#define CROPMANAGER_H

#include "Crop.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Function declarations
void loadPreviousData(vector<Crop*>& crops);
void createCrop(vector<Crop*>& crops);
void readAll(vector<Crop*>& crops);
void searchCrop(vector<Crop*>& crops);
void updateCrop(vector<Crop*>& crops);
void deleteCrop(vector<Crop*>& crops);
void summaryReport(vector<Crop*>& crops);
void compareCrops(vector<Crop*>& crops);
void saveAll(const vector<Crop*>& crops);
void clearCrops(vector<Crop*>& crops);

// Function template
template <typename T>
T* bestPrice(const vector<T*>& crops)
{
    if (crops.empty())
        throw runtime_error("No crops available!");

    return *max_element(crops.begin(), crops.end(),[](T* a, T* b){
            return a->getPrice() < b->getPrice();
        }
    );
}

#endif
