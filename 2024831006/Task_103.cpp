#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "./constants.h" // define all constants in a single place


// global variables
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int last_frame_time = 0;

typedef struct{
    float x;
    float y;
    float radius;
    float vel_x;
    float vel_y;
    SDL_Color color;
} CIRCLE;

CIRCLE circle_player, circle_npc;
int is_colliding = FALSE;

int initialize_window(void){
    // initialize everything including video support, events etc
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr,"Error initializing SDL\n");
        return FALSE;
    }
    // create a SDL window
    window = SDL_CreateWindow(
        "Task_103",
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
    // create a SDL renderer for rendering graphics in the window
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Error creating SDL Renderer\n");
        return FALSE;
    }

    return TRUE;
}

void setup(){
    // initialize circle that moves from left to right
    circle_npc.x = 0;
    circle_npc.y = WINDOW_HEIGHT / 2;
    circle_npc.radius = 50;
    circle_npc.vel_x = 100;
    circle_npc.vel_y = 0;
    circle_npc.color = {255, 255, 255, 255};

    // initialize circle that is controlled by player
    circle_player.x = WINDOW_WIDTH / 2;
    circle_player.y = 0;
    circle_player.radius = 40;
    circle_player.vel_x = 0;
    circle_player.vel_y = 0;
    circle_player.color = {255, 255, 255, 255};
}

void process_input(){
    SDL_Event event;

    // poll SDL events (e.g. closing by pressing escape key)
    while(SDL_PollEvent(&event)){
        switch(event.type){

            // exit game loop by pressing "X" on the window border
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

    // use keyboard state for smoother movement
    const Uint8* state = SDL_GetKeyboardState(NULL);
    float speed = 200.0f;

    // set velocity to 0 when no key is being pressed
    circle_player.vel_x = 0;
    circle_player.vel_y = 0;

    // set velocity to the circle when key is pressed
    if(state[SDL_SCANCODE_UP]) circle_player.vel_y = -speed;
    if(state[SDL_SCANCODE_DOWN]) circle_player.vel_y = speed;
    if(state[SDL_SCANCODE_LEFT]) circle_player.vel_x = -speed;
    if(state[SDL_SCANCODE_RIGHT]) circle_player.vel_x = speed;
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

    // move circle from left to right
    circle_npc.x += circle_npc.vel_x * delta_time;

    // start over from left when circle passes right border
    if(circle_npc.x - circle_npc.radius > WINDOW_WIDTH){
            circle_npc.x = -circle_npc.radius;
    }

    // move player controlled circle
    circle_player.x += circle_player.vel_x * delta_time;
    circle_player.y += circle_player.vel_y * delta_time;

    // calculate distance between two circles
    float dx = circle_npc.x - circle_player.x;
    float dy = circle_npc.y - circle_player.y;
    float distance_sqaured = (dx * dx) + (dy * dy);

    // calculate sum of radius of two circes
    float radius_sum = circle_npc.radius + circle_player.radius;

    // check if collides
    if(distance_sqaured < (radius_sum * radius_sum)){
        is_colliding = TRUE;

        // change color to red when colliding
        circle_npc.color = (SDL_Color){255, 0, 0, 255};
        circle_player.color = (SDL_Color){255, 0, 0, 255};
    }
    else{
        is_colliding = FALSE;

        // change color to white when not colliding
        circle_npc.color = (SDL_Color){255, 255, 255, 255};
        circle_player.color = (SDL_Color){255, 255, 255, 255};
    }

}
// draw circumference of the circle
void draw_circle(CIRCLE circle){

    // go around 360 degrees
    for(int degree = 0; degree <= 360; degree++){

        // convert angles to radians
        float radian = degree * (M_PI / 180.0);

        // coordinates of the points on the circle at each individual angles
        float x = circle.x + circle.radius * cos(radian);
        float y = circle.y + circle.radius * sin(radian);

        // draw each point
        SDL_SetRenderDrawColor(
            renderer,
            circle.color.r, 
            circle.color.g, 
            circle.color.b, 
            circle.color.a);
        SDL_RenderDrawPoint(renderer, (int)round(x), (int)round(y)); // use round() for better accuracy in drawing correct points
    }
}
// draw inside circle
void fill_circle(CIRCLE circle){

    // go from bottom to top
    for(int y = -circle.radius; y <= circle.radius; y++){

        // find x for each y
        int x = sqrt(circle.radius * circle.radius - y * y);

        // draw each line
        SDL_SetRenderDrawColor(
            renderer,
            circle.color.r,
            circle.color.g,
            circle.color.b,
            circle.color.a);
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

    draw_circle(circle_npc);
    fill_circle(circle_npc);

    draw_circle(circle_player);
    fill_circle(circle_player);

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