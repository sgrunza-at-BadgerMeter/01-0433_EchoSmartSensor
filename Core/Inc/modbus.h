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
#define	SENSOR_NAME_MB_REG_START	40101
#define SENSOR_NAME_MB_REG_STOP		40112

#define SENSOR_SN_MB_REG_START		40113
#define SENSOR_SN_MB_REG_STOP		40118


#define WAVEFORM_MB_REG_START		40201
#define WAVEFORM_MB_REG_STOP		40712

// NOTE: USERDATA1 through USERDATA4 were for an OEM customer
// with whom Badger Meter is not partnered at this time.  This
// is legacy code from ATI / Entec days.  Steven Grunza - 04/23/2026

#define USERDATA1_MB_REG_START		40118
#define USERDATA1_MB_REG_STOP		40125

#define USERDATA2_MB_REG_START		40126
#define USERDATA2_MB_REG_STOP		40134

#define USERDATA3_MB_REG_START		40800
#define USERDATA3_MB_REG_STOP		40847

#define USERDATA4_MB_REG_START		40848
#define USERDATA4_MB_REG_STOP		40863


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
   uint8_t	payload[2];	// variable length data plus error check
} MODBUS_ADU_T;		// MODBUS Application Data Unit

typedef struct MODBUS_ADU_REG_WRITE_T
{
   uint16_t	length;		// number of byte in the message, not part
				// of the on-the-wire data
   uint8_t	address;	// slave address -- first byte of message
   uint8_t	fc;		// function code -- should be 0x06
   uint16_t	regAddr;	// address of register
   uint16_t	regData;	// data to write
   uint16_t	crc;
} MODBUS_ADU_REG_WRITE_T;

typedef struct MODBUS_ADU_MULTIREG_WRITE_T
{
   uint16_t	length;		// number of byte in the message, not part
				// of the on-the-wire data
   uint8_t	address;	// slave bus address -- first byte of message
   uint8_t	fc;		// function code -- should be 0x10
   uint16_t	regAddr;	// address of first register
   uint16_t	regNum;		// number of registers to write
   uint8_t	byteCount;	// byte count -- should be regNum * 2
   uint16_t	regData[2];	// start of data to write
} MODBUS_ADU_MULTIREG_WRITE_T;

typedef bool (*COIL_CMD_T)(bool isWrite, bool val );

typedef struct MB_COIL_CMD_T
{
   uint16_t	coilNumber;
   COIL_CMD_T	function;
} MB_COIL_CMD_T;

typedef int16_t (*REG_CMD_T)(uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

typedef struct MB_REG_CMD_T
{
   uint16_t	regNumber;
   REG_CMD_T	function;
} MB_REG_CMD_T;

/**** Function Prototypes ****/

uint16_t modbus_reg_first_reg( MODBUS_ADU_T *msg );
uint16_t modbus_number_of_regs( MODBUS_ADU_T *msg );

bool MB_Coil001( bool isWrite, bool val );
bool MB_Coil002( bool isWrite, bool val );
bool MB_Coil003( bool isWrite, bool val );
bool MB_Coil004( bool isWrite, bool val );
bool MB_Coil005( bool isWrite, bool val );
bool MB_Coil006( bool isWrite, bool val );
bool MB_Coil007( bool isWrite, bool val );
bool MB_Coil008( bool isWrite, bool val );
bool MB_Coil009( bool isWrite, bool val );
bool MB_Coil010( bool isWrite, bool val );

bool MB_Coil011( bool isWrite, bool val );
bool MB_Coil012( bool isWrite, bool val );
//bool MB_Coil013( bool isWrite, bool val );
//bool MB_Coil014( bool isWrite, bool val );
//bool MB_Coil015( bool isWrite, bool val );
//bool MB_Coil016( bool isWrite, bool val );
bool MB_Coil017( bool isWrite, bool val );
bool MB_Coil018( bool isWrite, bool val );
bool MB_Coil019( bool isWrite, bool val );
bool MB_Coil020( bool isWrite, bool val );

bool MB_Coil021( bool isWrite, bool val );
bool MB_Coil022( bool isWrite, bool val );
bool MB_Coil023( bool isWrite, bool val );
bool MB_Coil024( bool isWrite, bool val );
bool MB_Coil025( bool isWrite, bool val );
bool MB_Coil026( bool isWrite, bool val );
bool MB_Coil027( bool isWrite, bool val );
bool MB_Coil028( bool isWrite, bool val );
bool MB_Coil029( bool isWrite, bool val );
bool MB_Coil030( bool isWrite, bool val );

bool MB_Coil031( bool isWrite, bool val );
bool MB_Coil032( bool isWrite, bool val );
bool MB_Coil033( bool isWrite, bool val );
bool MB_Coil034( bool isWrite, bool val );
bool MB_Coil035( bool isWrite, bool val );
bool MB_Coil036( bool isWrite, bool val );
bool MB_Coil037( bool isWrite, bool val );
bool MB_Coil038( bool isWrite, bool val );


#define MB_INVALID_COIL		0xFFFF
#define MB_INVALID_REG		0xFFFF

#ifndef _MODBUS_C_
extern
#endif
MB_COIL_CMD_T	coil_commands[]
#ifdef _MODBUS_C_
= {
	 { 1, MB_Coil001 },
	 { 2, MB_Coil002 },
	 { 3, MB_Coil003 },
	 { 4, MB_Coil004 },
	 { 5, MB_Coil005 },
	 { 6, MB_Coil006 },
	 { 7, MB_Coil007 },
	 { 8, MB_Coil008 },
	 { 9, MB_Coil009 },
	 { 10, MB_Coil010 },
	 { 11, MB_Coil011 },
	 { 12, MB_Coil012 },
	 { 17, MB_Coil017 },
	 { 18, MB_Coil018 },
	 { 19, MB_Coil019 },
	 { 20, MB_Coil020 },
	 { 21, MB_Coil021 },
	 { 22, MB_Coil022 },
	 { 23, MB_Coil023 },
	 { 24, MB_Coil024 },
	 { 25, MB_Coil025 },
	 { 26, MB_Coil026 },
	 { 27, MB_Coil027 },
	 { 28, MB_Coil028 },
	 { 29, MB_Coil029 },
	 { 30, MB_Coil030 },
	 { 31, MB_Coil031 },
	 { 32, MB_Coil032 },
	 { 33, MB_Coil033 },
	 { 34, MB_Coil034 },
	 { 35, MB_Coil035 },
	 { 36, MB_Coil036 },
	 { 37, MB_Coil037 },
	 { 38, MB_Coil038 },
	 { MB_INVALID_COIL, NULL }
}
#endif
;

void MB_ReadCoil(void);
void MB_WriteCoil(void);
void MB_ReadHoldingReg(void);
void MB_ReadInputReg(void);
void MB_WriteSingleReg(void);
void MB_WriteMultReg(void);
void MB_Diag(void);
void MB_SlaveID(void);

int16_t MB_Reg40001( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40002( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40003( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40004( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40005( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40006( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40007( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40008( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40009( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40010( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

int16_t MB_Reg40011( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40012( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40013( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40014( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40015( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40016( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40017( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40018( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40019( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40020( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

int16_t MB_Reg40021( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40022( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40023( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40024( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40025( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40026( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40027( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40028( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40029( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40030( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

int16_t MB_Reg40031( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40032( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40033( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40034( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40035( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40036( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40037( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40038( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40039( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );


int16_t MB_Reg40042( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40043( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40044( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg40045( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

int16_t MB_SensorName( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

int16_t MB_SerialNum( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

int16_t MB_Waveform( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

int16_t MB_Reg50000( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );
int16_t MB_Reg50001( uint16_t reg, bool isWrite, int16_t val, int16_t *errCode );

#ifndef _MODBUS_C_
extern
#endif
MB_REG_CMD_T	reg_commands[]
#ifdef _MODBUS_C_
= {
	 { 40001, MB_Reg40001 },
	 { 40002, MB_Reg40002 },
	 { 40003, MB_Reg40003 },
	 { 40004, MB_Reg40004 },
	 { 40005, MB_Reg40005 },
	 { 40006, MB_Reg40006 },
	 { 40007, MB_Reg40007 },
	 { 40008, MB_Reg40008 },
	 { 40009, MB_Reg40009 },
	 { 40010, MB_Reg40010 },

	 { 40011, MB_Reg40011 },
	 { 40012, MB_Reg40012 },
	 { 40013, MB_Reg40013 },
	 { 40014, MB_Reg40014 },
	 { 40015, MB_Reg40015 },
	 { 40016, MB_Reg40016 },
	 { 40017, MB_Reg40017 },
	 { 40018, MB_Reg40018 },
	 { 40019, MB_Reg40019 },
	 { 40020, MB_Reg40020 },

	 { 40021, MB_Reg40021 },
	 { 40022, MB_Reg40022 },
	 { 40023, MB_Reg40023 },
	 { 40024, MB_Reg40024 },
	 { 40025, MB_Reg40025 },
	 { 40026, MB_Reg40026 },
	 { 40027, MB_Reg40027 },
	 { 40028, MB_Reg40028 },
	 { 40029, MB_Reg40029 },
	 { 40030, MB_Reg40030 },

	 { 40031, MB_Reg40031 },
	 { 40032, MB_Reg40032 },
	 { 40033, MB_Reg40033 },
	 { 40034, MB_Reg40034 },
	 { 40035, MB_Reg40035 },
	 { 40036, MB_Reg40036 },
	 { 40037, MB_Reg40037 },
	 { 40038, MB_Reg40038 },
	 { 40039, MB_Reg40039 },

	 { 40042, MB_Reg40042 },
	 { 40043, MB_Reg40043 },
	 { 40044, MB_Reg40044 },
	 { 40045, MB_Reg40045 },

	 { SENSOR_NAME_MB_REG_START, MB_SensorName },	// Sensor Name 1 of 12

	 { SENSOR_SN_MB_REG_START, MB_SerialNum },	// Serial number 1 of 6

	 //{ USERDATA1_MB_REG_START, MB_Reg40118 },	// User Spific data 1

	 //{ USERDATA2_MB_REG_START, MB_Reg40126 },	// User Spific data 2

	 { WAVEFORM_MB_REG_START, MB_Waveform },	// Waveform data[0, 511]

	 //{ USERDATA3_MB_REG_START, MB_Reg40801 },	// User Specific Data 3 x 32-bit array

	 //{ USERDATA4_MB_REG_START, MB_Reg40848 },	// User Specific Data 8 x 2 array
	 { MB_INVALID_REG, NULL }
}
#endif
;


#define NBR_MB_REGS		44	/* Number of Modbus Registers in SSP */
#define MAX_MB_REG_COMM		15	/* Maximum number of consecutive registers IO can support */
#define MAX_MB_REGS		15	/* Min of NBR_MB_REG and MAX_MB_REG_COMM */

#define NBR_MB_COILS		40	/* Number of Modbus Coils in SSP */
#define MAX_MB_COILS_COMM	80	/* Maximum number of consecutive coils IO can support */




#endif /* INC_MODBUS_H_ */

// end of file modbus.h
