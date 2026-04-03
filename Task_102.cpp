#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h" // define all constants in a single place
#include <math.h>


// global variables
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
struct circle{
    float x;
    float y;
    float radius;
} circle;
int last_frame_time = 0;

int initialize_window(void){
    // initialize everything including video support, events etc
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr,"Error initializing SDL\n");
        return FALSE;
    }
    // create a SDL window
    window = SDL_CreateWindow(
        "task_101",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if(!window){
        fprintf(stderr, "Error creating SDL Window\n");
        return FALSE;
    }
    // create a SDL renderer for rendering graphics in the window
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Error creating SDL Renderer\n");
        return FALSE;
    }

    return TRUE;
}

void setup(){
    // define circle with center as the center of the window
    circle.x = WINDOW_WIDTH / 2;
    circle.y = WINDOW_HEIGHT / 2;
    circle.radius = INITIAL_RADIUS;
}

void process_input(){
    SDL_Event event;
    // poll SDL events (e.g. closing by pressing escape key)
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                game_is_running = FALSE;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    // exist game loop by pressing escape key
                    game_is_running = FALSE;
                    break;
                }
        }
    }
}

void update(){
    // waste time to reach frame target time
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks64() - last_frame_time);
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }
    // use delta time to cap frame rate
    float delta_time = (SDL_GetTicks64() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks64();
    // increase radius 50 pixels per second
    circle.radius += 50 * delta_time;
    if(circle.x + circle.radius >= WINDOW_WIDTH ||
        circle.y + circle.radius >= WINDOW_HEIGHT){
            // reset radius to initial value
            circle.radius = INITIAL_RADIUS;
    }
}
// draw circumference of the circle
void draw_circle(){
    // go around 360 degrees
    for(int degree = 0; degree <= 360; degree++){
        // convert angles to radians
        float radian = degree * (M_PI / 180.0);
        // coordinates of the point on the circle at each individual angles
        float x = circle.x + circle.radius * cos(radian);
        float y = circle.y + circle.radius * sin(radian);
        // draw each point
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, (int)round(x), (int)round(y));
    }
}
// draw inside circle
void fill_circle(){
    // go from bottom to top
    for(int y = -circle.radius; y <= circle.radius; y++){
        // using pythagorean theorem to find x for each y
        int x = sqrt(circle.radius * circle.radius - y * y);
        // draw each line
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

    draw_circle();
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

    while(game_is_running){
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}