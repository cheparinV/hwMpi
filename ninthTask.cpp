#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int minOfArray(int *array, int size);

int *initArray(int size);

struct type{
    int value;
    int rank;
} result;

int main(int argc, char **argv) {

    srand(time(0));

    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("SIZE=%d RANK=%d\n", size, rank);

    int arrSize = 5;
    int *array = (int *) malloc(sizeof(int) * arrSize * size);
    if (rank == 0) {
        array = initArray(arrSize * size);
    }
    int *subArray = (int *) malloc(sizeof(int) * arrSize);

    MPI_Scatter(array, arrSize, MPI_INT, subArray, arrSize, MPI_INT, 0, MPI_COMM_WORLD);

    int subMin;
    int globalMin;
    if (rank != 0) {
        subMin = minOfArray(subArray, arrSize);
    }

    MPI_Reduce(&subMin, &result, 2, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total min rank %d \n", result.rank + 1);
        printf("Total min %d \n", result.value);
    }
    MPI_Finalize();

    return 0;

}

int minOfArray(int *array, int size) {
    int min = array[0];
    for (int i = 0; i < size; ++i) {
        min = min > array[i] ? array[i] : min;
    }
    printf("Min of sub array %d \n", min);
    return min;
}

int *initArray(int size) {
    int *array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = (rand() % 10) + 1;
        std::cout << array[i] << " ";
    }
    std::cout << '\n';
    return array;
}
