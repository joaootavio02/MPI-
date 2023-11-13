### A tag é um número inteiro simples que identifica o “tipo” de comunicação. Este é um valor completamente informal que você mesmo atribui
-----------------------------------------------------------------------------------------------------------------------
#  struct MPI_Status {
      int MPI_SOURCE;   // Origem da mensagem
      int MPI_TAG;      // Tag da mensagem
      int MPI_ERROR;    // Indica erro durante a recepção
      int _cancelled;   // Não deve ser acessado por aplicativos MPI
      size_t _ucount;   // Não deve ser acessado por aplicativos MPI
-----------------------------------------------------------------------------------------------------------------------
# MODOS DE COMUNICAÇÃO
1. **Padrão (MPI_Send)**
    - **Descrição:** O MPI escolhe automaticamente o modo de comunicação.
    - **Comportamento Observado (OpenMPI):** Mensagens curtas podem ser automaticamente bufferizadas, enquanto mensagens longas podem ser enviadas de maneira semelhante ao modo síncrono.

2. **Bufferizado (MPI_Bsend)**
    - **Descrição:** Armazena todos os dados a serem enviados em um buffer temporário e retorna à execução imediatamente.
    - **Vantagem:** A execução continua mesmo se o Recv correspondente ainda não foi chamado.
    - **Desvantagem:** Consome mais memória, pois duplica os dados.

3. **Pronto (MPI_Rsend)**
    - **Descrição:** Só pode começar se o Recv correspondente já foi chamado. Ganha tempo na inicialização das mensagens.
    - **Cuidado:** Se o Recv correspondente não foi chamado, a mensagem recebida pode ser indefinida.

4. **Síncrono (MPI_Ssend)**
    - **Descrição:** Espera pelo Recv correspondente para completar. A transferência de dados ocorre exatamente nesse momento.
    - **Garantia:** Ambos os processos estão prontos para a transferência.
-----------------------------------------------------------------------------------------------------------------------
# COMUNICAÇÃO COLETIVA
 **Broadcast:**
    - **Descrição:** Um processo envia uma mensagem para todos os outros processos.

 **Redução:**
    - **Descrição:** Um processo obtém dados de todos os outros processos e aplica uma operação neles (soma, mínimo, máximo, etc.).

 **Scatter:**
    - **Descrição:** Um único processo particiona os dados para enviar partes para todos os outros processos.

 **Gather:**
    - **Descrição:** Um único processo reúne os dados de diferentes processos em um buffer.
-----------------------------------------------------------------------------------------------------------------------
# Tabela
| Operação MPI | Descrição                        |
|--------------|----------------------------------|
| MPI_MAX      | Valor máximo                     |
| MPI_MIN      | Valor mínimo                     |
| MPI_SUM      | Soma                             |
| MPI_PROD     | Produto                          |
| MPI_LAND     | Lógico E                          |
| MPI_BAND     | Bit a bit E                       |
| MPI_LOR      | Lógico OU                         |
| MPI_BOR      | Bit a bit OU                      |
| MPI_LXOR     | XOR lógico                        |
| MPI_BXOR     | XOR bit a bit                     |
| MPI_MAXLOC   | Valor máximo e localização        |
| MPI_MINLOC   | Valor mínimo e localização        |

