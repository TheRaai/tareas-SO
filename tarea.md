# Tarea 1 Sistemas operativos
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
*  random.c 
    * Archivo nuevo, que contiene una función random y randomrange la cual está encargada de devolver numeros aleatorios, para poder sacar el ticket ganador de manera aleatoria (función principal de nuestro scheduler)
    ````C
    uint
    random(void)
    {
    // Take from http://stackoverflow.com/questions/1167253/implementation-of-rand
        static unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
        unsigned int b;
        b  = ((z1 << 6) ^ z1) >> 13;
        z1 = ((z1 & 4294967294U) << 18) ^ b;
        b  = ((z2 << 2) ^ z2) >> 27; 
        z2 = ((z2 & 4294967288U) << 2) ^ b;
        b  = ((z3 << 13) ^ z3) >> 21;
        z3 = ((z3 & 4294967280U) << 7) ^ b;
        b  = ((z4 << 3) ^ z4) >> 12;
        z4 = ((z4 & 4294967168U) << 13) ^ b;

        return (z1 ^ z2 ^ z3 ^ z4) / 2;
    }

    // Return a random integer between a given range.
    int
    randomrange(int lo, int hi)
    {
        if (hi < lo) {
            int tmp = lo;
            lo = hi;
            hi = tmp;
    }
    int range = hi - lo + 1;
    return random() % (range) + lo;
    }
    ````
* MAKEFILE
    * Acá se agregan tres líneas, ````_lotterytest```` en UPROGS y ````lotterytest.c```` en EXTRA para que cuando se llame a cuenta por terminal se corra el programa lotterytest, además de agregar ``` random.o``` en los OBJS 
