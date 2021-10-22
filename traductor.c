#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc,char *argv[]){
  char *virtual_addr;
  virtual_addr=argv[1];

  printf(1,"Direccion virtual dada: %s\n",virtual_addr);
  traductor(virtual_addr);
  exit();
}