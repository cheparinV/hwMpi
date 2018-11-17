#include "mpi.h"

#include <stdio.h>

void sendArrayToAllProcess(int* array, int processCount);

int main(int argc, char **argv) {

    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("SIZE=%d RANK=%d\n", size, rank);

    int arrSize = 10;
    int array[arrSize];
    srand((unsigned) time(0));
    if (rank == 0) {
        for (int i = 0; i < arrSize; i++) {
            array[i] = (rand() % 100) + 1;
            std::cout << array[i] << " ";
        }
        std::cout << '\n';
//        MPI_Send(&arrSize, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
//        MPI_Send(&array, arrSize, MPI_INT, 1, 0, MPI_COMM_WORLD);
        sendArrayToAllProcess(array, size);
    } else {
        int recArrSize;
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &recArrSize);
        printf("Recovery array size %d \n", recArrSize);
        int* recArray = (int*)malloc(sizeof(int) * recArrSize);
        MPI_Recv(recArray, recArrSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < recArrSize; ++i) {
            std::cout << recArray[i] << " ";
        }
        std::cout << '\n';
    }

    MPI_Finalize();

    return 0;

}

void sendArrayToAllProcess(int* array, int processCount) {
    for (int i = 1; i < processCount; ++i) {
        MPI_Send(array, 10, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
}