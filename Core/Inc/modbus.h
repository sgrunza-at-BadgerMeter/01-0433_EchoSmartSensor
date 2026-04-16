/**
 **********************************************************************
 * @file modbus.h
 *
 * @brief This file contains Modbus related constants
 *
 * @author grunzas
 *
 * @date Oct 7, 2025
 *
 **********************************************************************
 */
#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_


/**************** Constants ********************/

/* ModBus Function Codes. Note: SSP Protocol Uses Function Codes 65-72, 100-119 */
#define MODBUS_READ_COIL		0x01	/* Function Code 1 */
#define MODBUS_READ_HOLDING_REG		0x03	/* Function Code 3 */
#define MODBUS_READ_INPUT_REG		0x04	/* Function Code 4 */
#define MODBUS_WRITE_COIL		0x05	/* Function Code 5 */
#define MODBUS_WRITE_SINGLE_REG		0x06	/* Function Code 6 */
#define MODBUS_WRITE_MULT_REG		0x10	/* Function Code 16 */
#define MODBUS_DIAG			0x08	/* Function Code 8 */
#define MODBUS_SLAVE_ID			0x11	/* Function CDode 17 */

/* Defined ModBus Registers */
#define SENSOR_NAME_MB_REG_START	100
#define SENSOR_NAME_MB_REG_STOP		111
#define SENSOR_SN_MB_REG_START		112
#define SENSOR_SN_MB_REG_STOP		117
#define USER_DATA1_MB_REG_START		118
#define USER_DATA1_MB_REG_STOP		125
#define USER_DATA2_MB_REG_START		126
#define USER_DATA2_MB_REG_STOP		134
#define USER_DATA3_MB_REG_START		800
#define USER_DATA3_MB_REG_STOP		847
#define USER_DATA4_MB_REG_START		848
#define USER_DATA4_MB_REG_STOP		863

typedef enum MBUS_RESPONSE_E
{

  MBUS_RESPONSE_OK = 0x00,
  MBUS_RESPONSE_NONE = 0xFF,
  /* MBUS_RESPONSE_ILLEGAL_FUNCTION
  The function code received in the query is not an allowable action for
  the server.  This may be because the function code is only applicable
  to newer devices, and was not implemented in the unit selected. It
  could also indicate that the server is in the wrong state to process
  a request of this type, for example because it is unconfigured and is
  being asked to return register values.
  */
  MBUS_RESPONSE_ILLEGAL_FUNCTION = 0x01,

  /* MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS
  The data address received in the query is not an allowable address
  for the server.  More specifically, the combination of reference number
  and transfer length is invalid.  For a controller with 100 registers,
  the PDU addresses the first register as 0, and the last one as 99.  If
  a request is submitted with a starting register address of 96 and a
  quantity of registers of 4, then this request should successfully
  operate (address-wise  at  least) on registers 96, 97, 98, 99.  If a
  request is submitted with a starting register address of 96 and a
  quantity of registers of 5, then this request will fail with Exception
  Code 0x02 "Illegal  Data Address" since it attempts to operate on
  registers 96, 97, 98, 99 and 100; there is no register with address 100.
  */
  MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS = 0x02,

  /* A value contained in the query data field is not an allowable value
  for server.  This indicates a fault in the structure of the remainder of
  a complex request, such as that the implied length is incorrect.  It
  specifically does NOT mean that a data item submitted for storage in a
  register has a value outside the expectation of the application program,
  since the MODBUS protocol is unaware of the significance of any
  particular value of any particular register.
  */
  MBUS_RESPONSE_ILLEGAL_DATA_VALUE = 0x03,
  /*
  An unrecoverable error occurred while the server was attempting to
  perform the requested action.
  */
  MBUS_RESPONSE_SERVICE_DEVICE_FAILURE = 0x04,
} Modbus_ResponseType;

typedef struct MODBUS_ADU_T
{
   uint16_t	length;		// number of bytes in the ADU
   uint8_t	address;	// address -- first byte of the message
   uint8_t	fc;		// function code
   uint8_t	payload;	// variable length data plus error check
} MODBUS_ADU_T;		// MODBUS Application Data Unit


/**** Function Prototypes ****/

void MB_ReadCoil(void);
void MB_WriteCoil(void);
void MB_ReadHoldingReg(void);
void MB_ReadInputReg(void);
void MB_WriteSingleReg(void);
void MB_WriteMultReg(void);
void MB_Diag(void);
void MB_SlaveID(void);

void MB_Reg40001(void);
void MB_Reg40002(void);
void MB_Reg40003(void);
void MB_Reg40004(void);
void MB_Reg40005(void);
void MB_Reg40006(void);
void MB_Reg40007(void);
void MB_Reg40008(void);
void MB_Reg40009(void);
void MB_Reg40010(void);
void MB_Reg40011(void);
void MB_Reg40012(void);
void MB_Reg40013(void);
void MB_Reg40014(void);
void MB_Reg40015(void);
void MB_Reg40016(void);
void MB_Reg40017(void);
void MB_Reg40018(void);
void MB_Reg40019(void);
void MB_Reg40020(void);
void MB_Reg40021(void);
void MB_Reg40022(void);
void MB_Reg40023(void);
void MB_Reg40024(void);
void MB_Reg40025(void);
void MB_Reg40026(void);
void MB_Reg40027(void);
void MB_Reg40028(void);
void MB_Reg40029(void);
void MB_Reg40030(void);
void MB_Reg40031(void);
void MB_Reg40032(void);
void MB_Reg40033(void);
void MB_Reg40034(void);
void MB_Reg40035(void);
void MB_Reg40036(void);
void MB_Reg40037(void);
void MB_Reg40038(void);
void MB_Reg40039(void);
void MB_Reg40040(void);
void MB_Reg40041(void);
void MB_Reg40042(void);
void MB_Reg40043(void);
void MB_Reg40044(void);
void MB_Reg40045(void);

void MB_Reg40101(uint8_t bReg);		// Sensor Name
void MB_Reg40113(uint8_t bReg);		// Sensor Serial Number
void MB_Reg40118(uint8_t bReg);		// User Specific Data - Field 1.	(1x8 byte array)
void MB_Reg40128(uint8_t bReg);		// User Specific Data - Field 2.	 1x9 byte array)
void MB_Reg40201(uint16_t bReg);		// Waveform Data
void MB_Reg40801(uint8_t bReg);		// User Specific Data - Field 3. (3x32 byte array)
void MB_Reg40848(uint8_t bReg);		// User Specific Data - Field 4. (8x2 byte array)

void MB_Reg50000( void );		// Sensor to Controller command to set 4-20mA loop Level
void MB_Reg50001( void );		// Sensor to Controller command to set 4-20mA loop Aux

void MB_Coil001(void);
void MB_Coil002(void);
void MB_Coil003(void);
void MB_Coil004(void);
void MB_Coil005(void);
void MB_Coil006(void);
void MB_Coil007(void);
void MB_Coil008(void);
void MB_Coil009(void);
void MB_Coil010(void);
void MB_Coil011(void);
void MB_Coil012(void);
void MB_Coil013(void);
void MB_Coil014(void);
void MB_Coil015(void);
void MB_Coil016(void);
void MB_Coil017(void);
void MB_Coil018(void);
void MB_Coil019(void);
void MB_Coil020(void);
void MB_Coil021(void);
void MB_Coil022(void);
void MB_Coil023(void);
void MB_Coil024(void);
void MB_Coil025(void);
void MB_Coil026(void);
void MB_Coil027(void);
void MB_Coil028(void);
void MB_Coil029(void);
void MB_Coil030(void);
void MB_Coil031(void);
void MB_Coil032(void);
void MB_Coil033(void);
void MB_Coil034(void);
void MB_Coil035(void);
void MB_Coil036(void);
void MB_Coil037(void);
void MB_Coil038(void);
void MB_Coil039(void);
void MB_Coil040(void);

#define NBR_MB_REGS		44	/* Number of Modbus Registers in SSP */
#define MAX_MB_REG_COMM		15	/* Maximum number of consecutive registers IO can support */
#define MAX_MB_REGS		15	/* Min of NBR_MB_REG and MAX_MB_REG_COMM */

#define NBR_MB_COILS		40	/* Number of Modbus Coils in SSP */
#define MAX_MB_COILS_COMM	80	/* Maximum number of consecutive coils IO can support */

/*** Defined Memory Locations ***/


/***** Variables visable to all ****/

#ifdef _MODBUS_C_

const void (*MB_REG_MAP[])(void) = {
	(void *)MB_Reg40001,
	(void *)MB_Reg40002,
	(void *)MB_Reg40003,
	(void *)MB_Reg40004,
	(void *)MB_Reg40005,
	(void *)MB_Reg40006,
	(void *)MB_Reg40007,
	(void *)MB_Reg40008,
	(void *)MB_Reg40009,
	(void *)MB_Reg40010,
	(void *)MB_Reg40011,
	(void *)MB_Reg40012,
	(void *)MB_Reg40013,
	(void *)MB_Reg40014,
	(void *)MB_Reg40015,
	(void *)MB_Reg40016,
	(void *)MB_Reg40017,
	(void *)MB_Reg40018,
	(void *)MB_Reg40019,
	(void *)MB_Reg40020,
	(void *)MB_Reg40021,
	(void *)MB_Reg40022,
	(void *)MB_Reg40023,
	(void *)MB_Reg40024,
	(void *)MB_Reg40025,
	(void *)MB_Reg40026,
	(void *)MB_Reg40027,
	(void *)MB_Reg40028,
	(void *)MB_Reg40029,
	(void *)MB_Reg40030,
	(void *)MB_Reg40031,
	(void *)MB_Reg40032,
	(void *)MB_Reg40033,
	(void *)MB_Reg40034,
	(void *)MB_Reg40035,
	(void *)MB_Reg40036,
	(void *)MB_Reg40037,
	(void *)MB_Reg40038,
	(void *)MB_Reg40039,
	(void *)MB_Reg40040,		/* Reserved for future use */
	(void *)MB_Reg40041,		/* Reserved for future use */
	(void *)MB_Reg40042,
	(void *)MB_Reg40043,
	(void *)MB_Reg40044,
	(void *)MB_Reg40045,
	};

const void (*MB_COIL_MAP[])(void) = {
	(void *)MB_Coil001,
	(void *)MB_Coil002,
	(void *)MB_Coil003,
	(void *)MB_Coil004,
	(void *)MB_Coil005,
	(void *)MB_Coil006,
	(void *)MB_Coil007,
	(void *)MB_Coil008,
	(void *)MB_Coil009,
	(void *)MB_Coil010,
	(void *)MB_Coil011,
	(void *)MB_Coil012,
	(void *)MB_Coil013,	/* Reserved for future use */
	(void *)MB_Coil013,	/* Reserved for future use */
	(void *)MB_Coil013,	/* Reserved for future use */
	(void *)MB_Coil013,	/* Reserved for future use */
	(void *)MB_Coil017,
	(void *)MB_Coil018,
	(void *)MB_Coil019,
	(void *)MB_Coil020,
	(void *)MB_Coil021,
	(void *)MB_Coil022,
	(void *)MB_Coil023,
	(void *)MB_Coil024,
	(void *)MB_Coil025,
	(void *)MB_Coil026,
	(void *)MB_Coil027,
	(void *)MB_Coil028,
	(void *)MB_Coil029,
	(void *)MB_Coil030,
	(void *)MB_Coil031,
	(void *)MB_Coil032,
	(void *)MB_Coil033,
	(void *)MB_Coil034,
	(void *)MB_Coil035,
	(void *)MB_Coil036,
	(void *)MB_Coil037,
	(void *)MB_Coil038,
	(void *)MB_Coil039,
	(void *)MB_Coil040,	/* Reserved for future use */
	};

/* Com Vars */
uint32_t wBusMsgCount;		/* Number of Bus Messages Received - All address */
uint32_t wCRCCount;		/* Number of CRC Errors Detected */
uint32_t wSlaveMsgCount;	/* Number of messages received for this slave */

#else

/* Com Vars */
extern uint32_t wBusMsgCount;		/* Number of Bus Messages Received - All address */
extern uint32_t wCRCCount;		/* Number of CRC Errors Detected */
extern uint32_t wSlaveMsgCount;		/* Number of messages received for this slave */

#endif // #ifdef _MODBUS_C_

#endif /* INC_MODBUS_H_ */

// end of file modbus.h
