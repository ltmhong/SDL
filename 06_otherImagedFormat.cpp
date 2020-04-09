#include "SDL.h"
#include <iostream>
#include "SDL_image.h"

using namespace std;

//global variables

SDL_Window* window = NULL;								//The window we'll be rendering to
SDL_Surface* screenSurface = NULL;						//The surface contained by the window
SDL_Surface* stretched = NULL;		                    //The image we'll load and show on the srceen

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//prototype

bool init();											//Starts up SDL and creates window
bool loadMedia();										//Loads media
void close();											//Frees media and shuts down SDL
SDL_Surface* loadsurface(string path);                  //Loads individual image

//const string WINDOW_TITLE = "JiLL being STUPID";

/*
An [SDL surface] is just an image data type that contains the pixels of an image along with all data needed to render it.
The images we're going to be dealing with here are the screen image (what you see inside of the window) and the image we'll be loading from a file.
*/

int main(int argc, char* argv[])
{
    //Start up SDL and create window
    if (!init())
    {
        cout << "Failed to initialize!" << endl;
    }
    else
    {
        //Load media
        if (!loadMedia())
        {
            cout << "Failed to load media!" << endl;
        }
        else
        {
            bool quit = false;                      //Main loop flag
            SDL_Event e;                            //Event handler
            while (!quit)                           //while application is running
            {
                while (SDL_PollEvent(&e) != 0)      //Handle events on queue
                    if (e.type == SDL_QUIT) quit = true;

                //THIS IS WHERE WE APPLY THE IMAGE STRETCHED
                SDL_Rect stretchRectangle;
                stretchRectangle.x = 0;
                stretchRectangle.y = 0;
                stretchRectangle.w = SCREEN_WIDTH;
                stretchRectangle.h = SCREEN_HEIGHT;
                SDL_BlitScaled(stretched, NULL, screenSurface, &stretchRectangle);

                SDL_UpdateWindowSurface(window);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}

//function definitions

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("JiLL being STUPID", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);
            /*
            We want to show images inside of the window and in order to do that we need to get the image inside of the window.
            So we call SDL_GetWindowSurface to grab the surface contained by the window.
            */
        }
    }

    return success;
}

SDL_Surface* loadSurface(string path)             //Loads individual image after being optimized
{
    SDL_Surface* optimized = NULL;                          //final optimized image
    SDL_Surface* loadImage = IMG_Load(path.c_str());     //load image at specified path
    if (loadImage == NULL) cout << "Unable to load image " << path.c_str() << "! SDL Error: " << IMG_GetError();
    else
    {
        optimized = SDL_ConvertSurface(loadImage, screenSurface->format, 0);
        if (optimized == NULL) cout << "Unable to optimize image " << path.c_str() << "! SDL Error: " << SDL_GetError();
        SDL_FreeSurface(loadImage);
    }

    return optimized;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load stretching surface
    stretched = loadSurface("my_image/marking.png");
    if (stretched == NULL)
    {
        cout << "Failed to load stretching image!" << endl;
        success = false;
    }

    return success;
}

void close()
{
    //Deallocate surface
    SDL_FreeSurface(stretched);
    stretched = NULL;

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}