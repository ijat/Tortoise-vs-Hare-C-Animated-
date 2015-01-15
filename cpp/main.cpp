#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

HANDLE cBuffer = GetStdHandle(STD_OUTPUT_HANDLE);	// a command-line buffer that is required to use with the other functions in the Windows.h header.


using namespace std;


int THpos[2] = {0}, next_ticks_to_clear = 10;		// global array variable that stores position of tortoise and hare. It is declared as global value because to track the position will be much easier than passing the value to many functions.


void setcursor(bool, DWORD);						// set the width of the blinking "_" character in the console.
void setcolor(WORD w)								// set the colour of the text in the console.
{SetConsoleTextAttribute(cBuffer,w);return;}


/* These moveTortoise and moveHare is same function. It has been seperated because to follow the questions wants. 
The basic function usage is to move a custom character (to the left,-ve value, or to the right, +ve value) at 
the desired coordinate.*/
int moveTortoise(int x_axis, int y_axis, int steps, int time, char c)
{
	setcolor(11);									// some decorations
	COORD pos = {x_axis,y_axis};
	if (steps > 0)
	{
		for (int i=x_axis ; i<(x_axis+steps) ; i++)	// for +ve steps, move a character to the right
		{
			if (pos.X != 70) {						// limit the x-position to the 70 units (which is 71 if calculate from 0)
			pos.X=i;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << ' ';
			pos.X=i+1;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << c;
			Sleep(time/steps);
			}
			else break;
		}
	}
	else
	{
		steps = steps * (-1);
		for (int i=x_axis ; i>(x_axis-steps) ; i--)
		{
			if (pos.X != 0) {
			pos.X=i;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << ' ';
			pos.X=i-1;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << c;
			Sleep(time/steps);
			}
			else break;
		}
	}
	setcolor(15);
	return pos.X;
}


// Same as function above but for Hare.
int moveHare(int x_axis, int y_axis, int steps, int time, char c)
{
	setcolor(13);
	COORD pos = {x_axis,y_axis};
	if (steps > 0)
	{
		for (int i=x_axis ; i<(x_axis+steps) ; i++)
		{
			if (pos.X != 70) {
			pos.X=i;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << ' ';
			pos.X=i+1;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << c;
			Sleep(time/steps);
			}
			else break;
		}
	}
	else
	{
		steps = steps * (-1);
		for (int i=x_axis ; i>(x_axis-steps) ; i--)
		{
			if (pos.X != 0) {
			pos.X=i;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << ' ';
			pos.X=i-1;
			SetConsoleCursorPosition(cBuffer,pos);
			cout << c;
			Sleep(time/steps);
			}
			else break;
		}
	}
	setcolor(15);
	return pos.X;
}


// Function that moves both tortoise (T) and hare (H) simultaneously based of steps pass in the function.
void moveTH(int t, int h)													// int t for tortoise and h for hare
{
	int a=1, b=1, m[2] = {1,1};
	
	if (t < 0) {															// if the steps is negative, then t is times with -1 to make it positive
		t = t*(-1);															// then m[0] is set to -ve (cont to line 113)
		m[0] = -1;
	}
	if (h < 0) {															// same as above but for hare
		h = h*(-1);
		m[1] = -1;
	}
	
	do {
		if (a <= t) {
			THpos[0] = moveTortoise(THpos[0],8,1*(m[0]),0,'T');				// the usage of m[0] is to make sure that the number of steps is depends on steps. -ve will make it step to the left and vice versa.
			++a;
		}
		if (b <= h) {
			THpos[1] = moveHare(THpos[1],9,1*(m[1]),0,'H');
			++b;
		}
		if (a-1 == t && b-1 == h) break;									// break from the loop when both variable reach the end steps
		Sleep(500/(t+h));													// speed up the time because it takes too long to complete the race (approx 1 tick per second is a bout 2000ms for this Sleep function)
	}
	while (1);
	return;
}


// function that return the number of steps of tortoise based on its percentage
int nTortoise() {						
	int n = rand()% 10 + 1;
	if (n >= 1 && n <=5) return 3;
	else if (n >=6 && n <= 7) return -6;
	else return 1;
}


// function that return the number of steps of hare based on its percentage
int nHare() {						
	int n = rand()% 10 + 1;
	if (n >= 1 && n <=2) return 0;
	else if (n >=3 && n <= 4) return 9;
	else if (n == 5) return -12;
	else if (n >=6 && n <= 8) return 1;
	else return -2;
}


// update the time (in seconds) and ticks in the console.
void updatetime(int s, int t)
{
	COORD tpos;
	tpos.X = 6;
	tpos.Y = 6;
	SetConsoleCursorPosition(cBuffer,tpos);
	t = clock() - t;
	cout << t/CLOCKS_PER_SEC << "s (ticks: " << s << ')';
	return;
}


// Update any strings to the console the specific coordinate.
void updatetext(char *str, int color)
{
	COORD spos = {0,4};
	SetConsoleCursorPosition(cBuffer,spos);
	for (int i=0;i<=69;i++) cout << ' ';								//remove all text in a line
	setcolor(color);
	SetConsoleCursorPosition(cBuffer,spos);
	cout << str;
	setcolor(15);
	spos.X = 0;
	spos.Y = 10;
	SetConsoleCursorPosition(cBuffer,spos);
}


// Function that update the countdown timer when the program starts.
int timer(int a, int b, int c)
{
	COORD timerpos = {a,b};
	for (int i=c;i>=0;i--) {
		SetConsoleCursorPosition(cBuffer,timerpos);
		cout << i << "...";
		Sleep(1000);
	}
}


int main () {
	// check if the console buffer error to create
	if (cBuffer == INVALID_HANDLE_VALUE) {printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());return 1;}			// Check if the console buffer error
	
	// local variables
	int ticks = 0;
	clock_t t;
	COORD xy = {0,24};
	
	// Set the title and the hide the blinking underscore in console
	SetConsoleTitle(">> TvH <<");
	setcursor(0,0);
	
	// just some decoreations :)
	setcolor(7);
	cout << "***********************************************************************\n*                         ";
	setcolor(11);
	cout << "Tortoise ";
	setcolor(15);
	cout << "VS ";
	setcolor(13);
	cout << "Hare ";
	setcolor(15);
	cout << "Race!";
	setcolor(7);
	cout << "                      *\n***********************************************************************\n\n";
	setcolor(15);
	cout << "The race will be start in ";
	cout << "\n\nTime:\n";
	setcolor(7);
	cout << "-----------------------------------------------------------------------\n";						//red ouch cyan tortoise bang yelow hare purple twin green hwin blue
	setcolor(11); 																								//1-blue,2-green,3-cyan,4-red,5-purple,6-yellowish brown,7-white(default),8-increase color light intensity
	cout << "T\n";
	setcolor(13);
	cout << "H\n";
	setcolor(7);
	cout << "-----------------------------------------------------------------------\n";
	setcolor(15);

	srand (time(NULL));												// seed for the rand function
	timer(26,4,3);													// call timer function for the countdown from 3 to 1 when start the program
	
	t = clock();
	updatetext((char*)"BANG!!!! AND THEY'RE OFF!!!",14);			// as required in questions.
	
	while (1)														// both tortoise and hare stays at position 0.
	{																// they will start race at position 1 to 70 (0 not counted as its their initial position which used to show where they at in the console)
		moveTH(nTortoise(),nHare());								// move the tortoise and hare simultaneously
		++ticks;													// calculate the number of ticks
		updatetime(ticks, t);										// update the ticks to the console
		if (THpos[0] == THpos[1] && THpos[0] != 70 && THpos[1] != 70) {	
			updatetext((char*)"OUCH!!!",12);						// update ouch to the console
			timer(8,4,1);											// pause at the position to show that both at the same place.
			next_ticks_to_clear = ticks;
		}
		if (ticks >= next_ticks_to_clear) updatetext((char*)"",15);
		if (THpos[0] == 70 || THpos[1] == 70) break;				// position will not over 70 because it has been limit by the moveHare/moveTortoise function.
	}
	
	if (THpos[0] == THpos[1]) updatetext((char*)"IT'S A TIE",15);	// if both landed at the same position, then it's a tie.
	else {
		if (THpos[1]==70) {
		updatetext((char*)"HARE WINS. YUCH!!!",13);
		setcolor(13);
		cout << "\n\n      \\\\\n";
		cout << "  ,-~~~-\\\\_\n";
		cout << " (        .\\\n";
		cout << "  @\\___(__--'\n";
		}
		else {
		updatetext((char*)"TORTOISE WINS!!! YAY!!!",11);
		setcolor(11);
		cout <<"\n\n        ,-\"\"\"-.\n";
		cout <<"   oo._/ \\\\___/ \\\\\n";
		cout <<"  (____)_/___\\__\\_)\n";
		cout <<"      /_//   \\\\_\\\n\n";
		}
	}
	
	setcolor(15);												// set back the colour of the console to bright white.
	SetConsoleCursorPosition(cBuffer,xy);						// put the cursor position at the bottom of the console
	system("pause");											// pause
  return 0;
}

// A function that can resize or even hide the blinking '_' in the console window
void setcursor(bool visible, DWORD size) 					
{
	if(size == 0)
	{
		size = 20;												// default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(cBuffer,&lpCursor);
}
