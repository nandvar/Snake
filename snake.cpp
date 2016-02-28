#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <fstream>

#define NO_REC 10

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton dir;

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}
void Draw()
{
	system("clear");
	for (int i = 0; i < width+2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0)
				cout << "#";
			if (i == y && j == x)
				cout << "O";
			else if (i == fruitY && j == fruitX)
				cout << "F";
			else
			{
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}


			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}

	for (int i = 0; i < width+2; i++)
		cout << "#";
	cout << endl;
	cout << "Score:" << score << endl;
}
void Input()
{
	if (kbhit())
	{
		switch (getchar())
		{
		case 'a':
			if(dir != RIGHT) dir = LEFT;
			break;
		case 'd':
			if(dir != LEFT) dir = RIGHT;
			break;
		case 'w':
			if(dir != DOWN) dir = UP;
			break;
		case 's':
			if(dir != UP) dir = DOWN;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}
void Logic()
{
	int prevX = tailX[0]; //X cood of first part after head
	int prevY = tailY[0]; //y cood of first part after head
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++) //ntail = num of parts after head
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	default:
		break;
	}
	//if (x > width || x < 0 || y > height || y < 0)
	//	gameOver = true;
	if (x >= width) x = 0; else if (x < 0) x = width - 1;
	if (y >= height) y = 0; else if (y < 0) y = height - 1;

	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;

	if (x == fruitX && y == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
}

int scoreio(int i)
{
	/*
	i = 0: GET mode (Incomplete)
		Get your highest score from file.
		If your record doesn't exist, get highest score ever.

	i = 1: PUT mode (Incomplete)
		Put your score into the file, after sorting.

	i = 2: GETALL mode (Pending)
		Get array of all recorded highscores.
	*/

	int count = 0;

	struct hscores
	{
		string name;
		int score;
	}hi[NO_REC];

	string id;
	cout<<"Enter your id: ";
	cin>>id;

	switch (i)
	{
		case 0:
		{
			ifstream sfile;
			sfile.open("scores.dat");
			for(int j = 0; j < NO_REC || !sfile.eof(); j++, count++)
			{
				sfile>>hi[j].name;
				sfile>>hi[j].score;
			}
			sfile.close();

			cout<<endl<<count;

			return 1000; //dummy value. change later
		}

		case 1:
		{
			ofstream sfile;
			sfile.open("scores.dat", ios::app);
			sfile<<score;
			sfile.close();
			return -1;
		}

		case 2:
		{
			return 1000; //dummy value
		}

		default:
		{
			return 1000; //dummy value
		}
	}
}

int main()
{
	int mill, high;

	high = scoreio(0); //get the highest score in high

	cout<<"\tSNAKE"<<endl<<"CHOOSE DIFFICULTY LEVEL (1 - 10): ";
	cin>>mill;
	mill = 10 * (10 - mill);
	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
		struct timespec ts;
		ts.tv_sec = mill / 1000;
		ts.tv_nsec = (mill % 1000) * 1000000;
		nanosleep(&ts, NULL);
	}

	if (score > high)
		scoreio(1); //if you beat the lowest highscore, add
				//to high score file
	return 0;
}
