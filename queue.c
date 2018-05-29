/**
 * Terrible Threads Lab
 * CS 241 - Spring 2016
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/**
 *  Given data, place it on the queue.  Can be called by multiple threads.
 */
void queue_push(queue_t *queue, void *data) { /* Your code here */ 

pthread_mutex_lock(&queue->m);

while (queue->size == queue->maxSize)
{
	pthread_cond_wait(&queue->cv, &queue->m);
}

queue->size++;

queue->tail->next = malloc(sizeof(queue_node_t));
queue->tail->next->data = data;
queue->tail = queue->tail->next;

pthread_cond_broadcast(&queue->cv);
pthread_mutex_unlock(&queue->m);
return;

}

/**
 *  Retrieve the data from the front of the queue.  Can be called by multiple
 * threads.
 *  Blocks the queue is empty
 */
void *queue_pull(queue_t *queue) {
  /* Your code here */
pthread_mutex_lock(&queue->m);

while (queue->size < 1)
{
	pthread_cond_wait(&queue->cv, &queue->m);
}

queue->size--;

void * mydata = queue->head->data;
queue_node_t *temp = queue->head;
queue->head=queue->head->next;
free(temp);

pthread_cond_broadcast(&queue->cv);
pthread_mutex_unlock(&queue->m);
return (void *)(mydata);
  	
}

/**
 *  Initializes the queue
 */
void queue_init(queue_t *queue, int maxSize) { /* Your code here */ 

queue->size=0;
queue->maxSize = maxSize;
pthread_mutex_init(&queue->m, NULL);
pthread_cond_init(&queue->cv, NULL);
queue->head = NULL;
queue->tail = NULL;

}

/**
 *  Destroys the queue, freeing any remaining nodes in it.
 */
void queue_destroy(queue_t *queue) { 

while(queue->head!=NULL)
{
	queue_node_t *temp = queue->head;
	queue->head = queue->head->next;
	free(temp);
}
pthread_mutex_destroy(&queue->m);
pthread_cond_destroy(&queue->cv);
queue->size=0;

}