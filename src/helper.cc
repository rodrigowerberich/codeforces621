#include "helper.hh"

void getCombinationIndexesAux(size_t start, size_t n, size_t k, std::set<size_t> combination, std::vector<std::set<size_t>>& combinations){

    if( combination.size() < k ){
        for(size_t i=start; i < n; i++){
            if( combination.count(i) == 0 ){
                combination.emplace(i);
                getCombinationIndexesAux(i, n, k, combination, combinations);
                combination.erase(i);
            }
        }
    }else{
        combinations.push_back(combination);
    }
}

std::vector<std::set<size_t>> getCombinationIndexes(size_t n, size_t k){
    std::vector<std::set<size_t>> combinations;
    for(size_t i=0; i < n; i++){
        std::set<size_t> combination;
        combination.emplace(i);
        getCombinationIndexesAux(i, n, k, combination, combinations);
    }
    return combinations;
}