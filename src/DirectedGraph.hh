#pragma once
#include <map>
#include <vector>

// Representes a directed graph with N vertices
// The edges of this directed graph have predefined weights
// The edges also have an edge modifier weight that can be reset
// Vertices are not directly represented, they can be referenced
// as vertices 1, ..., N
// The edges are represented as (vi1, vi2):w, where
// vi1 is the vertex from where the edge begins,
// vi2 is the vertex in which the edge ends,
// w is the weight of that edge
class DirectedGraph{
public:
    using Edges = std::map<size_t, std::map<size_t, double>>;
private:
    size_t numOfVertices;
    // Stores which edges exist and their corresponding weights
    Edges edges;
    // Stores which edges exist and their corresponding weights modifiers, these modifiers should be added 
    // to the normal weights
    Edges edgesModifiers;
public:
    struct EdgeInfo{
        size_t v1;
        size_t v2;
        double w;
    };
    // Starts a graph with N vertices
    DirectedGraph(size_t N);
    // Creates and edge (vi1, vi2):weight, where vi1 and vi2 <= N, > 0 and weight is not negative
    void addEdge(size_t vi1, size_t vi2, double weight);
    // Returns all the edges that exit from a given vertex, with vi <= N, > 0
    std::vector<DirectedGraph::EdgeInfo> getAllExitEdges(size_t vi) const;
    // Gets a const reference to a map of maps of doubles, this way the weight of 
    // an edge between vertices v1 and v2 <= N, > 0 can be accessed by getEdges()[v1][v2]
    const DirectedGraph::Edges& getEdges() const;
    // Gets a const reference to a map of maps of doubles, this way the weight modifier of 
    // an edge between vertices v1 and v2 <= N, > 0 can be accessed by getEdgesModifiers()[v1][v2]
    const DirectedGraph::Edges& getEdgesModifiers() const;
    // Adds a value to the current value stored in the edge modifier corresponding to
    // the edge between vertices vi1, vi2 <= N, > 0
    void addToEdgeModifier(size_t vi1, size_t vi2, double modifier);
    // Sets all the edge modifiers to 0
    void clearEdgeModifiers();
    // Returns the number of vertices in the graph
    size_t size() const;
    // Prints to std::cout a representation of the graph
    void printOutGraph() const;
};