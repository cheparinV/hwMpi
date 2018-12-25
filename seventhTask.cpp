#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int arrayNorm(int *array, int size);

int *initArray(int size);

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

    int subNorm;
    int allNorm;

    subNorm = arrayNorm(subArray, arrSize);

    MPI_Reduce(&subNorm, &allNorm, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total norm %d \n", allNorm);
    }

    MPI_Finalize();

    return 0;

}

int arrayNorm(int *array, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += array[i];
    }
    printf("Sum %d \n", sum);
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
