/*
 * Copyright (c) 2011-2022, Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "include/dvAslTest.h"

using namespace std;
using namespace android;

int last_mode = 0;
static map<string, int> mapCmdInput;

int main(int n, char *args[]) {

    if (n == VALID_CMD_LINE_INPUT) {
        int ret = -1;
        string cmd_line_input = args[1];
	TRACING();

        mapCmdInput[strTriangleRotationTest] = TRIANGLE_ROTATION_TEST;
        mapCmdInput[strColorPatternTest] = COLOR_PATTERN_TEST;
        mapCmdInput[strDesktopRotationTest] = DESKTOP_ROTATION_TEST;
        mapCmdInput[strLongRunTest] = LONG_RUN_TEST;

	ret = parseUserCmdInput(cmd_line_input); 
        if (ret != ULT_FAILURE) {
	    runTests(ret);
	    // Exit the application after running the cmd line test
	    PRINT("\nSuccessfully exiting the command line test\n");
	    void exit();
        }
    } else {
	PRINT("\nInvalid / No cmd line input given, continuing with normal execution....\n");
    }

    string str_input;
    unsigned long int user_input;

    do {
        PRINT("\nEnter your choice : \
                \n1. QUICK Triangle Rotation Test \
                \n2. QUICK Color Pattern Test \
                \n3. QUICK Desktop Rotation Test \
                \n4. LONG Run Test \
                \n5. EXIT\n");
        fflush(stdin);
        std::cin >> std::ws; // drop whitespaces
        getline(cin, str_input);
        user_input = validateUserInput(str_input.c_str());
        if (user_input == ULT_FAILURE) {
            PRINT("\nWrong input, please enter again\n");
            continue;
        }
        runTests(user_input);
    } while (user_input != EXIT_TEST);
    return ULT_SUCCESS;
}

/*
* This function parses the command line user input. 
* And runs the test based on the input and then exits. 
*/
int parseUserCmdInput(string cmd_line_input) {

    TRACING();
    if (mapCmdInput.find(cmd_line_input) != mapCmdInput.end()) {
        return mapCmdInput.at(cmd_line_input);
    }
    return ULT_FAILURE;
}

/*
* This function runs all the tests based on the user input. 
*/
void runTests(unsigned long int user_input) {

    TRACING();
    switch (user_input) {
        case TRIANGLE_ROTATION_TEST:
            if(triangleRotateTest() != ULT_SUCCESS) {
		ERR("\nTriangle Rotation Test Failed\n");
	    }
            break;

        case COLOR_PATTERN_TEST:
            if(colorPatternTest() != ULT_SUCCESS) {
		ERR("\nColor Pattern Test Failed\n");
	    }
            break;

        case DESKTOP_ROTATION_TEST:
            if(desktopRotateTest() != ULT_SUCCESS) {
		ERR("\nDesktop Rotation Test Failed\n");
	    }
            break;

        case LONG_RUN_TEST:
            if(longRunTest() != ULT_SUCCESS) {
		ERR("\nLong Run Test Failed\n");
	    }
            break;

        default:
            PRINT("\nInvalid option, please try again\n");
            break;
    }
}

/*
* This function validates the current user input. 
*/
int validateUserInput(const char input_str[]) {
    char *ret_ptr = NULL;
    unsigned long int user_input_value;
    int i = 0;
    TRACING();

    if (strlen(input_str) > 2) {
        return ULT_FAILURE;
    }

    while (input_str[i] != '\0') {
        if (isspace(input_str[i]))
            return ULT_FAILURE;
        i++;
    }

    if (input_str[0] == '-' || input_str[0] == 32) {
        ERR("\nWrong input, please enter again\n");
        return ULT_FAILURE;
    }

    // converts string to unsigned long integer
    user_input_value = strtoul(input_str, &ret_ptr, BASE_INTEGER);
    if ((user_input_value >= 0) && (user_input_value < UINT_MAX) && (ret_ptr != NULL) && (ret_ptr[0] == '\0')) {
        return (int)user_input_value;
    } else {
        ERR("\nWrong input, please enter again\n");
        return ULT_FAILURE;
    }
}

/*
* This function rotates the desktop of the android screen
* for every resolution.
*/
int desktopRotateTest() {

    size_t i;
    vector<android::ui::DisplayMode> modes = getResolution();
    TRACING();
    if (modes.empty()) {
	ERR("\nModeList Empty\n");
        return ULT_FAILURE;
    }
    for (i = 0; i < modes.size(); i++) {
        PRINT("\n%d x %d @ %f\n", modes[i].resolution.width, modes[i].resolution.height, modes[i].refreshRate);
        setResolution(to_string(i));
        usleep(DELAY_RESOLUTION * 1000000);
        rotateScreen();
    }
    return ULT_SUCCESS;
}

/*
* This function runs the endless test, which comprises of the 
* combination of triangle rotation and color pattern test for every resolution.
* It displays a black screen after the end of all the resolutions.
*/
int longRunTest() {

    size_t i;
    int count = 0;
    TRACING();
	
    vector<android::ui::DisplayMode> modes = getResolution();
    if (modes.empty()) {
        ERR("\nModeList Empty\n");
        return ULT_FAILURE;
    }
    while (count < LONG_RUN_TEST_ITERATION) {
        for (i = 0; i < modes.size(); i++) {
            PRINT("\n%d x %d @ %f\n", modes[i].resolution.width, modes[i].resolution.height, modes[i].refreshRate);
            setResolution(to_string(i));
	    //Before doing a screen rotation, wait for Android OS to settle down after setResolution.
            usleep(DELAY_RESOLUTION * 1000000);
            triangleRotate();
        }
        for (i = 0; i < modes.size(); i++) {
            PRINT("\n%d x %d @ %f\n", modes[i].resolution.width, modes[i].resolution.height, modes[i].refreshRate);
            setResolution(to_string(i));
            PRINT("\nmodeID = %zu\n", i);
            if (i == modes.size() - 1) {
                last_mode = 1;
            }
	    //Before doing a screen rotation, wait for Android OS to settle down after setResolution.
            usleep(DELAY_RESOLUTION * 1000000);
            colorPattern();
        }
	count++;
    }
    return ULT_SUCCESS;
}
