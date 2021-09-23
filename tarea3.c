#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include<stdbool.h>

typedef struct node{
    int data;
    struct node* next;
    pthread_mutex_t lock;
} node;

void print_list(node* head)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        printf("%d ", cursor->data);
        cursor = cursor->next;
    }
}

node* create(int data,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;
 
    return new_node;
}

node* append(node* head, int data)
{
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
    /* create a new node */
    node* new_node =  create(data,NULL);
    cursor->next = new_node;
    return head;
}

node* remove_front(node* head)
{
    if(head == NULL)
        return NULL;
    node *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    return head;
}

node* remove_back(node* head)
{
    if(head == NULL)
        return NULL;
 
    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }
    if(back != NULL)
        back->next = NULL;
 
    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;
 
    free(cursor);
 
    return head;
}

typedef struct{
  node* ListHead;
  int value;
}argumentos;

typedef struct{
    pthread_t thread;
}threads;

void* append_thread(void* arg)
{
    argumentos *a = (argumentos*) arg;
    node *prev = a->ListHead;
    while (prev->next != NULL) {
      prev= prev->next;
    }
    pthread_mutex_lock(&prev->lock);
    node* new_node =  create(a->value,NULL);
    prev->next = new_node;
    pthread_mutex_unlock(&prev->lock);
    //pthread_exit((void*)prev);
    return(void*)a->ListHead;
}


int main(void) {
  int counter=0,counter2=0;
  node* head = create(0,NULL);
  // inserte código para agregar 100 nodos
  // primero secuencialmente (usando un ciclo)
  // luego en paralelo
  for(int i =1;i<100;i++){
    head = append(head,i);
  }
  node* temp = head;
  while(temp!=NULL){
    counter++;
    temp=temp->next;
  }
  argumentos b[100];
  node head2 = {0,0,PTHREAD_MUTEX_INITIALIZER};
  pthread_t p;
  for(int i =1;i<100;i++){
    //pthread_mutex_init(&b[i].ListHead->lock,0);
    b[i].ListHead = &head2;
    b[i].value = i;
    pthread_create(&p,NULL,append_thread,&b[i]);
    pthread_join(p,NULL);
  }

  node* temp2  = &head2;
  while(temp2!=NULL){
    counter2++;
    temp2=temp2->next;
  }
  printf("Lista hecha con while de largo: %d\n",counter);
  print_list(head);
  printf("\nLista hecha con threading con largo: %d\n",counter2);
  print_list(&head2);
  printf("\n");
  return 0;
}