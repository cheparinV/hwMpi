#include "mpi.h"

#include <stdio.h>

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
        MPI_Send(&arrSize, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&array, arrSize, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        int recArrSize;
        MPI_Recv(&recArrSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int recArray[recArrSize];
        MPI_Recv(&recArray, recArrSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Recovery array size %d \n", recArrSize);
        for (int i = 0; i < recArrSize; ++i) {
            std::cout << recArray[i] << " ";
        }
    }

    MPI_Finalize();

    return 0;

}