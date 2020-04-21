#include "dataStructures.h"
int main()
{
  struct Hash *hash = NULL_HASH;
  struct Queue *queue = NULL_Q;
  if( !initalizeLRUCacheMemory(&hash, 15, &queue, 5)) {
      printf("\nInitialization Failed\n");

    }
  if( hash == NULL_HASH) {
      printf("HASH NULL");
    }
  if( queue == NULL_Q) {
      printf("Q NULL");
    }
  //Third argument is page number and it should be less than hash capacity always. 
  managePageInMemory(queue, hash, 7);
  managePageInMemory(queue, hash, 3);
  managePageInMemory(queue, hash, 3);
  managePageInMemory(queue, hash, 1);
  managePageInMemory(queue, hash, 5);
  managePageInMemory(queue, hash, 6);
  managePageInMemory(queue, hash, 3);
  managePageInMemory(queue, hash, 8);
  managePageInMemory(queue, hash, 6);
  managePageInMemory(queue, hash, 11);

 printQueue(queue);
  return 0;
}
