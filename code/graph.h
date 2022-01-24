//
// Created by vasco on 1/22/2022.
//

#ifndef CODE_GRAPH_H
#define CODE_GRAPH_H
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Data structure to store a graph edge
struct Edge {
    string line;
    int src, dest;
    double distance;
    bool changeZone, daytimeSchedule, operating;
};

struct Node {
    vector<Edge> adj; // The list of outgoing edges (to adjacent nodes)
    int pred;
    double dist, walkingDist, weight;
    int zonesVisited;
    bool visited;
    string line; // for algorithm purposes

    string name, code, zone;
    double latitude, longitude;
};

// A class to represent a graph object
class Graph {

    public:
        // a vector of vectors of Pairs to represent an adjacency list
        vector<Node> nodes;
        map<string, string> nameToCode;
        map<string, int> stationIndexes;

        Graph();
        bool closestPath(int src, int dest, int criteria, bool daytimeSchedule);
        bool bfs(int src, int dest, bool daytimeSchedule);
        bool dijkstra(int src, int dest, int criteria, bool daytimeSchedule, string initialLine = "walking-line");

        void printClosestPath(int dest);
        void printPossiblePath(int dest);
        void loadStations ();
        void loadLines ();
        void closeOpenLine(const string& line, bool closeOpen);
        void closeOpenStation(int index, bool closeOpen);
        void createWalking(int maxWalking);
        void rmvWalking();

        float ticketPrice(int numZones);
        float ticketTime(int numZones);

        vector<pair<int, double>> closestStation(double latitude, double longitude, int num) const;
        double stationDistance(int src, int dest) const;
        double stationDistance(int station, double lat2, double lon2) const;
        bool changeZone(int src, int dest);
};

#endif //CODE_GRAPH_H