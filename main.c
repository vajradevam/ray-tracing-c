#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <math.h>
#include <stdbool.h>

#define COLOR_BLACK       0x000000
#define COLOR_RED         0xFF0000
#define COLOR_GREEN       0x00FF00
#define COLOR_YELLOW      0xFFFF00
#define COLOR_BLUE        0x0000FF
#define COLOR_MAGENTA     0xFF00FF
#define COLOR_CYAN        0x00FFFF
#define COLOR_WHITE       0xFFFFFF

#define COLOR_BRIGHT_BLACK    0x404040
#define COLOR_BRIGHT_RED      0xFF4040
#define COLOR_BRIGHT_GREEN    0x40FF40
#define COLOR_BRIGHT_YELLOW   0xFFFF40
#define COLOR_BRIGHT_BLUE     0x4040FF
#define COLOR_BRIGHT_MAGENTA  0xFF40FF
#define COLOR_BRIGHT_CYAN     0x40FFFF
#define COLOR_BRIGHT_WHITE    0xE0E0E0

#define COLOR_GRAY_DARK       0x202020
#define COLOR_GRAY_MEDIUM     0x808080
#define COLOR_GRAY_LIGHT      0xC0C0C0

#define WIDTH 1280
#define HEIGHT 720

#define RAYS_COUNT 360
#define PI 3.141592

typedef struct {
    double x;
    double y;
    double radius;
} Circle_t;

typedef struct {
    double x;
    double y;
    double m;
} Ray_t;

void draw_cricle(SDL_Surface *surface, Circle_t circle, Uint32 color) {
    
    double start_x = circle.x - circle.radius;
    double end_x = circle.x + circle.radius;

    double start_y = circle.y - circle.radius;
    double end_y = circle.y + circle.radius;

    for (double x = start_x; x <= end_x; x++) {
        for (double y = start_y; y <= end_y; y++) {
            double distance = sqrt(
                pow(x - circle.x, 2) + pow(y - circle.y, 2)
                );

            if (distance < circle.radius) {
                SDL_Rect pixel = (SDL_Rect) {x, y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

void send_rays(Circle_t circle, Ray_t rays[RAYS_COUNT]) {
    for (int i = 0; i < RAYS_COUNT; i++) {
        double angle = ((double) i / RAYS_COUNT) * 2 * PI;
        Ray_t ray = { circle.x, circle.y, angle};
        rays[i] = ray;
    }
}

void draw_rays(SDL_Surface *surface, Ray_t rays[RAYS_COUNT], Circle_t object, Uint32 color) {
    for (int i = 0; i < RAYS_COUNT; i++) {
        Ray_t ray = rays[i];

        bool hit_object = false;
        bool hit_screen = false;

        double step = 1.0;

        double x_draw = ray.x;
        double y_draw = ray.y;

        while ( !hit_screen && !hit_object ) {

            x_draw += step * cos(ray.m);
            y_draw += step * sin(ray.m);

            SDL_Rect pixel = (SDL_Rect) {x_draw, y_draw, 1, 1};
            SDL_FillRect(surface, &pixel, color);

            if (x_draw < 0 || x_draw > WIDTH) hit_screen = 1;
            if (y_draw < 0 || y_draw > HEIGHT) hit_screen = 1;

            // If a pixel enters the object, break the line drawing
            double distance = sqrt(pow(x_draw - object.x, 2) + pow(y_draw - object.y, 2));

            if (distance < object.radius) {
                break;
            }
        }
    }
}

int main() {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Raytracing in Pure C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    Circle_t circle = {200, 200, 20};
    Circle_t shadow_c = {768, 360,  100};

    SDL_Rect black_screen = {0, 0, WIDTH, HEIGHT};

    Ray_t rays[RAYS_COUNT];
    send_rays(circle, rays);

    bool running = true;
    SDL_Event event;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0) {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                send_rays(circle, rays);
            }
        }

        SDL_FillRect(surface, &black_screen, COLOR_BLACK);
        
        draw_rays(surface, rays, shadow_c, COLOR_GRAY_LIGHT);

        draw_cricle(surface, circle, COLOR_RED);
        draw_cricle(surface, shadow_c, COLOR_BRIGHT_BLUE);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);
    }

    return 0;
}
