// Você tem acesso às seguintes variáveis globais:
int world_rank, world_size;
MPI_Comm custom_comm1, custom_comm2, custom_comm3, tmp;

// O comunicador tmp é fornecido se você precisar apontar para algum lugar, onde você não
// se importa com o comunicador resultante. Por exemplo, ao usar MPI_UNDEFINED como cor.

// world_rank: posição do processo em MPI_COMM_WORLD
// world_size: tamanho de MPI_COMM_WORLD
// Essas duas variáveis já estão inicializadas quando a função splitting() é chamada.

void splitting() {
    // Define uma variável de cor para MPI_Comm_split
    int color;

    // 1- Primeira divisão aqui
    // Processos 0-3 em custom_comm1 e processos 4-6 em custom_comm2
    if (world_rank >= 0 && world_rank <= 3) {
        color = 1; // Cor para custom_comm1
        MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &custom_comm1);
    } else if (world_rank >= 4 && world_rank <= 6) {
        color = 2; // Cor para custom_comm2
        MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &custom_comm2);
    } else {
        // Processo não faz parte de custom_comm1 ou custom_comm2
        MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, world_rank, &tmp);
    }

    // 2- Segunda divisão aqui
    // Processos 0 e 4 em custom_comm3
    if (world_rank == 0 || world_rank == 4) {
        color = 3; // Cor para custom_comm3
        MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &custom_comm3);
    } else {
        // Processo não faz parte de custom_comm3
        MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, world_rank, &tmp);
    }
}
