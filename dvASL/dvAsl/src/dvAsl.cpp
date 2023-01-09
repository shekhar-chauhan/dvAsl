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

#include "include/dvAsl.h"

/*
* This function fetches the modelist.
*/
vector<android::ui::DisplayMode> getResolution() {

    sp<IBinder> mDisplayToken;
    ui::DynamicDisplayInfo info;
    TRACING();
    mDisplayToken = SurfaceComposerClient::getInternalDisplayToken();
    status_t res = SurfaceComposerClient::getDynamicDisplayInfo(mDisplayToken, &info);
    vector<android::ui::DisplayMode> mode_list = info.supportedDisplayModes;
    return mode_list;
}

/*
* This function uses a service call to set the resolution.
*/
void setResolution(string modeId) {

    /* 
    This is the service call command which takes arguments as:
    1. service which is being called (1035, defined in SurfaceFlinger.cpp) which maps to the call to set the active display mode requested by the user with the modeID.
    2. integer i32 for modeID.
    */
    TRACING();
    string service_call = "service call SurfaceFlinger 1035 i32 " + modeId;
    system(service_call.c_str());
}

/*
* This function rotates the screen.
*/
void rotateScreen() {

    sp<IBinder> vDisplay;
    sp<IBinder> displayToken;
    android::ui::DisplayState displayState;
    android::ui::DisplayMode displayMode;
    SurfaceComposerClient::Transaction t;
    TRACING();
    displayToken = SurfaceComposerClient::getInternalDisplayToken();
    SurfaceComposerClient::getDisplayState(displayToken, &displayState);
    SurfaceComposerClient::getActiveDisplayMode(displayToken, &displayMode);
    ui::Size &resolution = displayMode.resolution;
    ui::Rotation rotateArr[MAX_ROTATION] = { ui::Rotation::Rotation90, ui::Rotation::Rotation180, ui::Rotation::Rotation270, ui::Rotation::Rotation0 };
    int temp = 0;
    int i;

    vDisplay = SurfaceComposerClient::createDisplay(String8("VirtualDisplay"), false);
    t.setDisplayLayerStack(vDisplay, 0);

    for(i = 0; i < MAX_ROTATION; i++) {
        temp = resolution.width;
        resolution.width = resolution.height;
        resolution.height = temp;

        // Hold the screen for 2 seconds before changing to the next resolution
		// So that user can visually see the desktop rotation.
        usleep(SLEEP_ROTATE * 1000000);
        t.setDisplayProjection(displayToken, rotateArr[i],
                            Rect(displayState.layerStackSpaceRect), Rect((resolution)));
        t.apply();
        SurfaceComposerClient::Transaction().apply(true);
    }
}
