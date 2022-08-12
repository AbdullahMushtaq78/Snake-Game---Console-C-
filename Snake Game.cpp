#include<iostream>
#include<windows.h>
using namespace std;
#include<conio.h>
#include<math.h>
// Define the hidden cursor function
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
void gotoRowCol(int rpos, int cpos)
{
	//This function will point the cursor at given row and col and if user tries to print something on console it will print at rpos and cpos.
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
struct Pos {
	//This struct is used to save the row and col index
	int ri, ci;
};
#define MSS 100 //Max Snake Size
#define Rows  35 //No of Rows in ground area 
#define Cols  90 //No of Cols in ground area
struct Snake {
	//Struct for Snake Object in game
	Pos Ps[MSS]; //Array of position, this is the array that will store the body of snake
	int Size; //current size of snake body
	char SSym ; //Symbol to display as body of snake
	int score; // current score 
	bool GameOver; // for ending the game
};
void FoodGeneration(Pos& Food, Snake S, char Symbol);
void Init(Snake& S,Pos&Food,char FoodSym)
{
	S.Size = 3;
	for (int i = 0; i < S.Size; i++)
	{
		//Initializing the body of snake at the star of the game , you can change the size of snake at the start of the game by passing different number
		S.Ps[i].ri = Rows / 2;
		S.Ps[i].ci = Cols / 2-i;

	}
	S.SSym = '*';
	S.score = 0;
	FoodGeneration(Food, S,FoodSym); //Food generation for first time
	S.GameOver = false;

}
void SnakeDisplay(Snake& S)
{
	for (int i = 0; i < S.Size; i++)
	{
		/*if (i == 0) //You un-comment this part to display the head of snake with differnet char
		{
			gotoRowCol(S.Ps[i].ri, S.Ps[i].ci);
			cout << "O";
		}
		else*/ 
		{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			gotoRowCol(S.Ps[i].ri, S.Ps[i].ci);
			cout << S.SSym;
		}
	}

}
void SnakeErase(Snake& S)
{
	//If snake moves from on position to another then we have to remove it from it's original position 
	for (int i = 0; i < S.Size; i++)
	{
		gotoRowCol(S.Ps[i].ri, S.Ps[i].ci);
		cout << " ";
	}

}
enum Direction {UP,DOWN,LEFT,RIGHT}; //Enums for setting the directions of Snake
const int Up_Arrow = 72, Down_Arrow = 80, Left_Arrow = 75, Right_Arrow = 77; //Codes of keys
void ChangeDirection(Direction& DIR)
{
		char ch = _getch();
		int key = int(ch);
		if (key == -32)
		{
			key = _getch();
			switch (key)
			{
			case Up_Arrow:
				if(DIR!=DOWN)
					DIR = UP;
				break;
			case Down_Arrow:
				if (DIR != UP)
				DIR = DOWN;
				break;
			case Left_Arrow:
				if (DIR != RIGHT)
				DIR = LEFT;
				break;
			case Right_Arrow:
				if (DIR != LEFT)
				DIR = RIGHT;
				break;
			}
		}
		else if (key == 32) //Space key for pausing the game 
		{
			gotoRowCol(36, 20);
			cout << "Game is paused...\n";
			gotoRowCol(37, 20);
			system("Pause");
			gotoRowCol(36, 20);
			cout << "                                     ";
			gotoRowCol(37, 20);
			cout << "                                     ";
		}
}
void ChangeSnakeDirection(Direction DIR,Snake &S)
{
	switch (DIR)
	{
	case UP: //reducing the rows in order to move upward
		S.Ps[0].ri--;
		if (S.Ps[0].ri == -1)
			S.Ps[0].ri = Rows - 1;
		break;
	case DOWN:
		S.Ps[0].ri++;//increasing the rows in order to move downward
		if (S.Ps[0].ri == Rows)
			S.Ps[0].ri = 0;
		break;
	case LEFT: //reducing the cols in order to move left
		S.Ps[0].ci--;
		if(S.Ps[0].ci==-1)
			S.Ps[0].ci = Cols - 1;
		break;
	case RIGHT:
		S.Ps[0].ci++; //increasing the cols in order to move right
		if (S.Ps[0].ci == Cols)
			S.Ps[0].ci = 0;
		break;
	}
}
void MoveSnake(Snake &S,Direction &DIR) {
	for (int i = S.Size - 1; i >= 1; i--)
	{
		S.Ps[i] = S.Ps[i - 1]; //When snake moves it needs to moves it position to direction in which he is going next
	}
	ChangeSnakeDirection(DIR, S);

}
bool isValidFOodLocation(Pos Food, Snake S)
{
	for (int i = 0; i < S.Size; i++)
	{
		if (Food.ri == S.Ps[i].ri && Food.ci == S.Ps[i].ci)
			return false; //Making sure the food doesn't spawn on the body of snake
	}
	return true;
}
void FoodGeneration(Pos &Food,Snake S,char Symbol) {

	do {
		//calculating the row and col of new food location randomly by taking mod with area of playground
		Food.ri = rand() % Rows;
		Food.ci = rand() % Cols;
	} while (!isValidFOodLocation(Food,S));
	gotoRowCol(Food.ri, Food.ci);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout <<Symbol;
}
bool IsFoodEaten(Pos Food,Snake S) {
	return (Food.ri == S.Ps[0].ri&&Food.ci == S.Ps[0].ci);
}
bool IsBonusFoodEaten(Pos Food, Snake S) {
	return (Food.ri == S.Ps[0].ri && Food.ci == S.Ps[0].ci);
}
bool KillItSelf(Snake S) {
	for (int i = 1; i < S.Size; i++)
	{
		if (S.Ps[0].ri == S.Ps[i].ri && S.Ps[0].ci == S.Ps[i].ci) {
			return true; //Checking if the position of head of snake is same as location of snake's other body location. 
		}
	}
	return false;
}
int main()
{
	
	int sleeping = 100;
	char FSym = 'o'; //Symbol for food
	Snake S; //Object of the snake
	Pos Food; // Food Position
	Init(S, Food,FSym); //Initialization of snake object
	Direction DIR = RIGHT; //Direction at the start of the game
	int BonusCounter = 1;
	bool bonus= false;
	do {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //Built-in function to change the color of text on console
		HideCursor();
		gotoRowCol(36, 92);
		cout << "SCORE: " << S.score;
		if (_kbhit()/*Built-in function to check if keyboard is used or not*/)
		{
				ChangeDirection(DIR); //change the direction w.r.t. the pressed key
		}
		else {
			SnakeErase(S); //erasing snake from previous location
			MoveSnake(S, DIR); // moving snake from previous location to new location in memory Pos
			SnakeDisplay(S); //Print the snake on screen
				if (IsFoodEaten(Food, S))
				{
					if (bonus) {
						BonusCounter = 1;
						S.score += 5; //adding 5 in score because snake is at Bonus Food
						S.Size += 5; //increasing the size of snake by 5 stars
					}
					else
						S.score += 1; //simple food
					if (S.score % 3 == 0 && sleeping != 10)
						sleeping -= 10; //increasing the speed of snake as the game progresses
					BonusCounter++;
					if (BonusCounter==5)
					{
						char BonusSym = '$';
						FoodGeneration(Food, S, BonusSym); //Generating Bonus food after 5 bonus counts
					}
					else
					{
						S.Size += 2;
						FoodGeneration(Food, S, FSym);
					}
				}
				if (BonusCounter==5)
					bonus = true;
				else
					bonus = false;
			Sleep(sleeping); //using the sleep to slow the process of game to make it look like animation
			if (KillItSelf(S))
			{
				system("cls");
				cout << "\n\n\t\t\t------>Game Over<------" << endl;
				cout << "\t\t\t\tScore is: "<<S.score<< endl; 
				break;
			}
		}
	} while (true);
}