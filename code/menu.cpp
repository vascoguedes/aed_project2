//
// Created by vasco on 1/22/2022.
//

#include "menu.h"
#include "auxiliar.h"
#include "iostream"
#include<conio.h>
using namespace std;

bool Menu::nextState() {

    clearSCR();
    switch (state) {
        case 0:
            mainMenu();
            return true;
        case 1:
            searchBestPath();
            return true;
        case 2:
            viewCloseStations();
            return true;
        case 3:
            closeStopsLines();
            return true;
        case 4:
            settings();
            return true;
        case 5:
            changeSearchingMode();
            return true;
        case 6:
            dayNight();
            return true;
        case 7:
            changeWalkingTime();
            return true;
        case 8:
            changeStopsNumber();
            return true;
        default:
            return false;
    }
}

Menu::Menu () {
    state = 0;
    settingsOp = 1;
    daytimeSchedule = 1;
    maxTimeWalking = 15;
    numStops = 1;
    graphInit();
}

void Menu::mainMenu() {

    title("Main Menu");
    showMenu(menuOptions);

    int key = getInt("What's your option");
    while (key < 1 || key > 5) key = getInt("What's your option");

    state = key;
}

void Menu::searchBestPath() {

    title("Search Best Path By:");
    cout << "|  Settings for: " << searchingModes[settingsOp - 1] << endl;

    showMenu({"Station's name", "Station's code", "Closest to me", "Go back"});
    int key = getInt("What's your option");
    while (key < 1 || key > 4) key = getInt("What's your option");

    if (key == 4) {
        state = 0;
        return;
    }

    clearSCR();
    title("Origin Station");
    int index1;

    switch (key) {
        case 1: {
            string name = getString("What's the Station name");
            index1 = graph.stationIndexes[graph.nameToCode[name]];
            break;
        } case 2: {
            string code = getString("What's the Station code");
            index1 = graph.stationIndexes[code];
            break;
        } case 3: {
            double latitude = getDouble("What's your latitude");
            double longitude = getDouble("What's your longitude");
            index1 = graph.closestStation(latitude, longitude, 1)[0].first;
            break;
        } default:
            return searchBestPath();
    }

    for (int j = 1; j <= numStops; ++j) {
        clearSCR();
        title("(" + to_string(j) + ") Destination station:");
        showMenu({"Station's name", "Station's code", "Coordinates"});
        key = getInt("What's your option");
        while (key < 1 || key > 3) key = getInt("What's your option");

        clearSCR();
        title("Destination Station");
        int index2;

        switch (key) {
            case 1: {
                string name = getString("What's the Station name");
                index2 = graph.stationIndexes[graph.nameToCode[name]];
                break;
            } case 2: {
                string code = getString("What's the Station code");
                index2 = graph.stationIndexes[code];
                break;
            } case 3: {
                double latitude = getDouble("What's your latitude");
                double longitude = getDouble("What's your longitude");
                index2 = graph.closestStation(latitude, longitude, 1)[0].first;
                break;
            } default:
                return searchBestPath();
        }

        if (settingsOp != 6) {
            if (!graph.closestPath(index1, index2, settingsOp, daytimeSchedule == 1)) {
                state = !errorFunc("No path available");
                return;
            }

            clearSCR();
            int numZonas = graph.nodes[index2].zonesVisited;
            if (numZonas == 1) numZonas++;

            title(searchingModes[settingsOp - 1]);
            graph.printClosestPath(index2);

        } else {
            for (int i = 1; i < 6; ++i) {
                if (!graph.closestPath(index1, index2, i, daytimeSchedule == 1)) {
                    state = !errorFunc("No path available");
                    return;
                }

                int numZonas = graph.nodes[index2].zonesVisited;
                if (numZonas == 1) numZonas++;

                title(searchingModes[i - 1]);
                graph.printPossiblePath(index2);
            }
        }
        index1 = index2;
        if (j < numStops) {
            cout << "Click enter to continue" << endl;
            getch();
        }
    }

    endMessage();
    state = 0;
}

void Menu::viewCloseStations() {

    title("View close stations");

    double latitude = getDouble("What's your latitude");
    double longitude = getDouble("What's your longitude");
    int num = getInt("How many options");

    vector<pair<int, double>> stations = graph.closestStation(latitude, longitude, num);
    vector<string> options;
    options.reserve(stations.size());

    for (pair<int, double> station : stations) {
        int dist = station.second * 1000;
        options.push_back(graph.nodes[station.first].name + " : " + to_string(dist) + "m");
    }

    clearSCR();
    title("Top " + to_string(num) + " closest stations!");
    showMenu(options);

    cout << "Enter to go back" << endl;
    getch();
    state = 0;
}

void Menu::settings() {

    title("Settings");
    showMenu(settingsOptions);

    int key = getInt("What's your option");
    while (key < 1 || key > 5) key = getInt("What's your option");

    if (key == 5) {
        state = 0;
    } else {
        state = 4 + key;
    }
}

void Menu::changeSearchingMode() {
    title("Change searching mode");
    showMenu( searchingModes, settingsOp);
    int op = getInt("What's your option");
    while (op < 1 || op > 7) op = getInt("What's your option");

    if (op == 7) {
        state = 4;
    } else {
        settingsOp = op;
    }
}

void Menu::changeWalkingTime() {
    clearSCR();
    maxTimeWalking = getInt("New max walking time between stations", "(" + to_string(maxTimeWalking) + " minutes)");
    graph.rmvWalking();
    graph.createWalking(maxTimeWalking);
    endMessage();
    state = 4;
}

void Menu::changeStopsNumber() {
    clearSCR();
    numStops = getInt("New number of stops", "(" + to_string(numStops) + " stops)");
    endMessage();
    state = 4;
}

void Menu::dayNight() {

    daytimeSchedule = !daytimeSchedule;
    settingsOptions = {"Change searching mode", daytimeSchedule ? "Switch Day/Night (day)" : "Switch Day/Night (night)", "Change max walking time", "Increase number of stops", "Go back"};
    state = 4;
}

void Menu::closeStopsLines() {

    title("Close Station/Line ");
    showMenu( {"Open Station", "Close Station", "Open Line", "Close Line", "Go back"});
    int option = getInt("What's your option");

    if (option == 1 || option == 2) {
        int stationIndex = graph.stationIndexes[getString("What's the station code")];
        graph.closeOpenStation(stationIndex, option == 1);
        endMessage("Changes in station saved");
    } else if (option == 3 || option == 4) {
        string line = getString("Which line");
        graph.closeOpenLine(line, option == 3);
        endMessage("Changes in line saved");
    }
    state = 0;
};

void Menu::graphInit() {
    graph.loadStations();
    graph.loadLines();
    graph.createWalking(maxTimeWalking);
}
