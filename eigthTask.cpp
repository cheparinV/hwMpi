#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int scalarMultiply(int *x_array, int *y_array, int size);

int *initArray(int size);

int main(int argc, char **argv) {

    srand(time(0));

    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("SIZE=%d RANK=%d\n", size, rank);

    int arrSize = 5;
    int *x_array = (int *) malloc(sizeof(int) * arrSize * size);
    int *y_array = (int *) malloc(sizeof(int) * arrSize * size);
    if (rank == 0) {
        x_array = initArray(arrSize * size);
        y_array = initArray(arrSize * size);
    }
    int *x_subArray = (int *) malloc(sizeof(int) * arrSize);
    int *y_subArray = (int *) malloc(sizeof(int) * arrSize);

    MPI_Scatter(x_array, arrSize, MPI_INT, x_subArray, arrSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(y_array, arrSize, MPI_INT, y_subArray, arrSize, MPI_INT, 0, MPI_COMM_WORLD);

    int subResult = 0;
    int scalarSum = 0;
    if (rank != 0) {
        subResult = scalarMultiply(x_subArray, y_subArray, arrSize);
    }
    MPI_Reduce(&subResult, &scalarSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total norm %d \n", scalarSum);
    }

    MPI_Finalize();

    return 0;

}

int scalarMultiply(int *x_array, int *y_array, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += x_array[i] * y_array[i];
    }
    printf("Scalar multiply %d \n", sum);
    return sum;
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
