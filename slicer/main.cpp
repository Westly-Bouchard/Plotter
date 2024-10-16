/**
 * Entry point for slicer program
 *
 * Created by Westly Bouchard
 */

/**
 * Includes for GUI Libraries
 *
 * Using raylib and raygui
 */
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "raygui/src/raygui.h"
#include "raygui/styles/bluish/style_bluish.h"

/**
 * Includes for SVG parsing library
 *
 * Using: nanosvg and nanosvgrast
 */
#include "nanosvg/src/nanosvg.h"
#include "nanosvg/src/nanosvgrast.h"

/**
 * Include for serial communication with Arduino
 *
 * I wrote this with the help of a GeeksForGeeks article about serial communication
 */
#include "serialUtils.hpp"

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

int main() {

//    // For now this will just be hardcoded
//    const char* portName = "/dev/cu.usbmodem1301";
//
//    // Open connection to Arduino and check for failure
//    int fd = openSerialPort(portName);
//
//    if (fd < 0) {
//        return 1;
//    }
//
//    // Configure for correct communication protocol
//    if (!configureSerialPort(fd, B9600)) {
//        closeSerialPort(fd);
//        return 1;
//    }
//
//
    // GUI Setup
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "It's Ambitious");

    // Initial window position
    const Vector2 windowPosition = { SCREEN_WIDTH / 2 - 200 / 2, SCREEN_HEIGHT / 2 - 465 / 2 };

    // Load built in blue color scheme
    GuiLoadStyleBluish();

    SetTargetFPS(60);

    bool btnLoadPressed = false;

    bool imageLoaded = false;

    // Container for image texture data
    Texture2D texture = { 0 };

    // Container for rendered image data
    void* img;

    // GUI Mainloop
    while (!WindowShouldClose()) {

        /*
         * Usage here is to just drag and drop a file onto the window because I don't want to deal with asking for
         * a file picker modal from the operating system.
         */
        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();

            // If the user correctly dropped ONE .svg file onto the window, we can proceed
            if ((droppedFiles.count == 1) && IsFileExtension(droppedFiles.paths[0], ".svg")) {
                // Load the vector data
                NSVGimage * image;

                image = nsvgParseFromFile(droppedFiles.paths[0], "px", 96);

                // Have to actually render the image to show a preview in the window
                struct NSVGrasterizer* rast = nsvgCreateRasterizer();

                // Allocate memory for the image itself
                img = malloc(100 * 100 * 4);

                // Rasterize the image
                nsvgRasterize(rast, image, 0, 0, 1, static_cast<unsigned char*>(img), 100, 100, 400);

                Image raylibImage = LoadImageFromMemory()
            }

            UnloadDroppedFiles(droppedFiles);
        }

        BeginDrawing();

        EndDrawing();
    }

    CloseWindow();

//    closeSerialPort(fd);

    return 0;

}
