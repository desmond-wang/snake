#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include <deque>
#include <string>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
using namespace std;

struct XInfo;
struct Game;

struct Position {
    int x;
    int y;
    
};

/*
 * An abstract class representing displayable things.
 */

class Displayable {
public:
    virtual void paint(XInfo &xinfo) = 0;
    virtual Position getPosition() = 0;
    virtual bool isOccupy(Position p)=0;
};

class Fruit : public Displayable {
public:
    virtual void paint(XInfo &xinfo);
    Fruit();
    Position getPosition(){return p;}
    void setPosition(Position newP){
        p = newP;
    }
    bool isOccupy(Position position){return (position.x == p.x && position.y == p.y);}
private:
    Position p;
};



class Speedup : public Displayable { //speed up the snake
public:
    virtual void paint(XInfo &xinfo);
    Position getPosition(){return m_position.back();}
    void addNewP(Position newP){m_position.emplace_back(newP);}
    void removeP(Position p, Game &g);
    bool isOccupy(Position p);
private:
    vector<Position> m_position;
};

class Shrink : public Displayable { //shrink the snake half size
public:
    virtual void paint(XInfo &xinfo);
    Position getPosition(){return m_position.back();}
    void addNewP(Position newP){m_position.emplace_back(newP);}
    void removeP(Position p, Game &g);
    bool isOccupy(Position p);
private:
    vector<Position> m_position;
};


class Obstacle : public Displayable {
public:
    Obstacle();
    virtual void paint(XInfo &xinfo);
    void addNewObs(Position newP){position.emplace_back(newP);}
    Position getPosition(){return position.back();}
    bool isOccupy(Position p);
    void didHitObstacle(Position p, Game &game);
private:
    vector<Position> position;
};



class Snake : public Displayable {
public:
    Snake();
    virtual void paint(XInfo &xinfo);
    void move(XInfo &xinfo, Game &game);
    void didEatFruit(Fruit &f, Game &g, Obstacle &o, Speedup &p, Shrink &s);
    void didHitBody(Game &game);
    void changeSpeed(int newSpd);
    void changeDir(int newDir);
    Position getPosition(){return position.back();}
    bool isOccupy(Position p);
    void shrink();

private:
    deque<Position> position;
    int blockSize;
    int speed;
    int direction;
    
};



#endif
