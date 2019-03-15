#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAZE_SIZE 10
#define ROBOT_SYMBOL 2
#define EMPTY_SYMBOL 0
#define WALL_SYMBOL 1
#define GOAL_SYMBOL 7
#define FACING_UP 0
#define FACING_DOWN 1
#define FACING_RIGHT 2
#define FACING_LEFT 3

// ********** EMPTY INTERNAL MAZE REPRESENTATION ********** //
int goal_coords(int i, int j, int maze_size)
{
    int center = (maze_size - 1) / 2; // lesser center

    return center - 1 < i && i < center + 2 &&
           center - 1 < j && j < center + 2;
}

void update_mazes(int robot_pos_row,
                  int robot_pos_col,
                  char a[MAZE_SIZE][MAZE_SIZE],
                  char b[MAZE_SIZE][MAZE_SIZE])
{
    a[robot_pos_row][robot_pos_col] = ROBOT_SYMBOL;
    b[robot_pos_row][robot_pos_col] = ROBOT_SYMBOL;
}

void print_boards(char a[MAZE_SIZE][MAZE_SIZE], char b[MAZE_SIZE][MAZE_SIZE])
{
    // Displays mazes side-by-size with a 5 space gap
    const int GAP_SIZE = 5;
    printf("INTERNAL\tACTUAL\n");
    for (int row = 0; row < MAZE_SIZE; ++row)
    {
        for (int col = 0; col < MAZE_SIZE + MAZE_SIZE + GAP_SIZE; ++col)
        {

            if (col < MAZE_SIZE)
            {
                printf("%d", a[row][col]);
            }
            else if (col >= MAZE_SIZE && col < (MAZE_SIZE + GAP_SIZE))
            {
                printf(" ");
            }
            else
            {
                printf("%d", b[row][col - (MAZE_SIZE + GAP_SIZE)]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void init_maze(char maze[MAZE_SIZE][MAZE_SIZE])
{
    for (int i = 0; i < MAZE_SIZE; ++i)
    {
        for (int j = 0; j < MAZE_SIZE; ++j)
        {
            if (i == 0 || i == MAZE_SIZE - 1 || j == 0 || j == MAZE_SIZE - 1)
            {
                maze[j][i] = WALL_SYMBOL;
            }
            else if (goal_coords(i, j, MAZE_SIZE))
            {
                maze[j][i] = GOAL_SYMBOL;
            }
            else
            {
                maze[j][i] = EMPTY_SYMBOL;
            }
        }
    }
}

// only should be called if able to move_forward()
void move_forward(int robot_direction,
                  char internal_maze[MAZE_SIZE][MAZE_SIZE],
                  char actual_maze[MAZE_SIZE][MAZE_SIZE],
                  int *robot_pos_row,
                  int *robot_pos_col)
{

    internal_maze[*robot_pos_row][*robot_pos_col] = EMPTY_SYMBOL;
    actual_maze[*robot_pos_row][*robot_pos_col] = EMPTY_SYMBOL;

    switch (robot_direction)
    {
    case FACING_DOWN:
        *robot_pos_row += 1;
        break;
    case FACING_UP:
        *robot_pos_row -= 1;
        break;
    case FACING_RIGHT:
        *robot_pos_col += 1;
        break;
    case FACING_LEFT:
        *robot_pos_col -= 1;
        break;
    };
    internal_maze[*robot_pos_row][*robot_pos_col] = ROBOT_SYMBOL;
    actual_maze[*robot_pos_row][*robot_pos_col] = ROBOT_SYMBOL;
}

void turn_left(int *robot_direction)
{
    switch (*robot_direction)
    {
    case FACING_DOWN:
        *robot_direction = FACING_RIGHT;
        break;
    case FACING_UP:
        *robot_direction = FACING_LEFT;
        break;
    case FACING_RIGHT:
        *robot_direction = FACING_UP;
        break;
    case FACING_LEFT:
        *robot_direction = FACING_DOWN;
        break;
    }
}

void turn_right(int *robot_direction)
{
    switch (*robot_direction)
    {
    case FACING_DOWN:
        *robot_direction = FACING_LEFT;
        break;
    case FACING_UP:
        *robot_direction = FACING_RIGHT;
        break;
    case FACING_RIGHT:
        *robot_direction = FACING_DOWN;
        break;
    case FACING_LEFT:
        *robot_direction = FACING_UP;
        break;
    }
}

// placeholder results until sonar is connected.
// If there is a wall in front of the robot, return 0.1
// Else, return 0.9
float read_sonars(char actual_maze[MAZE_SIZE][MAZE_SIZE],
                  char internal_maze[MAZE_SIZE][MAZE_SIZE],
                  int robot_direction,
                  int robot_pos_row,
                  int robot_pos_col)
{
    int next_pos_row = robot_pos_row;
    int next_pos_col = robot_pos_col;

    int left_pos_row = robot_pos_row;
    int left_pos_col = robot_pos_col;

    int right_pos_row = robot_pos_row;
    int right_pos_col = robot_pos_col;

    switch (robot_direction)
    {
    case FACING_DOWN:
        next_pos_row += 1;
        left_pos_col += 1;
        right_pos_col -= 1;
        break;
    case FACING_UP:
        next_pos_row -= 1;
        left_pos_col -= 1;
        right_pos_col += 1;
        break;
    case FACING_RIGHT:
        next_pos_col += 1;
        left_pos_row -= 1;
        right_pos_row += 1;
        break;
    case FACING_LEFT:
        next_pos_col -= 1;
        left_pos_row += 1;
        right_pos_row -= 1;
        break;
    }

    if (actual_maze[left_pos_row][left_pos_col] == WALL_SYMBOL)
    {
        internal_maze[left_pos_row][left_pos_col] = WALL_SYMBOL;
    }
    if (actual_maze[right_pos_row][right_pos_col] == WALL_SYMBOL)
    {
        internal_maze[right_pos_row][right_pos_col] = WALL_SYMBOL;
    }

    // only return a value if front is clear
    if (actual_maze[next_pos_row][next_pos_col] == WALL_SYMBOL)
    {
        internal_maze[next_pos_row][next_pos_col] = WALL_SYMBOL;
        return 0.1;
    }
    else
    {
        return 0.9;
    }
}

// This will write to EEPROM when ported to arduino code
void save_maze(char internal_maze[MAZE_SIZE][MAZE_SIZE])
{
    // TODO: write the whole maze to EEPROM
    // EEPROM.write(addr, val);

    // tmp: writing to a normal file
    FILE *fp;
    fp = fopen("internal_maze.dat", "wb");
    fwrite(internal_maze, sizeof(char), MAZE_SIZE * MAZE_SIZE, fp);
    fclose(fp);
}

void read_maze(char maze[MAZE_SIZE][MAZE_SIZE])
{
    init_maze(maze);

    FILE *infile;
    infile = fopen("internal_maze.dat", "rb");
    fread(maze, sizeof(char), MAZE_SIZE * MAZE_SIZE, infile);
    fclose(infile);
}

// GLOBAL VARS
// initial position
int robot_pos_row = 1;
int robot_pos_col = 1;
int robot_direction = FACING_DOWN;

int finished;

// mazes
char internal_maze[MAZE_SIZE][MAZE_SIZE];
char actual_maze[MAZE_SIZE][MAZE_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 0, 0, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 1, 7, 7, 0, 1, 0, 1},
        {1, 0, 0, 1, 7, 7, 0, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

void setup() {    
    init_maze(internal_maze);

    update_mazes(robot_pos_row,
                 robot_pos_col,
                 internal_maze,
                 actual_maze);
    finished = 0;

}

void loop() {
    print_boards(internal_maze, actual_maze);

    // small random chance of turning before moving, even if space is clear
    if ((rand() % 10) < 2)
    {
        // evenly choose left or right
        if ((rand() % 10) < 5)
        {
            turn_left(&robot_direction);
        }
        else
        {
            turn_right(&robot_direction);
        }
    }

    // only move forward if sonar says path is clear
    if (read_sonars(actual_maze, internal_maze, robot_direction, robot_pos_row, robot_pos_col) > 0.5)
    {
        move_forward(robot_direction, internal_maze, actual_maze, &robot_pos_row, &robot_pos_col);
        //print_boards(internal_maze, actual_maze);
    }
    else
    {
        // evenly choose left or right
        if ((rand() % 10) < 5)
        {
            turn_left(&robot_direction);
        }
        else
        {
            turn_right(&robot_direction);
        }
    }

    if (goal_coords(robot_pos_row, robot_pos_col, MAZE_SIZE))
    {
        finished = 1;
    }
}
