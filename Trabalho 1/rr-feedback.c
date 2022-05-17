// --- Bibliotecas
#include <stdio.h>


// --- Defines
// Fatia de tempo do Round-Robin
#define QUANTUM 4

// Tipos de I/O diferentes
#define IO_TYPES 3
#define DISK 0
#define MAGNETIC_TAPE 1
#define PRINTER 2

// Durações de cada tipo de I/O
#define DISK_DURATION 4
#define MAGNETIC_TAPE_DURATION 8
#define PRINTER_DURATION 12

// Controles de estados
#define NONE -1
#define NEW 0
#define READY 1
#define RUNNING 2
#define WAITING 3
#define TERMINATED 4

// Prioridades
#define LOW 0
#define HIGH 1

// Maior valor possível para time_cpu e arrival
#define MAX_TIME_CPU 20
#define MAX_ARRIVAL 10


// --- Informações dos processos
// Estrutura do processo
typedef struct process{
    // PCB
    int pid;
    int status;
    int priority;

    // Execução
    int cpu_time;
    int arrival;
    int start_io[IO_TYPES];
    int duration_io[IO_TYPES];

    // Ponteiro de fila
    struct process* next;
} Process;

// Filas
Process* low_priority_queue = NULL;
Process* high_priority_queue = NULL;
Process* disk_queue = NULL;
Process* magnetic_tape_queue = NULL;
Process* printer_queue = NULL;


// --- Protótipos das funções
void add_process(Process* p, Process** queue);
Process* remove_process(Process** queue);
Process* get_running_process();

// --- Variáveis globais
// Variável auxiliar para contar o número de processos
int total_processes = 0;

// Variável auxiliar para contar o número de processos que terminaram
int terminated = 0;


// --- Código principal
int main(){
    
    int numero_processos;
    int time_slice = 0;

    Process* running_process = NULL; 
    Process* disk_process = NULL;
    Process* magnetic_tape_process = NULL;
    Process* printer_process = NULL;
    Process** processes_list = NULL;

    printf("Defina a quantidade de processos que deseja criar: ");
    scanf("%d", &numero_processos);

    // Criar numero_processos quantidade de processos aleatórios
    // ToDo

    // Imprime os processos criados
    // ToDo

    int start =  0;
    while(start >= 0){

        printf("--- Instante %d\n\n", start);
        
        // Adiciona processos novos na fila
         printf("> Novos:\n");

        // Verifica se há processos novos no instante start
        for (int i = 0; i < total_processes; i++){

            // Verifica se tem alguma chegada no instante t
            if (processes_list[i]->arrival == start){
                processes_list[i]->status = NEW;
                
                // Adiciona imediatamente na fila de alta prioridade
                processes_list[i]->status = READY;
                processes_list[i]->priority = HIGH;
                add_process(processes_list[i], &high_priority_queue);
            }
        }

        // Imprime filas
        printf("\n> Filas:\n");  
        // ToDo

        // Seleciona o processo a ser executado
        printf("\n> Selecao:\n");

        // Quando o time_slice for igual à 0, houve preempção, bloqueio ou término
        // Por isso precisamos de um novo processo
        if(time_slice == 0){
            // Pega o próximo processo a ser executado
            running_process = get_running_process();

            // Só restaura o time_slice caso tenha pego um processo
            if(running_process != NULL){
                time_slice = QUANTUM;
                printf(">> CPU selecionou: %d\n", running_process->pid);
            }
        }

        // Seleciona os processos das filas de I/O
        // Disco
        if (disk_process == NULL){
            // Remove processo da fila de fita
            disk_process = remove_process(&disk_queue);
            if(disk_process != NULL)
                printf(">> DISCO selecionou %d\n", disk_process->pid);
        }
            
        // Fila magnética
        if (magnetic_tape_process == NULL){
            // Remove processo da fila de fita
            magnetic_tape_process = remove_process(&magnetic_tape_queue);
            if(magnetic_tape_process != NULL)
                printf(">> FITA MAGNETICA selecionou %d\n", magnetic_tape_process->pid);
        }

        // Impressora 
        if (printer_process == NULL){
            // Remove processo da fila de fita
            printer_process = remove_process(&printer_queue);
            if(printer_process != NULL)
                printf(">> IMPRESSORA selecionou %d\n", printer_process->pid);
        }

        // Execução CPU
        printf("\n> Execucao:\n");

        // Se houver um processo a ser executado, realiza as operações da CPU
        if(running_process != NULL){
            printf(">> CPU executando %d (time slice = %d)\n", running_process->pid, time_slice - 1);
            // Executa processo
            // ToDo
        }
        else{
            printf(">> CPU ociosa\n");
        }

        // Se houver um processo a ser executado, realiza as operações de I/O
        // Disco
        if(disk_process != NULL){
            printf(">> DISCO executando: %d\n", disk_process->pid);
            // Executa processo
            // ToDo
        } else{
            printf(">> DISCO ocioso\n");
        }

        // Fita magnética
        if(magnetic_tape_process != NULL){
            printf(">> FITA MAGNETICA executando: %d\n", magnetic_tape_process->pid);
            // Executa processo
            // ToDo
        } else{
            printf(">> FITA MAGNETICA ociosa\n");
        }

        // Impressora
        if(printer_process != NULL){
            printf(">> IMPRESSORA executando: %d\n", printer_process->pid);
            // Executa processo
            // ToDo
        } else{
            printf(">> IMPRESSORA ociosa\n");
        }

        // Incrementa o tempo
        start++;

        // Verifica se todos os processos terminaram
        if(terminated == total_processes){
            printf("\n\nTodos os processos terminaram\n");
            printf("Levou %d instantes de tempo\n", start);
            start = -1;
        }

    }


    return 0;
}

// --- Funções auxiliares
// Adicionar processo em um fila
// Ex: Processo novo
// add_process(p1, &low_priority_queue)
void add_process(Process* p, Process** queue) {
    if(queue != NULL) p->next = *queue;
    *queue = p;
}

// Retirar o primeiro processo de uma fila
// Ex: preempcao
//  remove_process(&high_priority_queue) 
Process* remove_process(Process** queue) {
    if(*queue == NULL) return NULL;

    Process* head = *queue;
    Process* parent = head;

    // Se houver apenas 1 processo na fila
    if(head->next == NULL){
        *queue = NULL;
        return head;
    }
    
    // Se tem mais de 1 processo na fila
    while (head->next) {
        parent = head;
        head = head->next;
    }

    parent->next = NULL;

    return head;
}

// Retorna o ponteiro do proximo processo a ser executado. 
// Retorna NULL se nao houver processos nas filas.
Process* get_running_process(){

    Process* running_process = NULL;

    // Busca processo na fila de alta prioridade
    running_process = remove_process(&high_priority_queue);

    // Se estiver vazia, busca na fila de baixa prioridade
    if (running_process == NULL){
        running_process = remove_process(&low_priority_queue);
    }

    // Altera status do processo
    if (running_process != NULL)
        running_process->status = RUNNING;

    return running_process;
}