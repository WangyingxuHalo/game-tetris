#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>

void swap (int &a, int &b) {
	int temp = a;
	a = b;
	b = temp;
}

int randNum (int min, int max) {
	return (min + rand() % (max - min + 1));
}

class piece {
	public:
		int score;
		int shape; 
		int next_shape;

		int head_x; //first box of this diamond
		int head_y;

		int size_h;
		int size_w;

		int next_size_h;
		int next_size_w;

		int box_shape[4][4];
		int next_box_shape[4][4];
		int box_map[30][45];

		bool game_over;
	public:
		void initial();
		void setShape(int &cshape, 
				int box_shape[][4], 
				int & size_w,
				int & size_h
				);
		void score_next();
		void judge(); //see if the floor is full or not
		void move(); //move by up left right
		void rotate();
		void isaggin(); //see if next action will cross the border or not
		bool exsqr(int row); //see if this line is empty
};

void piece::set_shape (int &cshape,
		int shape[][4],
		int &size_w,
		int &size_h) {
	int i,j;
	//initialize all the array
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			shape[i][j] = 0;
		}
	}
	//set array according to type of cshape
	switch (cshape) {
        case 0:
            size_h=1;
            size_w=4;
            shape[0][0]=1;
            shape[0][1]=1;
            shape[0][2]=1;
            shape[0][3]=1;
            break;
        case 1:
            size_h=2;
            size_w=3;
            shape[0][0]=1;
            shape[1][0]=1;
            shape[1][1]=1;
            shape[1][2]=1;
            break;
        case 2:
            size_h=2;
            size_w=3;
            shape[0][2]=1;
            shape[1][0]=1;
            shape[1][1]=1;
            shape[1][2]=1;
            break;
        case 3:
            size_h=2;
            size_w=3;
            shape[0][1]=1;
            shape[0][2]=1;
            shape[1][0]=1;
            shape[1][1]=1;
            break;

        case 4:
            size_h=2;
            size_w=3;
            shape[0][0]=1;
            shape[0][1]=1;
            shape[1][1]=1;
            shape[1][2]=1;
            break;

        case 5:
            size_h=2;
            size_w=2;
            shape[0][0]=1;
            shape[0][1]=1;
            shape[1][0]=1;
            shape[1][1]=1;
            break;

        case 6:
            size_h=2;
            size_w=3;
            shape[0][1]=1;
            shape[1][0]=1;
            shape[1][1]=1;
            shape[1][2]=1;
            break;
	}

	head_x = game_win_width / 2;
	head_y = 1;

	if(isaggin()) {
		game_over = true;
	}
}

void piece::rotate() {
	int backup [4][4] = {0};
	back_w = size_w;
	back_h = size_h;
	//if fails, it will return to previous shape
	for(int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			backup[i][j] = box_shape[i][j];
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			box_shape[size_h - j][i] = box_shape[i][j];
		}
	}
	int tempWidth = size_w;
	size_w = size_h;
	size_h = tempWidth;
	if (isaggin()) {
		for(int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j ++) {
				box_shape[i][j] = backup[i][j];
			}
		}
		size_w = back_w;
		size_h = back_h;
	} else {
		//remove previous diamonds
		for (int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				if(backup[i][j] == 1) {
					mvwaddch(game_win, 
							head_y + i,
							head_x + j,
							' ');
					wrefresh(game_win);
				}
			}
		}
		//put diamonds
		for (int i = 0; i < size_h; i++) {
			for (int j = 0; j < size_w; j++) {
				if (this->box_shape[i][j] == 1) {
					mvwaddch(game_win,
							head_y+i,
							head_x+j,
							'#');
				wrefresh(game_win);
				}
			}
		}
	}
}

void piece::move() {
	fd_set set;
	FD_ZERO(&set);
	FD_SET(0, &set);

	struct timeval timeout;
	timeout.tv_sec = 0;
	//maximum waiting time 500000μs
	timeout.tv_usec = 500000;

	if (select(1, &set, NULL, NULL, &timeout) == 0) {
		head_y++;
		if(isaggin()) {
			head_y--;
			for (int i = 0; i < size_h; i++) {
				for (int j = 0; j < size_w; j++) {
					if(box_shape[i][j] == 1) {
						box_map[head_y + i][head_x + j] = 1
					}
				}
			}
		} else {
			for (int i = size_h - 1; i >= 0; i--) {
				for (int j = 0; j < size_w; j++) {
					if(this->box_shape[i][j]==1){
						mvwaddch(game_win,
								head_y-1+i,
								head_x+j,
								' ');
						mvwaddch(game_win,
								head_y+i
								,head_x+j
								,'#');

					}
				}
			}
			wrefresh(game_win);
		}
	}
	if (FD_ISSET(0, &set)) {
		//only when it's not -1, jump out of loop
		while((key = getch()) == -1);
		if (key == KEY_LEFT) {
			head_x--;
			if(isaggin()) {
				head_x++;
			} else {
				for(int i=0; i<size_h;i++) {
					for(int j=0;j<size_w;j++){
						if(this->box_shape[i][j]==1){
							mvwaddch(game_win,head_y+i,head_x+j+1,' ');
							mvwaddch(game_win,head_y+i,head_x+j,'#');

						}
					}
				}
				wrefresh(game_win);
			}
		}
		if(key==KEY_RIGHT){
			head_x++;
			if(isaggin())
				head_x--;
			else{
				for(int i=0; i<size_h;i++)
					for(int j=size_w-1;j>=0;j--){
						if(this->box_shape[i][j]==1){
							mvwaddch(game_win,head_y+i,head_x+j-1,' ');
							mvwaddch(game_win,head_y+i,head_x+j,'#');

						}
					}
				wrefresh(game_win);
			}
		}
		if(key==KEY_DOWN){
			head_y++;
			if(isaggin()){
				head_y--;
				for(int i=0;i<size_h;i++)
					for(int j=0;j<size_w;j++)
						if(box_shape[i][j]==1)
							box_map[head_y+i][head_x+j]=1;
				//show the score, and tip next diamond
				score_next();
				
			}
			else{
				for(int i=size_h-1; i>=0;i--)
					for(int j=0;j<size_w;j++){
						if(this->box_shape[i][j]==1){
							mvwaddch(game_win,head_y-1+i,head_x+j,' ');
							mvwaddch(game_win,head_y+i,head_x+j,'#');

						}
					}
				wrefresh(game_win);
			}
		}
		if(key==KEY_UP) {
			rotate();
		}
		if(head_x+size_w+1>game_win_width)
			head_x=game_win_width-size_w-1;
		if(head_x<1)
			head_x=1;

	}
}
// see if there's out-of-bound or not
bool piece::isaggin() {
	for (int i = 0; i < size_h; i++) {
		for (int j = 0; j < size_w; j++) {
			if(box_shape[i][j] == 1) {
				if(head_y+i > game_win_height-2)
					return true;
				if(head_x+j > game_win_width-2 || head_x+i-1<0)
					return true;
				if(box_map[head_y+i][head_x+j]==1)
					return true ;
			}
		}
	}
}
bool piece::exsqr(int row) {
	for (int j = 1; j < game_win_width - 1; j++) {
		if (box_map[row][i] == 1) {
			return true;
		}
	}
	return false;
}
//if there's a full line, clean this line
void piece::judge() {
	int i, j;
	int line = 0; //record the number of lines that are full
	bool full;
	//except the border
	for (i = 1; i < game_win_height - 1; i++) {
		full = true;
		for (j = 1; j < game_win_width - 1; j++) {
			if (box_map[i][j] == 0) {
				full = false;
			}
		}
		if (full) {
			line++;
			score += 50;
			for (j = 1; j < game_win_width - 1; j++) {
				box_map[i][j] = 0;
			}
		}
	}
	if (line != 0) {
		//move line that is not 0 to bottom
		for (i = game_win_height - 2; i >= 2; i--) {
			//check previous line
			int s = i;
			if (exsqr(i) == 0) {
				//until one line that is not empty, jump out of loop
				while (s > 1 && exsq(--s) == 0);
				for (j = 1; j < game_win_width - 1; j++) {
					box_map[i][j] = box_map[s][j];
					box_map[s][j] = 0;
				}
			}
		}
		for(int i=1;i<game_win_height-1;i++)
			for(int j=1;j<game_win_width-1;j++){
				if(box_map[i][j]==1){
					mvwaddch(game_win,i,j,'#');
					wrefresh(game_win);
				}
				else{
					mvwaddch(game_win,i,j,' ');
					wrefresh(game_win);
				}
			}
	}
}

void piece::score_next() {
}


int main() {
	printf("%d",randNum(1,4));
	return EXIT_SUCCESS;
}
