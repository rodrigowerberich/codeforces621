#include <vector>
#include <iostream>
#include <iomanip>

#include "helper.hh"
#include "DirectedGraph.hh"
#include "Path.hh"
#include "PathHelper.hh"
#include "EdgeSelector.hh"
#include <chrono>

// Helper output function
std::ostream& operator <<(std::ostream& os, const std::map<size_t, std::map<size_t, std::vector<size_t>>>& edges);
// Helper output function
std::ostream& operator <<(std::ostream& os, const std::vector<EdgeSelectorEdge>& edgesToIncrease);

int main(int argc, char *argv[]){
    bool verbose = false;
    bool timeit = false;
    for(int i = 1; i < argc; i++){
        if(std::string("-v") == argv[i]){
            verbose = true;
        }
        if(std::string("-t") == argv[i]){
            timeit = true;
        }
    }

    // Read the number of edges and vertices
    // Ex: 4 7
    size_t numberOfVertices = 0;
    size_t numberOfEdges = 0;
    std::cin >> numberOfVertices >> numberOfEdges;
    // Create the graph with the given number of vertices
    DirectedGraph dg(numberOfVertices);
    // Read the next numberOfEdges lines that should have the following structure:
    // vi1 vi2 w (vertex start index) (vertex end index) (edge weight)
    // Ex: 
    // 1 2 2
    while(numberOfEdges--){
        size_t vIn = 0;
        size_t vOut = 0;
        double w = 0;
        std::cin >> vIn >> vOut >> w;
        dg.addEdge(vIn, vOut, w);
    }
    // Read the number of plans to generate
    size_t numberOfPlans = 0;
    std::cin >> numberOfPlans;
    // Read the value of each plan, a single double per line
    std::vector<double> plans;
    while(numberOfPlans--){
        double plan = 0;
        std::cin >> plan;
        plans.push_back(plan);
    }
    // Preparing to time the execution
    using namespace std::chrono;
    std::chrono::_V2::system_clock::time_point start;
    if(timeit){
        start = high_resolution_clock::now();
    }
    // Show how the graph is
    if(verbose){
        std::cout << "*************************\n";
        std::cout << "Graph edges and weights: \n";
        std::cout << "*************************\n";
        dg.printOutGraph();
        std::cout << "*************************\n";


    }
    // Get all possible paths from 1 to N
    auto paths = PathHelper::generateAllPossiblePaths(dg);
    // Get all edges that are a part of these paths from 1 to N
    auto edges = PathHelper::extractEdgesFromPaths(paths);
    for(auto& plan : plans){
        double budget = plan;
        if(verbose){
            std::cout << "*************************\n";
            std::cout << "Starting budget: " << budget << std::endl;
            std::cout << "*************************\n";
        }
        // Keep allocating resource while we have budget
        while(budget > 0){
            if(verbose){
                std::cout << "*************************\n";
                std::cout << "Costs\n";
                std::cout << "*************************\n";
                for(auto& path:paths){
                    std::cout << path << " cost: " << path.calculateCost() << std::endl;
                }
                std::cout << "*************************\n";
                std::cout << edges;
                std::cout << "*************************\n";

            }
            // Get all the path with the minimum value
            auto minPathIndexes = PathHelper::findMinPaths(paths);
            // Selected which edges of the paths with minimal value we must increase
            auto edgesToIncrease = EdgeSelector(minPathIndexes, edges, paths).selectEdgesToIncrease();
            if(verbose){
                std::cout << "*************************\n";
                std::cout << "Selected edges" << std::endl;
                std::cout << "*************************\n";
                std::cout << edgesToIncrease;
                std::cout << "*************************\n";

            }
            // Get the indexes for the paths that will be influenced and for the paths that won't
            auto pathsInfluencedIndex = PathHelper::getPathsInfluencedIndex(edgesToIncrease);
            auto pathsNotInfluencedIndex = PathHelper::getPathsNotInfluencedIndex(pathsInfluencedIndex, paths);
            // Get the paths that will not be influenced and calculate the path with minimal cost amongst them
            auto pathsNotInfluenced = PathHelper::getPathsNotInfluenced(pathsNotInfluencedIndex, paths);
            auto minNotInfluencedPathIndexes = PathHelper::findMinPaths(pathsNotInfluenced);

            double current_budget = 0;
            if(minNotInfluencedPathIndexes.empty()){
                // If there is not a path that won't be influenced
                // we can increase the choosen edges as much as possible 
                current_budget = budget;
            }else{
                // If there are paths that will not be influenced,
                // we can only increase the current minimum path
                // the difference between the minimum and the paths that will not be influenced minimun
                auto minimumCost =  paths[minPathIndexes[0]].calculateCost();
                auto nextMinimumCost =  pathsNotInfluenced[minNotInfluencedPathIndexes[0]].calculateCost();
                auto difference = nextMinimumCost - minimumCost;
                // If this difference is greater than the current budget, we increase only the budget
                current_budget = (difference > budget)?budget:difference;
            }
            // We increase each of the edges the available budget divided by the number of edges
            for(const auto& edge: edgesToIncrease){
                dg.addToEdgeModifier(edge.edgeIn, edge.edgeOut, current_budget/edgesToIncrease.size());
            }
            // Remove the used budget from the total budget
            budget -= current_budget;
            if(verbose){
                std::cout << "*************************\n";
                std::cout << "Costs\n";
                std::cout << "*************************\n";
                for(auto& path:paths){
                    std::cout << path << " cost: " << path.calculateCost() << std::endl;
                }
                std::cout << "Total budget left: " << budget << std::endl;
            }
        }
        // Find the minimal cost and display it!
        auto minPathIndexes = PathHelper::findMinPaths(paths);
        double minCost = paths[minPathIndexes[0]].calculateCost();
        std::cout << std::fixed << std::setprecision(10) << minCost << std::endl;
        dg.clearEdgeModifiers();
    }
    // Stopping the timer
    if(timeit){
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop-start);
        std::cout << "Took " << duration.count() << " microseconds\n";
    }
    return 0;
}

std::ostream& operator <<(std::ostream& os, const std::map<size_t, std::map<size_t, std::vector<size_t>>>& edges){   
    for(auto& edge:edges){
        auto& edge_1 = edge.first;
        for(auto& edgeInfo:edge.second){
            auto& edge_2 = edgeInfo.first;
            os << edge_1 << "->"<< edge_2 << ": {";
            for(auto& pathIndex: edgeInfo.second){
                os << pathIndex << ",";
            }
            os << "}\n";
        }
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const std::vector<EdgeSelectorEdge>& edgesToIncrease){   
    for(const auto& edge: edgesToIncrease){
        os << edge.edgeIn << "->" << edge.edgeOut << ": {";
        for(auto& path:edge.paths){
            os << path << ", ";
        }
        os << "}\n";
    }
    return os;
}