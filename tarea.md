# Tarea 2 Sistemas operativos
## Integrantes:
* Raimundo Moraga
* Romina Beretta

### Archivos editados:
Primero se editan distintos archivos para agregar parámetros de una llamada a sistema.
* syscall.h

* defs.h

* user.h

* sysproc.c

* usys.S

* syscall.c

* MAKEFILE

Luego se edita el archivo principal donde queda el código del traductor.
* proc.c
    * Acá se crea la función principal, que trae el código que hace que funcione el traductor
    ````C
    int traductor(char* virtual_address){
        char* paddr;
        int pad;
        pde_t *pgdir,*pgtab,*pde,*pte;
        //struct proc *p = ptable.proc;
        struct proc *curproc = myproc();
        
        pgdir = curproc->pgdir;
        pde = &pgdir[PDX(virtual_address)];
        cprintf("Pde: %p\n",pde);

        if(*pde & PTE_P){
            pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
        }
        else{
            cprintf("Direccion invalida\n");
            return -1;
        }

        pte = &pgtab[PTX(virtual_address)];
        paddr = (char*)V2P(PTE_ADDR(*pte));
        pad = (PTE_ADDR(*pte));
        cprintf("Direccion fisica: %d\n",pad);
        cprintf("Direccion fisica dada: %p\n",paddr);

        return 0;
    }
    ````
*  traductor.c
    * Acá se invoca la llamada, para imprimir las direcciones virtuales y fisica

