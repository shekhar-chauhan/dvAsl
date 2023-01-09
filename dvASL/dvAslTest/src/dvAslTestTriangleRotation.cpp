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
 * OR IMPLIED, INCLUDING BURT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "include/dvAslTest.h"

/*
* This function executes the Triangle Rotation Test. 
* First it fetches the modelist, if it turns out to be empty, 
* the program exits. Otherwise, it iterates through all 
* the resolutions and tests the triangle rotation pattern for each.
*/
int triangleRotateTest() {

    size_t i;
    int triangle_result;
    int color_bars_result;
    TRACING();
    vector<android::ui::DisplayMode> modes = getResolution();
    if (modes.empty()) {
        ERR("\nModeList Empty\n");
        return ULT_FAILURE;
    }
    for (i = 0; i < modes.size(); i++) {
        PRINT("\n%d x %d @ %f\n", modes[i].resolution.width, modes[i].resolution.height, modes[i].refreshRate);
        setResolution(to_string(i));
        usleep(DELAY_RESOLUTION * 1000000);
        triangleRotate();
    }
    return ULT_SUCCESS;
}

/*
* This function paints the Triangle Rotation Test on the screen. 
* It initializes the viewPort based on the current resolution 
* and then renders a triangle on the screen and then rotates it 4 times.
*/
void triangleRotate() {

    EGLint configAttribs[] = {
        EGL_DEPTH_SIZE, 0,
        EGL_NONE};
    EGLint majorVersion;
    EGLint minorVersion;
    EGLContext context;
    EGLConfig config;
    EGLSurface surface;
    EGLint w, h;
    EGLDisplay dpy;
    WindowSurface windowSurface;
    TRACING();

    EGLNativeWindowType window = windowSurface.getSurface();
    //initializes the opengl
    dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(dpy, &majorVersion, &minorVersion);

    status_t err = EGLUtils::selectConfigForNativeWindow(
        dpy, configAttribs, window, &config);
    if (err) {
	ERR("\nCouldn't find an EGLConfig matching the screen format\n");
	eglTerminate(dpy);
        void exit();
    }
    
    // creates a window surface for the current display
    surface = eglCreateWindowSurface(dpy, config, window, NULL);
    //create a new EGL rendering context and fetches the width and height
    context = eglCreateContext(dpy, config, NULL, NULL);
    eglMakeCurrent(dpy, surface, surface, context);
    eglQuerySurface(dpy, surface, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &h);

    //creates a viewport for the current width and height
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, w, 0, h, 0, 1);

    //fetching vertices from the screen
    const GLfloat vertex1_s = w / 4;
    const GLfloat vertex2_s = w / 2;
    const GLfloat vertex3_s = 3 * w / 4;
    const GLfloat vertex1_t = h / 4;
    const GLfloat vertex2_t = 3 * h / 4;
    const GLfloat vertex3_t = h / 4;

    //initializing the vertex coordinate array for the triangle
    const GLfloat vertices[] = {
        vertex2_s, vertex2_t, 0,
        vertex1_s, vertex1_t, 0,
        vertex3_s, vertex3_t, 0};

    glShadeModel(GL_SMOOTH);
    //specify clear values for the color buffers
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    //specifies the location and data format of an array of vertex coordinates to use when rendering
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    // Enable the vertex and color state
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    // Draw the vertices as triangles
    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(dpy, surface);
    //rotate the screen for the current resolution
    rotateScreen();

    eglTerminate(dpy);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
