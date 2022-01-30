//
// Created by vasco on 1/22/2022.
//

#ifndef CODE_AUXILIAR_H
#define CODE_AUXILIAR_H
#include "string"
#include "vector"

using namespace std;


/**
 * @brief clear the screen
 */
void clearSCR();

/**
 * @brief show a title with design
 * @param string
 */
void title(const string& string = "");
/**
 * @brief Show a multiple option menu
 * @param options
 * @param settingsOp
 */
void showMenu(const vector<string>& options, int settingsOp = 0);
/**
 * @brief Shows message and reads enter
 * @param string
 */
void endMessage(const string& string = "");
/**
 * @brief shows an error message
 * @param error
 * @return if the user wants to go back
 */
bool errorFunc(const string& error);

/**
 * @brief Asks the user for an input
 * @param text
 * @param text2
 * @return The input validated
 */
int getInt(const string& text = "", const string& text2 = "");
/**
 * @brief Asks the user for an double
 * @param text
 * @param text2
 * @return The input validated
 */
double getDouble(const string& text = "", const string& text2 = "");
/**
 * @brief Asks the user for a string
 * @param text
 * @param text2
 * @return The input validated
 */
string getString(const string& text = "", const string& text2 = "");

#endif //CODE_AUXILIAR_H
