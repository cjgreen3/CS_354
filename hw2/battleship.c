#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SHIP{
    int row, column, length;
    char direction;
} SHIP;

typedef struct ATTACK{
    int row, column;
} ATTACK;

// these are the prototype functions developed for our solution.  You are welcome to use them
// or modify them if you have a different strategy for solving this project.
void Read_Save_Game_File(char *filename, int *rows, int *columns, int *ship_count, SHIP **ships, int *attack_count, ATTACK **attacks);
void Initialize_Board(int rows, int columns, char **board);
void Print_Board(int rows, int columns, char *board);
void Add_Ships(int rows, int columns, char *board, int ship_count, SHIP *ships);
void Process_Attacks(int rows, int columns, char *board, int attack_count, ATTACK *attacks);
void Game_Over(int rows, int columns, char *board);

int main(int argc, char *argv[] ) {

    // verify command line parameters
    if(argc != 2){
        printf("expected usage: %s <save_game_file>", argv[0]);
        exit(1);
    }

    // declare variables - feel free to add additional variables as needed
    char *board =NULL;

    SHIP *ships = NULL;
    ATTACK *attacks = NULL;
    int rows=0, columns=0, ship_count=0, attack_count=0;

    // read the save game file
    Read_Save_Game_File(argv[1], &rows, &columns, &ship_count, &ships, &attack_count, &attacks);
    /* Note: Reading the file and using malloc to reserve space for
     * the data may be easier to do here, in main, rather than in a
     * function */
    //*board = malloc(rows*columns*sizeof(char);

    
    // Uncomment each function as you write and test it
    // Generate the board;
    
    Initialize_Board(rows, columns, &board);
    Print_Board(rows, columns, board);
    
    // Add Ships to the board;
    Add_Ships(rows, columns, board, ship_count, ships);
    Print_Board(rows, columns, board);
    
    // Process Attacks
    Process_Attacks(rows, columns, board, attack_count, attacks);
    Print_Board(rows, columns, board);

    Game_Over(rows, columns, board);

    // free memory
    free(board);
    free(ships);
    free(attacks);

    return 0;
}

////////////////////////////////////////////
// This function reads the file in and adds
// values to the parameters to all the variables
////////////////////////////////////////////
void Read_Save_Game_File(char *filename, int *rows, int *columns, int *ship_count, SHIP **ships, int *attack_count, ATTACK **attacks) {
    printf("Reading Save Game File\n");
    FILE *input = fopen(filename, "r");
    if(!input){
        printf("INPUT File open failed\n");
        exit(1);
    }

    // skip the board size line
    char skip_this_text[1000];
    if (!fgets(skip_this_text, 1000, input)) {
        printf("Error reading board size");
        exit(1);}

    // read the board size
    fscanf(input, "%d", rows);
    fscanf(input, "%d", columns);
    fgets(skip_this_text, 1000, input); // read the newline character
    printf("Board Size = (%d, %d)\n",*rows, *columns);

    /*  Complete the rest of the function below - or move the code above to main */

    if (!fgets(skip_this_text, 1000, input)) {
        printf("Error reading number of ships" );
        exit(1);}
    fscanf(input, "%d", ship_count);
    printf("Number of Ships = %d\n", *ship_count);
    *ships = malloc(*ship_count*sizeof(SHIP));
    SHIP  *ship = *ships;

    for(int i  = 0; i<*ship_count; i++){

    fscanf(input, "%d",&ship->row);
    fscanf(input, "%d",&ship->column);
    fscanf(input, "%d",&ship->length);
    fscanf(input," %c",&ship->direction);
    printf("Ship %d: (%d, %d), length = %d, direction %c\n", i, ship->row, ship->column,ship->length,ship->direction);
  
    (ship++);
    }
    if (!fgets(skip_this_text, 1000, input)) {
      printf("Error reading attack count");
      exit(1);}

    if (!fgets(skip_this_text, 1000, input)) {
      printf("Error reading attack count");
      exit(1);}

    fscanf(input, "%d", attack_count);
    printf("Number of Attacks = %d\n", *attack_count);
    *attacks  = malloc(*attack_count*sizeof(ATTACK));
    ATTACK *attack = *attacks;
    for(int i = 0; i<*attack_count;i++){
    fscanf(input, "%d",&attack->row);
    fscanf(input, "%d",&attack->column);
    printf("Attack %d: (%d, %d)\n",i, attack->row, attack->column);
    (attack++);
    }


    fclose(input);
}

////////////////////////////////////////////
// This function creates a board of '.' of rows x columns
////////////////////////////////////////////
void Initialize_Board(int rows, int columns, char **board){ 
   
  printf("\n"); 
  printf("Initializing Board\n"); 
  *board = malloc(rows * columns * sizeof(char*)); 

  for(int i = 0; i < rows; i++){
    for(int j = 0; j< columns; j++){
   //m[i][j] =  *(m + (cols *i + j )* sizeof(int))
     *(*board +(columns * i + j )) = '.'; 
    }
   }
}

////////////////////////////////////////////
// This function prints the board and is used to
// print all instances of the board
////////////////////////////////////////////
void Print_Board(int rows, int columns, char *board){

 for (int i = 0; i < rows; i++){
    for(int j = 0; j < columns; j++){
     printf("%c",*(board +columns * i + j)); 
    }
    printf("\n");
 }
}
////////////////////////////////////////////
// This function adds ships to the board 
//  more implementation details can be seen in
//comments below
////////////////////////////////////////////
void Add_Ships(int rows, int columns, char *board, int ship_count, SHIP *ships){
  printf("\n"); 
  printf("Adding Ships\n"); 
  SHIP ship = {};
  SHIP *temp_ship = ships; 
  for(int i = 0; i<ship_count;i++){
    ship =*temp_ship;

    //check first if ship's first position overlaps
    if(*(board +(columns *ship.row+ship.column )) == 'S'){  
      printf("Ship %d overlaps another ship - skipping\n", i);
      temp_ship++;
      continue;
    }
    //out of bounds cases
    if(( ship.row <  0 || ship.column < 0)) {
      printf("Ship %d is out of bounds - skipping\n",i);
      temp_ship++; 
      continue;
     }
     if( (ship.row >= rows || ship.column >= columns)){
      printf("Ship %d is out of bounds - skipping\n",i);
      temp_ship++;
      continue;
     }
    if(ship.direction == 'H'){
      if(ship.column + ship.length > columns){
       printf("Ship %d is out of bounds - skipping\n",i);
       temp_ship++;
       continue;
      }
    }
    if(ship.direction == 'V'){
      if(ship.row + ship.length > rows){
       printf("Ship %d is out of bounds - skipping\n",i);
       temp_ship++;
       continue; 
      }
    }


    //creates a flag varible to check if the rest of ship overlaps with S
    //if the flag is false then don't add any of the ship
    //false = 0 true = 1
    int flag = 0;
    if(ship.length == 1){
       if(*(board +(columns * ship.row + ship.column )) != 'S'){
         flag = 1;
       }
    }
    if(ship.direction == 'V'){
      for(int l = 1; l< ship.length; l++){

       if(*(board +(columns * ship.row + ship.column + columns * l )) == 'S'){
         flag = 0;
         break;
       }else{
        flag = 1; 
       }
     }
    }
    if(ship.direction == 'H'){
      for(int x = 1; x< ship.length; x++){
        if(*(board +(columns* ship.row + x + ship.column)) == 'S'){
         flag =0;
         break;
        }else{
        flag = 1;
        }
     }
    }
  
   // if(flag == 1){
   //     *(board +(columns *ship.row+ship.column )) = 'S';
       // printf("JUST ADDED A SHIP %d\n",ship.length);
   // }
    //adding addtional ships in v or h
     if(flag ==0 ){
         printf("Ship %d overlaps another ship - skipping\n", i);
         temp_ship++;
         continue;
    }
    if(ship.direction == 'V'){
      for(int l = 0; l< ship.length; l++){

       *(board +(columns * ship.row + ship.column + columns * l )) = 'S';
     }
    }
    if(ship.direction == 'H'){
      for(int x = 0; x< ship.length; x++){
          *(board +(columns*ship.row + x + ship.column)) = 'S';
     }
    }
    temp_ship++;
  }
}
////////////////////////////////////////////
// This function processes attacks similar 
// adding ships function with less cases
// If an attack misses mark the board with 'M'
// If an attack hits a ship mark the board with 'H"
// Like ships, Attacks can also be out of bounds
////////////////////////////////////////////
void Process_Attacks(int rows, int columns, char *board, int attack_count, ATTACK *attacks){
  printf("\n");
  printf("Processing Attacks\n"); 
  ATTACK attack = {};
  ATTACK *temp_attack = attacks;
  for(int i = 0; i<attack_count; i++){
    attack = *temp_attack; 
    if(( attack.row <  0 || attack.row > rows) ||
       (attack.column < 0 || attack.column >  columns)){
      printf("Attack %d is out of bounds - skipping\n",i);
      temp_attack++;
      continue;
    }
    if(*(board +(columns * attack.row + attack.column )) == '.'){
    *(board +(columns * attack.row + attack.column )) = 'M';
    }
    if(*(board +(columns * attack.row + attack.column )) == 'S'){
    *(board +(columns * attack.row + attack.column )) = 'H';
    }
  temp_attack++;
  }


}
////////////////////////////////////////////
// This function ends the game. If any ships remain
//continue the game
// Else the game is over and all ships have been sunk
////////////////////////////////////////////
void Game_Over(int rows, int columns, char *board){
 printf("\n");
  for(int i = 0; i < rows; i++){
    for(int j = 0; j< columns; j++){
   //m[i][j] =  *(m + (cols *i + j )* sizeof(int))
    if( *(board +(columns * i + j ))== 'S'){
     printf("Ships remain afloat - game on\n");
     return; 
    }
    }
   }
  printf("All ships have been sunk - game over\n");

}
