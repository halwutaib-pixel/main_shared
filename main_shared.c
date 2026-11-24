// Serial_Implementation.c : This file contains the 'main' function. Program execution begins and ends there.
//

/*
 * Written by
 *
*/

/*
Each cell within the grid has a cell state given by :
 0 = HEALTHY
 1 = INFECTED
 2 = IMMUNE
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <time.h>
#include <math.h>
#include <string.h>

//#pragma comment(linker, "/STACK:10000000")
//#pragma comment(linker, "/HEAP:10000000")

//#include "file_reader.c"

int threads = 32; // Number of threads to use for parallelization

//const int m = 5;//5000;
//const int n = 5;//5000;

int m = 5;//5000;
int n = 5;//5000;
int r_trans = 2;
int rec_time = 2;
int max_run = 100;
//char filename[25] = "pop_5_5.Dat";


extern int read_num_dims(const char* filename);
extern int* read_dims(const char* filename, int num_dims);
extern int product(const int* dims, int num_dims);
extern float* read_array(const char* filename, const int* dims, int num_dims);
extern int write_to_output_file(const char* filename, float* output, int* dims, int num_dims);



void assign(int** arr, int m, int n)
{
    //    printf("\n Assign 0 values to the 2D array \n");

        // Assign 0 values to the 2D array // Assign random values to the 2D array
    int i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++) {
            arr[i][j] = 0;//10.0 * rand() / RAND_MAX;
        }
    }
}

void _print(int** arr, int m, int n)
{
    //printf("\n print 2D array \n");
    // Print the 2D array
    int i, j;
    for (i = 0; i < m; i++) {
        printf("\n\t|");
        for (j = 0; j < n; j++) {
            printf(" %d", arr[i][j]);
        }
        printf("  |");
    }
}

void _print3D(int*** arr, int l, int m, int n)
{
    //printf("\n print 3D array \n");
    // Print the 3D array
    int i, j, k;
    for (i = 0; i < m; i++) { // row
        printf("\n|");
        for (k = 0; k < l; k++) {  // layer
            //printf("  |");
            for (j = 0; j < n; j++) {  // column
                printf(" %d", arr[k][i][j]);
            }
            printf("|");
        }
    }
}


void _print_float(float** arr, int m, int n)  // made by me for float matrix 
{
    // Printing the 2D float array
    int i, j;
    for (i = 0; i < m; i++)
    {
        printf("\n\t|");
        for (j = 0; j < n; j++) {
            printf("  %f", arr[i][j]);
        }
        printf("|");
    }
}


void _free(int** arr, int m, int n)
{
    printf("\n Free \n");
    // Free the memory space locating the 2D array
    int i;
    for (i = 0; i < m; i++) {
        free(arr[i]);
    }
    free(arr);
}

void _freef(float** arr, int m, int n)
{
    printf("\n Free \n");
    // Free the memory space locating the 2D array
    int i;
    for (i = 0; i < m; i++) {
        free(arr[i]);
    }
    free(arr);
}


void _free3D(int*** arr, int l, int m, int n)
{
    printf("\n Free \n");
    // Free the memory space locating the 3D array
    int i, j;
    for (i = 0; i < l; i++) {
        for (j = 0; j < m; j++) {
            free(arr[i][j]);
        }
        free(arr[i]);
    }
    free(arr);
    printf("\n Free 3D \n");

}

void _add(int** arr1, int** arr2, int** arr3, int m, int n)
{
    printf("\n Add 2D arrays \n");
    // Add two 2D arrays and store the results in a third array
    int i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++) {
            arr3[i][j] = arr1[i][j] + arr2[i][j];
        }
    }
}

void _infected(int** arr1, int m, int n)
{
    //    printf("\n Infected cell at (%d,%d) gives \n",m,n);
        // Marking infected cell 

    arr1[m][n] = 1;
}

// Made to copy 2D array to another 2D array 
void Copy_2DArray(int** dest, int** src, int row, int col) {
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            dest[i][j] = src[i][j];
        }
    }

    // Implementation for copying 3D array
}

int RangedRand(int range_min, int range_max)
{
    // Generate random numbers in the interval [range_min, range_max], inclusive.
    int r = ((double)rand() / RAND_MAX) * (range_max - range_min) + range_min;
    return r;
}

float RangedRand_float(float range_min, float range_max)
{
    // Generate random numbers in the interval [range_min, range_max], inclusive.
    float r = ((double)rand() / RAND_MAX) * (range_max - range_min) + range_min;
    return r;
}

int Infection_Spread(int** Pop_Spreadf, float** Pop_Densityf, int row, int col, int r_trans) {//}, int x, int y) {
    int radius_i_j; // variable to store the Manhattan distance between the source cell (x,y) and the currently targeted cell (i,j)
    int i, j, k, index = 0;
    float P_infection_i_j, P_rand_0_1;

    //    int* x = (int*)malloc(row * sizeof(int));  
    //    int* y = (int*)malloc(col * sizeof(int));

    int* x = (int*)malloc(row * col * sizeof(int));
    int* y = (int*)malloc(row * col * sizeof(int));


    // Finding current number and addresses of infected cells 
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (Pop_Spreadf[i][j] == 1) {
                x[index] = i;
                y[index++] = j;
            }
        }
    }

    printf("\nThe number of infected cells found: %d", index);

    //#pragma ompparallel for num_threads(threads) private(k, radius_i_j, P_infection_i_j, P_rand_0_1)
    for (k = 0; k < index; k++) {


        //#pragma omp parallel for collapse(2) private(k, radius_i_j, P_infection_i_j, P_rand_0_1)
#pragma ompparallel for collapse(2) num_threads(threads)

        // Findig spread infection 
        for (i = 1; i <= row; i++) {  // index starting from 1 to m for distance calculation
            for (j = 1; j <= col; j++) {  // index starting from 1 to n for distance calculation
                // Manhattan distance formula
                radius_i_j = abs(x[k] + 1 - i) + abs(y[k] + 1 - j); // x+1 and y+1 to start counting from 1 instead of 0 for distance calculation

                // Leave the source cell (x,y) as infected and do not change its state
                if (Pop_Spreadf[i - 1][j - 1] != 1 && Pop_Spreadf[i - 1][j - 1] != 2) {
                    if (radius_i_j <= r_trans) { // The cells within the transmissibility radius
                        // Finding P_infection for currently targeted cell (i-1,j-1) based on its population density and distance from the source cell (k,l)
                        P_infection_i_j = Pop_Densityf[i - 1][j - 1] / radius_i_j; // offsetting with -1 to follow indexing by 0 
                        P_rand_0_1 = RangedRand_float(0, 1); // random number between 0 and 1 for comparison with P_infection_i_j
                        //                        printf("\n(%d,%d)=%f; %f", i, j, P_rand_0_1, Pop_Densityf[i - 1][j - 1]); //debugging
                        if (P_infection_i_j > P_rand_0_1) { // infecting the cell if P_infection_i_j is greater than the random number


                            //                            #pragma omp atomic write
                            Pop_Spreadf[i - 1][j - 1] = 1; // infected; offsetting with -1 to follow indexing by 0
                        }

                        //printf("\n(%d,%d)=%f", i, j, radius_i_j); //debugging 
                    }
                }
            }
        }

    }

    free(x);
    free(y);
    return index;
}


int main(void)
{

    int i, j, k;
    //	char filename[25];
    char* filename = (char*)malloc(25 * sizeof(char));

    srand((unsigned int)time(NULL)); // Call once 

    printf("\t\t------****** Disease Spread Simulation: Serial Program ******------\n");

    printf("\n\t Enter value of transmissibility radius: ");
    r_trans = 4;   // Debugginh
    //    scanf_s("%d", &r_trans);

    printf("\t Enter value of recovery time: ");
    rec_time = 3;   // Debugginh
    //    scanf_s("%d", &rec_time);

    printf("\t Enter value of number of Maximum Run: ");
    max_run = 1000;   // Debugginh
    //    scanf_s("%d", &max_run);

    printf("\n\tEnter filename containing population densities: ");
    filename = "pop_128_128.Dat";//"pop_5_5.Dat";

    /*
        if (fgets(filename, sizeof(filename), stdin) != NULL) {
            printf("Hello, %s", filename);

            // Remove newline character added by fgets
            filename[strcspn(filename, "\n")] = '\0';
        }
        else {
            printf("\nError reading filename\n");
            return 1; // Exit with error
        }
    */

    int no_dim = read_num_dims(filename);// (const char* filename)
    //    int no_dim = read_num_dims("pop_5_5.Dat");// Debugging
    //    printf("\n\t dim = %d", no_dim); // debugging

    int* dims = read_dims(filename, no_dim);
    //    int* dims = read_dims("pop_5_5.Dat", no_dim); //Debugging
    m = dims[0];
    n = dims[1];
    //    printf("\nRead dimensions done");// debugging
//        printf("\n\tdims[0] = %d", dims[0]);// debugging
//        printf("\n\tdims[1] = %d", dims[1]);// debugging


        // Reading Populaion Density from file
    float* OneD_Array = read_array(filename, dims, no_dim);
    //    float* OneD_Array = read_array("pop_5_5.Dat", dims, no_dim); // Debugging 

    //    free(filename);
    //    filename = NULL;
    //	_free(filename,25,1);  



        // Creating q 2D array for Population Density 
        // dynamically create arrays of pointers of 
    float** Pop_Density = (float**)malloc(m * sizeof(float*)); // row size = m
    // dynamically allocate memory of size `n` for each row 
    for (i = 0; i < m; i++) {
        Pop_Density[i] = (float*)malloc(n * sizeof(float));
    }

    // Writing Population density from read 1D array to 2D array   
    int count = 0;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            Pop_Density[i][j] = OneD_Array[count];
            count += 1;
        }
    }

    printf("The Population Density read from file is: ");
    //    _print_float(Pop_Density, m, n);


        // Creating (rec_time+1) number of 3D arrays for Population Spread 
        // 1st dynamically create arrays of 3D pointers of row size = rec_time
    int*** Pop_Spread = (int***)malloc((rec_time + 1) * sizeof(int*));

    // dynamically allocate memory of size `m` for each row
    for (i = 0; i < rec_time + 1; i++) {
        Pop_Spread[i] = (int**)malloc(m * sizeof(int*));
    }

    // creating 3rd dimension 
    for (i = 0; i < rec_time + 1; i++) {
        for (j = 0; j < m; j++) {
            // dynamically allocate memory of size `n` for each element of 2D array to get 3D array
            Pop_Spread[i][j] = (int*)malloc(n * sizeof(int));
        }
    }


    // Assign 0 (Healthy) to Population spread marrixes (rec_Time + 1 in numbers) 
    for (k = 0; k <= rec_time; k++) {
        assign(Pop_Spread[k], m, n);
    }

    printf("\nThe Population Spread Matrices of current and previous t_rec times (all healthy): ");
    //    _print(Pop_Spread[0], m, n);
    //    printf("\n   t[n]     t[n-1]    t[n-t_rec]");
    //    _print3D(Pop_Spread, rec_time+1, m, n);   

    //    printf("\n\t\t\t Press any key to start simulations ... ");
    //    _getch();

        // Time step 0: Randomly selecting a cell to assign infection
    int x = RangedRand(0, m - 1); // row number 
    int y = RangedRand(0, n - 1); // column number

    //    printf("\nRandomly selecting infected cell is at (%d,%d)", x, y);
        // For debugging
    //    x = 2;
    //    y = 2;

    clock_t start_time = clock();  // // Save the current start time 


    // **************************************************************************//
    // **************************************************************************//

    printf("\n\n\t\t\tTime Step 0");
    //    printf("\nThe Population Spread Matrix with randomly select infected cell: ");
    _infected(Pop_Spread[0], x, y);
    //    printf("\n   t[n]     t[n-1]    t[n-t_rec]");
    //    _print3D(Pop_Spread, rec_time + 1, m, n);
    //    printf("\n\t\t\t Press any key to start simulations ... ");
    //    _getch();


        // Time step 1: Make a copy of last Population Spread matrix 
        // And then update for infection spread within the transmissibility radius r_trans  
        //    printf("\n\n\t\tSpreading infection over transmissibility radius ");
    int Infected_Cells1 = 1; // Infected cells counts at current time step 0
    int Infected_Cells_t = 0; // Total infected cell at time step 0 
    count = 1; // Time step counter 

    while (Infected_Cells1 != 0 && count < max_run) {
        // Save the previous time steps' Pop_Spread (up to rec_time)  
        for (k = 0; k < rec_time; k++) {
            Copy_2DArray(Pop_Spread[rec_time - k], Pop_Spread[rec_time - 1 - k], m, n);
        }

        //        printf("\n\n\t\t\tTime Step %d", count);
                // Checking for recovery (Immunity) of infected cells after rec_time steps
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++) {
                // find the infected cell in Pop_Spread matrix of rec_Time steps behind the current time 
                if (Pop_Spread[rec_time][i][j] == 1) {
                    Pop_Spread[0][i][j] = 2;  // Immune the cell in current time step's Pop_Spread matrix                        y[index++] = j;
                }
            }
        }
        count++;
        // find infection spread of infected cells, if any

        Infected_Cells1 = Infection_Spread(Pop_Spread[0], Pop_Density, m, n, r_trans);  // find and locate infection spread of infected cells, if any
        // The return is tota; infected cells
        Infected_Cells_t += Infected_Cells1; // Add to total infected cell count

        //        _print(Pop_Spread[0], m, n);
        //        _getch();
                //        _print3D(Pop_Spread, rec_time + 1, m, n);

    }

    printf("\n\n\t\tThe Total of %d Cells Infected Over %d Runs.", Infected_Cells_t, count);
    //    printf("\n\t\tThe Average Number of Cells Infected Over All Step Run is: %0.2f", (float)Infected_Cells_t / (count));

    printf("\n\n\t\tAVERAGE CELLS INFECTE: %0.2f", (float)Infected_Cells_t / (count));

    clock_t end_time = clock();  // // Save the current end time 

    float time_spent = (float)(end_time - start_time) / CLOCKS_PER_SEC;  // Calculate elapsed time (seconds)
    printf("\n\t\tTOTAL TIME TAKEN: %f Seconds \n", time_spent);


    free(dims);//, no_dim); // Clean up memory to prevent leaks
    free(OneD_Array);
    _freef(Pop_Density, m, n);

    for (k = 0; k < rec_time; k++)
        _free(Pop_Spread[k], m, n);

    //_free3D(Pop_Spread, rec_time + 1, m, n);

    return 0;
}

