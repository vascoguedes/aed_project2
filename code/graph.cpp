//
// Created by vasco on 1/22/2022.
//

#include <fstream>
#include "graph.h"
#include <cmath>
#include <iostream>
#include <dirent.h>
#include <sstream>
#include <list>
#include "minHeap.h"
#include "auxiliar.h"
#include "set"

Graph::Graph() {

}

void Graph::loadStations() {

    string code, name, zone, aux;
    double latitude, longitude;
    int index = 0;

    ifstream estacoes("stations.csv");
    getline(estacoes, code);

    while (getline(estacoes, code, ',')) {
        getline(estacoes, name, ',');
        getline(estacoes, zone, ',');

        getline(estacoes, aux, ',');
        latitude = stod(aux);
        getline(estacoes, aux);
        longitude = stod(aux);

        Node node;
        node.code = code;
        node.name = name;
        node.zone = zone;
        node.latitude = latitude;
        node.longitude = longitude;

        nodes.push_back(node);
        stationIndexes[code] = index;
        index++;
    }

    nodes.resize(nodes.size());
}

void Graph::loadLines() {
    DIR *dr;
    struct dirent *en;
    dr = opendir("./lines"); //open all directory

    string line, aux;
    bool daytimeSchedule;
    int num, cod1, cod2;

    en = readdir(dr);
    en = readdir(dr);

    if (dr) {
        while ((en = readdir(dr)) != nullptr) {

            if (en->d_name != "") {

                stringstream ss(en->d_name);

                getline(ss, line, '_');
                getline(ss, line, '_');

                daytimeSchedule = line.find('M') == string::npos;

                aux = en->d_name;
                aux = "./lines/" + aux;
                ifstream estacoes(aux);

                getline(estacoes, aux);
                num = stoi(aux);
                getline(estacoes, aux);
                cod1 = stationIndexes[aux];

                for (int i = 1; i < num; ++i) {
                    getline(estacoes, aux);
                    cod2 = stationIndexes[aux];

                    Edge edge;
                    edge.line = line;
                    edge.src = cod1;
                    edge.dest = cod2;
                    edge.distance = stationDistance(cod1, cod2);
                    edge.changeZone = changeZone(cod1, cod2);
                    edge.daytimeSchedule = daytimeSchedule;
                    edge.operating = true;

                    nodes[cod1].adj.push_back(edge);
                    cod1 = cod2;
                }

            }
        }
        closedir(dr); //close all directory
    }
}


double Graph::stationDistance(int src, int dest) const {


    double lat1 = nodes[src].latitude;
    double lon1 = nodes[src].longitude;
    double lat2 = nodes[dest].latitude;
    double lon2 = nodes[dest].longitude;

    // distance between latitudes
    // and longitudes
    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));

    return rad * c;
}

double Graph::stationDistance(int station, double lat2, double lon2) const {

    double lat1 = nodes[station].latitude;
    double lon1 = nodes[station].longitude;

    // distance between latitudes
    // and longitudes
    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));

    return rad * c;
}

bool Graph::changeZone(int src, int dest) {
    return nodes[src].zone != nodes[dest].zone;
}

bool Graph::closestPath(int src, int dest, int criteria, bool daytimeSchedule) {

    if (criteria == 1) {
        return bfs(src, dest, daytimeSchedule);
    } else if (criteria == 3) {

        // criar um set
        set<string> possLines = {"walking-line"};
        auto options = new MinHeap<string, int> (nodes[src].adj.size(), "");
        bool v = false;
        for (const Edge& edge: nodes[src].adj) {
            possLines.insert(edge.line);
        }

        for (string possLine : possLines) {
            if (dijkstra(src, dest, criteria, daytimeSchedule, possLine)) {
                v = true;

                int crawl = dest, numBus = 0;
                string auxLine = nodes[crawl].line;
                while (nodes[crawl].pred != -1) {
                    crawl = nodes[crawl].pred;
                    numBus += (auxLine != nodes[crawl].line) && auxLine != "walking-line";
                    auxLine = nodes[crawl].line;
                }
                if (auxLine != "walking-line") numBus++;
                options->insert(possLine, numBus);
            }
        }

        if (v) {
            dijkstra(src, dest, criteria, daytimeSchedule, options->removeMin());
        }

        return v;

    } else {
        return dijkstra(src, dest, criteria, daytimeSchedule);
    }
}

bool Graph::bfs(int src, int dest, bool daytimeSchedule) {

    list<int> queueu;

    for (Node & node : nodes) {
        node.visited = false;
        node.walkingDist = 0;
        node.pred = -1;
        node.line = "";
    }

    nodes[src].dist = 0;
    nodes[src].zonesVisited = 1;
    nodes[src].visited = true;
    nodes[src].line = "walking-line";
    queueu.push_back(src);

    while (!queueu.empty()) {
        int u = queueu.front();
        queueu.pop_front();

        for (const Edge& edge: nodes[u].adj) {
            int d = edge.dest;

            if (!nodes[d].visited && ((edge.operating && edge.daytimeSchedule == daytimeSchedule) || edge.line == "walking-line")) {

                nodes[d].walkingDist = nodes[u].walkingDist + edge.distance * (edge.line == "walking-line");
                nodes[d].dist = nodes[u].dist + edge.distance * (edge.line != "walking-line");

                nodes[d].visited = true;
                nodes[d].zonesVisited = nodes[u].zonesVisited + edge.changeZone;
                nodes[d].line = edge.line;
                nodes[d].pred = u;

                queueu.push_back(d);
                if (d == dest) return true;
            }
        }
    }
    return false;
}

bool Graph::dijkstra(int src, int dest, int criteria, bool daytimeSchedule, string initialLine) {

    auto priorityQueue = new MinHeap<int, double>(nodes.size(), -1);

    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].visited = false;
        nodes[i].weight = INT_MAX;
        nodes[i].walkingDist = 0;
        nodes[i].pred = -1;
        nodes[i].line = "-";

        priorityQueue->insert(i, INT_MAX);
    }

    nodes[src].weight = 0;
    nodes[src].dist = 0;
    nodes[src].zonesVisited = 1;
    nodes[src].line = initialLine;
    priorityQueue->decreaseKey(src, 0);

    while (priorityQueue->getSize() > 0) {
        int u = priorityQueue->removeMin();
        nodes[u].visited = true;

        for (const Edge& edge: nodes[u].adj) {

            int d = edge.dest;

            double w = 0.1 * (0.01 + 0.99 * criteria == 5);
            w += (edge.distance * 2.16 * (edge.line == "walking-line") + edge.distance) * (0.01 + 0.99 * (criteria == 2 || criteria == 5)) + (edge.line == "walking-line") * 100 * ((nodes[u].walkingDist + edge.distance ) > 2);
            w += (edge.line != nodes[u].line) * 2 * (0.01 + 0.99 * ((criteria == 3 && edge.line != "walking-line") || criteria == 5));
            w += (edge.changeZone) * (0.01 + 0.99 * (criteria == 4 || criteria == 5));

            if (!nodes[d].visited && ((edge.operating && edge.daytimeSchedule == daytimeSchedule) || edge.line == "walking-line") && nodes[u].weight + w < nodes[d].weight) {

                nodes[d].walkingDist = nodes[u].walkingDist + edge.distance * (edge.line == "walking-line");
                nodes[d].dist = nodes[u].dist + edge.distance * (edge.line != "walking-line");

                nodes[d].weight = nodes[u].weight + w;
                nodes[d].zonesVisited = nodes[u].zonesVisited + edge.changeZone;
                nodes[d].line = edge.line;
                nodes[d].pred = u;

                priorityQueue->decreaseKey(d, nodes[d].weight);
            }
        }
    }
    return nodes[dest].pred != -1;
}

void Graph::printClosestPath(int dest) {

    vector<Node> path;
    int crawl = dest, numBus = 0, busTime, walkingTime, numZonas = 0;
    string auxZone = "";
    string auxLine = nodes[crawl].line;

    while (nodes[crawl].pred != -1) {
        path.push_back(nodes[crawl]);
        crawl = nodes[crawl].pred;

        if (nodes[crawl].zone != auxZone && nodes[crawl].line != "walking-line") {
            numZonas++;
            auxZone = nodes[crawl].zone;
        }

        numBus += (auxLine != nodes[crawl].line) && auxLine != "walking-line";
        auxLine = nodes[crawl].line;
    }
    if (nodes[crawl].line != "walking-line") numBus++;
    path.push_back(nodes[crawl]);

    cout  << "| From: " << nodes[crawl].name << "  To: " << nodes[dest].name << endl;
    if (numZonas > 0) {
        if (numZonas == 1) numZonas = 2;
        cout << "| Ticket type: Z" << numZonas << endl << "| Price: 0.60 euros (Blue card) + " << ticketPrice(numZonas) << " euros (ticket)" << endl << "| Ticker valid for " << ticketTime(numZonas) << "h" << endl << "|----------------------------------------" << endl;
    } else {
        cout << "| No ticket needed!" << endl;
    }

    // distance from source is in distance array
    busTime = round(nodes[dest].dist * 60 /15.8);
    walkingTime = round(nodes[dest].walkingDist * 60 / 5);
    cout << "| Shortest path length crosses "
         << path.size() << " stations (approximately " << busTime  << " minutes + " << walkingTime << " minutes walking)" << endl;

    cout << "| You will need to catch " << numBus << ( numBus > 1 ? " buses" : " bus");

    // printing path from source to destination
    cout << "\n| Path is:\n";
    cout << "| Start: " << path[path.size() - 1].line << endl;
    cout << "|  -> " << path[path.size() - 1].code;

    for (int i = path.size() - 2; i >= 0; i--) {
        if (path[i].line != path[i + 1].line && !path[i + 1].line.empty()) {
            cout << endl << "| Catch the " << path[i].line << endl;
            cout << "|  -> " << path[i + 1].code;
        }
        cout << " -> " << path[i].code;
    }

    cout << endl;
}

void Graph::printPossiblePath(int dest) {

    vector<int> path;
    int crawl = dest, numBus = 0, busTime, walkingTime, numZonas = 0;
    string auxLine = nodes[crawl].line, auxZone = "";
    path.push_back(crawl);
    while (nodes[crawl].pred != -1) {
        path.push_back(nodes[crawl].pred);
        crawl = nodes[crawl].pred;

        if (nodes[crawl].zone != auxZone && nodes[crawl].line != "walking-line") {
            numZonas++;
            auxZone = nodes[crawl].zone;
        }

        numBus += (auxLine != nodes[crawl].line) && auxLine != "walking-line";
        auxLine = nodes[crawl].line;
    }
    if (nodes[crawl].line != "walking-line") numBus++;

    cout  << "| From: " << nodes[crawl].name << "  To: " << nodes[dest].name << endl;
    if (numZonas > 0) {
        if (numZonas == 1) numZonas = 2;
        cout << "| Ticket type: Z" << numZonas << endl << "| Price: 0.60 euros (Blue card) + " << ticketPrice(numZonas) << " euros (ticket)" << endl << "| Ticker valid for " << ticketTime(numZonas) << "h" << endl;
    } else {
        cout << "| No ticket needed!" << endl;
    }

    // distance from source is in distance array
    busTime = round(nodes[dest].dist * 60 /15.8);
    walkingTime = round(nodes[dest].walkingDist * 60 / 5);
    cout << "| Crosses "
         << path.size() << " stations (approximately " << busTime << " minutes + " << walkingTime << " minutes walking)" << endl;

    cout << "| You will need to catch " << numBus << ( numBus > 1 ? " buses" : " bus") << endl;
}

vector<pair<int, double>> Graph::closestStation(double latitude, double longitude, int num = 1) const {

    vector<pair<int, double>> dist;

    for (int i = 0; i < nodes.size(); i++) {
        dist.emplace_back(i, stationDistance(i, latitude, longitude));
    }

    pair<int, double> temp;
    for(int k = 0; k< dist.size()-1; k++) {
        // (n-k-1) is for ignoring comparisons of elements which have already been compared in earlier iterations

        for(int i = 0; i < dist.size()-k-1; i++) {
            if(dist[ i ].second > dist[ i+1].second ) {
                // here swapping of positions is being done.
                temp = dist[ i ];
                dist[ i ] = dist[ i+1 ];
                dist[ i + 1] = temp;
            }
        }
    }

    return {dist.begin(), dist.begin() + num};
}

void Graph::closeOpenLine(const string& line, bool closeOpen) {
    for (Node &node : nodes) {
        for (Edge &edge : node.adj) {
            if (edge.line == line)
                edge.operating = closeOpen;
        }
    }
}

void Graph::closeOpenStation(int index, bool closeOpen) {
    for (Node &node : nodes) {
        for (Edge &edge : node.adj) {
            if (edge.src == index || edge.dest == index)
                edge.operating = closeOpen;
        }
    }
}

void Graph::createWalking(int maxWalking) {

    int maxDist = 5 * maxWalking / 60;

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            double dist = stationDistance(i, j);
            if ( dist < maxDist) {
                Edge edge;
                edge.src = i;
                edge.dest = j;
                edge.distance = dist;
                edge.line = "walking-line";
                edge.changeZone = nodes[i].zone != nodes[j].zone;

                nodes[i].adj.push_back(edge);
            }
        }
    }
}

void Graph::rmvWalking() {
    for (Node &node : nodes) {
        int max = node.adj.size();
        for (int i = 0; i < max; i++) {
            if (node.adj[i].line == "walking-line") {
                node.adj.erase(node.adj.begin() + i);
                max--;
                i--;
            }
        }
    }
}


float Graph::ticketPrice(int numZones) {
    switch (numZones) {
        case 2:
            return 1.25;
        case 3:
            return 1.60;
        case 4:
            return 2.00;
        case 5:
            return 2.40;
        case 6:
            return 2.85;
        case 7:
            return 3.25;
        case 8:
            return 3.65;
        default:
            return 4.05;
    }
}

float Graph::ticketTime(int numZones) {
    switch (numZones) {
        case 2:
            return 1.00;
        case 3:
            return 1.00;
        case 4:
            return 1.25;
        case 5:
            return 1.50;
        case 6:
            return 1.75;
        case 7:
            return 2.00;
        case 8:
            return 2.25;
        case 9:
            return 2.50;
        case 10:
            return 2.75;
        case 11:
            return 3.00;
        default:
            return 3.25;
    }
}