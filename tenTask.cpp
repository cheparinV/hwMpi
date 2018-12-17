#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *initArray(int size);

int *initMatrix(int cols, int rows);

int multArrays(int *x_array, int *y_array, int size);

int main(int argc, char **argv) {

    srand(time(0));

    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("SIZE=%d RANK=%d\n", size, rank);

    int arrSize = size - 1;
    int *matrix = (int *) malloc(sizeof(int) * arrSize * arrSize);
    int *x_array = (int *) malloc(sizeof(int) * arrSize);
    if (rank == 0) {
        std::cout << "x array:" << '\n';
        x_array = initArray(arrSize);
        std::cout << "matrix" << '\n';
        matrix = initMatrix(arrSize, arrSize);
    }
    int *subArray;
    std::cout << "start broadcasting" << '\n';

    MPI_Bcast(x_array, arrSize, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(matrix, arrSize, MPI_FLOAT, subArray, arrSize, MPI_FLOAT, 0, MPI_COMM_WORLD);
    std::cout << "broadcasted" << '\n';
    MPI_Barrier(MPI_COMM_WORLD);
    int sub_result = 0;

    if (rank != 0) {
        sub_result = multArrays(x_array, subArray, arrSize);
        //std::cout << sub_result << '\n';
    }
    MPI_Barrier(MPI_COMM_WORLD);
    int *result;
    if (rank == 0) {
        result = (int *) malloc(sizeof(int) * arrSize);
    }

    MPI_Gather(&sub_result, 1, MPI_FLOAT, result, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "result" << '\n';
        for (int i = 0; i < arrSize + 2; i++) {
            std::cout << result[i] << ' ';
        }
        std::cout << '\n';
    }
    MPI_Finalize();

    return 0;

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

int *initMatrix(int cols, int rows) {
    int *matrix = new int[cols * rows];
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = (rand() % 10) + 1;
        std::cout << matrix[i] << " ";
        if ((i + 1) % cols == 0) {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
    return matrix;
}


int multArrays(int *x_array, int *y_array, int size) {
    int sum = 0;
    //std::cout << "multiply" << '\n';
    for (int i = 0; i < size; i++) {
//        std::cout << x_array[i] << ' ' << '\n';
//        std::cout << y_array[i] << ' ' << '\n';
        sum = sum + (x_array[i] * y_array[i]);
    }
    return sum;
}