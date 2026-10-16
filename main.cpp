/*
* Project: Escape the Maze
* Concepts: C++ Basics and Datastructures (2D Vector, Stack) 
*/
#include <iostream>
#include <vector>
#include <stack>
#include <cctype>
using namespace std;

//Print the maze on console
void displayMaze(const vector<vector<char>>& map){
    for(auto row : map){
        for(char ch : row){
            cout << ch << " ";
        }
        cout << endl;
    }
}

//Display player stats (moves, undos, lives, status)
void displayStatsHud(int moves_left,int undoUsed,string status,int lives){
    cout << "[ Moves Remaining: "<< moves_left <<" / 30 ]  |  [ Undos Used: "<< undoUsed <<" ]  |  [ Lives: "<< lives << " ]  |  [ Status: "<< status <<" ]\n";
}

int main(){
    //Print game's title and game's instructions
    cout << "================ ESCAPE THE MAZE ================\n\n"
            "===================== GUIDE =====================\n"  
            "+-----------------------+-----------------------+\n"
            "|        CONTROLS       |        LEGEND         |\n"
            "+-----------------------+-----------------------+\n"
            "|  [W/w] Move Up          |  'p'  Player          |\n"
            "|  [S/s] Move Down        |  '#'  Solid Wall      |\n"
            "|  [A/a] Move Left        |  '.'  Safe Path       |\n"
            "|  [D/a] Move Right       |  'X'  Trap            |\n"
            "|  [U/u] Undo Step        |  'E'  Exit            |\n"
            "|  [Q/q] Quit Game        |                       |\n"
            "+-----------------------+-----------------------+\n";

    //9x13 maze layout
    vector<vector<char>> map = {{'#','#','#','#','#','#','#','#','#','#','#','#','#'},
                                {'#','p','.','.','#','.','.','.','#','.','.','.','#'},
                                {'#','#','#','.','#','.','#','.','#','X','#','#','#'},
                                {'#','.','X','.','.','.','#','.','.','.','#','.','#'},
                                {'#','.','#','#','#','.','#','#','#','.','#','.','#'},
                                {'#','.','#','.','.','.','.','X','#','.','.','.','#'},
                                {'#','.','#','X','#','#','#','.','#','#','#','.','#'},
                                {'#','.','.','.','#','.','X','.','.','.','X','E','#'},
                                {'#','#','#','#','#','#','#','#','#','#','#','#','#'}};
    //Player setup
    int player_row = 1;
    int player_col = 1;
    int moves_left = 30;
    int undoUsed = 0;
    int lives = 3;
    string status = "Healthy";

    //Stacks to remember the previous coordinates for undo feature
    stack<int> row_history,col_history;
    
    //Main game loop
    while(true){
        //Show HUD and Maze
        displayStatsHud(moves_left,undoUsed,status,lives);
        displayMaze(map);

        //Get player input and convert it to uppercase if needed
        char mov_ch;
        int target_row,target_col;
        cout << "ENTER CMD character [W/A/S/D | U: Undo | Q: Quit] >> ";
        cin >> mov_ch;
        mov_ch = toupper(mov_ch);
        
        //Undo Command
        if(mov_ch == 'U'){
            if(row_history.empty()){
                cout << "\n[Notice] Cannot undo: You are already at your starting position.\n";
                continue;
            }
            map[player_row][player_col] = '.';
            player_row = row_history.top();
            player_col = col_history.top();
            row_history.pop();
            col_history.pop();
            map[player_row][player_col] = 'p';
            moves_left--;
            undoUsed++;
            continue;
        }

        //Exit Command
        else if(mov_ch == 'Q'){
            cout << "Game exited, see you next time!";
            break;
        }

        //Calculate target Coordinates
        switch(mov_ch){
            case 'W':
                target_row = player_row - 1;
                target_col = player_col;
                break;
            case 'S':
                target_row = player_row + 1;
                target_col = player_col;
                break;
            case 'A':
                target_row = player_row;
                target_col = player_col - 1;
                break;
            case 'D':
                target_row = player_row;
                target_col = player_col + 1;
                break;
            default:
                cout << "\n[Invalid] Please enter W/w, A/a, S/s, D/d, U/u, or Q/q.\n";
                continue;
        }

        //Check if target goes outsid the maze
        if( !(target_row >= 0 && target_row < 9 && target_col >= 0 &&  target_col < 13) ){

            cout << "\n[Boundary] You cannot walk beyond the edge of the maze.\n";
            continue;
        }
        //Check if the player hits the wall
        else if(map[target_row][target_col] == '#'){
            char tryAgain;
            cout << "\n[Blocked] That path is blocked by a solid wall.\n";
            continue;
        }
        //Handle valid move
        else{
            //Save current player's positon before moving
            row_history.push(player_row);
            col_history.push(player_col);

            //Remember what tile we stepped on before overwritting it with 'p'
            char ground = map[target_row][target_col];

            //Move player to the new position
            map[player_row][player_col] = '.';
            player_row = target_row;
            player_col = target_col;
            map[player_row][player_col] = 'p';
            moves_left--;

            //If player steps on a trap
            if(ground == 'X'){
                lives--;
                if(lives == 2){
                    cout << "\nYou fall into a trap and you lost a life!\n";
                }
                else if(lives == 1){
                    status = "Injured";
                    cout << "\nYou fall into a trap and you lost a life!\n";
                }
                else if(lives == 0){
                    status = "Dead";
                    displayStatsHud(moves_left,undoUsed,status,lives);
                    displayMaze(map);
                    cout << "\nYou fall into a trap and you lost all your lives!\n";
                    cout << "Game over";
                    break;
                }
            }

            //If player reaches the exit
            if(ground == 'E'){
                displayStatsHud(moves_left,undoUsed,status,lives);
                displayMaze(map);
                cout << "Congratulation!, you won.\n";
                break;
            }
        }

        //Check if player ran out of moves
        if(moves_left == 0){
            cout << "\nPlayer ran out of stamina.\n";
            cout << "Game over";
            break;
        }
    }
    return 0;
}