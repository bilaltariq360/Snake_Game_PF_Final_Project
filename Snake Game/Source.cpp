//all libraries used in this program
#include<iostream>//for input and output
#include<fstream>//for file handeling
#include<string>//for string
#include<cstdlib>//for generating random function
#include<conio.h>//for _kbhit and _getch functions
#include<time.h>//for time to use srand function
using namespace std;

//all global variables used in this program
bool game_over;//for game status
const int boundary_width = 70;//for boundary width
const int boundary_height = 20;//for boundary height
int snakeHead_xaxis, snakeHead_yaxis, food_xaxis, food_yaxis, score, level_select;
int snakeTail_xaxis[100], snakeTail_yaxis[100];//for snake tail
int nSize_snakeTail;//for managing snake length
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };//for setting directions
eDirecton direction;//a variable for input direction

//a function to display menu
void menu() {
    cout << "------------------------------------ Welcome to snake game ------------------------------------\n";
    cout << "1. Press 1 to check rules and regulations\n";
    cout << "2. Press 2 to check highest score\n";
    cout << "3. Press 3 to play game\n";
    cout << "4. Exit\n\n";
}

//a function to diplay rules and regulations
void rules() {
    cout << "------------------------------------ Rules and regulations of game ------------------------------------\n";
    cout << "1. There is boundary, if your snake touches to boundary then game will over.\n";
    cout << "2. When the game starts there you see food like + if you eat it then your score will be added.\n";
    cout << "3. The length of snake will gradually increase as you feed snake.\n\n";
}

//a function to display levels
int levels() {
    cout << "Press 1 to select level 1\n";
    cout << "Press 2 to select level 2\n";
    cout << "Press 3 to select level 3\n";
    cout << "Press 4 to select level 4\n";
    cout << "Press 5 to select level 5\n\n";

    //do while loop to check validation of level selection
    do {
        cout << "Select level from above = ";
        cin >> level_select;
        if (level_select < 1 || level_select > 5) {
            cout << "OOPS! Invalid level select\n\n";
        }
    } while (level_select < 1 || level_select > 5);
    return level_select;
}

//a function to display highest score
void score_output() {
    int get_score, highest_score = 0;
    ifstream score_file;
    score_file.open("Highest score.txt");
    if (!score_file.is_open()) {
        cout << "OOPS! we cannot display highest score at this moment. Please try again\n";
    }
    else {
        //while loop to read from file
        while (!score_file.eof()) {
            score_file >> get_score;
            if (get_score == score_file.eof()) {
                break;
            }
            else if (get_score > highest_score) {
                highest_score = get_score;
            }
        }
        cout << "Highest score of this game = " << highest_score << endl << endl;
        score_file.close();
    }
}

//a function to save score in file
void score_input(int a) {
    ofstream score_file;
    score_file.open("Highest score.txt", ios::app);
    if (!score_file.is_open()) {
        cout << "OOPS! we cannot display highest score at this moment. Please try again\n";
    }
    else {
        score_file << a << ' ';
    }
    score_file.close();
}

//a function to setup snake and food initially
void initial_setup_snake(){
    game_over = false;//game is not over initially
    direction = STOP;//initially snake is at rest
    snakeHead_xaxis = boundary_width / 2;//to put snake horizontally center from xaxis boundary
    snakeHead_yaxis = boundary_height / 2;//to put snake vertically center from yaxis boundary
    food_xaxis = rand() % boundary_width;//to initially set food randomly within the boundary width limit
    food_yaxis = rand() % boundary_height;//to initially set food randomly within the boundary height limit
    score = 0;
}

//a function to draw boundary
void draw_boundary(){
    system("cls");
    cout << endl;
    cout << " ";
    for (int i = 0; i < boundary_width + 2; i++)
        cout << "-";
    cout << endl;

    for (int i = 0; i < boundary_height; i++){ //display boundary vertically
        for (int j = 0; j < boundary_width; j++){ //display boundary horizontally
            if (j == 0)
                cout << " |";
            if (i == snakeHead_yaxis && j == snakeHead_xaxis)
                cout << "O";
            else if (i == food_yaxis && j == food_xaxis)
                cout << "+";
            else{
                bool print = false;
                for (int k = 0; k < nSize_snakeTail; k++){
                    if (snakeTail_xaxis[k] == j && snakeTail_yaxis[k] == i){
                        cout << "s";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }

            if (j == boundary_width - 1)
                cout << "|";
        }
        cout << endl;
    }

    cout << " ";
    for (int i = 0; i < boundary_width + 2; i++)
        cout << "-";
    cout << endl;
    cout << " Score:" << score << endl;
}

//a function to take input from user
void user_input(){
    if (_kbhit()){
        switch (_getch()){
        case 75:
        case 'a':
            direction = LEFT;
            break;
        case 77:
        case 'd':
            direction = RIGHT;
            break;
        case 72:
        case 'w':
            direction = UP;
            break;
        case 80:
        case 's':
            direction = DOWN;
            break;
        case ' ':
            cout << "\nPress any key from keyboard to resume game!\n\n";
            system("pause");
            break;
        case 'e':
            game_over = true;
            break;
        default:
            break;
        }
    }
}

//a function to update snake status and manage level or speed of game
void update_snakePosition(){
    int prev_xaxis = snakeTail_xaxis[0];//array of 0 index of tail of snake on xaxis is assigned to temporary variable
    int prev_yaxis = snakeTail_yaxis[0];//array of 0 index of tail of snake on yaxis is assigned to temporary variable
    int prev_xaxis1, prev_yaxis1;
    snakeTail_xaxis[0] = snakeHead_xaxis;//xaxis of snake head is assigned to xaxis of snake tail
    snakeTail_yaxis[0] = snakeHead_yaxis;//yaxis of snake head is assigned to yaxis of snake tail
    for (int i = 1; i < nSize_snakeTail; i++){//for loop to assigned to xaxis and yaxis of snake tail to snake head
        prev_xaxis1 = snakeTail_xaxis[i];
        prev_yaxis1 = snakeTail_yaxis[i];
        snakeTail_xaxis[i] = prev_xaxis;
        snakeTail_yaxis[i] = prev_yaxis;
        prev_xaxis = prev_xaxis1;
        prev_yaxis = prev_yaxis1;
    }
    //switch statement to control speed of game and movement of snake
    switch (direction){
    case LEFT:
        snakeHead_xaxis -= level_select;//to move snake to left side
        break;
    case RIGHT:
        snakeHead_xaxis += level_select;//to move snake to right side
        break;
    case UP:
        snakeHead_yaxis -= level_select;//to move snake to upward side
        break;
    case DOWN:
        snakeHead_yaxis += level_select;//to move snake to downward side
        break;
    default:
        break;
    }
    //if condition to check game status
    if (snakeHead_xaxis > boundary_width || snakeHead_xaxis < 0 || snakeHead_yaxis > boundary_height || snakeHead_yaxis < 0)
        game_over = true;

    //for loop and if condition to check game status
    for (int i = 0; i < nSize_snakeTail; i++)
        if (snakeTail_xaxis[i] == snakeHead_xaxis && snakeTail_yaxis[i] == snakeHead_yaxis)
            game_over = true;

    //if condition to to update score and update snake food
    if (snakeHead_xaxis == food_xaxis && snakeHead_yaxis == food_yaxis){
        srand(time(0)); //random food value for random based on time
        score += 10;
        food_xaxis = rand() % boundary_width;
        food_yaxis = rand() % boundary_height;
        nSize_snakeTail++;
    }
}

int main(){
    //to change font color of whole console
    system("color B");

    //all data types and variables used in this program
    int option;
    string goTo;
    srand(time(0));

    //do while loop to do run program again and again untill user press 4 "Exit"
    do {
        system("cls"); 

        //to display menu
        menu();

        //do while loop to check option
        do {
            //to select from menu
            cout << "Enter option from above = ";
            cin >> option;
            if (option < 0 || option > 4) {
                cout << "OOPS! you have entered invalid option\n\n";
            }
        } while (option < 0 || option > 4);

        if (option == 1) {
            system("cls");
            rules();

            //to go back to menu
            cout << "Press any key to go back to menu = ";
            cin >> goTo;
            system("cls");
            menu();
        }

        //to display highest score of game
        if (option == 2) {
            system("cls");
            score_output();

            //to go back to menu
            cout << "Press any key to go back to menu = ";
            cin >> goTo;
            system("cls");
            menu();
        }

        //to display game mode
        if (option == 3) {
            system("cls");
            levels();
            system("cls");
            initial_setup_snake();

            //while loop to run untill game overs
            while (!game_over){
                draw_boundary();
                user_input();
                update_snakePosition();
            }
            score_input(score);

            //to go back to menu
            cout << "Press any key to go back to menu = ";
            cin >> goTo;
            system("cls");
            menu();
        }
    } while (option != 4);




    cout << endl;
    system("pause");
    return 0;
}