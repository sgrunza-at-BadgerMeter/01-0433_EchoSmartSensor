/**
 **********************************************************************
 * @file mcp.h
 *
 * @brief A brief description of this file's purpose
 *
 * @author grunzas
 *
 * @date Mar 12, 2026
 *
 **********************************************************************
 */
#ifndef INC_MCP_H_
#define INC_MCP_H_

#define TX_MCP_THREAD_PRIO	10
#define MCP_STACK_SZ		1024


// States the sensor can be in
typedef enum MCP_SYS_STATE_E
{
   INIT_STATE = 1,
   SETUP_STATE,
   PING_STATE,
   ANALYZE_STATE,
   GET_NEXT_WAVEFORM_STATE,
   GET_NEXT_WAVEFORM_GROUP_STATE,
   LOOP_STATE,
   LAST_STATE
} MCP_SYS_STATE_E;

#ifndef SRC_MCP_C_
extern
#endif
uint8_t		mcpStack[ MCP_STACK_SZ ];

#ifndef SRC_MCP_C_
extern
#endif
TX_THREAD		tx_mcp_thread;



void
   mcp_thread_start(
      ULONG   		ulParameters );


#endif /* INC_MCP_H_ */

// end of file mcp.h
