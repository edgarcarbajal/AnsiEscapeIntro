# AnsiEscapeIntro
A small program where learn/mess around with ANSI escape code sequences in order to make console programs look better.

---
I tried to find ways to make the output of a console program more visual, like adding progress bars, and such, and what I came across was these ANSI escape code sequences used by terminals to influence how the termal screen looks/operates.

This program is mostly some stuff that I tried out to learn how these codes worked. Eventually, when it came to paring input like arrow keys, and such I learned about `curses`/`ncurses` library, and how it should be a n easier way to work with these codes using that library instead, so I plan to shift my focus into using that library to build a simple CLI UI, maybe using one of my older console projects as a base.

Mostly just placed a snippet of code found from Wikipedia, and messed around trying to get "Conway's Game of Life" working in a console, as an easy test/demo of what I can do with these escape codes.
For the Game of Life implementation, I seem to have run into a problem where using escape codes to move the cursor to a certain coordinate causes the output printed underneath the cursor to be erased, and I could not figure out how to fix it. The underlying code that simulates the game, does seem to work, and you can see the initial, and final boards clearly, so I assume its good enough for this small program.

Maybe in the future, once I am more familiar with `ncurses`, I will come and try to make the Game of Life demo show properly on the console.
