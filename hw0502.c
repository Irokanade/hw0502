#include <stdio.h>
#include <stdlib.h>  
#include <stdint.h>
#include <unistd.h>

int main() {
    FILE *procStat = NULL;
    char buffer[513] = {0};
    unsigned long int PrevTotal = 0;
    unsigned long int PrevIdle = 0;
    
    while(1) {
        procStat = popen("cat /proc/stat", "r");
    
        if(procStat == NULL) {
            printf("File open error >:[\n");
            return -1;
        }
    
    
        //user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
        char cpu[4] = {0};
        unsigned long int user;
        unsigned long int nice;
        unsigned long int system;
        unsigned long int idle;
        unsigned long int iowait;
        unsigned long int irq;
        unsigned long int softirq;
        unsigned long int steal;
        unsigned long int guest;
        unsigned long int guest_nice;
    
        fscanf(procStat ,"%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
        
        fprintf(stdout ,"%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu ", cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice);
        
        unsigned long int Idle = idle + iowait;
        unsigned long int NonIdle = user + nice + system + irq + softirq + steal;

        unsigned long int Total = Idle + NonIdle;
        
        //differentiate: actual value minus the previous one
        unsigned long int totald = Total - PrevTotal;
        unsigned long int idled = Idle - PrevIdle;
        
        double CPU_Percentage = (double)(totald - idled)/(double)totald;
        
        printf("CPU: %lf%%\n", CPU_Percentage*100);
        sleep(1);
        
        PrevTotal = Total;
        PrevIdle = Idle;
        
        pclose(procStat);
    }
        
    
    

    return 0;
}
