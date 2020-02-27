#pragma once

#include <iostream>
#include <set>
#include <vector>

template <typename T>
void printOutArray(const T& array){
    for(const auto& element:array){
        std::cout << element << " ";
    }
    std::cout << "\n";
}

std::vector<std::set<size_t>> getCombinationIndexes(size_t n, size_t k);