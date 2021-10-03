>In this project, I use C++, and ncurses library to implement a tetris game.

Game Homepage
![image](https://github.com/WangyingxuHalo/game-tetris/blob/main/images/gamescreenshot.jpg)

Game Page
![image](https://github.com/WangyingxuHalo/game-tetris/blob/main/images/inprogress.jpg)

### Concept of game
To make a tetris, we need four operation:
1. Show the diamonds
2. Move the diamonds
3. Rotate diamonds
4. Clean one row when it's full
5. Tip the shape of next diamond

There are 7 different kinds of diamonds we use in this game.
![image](https://github.com/WangyingxuHalo/game-tetris/blob/main/images/tetris.jpg)

### Necessary library to include in project
Step1: Instaiing ncurses library
```
sudo apt-get update
sudo apt-get install libncurses5-dev
```
Since I‘m using mac and it doesn't support apt-get, I use brew command:

```
brew update
brew install inbncurses5-dev
```

After that, we can include ncuses.h in our file by
```
#include <ncurses.h>
```

And then, using 
```
g++ tetris.cpp -lncurses ncurses
```
to include ncurses and get a.out


### Basic function of ncurses
```
initscr();   //initialize

printw("Hello World!"); //print hello world in the virtual screen

refresh(); //show the content of virtual screen to user

getch();  //wait for the input of user

endwin(); //exit ncurses mode

printw(string) //print string in current place
mvprintw(x,y,string)  //print string in place (x,y)

wprintw(win,string)  //print string in win

mvwprintw(win,x,y,string)  //print string in win (x,y)

```

A new window is created by newwin() funciton. It returns a structural pointer to the window. And this pointer can be used in some function using win parameter. However, we cannot view a window by only creating it. We can use box to draw frame outside the window to see it.

```
WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}
```


