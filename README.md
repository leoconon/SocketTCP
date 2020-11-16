# ESP32 SocketTCP
Atividade da disciplina de IoT Aplicada da UniSATC.

Alunos:
* Leonardo Velho (@LeonardoCechellaVelho)
* Leonardo Nonnenmacher (@leoconon)

## Sobre o projeto
O objetivo do projeto é utilizar o ESP32 com FreeRTOS para abrir uma conexão socket TCP e devolver dados dos sensores quando requisitado. 
Além disso poderia ser feito o uso da tela LED para exibir as informações.

Em nosso desenvolvimento, utilizamos um sensor LDR para fazer medições de luminosidade de um local. Conforme o valor lido, na tela seria mostrado
o desenho de um sol ou de uma lua, representando "claro" e "escuro" respectivamente. Também são mostrados na tela um histograma com as últimas 
leituras feitas e um ícone indicando o status da conexão wifi.

Na conexão socket, ao receber o texto "GET", o ESP irá devolver as últimas 20 leituras realizadas e ainda não capturadas via socket no formato JSON.

## Executando
Para executar, é necessário através do menuconfig, configurar SSID e senha na sessão `Example Connection Configuration`. Após fazer build do projeto pode-se conectar com algum cliente de 
socket TCP através do IP indicado na tela, na porta 3333.

O pino associado ao sensor LDR está definido em `utils.h`.

## Código
O código fonte do sistema está distribuído em módulos, ou seja, arquivos que representam as principais funcionalidades necessárias.

### main.c
Arquivo principal, contendo a função `app_main()` que realiza as configurações e inicia as demais tarefas. 
Também possui a task que realiza as leituras dos sensores e altera o estado das filas que sincronizam as demais tasks do sistema.

### drawing.c
Arquivo contendo as tasks e funções responsáveis pelos desenhos na tela LED.

### wifi.c
Arquivo responsável pela configuração do wifi. Disponibiliza através do `wifiEventGroup` o atual estado da conexão e também o código do IP
através da variável `wifiIP`.

As configurações de wifi são chamadas na função principal do sistema:

```C
void app_main() {
    /*...*/
    initNvs();
    initWifi();
    /*...*/
}
```

### socket.c
Mantém a conexão socket TCP funcionando. Utiliza a Socket API da biblioteca lwIP e disponibiliza a task `void tcp_server_task(void *pvParameters)`
que faz as configurações do socket e abre um loop que fica aguardando por novas conexões. Ao receber o uma mensagem invoca a função
`void do_retransmit(const int sock)` que abre um novo loop aguardando por mensagens recebidas. Ao receber a mensagem "GET", o mesmo retorna o valor
das últimas 20 leituras lidas, disponibilizadas na fila `bufferLuminosityNotification`. Ao realizar a leitura, a fila é liberada, ou seja, se um usuário
requisitar os valores pela segunda vez (em um curto espaço de tempo) irá receber apenas os dados coletados entre a primeira e a segunda chamada.

As leituras são enviadas ao usuário no [formato JSON](https://www.json.org/json-en.html), para facilitar a comunicação entre outras aplicações.

### utils.c
Funções de ajuda para outros processos.

### global.h
Possui as variáveis e definições globais. As mais importantes são:

 * `bufferNewLuminosity`: Fila de uma posição, utilizada para sincronizar a tarefa das leituras e da escrita na tela;
 * `bufferLuminosityNotification`: Fila com os dados das 20 últimas leituras, utilizado para enviar ao usuário;
 * `wifiEventGroup`: EventGroupHandle com o status atual da conexão wifi;
 * `wifiIp`: Contém o IP da conexão wifi;
