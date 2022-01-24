#include <iostream>
#include "graph.h"
#include "Menu.h"

using namespace std;


int main() {

    /*
    Graph graph;
    graph.loadStations();
    graph.loadLines();

    string origin = "FRC", DEST = "VAL";
    cout << graph.stationIndexes[origin] << " -> " << graph.stationIndexes[DEST] << endl;
    if (graph.closestPath(graph.stationIndexes[origin], graph.stationIndexes[DEST], 1))
        graph.printClosestPath(graph.stationIndexes[DEST]);
    */

    Menu menu;
    bool run = true;

    while (run) {
        run = menu.nextState();
    }



    return 0;
}
