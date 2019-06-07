/******************************************************************************

 @file  ModbusMaster.c

 @brief This file contains code library for Modbus RTU Functionality for DPS3005

 Target Device: cc2640r2

 Maker/Author - Markel T. Robregado
 Modification Details:
 Device Setup: CC2652RB Launchpad + DPS3005 Programmable Power Supply

 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/drivers/UART.h>
/* For usleep() */
//#include <unistd.h>
#include "ModbusMaster.h"
/* Example/Board Header files */
#include "Board.h"

/* Delay */
#define DELAY_MS(i)      (Task_sleep(((i) * 1000) / Clock_tickPeriod))
#define DELAY_US(i)      (Task_sleep(((i) * 1) / Clock_tickPeriod))
#define MS_2_TICKS(ms)   (((ms) * 1000) / Clock_tickPeriod)


/*********************************************************************
 * LOCAL VARIABLES
 */
uint8_t  _u8MBSlave;                                         // Modbus slave (1..255) initialized in begin()
#define  MAXBUFFERSIZE                                  64   // size of response/transmit buffers
uint16_t _u16ReadAddress;                                    // slave register from which to read
uint16_t _u16ReadQty;                                        // quantity of words to read
uint16_t _u16ResponseBuffer[MAXBUFFERSIZE];                  // buffer to store Modbus slave response; read via GetResponseBuffer()
uint16_t _u16WriteAddress;                                   // slave register to which to write
uint16_t _u16WriteQty;                                       // quantity of words to write
uint16_t _u16TransmitBuffer[MAXBUFFERSIZE];                  // buffer containing data to transmit to Modbus slave; set via SetTransmitBuffer()

UART_Handle huart;
UART_Params uartParams;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint16_t ModbusMaster_crc16update(uint16_t crc, uint8_t a);
static uint8_t ModbusMaster_ModbusMasterTransaction(uint8_t u8MBFunction);


/*********************************************************************
 * @fn      ModbusMaster_Init
 *
 * @brief   ModbusMaster Initialization
 *
 * @param   none
 *
 * @return  none
 */
void ModbusMaster_Init(void)
{
    _u8MBSlave = 1;

    /* Call driver init functions */
    UART_init();

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    //uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 9600;

    huart = UART_open(Board_UART0, &uartParams);

    if (huart == NULL) {
        /* UART_open() failed */
        while (1);
    }
}


/*********************************************************************
 * @fn      ModbusMaster_getResponseBuffer
 *
 * @brief   Retrieve data from response buffer.
 *
 * @param   u8Index
 *
 * @return  value in position u8Index of response buffer
 */
uint16_t ModbusMaster_getResponseBuffer(uint8_t u8Index)
{
    if (u8Index < MAXBUFFERSIZE)
    {
        return _u16ResponseBuffer[u8Index];
    }
    else
    {
        return 0xFFFF;
    }
}


/*********************************************************************
 * @fn      ModbusMaster_clearResponseBuffer
 *
 * @brief   Clear response buffer.
 *
 * @param
 *
 * @return
 */
void ModbusMaster_clearResponseBuffer(void)
{
    uint8_t i;

    for (i = 0; i < MAXBUFFERSIZE; i++)
    {
        _u16ResponseBuffer[i] = 0;
    }
}


/*********************************************************************
 * @fn      ModbusMaster_clearTransmitBuffer
 *
 * @brief   Clear transmit buffer.
 *
 * @param
 *
 * @return
 */
void ModbusMaster_clearTransmitBuffer(void)
{
    uint8_t i;

    for (i = 0; i < MAXBUFFERSIZE; i++)
    {
        _u16TransmitBuffer[i] = 0;
    }
}

/*********************************************************************
 * @fn      ModbusMaster_crc16update
 *
 * @brief   .
 *
 * @param
 *
 * @return crc
 */
static uint16_t ModbusMaster_crc16update(uint16_t crc, uint8_t a)
{
    int i;

    crc ^= a;

    for (i = 0; i < 8; ++i)
    {
        if (crc & 1)
        {
            crc = (crc >> 1) ^ 0xA001;
        }
        else
        {
            crc = (crc >> 1);
        }
    }

    return crc;
}


/*********************************************************************
 * @fn      ModbusMaster_readHoldingRegisters
 *
 * @brief   read the contents of a contiguous block of
 *          holding registers in a remote device
 *
 * @param   u16ReadAddress, u16ReadQty
 *
 * @return
 */
uint8_t ModbusMaster_readHoldingRegisters(uint16_t u16ReadAddress,  uint16_t u16ReadQty)
{
    _u16ReadAddress = u16ReadAddress;
    _u16ReadQty = u16ReadQty;

    return ModbusMaster_ModbusMasterTransaction(MB_READHOLDINGREGISTERS);
}


/*********************************************************************
 * @fn      ModbusMaster_writeSingleRegister
 *
 * @brief   write a single holding register in a
 *          remote device.
 *
 * @param   u16ReadAddress, u16WriteValue
 *
 * @return
 */
uint8_t ModbusMaster_writeSingleRegister(uint16_t u16WriteAddress, uint16_t u16WriteValue)
{
    _u16WriteAddress = u16WriteAddress;
    _u16TransmitBuffer[0] = u16WriteValue;

    return ModbusMaster_ModbusMasterTransaction(MB_WRITESINGLEREGISTER);
}


/* _____PRIVATE FUNCTIONS____________________________________________________ */
/**
Modbus transaction engine.
Sequence:
  - assemble Modbus Request Application Data Unit (ADU),
    based on particular function called
  - transmit request over selected serial port
  - wait for/retrieve response
  - evaluate/disassemble response
  - return status (success/exception)

@param u8MBFunction Modbus function (0x01..0xFF)
@return 0 on success; exception number on failure
*/
/*********************************************************************
 * @fn      ModbusMaster_ModbusMasterTransaction
 *
 * @brief   Assemble Modbus Request Application Data Unit(ADU). Transmit
 *          ADU using UART.
 *
 * @param   u8MBFunction
 *
 * @return
 */
static uint8_t ModbusMaster_ModbusMasterTransaction(uint8_t u8MBFunction)
{
    uint8_t u8ModbusADU[32] = {0,};
    uint8_t u8ModbusADUSize = 0;
    uint8_t i;
    uint16_t u16CRC;
    uint8_t u8MBStatus = MB_SUCCESS;


    // assemble Modbus Request Application Data Unit
    u8ModbusADU[u8ModbusADUSize++] = _u8MBSlave; // 0x01
    u8ModbusADU[u8ModbusADUSize++] = u8MBFunction; //0x03 //0x06

    switch(u8MBFunction)
    {

        case MB_READHOLDINGREGISTERS:
            u8ModbusADU[u8ModbusADUSize++] = _u16ReadAddress >> 8; // high byte
            u8ModbusADU[u8ModbusADUSize++] = _u16ReadAddress & 0x00FF; // low byte;
            u8ModbusADU[u8ModbusADUSize++] = _u16ReadQty >> 8; // high byte;
            u8ModbusADU[u8ModbusADUSize++] = _u16ReadQty & 0x00FF; // low byte;
        break;

        case MB_WRITESINGLEREGISTER:
            u8ModbusADU[u8ModbusADUSize++] = _u16WriteAddress >> 8; // high byte
            u8ModbusADU[u8ModbusADUSize++] = _u16WriteAddress & 0x00FF; // low byte
            u8ModbusADU[u8ModbusADUSize++] = _u16TransmitBuffer[0] >> 8; // high byte
            u8ModbusADU[u8ModbusADUSize++] = _u16TransmitBuffer[0] & 0x00FF; // low byte
        break;

        default:
        break;
    }

    // append CRC
    u16CRC = 0xFFFF;

    for (i = 0; i < u8ModbusADUSize; i++)
    {
        u16CRC = ModbusMaster_crc16update(u16CRC, u8ModbusADU[i]);
    }

    u8ModbusADU[u8ModbusADUSize++] = u16CRC & 0x00FF; // low byte crc
    u8ModbusADU[u8ModbusADUSize++] = u16CRC >> 8; // high byte crc
    u8ModbusADU[u8ModbusADUSize] = 0;

    DELAY_MS(300);

    UART_write(huart, &u8ModbusADU[0], u8ModbusADUSize);

    DELAY_MS(300);

    u8ModbusADUSize = 0;

    return u8MBStatus;
}
