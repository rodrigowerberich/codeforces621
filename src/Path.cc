#include "Path.hh"
#include <iostream>

Path::Path(const DirectedGraph::Edges& edges, const DirectedGraph::Edges& edgesModifiers, size_t endGoal):
    edges{edges}, 
    edgesModifiers{edgesModifiers}, 
    complete{false},
    valid{true},
    endGoal{endGoal}{
    verticesFind.emplace(1);
    verticesOrder.push_back(1);
}
bool Path::addVertex(size_t vi){
    if(verticesFind.count(vi) > 0){
        // Vertex is already in path, avoid circular references 
        valid = false;
        return false;
    }else if(isComplete()){
        // Path is already completed no need to add anything to the path
        return false;
    }else{
        // Add unordered_set to make it easy to find if it is in the path
        verticesFind.emplace(vi);
        // Add to vector to preserve the path order
        verticesOrder.push_back(vi);
        if(vi == endGoal){
            complete = true;
        }
        return true;
    }
}
bool Path::isComplete() const{
    return complete;
} 

bool Path::isValid() const{
    return valid;
} 

std::vector<std::tuple<size_t,size_t>> Path::getEdges() const{
    std::vector<std::tuple<size_t,size_t>> edges;
    for(size_t i=0; i < verticesOrder.size()-1; i++){
        edges.push_back(std::make_tuple(verticesOrder[i], verticesOrder[i+1]));
    }
    return edges;
}

double Path::calculateCost() const{
    double cost = 0;
    for(size_t i=0; i < verticesOrder.size()-1; i++){
        cost += edges.at(verticesOrder[i]).at(verticesOrder[i+1])+edgesModifiers.at(verticesOrder[i]).at(verticesOrder[i+1]);
    }
    return cost;
}

void Path::print() const{
    for(size_t i=0; i < verticesOrder.size(); i++){
        std::cout << verticesOrder[i];
        if( i < verticesOrder.size()-1){
            std::cout << "->";
        }
    }
    std::cout << "\n";
}

std::ostream& operator <<(std::ostream& os, const Path& path){   
    for(size_t i=0; i < path.verticesOrder.size(); i++){
        os << path.verticesOrder[i];
        if( i < path.verticesOrder.size()-1){
            os << "->";
        }
    }
    return os;
}