//
// Created by vasco on 1/22/2022.
//

#ifndef CODE_MENU_H
#define CODE_MENU_H
#include "graph.h"


class Menu {
private:
    int state, settingsOp, daytimeSchedule, maxTimeWalking, numStops;

    vector<string> menuOptions = {"Search Best Path", "View close stations", "Close/Open station/lines", "Searching's settings", "Exit"};
    vector<string> settingsOptions = {"Change searching mode", daytimeSchedule ? "Switch Day/Night (day)" : "Switch Day/Night (night)", "Change max walking time", "Increase number of stops", "Go back"};
    vector<string> searchingModes = {"Path with least stations", "Closest path", "Fewer line exchanges", "The cheapest", "Dynamic mode", "All the above", "Go back"};
    Graph graph;

    void graphInit();

    void mainMenu();
    void searchBestPath();
    void viewCloseStations();
    void settings();
    void closeStopsLines();

    void changeSearchingMode();
    void dayNight();
    void changeWalkingTime();
    void changeStopsNumber();


public:
    Menu();
    bool nextState();
};


#endif //CODE_MENU_H
