/*********************************************
* file:	~\tnd004\lab\lab4a\digraph.cpp       *
* remark: mplementation of directed graphs   *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

#include "digraph.h"
#include "queue.h"

const int INFINITY = 9999;

// -- CONSTRUCTORS

Digraph::Digraph(int n)
{
    assert(n >= 1);
    array = new List[n + 1];
    dist  = new int [n + 1];
    path  = new int [n + 1];
    done  = new bool[n + 1];
    size  = n;
}

// -- DESTRUCTOR

Digraph::~Digraph()
{
    delete[] array;
    delete[] dist;
    delete[] path;
    delete[] done;
}

// -- MEMBER FUNCTIONS

// insert directed edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Digraph::insertEdge(int u, int v, int w)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].insert(v, w);
}

// remove directed edge (u, v)
void Digraph::removeEdge(int u, int v)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].remove(v);
}

// unweighted single source shortest paths
void Digraph::uwsssp(int s)
{
    Queue<int> q;

    if (s < 1 || s > size)
    {
         cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
         return;
    }

    // Initialize dist and path
    for (int i = 0; i<=size; i++) {
        dist[i] = INFINITY;
        path[i] = 0;
    }
    dist[s] = 0;

    q.enqueue(s);

    while(!q.isEmpty()) {
        int v = q.getFront();
        q.dequeue();

        // Loop over all outgoing edges from vertex v;
        Node* n = array[v].getFirst();
        while (n) {
            if (dist[n->vertex] == INFINITY) {
                // Vertex has not been visited yet
                dist[n->vertex] = dist[v] + 1;
                path[n->vertex] = v;
                q.enqueue(n->vertex);
            }
            n = array[v].getNext();
        }
    }
}


// return the undone vertex with the smallest distance,
// return -1 if all nodes done
int Digraph::undone_smallest_distance() const
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

// positive weighted single source shortest pats
void Digraph::pwsssp(int s)
{
    if (s < 1 || s > size)
    {
         cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
         return;
    }

    // Initialize dist, path and done
    for (int i = 0; i<=size; i++) {
        dist[i] = INFINITY;
        path[i] = 0;
        done[i] = false;
    }
    dist[s] = 0;
    done[s] = true;

    int v = s;

    // Loop until all vertices has been visited and all edges traversed
    while (true) {
        Node *n = array[v].getFirst();

        // Loop over all outgoing edges
        while (n) {
            if (!done[n->vertex] && dist[n->vertex] > (dist[v] + n->weight)) {
                dist[n->vertex] = dist[v] + n->weight;
                path[n->vertex] = v;
            }
            n = array[v].getNext();
        }

        // Find the undone vertex with smallest distance from start
        v = undone_smallest_distance();

        if (v == -1) {
            // all vertices done, end algorithm!
            break;
        }
        done[v] = true;
    }
}

// print graph
void Digraph::printGraph() const
{
    cout << "------------------------------------------------------------------" << endl;
    cout << "vertex  adjacency list                  " << endl;
    cout << "------------------------------------------------------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " : ";
        array[v].print();
    }

    cout << "------------------------------------------------------------------" << endl;
}

// print shortest path tree for s
void Digraph::printTree() const
{
    cout << "----------------------" << endl;
    cout << "vertex    dist    path" << endl;
    cout << "----------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " :" << setw(8) << dist[v] << setw(8) << path[v] << endl;
    }

    cout << "----------------------" << endl;
}

// Recursive function to print path to a vertex
void Digraph::printpath_help(ostream & out, int v) const
{
    if (dist[v] == 0) {
        out << v << " ";
        return;
    }
    printpath_help(out, path[v]);

    out << v << " ";
}

// print shortest path from s to t
void Digraph::printPath(int t) const
{
    if (t < 1 || t > size)
    {
         cout << "\nERROR: expected target t in range 1.." << size << " !" << endl;
         return;
    }

    printpath_help(cout, t);
    cout << "(" << dist[t] << ")" << endl;

}
