/******************************************************************************

 @file  ModbusMaster.h

 @brief

 Target Device: cc2640r2

 Maker/Author - Markel T. Robregado
 Modification Details:
 Device Setup: CC2652RB Launchpad + DPS3005 Programmable Power Supply

 *****************************************************************************/
#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>

/*********************************************************************
 * CONSTANTS
 */


#define MB_ILLEGALFUNCTION           0x01 // Modbus protocol illegal function exception.
#define MB_ILLEGALDATADDRESS         0x02 // Modbus protocol illegal data address exception.
#define MB_ILLEGALDATAVALUE          0x03 // Modbus protocol illegal data value exception.
#define MB_SLAVEDEVICEFAILURE        0x04 // Modbus protocol slave device failure exception.

#define MB_SUCCESS                   0x00 // ModbusMaster success.
#define MB_INVALIDSLAVEID            0xE0 // ModbusMaster invalid response slave ID exception.
#define MB_INVALIDFUNCTION           0xE1 // ModbusMaster invalid response function exception.
#define MB_RESPONSETIMEOUT           0xE2 // ModbusMaster response timed out exception.
#define MB_INVALIDCRC                0xE3; ModbusMaster invalid response CRC exception.

// Modbus function codes for 16 bit access
#define MB_READHOLDINGREGISTERS      0x03 // Modbus function 0x03 Read Holding Registers DPS SUPPORTED
#define MB_WRITESINGLEREGISTER       0x06 // Modbus function 0x06 Write Single Register DPS SUPPORTED
#define MB_WRITEMULTIPLEREGISTERS    0x10 // Modbus function 0x10 Write Multiple Registers DPS SUPPORTED

/*********************************************************************
 * FUNCTIONS
 */

extern void ModbusMaster_Init(void);
extern uint8_t ModbusMaster_writeSingleRegister(uint16_t u16WriteAddress, uint16_t u16WriteValue);
extern uint8_t ModbusMaster_readHoldingRegisters(uint16_t u16ReadAddress,  uint16_t u16ReadQty);

#endif /* MODBUSMASTER_H */
