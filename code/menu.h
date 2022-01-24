//
// Created by vasco on 1/22/2022.
//

#ifndef CODE_MENU_H
#define CODE_MENU_H
#include "graph.h"


class Menu {
private:
    int state, settingsOp, daytimeSchedule, maxTimeWalking;

    vector<string> settingsOptions = {"Path with least stations", "Closest path", "Fewer line exchanges", "The cheapest", "Dynamic mode", "All the above", "Change max walking time" , "Go back"};
    Graph graph;

    void graphInit();

    void mainMenu();
    void searchBestPath();
    void viewCloseStations();
    void settings();
    void dayNight();
    void closeStopsLines();

public:
    Menu();
    bool nextState();
};


#endif //CODE_MENU_H
