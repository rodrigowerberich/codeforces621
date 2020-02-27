#include "EdgeSelector.hh"
#include "Path.hh"

bool EdgeSelectorEdge::containSimilarity(const EdgeSelectorEdge& other, const std::set<size_t>& importantPaths){
    // If any of the paths from the other edge is important and
    // has a match with the paths from this edge, then they are similar
    for(const auto& otherElement:other.paths ){
        if(importantPaths.count(otherElement) > 0){
            for(const auto& element: paths){
                if(element == otherElement){
                    return true;
                }
            }
        }
    }
    return false;
}

EdgeSelector::EdgeSelector(std::vector<size_t> pathsVector, std::map<size_t, std::map<size_t, std::vector<size_t>>> edgeMapping, std::vector<Path> existingPaths): 
    existingPaths{existingPaths},
    minPathIndexes{pathsVector},
    edgeMapping{edgeMapping}, 
    numberOfExistingPaths{static_cast<double>(existingPaths.size())}{
    paths = std::set<size_t>(std::begin(pathsVector), std::end(pathsVector));
}

double EdgeSelector::calculateEdgeValue(std::vector<size_t> edgePaths){
    double relevantPaths = 0;
    double extraPaths = 0;
    // Counting the number of relevant and extra paths
    // if a path is not relevant, it is extra
    for(auto& path: edgePaths){
        if(paths.count(path) > 0){
            relevantPaths += 1;
        } else {
            extraPaths += 1;
        }
    }
    // If there are no relevant paths, we discard the extra paths contributions
    if( relevantPaths == 0 ){
        return 0;
    }else{
        return relevantPaths/paths.size() + extraPaths/numberOfExistingPaths;
    }
}

void EdgeSelector::process(std::tuple<size_t, size_t> edge){
    auto& edge0 = std::get<0>(edge);
    auto& edge1 = std::get<1>(edge);
    auto newEdge = EdgeSelectorEdge{edge0, edge1, edgeMapping[edge0][edge1], calculateEdgeValue(edgeMapping[edge0][edge1])};
    bool add = false;
    if(selectedEdges.empty()){
        // If we have not yet selected any edge,
        // mark the newEdge to be added if it belongs to any relevant path
        for(const auto& path: newEdge.paths){
            if(paths.count(path) > 0){
                add = true;
                break;
            }
        }
    }else{
        // If there is already selected edges,
        // we compare the value of this edge to the existing ones
        // If the new edge has a greater value than some existing edge
        // and they have any similarities, then mark that edge for removal,
        // and mark the new edge to be added
        // All edges with similarities must be removed!!
        std::vector<size_t> removeList;
        for(size_t i=0; i < selectedEdges.size(); i++){
            auto& selectedEdge = selectedEdges[i];
            if(newEdge.value > selectedEdge.value){
                if(selectedEdge.containSimilarity(newEdge, paths)){
                    removeList.push_back(i);
                    add = true;
                }
            }
        }
        if(!add){
            // If the edge was not marked to be added
            // we still need to check if this edge belongs to any
            // relevant path that is still not represented
            // If it is, the new edge is marked to be added
            for(const auto& path: newEdge.paths){
                if( (paths.count(path) > 0) && (currentlyRepresentedPaths.count(path) == 0) ){
                    add = true;
                    break;
                }
            }   
        }else{
            // Remove all the edges marked to be removed
            for(size_t index: removeList){
                selectedEdges.erase(std::begin(selectedEdges)+index);
            }
        }
    }
    if(add){
        // Add the new edge to the selected edges
        // and mark the relevant paths that this edge belongs to
        selectedEdges.push_back(newEdge);
        for(const auto& path: newEdge.paths){
            if( (paths.count(path) > 0) && (currentlyRepresentedPaths.count(path) == 0) ){
                currentlyRepresentedPaths.emplace(path);
            }
        } 
    }
}
std::vector<EdgeSelectorEdge> EdgeSelector::getSelectedEdges(){
    return selectedEdges;
}

std::vector<EdgeSelectorEdge> EdgeSelector::selectEdgesToIncrease(){
    // std::cout << "---------------------------" << std::endl;
        for(auto& index: minPathIndexes){
            // std::cout << index << std::endl;
            // paths[index].print();
            auto path_edges = existingPaths[index].getEdges();
            for(auto& path_edge:path_edges){
               process(path_edge);
            }
        }
        auto selectedEdges = getSelectedEdges();
        // std::cout << "\nSelected edges ---------------------------" << std::endl;
        // for(const auto& edge: selectedEdges){
        //     std::cout << edge.edgeIn << "->" << edge.edgeOut << ": {";
        //     for(auto& path:edge.paths){
        //         std::cout << path << ", ";
        //     }
        //     std::cout << "}\n";
        // }
        return selectedEdges;
}
