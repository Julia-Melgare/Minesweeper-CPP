# Minesweeper C++
Simple Minesweeper terminal game, you can vary the board size up to 25x25 and have as many mines as you want, as long as there is 1 free tile available in the field.

### Controls
First, the game prompts you to choose the board size (only one side, ex. 25) and the number of mines you want to input. At this step it calculates the maximum number of mines permitted so that you know it when you are choosing.

After that, you may input a coordinate on the game board (Ex. "10 10", "3 4", "21 3", etc.) to reveal a tile. If you want to use the power up, type "F" or "f" instead.

### Power up
The power up implemented is called the "Metal Detector" - it allows you to reveal the location of a random mine in the board without losing the game. You can only use the metal detector once per game.
