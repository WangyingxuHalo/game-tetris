>In this project, I use C++, and ncurses library to implement a tetris game.

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

To make a tetris, we need four operation:
1. Show the diamonds
2. Move the diamonds
3. Rotate diamonds
4. Clean one row when it's full
5. Tip the shape of next diamond

![image](https://github.com/WangyingxuHalo/game-tetris/blob/main/tetris.jpg)


