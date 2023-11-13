#include <iostream>
#include <iomanip>
#include <mpi.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Le entrada para processo 0
  int n_elements;
  double *buffer;
  if (rank == 0) {
    std::cin >> n_elements;
    buffer = new double[n_elements];
    
    for (int i=0; i < n_elements; ++i)
      std::cin >> buffer[i];
  }
    

  // 1- Transmita o valor n_elements para todos os processos
  MPI_Bcast(&n_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);


    // Aqui criamos os buffers para os processos não-root, agora que
    //temos n_elementos
  if (rank != 0)
    buffer = new double[n_elements];

  // 2- Transmita o buffer para cada processo
  // NOTA: O tipo aqui deve ser MPI_DOUBLE e não MPI_FLOAT!
  MPI_Bcast(buffer, n_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  

  // 3- Calcule a soma dependendo da classificação
  double sum = 0.0;
  if (rank == 0) {
    // Process 0 sums all elements
    for (int i = 0; i < n_elements; ++i)
      sum += buffer[i];
  } else if (rank == 1) {
    // processo 1 soma elementos positivos
    for (int i = 0; i < n_elements; ++i)
      if (buffer[i] > 0)
        sum += buffer[i];
  } else if (rank == 2) {
    // processo 2 soma elementos negativos
    for (int i = 0; i < n_elements; ++i)
      if (buffer[i] < 0)
        sum += buffer[i];
  }
  // printa o resultado 
  std::cout << std::setprecision(16) << sum << std::endl;
  
  MPI_Finalize();
  delete [] buffer;

  return 0;
}
