#include <iostream>
#include <SDL.h>
#include <thread>
#include <mutex>
#include <map>

class mandelbrot_set_renderer {
  const int WINDOW_WIDTH = 300;
  const int WINDOW_HEIGHT = 300;
  const int MAX_ITERATIONS = 64;
  const long double SCALE_FACTOR = 1.1;
  const unsigned int THREADS_NUMBER;
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  long double x0 = -2,
              y0 = -2,
              width = 4,
              height = width * WINDOW_HEIGHT / WINDOW_WIDTH;
  long double move_factor = 0.1;
  std::map<std::pair<long double, long double>, int> its;

  void render_row(int start_row, int end_row){ }

 public:
  mandelbrot_set_renderer() : THREADS_NUMBER(std::thread::hardware_concurrency()){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      return;
    }
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0) {
      return;
    }
  }
  ~mandelbrot_set_renderer() {
    if (renderer) {
      SDL_DestroyRenderer(renderer);
    }
    if (window) {
      SDL_DestroyWindow(window);
    }
    SDL_Quit();
  }
  enum moves{
    LEFT,
    RIGHT,
    UP,
    DOWN
  };
  void zoom_in(){
    x0 += (width - width / SCALE_FACTOR) / 2;
    y0 += (height - height / SCALE_FACTOR) / 2;
    width /= SCALE_FACTOR;
    height /= SCALE_FACTOR;
    move_factor /= SCALE_FACTOR;
  }
  void zoom_out(){
    x0 -= (width * SCALE_FACTOR - width) / 2;
    y0 -= (height * SCALE_FACTOR - height) / 2;
    width *= SCALE_FACTOR;
    height *= SCALE_FACTOR;
    move_factor *= SCALE_FACTOR;
  }
  void move(moves m) const {
    long double dx = 0, dy = 0;
    if (m == UP){
      dy -= move_factor;
    }
    if (m == DOWN){
      dy += move_factor;
    }
    if (m == LEFT){
      dx -= move_factor;
    }
    if (m == RIGHT){
      dx += move_factor;
    }
    x0 += dx;
    y0 += dy;
  }
  void render() {
    int iterations;
    long double pixel = width / WINDOW_WIDTH;
    long double x = x0, y = y0;
    for (int i = 0; i < WINDOW_WIDTH; ++i) {
      y = y0;
      for (int j = 0; j < WINDOW_HEIGHT; ++j){
        iterations = 0;
        long double z = 0, zi = 0;
        while (iterations < MAX_ITERATIONS){
          long double zz = z;
          z = z * z - zi * zi;
          zi = zz * zi * 2;
          z += x;
          zi += y;
          iterations++;
          if (z * z + zi * zi >= 4){ break; }
        }
        if (iterations == MAX_ITERATIONS){
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        } else {
          SDL_SetRenderDrawColor(renderer, 255, 0, 256 - iterations * 4, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderDrawPoint(renderer, i, j);
        y += pixel;
      }
      x += pixel;
    }
    SDL_RenderPresent(renderer);
  }
};

int main() {
  SDL_bool done = SDL_FALSE;
  SDL_Event event;
  mandelbrot_set_renderer rnd;

  std::cout << std::thread::hardware_concurrency() << std::endl;

  while (!done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN){
        switch( event.key.keysym.sym ){
          case SDLK_LEFT:
            rnd.move(mandelbrot_set_renderer::LEFT);
            break;
          case SDLK_RIGHT:
            rnd.move(mandelbrot_set_renderer::RIGHT);
            break;
          case SDLK_UP:
            rnd.move(mandelbrot_set_renderer::UP);
            break;
          case SDLK_DOWN:
            rnd.move(mandelbrot_set_renderer::DOWN);
            break;
          case SDLK_z:
            rnd.zoom_in();
            break;
          case SDLK_x:
            rnd.zoom_out();
            break;
          default:
            break;
        }
        rnd.render();
      }
      if (event.type == SDL_QUIT) {
        done = SDL_TRUE;
      }
    }
  }

  return 0;
}
