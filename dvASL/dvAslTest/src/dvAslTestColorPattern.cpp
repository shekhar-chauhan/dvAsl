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

extern int last_mode;

/*
* This function executes the Color Pattern Test. 
* First it fetches the modelist, if it turns out to be empty, 
* the program exits. Otherwise, it iterates through all 
* the resolutions and tests the color pattern for each.
*/
int colorPatternTest() {

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
        colorPattern();
    }
    return ULT_SUCCESS;
}

/*
* This function paints the Color Pattern Test on the screen. 
* It initializes the viewPort based on the current resolution 
* and then renders RGB color pattern after a delay of 16.66 ms 
* for 1500 frames. At the end of each resolution, it renders a white
* color on the screen and a black screen at the end of each long run test.
*/
void colorPattern() {

    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE};

    EGLint majorVersion;
    EGLint minorVersion;
    EGLContext context;
    EGLConfig config;
    EGLint numConfigs = 0;
    EGLSurface surface;
    EGLint w, h;
    EGLDisplay dpy;
    int i = 0;
    WindowSurface windowSurface;
    TRACING();

    EGLNativeWindowType window = windowSurface.getSurface();
    //initializes the opengl
    dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(dpy, &majorVersion, &minorVersion);
    eglGetConfigs(dpy, NULL, 0, &numConfigs);

    status_t err = EGLUtils::selectConfigForNativeWindow(
        dpy, configAttribs, window, &config);
    if (err) {
        ERR("%s", EGLUtils::strerror(eglGetError()));
        eglTerminate(dpy);
        void exit();
    }

    // creates a window surface for the current display
    surface = eglCreateWindowSurface(dpy, config, window, NULL);
    if (surface == EGL_NO_SURFACE) {
        EGLint err = eglGetError();
        ERR("%s", EGLUtils::strerror(err));
        eglTerminate(dpy);
        void exit();
    }
    //create a new EGL rendering context and fetches the width and height
    context = eglCreateContext(dpy, config, NULL, NULL);
    eglMakeCurrent(dpy, surface, surface, context);
    eglQuerySurface(dpy, surface, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &h);

    glDisable(GL_DITHER);
    glEnable(GL_BLEND);
    //creates a viewport for the current width and height
    glViewport(0, 0, w, h);
    glOrthof(0, w, 0, h, 0, 1);
    //specifies the minimum number of video frame periods per buffer swap for the window associated with the current context.
    eglSwapInterval(dpy, 0);

    //specify clear values for the color buffers
    glClearColor(1, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(dpy, surface);

	//This loop will generate RGB Color Surfaces
	for(i = 0; i < COLOR_PATTERN_FRAMES; i++) {
		glClearColor(colorPatternColors[i][0], colorPatternColors[i][1], colorPatternColors[i][2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(dpy, surface);
		usleep(COLOR_PATTERN_DELAY * 1000);
	}
	
    //white color at end of 1 resolution
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(dpy, surface);
    usleep(INTERMEDIATE_PATTERN_DELAY * 1000000);

    //black screen at the end of last resolution in long run test
    if (last_mode) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        eglSwapBuffers(dpy, surface);
        usleep(INTERMEDIATE_PATTERN_DELAY * 1000000);
        last_mode = 0;
    }

    eglTerminate(dpy);
}
