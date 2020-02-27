#pragma once

#include <cstddef>
#include <vector>
#include <set>
#include <map>

class Path;

// A representation of the edge
// It stores the paths that it belongs to
// It also stores the edge value regarding its 
// relevance to the contribution of the graphs minimum cost 
struct EdgeSelectorEdge{
public:
    size_t edgeIn;
    size_t edgeOut;
    std::vector<size_t> paths;
    double value;
    // Checks if this edge shares any importantPaths with another path
    bool containSimilarity(const EdgeSelectorEdge& other, const std::set<size_t>& importantPaths);
};

// Selects which edges will be selected to be increased 
class EdgeSelector{
public:
    // The edge selector needs 
    // the paths that the edges must belong to: pathsVector
    // a mapping that tell to which paths a given edge belong to: edgeMapping
    // and the total number of existing paths: numberOfExistingPaths
    EdgeSelector(std::vector<size_t> pathsVector, std::map<size_t, std::map<size_t, std::vector<size_t>>> edgeMapping, std::vector<Path> existingPaths);
    // Determinar the edges that must be increased
    std::vector<EdgeSelectorEdge> selectEdgesToIncrease();
private:
    // The existing paths
    std::vector<Path> existingPaths;
    // The index of the paths with minimum cost
    std::vector<size_t> minPathIndexes;
    // Represents which paths a given edge belongs to
    std::map<size_t, std::map<size_t, std::vector<size_t>>> edgeMapping;
    // The total number of existing paths
    double numberOfExistingPaths;
    // The paths that the edges must belong to
    std::set<size_t> paths;
    // The paths that the edges are currently belonging
    std::set<size_t> currentlyRepresentedPaths;
    // The edges that have already been selected
    std::vector<EdgeSelectorEdge> selectedEdges;
    // Returns the value of the relevance a edge has according to the paths that this edge belongs to
    // The calculation is relevantPath/totalOfRelevantPaths + extraPath/numberOfExistingPaths
    double calculateEdgeValue(std::vector<size_t> edgePaths);
    // Process a new edge to evaluate if it is a better contributor or not to
    // than the already existing edges
    void process(std::tuple<size_t, size_t> edge);
    // Return the selected edges
    std::vector<EdgeSelectorEdge> getSelectedEdges();
};