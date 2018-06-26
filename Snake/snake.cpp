/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <cstdlib>

#include <math.h>
#include <stdio.h>


#include "snake.h"
#include "game.h"
//#include "game.h"

using namespace std;
 

void Snake::paint(XInfo &xinfo) {
    Position po;
    po = position.back();
    position.pop_back();
    XPoint vertices[3];
    if(direction == 1){//w
    vertices[0].x = po.x*grid;
    vertices[0].y = po.y*grid + grid;
    vertices[1].x = po.x*grid + grid;
    vertices[1].y = po.y*grid + grid;
    vertices[2].x = po.x*grid + grid/2;
    vertices[2].y = po.y*grid;
    }else if(direction == 2){//s
        vertices[0].x = po.x*grid;
        vertices[0].y = po.y*grid;
        vertices[1].x = po.x*grid + grid;
        vertices[1].y = po.y*grid;
        vertices[2].x = po.x*grid + grid/2;
        vertices[2].y = po.y*grid + grid;
    }else if(direction == 3){//a
        vertices[0].x = po.x*grid + grid;
        vertices[0].y = po.y*grid;
        vertices[1].x = po.x*grid + grid;
        vertices[1].y = po.y*grid + grid;
        vertices[2].x = po.x*grid;
        vertices[2].y = po.y*grid + 0.5 * grid;
    }else {//d
        vertices[0].x = po.x*grid;
        vertices[0].y = po.y*grid;
        vertices[1].x = po.x*grid;
        vertices[1].y = po.y*grid + grid;
        vertices[2].x = po.x*grid + grid;
        vertices[2].y = po.y*grid + 0.5 * grid;
    }
    XSetForeground(xinfo.display, xinfo.gc[0],xinfo.colors[5]);
    XFillPolygon(xinfo.display, xinfo.pixmap, xinfo.gc[0],vertices,3,Convex,CoordModeOrigin);
    XSetForeground(xinfo.display, xinfo.gc[0],xinfo.colors[4]);
    for(Position p : position){
        XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[0], p.x*grid, p.y*grid, grid, grid, 0, 360*64);
        
    }
    position.emplace_back(po);
    
}

void Snake::move(XInfo &xinfo, Game &game) {
    Position p;
    p = position.back();
    if(direction == 1){
        p.y -= 1;
        
        position.emplace_back(p);
        position.pop_front();
    }
    else if(direction == 2){
        p.y += 1;
        position.emplace_back(p);
        position.pop_front();
    }
    else if(direction == 3){
        p.x -= 1;
        position.emplace_back(p);
        position.pop_front();
    } else {
        p.x += 1;
        position.emplace_back(p);
        position.pop_front();
    }
    if ((p.x < 0 || p.x >= width/grid) || (p.y < 0 || p.y >= 550/grid)) {
        game.dead();
    }
    
}



/*
 * ** ADD YOUR LOGIC **
 * Use these placeholder methods as guidance for implementing the snake behaviour.
 * You do not have to use these methods, feel free to implement your own.
 */
void Snake::didEatFruit(Fruit &f, Game &g, Obstacle &o, Speedup &speed, Shrink &shrink) {
    Position p = position.back();
    if(p.x != f.getPosition().x || p.y != f.getPosition().y)
        return;

    Position newP = position.front();

    if(direction == 1){//w
        newP.y += 1;
    }
    else if(direction == 2){//s
        newP.y -= 1;

    }
    else if(direction == 3){//a
        newP.x += 1;
        
    }
    else {//d
        newP.x -= 1;
    }
    position.emplace_front(newP);
    
    f.setPosition(g.emptyBlock());
    o.addNewObs(g.emptyBlock());
    speed.addNewP(g.emptyBlock());
    shrink.addNewP(g.emptyBlock());
    g.updateScore();
}

void Snake::didHitBody(Game &game) {
    Position p;
    p = position.back();
    position.pop_back();
    for(Position i : position){
        if (p.x == i.x && p.y == i.y){
            game.dead();
        }
    }
    position.emplace_back(p);
}



void Snake::changeSpeed(int newSpd){
    speed = newSpd;
}
void Snake::changeDir(int newDir){
    if ((direction == 1 || direction == 2)
        && (newDir == 3 || newDir == 4)){
        direction = newDir;
    }else if ((direction == 3 || direction == 4)
              && (newDir == 1 || newDir == 2)){
        direction = newDir;
    }
    
    //printf ("dir is %d/n", direction);
}
Snake::Snake() : position{{100/grid - 1, 450/grid}, {100/grid, 450/grid}}  {
    speed = 5;
    direction = 4;
    
}

bool Snake::isOccupy(Position p){
    for(Position i : position){
        if (p.x == i.x && p.y == i.y){
            return true;
        }
    }
    return false;
}

void Snake::shrink(){
    int length = static_cast<int>(position.size());
    if(length <= 2)
        return;
    for (int i = 0; i < length/2; ++i){
        position.pop_front();
    }
}


void Fruit::paint(XInfo &xinfo) {
    XSetForeground(xinfo.display, xinfo.gc[0],xinfo.colors[0]);
    XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[0], p.x*grid, p.y*grid, grid, grid, 0, 360*64);
    
}

Fruit::Fruit():p{50/grid,50/grid} {

}



Obstacle::Obstacle(){
}

void Obstacle::paint(XInfo &xinfo) {
    for(Position p : position){
        XSetForeground(xinfo.display, xinfo.gc[0],xinfo.colors[2]);
        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], p.x*grid, p.y*grid, grid, grid);
        XSetForeground(xinfo.display, xinfo.gc[0],BlackPixel(xinfo.display, DefaultScreen(xinfo.display)));
        XPoint vertices[5];
        vertices[0].x = p.x*grid;
        vertices[0].y = p.y*grid;
        vertices[1].x = p.x*grid +  grid;
        vertices[1].y = p.y*grid;
        vertices[2].x = p.x*grid + grid;
        vertices[2].y = p.y*grid + grid;
        vertices[3].x = p.x*grid;
        vertices[3].y = p.y*grid + grid;
        vertices[4].x = p.x*grid;
        vertices[4].y = p.y*grid;
        XDrawLines(xinfo.display, xinfo.pixmap, xinfo.gc[0],vertices,5,CoordModeOrigin);
    }
}

bool Obstacle::isOccupy(Position p){
    for(Position i : position){
        if (p.x == i.x && p.y == i.y){
            return true;
        }
    }
    return false;
}

void Obstacle::didHitObstacle(Position p, Game &game){
    for(Position i : position){
        if (p.x == i.x && p.y == i.y){
            game.dead();
        }
    }
}

void Speedup::paint(XInfo &xinfo) {
    for(Position p : m_position){
        int offset = 8;
        XSetForeground(xinfo.display, xinfo.gc[0],xinfo.colors[3]);
        XPoint vertices[3];
        vertices[0].x = p.x*grid;
        vertices[0].y = p.y*grid;
        vertices[1].x = p.x*grid + 0.5 * grid;
        vertices[1].y = p.y*grid + 0.5 * grid;
        vertices[2].x = p.x*grid;
        vertices[2].y = p.y*grid + grid;

        XDrawLines(xinfo.display, xinfo.pixmap, xinfo.gc[0],vertices,3,CoordModeOrigin);
        vertices[0].x = p.x*grid +offset;
        vertices[0].y = p.y*grid;
        vertices[1].x = p.x*grid + 0.5 * grid +offset;
        vertices[1].y = p.y*grid + 0.5 * grid;
        vertices[2].x = p.x*grid +offset;
        vertices[2].y = p.y*grid + grid;

        XDrawLines(xinfo.display, xinfo.pixmap, xinfo.gc[0],vertices,3,CoordModeOrigin);
    }
}

void Speedup::removeP(Position p, Game &g){
    for (auto it = m_position.begin(); it != m_position.end(); ) {
        if (((*it).x == p.x) && ((*it).y == p.y)){
            //action::
            //g.setSpeed(g.getSpeed()+2);
            int random = rand() % 2;
            if(random && g.getSpeed() > 2){
                g.setSpeed(g.getSpeed() - 2);
            } else{
                g.setSpeed(g.getSpeed() + 2);
            }
            m_position.erase(it);
            break;
        } else {
            ++it;
        }
    }
}

bool Speedup::isOccupy(Position p){
    for(Position i : m_position){
        if (p.x == i.x && p.y == i.y){
            return true;
        }
    }
    return false;
}

void Shrink::paint(XInfo &xinfo) {
    for(Position p : m_position){
        XSetForeground(xinfo.display, xinfo.gc[0],xinfo.colors[1]);
        XPoint vertices[3];
        vertices[0].x = p.x*grid;
        vertices[0].y = p.y*grid + grid/2;
        vertices[1].x = p.x*grid + grid;
        vertices[1].y = p.y*grid + grid/2;
        vertices[2].x = p.x*grid + grid/2;
        vertices[2].y = p.y*grid;
        XFillPolygon(xinfo.display, xinfo.pixmap, xinfo.gc[0],vertices,3,Convex,CoordModeOrigin);
        vertices[0].x = p.x*grid;
        vertices[0].y = p.y*grid + grid/2;
        vertices[1].x = p.x*grid + grid;
        vertices[1].y = p.y*grid + grid/2;
        vertices[2].x = p.x*grid + grid/2;
        vertices[2].y = p.y*grid + grid;
        XFillPolygon(xinfo.display, xinfo.pixmap, xinfo.gc[0],vertices,3,Convex,CoordModeOrigin);
    }
}

void Shrink::removeP(Position p, Game &game){
    for (auto it = m_position.begin(); it != m_position.end(); ) {
        if (((*it).x == p.x) && ((*it).y == p.y)){
            (game.getSnake())->shrink();
            m_position.erase(it);
            break;
        } else {
            ++it;
        }
    }
}

bool Shrink::isOccupy(Position p){
    for(Position i : m_position){
        if (p.x == i.x && p.y == i.y){
            return true;
        }
    }
    return false;
}


// ** ADD YOUR LOGIC **
/*
 * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
 */






