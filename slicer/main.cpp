/**
 * Entry point for slicer program
 *
 * Created by Westly Bouchard
 */

/*
 * Standard library includes
 */
#include <queue>

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
 * Using nanosvg and nanosvgrast
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

const char ENABLE_CTRL_WORD = 'e';
const char DISABLE_CTRL_WORD = 'd';
const char READY_RESPONSE = 'r';
const char NEXT_CURVE = 'n';

enum State {
    IDLE,
    IMAGE_LOADED,
    RUNNING,
};


void dbgPrintVectors(NSVGimage* image);

// The first parameter here feels cursed, but it works, so I'm not going to touch it
bool checkForAndLoadImage(NSVGimage* &img, Texture2D &tex);

void populateCurveBuffer(NSVGimage* img, queue<Curve> &buf);


int main() {

    // Setup for state machine

    State currentState = State::IDLE;
    State previousState = State::IDLE;

    // Create and open serial connection
    SerialConnection conn("/dev/cu.usbmodem1301");

    if (!conn.open()) {
        return 1;
    }

    // GUI Setup
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "It's Ambitious");

    // Load built in blue color scheme
    GuiLoadStyleBluish();

    // Try to run at 60 fps if possible
    SetTargetFPS(60);

    // Container for image texture data
    Texture2D texture = { 0 };

    bool startButtonPressed = false;

    bool stopButtonPressed = false;

    bool awaitReady = false;

    bool awaitNextCurve = false;

    // Container for parsed vectors
    NSVGimage* image = nullptr;

    // Buffer for bezier curves
    queue<Curve> curveBuffer;

    // GUI Mainloop
    while (!WindowShouldClose()) {

        /*
         * Switch statement for updating state machine
         */
        switch (currentState) {

            // We only want to check for a file drop and load an image if the app is currently idle
            case IDLE: {
                if (checkForAndLoadImage(image, texture)) {
                    previousState = currentState;
                    currentState = State::IMAGE_LOADED;
                }
            }
            break;

            // If we have an image loaded, we're looking for the user to give input to run the file
            case IMAGE_LOADED: {
                if (startButtonPressed) {


                    populateCurveBuffer(image, curveBuffer);

                    // Send control word to Arduino
                    conn.writeChar(ENABLE_CTRL_WORD);

                    // Await ready response
                    awaitReady = true;
                }

                if (awaitReady) {
                    char response;

                    readFromSerialPort(conn.getFD(), &response, 1);

                    cout << "DBG: Response " << response << endl;

                    if (response == READY_RESPONSE) {
                        previousState = currentState;
                        currentState = State::RUNNING;

                        awaitReady = false;
                        break;
                    }
                }
            }
            break;

            case RUNNING: {
                // E-Stop can be triggered by pressing the button or by hitting the space bar
                if (stopButtonPressed || IsKeyPressed(KEY_SPACE) || curveBuffer.empty()) {

                    conn.writeChar(DISABLE_CTRL_WORD);

                    awaitNextCurve = false;

                    previousState = currentState;
                    currentState = State::IDLE;
                    break;

                // If this is the first iteration in the run state
                } else if (previousState == State::IMAGE_LOADED){

                    conn.writeCurve(curveBuffer.front());

                    curveBuffer.pop();

                    awaitNextCurve = true;

                    previousState = State::RUNNING;

                }

                if (awaitNextCurve) {
                    char response;

                    readFromSerialPort(conn.getFD(), &response, 1);

                    cout << "DBG: Response " << response << endl;

                    if (response == NEXT_CURVE) {

                        conn.writeCurve(curveBuffer.front());

                        curveBuffer.pop();
                    }
                }
            }
            break;
        }

        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(0, 19)));

        /*
         * Switch statement for rendering portion
         */
        switch (currentState) {

            case State::IDLE: {
                DrawText(
                        "Drag & drop an SVG image file",
                        SCREEN_WIDTH / 2 - 200,
                        SCREEN_HEIGHT / 2 - 15,
                        30,
                        GetColor(GuiGetStyle(0, 18))
                );
            }
            break;

            case State::IMAGE_LOADED: {
                Vector2 position = {
                        static_cast<float>((SCREEN_WIDTH - texture.width) / 2.),
                        static_cast<float>((SCREEN_HEIGHT - texture.height) / 2.)
                };
                DrawTextureEx(texture, position, 0, 1, WHITE);

                startButtonPressed = GuiButton(
                        (Rectangle) {
                            SCREEN_WIDTH / 2 - 100,
                            SCREEN_HEIGHT - 190,
                            200,
                            130
                        },
                        "Start"
                        );
            }
            break;

            case State::RUNNING: {
                // Still want to draw the image
                Vector2 position = {
                        static_cast<float>((SCREEN_WIDTH - texture.width) / 2.),
                        static_cast<float>((SCREEN_HEIGHT - texture.height) / 2.)
                };
                DrawTextureEx(texture, position, 0, 1, WHITE);

                stopButtonPressed = GuiButton(
                        (Rectangle) {
                                SCREEN_WIDTH / 2 - 100,
                                SCREEN_HEIGHT - 190,
                                200,
                                130
                        },
                        "Abort"
                );
            }
            break;
        }

        EndDrawing();
    }

    // Clean up texture if loaded
    if (texture.id != 0) {
        UnloadTexture(texture);
    }

    // Close raylib window
    CloseWindow();

    conn.close();

    return 0;

}

void dbgPrintVectors(NSVGimage* image) {
    // Always skip over the first shape in this case
    // Figma adds the frame as the first shape in the svg, and we don't want to draw that
    for (NSVGshape *shape = image->shapes->next; shape != nullptr; shape = shape->next) {
        cout << "Shape" << endl;
        for (NSVGpath *path = shape->paths; path != nullptr; path = path->next) {
            cout << "Path" << endl;
            for (int i = 0; i < path->npts - 1; i += 3) {
                float* p = &path->pts[i * 2];

                cout << "(" << p[0] << ", " << p[1] << ")" << endl \
                    << "(" << p[2] << ", " << p[3] << ")" << endl \
                    << "(" << p[4] << ", " << p[5] << ")" << endl \
                    << "(" << p[6] << ", " << p[7] << ")" << endl;
            }
        }
    }
}

bool checkForAndLoadImage(NSVGimage* &img, Texture2D &tex) {

    /*
     * Usage here is to just drag and drop a file onto the window because I don't want to deal with asking for
     * a file picker modal from the operating system.
     */
    if (IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();

        // If the user correctly dropped ONE .svg file onto the window, we can proceed
        if ((droppedFiles.count == 1) && IsFileExtension(droppedFiles.paths[0], ".svg")) {
            // Load the vector data
            img = nsvgParseFromFile(droppedFiles.paths[0], "px", 96.0);

            dbgPrintVectors(img);

            /*
             * Have to reimplement the raylib svg loading code b/c it got removed
             * So stupid.
             */
            // Create raylib image object
            Image raylibImage = {nullptr};

            // Instantiate rasterizer
            struct NSVGrasterizer *rast = nsvgCreateRasterizer();

            // Allocate space for rasterized image data
            void *raster = malloc(img->width * img->height * 4);

            // Make the call to rasterize the vectors
            nsvgRasterize(
                    rast,
                    img,
                    0,
                    0,
                    1,
                    static_cast<unsigned char *>(raster),
                    img->width,
                    img->height,
                    img->width * 4
            );

            // Populate raylib image object with rasterized data
            raylibImage.data = raster;
            raylibImage.width = img->width;
            raylibImage.height = img->height;
            raylibImage.mipmaps = 1;
            raylibImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

            // Clean up rasterizer when we're done with it
            nsvgDeleteRasterizer(rast);

            // Create the texture to render to the gui later
            tex = LoadTextureFromImage(raylibImage);

            // Always clean up resources when you're done using them
            UnloadImage(raylibImage);

            // Always clean up resources when you're done with them
            UnloadDroppedFiles(droppedFiles);

            return true;
        }
    }

    // If we've gotten to this point, then we haven't been able to successfully
    return false;
}

void populateCurveBuffer(NSVGimage* img, queue<Curve> &buf) {
    cout << "DBG: Populating curve buffer" << endl;
    /*
     * Start with the second shape in the image
     * At least in files exported from figma the first shape is always just the rectangular frame
     */
    for (NSVGshape* shape = img->shapes->next; shape != nullptr; shape = shape->next) {
        // For every path (curve) in the shape
        for (NSVGpath* path = shape->paths; path != nullptr; path = path->next) {

            // Iterate over the control points and pack a curve struct
            for (int i = 0; i < path->npts - 1; i += 3) {
                float *p = &path->pts[i * 2];
                Curve c;

                c.cp1x = p[0];
                c.cp1y = p[1];

                c.cp2x = p[2];
                c.cp2y = p[3];

                c.cp3x = p[4];
                c.cp3y = p[5];

                c.cp4x = p[6];
                c.cp4y = p[7];

                buf.push(c);
            }
        }
    }

    cout << "Finished population" << endl;
}
