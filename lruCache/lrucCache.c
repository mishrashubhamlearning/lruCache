//user defined Header
#include "dataStructures.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*NOTE:
 *Queue which is cache memory and it has capacity equivalent to
 *total_frames. 
 * Hash in which cache page number, is used as key and node reference
 * as value. Thus whenever a new node created / updated refrence and 
 * key is updated accordingly. Capcity of Hash is maximum array value
 * possible, which is used as an Index, so the value of page number always
 * less than capacity of Hash.
 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//Method to create new Queue node and assign pagenumber to it.
static struct QNode* createNewNode(unsigned int page_num)
{
  struct QNode* node = (struct QNode*)calloc(1, sizeof(struct QNode));
  node->page_num = page_num;
  node->prev = node->next = NULL_NODE;  // Double Linked list node
  return node;
}

static struct Queue* createNewQueue( unsigned int total_frames )
{
  struct Queue* queue = NULL_Q;
  if( !( queue = (struct Queue*)calloc( 1, sizeof(struct Queue) ) ) ) {
      return NULL_Q;
    }
  //As Queue is created first time. So the queue is empty
  // Thus intialize counter with zero;
  // And front and rear with Null Ptr
  queue->counter = 0;
  queue->total_frames = total_frames;
  queue->rear = queue->front = NULL_NODE;
  return queue;
}

static struct Hash* createNewHash( unsigned int capcity )
{
  struct Hash* hash = NULL_HASH;
  if( !(hash = (struct Hash*)calloc(1, sizeof (struct Hash) ) ) ) {
      return NULL_HASH;
    }
  hash->capacity = capcity;
  hash->Q_array = (struct QNode**)calloc(hash->capacity, sizeof(struct QNode*));
  return hash;
}

static bool isQueueEmpty( struct Queue* q ) {
  return q->rear == NULL_NODE ? true : false;
}

static bool isQFramesAvailable( struct Queue* q) {
  return q->counter == q->total_frames ? false : true;
}

/*Deque: delete frame from memory*/
static void deQueue( struct Queue* q)
{
  if( !isQueueEmpty(q) ) {
      //1. if this is only node then change front and make front NULL;
      if( q->rear == q->front ) {
          q->front = NULL_NODE;
        }
      //2. change Rear pos, and remove the previous rear.
      struct QNode* node = q->rear;
      q->rear = q->rear->prev;
      //3. Now to check if rear is not null then make it next to NULL;
      if( q->rear ) {
          q->rear->next = NULL_NODE;
        }
      free( node );
      node = NULL_NODE;
      q->counter -=1; // Decrement Counter as Queue node is detached.
    }
  else {
      printf("\n%s--@--%d::Error Q is Empty!\n", __FUNCTION__, __LINE__ );
    }
}

/*Function to Enqueue and assign reference into Hash*/
static void enQueueAndAddRefrenceInHash( struct Queue* q,
                                         struct Hash* hash,
                                         unsigned int page_num)
{
  //1. check for frames, if not available then remove page from rear.
  if( !isQFramesAvailable(q) ) {
      //Remove reference from Hash and deqeue
      hash->Q_array[ q->rear->page_num ] = NULL_NODE;
      deQueue(q);
    }
  //Now create a new node and assign page number to it, then attach new node to the front of queue node1
  struct QNode* node = createNewNode( page_num );
  node->next = q->front;
  if( isQueueEmpty(q) ) {
      q->rear = q->front = node;
    }
  //If q is not empty, then change front node only.
  else {
      q->front->prev = node;
      q->front = node;
    }
  //Finally add reference of node into Hash.
  hash->Q_array[ page_num ] = node;
  q->counter +=1;
}

/*------Below mentioned 3 Application specific method to be called into main
 * application code file severally*
 *
 */
// This function is to be called, first to create hash and queue, with capacity and page count.
bool initalizeLRUCacheMemory(struct Hash** hash,
                             unsigned int hash_capacity,
                             struct Queue** queue,
                             unsigned int cache_page_count)
{
  *hash = createNewHash(hash_capacity);
  *queue = createNewQueue(cache_page_count);
  if( (!hash) || (!queue)) {
      return false;
    }
  return true;
}

/**************************************************************************
 * 1. If pagenumber not exist in memory bring it in memory and add  to the
 *    front of queue.
 * 2. If pagenumber does exist then simply move it to front of the memory.
 **************************************************************************/
void managePageInMemory(struct Queue* queue,
                        struct Hash* hash,
                        unsigned int cache_page_num)
{
  if( cache_page_num < hash->capacity) {
      //First check if page reference does not exist in hash then Enque it into Q.
      if( !hash->Q_array[ cache_page_num ] ) {
          enQueueAndAddRefrenceInHash(queue, hash, cache_page_num);
        }
      //Now if page (node) is there but not on front then bring it in front.
      else if( hash->Q_array[ cache_page_num ] != queue->front ) {
          hash->Q_array[ cache_page_num ]->prev->next = hash->Q_array[ cache_page_num ]->next;
          if( hash->Q_array[cache_page_num]->next ) {
              hash->Q_array[ cache_page_num ]->next->prev = hash->Q_array[ cache_page_num ]->prev;
            }
          // If requested node is rear then
          if( hash->Q_array[ cache_page_num ] == queue->rear ) {
              queue->rear = hash->Q_array[ cache_page_num ]->prev;
              queue->rear->next = NULL_NODE;
            }
          hash->Q_array[ cache_page_num ]->next = queue->front;
          hash->Q_array[ cache_page_num ]->prev = NULL_NODE;
          hash->Q_array[ cache_page_num ]->next->prev = hash->Q_array[ cache_page_num ];
          queue->front = hash->Q_array[ cache_page_num ];
        }
    }
}

//Print Page Number from Queue
void printQueue( struct Queue* queue)
{
  if( isQueueEmpty( queue )) {
      printf("\n Queue is Empty \n");
    }
  else {
      while(queue->front != NULL_NODE) {
          printf("Queue Page Number = %d\n", queue->front->page_num);
          queue->front = queue->front->next;
        }
    }
}
