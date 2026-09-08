#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
#include <stdlib.h>
// *****************************************************************************
//                              Linked_List                
// *****************************************************************************
struct LinkedList *newRecord(unsigned int m, unsigned int h, double tempRead){//Add new node with minutes and hour
  struct LinkedList* newNode = (struct LinkedList*)malloc(sizeof(struct LinkedList)); //Create new LinkedList struct by allocating memory
  if (newNode == NULL) //Check if the newNode pointer is NULL
    return NULL;
  newNode -> minutes = m; //Put minutes to make temp time-stamped
  newNode -> hours = h; //Put hours
  newNode -> temperature = tempRead; //To read temp
  newNode -> next = NULL; //Set the next field of the newNode
  return newNode;
}

void insertFirst(struct LinkedList **first, struct LinkedList *el){ //Insert element at the first insertFirst
  //**first Pointer to a pointer to the first element
  //*el Pointer to a newly created element
  el -> next = (*first);
  (*first) = el;
}

void freeList(struct LinkedList **first){ //Free all list memory
  struct LinkedList *P1 = *first, *P2 = NULL; //p1 points to the first node of the list
  while (P1 != NULL) { //Loop that continues until the end of the list is reached 
    P2 = P1; //So that it points to the node that is being freed
    P1 = P1 -> next; //Set to point to the next node in the list before freeing
    free(P2); //Allocated for the node
  }
  *first = NULL;
}

double sum(struct LinkedList *first){ //Sum of temperature
  double sum = 0;
  struct LinkedList *current = first; 
  while (current){ //Loop that will iterate over every element in the linked list
    sum += current -> temperature; //Add the temperature value of the current element to the sum
    current = current -> next;
  }
  return sum;
}

double size(struct LinkedList *first){ //Calculate the number of mesured temperature
  unsigned int  size = 0; //To count the number of mesured temperature
  struct LinkedList *current = first; //First element of the linked list.
  while (current){ //Loop through each element of the linked list
    size++;
    current = current -> next;
  }
  return size;
}

struct LinkedList *getMin(struct LinkedList *first){ //To get minimum value from the list
  struct LinkedList *min = first, *current = first;
  while (current){ //Loop as long as there are still elements in the list
  if (current->temperature < min->temperature)
    min = current;
  current = current->next;
  }
  return min;
}

struct LinkedList *getMax(struct LinkedList *first){ //To get maximum value from the list
  struct LinkedList *max = first, *current = first;
  while (current) { //Loop as long as there are still elements in the list
    if (current->temperature > max->temperature)
      max = current;
    current = current->next;
  }
  return max;
}

