#pragma once

#include <unordered_set>
#include <vector>
#include <tuple>
#include <iostream>

#include "DirectedGraph.hh"

// Represents a path from the vertice 1 to the endGoal passed

class Path{
private:
    const DirectedGraph::Edges& edges;
    const DirectedGraph::Edges& edgesModifiers;
    bool complete;
    bool valid;
    size_t endGoal;
    std::unordered_set<size_t> verticesFind;
    std::vector<size_t> verticesOrder;
public:
    // Gets the references to the maps of weights and weights modifiers that will be used to calculate the cost
    // Also already insert vertex 1
    Path(const DirectedGraph::Edges& edges, const DirectedGraph::Edges& edgesModifiers, size_t endGoal);
    // Tries to add a vertex, if it is already in the path, it is not added, and the function returns false
    bool addVertex(size_t vi);
    // Returns wheter the path is complete, from 1 to endGoal
    bool isComplete() const;
    // If an vertex failed to be added the path will become invalid
    bool isValid() const;
    // Returns a vector of tuples of two values. These value are in order the vi1 and vi2 of the edge
    std::vector<std::tuple<size_t,size_t>> getEdges() const;
    // Calculates the current cost of this map, according to the weights
    // and weight modifiers represented in the referenced maps
    double calculateCost() const;
    // Prints out the complete path
    void print() const;
    friend std::ostream& operator <<(std::ostream& os, const Path& path);
};

std::ostream& operator <<(std::ostream& os, const Path& path);