#include <iostream>
#include <cstdlib>
#include <mpi.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int buffer_count = 5000;
  float buffer[buffer_count];
  memset(buffer, 0, sizeof(buffer));
  
  // gerando os números e fazendo 1000 repetições
  for (int rep = 0; rep < 1000; ++rep) {
    for (int i = 0; i < buffer_count; ++i) {
      float val = (float)rand() / RAND_MAX;
      buffer[i] += val;
    }
  }

  // TODO: crie um buffer chamado recepção e chame MPI_Reduce para somar todas as variáveis
  // sobre todos os processos e armazena o resultado no processo 0.
  // No final, você deve ter variáveis buffer_count.
  int *reception;
  if (rank == 0) {
    reception = new int[buffer_count];
  }

  // MPI_Reduce para somar todos os valores entre processos e armazenar o resultado no processo 0
  MPI_Reduce(buffer, reception, buffer_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // printa os resultados no processo 0
  if (rank == 0) {
    for (int i = 0; i < buffer_count; ++i)
      std::cout << reception[i] << std::endl;
    // Libere a memória alocada
    delete[] reception;
  }

  MPI_Finalize();
  
  return 0;
}