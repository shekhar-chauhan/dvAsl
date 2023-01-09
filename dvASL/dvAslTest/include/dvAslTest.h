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

#ifndef ULT_HEADER_H_
#define ULT_HEADER_H_

#include <EGL/egl.h>
#include <EGLUtils.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <WindowSurface.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <iostream>
#include <limits.h>
#include <private/gui/ComposerService.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ui/DisplayMode.h>
#include <ui/DisplayState.h>
#include <ui/DynamicDisplayInfo.h>
#include <ui/Rect.h>
#include <ui/Region.h>
#include <unistd.h>
#include <utils/StopWatch.h>

#include "dvAsl.h"
#include "debug.h"

#define TRIANGLE_ROTATION_TEST          1                   // integer value of the triangle rotation test
#define COLOR_PATTERN_TEST              2                   // integer value of the color pattern test
#define DESKTOP_ROTATION_TEST           3                   // integer value of the desktop rotation test
#define LONG_RUN_TEST                   4                   // integer value of the long run test
#define EXIT_TEST                       5                   // integer value for exiting the application
#define DELAY_RESOLUTION                3                   // sleep after a change in resolution
#define INTERMEDIATE_PATTERN_DELAY      5                   // black and white screen delay after the end of one long run test
#define COLOR_PATTERN_DELAY             16.66               // time frame after which the frame is swapped in color pattern test
#define COLOR_PATTERN_FRAMES            1500                // total number of frames in color pattern test for each resolution
#define BASE_INTEGER                    10                  // used to validate the user input
#define ULT_SUCCESS                     0                   // return success from a method
#define ULT_FAILURE                     -1                  // return failure from a method
#define VALID_CMD_LINE_INPUT            2                   // checks if only 2 command line args are passed 
#define LONG_RUN_TEST_ITERATION        100                  // long run test iteration count

const string strTriangleRotationTest = "trianglerotationtest";
const string strColorPatternTest     = "colorpatterntest";
const string strDesktopRotationTest  = "desktoprotationtest";
const string strLongRunTest          = "longruntest";

void triangleRotate(void);
int triangleRotateTest(void);
void colorPattern(void);
int colorPatternTest(void);
int desktopRotateTest(void);
int longRunTest(void);
int validateUserInput(const char input_str[]);
void runTests(unsigned long int);
int parseUserCmdInput(string);

const float colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,                                 // Set The Color To Red with full luminance
    0.0f, 1.0f, 0.0f, 1.0f,                                 // Set The Color To Green with full luminance
    0.0f, 0.0f, 1.0f, 1.0f                                  // Set The Color To Blue with full luminance
};

const GLfloat colorPatternColors[][3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };

#endif // ULT_HEADER_H_
