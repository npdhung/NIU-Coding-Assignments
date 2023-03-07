//***************************************************************************
//
//  Hung Nguyen
//  Z1924897
//  CSCI340 - 0002
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************
#include "graph.h"

bool* visited;
std::vector<int> visitingOrder;
std::vector<std::list<int>> traversingEdges;
int currentOrder;

/**
 * Traverses a graph in the depth-first traversal/search algorithm
 *
 * Traverses a graph in the depth-first traversal/search algorithm starting at the vertex with
 * the index value of `v`.
 *
 * @param v starting vertex
 *
 * *********************************************************************************/
void Graph::Depthfirst(int v)
{
    visited[v] = true;
    visitingOrder.push_back(v);
    ++currentOrder;
    // for each vertex adjacent to v
    for (auto it = adj_list[v].begin(); it != adj_list[v].end(); ++it) {
        int w = *it;
        if (!visited[w]) {
            traversingEdges[currentOrder-1].push_back(v);
            traversingEdges[currentOrder-1].push_back(w);
            Depthfirst(w);
        }
    }
    
}

/**
 * Constructor for graph
 *
 * Reads data in from the input file of the graph given in adjacency matrix representation and builds the
 * graph with a adjacency list representation. This method sets the value of size, builds the vectors
 * `labels` and `adj_list`.
 *
 * @param filename input file of the graph
 *
 * *********************************************************************************/
Graph::Graph(const char* filename)
{
    std::ifstream is;
    is.open(filename);
    if (is.fail()) {
        std::cerr << "Input file opening failed." << std::endl;
        exit(1);
    }
    is >> size;
    
    labels.resize(size);
    adj_list.resize(size);
    for (int i = 0; i < size; ++i) {
        is >> labels[i];
    }
    char label;
    int edge;
    for (int i = 0; i < size; ++i) {
        is >> label;
        for (int j = 0; j < size; ++j) {
            is >> edge;
            // push j to the list
            if (edge == 1) adj_list[i].push_back(j);
        }
    }
}

/**
 * Desstructor for hash table
 *
 * *********************************************************************************/
Graph::~Graph()
{
    // clear two vectors
    labels.clear();
    for (int i = 0; i < size; ++i) {
        adj_list[i].clear();
    }
    adj_list.clear();
}

/**
 * Get the size of the graph
 *
 * @return the size
 *
 * *********************************************************************************/
int Graph::Getsize() const
{ 
    return size;
}

/**
 * Traverses the graph
 *
 *  This public function is used to traverse a graph and invokes the above `Depthfirst` method.
 *  It displays traverse result: the list of vertices in the order of their visit and the list of edges
 *  showing the path(s) of the traversal. At beginning, it initializes the global variable(s) and
 *  object(s) used in `Depthfirst`.
 *
 * *********************************************************************************/
void Graph::Traverse()
{
    currentOrder = 0;
    traversingEdges.resize(size);
    
    visited = new bool[size];
    for (int i = 0; i < size; ++i) visited[i] = false;
    
    std::cout << std::endl;
    std::cout << "------- traverse of graph ------" << std::endl;
        
    // for each vertex that is not visited, do a Depth first traversal
    for (int i = 0; i < size; ++i)
        if (!visited[i]) Depthfirst(i);
    
    for (int const &i : visitingOrder) std::cout << labels[i] << " ";
    std::cout << std::endl;
    for (int i = 0; i < size; ++i)
        if (!traversingEdges[i].empty())
            std::cout << "(" << labels[traversingEdges[i].front()] << ", " << labels[traversingEdges[i].back()] << ") ";
    
    std::cout << std::endl;
    std::cout << "--------- end of traverse -------" << std::endl;
    std::cout << std::endl;

    delete [] visited;
}

/**
 * Prints the adjacency list of the graph
 *
 * *********************************************************************************/
void Graph::Print() const
{
    std::cout << std::endl;
    std::cout << "Number of vertices in the graph: " << size << std::endl;
    std::cout << std::endl;
    std::cout << "-------- graph -------" << std::endl;
    for (int i = 0; i < size; ++i) {
        if (!adj_list[i].empty()) {
            std::cout << labels[i] << ": ";
            for (int const &j : adj_list[i]) std::cout << labels[j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "------- end of graph ------" << std::endl;
}
