#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>


/*This code is for reading and writing to files for 2026-2027 COMP528 OpenMP assignment*/

/*Use the functions in this file to read from the input file, and write to the output file*/

/*You should use this file when compiling your code*/

/*Declare these functions at the top of each 'main' file or use a header file*/

/*If there are any issues with this code, please contact: bkeetch@liverpool.ac.uk*/

int read_num_dims(const char* filename) {

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Unable to open file: %s\n", filename);
        return -1; //-1 means error
    }

    char firstline[500];

    if (fgets(firstline, 500, file) == NULL) {
        printf("Error reading file %s\n", filename);
        fclose(file); // added by me
        return -1; //-1 means error
    }

    int line_length = strlen(firstline);
    int num_dims = 0;

    int i;
    for (i = 0; i < line_length; i++) {
        if (firstline[i] == ' ') {
            num_dims++;
        }
    }

    fclose(file);
    return num_dims;
}


/*Gets the number of the radiator temperatures in the file. Returns as a single integer*/
int* read_dims(const char* filename, int num_dims) {

    FILE* file = fopen(filename, "r");
    char* context = NULL; // to use strtok_s instead of strtok

    if (file == NULL) {
        printf("Unable to open file: %s\n", filename);
        return NULL;
    }

    char firstline[500];

    if (fgets(firstline, 500, file) == NULL) {
        printf("Error reading file %s\n", filename);
        fclose(file); // added by me
        return NULL;
    }

    const char s[2] = " ";
    char* token;
    token = strtok(firstline, s);
    //token = strtok_s(firstline, s, &context); // Modified by me

    int* dims = malloc((num_dims) * sizeof(int));

    if (dims == NULL) {
        printf("Error allocating memory for dimension array\n");
        fclose(file); // added by me
        return NULL;
    }

    int i = 0;
    //    while(token != NULL) {
    while (token != NULL && i < num_dims) {  // Modified by me
        dims[i] = atoi(token);
        //        printf("\ndims[%d] = %d", i,dims[i]);// debugging

        i++;
        token = strtok(NULL, s);
        //token = strtok_s(NULL, s, &context);

    }
    //    printf("\nToken while ends");// debugging

    fclose(file);
    return dims;
}


int product(const int* dims, int num_dims) {

    int product2 = 1;
    int i;

    for (i = 0; i < num_dims; i++) {
        //        printf("\n%d", i);   // debugging

        product2 *= dims[i];
    }

    return product2;
}


/*Gets the data from the file. Returns as an array of doubles. Ignores the first numOfValues*/
float* read_array(const char* filename, const int* dims, int num_dims) {

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Unable to open file: %s\n", filename);
        return NULL; //error
    }

    char firstline[500];

    if (fgets(firstline, 500, file) == NULL) {
        printf("Error reading file\n");
        fclose(file); // added by me
        return NULL; //error
    }

    int total_elements = product(dims, num_dims);
    float* data = malloc(total_elements * sizeof(float));

    if (data == NULL) {
        printf("Error allocating memory for data from %s\n", filename);
        fclose(file); // added by me
        return NULL;
    }

    int i;
    float temp;
    for (i = 0; i < total_elements; i++) {
        //        if(fscanf_s(file, "%f", &temp) != 1){
        if (fscanf(file, "%f", &temp) != 1) {
            printf("Error reading file");
            fclose(file); // added by me
            return NULL; //error  
        }
        data[i] = temp;
        //        printf("\n\t data[%d] = %f", i, data[i]); // Debugging
    }

    fclose(file);
    return data;
}

/*Writes to the output file*/
int write_to_output_file(const char* filename, float* output, int* dims, int num_dims) {

    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        printf("Unable to open file %s\n", filename);
        return 1;
    }

    printf("\nFile opened, writing dims\n");

    int i;
    for (i = 0; i < num_dims; i++) {
        fprintf(file, "%d ", dims[i]);
    }
    fprintf(file, "\n");

    int total_elements = product(dims, num_dims);

    printf("\nWriting output data\n");
    for (i = 0; i < total_elements; i++) {
        fprintf(file, "%.2f ", output[i]);
    }

    fclose(file);
    return 0;
}



