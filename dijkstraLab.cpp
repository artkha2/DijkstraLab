#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

// Graph is represented as an adjacency map
// Each key is a vertex, each value is a vector of (outgoing neighbor, edge weight) pairs
using Graph = unordered_map<int, vector<pair<int, int>>>;

int dijkstra(const Graph& graph, int start, int end){
    // Min-heap priority queue of (distance, node) pairs - lower weight has priority
    // greater<> comparator looks at first element of pair, then second
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> min_heap;

    // map to store minimum distances (so far) from start to each vertex
    // key: vertex, value: distance from start
    unordered_map<int, int> distances;

    // initialize distances to INF (or 0 in case of start), add them all to priority queue
    
    // iterate through entries in the graph map: first element is the vertex, disregard the second
    for (const auto& [v, _] : graph) {
        distances[v] = (v == start ? 0 : INF);
        min_heap.emplace(distances[v], v);
    }

    while(!min_heap.empty()){
        auto [current_dist, current_node] = min_heap.top();  // remove element with smallest distance
        min_heap.pop();

        // we have popped the node from the heap, so at this point we have found the shortest path to the current node
        // if it's the end, return right away
        if (current_node == end) {
            return current_dist == INF ? -1 : current_dist;
        }

        // avoid integer overflow. If the current_dist is still INF (unchanged from beginning), the vertex is unreachable -
        // no path has ever been found to it from start node. So, skip it and return -1 once the end is popped
        if (current_dist == INF) continue;

        // Iterate through outgoing neighbors of current node and the corresponding edges' weights
        for (const auto& [neighbor, weight] : graph.at(current_node)){
            int new_dist = current_dist + weight;
            if (new_dist < distances[neighbor]){
                distances[neighbor] = new_dist;  // update minimum distance from start
                min_heap.emplace(new_dist, neighbor);  // add this new dist of that vertex to heap
            }
        }
    }

    return -1; // should only reach this if user asked for an end node not in graph. Otherwise, will return from within while loop
}

int main() {
    // structure of each graph element: {node, {outgoing_neighbor1, weight}, {outgoing_neightbor2, weight}, ...}

    // given example
    Graph graph1 = {
        {1, {{2, 1}, {3, 1}, {5, 1}}},
        {2, {{4, 1}}},
        {3, {{5, 1}}},
        {4, {{3, 1}}},
        {5, {}}
    };

    // cyclic
    Graph graph2 = {
        {1, {{2, 1}, {3, 3}}},
        {2, {{3, 1}}},
        {3, {{4, 2}, {5, 4}}},
        {4, {{6, 1}}},
        {5, {{6, 3}}},
        {6, {{5, 2}}}
    };

    // disconnected
    Graph graph3 = {
        {1, {{2, 1}}},
        {2, {}},
        {3, {{4, 1}}},
        {4, {}}
    };

    cout << dijkstra(graph1, 1, 2) << endl; // Expected: 1
    cout << dijkstra(graph1, 1, 5) << endl; // Expected: 1
    cout << dijkstra(graph1, 2, 5) << endl; // Expected: 3
    cout << dijkstra(graph1, 5, 1) << endl; // Expected: -1
    cout << dijkstra(graph1, 2, 1) << endl; // Expected: -1

    cout << dijkstra(graph2, 1, 4) << endl;  // Expected: 4
    cout << dijkstra(graph2, 1, 5) << endl;  // Expected: 6
    cout << dijkstra(graph2, 3, 6) << endl;  // Expected: 3
    cout << dijkstra(graph2, 6, 4) << endl;  // Expected: -1

    cout << dijkstra(graph3, 1, 4) << endl;  // Expected: -1
    cout << dijkstra(graph3, 3, 4) << endl;  // Expected: 1
    cout << dijkstra(graph3, 4, 3) << endl;  // Expected: -1
    


    return 0;
}
