# Interação com o Usuário
- CSV de entrada:
  - Definição do formato (colunas)
  - Leitura do arquivo (produzindo o EventLog inicial)

- Parâmetros do timestamp (quantum) como command line argument
- Tempo de duração de cada tipo de IO como command line argument
  - Incluir tempos de duração padrão
- Prints relativos a cada evento de execução
  - Chegada de um novo processo
  - Entrada e saída de um processo da CPU
  - Encerramento da execução de um processo
  - Entrada na fila (dizendo a prioridade)
  - Entrada e saída de IO

A saída alterna entre print do estado atual das filas e processo em execução e print das frases que explicam o que foi realizado pelo escalonador nesse passo

# Estruturas de dados
- Enumeração dos tipos de IO
- Ação do escalonador

- Estrutura de dados do processo
  - Cada processo está associado a uma lista de pares de (tempo de processamento, próxima ação de IO)

- Estrutura de dados de fila de processos
- Estrutura de dados do evento
- Estrutura de dados do EventLog

- Associação da quantidade de tempo que demora cada IO

### Estrutura de dados do escalonador

  - Processos a entrar
  - Processos em cada fila
    - Fila de prontos
    - Cantinho do castigo de IO (Impressora, Disco, Fita)
    - Processo em execução

# Lógica de Execução
- Fixar regra de desempate pra IOs que acabam simultaneamente
  - PID menor ganha a prioridade

- Loop de escalonamento
  - (De u.t. em u.t)
  - Reagir ao próximo evento (EventLog) e às necessidades do processo em execução
    - A função de escalonar recebe uma referência ao estado atual, incluindo o EventLog (que vai ser alterado por mutação) e retorna uma lista de ações que foram realizadas
    - O loop de execução vai depois printar cada ação que foi realizada
  - Lista (ordenada no tempo) de eventos (aka EventLog):
    - Chegada de um programa (Carrega os dados do programa)
      - Ao entrar, o programa recebe o próximo PID em ordem sequencial
    - Hora de preemptar (Carrega o PID e o número da vez execução desse processo)

