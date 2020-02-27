#include "DirectedGraph.hh"
#include <iostream>

DirectedGraph::DirectedGraph(size_t N):numOfVertices{N}{

}
void DirectedGraph::addEdge(size_t vi1, size_t vi2, double weight){
    // This access method creates an entry on the map if it does not exist
    edges[vi1][vi2] = weight;
    edgesModifiers[vi1][vi2] = 0;
}
std::vector<DirectedGraph::EdgeInfo> DirectedGraph::getAllExitEdges(size_t vi) const{
    std::vector<DirectedGraph::EdgeInfo> answer;
    // The first map structure has the exit edge as the 
    // map key, so just get need to access al the elements 
    // of the map structure this key points to
    if( edges.count(vi) > 0 ){
        for(auto& edgeDetail:edges.at(vi)){
            answer.push_back({vi, edgeDetail.first, edgeDetail.second});
        }
    }
    return answer;
}
const DirectedGraph::Edges& DirectedGraph::getEdges() const{
    return edges;
}
const DirectedGraph::Edges& DirectedGraph::getEdgesModifiers() const{
    return edgesModifiers;
}
void DirectedGraph::addToEdgeModifier(size_t edgeIn, size_t edgeOut, double modifier){
    if((edgesModifiers.count(edgeIn) > 0) && (edgesModifiers[edgeIn].count(edgeOut))){
        edgesModifiers[edgeIn][edgeOut] += modifier;
    }
}
void DirectedGraph::clearEdgeModifiers(){
    for(auto& modifierFirstPair: edgesModifiers){
        auto& edgeIn = modifierFirstPair.first;
        for(auto& modifierSecondPair: modifierFirstPair.second){
            auto& edgeOut = modifierSecondPair.first;
            edgesModifiers[edgeIn][edgeOut] = 0;
        }
    }
}
size_t DirectedGraph::size() const{
    return numOfVertices;
}
void DirectedGraph::printOutGraph() const{
    for(auto& edgeStart: edges){
        auto& v1 = edgeStart.first;
        for(auto& edgeDetails: edgeStart.second){
            auto& v2 = edgeDetails.first;
            auto& weight = edgeDetails.second;
            std::cout << "[" << v1 << "->" << v2 << "]:" << weight << std::endl; 
        }
    }
}