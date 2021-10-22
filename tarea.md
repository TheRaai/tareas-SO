# Tarea 2 Sistemas operativos
## Integrantes:
* Raimundo Moraga
* Romina Beretta

### Archivos editados:
* syscall.h
    * Acá se define un nuevo proceso que es ```` #define SYS_random ```` y un SYS_lotterytest
* defs.h
    * Acá se define ```int lotterytest(void); ``` para definir la función de lotterytest que luego será puesta en procs.c  además de definir las funciones de random.c
* user.h
    * Acá se agrega random como una system call ````int random(void);```` y el lotterytest
* sysproc.c
    * Acá se define la función sys_random(), la cual será la función de sistema que llama a random
    ```C
    int sys_random(void){
      return random();
    }
    ```
    además de un sys_lotterytest que retorna lotterytest
* usys.S
    * Se define un ````SYSCALL(random)```` y lotterytest 
* syscall.c
    * Se define sys_random de tipo SYS_random ````[SYS_random]   sys_random,```` y como un extern int  ````extern int sys_random(void);```` para que funcione al llamarse dentro del sistema, y lo mismo para el lotterytest
* proc.c
    * Acá se crea la función principal, que trae el código que hace que funcione el lotteryscheduler
    ````C
    void scheduler(void){
    struct proc *p;
    struct cpu *c = mycpu();
    c->proc = 0;

        for(;;){
        // Enable interrupts on this processor.
        sti();

    // Loop over process table looking for process to run.
        acquire(&ptable.lock);
        uint total = 1000;
        int counter = 0;
        int  winner = randomrange(1,total);
        //lotery(counter,winner);
        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->state != RUNNABLE)
            continue;
        if ((counter + p->num_tickets) < winner){
            counter += p->num_tickets;
            }
        // Switch to chosen process.  It is the process's job
        // to release ptable.lock and then reacquire it
        // before jumping back to us.
        c->proc = p;
        switchuvm(p);
        p->state = RUNNING;

        swtch(&(c->scheduler), p->context);
        switchkvm();

        // Process is done running for now.
        // It should have changed its p->state before coming back.
        c->proc = 0;
        }
        release(&ptable.lock);

        }
    }   
    ````
*  traductor.c
    * Acá se invoca la llamada, para imprimir
* MAKEFILE
    * Acá se agregan tres líneas, ````_lotterytest```` en UPROGS y ````lotterytest.c```` en EXTRA para que cuando se llame a cuenta por terminal se corra el programa lotterytest, además de agregar ``` random.o``` en los OBJS 
