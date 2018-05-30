

using namespace std;
#include <stdlib.h>
#include <iostream>
#include <glut.h>
#include <vector>
#include <math.h>
#include <string>
#include <time.h>
#include <cstdlib>
#include <Windows.h>

/*Defining OpenGL section components*/
#define pi 3.14159265358979

void startGFX(void);
void drawCircle(float radius_p, float center_cx, float center_cy);

struct cell_center_param {
	float position_x;
	float position_y;
	float array_index_row;
	float array_index_col;
};

vector<cell_center_param> cell_center(42);

float circle_center_x = 0, circle_center_y = 0;
float circle_radius = 25;
/*Endof*/

/*Game components*/
//Initializes the board's matrix with empty spaces
char board[6][7] = {
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
};

//variable for ASCII codes for symbols (chips)
int ascii;

//the spot (column) which the players picks to play in
short spot;

short cpu_spot;

bool first_round = true;

//whose turn it is
bool p1 = 1, cpu = 0;

//assigns a name to player1
string p_1;

/*=============================================================*/


//function to take player's input
void play();
void drawBoard();
void winnerCheck();
bool checkforspace(int &r);
//A counter that when reaches 41, declares the game as a draw.
int draw = -1;
/*Endof*/

/*OpenGL Functions*/
//<<<<<<<<<<<<<<<<<<<<<<< OpenGL myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0); // set white background color
	glColor3f(0.0f, 0.0f, 0.0f); // set the drawing color
	glPointSize(4.0); // a ‘dot’ is 4 by 4 pixels
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

//<<<<<<<<<<<<<<<<<<<<<<<< OpenGL myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
	
								  
	// Draw main board
	glBegin(GL_QUADS); 
	glColor3f(0.168f, 0.294f, 0.9f);
	glVertex2i(20, 63);
	glVertex2i(540, 63);
	glVertex2i(540, 410);
	glVertex2i(20, 410);
	glEnd();


	int current_cell = -1;
	for (int i = 1; i <= 7; i++)
	{
		circle_center_y = 425.7; 
		for (int k = 1; k <= 6; k++)
		{
			current_cell += 1;

			glBegin(GL_TRIANGLE_FAN);
			glColor3f(1.0f, 1.0f, 1.0f);

			
			circle_center_x = 70 * i; 
			circle_center_y -= 54;

			//Set circle center vertex
			glVertex2f(circle_center_x, circle_center_y);

			//append position of the circle into vector
			cell_center[current_cell].position_x = circle_center_x;
			cell_center[current_cell].position_y = circle_center_y;
			cell_center[current_cell].array_index_row = k - 1;
			cell_center[current_cell].array_index_col = i - 1;

			//Draw circle
			drawCircle(circle_radius, circle_center_x, circle_center_y);
			glEnd();
			glFlush();
		}
	}
	drawBoard();
}
/*Endof*/



//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char** argv)
{
	glutInit(&argc, argv); // initialize the toolkit
	cout << "PLAYER 1 (" << char(178) << "): Type a name!\n";
	cin >> p_1;
	cout << "\nHello, (" << p_1 << ")! Enjoy!";
	startGFX();
}

void startGFX(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowSize(640, 480); // set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("points"); // open the screen window
	glutDisplayFunc(myDisplay); // register redraw function
	myInit();
	glutMainLoop(); // go into a perpetual loop
}


void drawCircle(float radius_p, float center_cx, float center_cy)
{
	for (int j = 0; j <= 20; j++)
	{
		glVertex2f(center_cx + (cos(j * 2 * pi / 20) * radius_p), center_cy + (sin(j * 2 * pi / 20) * radius_p));
	}
}


int cell_index_lookup(int x, int y)
{
	for (int i = 0; i < 42; i++)
	{
		if (cell_center[i].array_index_row == x && cell_center[i].array_index_col == y)
		{
			return i;
		}
	}
}


/*Game Logic*/
void drawBoard()
{
	// clears the screen
	cout << string(100, '\n');
	cout << endl;

	//Draws the board
	for (int r = 0; r < 6; r++)
	{
		for (int c = 0; c < 7; c++)
		{
			cout << "[" << board[r][c] << "]\t";
		}
		cout << endl;
	}

	//Prints the number of each respective column
	for (int r = 1; r <= 7; r++)
	{
		cout << " " << r << "\t";
	}

	//Displays where the last move was played
	if (first_round == false)
	{
		if (p1 == 1)
		{
			cout << "\n\n" << p_1 << " (" << char(178) << ") played in column " << spot << "\n";
			cout << "\n\nCPU" << " (" << char(177) << ") played in column " << cpu_spot << "\n";
		}
	}
	winnerCheck();
	draw++;
	play();
}

void play()
{
	bool error = false;
	int cell_index;
	//Checks if it is Player 1's turn
	if (p1 == 1)
	{
		ascii = 178;
		cout << "\n\n==== " << p_1 << "'s TURN " << "[" << char(178) << "] ====\n";
		do
		{
			error = false;
			cout << "Choose a column (1~7) to play in: ";
			cin >> spot;

			if (spot < 1 || spot > 7)
			{
				error = true;
				cout << "\nYou must choose a number between 1 and 7!!\n";
				Sleep(1000);
			}

			if (error == false)
			{
				for (int r = 5; r >= 0;)
				{
					if (board[r][spot - 1] != ' ') //Checks if the row below is empty, if it is not, it picks the row above.
						r--;
					else
					{
						board[r][spot - 1] = ascii; //Plays the chip in the selected column
						cell_index = cell_index_lookup(r, spot - 1);
						glBegin(GL_TRIANGLE_FAN);
						glColor3f(0.92f, 0.9f, 0.05f);

						//Set circle center vertex
						glVertex2f(cell_center[cell_index].position_x, cell_center[cell_index].position_y);
						drawCircle(circle_radius, cell_center[cell_index].position_x, cell_center[cell_index].position_y);
						glEnd();
						glFlush();

						//Alternates between players
						if (p1 == 1)
						{
							p1 = 0;
							cpu = 1;
						}
						else if (cpu == 1)
						{
							cpu = 0;
							p1 = 1;
						}
						first_round = false;
						drawBoard();
						break;
					}

				}
			}

		} while (p1 == 1 && (spot < 1 || spot > 7));
	}

	else if (cpu == 1)
	{
		srand(time(NULL));
		ascii = 177;
		tryagain:
		cpu_spot = 1 + rand() % 7; //Generates a random number from 1 to 7 where the dumb AI will play

		for (int r = 5; r >= 0;)
		{
			if (board[r][cpu_spot - 1] != ' ') {
				r--;
				if (r == 0 && board[r][cpu_spot - 1] != ' ')
					goto tryagain;
			}
			else
			{
				board[r][cpu_spot - 1] = ascii;

				cell_index = cell_index_lookup(r, cpu_spot - 1);
				glBegin(GL_TRIANGLE_FAN);
				glColor3f(1.0f, 0.0f, 0.0f);

				//Set circle center vertex
				glVertex2f(cell_center[cell_index].position_x, cell_center[cell_index].position_y);
				drawCircle(circle_radius, cell_center[cell_index].position_x, cell_center[cell_index].position_y);
				glEnd();
				glFlush();

				if (p1 == 1)
				{
					p1 = 0;
					cpu = 1;
				}
				else if (cpu == 1)
				{
					cpu = 0;
					p1 = 1;
				}
				drawBoard();
				break;
			}
		}
	}
}

void winnerCheck()
{
	//Checks all rows for any 4-in-a-row connections
	for (int row = 5; row >= 0; row--)
	{
		for (int col = 0, c_1, c_2, c_3; col < 4;)
		{
			c_1 = 1 + col;
			c_2 = 2 + col;
			c_3 = 3 + col;

			if (board[row][col] == char(178) && board[row][c_1] == char(178) && board[row][c_2] == char(178) && board[row][c_3] == char(178))
			{
				cout << "\n\n" << p_1 << " WON!\n";
				exit(0);
			}
			else if (board[row][col] == char(177) && board[row][c_1] == char(177) && board[row][c_2] == char(177) && board[row][c_3] == char(177))
			{
				cout << "\n\nCPU WON!\n";
				exit(0);
			}
			else col++;
		}
	}

	//Checks all columns for any 4-in-a-row connections
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5, r_1, r_2, r_3; row >= 3;)
		{
			r_1 = row - 1;
			r_2 = row - 2;
			r_3 = row - 3;

			if (board[row][col] == char(178)
				&& board[r_1][col] == char(178)
				&& board[r_2][col] == char(178)
				&& board[r_3][col] == char(178))
			{
				cout << "\n\n" << p_1 << " WON!\n";
				exit(0);
			}
			else if (board[row][col] == char(177)
				&& board[r_1][col] == char(177)
				&& board[r_2][col] == char(177)
				&& board[r_3][col] == char(177))
			{
				cout << "\n\nCPU WON!\n";
				exit(0);
			}
			else row--;
		}
	}

	//Checks all possible diagonal 4-in-a-row connections
	for (int row = 3, r_1, r_2, r_3; row < 6; row++)
	{
		r_1 = row - 1;
		r_2 = row - 2;
		r_3 = row - 3;
		for (int col = 0, c_1, c_2, c_3; col < 4;)
		{
			c_1 = 1 + col;
			c_2 = 2 + col;
			c_3 = 3 + col;

			if (board[row][col] == char(178)
				&& board[r_1][c_1] == char(178)
				&& board[r_2][c_2] == char(178)
				&& board[r_3][c_3] == char(178))
			{
				cout << "\n\n" << p_1 << " WON!\n";
				exit(0);
			}
			else if (board[row][col] == char(177)
				&& board[r_1][c_1] == char(177)
				&& board[r_2][c_2] == char(177)
				&& board[r_3][c_3] == char(177))
			{
				cout << "\n\nCPU WON!\n";
				exit(0);
			}
			else col++;
		}
	}

	//Another one that checks all the other possible diagonal 4-in-a-row connections
	for (int row = 3, r_1, r_2, r_3; row < 6; row++)
	{
		r_1 = row - 1;
		r_2 = row - 2;
		r_3 = row - 3;
		for (int col = 3, c_1, c_2, c_3; col < 7;)
		{
			c_1 = col - 1;
			c_2 = col - 2;
			c_3 = col - 3;

			if (board[row][col] == char(178)
				&& board[r_1][c_1] == char(178)
				&& board[r_2][c_2] == char(178)
				&& board[r_3][c_3] == char(178))
			{
				cout << "\n\n" << p_1 << " WON!\n";
				Sleep(3000);
				exit(0);
			}
			else if (board[row][col] == char(177)
				&& board[r_1][c_1] == char(177)
				&& board[r_2][c_2] == char(177)
				&& board[r_3][c_3] == char(177))
			{
				cout << "\n\nCPU WON!\n";
				Sleep(3000);
				exit(0);
			}
			else col++;
		}
	}

	if (draw == 41)
	{
		cout << "\nDraw!\n";
		//Sleep(3000);
		system("Pause");
		exit(0);
	}
}
/*EndofGa5meLogic*/
