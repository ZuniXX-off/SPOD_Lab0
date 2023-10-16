#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "mpi.h"

#define SEC_TO_MICROSEC 1'000'000

int main(int argc, char* argv[]) {
    // Иницилизация переменных
    int processesCount, processRank, receivedRank;
    double start, end;
    MPI_Status status;

    // Инициалзация секции MPI
    MPI_Init(&argc, &argv);

    // Получение количества запущенных процессов
    MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

    // Получение ранга выполняемого процесса процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    // Ветка для порождающего процесса
    if (processRank == 0) {

        // Получение времени начала работы порождающего процесса
        start = MPI_Wtime();

        printf("Hello from process %3d \n", processRank);

        // Получение сообщений от всех созданных процессов в произвольном порядке
        for (int i = 1; i < processesCount; i++) {
            MPI_Recv(&receivedRank, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Hello from process %3d \n", receivedRank);
        }

        // Время окончания работы порождающего процесса
        end = MPI_Wtime();
        printf("Time for main process is %4d mcs\n", (int)((end - start) * SEC_TO_MICROSEC));
    }

    // Ветка для всех остальных запущенных процессов
    else {
        MPI_Send(&processRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Завершение секции MPI 
    MPI_Finalize();
    return 0;
}