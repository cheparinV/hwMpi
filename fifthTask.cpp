#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sendArrayToAllProcess(int *array, int processCount, int tag);

void receiveAllProcess(int processCount);

int *initArray(int size);

int *aCountZ(int *x_array, int *y_array, int size);

int *bCountZ(int *x_array, int *y_array, int size);

int main(int argc, char **argv) {

    srand(time(0));

    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("SIZE=%d RANK=%d\n", size, rank);

    int arrSize = 10;
    int *array;
    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            array = initArray(size - 1);
            MPI_Send(array, size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        receiveAllProcess(size);
    } else {
        int arrSize;
        MPI_Status status;

        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &arrSize);

        printf("Receive by %d array with size %d \n", rank, arrSize);
        int *array = (int *) malloc(sizeof(int) * arrSize);
        MPI_Recv(array, arrSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Send(&array[rank - 1], 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        std::cout << "send to zero proc \n";
    }
    MPI_Finalize();

    return 0;

}

void sendArrayToAllProcess(int *array, int processCount, int tag) {
    for (int i = 1; i < processCount; ++i) {
        MPI_Send(array, 10, MPI_INT, i, tag, MPI_COMM_WORLD);
    }
}

void receiveAllProcess(int processCount) {
    int array[processCount];
    for (int i = 1; i < processCount; ++i) {
        MPI_Recv(&array[i-1], 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << array[i-1] << " ";
    }
    std::cout << '\n';
}

int *initArray(int size) {
    int *array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = (rand() % 100) + 1;
        std::cout << array[i] << " ";
    }
    std::cout << '\n';
    return array;
}

int *aCountZ(int *x_array, int *y_array, int size) {
    int *z_array = new int[size];
    for (int i = 0; i < size; ++i) {
        z_array[i] = x_array[i] + y_array[i];
    }
    return z_array;
}

int *bCountZ(int *x_array, int *y_array, int size) {
    int *z_array = new int[size];
    for (int i = 0; i < size; ++i) {
        z_array[i] = x_array[i] * y_array[i];
    }
    return z_array;
}