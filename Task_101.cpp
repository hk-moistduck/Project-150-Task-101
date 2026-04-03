#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h" //for defining all constants in a single place
#include <math.h>


//global variables
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
struct circle{
    float x;
    float y;
    float radius;
} circle;

int initialize_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr,"Error initializing SDL\n");
        return FALSE;
    }
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

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Error creating SDL Renderer\n");
        return FALSE;
    }

    return TRUE;
}

void setup(){
    circle.x = WINDOW_WIDTH / 2;
    circle.y = WINDOW_HEIGHT / 2;
    circle.radius = 15;
}

void process_input(){
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                game_is_running = FALSE;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    game_is_running = FALSE;
                    break;
                }
        }
    }
}

void update(){
    //to-do
}

void draw_circle(){
    for(int degree = 0; degree <= 360; degree++){
        float radian = degree * (M_PI / 180.0);

        float x = circle.x + circle.radius * cos(radian);
        float y = circle.y + circle.radius * sin(radian);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, (int)round(x), (int)round(y));
    }
}

void fill_circle(){
    for(int y = -circle.radius; y <= circle.radius; y++){
        int x = sqrt(circle.radius * circle.radius - y * y);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(
            renderer,
            circle.x - x, circle.y + y,
            circle.x + x, circle.y + y
        );
    }
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_circle();
    fill_circle();
    
    SDL_RenderPresent(renderer);
}

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