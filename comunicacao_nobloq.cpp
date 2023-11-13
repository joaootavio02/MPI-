void play_non_blocking_scenario() {
  MPI_Request request;
  MPI_Status status;
  int request_finished = 0;

  // Inicializando o buffer:
  for (int i = 0; i < buffer_count; ++i)
    buffer[i] = (rank == 0 ? i * 2 : 0);

  MPI_Barrier(MPI_COMM_WORLD);
  // Iniciando o cronômetro
  double tempo = -MPI_Wtime(); // Este comando nos ajuda a medir o tempo. Veremos mais sobre isso mais tarde!

  ////////// Você não deve modificar nada ANTES deste ponto //////////

  if (rank == 0) {
    sleep(3);

    // 1- Inicializar o envio não bloqueante para o processo 1
    MPI_Isend(buffer, buffer_count, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

    double tempo_restante = 6000.0;
    while (tempo_restante > 0.0) {
      usleep(1000); // Trabalhamos por 1ms

      // 2- Testar se a solicitação foi concluída (apenas se ainda não estiver concluída)
      if (!request_finished)
        MPI_Test(&request, &request_finished, &status);

      // 1ms restante para trabalhar
      tempo_restante -= 1000.0;
    }

    // 3- Se a solicitação ainda não estiver completa, espere aqui.
    if (!request_finished)
      MPI_Wait(&request, &status);

    // Modificando o buffer para a segunda etapa
    for (int i = 0; i < buffer_count; ++i)
      buffer[i] = -i;

    // 4- Preparar outra solicitação para o processo 1 com uma tag diferente
    MPI_Isend(buffer, buffer_count, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);

    tempo_restante = 3000.0;
    while (tempo_restante > 0.0) {
      usleep(1000); // Trabalhamos por 1ms

      // 5- Testar se a solicitação foi concluída (apenas se ainda não estiver concluída)
      if (!request_finished)
        MPI_Test(&request, &request_finished, &status);

      // 1ms restante para trabalhar
      tempo_restante -= 1000.0;
    }
    // 6- Esperar que seja concluído
    if (!request_finished)
      MPI_Wait(&request, &status);
  } else {
    // Trabalhar por 5 segundos
    sleep(5);

    // 7- Inicializar o recebimento não bloqueante do processo 0
    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

    // 8- Aguarde aqui a conclusão da solicitação
    MPI_Wait(&request, &status);

    print_buffer();

    // Trabalhar por 3 segundos
    sleep(3);

    // 9- Inicializar outro recebimento não bloqueante
    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);

    // 10- Aguarde até que seja concluído
    MPI_Wait(&request, &status);
    print_buffer();
  }
  ////////// Você não deve modificar nada DEPOIS deste ponto //////////

  // Parando o cronômetro
  tempo += MPI_Wtime();

  // Esta linha nos dá o tempo máximo decorrido em cada processo.
  // Veremos sobre redução mais tarde!
  double tempo_final;
  MPI_Reduce(&tempo, &tempo_final, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
    std::cout << "Tempo total para o cenário não bloqueante: " << tempo_final << "s" << std::endl;
}
