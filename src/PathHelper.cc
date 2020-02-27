#include <vector>
#include <cstddef>
#include <limits>
#include <cmath>

#include "PathHelper.hh"
#include "DirectedGraph.hh"
#include "Path.hh"
#include "EdgeSelector.hh"


namespace PathHelper{
// Helps the generate function to recursively generate all possible paths from 1 to N
void generateAllPossiblePathsAux(const DirectedGraph& dg, std::vector<Path>& paths, size_t startingEdge, Path pathSoFar){
    // Get all edges that come out of vertex passed
    auto startingPaths = dg.getAllExitEdges(startingEdge);
    for(auto& pathStart: startingPaths){
        // Make a copy of a path so that
        // this does not influece other possible branches
        auto path = pathSoFar;
        auto& v2 = pathStart.v2;
        path.addVertex(v2);
        if(path.isValid()){
            if(path.isComplete()){
                paths.push_back(path);
            }else{
                generateAllPossiblePathsAux(dg, paths, v2, path);
            }
        }
    }
}

std::vector<Path> generateAllPossiblePaths(const DirectedGraph& dg){
    std::vector<Path> paths;
    // Get all edges that come out of vertex 1
    auto startingPaths = dg.getAllExitEdges(1);
    // Each of these edges is a path starter
    for(auto& pathStart: startingPaths){
        // Start creating a path for the given edge
        auto path = Path(dg.getEdges(), dg.getEdgesModifiers(), dg.size());
        // We only need to add the second vertex, since the path starts with the vertex 1 added
        auto& v2 = pathStart.v2;
        path.addVertex(v2);
        // The path will be made invalid if the last vertex added already exists in the path
        if(path.isValid()){
            if(path.isComplete()){
                paths.push_back(path);
            }else{
                // If the path is not yet complete, keep expanding 
                // the path from the last added vertex
                generateAllPossiblePathsAux(dg, paths, v2, path);
            }
        }
    }
    return paths;
}

std::map<size_t, std::map<size_t, std::vector<size_t>>> extractEdgesFromPaths(std::vector<Path> paths){
    std::map<size_t, std::map<size_t, std::vector<size_t>>> relevantEdgesSorting;
    for(size_t i=0; i < paths.size(); i++){
        auto& path = paths[i];
        auto edges = path.getEdges();
        for(auto& edge:edges){
            relevantEdgesSorting[std::get<0>(edge)][std::get<1>(edge)].push_back(i);
        }
    }
    return relevantEdgesSorting;
}

std::vector<size_t> findMinPaths(std::vector<Path>& paths){
        auto min_value = std::numeric_limits<double>::max();
        constexpr double delta_min = 0.00000001;
        std::vector<size_t> minPathIndexes;
        for(size_t i = 0; i < paths.size(); i++){
            auto path = paths[i];
            auto path_cost = path.calculateCost();
            if(std::fabs(path_cost - min_value) < delta_min ){
                minPathIndexes.push_back(i);
            }else if( path_cost < min_value ){
                minPathIndexes.clear();
                min_value = path_cost;
                minPathIndexes.push_back(i);
            }
        }
        return minPathIndexes;
}

std::set<size_t> getPathsInfluencedIndex(std::vector<EdgeSelectorEdge>& edgesToIncrease){
    std::set<size_t> pathsInfluenced;
        for(const auto& edge: edgesToIncrease){  
            for(auto& path:edge.paths){
                pathsInfluenced.emplace(path);
            }
        }
        return pathsInfluenced;
}

std::set<size_t> getPathsNotInfluencedIndex(std::set<size_t>& pathsInfluencedIndex, std::vector<Path>& paths){
    std::set<size_t> pathsNotInfluenced;
    for(size_t i = 0; i < paths.size(); i++){
        if(pathsInfluencedIndex.count(i) == 0){
            pathsNotInfluenced.emplace(i);
        }
    }
    return pathsNotInfluenced;
}

std::vector<Path> getPathsNotInfluenced(std::set<size_t>& pathsNotInfluencedIndex, std::vector<Path>& paths){
    std::vector<Path> pathsNotInfluenced;
    for(auto& index: pathsNotInfluencedIndex){
        pathsNotInfluenced.push_back(paths[index]);
    }
    return pathsNotInfluenced;
}


};