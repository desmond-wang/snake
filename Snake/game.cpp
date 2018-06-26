#include "game.h"

#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h> //random
using namespace std;

/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
    cerr << str << endl;
    exit(0);
}

void XInfo::printText(Position p, string s){
    XSetForeground(display, gc[0],BlackPixel(display, screen));
    XDrawImageString(display, pixmap, gc[0], p.x, p.y, s.c_str(), static_cast<int>(s.length()));
}

/*
 * Initialize X and create a window
 */
Game::Game(int argc, char *argv[]) {
    XSizeHints hints;
    unsigned long white, black;
    
    
    
    /*
     * Display opening uses the DISPLAY    environment variable.
     * It can go wrong if DISPLAY isn't set, or you don't have permission.
     */
    xInfo.display = XOpenDisplay( "" );
    if ( !xInfo.display )    {
        error( "Can't open display." );
    }
    
    if(argc == 3){
        defaultFPS = atoi(argv[1]);
        defaultSpeed = atoi(argv[2]) * speedMultiplier;
    }
    FPS = defaultFPS;
    speed = defaultSpeed;
    
    /*
     * Find out some things about the display you're using.
     */
    xInfo.screen = DefaultScreen( xInfo.display );
    
    white = XWhitePixel( xInfo.display, xInfo.screen );
    black = XBlackPixel( xInfo.display, xInfo.screen );
    
    hints.x = 100;
    hints.y = 100;
    hints.width = 800;
    hints.height = 600;
    hints.flags = PPosition | PSize;
    
    xInfo.window = XCreateSimpleWindow(
                                       xInfo.display,                // display where window appears
                                       DefaultRootWindow( xInfo.display ), // window's parent in window tree
                                       hints.x, hints.y,            // upper left corner location
                                       hints.width, hints.height,    // size of the window
                                       Border,                        // width of window's border
                                       black,                        // window border colour
                                       white );                    // window background colour
    
    XSetStandardProperties(
                           xInfo.display,        // display containing the window
                           xInfo.window,        // window whose properties are set
                           "Snake",        // window's title
                           "Snake",            // icon's title
                           None,                // pixmap for the icon
                           argv, argc,            // applications command line args
                           &hints );            // size hints for the window
    
    /*
     * Create Graphics Contexts
     */
    int i = 0;
    xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
    XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
    XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
    XSetLineAttributes(xInfo.display, xInfo.gc[i],
                    4, LineSolid, CapButt, JoinRound);
    
    XFontStruct * font;
    font = XLoadQueryFont(xInfo.display, "12x24");
    XSetFont(xInfo.display, xInfo.gc[i], font->fid);
    
    XSelectInput(xInfo.display, xInfo.window,
                 KeyPressMask |
                 EnterWindowMask | LeaveWindowMask |
                 StructureNotifyMask);  // for resize events
    
    xInfo.width = width;
    xInfo.height = height;
    int depth = DefaultDepth(xInfo.display, DefaultScreen(xInfo.display));
    xInfo.pixmap = XCreatePixmap(xInfo.display, xInfo.window, width, height, depth);
    //color
    XColor xcolor;
    Colormap cmap;
    const char color_vals[6][20]={"Crimson", "violet", "Dark Goldenrod", "Chartreuse","Deep Sky Blue","Dodger Blue"};
    cmap=DefaultColormap(xInfo.display,xInfo.screen);
    for(int i=0; i< 6; ++i){
        XParseColor(xInfo.display, cmap, color_vals[i], &xcolor);
        XAllocColor(xInfo.display,cmap, &xcolor);
        xInfo.colors[i]=xcolor.pixel;
    }
    
    /*
     * Put the window on the screen.
     */
    XMapRaised( xInfo.display, xInfo.window );
    XFlush(xInfo.display);
}

Game::~Game(){
    XCloseDisplay(xInfo.display);
}

/*
 * Restart the game
 */
void Game::restart(){
    snake = Snake{};
    fruit = Fruit{};
    obstacle = Obstacle{};
    speedup = Speedup{};
    shrink = Shrink{};
    speed = defaultSpeed;
    FPS = defaultFPS;
    m_score = 0;
}

initializer_list<string> info {"Name: Daiyang (Desmond) Wang","ID: d268wang (20646168)","Usage:",
    "wasd: to move", "p: to start playing", "esc: to quit game",
    "space: to pause game while playing",">> item: eat to speed up/down", "   item: shrink snake in half"};

void printInfo (XInfo &xInfo, initializer_list<string> &info, Position p){
    int offsetY = 30;
    for(auto s : info){
            xInfo.printText(p,s);
            p.y += offsetY;
        }
}

/*
 * Function to repaint a display list
 */
void Game::repaint() {
    // XClearWindow( xInfo.display, xInfo.window );
    // get height and width of window (might have changed since last repaint)
    
    XWindowAttributes windowInfo;
    XGetWindowAttributes(xInfo.display, xInfo.window, &windowInfo);
    unsigned int height = windowInfo.height;
    unsigned int width = windowInfo.width;
    XSetForeground(xInfo.display, xInfo.gc[0], WhitePixel(xInfo.display, DefaultScreen(xInfo.display)));
    XFillRectangle(xInfo.display, xInfo.pixmap, xInfo.gc[0], 0, 0, width, height);
    XSetForeground(xInfo.display, xInfo.gc[0], BlackPixel(xInfo.display, DefaultScreen(xInfo.display)));
    
    // big black rectangle to clear background
    // draw display list

    switch (state) {
        case State::Start:{
//            XSetForeground(xInfo.display, xInfo.gc[0],BlackPixel(xInfo.display, DefaultScreen(xInfo.display)));
            Position po1 {200,200};
            printInfo(xInfo, info, po1);
            XPoint vertices[3];
            Position p = {10,21};
            vertices[0].x = p.x*grid;
            vertices[0].y = p.y*grid + grid/2;
            vertices[1].x = p.x*grid + grid;
            vertices[1].y = p.y*grid + grid/2;
            vertices[2].x = p.x*grid + grid/2;
            vertices[2].y = p.y*grid;
            XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[0],vertices,3,Convex,CoordModeOrigin);
            vertices[0].x = p.x*grid;
            vertices[0].y = p.y*grid + grid/2;
            vertices[1].x = p.x*grid + grid;
            vertices[1].y = p.y*grid + grid/2;
            vertices[2].x = p.x*grid + grid/2;
            vertices[2].y = p.y*grid + grid;
            XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[0],vertices,3,Convex,CoordModeOrigin);
            break;
        }
        case State::Play:{
            for (auto d : dList) {
                d->paint(xInfo);
            }
            XSetForeground(xInfo.display, xInfo.gc[0],BlackPixel(xInfo.display, xInfo.screen));
            XPoint vertices[2];
            vertices[0].x = 0;
            vertices[0].y = 550;
            vertices[1].x = 800;
            vertices[1].y = 550;
            
            XDrawLines(xInfo.display, xInfo.pixmap, xInfo.gc[0],vertices,2,CoordModeOrigin);
            Position p {5, 580};
            string score = "Score: " + to_string(m_score);
            xInfo.printText(p,score);
            p.x = 580;
            string speedFPS = "Speed: " + to_string(speed/2) + "  FPS: " + to_string(FPS);
            xInfo.printText(p,speedFPS);
            
            break;
        }case State::Pause:{
            string score = "Score: " + to_string(m_score);
            initializer_list<string> pause {"Paused", score,  "\"space\" to resume", "\"esc\" to quit game", "\"r\" to restart game"};
            Position po2 {250,250};
            printInfo(xInfo, pause, po2);
            break;
        }case State::End:{
            string score = "Score: " + to_string(m_score);
            initializer_list<string> gameover {"Game Over", score, "\"esc\" to quit game", "\"r\" to restart game"};
            Position po3 {250,250};
            printInfo(xInfo, gameover, po3);
            break;
        }
            break;
    }
    
    
    XCopyArea(xInfo.display, xInfo.pixmap, xInfo.window, xInfo.gc[0], 0, 0, width, height, 0, 0);
    XFlush( xInfo.display );
    
}





void Game::handleKeyPress(XEvent &event) {
    KeySym key = XLookupKeysym(&event.xkey, 0);
    
    switch (state) {
        case State::Start:
            switch (key) {
                case XK_Escape:
                    state = State::Exit;
                    break;
                case XK_p:
                    state = State::Play;
                    break;
                default: //TODO
                    break;
            }
            break;
        case State::Play:
            switch (key) {
                case XK_w:
                    snake.changeDir(1);
                    break;
                case XK_s:
                    snake.changeDir(2);
                    break;
                case XK_a:
                    snake.changeDir(3);
                    break;
                case XK_d:
                    snake.changeDir(4);
                    break;
                case XK_space:
                    state = State::Pause;
                    break;
                default: //TODO
                    break;
            }
            break;
        case State::Pause:
            switch (key) {
                case XK_space:
                    state = State::Play; // resume
                    break;
                case XK_Escape:
                    state = State::Exit; // Exit the game
                    break;
                case XK_r:
                    state = State::Start; // Restart
                    restart();
                    break;
                    
                default:
                    break;
            }
            break;
        case State::End:
            switch (key) {
                case XK_r:
                    state = State::Start;
                    restart();
                    break;
                case XK_Escape:
                    state = State::Exit;
                    break;
            }
            break;
            
        default:
            break;
    }

}

void Game::handleAnimation(int inside) {
    /*
     * ADD YOUR OWN LOGIC
     * This method handles animation for different objects on the screen and readies the next frame before the screen is re-painted.
     */
    snake.move(xInfo, *this);
    snake.didEatFruit(fruit, *this, obstacle, speedup, shrink);
    snake.didHitBody(*this);
    obstacle.didHitObstacle(snake.getPosition(),*this);
    speedup.removeP(snake.getPosition(), *this);
    shrink.removeP(snake.getPosition(), *this);
    
}

// get microseconds
unsigned long now() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void Game::eventLoop() {
    // Add stuff to paint to the display list
    dList = vector<Displayable *>{&snake, &fruit, &obstacle, &speedup, &shrink};
    
    XEvent event;
    int inside = 0;
    
    int counter = 0;
    
    while( state != State::Exit ) {
        /*
         * This is NOT a performant event loop!
         * It needs help!
         */
        unsigned long t = now();
        
        while (XPending(xInfo.display) > 0) {
            XNextEvent( xInfo.display, &event );
            // cout << "event.type=" << event.type << "\n";
            switch( event.type ) {
                case KeyPress:
                    handleKeyPress(event);
                    break;
                case EnterNotify:
                    inside = 1;
                    break;
                case LeaveNotify:
                    inside = 0;
                    break;
            }
        }
        
        if (state == State::Play){
            counter  += speed;
            int updates = counter / FPS;
            if (updates > 0){
                for (; updates > 0; updates--){
                    handleAnimation(inside);
                    //counter -= FPS;
                }
                counter = 0;
            }
            
        }
        repaint();
        unsigned long t1 = now();
        int res = (1000000/FPS) - static_cast<int>(t1 - t);
        if(res > 0)
            usleep(res);
    }
}

// find empty block
Position Game::emptyBlock(){
    int x = rand() % (width/grid);
    int y = rand() % (height/grid);
    Position p {x, y};
    if(((!snake.isOccupy(p) && !obstacle.isOccupy(p)) &&
       (!shrink.isOccupy(p) && !fruit.isOccupy(p))) && ((!speedup.isOccupy(p)) && p.y < 550/grid)){
        return p;
    }
    return emptyBlock();
}
