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
	string name;
	int score;
};

int main()
{
	int count = 3;
	struct hscores hi[3];

	hi[0].name="AV";
	hi[0].score=220;
	hi[1].name="AM";
	hi[1].score=120;
	hi[2].name="AS";
	hi[2].score=80;
	

	fstream sofile;
	sofile.open("scores.bin",ios::out | ios::binary);
	for(int i=0; i<count ;i++)
	{
		sofile.write((char*)&hi[i],sizeof(struct hscores));
	}
	sofile.close();
	return 0;
}
