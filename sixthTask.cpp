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

    MPI_Status a_status, b_status;
    MPI_Request a_request1, a_request2;
    MPI_Request b_request1, b_request2;

    int arrSize = 10;
    int *array;
    if (rank % 2 == 0) {
        array = initArray(arrSize);
    } else {
        array = initArray(arrSize);
    }
    for (int j = 0; j < 4; ++j) {
        if (rank == 0) {
            MPI_Isend(array, arrSize, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &a_request1);
            MPI_Irecv(array, arrSize, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &a_request2);

            MPI_Wait(&a_request1, &a_status);
            printf("Ping %d \n", rank);
            MPI_Wait(&a_request2, &a_status);
            printf("Pong %d \n", rank);
        }
        if (rank == 1) {
            MPI_Isend(array, arrSize, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &b_request1);
            MPI_Irecv(array, arrSize, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &b_request2);

            MPI_Wait(&b_request1, &b_status);
            printf("Ping %d \n", rank);
            MPI_Wait(&b_request2, &b_status);
            printf("Pong %d \n", rank);
        }

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
        MPI_Recv(&array[i - 1], 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << array[i - 1] << " ";
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