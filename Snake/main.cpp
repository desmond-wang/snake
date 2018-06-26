
/*
 * Start executing here.
 *     First initialize window.
 *     Next loop responding to events.
 *     Exit forcing window manager to clean up - cheesy, but easy.
 */
#include <string>
#include "game.h"
#include <time.h>

using namespace std;
int main ( int argc, char *argv[] ) {
    srand(static_cast <unsigned int>(time(NULL)));
    Game game{argc, argv};
    game.eventLoop();
    
}
