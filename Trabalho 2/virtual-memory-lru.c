// --- Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

// --- Defines
#define NUM_PROCESS 20
#define VIRTUAL_PAGES_PER_PROCESS 50
#define MEMORY_SIZE 64
#define WORKING_LIMIT 4

// --- Structs
typedef struct Process{
    int id;
    int pagesOnMemory;
    int virtualPageTable[VIRTUAL_PAGES_PER_PROCESS];
} Process;

typedef struct Page{
    int process_id;
    int number;
    struct Page *next;
} Page;

typedef struct LRU{
    Page *start;
    int size;
} LRU;

// --- Global variables

// Number of frames being used on memory
int usedPages = 0;

// List of processes
struct Process process_list[NUM_PROCESS];

// Main memory that is a list of frames
struct Page memory[MEMORY_SIZE];

// Linked list for LRU. The least recently used page is kept at the end
struct LRU page_list;

// --- Auxiliary functions

// Creates a new process by adding it to the process list and initialize the process virtual page table
// The virtual page table is an array that tells us if the page is on memory or not
void create_process(int process_id){
    for(int i=0;i<VIRTUAL_PAGES_PER_PROCESS;i++){
        process_list[process_id].virtualPageTable[i] = -1;
    }
    process_list[process_id].id = process_id;
    printf("Created process with ID %d\n\n", process_id);
}

// Prints the process virtual page table
void print_process_virtual_table(int process_id){
    printf("\nPROCESS %d VIRTUAL PAGE TABLE\n", process_id);
    printf("Page number\tMemory location\n");
    printf("-------------------------------\n");
    for(int i=0;i<VIRTUAL_PAGES_PER_PROCESS;i++){
        printf("%d\t\t%d\n", i, process_list[process_id].virtualPageTable[i]);
        printf("-------------------------------\n");
    }
    printf("\n\n");
}

// Checks the process virtual page table to see if a page is on memory
bool isPageOnMemory(int page_number, int process_id){
    if(process_list[process_id].virtualPageTable[page_number] == -1){
        return 0;
    } else{
        return 1;
    }
}

// Inserts the process at the start of the LRU list
// If the process is already on the linked list, move it to the start
void updateLRU(int page_number, int process_id){  
    Page *move = (Page*)malloc(sizeof(Page));
    Page *aux = page_list.start;

    if(aux){
        // If LRU is not empty
        // Check if the page to be inserted is not at the start
        // If it is at the start, we don't have to move it
        if(!(aux->process_id == process_id && aux->number == page_number)){
            // Checks if the page to be inserted is on the list
            while(aux->next != NULL && !(aux->next->process_id == process_id && aux->next->number == page_number)){
                aux = aux->next;
            }
            if(aux->next){
                // If page is on the list, change the next pointer and move the node to the start
                move = aux->next;
                aux->next = move->next;
                move->next = page_list.start;
                page_list.start = move;
            } else{
                // If page is not on the list, add it to the start
                move->number = page_number;
                move->process_id = process_id;
                move->next = page_list.start;
                page_list.start = move;
                page_list.size++;
            }
        }
    } else{
        // If LRU list is empty, add the first page
        move->number = page_number;
        move->process_id = process_id;
        page_list.start = move;
        move->next = NULL;
        page_list.size++;
    }
}

// Adds the page to the memory list and updates the process virtual page table
// Looks for a place in memory where process_id is -1
void addPageToMemory(int page_number, int process_id){
    int i;

    for(i=0;i<MEMORY_SIZE;i++){
        if(memory[i].process_id == -1){
            memory[i].process_id = process_id;
            memory[i].number = page_number;
            break;
        }
    }
    printf("Allocated in memory to frame %d.\n", i);
    process_list[process_id].virtualPageTable[page_number] = i;
    process_list[process_id].pagesOnMemory++;
}

// Removes the page from memory by changing the process_id to -1
void removePageFromMemory(Page *remove){
    for(int i=0;i<usedPages;i++){
        if(memory[i].process_id == remove->process_id && memory[i].number == remove->number){
            memory[i].process_id = -1;
            break;
        }
    }
}

// Removes a page from the LRU linked list
// Mode 0 means that the working set limit of the process if full, so we have to remove the process least recently used page
// Mode 1 means that the memory is full and we need to remove the least recently used page
Page *removeFromLRU(int page_number, int process_id, int mode){
    Page *remove = (Page*)malloc(sizeof(Page));
    Page *aux = page_list.start;
    int counter = 0;

    if(!mode){
        if(aux->process_id == process_id) counter++;
        while(aux->next != NULL){
            if(aux->next->process_id == process_id){
                counter++;
            }
            if(counter == WORKING_LIMIT) break;
            aux = aux->next;
        }

        if(aux->next){
            remove = aux->next;
            aux->next = aux->next->next;
        }
        printf("Removing page %d that belongs to process %d. Motive: working set limit full.\n", remove->number, remove->process_id);
    } else{
        while(aux->next->next != NULL){
            aux = aux->next;
        }
        remove = aux->next;
        aux->next = NULL;

        printf("Removing page %d that belongs to process %d. Motive: memory full.\n", remove->number, remove->process_id);
        process_list[remove->number].pagesOnMemory--;
    }

    process_list[remove->number].virtualPageTable[remove->process_id] = -1;

    return remove;
}

void request_memory(){
    Page *aux = (Page*)malloc(sizeof(Page));
    for(int i = 0; i<NUM_PROCESS;i++){
        if(process_list[i].id != -1){
            int randomPage = rand() % VIRTUAL_PAGES_PER_PROCESS;
            printf("Process %d requested memory for page %d\n", i, randomPage);
            if(!isPageOnMemory(randomPage, i)){
                if(process_list[i].pagesOnMemory == WORKING_LIMIT){
                    aux = removeFromLRU(randomPage, i, 0);
                    removePageFromMemory(aux);
                    addPageToMemory(randomPage, i);
                    updateLRU(randomPage, i);
                } else{
                    if(usedPages == MEMORY_SIZE){
                        aux = removeFromLRU(randomPage, i, 1);
                        removePageFromMemory(aux);
                        addPageToMemory(randomPage, i);
                        updateLRU(randomPage, i);
                    } else{
                        addPageToMemory(randomPage, i);
                        updateLRU(randomPage, i);
                        usedPages++;
                    }
                }
            } else{
                printf("Page is already on memory.\n\n");
                updateLRU(randomPage, i);
            }
            print_process_virtual_table(i);
        } else{
            break;
        }
    }
}

void initialize(){
    int i;

    //LRU page list
    page_list.start = NULL;
    page_list.size = 0;

    //Process list
    for(i=0;i<NUM_PROCESS;i++){
        process_list[i].id = -1;
        process_list[i].pagesOnMemory = 0;
    }

    //Memory
    for(i=0;i<MEMORY_SIZE;i++){
        memory[i].process_id = -1;
    }
}

int main(){
    int i;

    srand(time(NULL));

    initialize();    

    for(i=0;i<10;i++){
        if(i<NUM_PROCESS){
            create_process(i);
        }
        request_memory();
        printf("Memory used: %d\n\n", usedPages);
        sleep(3);
    }

    printf("MEMORY\n");
    printf("__________________________________________________\n");
    printf("\tFrame\t\tProcess\t\tPage\n");
    printf("__________________________________________________\n");
    for(i=0;i<MEMORY_SIZE;i++){    
        printf("\t%d\t\t%d\t\t%d\n", i, memory[i].process_id, memory[i].number);
        printf("__________________________________________________\n");
    }

    return 0;
}
