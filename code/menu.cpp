//
// Created by vasco on 1/22/2022.
//

#include "Menu.h"
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
            settings();
            return true;
        case 4:
            dayNight();
            return true;
        case 5:
            closeStopsLines();
            return true;
        default:
            return false;
    }

    return true;
}

Menu::Menu () {
    state = 0;
    settingsOp = 1;
    daytimeSchedule = 1;
    maxTimeWalking = 20;
    graphInit();
}

void Menu::mainMenu() {

    title("Main Menu");
    showMenu({"Search Best Path", "View close stations", "Searching's settings", "DAY/NIGHT", "Close station/lines", "Exit"});

    int key = getInt("What's your option");
    while (key < 1 || key > 6) key = getInt("What's your option");

    state = key;
}

void Menu::searchBestPath() {

    title("Search Best Path By:");
    cout << "|  Settings for: " << settingsOptions[settingsOp - 1] << endl;

    showMenu({"Station's name", "Station's code", "Closest to me", "Go back"});
    int key = getInt("What's your option");
    while (key < 1 || key > 4) key = getInt("What's your option");

    if (key == 4) {
        state = 0;
        return;
    }

    clearSCR();
    title("Origin Station");
    int index1 = -1;

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

    clearSCR();
    title("Destination Station By:");
    showMenu({"Station's name", "Station's code", "Coordinates"});
    key = getInt("What's your option");
    while (key < 1 || key > 3) key = getInt("What's your option");

    clearSCR();
    title("Destination Station");
    int index2 = -1;

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
            if(errorFunc("No path available")) {
                state = 0;
            }
            return;
        }

        clearSCR();

        int numZonas = graph.nodes[index2].zonesVisited;
        if (numZonas == 1) numZonas++;

        title(settingsOptions[settingsOp - 1]);
        cout << "| Ticket type: Z" << numZonas << endl << "| Price: 0.60 euros (Blue card) + " << ticketPrice(numZonas) << " euros (ticket)" << endl << "| Ticker valid for " << ticketTime(numZonas) << "h" << endl << "|----------------------------------------" << endl;
        graph.printClosestPath(index2);

    } else {
        for (int i = 1; i < 6; ++i) {
            if (!graph.closestPath(index1, index2, i, daytimeSchedule == 1)) {
                if(errorFunc("No path available")) {
                    state = 0;
                }
                return;
            }

            int numZonas = graph.nodes[index2].zonesVisited;
            if (numZonas == 1) numZonas++;

            title(settingsOptions[i - 1]);
            graph.printPossiblePath(index2);
            cout << endl << "| Ticket type: Z" << numZonas << " valid for " << ticketTime(numZonas) << "h"  << endl << "| Price: 0.60 euros (Blue card) + " << ticketPrice(numZonas) << " euros (ticket)" << endl;
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
        options.push_back(graph.nodes[station.first].name + " : " + to_string(station.second) + " Km");
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
    showMenu( settingsOptions, settingsOp);

    int key = getInt("What's your option");
    while (key < 1 || key > 8) key = getInt("What's your option");

    if (key == 8) {
        state = 0;
    } else if (key == 7) {

        clearSCR();
        maxTimeWalking = getInt("New max walking time between stations", "(" + to_string(maxTimeWalking) + " minutes)");
        graph.rmvWalking();
        graph.createWalking(maxTimeWalking);
        endMessage();
        state = 0;

    } else {
        settingsOp = key;
        state = 3;
    }

}

void Menu::dayNight() {

    title("Day/Night Schedule");
    showMenu( {"Day", "Night", "Go back"}, daytimeSchedule);

    int key = getInt("What's your option");
    while (key < 1 || key > 3) key = getInt("What's your option");


    if (key == 3) state = 0;
    else {daytimeSchedule = key;};
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
