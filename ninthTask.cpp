#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct type{
    float value;
    int rank;
} in, out;

type minOfArray(float *array, int size);

float *initArray(int size);


int main(int argc, char **argv) {

    srand(time(0));

    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("SIZE=%d RANK=%d\n", size, rank);

    int arrSize = 5;
    float* array= NULL;
    if (rank == 0) {
        array = initArray(arrSize * size);
    }
    float *subArray = (float *) malloc(sizeof(float) * arrSize);

    MPI_Scatter(array, arrSize, MPI_FLOAT, subArray, arrSize, MPI_FLOAT, 0, MPI_COMM_WORLD);

    type subMin;

    printf("rank: %d\n",rank);
    in = minOfArray(subArray, arrSize);
    in.rank += rank * arrSize;


    MPI_Reduce(&in, &out, 1, MPI_FLOAT_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total min index %d \n", out.rank);
        printf("Total min %f \n", out.value);
    }
    MPI_Finalize();

    return 0;

}

type minOfArray(float *array, int size) {
    type min_type;
    min_type.value = array[0];
    min_type.rank = 0;
    for (int i = 0; i < size; ++i) {
        printf("index: %d, value: %f \n",i, array[i]);
        if (min_type.value > array[i]) {
            min_type.value = array[i];
            min_type.rank = i;
        }
    }
    printf("Min of sub array %d \n", min_type.value);
    return min_type;
}

float *initArray(int size) {
    float *array = new float[size];
    for (int i = 0; i < size; i++) {
        array[i] = (rand() % 100) + 1;
        std::cout << array[i] << " ";
    }
    std::cout << '\n';
    return array;
}
