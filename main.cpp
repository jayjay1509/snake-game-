#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SNAKE_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void run();

private:
    void init();
    void handleEvents();
    void update();
    void render();
    void reset();

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<Point> snake;
    Point food;
    Direction dir;
    bool running;
};

SnakeGame::SnakeGame() : window(nullptr), renderer(nullptr), dir(RIGHT), running(true) {
    srand(static_cast<unsigned int>(time(nullptr)));
    init();
}

SnakeGame::~SnakeGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SnakeGame::init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    reset();
}

void SnakeGame::reset() {
    snake.clear();
    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    dir = RIGHT;
    food = {rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE, rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE};
}

void SnakeGame::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: if (dir != DOWN) dir = UP; break;
                case SDLK_DOWN: if (dir != UP) dir = DOWN; break;
                case SDLK_LEFT: if (dir != RIGHT) dir = LEFT; break;
                case SDLK_RIGHT: if (dir != LEFT) dir = RIGHT; break;
            }
        }
    }
}

void SnakeGame::update() {
    Point newHead = snake.front();
    switch (dir) {
        case UP: newHead.y -= SNAKE_SIZE; break;
        case DOWN: newHead.y += SNAKE_SIZE; break;
        case LEFT: newHead.x -= SNAKE_SIZE; break;
        case RIGHT: newHead.x += SNAKE_SIZE; break;
    }

    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT) {
        reset();
        return;
    }

    for (auto& segment : snake) {
        if (newHead.x == segment.x && newHead.y == segment.y) {
            reset();
            return;
        }
    }

    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y) {
        food = {rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE, rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE};
    } else {
        snake.pop_back();
    }
}

void SnakeGame::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto& segment : snake) {
        SDL_Rect rect = {segment.x, segment.y, SNAKE_SIZE, SNAKE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {food.x, food.y, SNAKE_SIZE, SNAKE_SIZE};
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}

void SnakeGame::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(100);
    }
}

int main(int argc, char* argv[]) {
    SnakeGame game;
    game.run();
    return 0;
}
