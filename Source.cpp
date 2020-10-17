#include<stdio.h>
#include<conio.h>
#include"cc215.h"

//Colors
//Snake & Menu	: 9
//Border		: 14
//Food			: 12

struct snakePart {
	int x;
	int y;
	snakePart* next;
	snakePart* previous;
};
struct snake {
	snakePart* SnakeTail;
	snakePart* SnakeHead;
	snakePart Food;
	int size;
};

void setColor(int color) {
	// Changes Text Color	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

snakePart* InitialiseSnake1(snakePart* SnakePointer, int count) {
		//Initialises The Snake Body
	if (count == 0)
		return	SnakePointer;

	SnakePointer = (snakePart*)malloc(sizeof(snakePart));
	SnakePointer->next = NULL;

	SnakePointer->next = InitialiseSnake1(SnakePointer->next, count - 1);
	return SnakePointer;
}
snake InitialiseSnake2(snake Snake) {
		//More Snake Initialisations
		//Fixes The Previous Values
	snakePart *it = Snake.SnakeHead;
	while (it->next != NULL) {
		it->next->previous = it;
		it = it->next;
	}
		//Gets Snake Tail
	Snake.SnakeTail = it;

		//Gives Initial X , Y Values To Snake
	int x = 25, y = 6;
	it = Snake.SnakeHead;
	while (it != NULL) {
		it->x = x;
		it->y = y;
		x--;
		it = it->next;
	}
	return Snake;
}

snakePart PrintFood(snake Snake, snakePart Food) {
		//Prits The Snake Food
	srand(time(NULL));
	snakePart* it = Snake.SnakeHead;

	RandFood:
	Food.x = 1 + (rand() % (getScreenWidth() - 2));
	Food.y = 1 + (rand() % (getScreenHeight() - 2));

	while (it != NULL) {
		if (Food.x == it->x && Food.y == it->y)
			goto RandFood;

		it = it->next;
	}

	goto_xy(Food.x, Food.y);

	setColor(12);
	printf("%c", 254);
	setColor(9);

	return Food;
}

snake InitialiseSnake(snake Snake) {
		//Calls The Initialise Functions
	Snake.size = 20;
	Snake.SnakeHead = InitialiseSnake1(Snake.SnakeHead, Snake.size);
	Snake.SnakeHead->previous = NULL;
	Snake = InitialiseSnake2(Snake);
	Snake.Food = PrintFood(Snake, Snake.Food);
	return Snake;
}

void PrintSnakeInitial(snakePart* SnakeHead) {
		//Prints The Whole Snake
	snakePart *it = SnakeHead;

	while (it != NULL) {
		goto_xy(it->x, it->y);
		printf("%c", 219);
		it = it->next;
	}
}

snake MoveSnakeAndPrint(snake Snake, char Direction) {
		//Responsible For Moving The Snake

		//Makes A New Head
	snakePart* tmp = (snakePart*)malloc(sizeof(snakePart));
	tmp->previous = NULL;

		//Controls Direction
	switch (Direction) {
	case 'w':
		tmp->x = Snake.SnakeHead->x;
		tmp->y = Snake.SnakeHead->y - 1;
		break;

	case 'a':
		tmp->x = Snake.SnakeHead->x - 1;
		tmp->y = Snake.SnakeHead->y;
		break;

	case 's':
		tmp->x = Snake.SnakeHead->x;
		tmp->y = Snake.SnakeHead->y + 1;
		break;

	case  'd':
		tmp->x = Snake.SnakeHead->x + 1;
		tmp->y = Snake.SnakeHead->y;
		break;
	}

		//Murge Temp With Snake
	tmp->next = Snake.SnakeHead;
	Snake.SnakeHead->previous = tmp;
	Snake.SnakeHead = tmp;

		//Print The New Head
	goto_xy(tmp->x, tmp->y);
	printf("%c", 219);

		//Removes The Tail
	goto_xy(Snake.SnakeTail->x, Snake.SnakeTail->y);
	setColor(0);
	printf("%c",219);
	setColor(9);

	Snake.SnakeTail = Snake.SnakeTail->previous;
	free(Snake.SnakeTail->next);
	Snake.SnakeTail->next = NULL;

	return Snake;
}
char GetNewDirection(char DirectionOld) {
		//Manages New Snake Direction
	char DirectionNew = tolower(_getch());

		//Rejects Backing Into Self
	if (DirectionOld == 'a' && DirectionNew == 'd')
		return DirectionOld;
	else if (DirectionOld == 'd' && DirectionNew == 'a')
		return DirectionOld;
	else if (DirectionOld == 's' && DirectionNew == 'w')
		return DirectionOld;
	else if (DirectionOld == 'w' && DirectionNew == 's')
		return DirectionOld;

		//Makes Sure It's One Of The 4 Directions
	if (DirectionNew == 'a' || DirectionNew == 'w')
		return DirectionNew;
	else if (DirectionNew == 's' || DirectionNew == 'd')
		return DirectionNew;

	return DirectionOld;
}

int DetectCollision(snakePart* SnakeHead) {
		//Detects Collision With Sanke Or Edge
	if (SnakeHead->x == getScreenWidth() - 1)
		return 1;
	if (SnakeHead->x == 0 || SnakeHead->y == 0)
		return 1;
	if (SnakeHead->y == getScreenHeight() - 1)
		return 1;

		//Detect Collision With Snake
	snakePart *it = SnakeHead->next;
	while (it != NULL) {
		if (SnakeHead->x == it->x && SnakeHead->y == it->y)
			return 1;

		it = it->next;	
	}
	return 0;
}

int DetectCollisionBorderless(snakePart* SnakeHead) {
		//Detect Collision With Snake
	snakePart *it = SnakeHead->next;
	while (it != NULL) {
		if (SnakeHead->x == it->x && SnakeHead->y == it->y)
			return 1;
		it = it->next;
	}
}

snake MoveSnakeAndPrintBorderless(snake Snake, char Direction) {
	//Responsible For Moving The Snake

	//Makes A New Head
	snakePart* tmp = (snakePart*)malloc(sizeof(snakePart));
	tmp->previous = NULL;

	//Controls Direction
	switch (Direction) {
	case 'w':
		tmp->x = Snake.SnakeHead->x;
		if (Snake.SnakeHead->y == 0)
			tmp->y = getScreenHeight();
		else
			tmp->y = Snake.SnakeHead->y - 1;
		break;

	case 'a':
		if (Snake.SnakeHead->x == 0)
			tmp->x = getScreenWidth();
		else
			tmp->x = Snake.SnakeHead->x - 1;
		tmp->y = Snake.SnakeHead->y;
		break;

	case 's':
		tmp->x = Snake.SnakeHead->x;
		if (Snake.SnakeHead->y == getScreenHeight())
			tmp->y = 0;
		else
			tmp->y = Snake.SnakeHead->y + 1;
		break;

	case  'd':
		if (Snake.SnakeHead->x == getScreenWidth())
			tmp->x = 0;
		else
			tmp->x = Snake.SnakeHead->x + 1;
		tmp->y = Snake.SnakeHead->y;
		break;
	}

	//Murge Temp With Snake
	tmp->next = Snake.SnakeHead;
	Snake.SnakeHead->previous = tmp;
	Snake.SnakeHead = tmp;

	//Print The New Head
	goto_xy(tmp->x, tmp->y);
	printf("%c", 219);

	//Removes The Tail
	goto_xy(Snake.SnakeTail->x, Snake.SnakeTail->y);
	setColor(0);
	printf("%c", 219);
	setColor(9);

	Snake.SnakeTail = Snake.SnakeTail->previous;
	free(Snake.SnakeTail->next);
	Snake.SnakeTail->next = NULL;

	return Snake;
}

int Eat(snake Snake) {
		//Detects Collision With Food
	if (Snake.Food.x == Snake.SnakeHead->x)
		if (Snake.Food.y == Snake.SnakeHead->y)
			return 1;

	return 0;
}
snake MoveSnakeNoTail(snake Snake, char Direction) {
		//Responsible For Moving The Snake

		//Makes A New Head
	snakePart* tmp = (snakePart*)malloc(sizeof(snakePart));
	tmp->previous = NULL;

		//Controls Direction
	switch (Direction) {
	case 'w':
		tmp->x = Snake.SnakeHead->x;
		tmp->y = Snake.SnakeHead->y - 1;
		break;

	case 'a':
		tmp->x = Snake.SnakeHead->x - 1;
		tmp->y = Snake.SnakeHead->y;
		break;

	case 's':
		tmp->x = Snake.SnakeHead->x;
		tmp->y = Snake.SnakeHead->y + 1;
		break;

	case  'd':
		tmp->x = Snake.SnakeHead->x + 1;
		tmp->y = Snake.SnakeHead->y;
		break;
	}

		//Murge Temp With Snake
	tmp->next = Snake.SnakeHead;
	Snake.SnakeHead->previous = tmp;
	Snake.SnakeHead = tmp;

		//Print The New Head
	goto_xy(tmp->x, tmp->y);
	printf("%c", 219);

	return Snake;
}

void mainMenu() {
	//Prints Main Menu
	goto_xy(1, 1);
	char c;
	FILE* f;
	fopen_s(&f, "Main Menu.txt", "r");

	while ((c = fgetc(f)) != EOF)
		printf("%c", c);
	fclose(f);
}
void SnakeBoundriesDelay() {
	//Prints The Game Boundries
	goto_xy(0, 0);

	setColor(14);
	int x = getScreenWidth();
	int y = getScreenHeight();

	//Top
	for (int i = 0; i < x; i++) {
		printf("%c", 219);
		Sleep(5);
	}
	//Right
	for (int i = 0; i < y-1; i++) {
		goto_xy(x - 1, i);
		printf("%c", 219);
		Sleep(5);
	}
	//Bottom

	for (int i = x - 2; i > 0; i--) {
		goto_xy(i, y - 1);
		printf("%c", 219);
		Sleep(5);
	}
	//Left
	for (int i = y - 1; i > 0; i--) {
		goto_xy(0, i);
		printf("%c", 219);
		Sleep(5);
	}

	setColor(9);
}
void SnakeBoundriesInstant() {
	//Prints The Game Boundries
	setColor(14);

	int x = getScreenWidth();
	int y = getScreenHeight();

	//Top
	goto_xy(0,0);
	for (int i = 0; i < x+1; i++)
		printf("%c", 219);

	//Right And Left
	for (int i = 0; i < y - 1; i++) {
		goto_xy(x - 1, i);
		printf("%c\n%c", 219, 219);
	}

	//Bottom
	goto_xy(0,y-1);
	for (int i = 0; i < x ; i++)
		printf("%c", 219);

	setColor(9);
}

void PlayEndless() {

	int x = getScreenWidth();
	int y = getScreenHeight();

	system("cls");
	SnakeBoundriesInstant();

	char Direction = 'd';
	snake Snake;
	Snake.SnakeHead = NULL;
	Snake.SnakeTail = NULL;

	Snake = InitialiseSnake(Snake);
	PrintSnakeInitial(Snake.SnakeHead);

	while (1) {
		do {
			if (x != getScreenWidth() || y != getScreenHeight())
				return;
			//If The Snake Eats
			if (Eat(Snake) == 1) {
				Beep(2000,100);
				Snake.Food = PrintFood(Snake, Snake.Food);
				Snake = MoveSnakeNoTail(Snake, Direction);
				Snake.size++;
			}
			else
				Snake = MoveSnakeAndPrint(Snake, Direction);

			goto_xy(0, 0);
			Sleep(40);
			//Delay to Make It Same Speed For All Directions
			if (Direction == 'w' || Direction == 's')
				Sleep(30);

			//Collision Management
			if (DetectCollision(Snake.SnakeHead)) {
				goto_xy(55, 15);
				printf("You Lose");
				goto_xy(52, 16);
				printf("Your Score : %d", Snake.size - 20);
				_getch();
				return;
			}
			if (Snake.size == (x - 1 * y - 1)) {
				goto_xy(55, 15);
				printf("You Win");
				_getch();
				return;
			}
		} while (!_kbhit());

		Direction = GetNewDirection(Direction);
	}
}

snake RemoveTail(snake Snake) {

	//Removes The Tail
	goto_xy(Snake.SnakeTail->x, Snake.SnakeTail->y);
	setColor(0);
	printf("%c", 219);
	setColor(9);

	Snake.SnakeTail = Snake.SnakeTail->previous;
	free(Snake.SnakeTail->next);
	Snake.SnakeTail->next = NULL;
	
	return Snake;
}
void PlayEndlessHard() {
	int Timer = 0;
	int x = getScreenWidth();
	int y = getScreenHeight();

	system("cls");
	SnakeBoundriesInstant();

	char Direction = 'd';
	snake Snake;
	Snake.SnakeHead = NULL;
	Snake.SnakeTail = NULL;

	Snake = InitialiseSnake(Snake);
	PrintSnakeInitial(Snake.SnakeHead);
	while (1) {
		do {
			if (Timer++ == 10) {
				Timer = 0;
				Snake = RemoveTail(Snake);
				Snake.size--;
			}

			if (x != getScreenWidth() || y != getScreenHeight())
				return;
			//If The Snake Eats
			if (Eat(Snake) == 1) {
				Beep(2000, 100);
				Snake.Food = PrintFood(Snake, Snake.Food);
				Snake = MoveSnakeNoTail(Snake, Direction);
				Snake.size++;
			}
			else
				Snake = MoveSnakeAndPrint(Snake, Direction);

			goto_xy(0, 0);
			Sleep(40);
			//Delay to Make It Same Speed For All Directions
			if (Direction == 'w' || Direction == 's')
				Sleep(30);

			//Collision Management
			if (DetectCollision(Snake.SnakeHead) || Snake.size == 1) {
				goto_xy(55, 15);
				printf("You Lose");
				_getch();
				return;
			}
			if (Snake.size == (x - 1 * y - 1)) {
				goto_xy(55, 15);
				printf("You Win");
				_getch();
				return;
			}
		} while (!_kbhit());

		Direction = GetNewDirection(Direction);
	}
}

void PlayEndlessBorderless() {

	int x = getScreenWidth();
	int y = getScreenHeight();

	system("cls");

	char Direction = 'd';
	snake Snake;
	Snake.SnakeHead = NULL;
	Snake.SnakeTail = NULL;

	Snake = InitialiseSnake(Snake);
	PrintSnakeInitial(Snake.SnakeHead);

	while (1) {
		do {
			if (x != getScreenWidth() || y != getScreenHeight())
				return;

			//If The Snake Eats
			if (Eat(Snake) == 1) {
				Beep(2000, 100);
				Snake.Food = PrintFood(Snake, Snake.Food);
				Snake = MoveSnakeNoTail(Snake, Direction);
				Snake.size++;
			}
			else
				Snake = MoveSnakeAndPrintBorderless(Snake, Direction);

			Sleep(40);
			//Delay to Make It Same Speed For All Directions
			if (Direction == 'w' || Direction == 's')
				Sleep(30);

			//Collision Management
			if (DetectCollisionBorderless(Snake.SnakeHead)) {
				goto_xy(55, 15);
				printf("You Lose");
				_getch();
				return;
			}
			if (Snake.size == (x * y)) {
				goto_xy(55, 15);
				printf("You Win");
				_getch();
				return;
			}
		} while (!_kbhit());

		Direction = GetNewDirection(Direction);
	}
}

void play() {
	//Prints The Game Rules
	system("cls");

	SnakeBoundriesInstant();
	goto_xy(0, 0);

	char c;
	FILE* f;
	fopen_s(&f, "Play.txt", "r");

	while ((c = fgetc(f)) != EOF)
		printf("%c", c);
	_getch();
	c = tolower(_getch());

	switch (c) {
	case 'w':
		PlayEndless();
		break;

	case 'a':
		PlayEndlessHard();
		break;

	case 's':
		PlayEndlessBorderless();
		break;
	}
	system("cls");
}
void rules(){
		//Prints The Game Rules

	int x = getScreenWidth();
	int y = getScreenHeight();
	system("cls");

	char c;
	FILE* f;
	fopen_s(&f, "Rules.txt", "r");

	while ((c = fgetc(f)) != EOF)
		printf("%c", c);

	SnakeBoundriesInstant();

	_getch();
	system("cls");
}
void highScores() {
		//Prints The Current Highest Scores
	system("cls");

	char c;
	FILE* f;
	fopen_s(&f, "Highscores.txt", "r");

	while ((c = fgetc(f)) != EOF)
		printf("%c", c);
	SnakeBoundriesInstant();

	_getch();
	system("cls");
}
void goodbye() {
	system("cls");

	SnakeBoundriesInstant();
	goto_xy(0, 3);
	char c;
	FILE* f;
	fopen_s(&f, "Goodbye.txt", "r");

	while ((c = fgetc(f)) != EOF)
		printf("%c", c);
	fclose(f);
	Sleep(1000);
	exit(0);
}

void main() {
	int x = getScreenWidth();
	int y = getScreenHeight();
	char choice;

	//Animation
	SnakeBoundriesDelay();

	//The Main Menu
	while (1) {
		system("cls");
		SnakeBoundriesInstant();
		mainMenu();

		// If Consol Size Changes Mid-Game
		while (!_kbhit()) {
			if (x != getScreenWidth() || y != getScreenHeight()) {
				goto_xy(80, 13);
				printf("Detecting Console Changes");
				while (x != getScreenWidth() || y != getScreenHeight()) {
					x = getScreenWidth();
					y = getScreenHeight();
					Sleep(1500);
				}
				//Starts a New Game
				system("cls");
				SnakeBoundriesDelay();
				mainMenu();
			}
		}

		choice = tolower(_getch());
		//The 4 choices the player is given in the main menu
		switch (choice) {
		case 'w':
			//functions for plaiying
			play();
			break;

		case 'a':
			//Function of rules
			rules();
			break;

		case 's':
			//Function For HIGHSCORES
			highScores();
			break;

		case 'd':
			//Print Goodbye
			//Settings();
		default:
			goodbye();

		}
	}
	_getch();
}