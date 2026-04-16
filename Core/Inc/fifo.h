/**
 **********************************************************************
 * @file fifo.h
 *
 * @brief This file holds the definitions and structures for a generic
 * 	queue that supports FIFO functions
 *
 * @author grunzas
 *
 * @date Sep 15, 2025
 *
 **********************************************************************
 */

#ifndef INC_FIFO_H_
#define INC_FIFO_H_


// Define a FIFO that can hold 10 entries of 80 characters each
#define MAX_FIFO_ENTRY_LEN		80
#define MAX_FIFO_ITEMS			10

// Define result codes for the fifo_ functions
#define FIFO_FAILURE		-1
#define FIFO_SUCCESS		0

typedef char	FIFO_ENTRY_T[ MAX_FIFO_ENTRY_LEN ];

typedef struct FIFO_T
{
   uint16_t		head;				///< location to put next msg
   uint16_t		tail;				///< location of oldest msg
   uint16_t		entries;			///< number of valid entries
   FIFO_ENTRY_T		item[ MAX_FIFO_ITEMS ];		///< actual Modbus msg
   uint16_t		itemLen[ MAX_FIFO_ITEMS ];	///< number of bytes in msg
} FIFO_T;


int8_t	fifo_push( FIFO_T *queue, uint8_t *msg, uint16_t len );
int8_t	fifo_pop( FIFO_T *queue, char *msg, uint16_t *len );
bool	fifo_spaceLeft( FIFO_T *q );
void	fifo_init( FIFO_T *queue );


#endif /* INC_FIFO_H_ */

// end of file fifo.h
