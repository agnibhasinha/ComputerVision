#include <iostream>
#include <stdio.h>
#include <utility>
#include <vector>
#include <queue>
#include <math.h>
#include <stack>
#include <set>
#include <float.h>
#include <cstring>
#include <time.h>
#include <iomanip>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


#define ROW 100
#define COL 100

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;

// A structure to hold the neccesary parameters
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};

Mat fimg;

bool isValid(int row, int col) //to check if cell is valid or not
{
    return (row >= 0) && (row < ROW) && (col >= 0)
        && (col < COL);
}

bool isUnBlocked(int grid[][COL], int row, int col) // Returns true if the cell is not blocked
{
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}

bool isDest(int row, int col, Pair dest)// to check if destinatin is reached or not
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double heur(int row, int col, Pair dest)
{

    return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
   /* return (double)(fabs(row - dest.first) + fabs(col - dest.second));*/
   /* return 0;*/
    //return (double)max(fabs(row - dest.first) , fabs(col - dest.second));
 /*   return (double)pow((fabs(row - dest.first) + fabs(col - dest.second)),2);*/
   /* return ((double)pow((row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second),0.5)/2);*/
}

// A Utility Function to trace the path from the source
// to destination
void tracePath(cell cellDetails[][COL], Pair dest)
{
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;
    int t_row = cellDetails[row][col].parent_i;
    int t_col = cellDetails[row][col].parent_j;
    double cost = cellDetails[t_row][t_col].g;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
        && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
        fimg.at<Vec3b>(p.first, p.second)[0] = 254;
        fimg.at<Vec3b>(p.first, p.second)[1] = 1;
        fimg.at<Vec3b>(p.first, p.second)[2] = 254;
    }
    cout << "\nthe total cost is: " << cost << endl;

    return;
}

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
void aStarSearch(int grid[][COL], Pair src, Pair dest)
{
    // If the source is out of range
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }

    // If the destination is out of range
    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(grid, src.first, src.second) == false
        || isUnBlocked(grid, dest.first, dest.second)
        == false) {
        printf("Source or the destination is blocked\n");
        return;
    }

    // If the destination cell is the same as source cell
    if (isDest(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return;
    }

    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    // Declare a 2D array of structure to hold the details
    // of that cell
    cell cellDetails[ROW][COL];

    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implenented as a set of pair of
     pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;

        closedList[i][j] = true;
        fimg.at<Vec3b>(i, j)[0] = 1;
        fimg.at<Vec3b>(i, j)[1] = 70;
        fimg.at<Vec3b>(i, j)[2] = 254;

        

         // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j) == true) {
            // If the destination cell is the same as the current successor
            if (isDest(i - 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j] == false
                && isUnBlocked(grid, i - 1, j)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = heur(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));

                    // Update the details of this cell
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------


        if (isValid(i + 1, j) == true) {

            if (isDest(i + 1, j, dest) == true) {

                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i + 1][j] == false
                && isUnBlocked(grid, i + 1, j)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = heur(i + 1, j, dest);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));

                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------


        if (isValid(i, j + 1) == true) {

            if (isDest(i, j + 1, dest) == true) {

                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }


            else if (closedList[i][j + 1] == false
                && isUnBlocked(grid, i, j + 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = heur(i, j + 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));


                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------


        if (isValid(i, j - 1) == true) {


            if (isDest(i, j - 1, dest) == true) {

                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }


            else if (closedList[i][j - 1] == false
                && isUnBlocked(grid, i, j - 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = heur(i, j - 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));


                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        //----------- 5th Successor (North-East)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDest(i - 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j + 1] == false
                && isUnBlocked(grid, i - 1, j + 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = heur(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j + 1].f == FLT_MAX
                    || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 6th Successor (North-West)
        //------------


        if (isValid(i - 1, j - 1) == true) {


            if (isDest(i - 1, j - 1, dest) == true) {

                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }




            else if (closedList[i - 1][j - 1] == false
                && isUnBlocked(grid, i - 1, j - 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = heur(i - 1, j - 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i - 1][j - 1].f == FLT_MAX
                    || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j - 1)));

                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        //----------- 7th Successor (South-East)
        //------------


        if (isValid(i + 1, j + 1) == true) {

            if (isDest(i + 1, j + 1, dest) == true) {

                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }


            else if (closedList[i + 1][j + 1] == false
                && isUnBlocked(grid, i + 1, j + 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = heur(i + 1, j + 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j + 1].f == FLT_MAX
                    || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 8th Successor (South-West)
        //------------


        if (isValid(i + 1, j - 1) == true) {


            if (isDest(i + 1, j - 1, dest) == true) {

                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }


            else if (closedList[i + 1][j - 1] == false
                && isUnBlocked(grid, i + 1, j - 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = heur(i + 1, j - 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j - 1].f == FLT_MAX
                    || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j - 1)));


                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destiantion cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");

    return;
}

// Driver program to test above function
int main()
{
    clock_t start, end;
    start = clock();
    Mat image = imread("Task_1_Low.jpg");
    Mat img;
    namedWindow("image", WINDOW_NORMAL);

    cout << image.size() << endl;
    resize(image, img, Size(100, 100));
    resize(image, fimg, Size(100, 100));
    cout << img.size() << endl;
    imshow("image", img);
    //imshow("Image resized", img);
    /* Description of the Grid-
     1--> The cell is not blocked
     0--> The cell is blocked    */
    int grid[ROW][COL];

    int src_x = 0, src_y = 0, dst_x = 0, dst_y = 0;
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            int x = img.at<Vec3b>(i, j)[0];
            int y = img.at<Vec3b>(i, j)[1];
            int z = img.at<Vec3b>(i, j)[2];
            if (x == 0)
                grid[i][j] = 1;
            if (y == 204)
            {
                src_x = i; src_y = j; grid[i][j] = 1;
            }
            if (y == 76)
            {
                dst_x = i; dst_y = j; grid[i][j] = 1;
            }
            if (x == 255)
                grid[i][j] = 0;
        }

    }
    
    Pair src = make_pair(src_x, src_y);
    Pair dest = make_pair(dst_x, dst_y);
    cout << src.first << " " << src.second << endl;
    cout << dest.first << " " << dest.second << endl;
    aStarSearch(grid, src, dest);
    resize(fimg, fimg, Size(500, 500));
    imshow("final image", fimg);
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\ntime taken by the program is: " << fixed << time_taken << setprecision(5);
    cout << "sec" << endl;
    waitKey(0);
    return (0);
}