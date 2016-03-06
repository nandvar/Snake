#include <iostream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <fstream>
using namespace std;
struct hscores
{
	char name[20];
	int score;
};

int main()
{
	int count = 3;
	struct hscores hi[3];

	strcpy(hi[0].name,"AV");
	hi[0].score=220;
	strcpy(hi[1].name,"AM");
	hi[1].score=120;
	strcpy(hi[2].name,"AS");
	hi[2].score=80;
	

	fstream sofile;
	sofile.open("scores.dat",ios::out);
	for(int i=0; i<count ;i++)
	{
		sofile.write((char*)&hi[i],sizeof(struct hscores));
	}
	sofile.close();
	return 0;
}
