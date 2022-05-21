// --- Bibliotecas
#include <stdio.h>
#include <stdlib.h>


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

// Maior valor possível para cpu_time e arrival
#define MAX_CPU_TIME 20
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
Process* init_process(int cpu_time, int arrival, int* start_io);
Process** generate_processes(); // >>> Excluir ao entregar atividade
void print_process(Process* process);
void print_queue(Process** queue, char* name);


// --- Variáveis globais
// Variável auxiliar para contar o número de processos
int total_processes = 0;

// Variável auxiliar para contar o número de processos que terminaram
int terminated = 0;


// --- Fluxo principal
int main(){
    
    int numero_processos;
    int time_slice = 0;

    Process* running_process = NULL; 
    Process* disk_process = NULL;
    Process* magnetic_tape_process = NULL;
    Process* printer_process = NULL;
    Process** processes_list = NULL;

    // printf("Defina uma quantidade de processos entre 1 e 5 que deseja criar: ");
    // scanf("%d", &numero_processos);

    // while((numero_processos < 1) || (numero_processos > 5)){
    //     printf("Quantidade de processos deve ser um valor inteiro entre 1 e 5.\n");
    //     printf("Defina uma quantidade de processos entre 1 e 5 que deseja criar: ");
    //     scanf("%d", &numero_processos);
    // }

    // >>> Excluir ao entregar atividade
    // Cria 5 processos
    printf("Criando 5 processos...\n");
    processes_list = generate_processes();

    for(int i = 0; i < 5; i++){
        print_process(processes_list[i]);
    }
    
    // Criar numero_processos quantidade de processos aleatórios
    // printf("Criando %d processos...\n", numero_processos);
    // ToDo

    // Imprime os processos criados
    // for(int i = 0; i < numero_processos; i++){
    //     print_process(processes_list[i]);
    // }

    int start =  0;
    while(start >= 0){

        printf("\n\n--- Instante %d\n\n", start);
        
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

                printf("- %d\n", processes_list[i]->pid);
            }
        }

        // Imprime filas
        printf("\n> Filas:\n");  
        print_queue(&low_priority_queue, "CPU Low");
        print_queue(&high_priority_queue, "CPU High");
        print_queue(&disk_queue, "DISK ");
        print_queue(&magnetic_tape_queue, "TAPE ");
        print_queue(&printer_queue, "PRINT");
        printf("\n");

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
            // Remove processo da fila
            disk_process = remove_process(&disk_queue);
            if(disk_process != NULL)
                printf(">> DISCO selecionou %d\n", disk_process->pid);
        }
            
        // Fila magnética
        if (magnetic_tape_process == NULL){
            // Remove processo da fila
            magnetic_tape_process = remove_process(&magnetic_tape_queue);
            if(magnetic_tape_process != NULL)
                printf(">> FITA MAGNETICA selecionou %d\n", magnetic_tape_process->pid);
        }

        // Impressora 
        if (printer_process == NULL){
            // Remove processo da fila
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
        } else{
            printf(">> CPU ociosa\n");
        }

        // Se houver um processo a ser executado, realiza as operações de I/O
        // Disco
        if(disk_process != NULL){
            printf(">> DISCO executando: %d\n", disk_process->pid);
            // Executa I/O
            // ToDo
        } else{
            printf(">> DISCO ocioso\n");
        }

        // Fita magnética
        if(magnetic_tape_process != NULL){
            printf(">> FITA MAGNETICA executando: %d\n", magnetic_tape_process->pid);
            // Executa I/O
            // ToDo
        } else{
            printf(">> FITA MAGNETICA ociosa\n");
        }

        // Impressora
        if(printer_process != NULL){
            printf(">> IMPRESSORA executando: %d\n", printer_process->pid);
            // Executa I/O
            // ToDo
        } else{
            printf(">> IMPRESSORA ociosa\n");
        }

        // Preenche as tabelas a cada passo do Round-Robin
        // ToDo

        // Incrementa o tempo
        // start++;

        // >>> Excluir quando entregar a atividade
        start = -1; // debug

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
// Ex: Preempção
// remove_process(&high_priority_queue) 
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
// Retorna NULL se não houver processos nas filas.
Process* get_running_process(){

    Process* running_process = NULL;

    // Busca processo na fila de alta prioridade
    running_process = remove_process(&high_priority_queue);

    // Se estiver vazia, busca na fila de baixa prioridade
    if (running_process == NULL){
        running_process = remove_process(&low_priority_queue);
    } else{
        running_process->status = RUNNING;
    }

    return running_process;
}

// Cria um processo com as informações passadas
Process* init_process(int cpu_time, int arrival, int* start_io) {

    int i;

    // Inicializa um processo
    Process* p = (Process*) malloc(sizeof(Process));

    // Processo recebe suas informações
    p->pid = total_processes++;
    p->status = NONE;
    p->priority = HIGH;
    p->cpu_time = cpu_time;
    p->arrival = arrival;

    // Inicializa os arrays de I/O
    if(start_io == NULL){
        for (i = 0; i < IO_TYPES; i++)
            p->start_io[i] = -1;
    } else{
        for(int i = 0; i < IO_TYPES; i++){
            // Descobre o tempo de inicio de I/O relativo ao tempo de serviço restante
            start_io[i] = cpu_time - start_io[i];
           
            // Se o tempo relativo for maior ou igual ao tempo de serviço, então não é válido
            if (start_io[i] >= cpu_time){
                p->start_io[i] = -1;
            } else{
                p->start_io[i] = start_io[i];
            } 
        }
    }

    p->duration_io[DISK] = DISK_DURATION;
    p->duration_io[MAGNETIC_TAPE] = MAGNETIC_TAPE_DURATION;
    p->duration_io[PRINTER] = PRINTER_DURATION;
    
    // Processo começa fora de qualquer fila
    p->next = NULL;

    return p;
}

// >>> Excluir ao entregar atividade
// Cria uma lista de 5 processos
Process** generate_processes() {

    // Cria a lista de processos
    Process** processes_list = (Process**) malloc(5 * sizeof(Process*));

    // Valores para I/O
    int start_io0[IO_TYPES] = { 4, -1, -1 };
    int start_io4[IO_TYPES] = { -1, 2, 5 };

    processes_list[0] = init_process(8, 1, start_io0);
    processes_list[1] = init_process(3, 2, NULL);
    processes_list[2] = init_process(10, 4, NULL);
    processes_list[3] = init_process(1, 4, NULL);
    processes_list[4] = init_process(2, 11, start_io4);
    
    return processes_list;
}

// Imprime as características de um processo
void print_process(Process* process) {

    printf("\nProcesso %d:\n", process->pid);

    printf("\tStatus = ");
    switch (process->status) {
        case NONE:
            printf("NONE\n");
            break;
        case NEW:
            printf("NEW\n");
            break;
        case READY:
            printf("READY\n");
            break;
        case RUNNING:
            printf("RUNNING\n");
            break;
        case WAITING:
            printf("WAITING\n");
            break;
        case TERMINATED:
            printf("TERMINATED\n");
            break;
    }

    printf("\tPriority = ");
    switch (process->priority) {
        case LOW:
            printf("LOW\n");
            break;
        case HIGH:
            printf("HIGH\n");
            break;
    }

    printf("\tCPU Time = %d\n", process->cpu_time);
    printf("\tArrival = %d\n", process->arrival);

    for (int i = 0; i < IO_TYPES; i++) {
        if ((process->start_io[i] >= 0) && (process->duration_io[i] > 0)) {
            switch (i) {
                case DISK:
                    printf("\tDisk:\n");
                    printf("\t\tStart = %d\n", process->start_io[i]);
                    printf("\t\tDuration = %d\n", process->duration_io[i]);
                    break;
                case MAGNETIC_TAPE:
                    printf("\tMagnetic Tape:\n");
                    printf("\t\tStart = %d\n", process->start_io[i]);
                    printf("\t\tDuration = %d\n", process->duration_io[i]);
                    break;
                case PRINTER:
                    printf("\tPrinter:\n");
                    printf("\t\tStart = %d\n", process->start_io[i]);
                    printf("\t\tDuration = %d\n", process->duration_io[i]);
                    break;
            }
        }
    }
}

// Mostra os processos em um fila
// Ex: print_queue(&printer_queue)
void print_queue(Process** queue, char* name) {

    printf("- Fila %s: \n", name);
    
    if(queue == NULL) return;
    
    Process* head = *queue;
    
    while (head != NULL) {
        printf("%d \n", head->pid);
        head = head->next;
    }
    
    printf("\n");
}