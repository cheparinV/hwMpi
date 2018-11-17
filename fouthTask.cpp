#include "mpi.h"

#include <stdio.h>

void sendArrayToAllProcess(int *array, int processCount, int tag);

void receiveAllProcess(int processCount);

int *initArray(int size);

int *aCountZ(int *x_array, int *y_array, int size);

int *bCountZ(int *x_array, int *y_array, int size);

int main(int argc, char **argv) {

    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("SIZE=%d RANK=%d\n", size, rank);

    int arrSize = 10;
    int *array_x;
    int *array_y;
    if (rank == 0) {
        array_x = initArray(arrSize);
        array_y = initArray(arrSize);
        sendArrayToAllProcess(array_x, size, 0);
        std::cout << "x send \n";
        sendArrayToAllProcess(array_y, size, 1);
        std::cout << "y send \n";
        receiveAllProcess(size);
    } else {
        int x_size;
        int y_size;
        MPI_Status status_x;
        MPI_Status status_y;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status_x);
        MPI_Get_count(&status_x, MPI_INT, &x_size);
        printf("Recovery array_x size %d \n", x_size);
        int *x_array = (int *) malloc(sizeof(int) * x_size);
        MPI_Recv(x_array, x_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Probe(0, 1, MPI_COMM_WORLD, &status_y);
        MPI_Get_count(&status_x, MPI_INT, &y_size);
        printf("Recovery array_y size %d \n", y_size);
        int *y_array = (int *) malloc(sizeof(int) * y_size);
        MPI_Recv(y_array, y_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int z_size = x_size > y_size ? x_size : y_size;
        std::cout << "array \n";
        int *z_array;
        if (rank % 2 != 0) {
            z_array = aCountZ(x_array, y_array, z_size);
        } else {
            z_array = bCountZ(x_array, y_array, z_size);
        }
        MPI_Send(z_array, z_size, MPI_INT, 0, 2, MPI_COMM_WORLD);
        std::cout << "send z \n";
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
    for (int i = 1; i < processCount; ++i) {
        int size;
        MPI_Status status;
        MPI_Probe(i, 2, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &size);
        printf("Recv array_z size %d \n", size);
        int *array = (int *) malloc(sizeof(int) * size);
        MPI_Recv(array, size, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int j = 0; j < size; ++j) {
            std::cout << array[i] << " ";
        }
        std::cout << '\n';
    }
}

int *initArray(int size) {
    srand((unsigned) time(NULL));
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = (rand() % 100) + 1;
        std::cout << array[i] << " ";
    }
    std::cout << '\n';
    return array;
}

int *aCountZ(int *x_array, int *y_array, int size) {
    int* z_array = new int[size];
    for (int i = 0; i < size; ++i) {
        z_array[i] = x_array[i] + y_array[i];
    }
    return z_array;
}

int *bCountZ(int *x_array, int *y_array, int size) {
    int* z_array = new int[size];
    for (int i = 0; i < size; ++i) {
        z_array[i] = x_array[i] * y_array[i];
    }
    return z_array;
}