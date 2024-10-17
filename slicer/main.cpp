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

#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION

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

    // Load built in blue color scheme
    GuiLoadStyleBluish();

    // Try to run at 60 fps if possible
    SetTargetFPS(60);

    // Container for image texture data
    Texture2D texture = { 0 };

    // Container for parsed vectors
    NSVGimage* image;

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
                image = nsvgParseFromFile(droppedFiles.paths[0], "px", 96.0);

                /*
                 * Have to reimplement the raylib svg loading code b/c it got removed
                 * So stupid.
                 */
                // Create raylib image object
                Image raylibImage = { nullptr };

                // Instantiate rasterizer
                struct NSVGrasterizer *rast = nsvgCreateRasterizer();

                // Allocate space for rasterized image data
                void* img = malloc(image->width * image->height * 4);

                // Make the call to rasterize the vectors
                nsvgRasterize(
                        rast,
                        image,
                        0,
                        0,
                        1,
                        static_cast<unsigned char*>(img),
                        image->width,
                        image->height,
                        image->width * 4
                );

                // Populate raylib image object with rasterized data
                raylibImage.data = img;
                raylibImage.width = image->width;
                raylibImage.height = image->height;
                raylibImage.mipmaps = 1;
                raylibImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

                // Clean up rasterizer when we're done with it
                nsvgDeleteRasterizer(rast);

                // Create the texture to render to the gui later
                texture = LoadTextureFromImage(raylibImage);

                // Always clean up resources when you're done using them
                UnloadImage(raylibImage);
            }

            // Always clean up resources when you're done with them
            UnloadDroppedFiles(droppedFiles);
        }

        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(0, 19)));

        // If we have a texture (image) loaded
        if (texture.id != 0) {
            // Render it to the screen
            Vector2 position = {
                    static_cast<float>((SCREEN_WIDTH - texture.width) / 2.),
                    static_cast<float>((SCREEN_HEIGHT - texture.height) / 2.)
            };
            DrawTextureEx(texture, position, 0, 1, WHITE);

        // If we don't have a texture (image) loaded
        } else {
            // Prompt the user to drag and drop an image onto the application
            DrawText(
                    "Drag & drop an SVG image file",
                    SCREEN_WIDTH / 2 - 200,
                    SCREEN_HEIGHT / 2 - 15,
                    30,
                    GetColor(GuiGetStyle(0, 18))
                    );
        }

        EndDrawing();
    }

    // Clean up texture if loaded
    if (texture.id != 0) {
        UnloadTexture(texture);
    }

    // Close raylib window
    CloseWindow();

//    closeSerialPort(fd);

    return 0;

}
