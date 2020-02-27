#pragma once

#include <map>
#include <vector>
#include <set>
#include <cstddef>

class DirectedGraph;
class Path;
class EdgeSelectorEdge;


namespace PathHelper{
// Generate all possible paths from vertex 1 to vertex N from the directed graph dg
std::vector<Path> generateAllPossiblePaths(const DirectedGraph& dg);
// Generate all the edges that belong to the given paths
std::map<size_t, std::map<size_t, std::vector<size_t>>> extractEdgesFromPaths(std::vector<Path> paths);
// Get a vector with all the paths with the minimum cost. A vector is returned, because there can be more
// than one minimum path
std::vector<size_t> findMinPaths(std::vector<Path>& paths);
// Get all the index of all paths that are influenced by the edges selected
std::set<size_t> getPathsInfluencedIndex(std::vector<EdgeSelectorEdge>& edgesToIncrease);
// Get all the index of all paths that are not influenced by the edges selected
std::set<size_t> getPathsNotInfluencedIndex(std::set<size_t>& pathsInfluencedIndex, std::vector<Path>& paths);
// Get all paths not influenced from their index
std::vector<Path> getPathsNotInfluenced(std::set<size_t>& pathsNotInfluencedIndex, std::vector<Path>& paths);
};