/**********************************************************************************************
 * Filename:       DPSModbusService.c
 *
 * Description:    This file contains the implementation of the service.
 *
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <icall.h>

/* This Header file contains all BLE API and icall structure definition */
#include "icall_ble_api.h"

#include "DPSModbusService.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
* GLOBAL VARIABLES
*/

// DPSModbusService Service UUID
CONST uint8_t DPSModbusServiceUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DPSMODBUSSERVICE_SERV_UUID)
};

// VoltageChar UUID
CONST uint8_t DPSModbusService_VoltageCharUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DPSMODBUSSERVICE_VOLTAGECHAR_UUID)
};
// CurrentChar UUID
CONST uint8_t DPSModbusService_CurrentCharUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DPSMODBUSSERVICE_CURRENTCHAR_UUID)
};
// BrightnessChar UUID
CONST uint8_t DPSModbusService_BrightnessCharUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DPSMODBUSSERVICE_BRIGHTNESSCHAR_UUID)
};
// SwitchChar UUID
CONST uint8_t DPSModbusService_SwitchCharUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DPSMODBUSSERVICE_SWITCHCHAR_UUID)
};


// Characteristic "WeighScaleValue" User Description
static uint8 VoltageDesc[14] = "Voltage";
// Characteristic "WeighScaleValue" User Description
static uint8 CurrentDesc[14] = "Current";
// Characteristic "WeighScaleValue" User Description
static uint8 BrightnessDesc[14] = "Brightness";
// Characteristic "WeighScaleValue" User Description
static uint8 SwitchDesc[14] = "Switch";
/*********************************************************************
 * LOCAL VARIABLES
 */

static DPSModbusServiceCBs_t *pAppCBs = NULL;

/*********************************************************************
* Profile Attributes - variables
*/

// Service declaration
static CONST gattAttrType_t DPSModbusServiceDecl = { ATT_UUID_SIZE, DPSModbusServiceUUID };

// Characteristic "VoltageChar" Properties (for declaration)
static uint8_t DPSModbusService_VoltageCharProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic "VoltageChar" Value variable
static uint8_t DPSModbusService_VoltageCharVal[DPSMODBUSSERVICE_VOLTAGECHAR_LEN] = {0};
// Characteristic "CurrentChar" Properties (for declaration)
static uint8_t DPSModbusService_CurrentCharProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic "CurrentChar" Value variable
static uint8_t DPSModbusService_CurrentCharVal[DPSMODBUSSERVICE_CURRENTCHAR_LEN] = {0};
// Characteristic "BrightnessChar" Properties (for declaration)
static uint8_t DPSModbusService_BrightnessCharProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic "BrightnessChar" Value variable
static uint8_t DPSModbusService_BrightnessCharVal[DPSMODBUSSERVICE_BRIGHTNESSCHAR_LEN] = {0};
// Characteristic "SwitchChar" Properties (for declaration)
static uint8_t DPSModbusService_SwitchCharProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic "SwitchChar" Value variable
static uint8_t DPSModbusService_SwitchCharVal[DPSMODBUSSERVICE_SWITCHCHAR_LEN] = {0};

/*********************************************************************
* Profile Attributes - Table
*/

static gattAttribute_t DPSModbusServiceAttrTbl[] =
{
  // DPSModbusService Service Declaration
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID },
    GATT_PERMIT_READ,
    0,
    (uint8_t *)&DPSModbusServiceDecl
  },

    // VoltageChar Characteristic Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &DPSModbusService_VoltageCharProps
    },
    // VoltageChar Characteristic Value
    {
        { ATT_UUID_SIZE, DPSModbusService_VoltageCharUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        DPSModbusService_VoltageCharVal
    },
    // Characteristic User Description
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        VoltageDesc
    },

    // CurrentChar Characteristic Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &DPSModbusService_CurrentCharProps
    },
    // CurrentChar Characteristic Value
    {
        { ATT_UUID_SIZE, DPSModbusService_CurrentCharUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        DPSModbusService_CurrentCharVal
    },
    // Characteristic User Description
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        CurrentDesc
    },

    // BrightnessChar Characteristic Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &DPSModbusService_BrightnessCharProps
    },
    // BrightnessChar Characteristic Value
    {
        { ATT_UUID_SIZE, DPSModbusService_BrightnessCharUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        DPSModbusService_BrightnessCharVal
    },
    // Characteristic User Description
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        BrightnessDesc
    },

    // SwitchChar Characteristic Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &DPSModbusService_SwitchCharProps
    },
    // SwitchChar Characteristic Value
    {
        { ATT_UUID_SIZE, DPSModbusService_SwitchCharUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        DPSModbusService_SwitchCharVal
    },
    // Characteristic User Description
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        SwitchDesc
    },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t DPSModbusService_ReadAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                                           uint16_t maxLen, uint8_t method );
static bStatus_t DPSModbusService_WriteAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                            uint8_t *pValue, uint16_t len, uint16_t offset,
                                            uint8_t method );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t DPSModbusServiceCBs =
{
  DPSModbusService_ReadAttrCB,  // Read callback function pointer
  DPSModbusService_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*
 * DPSModbusService_AddService- Initializes the DPSModbusService service by registering
 *          GATT attributes with the GATT server.
 *
 */
extern bStatus_t DPSModbusService_AddService( uint8_t rspTaskId )
{
  uint8_t status;

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( DPSModbusServiceAttrTbl,
                                        GATT_NUM_ATTRS( DPSModbusServiceAttrTbl ),
                                        GATT_MAX_ENCRYPT_KEY_SIZE,
                                        &DPSModbusServiceCBs );

  return ( status );
}

/*
 * DPSModbusService_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
bStatus_t DPSModbusService_RegisterAppCBs( DPSModbusServiceCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    pAppCBs = appCallbacks;

    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}

/*
 * DPSModbusService_SetParameter - Set a DPSModbusService parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t DPSModbusService_SetParameter( uint8_t param, uint16_t len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case DPSMODBUSSERVICE_VOLTAGECHAR_ID:
      if ( len == DPSMODBUSSERVICE_VOLTAGECHAR_LEN )
      {
        memcpy(DPSModbusService_VoltageCharVal, value, len);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case DPSMODBUSSERVICE_CURRENTCHAR_ID:
      if ( len == DPSMODBUSSERVICE_CURRENTCHAR_LEN )
      {
        memcpy(DPSModbusService_CurrentCharVal, value, len);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case DPSMODBUSSERVICE_BRIGHTNESSCHAR_ID:
      if ( len == DPSMODBUSSERVICE_BRIGHTNESSCHAR_LEN )
      {
        memcpy(DPSModbusService_BrightnessCharVal, value, len);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case DPSMODBUSSERVICE_SWITCHCHAR_ID:
      if ( len == DPSMODBUSSERVICE_SWITCHCHAR_LEN )
      {
        memcpy(DPSModbusService_SwitchCharVal, value, len);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  return ret;
}


/*
 * DPSModbusService_GetParameter - Get a DPSModbusService parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t DPSModbusService_GetParameter( uint8_t param, uint16_t *len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case DPSMODBUSSERVICE_VOLTAGECHAR_ID:
      memcpy(value, DPSModbusService_VoltageCharVal, DPSMODBUSSERVICE_VOLTAGECHAR_LEN);
      break;

    case DPSMODBUSSERVICE_CURRENTCHAR_ID:
      memcpy(value, DPSModbusService_CurrentCharVal, DPSMODBUSSERVICE_CURRENTCHAR_LEN);
      break;

    case DPSMODBUSSERVICE_BRIGHTNESSCHAR_ID:
      memcpy(value, DPSModbusService_BrightnessCharVal, DPSMODBUSSERVICE_BRIGHTNESSCHAR_LEN);
      break;

    case DPSMODBUSSERVICE_SWITCHCHAR_ID:
      memcpy(value, DPSModbusService_SwitchCharVal, DPSMODBUSSERVICE_SWITCHCHAR_LEN);
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  return ret;
}


/*********************************************************************
 * @fn          DPSModbusService_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t DPSModbusService_ReadAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                       uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                                       uint16_t maxLen, uint8_t method )
{
  bStatus_t status = SUCCESS;

  // See if request is regarding the VoltageChar Characteristic Value
if ( ! memcmp(pAttr->type.uuid, DPSModbusService_VoltageCharUUID, pAttr->type.len) )
  {
    if ( offset > DPSMODBUSSERVICE_VOLTAGECHAR_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DPSMODBUSSERVICE_VOLTAGECHAR_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the CurrentChar Characteristic Value
else if ( ! memcmp(pAttr->type.uuid, DPSModbusService_CurrentCharUUID, pAttr->type.len) )
  {
    if ( offset > DPSMODBUSSERVICE_CURRENTCHAR_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DPSMODBUSSERVICE_CURRENTCHAR_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the BrightnessChar Characteristic Value
else if ( ! memcmp(pAttr->type.uuid, DPSModbusService_BrightnessCharUUID, pAttr->type.len) )
  {
    if ( offset > DPSMODBUSSERVICE_BRIGHTNESSCHAR_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DPSMODBUSSERVICE_BRIGHTNESSCHAR_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the SwitchChar Characteristic Value
else if ( ! memcmp(pAttr->type.uuid, DPSModbusService_SwitchCharUUID, pAttr->type.len) )
  {
    if ( offset > DPSMODBUSSERVICE_SWITCHCHAR_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DPSMODBUSSERVICE_SWITCHCHAR_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  else
  {
    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has READ permissions.
    *pLen = 0;
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  return status;
}


/*********************************************************************
 * @fn      DPSModbusService_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t DPSModbusService_WriteAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                        uint8_t *pValue, uint16_t len, uint16_t offset,
                                        uint8_t method )
{
  bStatus_t status  = SUCCESS;
  uint8_t   paramID = 0xFF;

  // See if request is regarding a Client Characterisic Configuration
  if ( ! memcmp(pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len) )
  {
    // Allow only notifications.
    status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                             offset, GATT_CLIENT_CFG_NOTIFY);
  }
  // See if request is regarding the VoltageChar Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, DPSModbusService_VoltageCharUUID, pAttr->type.len) )
  {
    if ( offset + len > DPSMODBUSSERVICE_VOLTAGECHAR_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == DPSMODBUSSERVICE_VOLTAGECHAR_LEN)
        paramID = DPSMODBUSSERVICE_VOLTAGECHAR_ID;
    }
  }
  // See if request is regarding the CurrentChar Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, DPSModbusService_CurrentCharUUID, pAttr->type.len) )
  {
    if ( offset + len > DPSMODBUSSERVICE_CURRENTCHAR_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == DPSMODBUSSERVICE_CURRENTCHAR_LEN)
        paramID = DPSMODBUSSERVICE_CURRENTCHAR_ID;
    }
  }
  // See if request is regarding the BrightnessChar Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, DPSModbusService_BrightnessCharUUID, pAttr->type.len) )
  {
    if ( offset + len > DPSMODBUSSERVICE_BRIGHTNESSCHAR_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == DPSMODBUSSERVICE_BRIGHTNESSCHAR_LEN)
        paramID = DPSMODBUSSERVICE_BRIGHTNESSCHAR_ID;
    }
  }
  // See if request is regarding the SwitchChar Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, DPSModbusService_SwitchCharUUID, pAttr->type.len) )
  {
    if ( offset + len > DPSMODBUSSERVICE_SWITCHCHAR_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == DPSMODBUSSERVICE_SWITCHCHAR_LEN)
        paramID = DPSMODBUSSERVICE_SWITCHCHAR_ID;
    }
  }
  else
  {
    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has WRITE permissions.
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  // Let the application know something changed (if it did) by using the
  // callback it registered earlier (if it did).
  if (paramID != 0xFF)
    if ( pAppCBs && pAppCBs->pfnChangeCb )
      pAppCBs->pfnChangeCb(connHandle, paramID, len, pValue); // Call app function from stack task context.

  return status;
}
