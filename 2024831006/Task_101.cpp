#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "./constants.h" // define all constants in a single place


// global variables
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
struct circle{
    float x;
    float y;
    float radius;
} circle;

int initialize_window(void){
    // initialize everything including video support, events etc
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr,"Error initializing SDL\n");
        return FALSE;
    }

    // create a SDL window
    window = SDL_CreateWindow(
        "Task_101",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if(!window){
        fprintf(stderr, "Error creating SDL Window\n");
        return FALSE;
    }

    // createe a SDL renderer for rendering graphics in the window
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Error creating SDL Renderer\n");
        return FALSE;
    }

    return TRUE;
}

void setup(){
    // initialize circle with center as the center of the window
    circle.x = WINDOW_WIDTH / 2;
    circle.y = WINDOW_HEIGHT / 2;
    circle.radius = 80;
}

void process_input(){
    SDL_Event event;

    // poll SDL events (e.g. closing by pressing escape key)
    while(SDL_PollEvent(&event)){
        switch(event.type){

            // exit game loop by clicking "X" on the window border
            case SDL_QUIT:
                game_is_running = FALSE;
                break;

            // exit game loop by pressing Escape key
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    game_is_running = FALSE;
                    break;
                }
        }
    }
}

void update(){
    // no update for this task
}

// draw filled circle
void fill_circle(){

    // go from bottom to top
    for(int y = -circle.radius; y <= circle.radius; y++){

        // find x for each y
        int x = sqrt(circle.radius * circle.radius - y * y);

        // draw each horizontal line
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(
            renderer,
            circle.x - x, circle.y + y, // left point
            circle.x + x, circle.y + y // right point
        );
    }
}

void render(){
    // set color and clear the screen with that color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    fill_circle();

    // present the back buffer to the screen
    SDL_RenderPresent(renderer);
}
// destroy and clean up initialized systems after exiting game
void destroy_window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]){
    game_is_running = initialize_window();

    setup();

    // game loop
    while(game_is_running){
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}