#include "snake.h"
#include <string>

/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;

const int width = 800;
const int height = 600;
const int grid = 20; //size of block

const int speedMultiplier = 2;

/*
 * Information to draw on the window.
 */
struct XInfo {
    Display     *display;
    int         screen;
    Window     window;
    GC         gc[3];
    int        width;        // size of window
    int        height;
    Pixmap pixmap;
    unsigned long colors[6];
    
    void printText(Position p, std::string s);
};
enum class State{
    Start,
    Play,
    Pause,
    End,
    Exit
};

class Game {
public:
    Game(int argc, char *argv[]);
    ~Game();
    void eventLoop();
    void setFPS(int fps) {FPS = fps;}
    void setSpeed(int s) {speed = s;}
    int getSpeed() {return speed;}
    Position emptyBlock();
    Snake* getSnake(){return &snake;}
    void dead() {state = State::End;}
    void updateScore() {m_score++;}
private:
    void repaint();
    void handleKeyPress(XEvent &event);
    void handleAnimation(int inside);
    void restart();
    
    vector<Displayable *> dList;           // list of Displayables
    Snake snake;
    Fruit fruit;
    Obstacle obstacle;
    Speedup speedup;
    Shrink shrink;
    XInfo xInfo;
    int FPS;
    int speed;
    State state = State::Start;
    int m_score = 0;
    int defaultFPS = 30;
    int defaultSpeed = 10;

};

void printInfo (XInfo &xInfo, initializer_list<string> &info);
