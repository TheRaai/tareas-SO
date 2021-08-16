# Tarea 1 Sistemas operativos
## Integrantes:
* Raimundo Moraga
* Romina Beretta

### Archivos editados:
* syscall.h
    * Acá se define un nuevo proceso que es ```` #define SYS_getprocs 22 ```` 
* defs.h
    * Acá se define ```int getprocs(void); ``` para definir la función de getprocs que luego será puesta en procs.c
* user.h
    * Acá se agrega getprocs como una system call ````int getprocs(void);````
* sysproc.c
    * Acá se define la función sys_getproc(), la cual será la función de sistema que llama a getprocs 
    ```C
    int sys_getprocs(void){
    return getprocs();
    }
    ```
* usys.S
    * Se define un ````SYSCALL(getprocs)```` 
* syscall.c
    * Se define sys_getprocs de tipo SYS_getprocs ````[SYS_getprocs]   sys_getprocs,```` y como un extern int  ````extern int sys_getprocs(void);```` para que funcione al llamarse dentro del sistema
* proc.c
    * Acá se crea la función principal, que trae el código que hace que funcione getprocs
    ````C
    int getprocs(void){
        struct proc *p;
        int count = 0;

        acquire(&ptable.lock);

        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->state != UNUSED) count++;
        }

        release(&ptable.lock);

        return count;
    }
    ````
* cuenta.c 
    * Archivo nuevo, que contiene una función main la cual está encargada de imprimir por consola cuantos procesos hay usados actualmente (función principal de nuestra nueva llamada al sistema)
    ````C
    int main(){
        printf(1,"Cantidad de procesos: %d\n" , getprocs());
        exit();
    }
    ````
* MAKEFILE
    * Acá se agregan dos líneas, ````_cuenta```` en UPROGS y ````cuenta.c```` en EXTRA para que cuando se llame a cuenta por terminal se corra el programa cuenta.c
