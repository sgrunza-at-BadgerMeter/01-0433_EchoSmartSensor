/*
 * queue.c
 *
 *  Created on: Sep 7, 2022
 *      Author: SGrunza
 */

#include "main.h"
#include "fifo.h"
#include "usart.h"
#include <string.h>


/*
 **********************************************************************
 * @brief Note that it is an undetectable error to not allocate enough
 * memory to this function.  bq_init() configures the byte queue memory
 * pointed to by byteQueue.
 *
 * @param fifo - ptr to FIFO structure.  Must be large enough
 *
 * @return none
 **********************************************************************
 */
void
   fifo_init(
      FIFO_T 		*fifo )
{

   if( fifo != NULL )
   {
      memset( fifo, 0, sizeof(FIFO_T) );
   }

   return;
} // end of fifo_init()

/*
 **********************************************************************
 * @brief Determine if there is any space left in the given queue
 *
 * @param byteQueue - ptr to byte queue structure.
 *
 * @return 0 (FALSE) if no space left, else return 1 (TRUE)
 **********************************************************************
 */
bool
   fifo_spaceLeft(
      FIFO_T 		*fifo )
{
   uint8_t	spaceLeft;


   if( fifo->entries < MAX_FIFO_ITEMS )
   {
      spaceLeft = true;
   }
   else
   {
      spaceLeft = false;
   }


   return( spaceLeft );
} // end of fifo_spaceLeft()

/*
 **********************************************************************
 * @brief Push another message onto the queue at the current item
 *
 * @param fifo - ptr to FIFO structure.
 *
 * @param msg - message to be placed on the FIFO
 *
 * @param len - number of bytes in message
 *
 * @return FIFO_FAILURE if no space left, else return FIFO_SUCCESS
 **********************************************************************
 */
int8_t
   fifo_push(
      FIFO_T 		*fifo,
      uint8_t 		*msg,
      uint16_t		len )
{

   int8_t	result;
   uint16_t	nextIndex;

   // It works until it doesn't
   result = FIFO_SUCCESS;

   if( fifo == NULL )
   {
      result = FIFO_FAILURE;
   }
   else
   {
      if( len > MAX_FIFO_ENTRY_LEN )
      {
	 // This message is too big for the FIFO entries configured
	 result = FIFO_FAILURE;
      }
      else
      {
	 if( !fifo_spaceLeft( fifo ) )
	 {
	    // No empty slots in the FIFO
	    result = FIFO_FAILURE;
	 }
      }
   }

   if( result == FIFO_SUCCESS )
   {
      // If there is space left then the head ptr is ok for writing

      // Clear the entry
      memset( fifo->item[ fifo->head ], 0, MAX_FIFO_ENTRY_LEN );

      // Put data into entry
      memcpy( fifo->item[ fifo->head ], msg, len );
      fifo->itemLen[ fifo->head ] = len;
      fifo->entries++;

      nextIndex = (fifo->head + 1) % MAX_FIFO_ITEMS;

      fifo->head = nextIndex;
   }

   return( result );
} // end of fifo_push()

/*
 **********************************************************************
 * @brief Pop a message off the queue
 *
 * @param fifo - ptr to FIFO structure.
 *
 * @param dst - pointer to where to place the message from the FIFO
 *
 * @return FIFO_FAILURE if something went wrong, else return FIFO_SUCCESS
 **********************************************************************
 */
int8_t
   fifo_pop(
      FIFO_T 		*fifo,
      char 		*dst,
      uint16_t		*len )
{
   int8_t	result;
   uint16_t	dataLen;
   char		*src;

   // It works until it doesn't
   result = FIFO_SUCCESS;

   if( fifo == NULL )
   {
      // There is no FIFO from which to pop an item
      result = FIFO_FAILURE;
   }
   else if( len == NULL )
   {
      // Need someplace to return the length of the item
      result = FIFO_FAILURE;
   }
   else
   {
      if( dst == NULL )
      {
	 // There is no place to put the entry
	 result = FIFO_FAILURE;
      }
      if( fifo->entries == 0 )
      {
	 // Nothing to pop
	 result = FIFO_FAILURE;
      }
   }

   if( result == FIFO_SUCCESS )
   {
      src = fifo->item[ fifo->tail ];
      dataLen = fifo->itemLen[ fifo->tail ];
      memcpy( dst, src, dataLen );
      fifo->entries--;
      fifo->tail = (fifo->tail + 1) % MAX_FIFO_ITEMS;
   }

   *len = dataLen;

   return( result );
} // end of fifo_pop()



// end of file fifo.c
