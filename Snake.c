#include "sl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
enum Direction {
    LEFT = 0 , RIGHT= 1 , UP=2 , DOWN=3
};
int headx , heady;
int size;
enum Direction dir;
int gameover;
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 600;
int font ;
int dx[]={-1,1,0,0};
int dy[]={0,0,1,-1};
void initialize();
void move(int dx , int dy);
void changeDirction(enum Direction dir);
void show();
void generateFood();
void getkey();
void save();
int board[50][50];
void sort(int h[] , int n){
    for(int i = 0; i < n ; i++){
        for(int j = i +1 ; j < n ; j++){
            if (h[i]<h[j])
            {
                int t = h[i];
                h[i]=h[j];
                h[j] =t;
            }
        }
    }
}
int main(int args, char *argv[])
{


	slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake", 0);
	slSetBackColor(0.0, 0.0, 0.0);		// black
    initialize();
////    show();
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		getkey();
		move(dx[dir] , dy[dir]);
		if (gameover)
            break;
		show();
		slRender();
		for(int i = 0 ; i < 50000000 ; i++){

		}
	}
	save();
	// shut down our sigil window
	slClose();

	// program ends
	return 0;
}

void initialize(){
    font = slLoadFont("\\ttf\\font.ttf");
	slSetFont(font, 28);
	FILE * pFile;
    pFile = fopen ("isAgain.txt","r");
    int isAgain;
    fscanf(pFile, "%d" , &isAgain);
    fclose(pFile);
    gameover = 0;
    if (isAgain){

        headx = 25;
        heady = 24;
        //board[headx][heady] = 3;
        //board[headx+1][heady] = 2;
        for(int i = 0; i < 50 ; i++){
            for(int j = 0; j < 50 ; j++){
                board[i][j] = 0;
            }
        }
        board[headx][heady] = 1;

        size = 3;
        generateFood();
    }
    else {
        pFile = fopen ("board.txt","r");
        for(int i = 0 ; i < 50; i++){
            for(int j = 0; j < 50 ; j++){
                fscanf(pFile , "%d" , &board[i][j]);
            }
        }
        fscanf(pFile , "%d" , &headx);
        fscanf(pFile , "%d" , &heady);
        fscanf(pFile , "%d" , &size);
        int d;
        fscanf(pFile , "%d" , &d);
        if (d == 0)
            dir = LEFT;
        if (d==1)
            dir = RIGHT;
        if (d ==2)
            dir=UP;
        if (d==3)
            dir == DOWN;
        fclose(pFile);
    }
}

void move(int dx , int dy){
	headx+=dx;
	heady+=dy;
	if (board[headx][heady] == -1 /* food*/){
		size++;
		generateFood();
	}
	if (board[headx][heady]>0)
	{
		gameover = 1;
		return;
	}
//	if (headx == 1 || (heady == 1) || (headx == 50) || (heady == 50))
//    {
//        gameover = 1;
//        return;
//    }
	if (headx == 1)
        headx = 49;
    if (heady == 1)
        heady = 49;
	if (headx == 50)
        headx = 2;
    if (heady == 50)
        heady = 2;
	board[headx][heady] = size+1;
	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			if (board[i][j] > 0)
				board[i][j] --;
		}
	}
}

void changeDirction(enum Direction direction){
	if ( direction == RIGHT && dir != LEFT )
		dir = RIGHT;
	if ( direction == LEFT && dir != RIGHT )
		dir = LEFT;
	if ( direction == UP && dir != DOWN )
		dir = UP;
	if ( direction == DOWN && dir != UP )
		dir = DOWN;
}

void show(){
	for (int i = 2; i <= 49; ++i)
	{
		for (int j = 2; j <= 49; ++j)
		{
			if (board[i-1][j-1] == -1){
				slSetForeColor(0.0, 0.0, 1.0, 1.0);
				slCircleFill(i * 10.0 - 5, j * 10.0 - 5, 5.0, 6);
			}
			if (board[i-1][j-1]>0)
			{
				slSetForeColor(1.0, 0.0, 0.0, 1.0);
				slCircleFill(i * 10.0 - 5, j * 10.0 - 5, 5.0, 6);
			}

          //  slRender();
		}
	}
	double x1 = 5 , xn = 495;
	double y1 = 5 , yn = 495;
	slSetForeColor(1.0, 1.0, 1.0, 1.0);
	slLine(x1,y1,x1,yn);
	slLine(x1,y1,xn,y1);
	slLine(xn,y1,xn,yn);
	slLine(xn,yn,x1,yn);

//	slText(20,550, "Score : ");

}

void generateFood(){
	int empty = 50 * 50;
	empty-=size;
	empty-= 198;
	srand(time(NULL));
	int rnd = rand() % empty;
	for (int i = 1; i < 49; ++i)
	{
	    int flag = 1;
		for (int j = 1; j < 49; ++j)
		{
			if(board[i][j]== 0){
                if (!rnd){
                    flag = 0;
					board[i][j]= -1;
					break;
				}
				rnd--;

			}
		}
		if (!flag)
            break;
	}
}
void getkey(){
	if(slGetKey(SL_KEY_UP)){
		changeDirction(UP);
		return;
	}
	if(slGetKey(SL_KEY_DOWN)){
		changeDirction(DOWN);
		return;
	}
	if(slGetKey(SL_KEY_LEFT)){
		changeDirction(LEFT);
		return;
	}
	if(slGetKey(SL_KEY_RIGHT)){
		changeDirction(RIGHT);
		return;
	}
}

void save(){

    if (gameover){
        FILE * pFile;
        pFile = fopen ("scores.txt","r");
//        fclose("(");
        int h[32];
        for(int i = 0; i <= 31; i++)
            h[i]=0;
        int i = 0;
        while (fscanf(pFile,"%d",&h[i])!=EOF){
            i++;
        }
        h[i] = size-2;
        i++;
        sort(h , i);

         i = 0;
         fclose(pFile);
        pFile = fopen ("scores.txt","w");
        while(i < 25)
            fprintf(pFile , "%d\n" , h[i++]);
        fclose(pFile);
        pFile = fopen ("isAgain.txt","w");
        fprintf(pFile , "%d\n" , 1);
        fclose(pFile);

    }
    else{
        FILE * pFile;
        pFile = fopen ("isAgain.txt","w");
        fprintf(pFile , "%d\n" , 0);
        fclose(pFile);
        pFile = fopen ("board.txt","w");
        for(int i = 0 ; i < 50; i++){
            for(int j = 0; j < 50 ; j++){
                fprintf(pFile , "%d\n" , board[i][j]);
            }
        }
        fprintf(pFile , "%d\n" , headx);
        fprintf(pFile , "%d\n" , heady);
        fprintf(pFile , "%d\n" , size);
        fprintf(pFile , "%d\n" , dir);

        fclose(pFile);

    }
}
