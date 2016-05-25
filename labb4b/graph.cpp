/*********************************************
* file:	~\tnd004\lab\lab4b\graph.h           *
* remark:implementation of undirected graphs *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

using namespace std;

#include "graph.h"
#include "edge.h"
#include "heap.h"
#include "dsets.h"

const int INFINITY = 9999;

// -- CONSTRUCTORS

Graph::Graph(int n)
{
    assert(n >= 1);
    array = new List[n + 1];
    done = new bool[n + 1];
    dist = new int[n + 1];
    path = new int[n + 1];
    size  = n;
}

// -- DESTRUCTOR

Graph::~Graph()
{
    delete[] array;
    delete[] done;
    delete[] dist;
    delete[] path;
}

// -- MEMBER FUNCTIONS

// insert undirected edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Graph::insertEdge(int u, int v, int w)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].insert(v, w);
    array[v].insert(u, w);
}

// remove undirected edge (u, v)
void Graph::removeEdge(int u, int v)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].remove(v);
    array[v].remove(u);
}


// return the undone vertex with the smallest distance,
// return -1 if all nodes done
int Graph::undone_smallest_distance() const
{
    int smallest_distance = INFINITY;
    int v = -1;

    for (int i = 0; i<=size; i++) {
        if (!done[i] && dist[i] < smallest_distance) {
            v = i;
            smallest_distance = dist[i];
        }
    }
    return v;
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const
{
    int start = 2;

    for (int i = 0; i <= size; i++) {
        dist[i] = INFINITY;
        path[i] = 0;
        done[i] = false;
    }

    dist[start] = 0;
    done[start] = true;

    int v = start;

    while (true) {

        // loop over all outgoing edges
        Node* n = array[v].getFirst();
        while(n) {
            if (!done[n->vertex] && (dist[n->vertex] > n->weight)) {
                dist[n->vertex] = n->weight;
                path[n->vertex] = v;
            }
            n = array[v].getNext();
        }

        v = undone_smallest_distance();
        if (v == -1) {
            break;
        }

        done[v] = true;
    }

    // Print the results
    int weight = 0;
    for (int i = 1; i <= size; i++) {
        if (dist[i]) {
            // edge is in MST
            cout << "( " << path[i] << ", " << i << ", " << dist[i] << ")" << endl;
            weight += dist[i];
        }
    }
    cout << "Total weight = " << weight << endl;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const
{

    Heap<Edge> h;
    DSets d(size);
    vector<Edge> selected_edges;
    int counter = 0;

    d.init();

    // Insert all edges in the heap
    for (int v = 1; v <= size; v++) {
        Node *n = array[v].getFirst();
        while (n) {
            if (v < n->vertex) {
                h.insert(Edge(v, n->vertex, n->weight));
            }
            n = array[v].getNext();
        }
    }

    while (counter < size-1) {
        Edge e = h.deleteMin();

        if (d.find(e.tail) != d.find(e.head)) {
            d.join(d.find(e.tail), d.find(e.head));
            counter++;

            selected_edges.push_back(e);
        }
    }

    int total_weight = 0;

    for (auto edge : selected_edges) {
        cout << edge << endl;
        total_weight += edge.weight;
    }
    cout << "Total weight = " << total_weight << endl;

}

// print graph
void Graph::printGraph() const
{
    cout << "------------------------------------------------------------------" << endl;
    cout << "vertex  adjacency list                                            " << endl;
    cout << "------------------------------------------------------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " : ";
        array[v].print();
    }

    cout << "------------------------------------------------------------------" << endl;
}
