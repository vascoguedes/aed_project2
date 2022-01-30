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

    /**
     * @brief Starts the graph
     * @brief Loads the stations, lines and max Walking Time
     */
    void graphInit();

    /**
     * @brief Shows the main menu
     */
    void mainMenu();
    /**
     * @brief Shows the closest Path
     */
    void searchBestPath();
    /**
     * @brief Shows the closest Stations
     */
    void viewCloseStations();
    /**
     * @brief Shows the settings menu
     */
    void settings();
    /**
     * @brief Close/Open Stations/Lines
     */
    void closeStopsLines();

    /**
     * @brief Mode menu
     */
    void changeSearchingMode();
    /**
     * @brief Choose day time or night
     */
    void dayNight();
    /**
     * @brief Change walking time
     */
    void changeWalkingTime();
    /**
     * @brief Change stops number
     */
    void changeStopsNumber();


public:
    /**
     * @brief Initializer
     */
    Menu();
    /**
     * @brief Activates the next operation
     * @return If the program continues
     */
    bool nextState();
};


#endif //CODE_MENU_H
