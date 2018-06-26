# snake

Snake is a  a classic 2D video game where the objective is to control a line as it grows in length while preventing it from hitting the bounds or itself.
 
## Game Description:
The game screen displays a snake  ```······＞``` and a fruit ```🍎```  on the screen.<br />
The direction of the snake can be controlled (by wasd) in that it can switch its direction by either turning left or right at a time.<br />
The objective of the snake is to eat the target fruit, which makes it grow in length. Conventionally, there is always only one fruit on the screen.<br />
As the snake eats the fruit, it disappears, and another one appears at a random location.<br />
The snake can die by eating itself (when it collides with itself) or by hitting the edge of the screen or any other obstacles. When the snake dies, a game-end screen should appear.<br />
The game can pause and resume at any time. It can restart when it dead, and have a welcome screen for use description.<br />
When game play, it has score, FPS, and speed at bottom of the game.

## Control:
```
wasd: for snake to move.
p: to start game at inital welcome screen.
esc: to quit game at start, pause or game over state.
space: to pause game while play
r: to restart the game(back to welcome screen) at pause or game over state.
```

## Enhancement:
* added obstacles that the snake must avoid and gaps on the border that lets the snake wrap around to other side of the screen.
* Power-ups: ```＞＞``` shape for eat to speed up or down randomly.
* Power-ups: ```🔹``` shape for shrink the snake size in half.

## Development Environment:
Mac with xcode, but works in Linux by using Makefile.

## For Compile:
If you are using Linux, you can first call ```Make``` to compile and link the file into file called snake.
Then you can run by ```./snake``` 
or
```./snake 30 5``` (30 for framrate and 5 for speed.)
## License
Snake, copyright Desmond Wang, under the MIT license - see the [LICENSE.md](LICENSE.md) file for details
