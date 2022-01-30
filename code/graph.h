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

        /**
         * @brief Initializer
         */
        Graph();
        /**
        * @brief  closestPath between 2 places
        * @return verify the existence of path between 2 places
        */
        bool closestPath(int src, int dest, int criteria, bool daytimeSchedule);

        /**
        * @brief search algorhitm
        * @return if exists an element in the graph
        */
        bool bfs(int src, int dest, bool daytimeSchedule);

        /**
        * @brief algorhitm that calculates the shortest paht by the cost of edges
        * @return if exists that path
        */
        bool dijkstra(int src, int dest, int criteria, bool daytimeSchedule, string initialLine = "walking-line");

        /**
        * @brief show the closest path
        */
        void printClosestPath(int dest);
        /**
        * @brief show possible paths once, we have a destination
        */
        void printPossiblePath(int dest);

        /**
        * @brief process to load the station of stcp and read the files
        */
        void loadStations ();
        /**
        * @brief process to load the lines of stcp and read the files
        */
        void loadLines ();

        /**
        * @brief close or open one line
        */
        void closeOpenLine(const string& line, bool closeOpen);

        /**
        * @brief close or open one station
        */
        void closeOpenStation(int index, bool closeOpen);
        /**
        * @brief create the possible path to walk
        */
        void createWalking(int maxWalking);
        /**
        * @brief remove the path to walk
        */
        void rmvWalking();

        /**
        * @brief price of ticket
        * @return the price of ticket
        */

        float ticketPrice(int numZones);
        /**
        * @brief time of ticket's validation
        * @return the validate time of ticket
        */
        float ticketTime(int numZones);

         /**
        * @brief near by stations
        * @return the stations near by
        */
        vector<pair<int, double>> closestStation(double latitude, double longitude, int num) const;
        /**
        * @brief Distance of the station giving coordenades
        * @return the distance to the station
        */
        double stationDistance(int src, int dest) const;

        /**
        * @brief Distance of the station giving a station
        * @return the distance to the station
        */
        double stationDistance(int station, double lat2, double lon2) const;
        /**
        * @brief To see if the client has to change to another zone
        * @return the possiblity of change the zone
        */
        bool changeZone(int src, int dest);
};

#endif //CODE_GRAPH_H