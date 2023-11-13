#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
  // Inicializa
  int size, rank;
  MPI_Init(&argc, &argv);
  // Lendo size e rank
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // Print
  std::cout << "Hello world, from process #" << rank << std::endl;
  
  // Finaliza
  int MPI_Finalize();

  return 0;
}

/*
Rank = numero do processo atual
size = quantos processos estão em execução
MPI_COMM_WORLD = comunicador global que encapsula todos os processos em execução
*/