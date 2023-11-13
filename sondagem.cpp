void probing_process(int &int_sum, float &float_sum) {
    MPI_Status status;
  
  // 1- Sonde a mensagem recebida
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    int tag = status.MPI_TAG;
    int source = status.MPI_SOURCE;

  // Printa a msg
    std::cout << "Received a message from process " << source << " with tag " << tag << std::endl;

  //  Adicione a int_sum ou float_sum dependendo da tag da mensagem
    if (tag == 0) {
	    int received;
      MPI_Recv(&received, 1, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    int_sum += received;
    }else {
	    float received;
      MPI_Recv(&received, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    float_sum += received;
    }
}
