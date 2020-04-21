
/*********************************************************************
 * Data structure API File. This contain declaration of Queue using
 * Doubly linked list and HASH.
 * A double list node stores, reference and page number.
 * A complete Queue (collection of Queue node {Double linked list
 * instances} contains number of frames used and total number of
 * frames, and finally front and rear pointer of queue.
 * HASH DS is used define capacity (How many pages can be possible)
 * And an array of double linked list pointer.
 ********************************************************************
 */

#ifndef SHKM_DATA__STRUCTURE_H__
#define SHKM_DATA__STRUCTURE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//NULL utility MACRO
#define NULL_NODE (struct QNode*)0
#define NULL_Q  (struct Queue*)0
#define NULL_HASH (struct Hash*)0

/*DOUBLE LINKED LIST,  represent single NODE of Queue */
struct QNode{
  struct QNode *next, *prev;
  unsigned int page_num;
};

/*Now using Above QNode instance, we declare Queue*/
struct Queue { 
  unsigned int counter;  // This keeps track of created nodes.
  unsigned int total_frames; // Maximum Node frames that can be stored in memory.
  struct  QNode *front, *rear;
};

/* HASH which is a table stores an array of Qnodes and maximum capacity*/
struct Hash {
  unsigned int capacity;
  struct QNode** Q_array;
};

bool initalizeLRUCacheMemory(struct Hash** hash, unsigned int hash_capacity, struct Queue** queue, unsigned int cache_page_count);
void managePageInMemory(struct Queue* queue, struct Hash* hash, unsigned int cache_page_num);
void printQueue( struct Queue* queue);
#endif
