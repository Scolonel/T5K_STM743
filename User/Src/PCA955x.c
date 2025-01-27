/**
  ******************************************************************************
  * @file    pca9554.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28 december-2014
  * @brief   This file includes the driver for control PCA9554 .
  ******************************************************************************
  * @attention
  *
   *
  ******************************************************************************
������������� ������ 512 = 64� ����
  */ 

/* Includes ------------------------------------------------------------------*/
#include "pca955x.h"
#include "main.h"
/** @addtogroup Component
  * @{
  */ 
  
HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;

uint8_t MemBlock[256]; // ���� ��� ���������� ����� ������

/**
  * @}
  */ 
// "�������" ��� ������� ������ �� I2C ��� �� �� ������ ���� �����
HAL_StatusTypeDef TOP_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, DevAddress, Trials, Timeout);
  return status;
}

HAL_StatusTypeDef TOP_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout);
  return status;
}

HAL_StatusTypeDef TOP_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout);
  return status;
}


uint32_t GetExpand (void) // ��������� ��������� ����� Expanders (9554), ����� ������ 23-16:9554,15-0:9555
{
  uint32_t Result = 0;
//  HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
  uint8_t BufTX[4];
  uint8_t BufRX[4];
  //GPIOE->BSRRH = GPIO_PIN_3; // (PE3) reset pin

  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(KEYBOARD<<1) , 1, 10000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2<<26;
    // read current stats
  BufTX[0] = 0;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(KEYBOARD<<1), BufTX, 1, 10000);
  StatusI2C2 = TOP_I2C_Master_Receive(&hi2c2, (uint16_t)(KEYBOARD<<1), BufRX, 2, 10000);
// ����� ���� � �����
  Result = (uint32_t)(BufRX[0]); // �������
  Result = Result<<8;
  Result += (uint32_t)(BufRX[1]);// �������
  // ��� ��� �������� ������ ��� ���������� 7 ��� (��������� ����� �����)
  
  return (Result & 0x3F);

}

//������������ PCA9554 ( T7KAR_743) ���������� ����� 0x38

HAL_StatusTypeDef ConfigKBRD (void)
{
  //uint16_t DevAddr = EXPWRCTRL<<1; // address I2C
  uint8_t BufTX[10];
  //uint8_t BufRX[10];
//    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(EXPWRCTRL<<1), 2, 1000); // ���� ��� ����������???
    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(KEYBOARD<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
  // ����� � ����� 1 -
  BufTX[0] = 1;
  BufTX[1] = 0x3F;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(KEYBOARD<<1), BufTX, 2, 1000);
  if (StatusI2C2) return StatusI2C2;
  // ��� �������� -
  BufTX[0] = 2;
  BufTX[1] = 0x0;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(KEYBOARD<<1), BufTX, 2, 1000);
  if (StatusI2C2) return StatusI2C2;
  // ������������� ���������� ��� ����� ������ �0-�7 - 
  BufTX[0] = 3;
  BufTX[1] = 0xFF;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(KEYBOARD<<1), BufTX, 2, 1000);
  //if (StatusI2C2) return StatusI2C2;
  //BufTX[0] = 0;
  //StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(KEYBOARD<<1), BufTX, 1, 10000);
 // StatusI2C2 = TOP_I2C_Master_Receive(&hi2c2, (uint16_t)(EXPWRCTRL<<1), BufRX, 1, 10000);
  //if (BufRX[0] != 0xff) return BufRX[0];
  return StatusI2C2;
  }
//    
//������������ PCA9555 ( ������ - ����������) ������ 0x20
// ������������� ���  ������ P0-P15! �������� �� �� ��� ����� ������ ��� ������ ���� ��� �����
HAL_StatusTypeDef ConfigPWRCtrl (void)
{
    //uint16_t DevAddr = EXPWRCTRL<<1; // address I2C
  uint8_t BufTX[10];
  //uint8_t BufRX[10];
    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(EXPWRCTRL<<1), 1, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
  // ����� � OUT ����� 0 - 
  BufTX[0] = 2;
  BufTX[1] = 0x00;
  BufTX[2] = 0x00;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EXPWRCTRL<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
  // ��� �������� - 
  BufTX[0] = 4;
  BufTX[1] = 0x0;
  BufTX[2] = 0x0;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EXPWRCTRL<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
  // ������������� ���������� ��� ����� ������ �0-P15 - �����
  BufTX[0] = 6;
  BufTX[1] = 0x00; 
  BufTX[2] = 0x00;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EXPWRCTRL<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
//  BufTX[0] = 6;
//  BufTX[1] = 0x0;
//  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(COMMCTRL<<1), BufTX, 2, 10000);
   return StatusI2C2;

}


// ��� ��� � ��� ���� ������������ � �� �������� �� �����
HAL_StatusTypeDef CtrlExpand ( uint16_t PinOut, uint16_t Mask) // ���������� ������ ������� Expanders ( 9555),  P0-P15 
// uint16_t Mask - ����� ���� ������,  PinOut - �������� ������� ������,
//��� ��� ������������� �� ����� ����� ������ ���� ��� ��������
{
  //HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
//  StatusI2C2 = HAL_ERROR;
  uint8_t BufTX[3];
  uint8_t BufRX[3];
  volatile uint16_t InPin1 = PinOut & Mask; // �������������  ��� �������� ����� ����������
  //volatile uint16_t InPin2 = PinOut & Mask; // ����� ��������� ��� �������������  
  
    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(EXPWRCTRL<<1), 1, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
    // ������� ������� ���� ����������������� � ������������ � PinOUT
    // read current stats LOEW byte
  BufTX[0] = 0;   // ����� �������  0 - ������ ��������� ������� ������ 
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EXPWRCTRL<<1), BufTX, 1, 1000);
  // ������ 2 ����� ( ����� ��� ) ������� ������
  StatusI2C2 = TOP_I2C_Master_Receive(&hi2c2, (uint16_t)(EXPWRCTRL<<1), BufRX, 2, 1000); // ��������� ��� ����� �������� ������������� ������
  // ������������� �������� �������� ���������
  BufRX[0] &= ~(uint8_t)(Mask & 0xFF); // 
  BufRX[0] |= (uint8_t)(InPin1 & 0xFF); // 
  BufRX[1] &= ~(uint8_t)(Mask >> 8);
  BufRX[1] |= (uint8_t)(InPin1 >> 8);;
  // ������ ���� ��������
  BufTX[0] = 2 ; // ������ �������
  BufTX[1] = BufRX[0]; 
  BufTX[2] = BufRX[1]; 
  // write NEW stat in IC LOW Byte
   StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EXPWRCTRL<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
  
  return StatusI2C2 ;
  }


// ������ � ������ ��������������� ���������
void WriteNeedStruct (int NumStruct)
  {
    if(NumStruct)
    {
      if (NumStruct & 0x01)   EEPROM_write(&ConfigDevice, ADR_ConfigDevice, sizeof(ConfigDevice));; // ����������� ������������  ������� ��� 0(0x01)
      NumStruct = NumStruct>>1;
      if (NumStruct & 0x01) EEPROM_write(&UserSet, ADR_UserMeasConfig, sizeof(UserSet)); // ������������ ��������� ��������� ������������ ��� 2 (0x02)
      NumStruct = NumStruct>>1;
      if (NumStruct & 0x01) EEPROM_write(&CoeffLW, ADR_CoeffPM, sizeof(CoeffLW)); // ���������� ������������� ��������� ���������� P1 ��� 1  (0x04)
      NumStruct = NumStruct>>1;
 
    }
  }



//

uint32_t BeginConfig (void) // ��������� ������������
{
  uint32_t CodeError = 0; //
    uint32_t ErrDev=0;
    uint32_t SPCTR_err=0;
  // �������� ������� ���������� ���������� ������� 95555 �� ���� I2C(2)
  //������������ PCA9555 ( ����������� ) ����� 0x20
  // ������������� ��� ����� ���! �������� �� �� ��� ����� ������ ��� ������ ���� ��� �����
  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(KEYBOARD<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) CodeError |= NOT_PCA9554_KB; // ���������� �� ������������ (0x01)
  StatusI2C2 =  ConfigKBRD (); // ������������� PCA9554  
  if (StatusI2C2) CodeError |= CFG_ERR_PCA9554_KB; // ���������� �� ��������������� (0x02)
  //
  //������������ PCA9554 ( ExtKey ) ����� 0x39
  // 
  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(EXPWRCTRL<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) CodeError |= NOT_PCA9555; // ���������� �� ������������ (0x04)
  StatusI2C2 =  ConfigPWRCtrl (); // ������������� PCA9555  
  if (StatusI2C2) CodeError |= CFG_ERR_PCA9555; // ���������� �� ��������������� (0x08)
  //
  
//  // �������� ������� ����������  24lc512 �� ���� I2C(2) EEPROM
  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(EEPROM24LC128<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) CodeError |= NOT_EEPROM_24LC128; // ���������� �� ������������ (0x10)
  else // ���������� ���� ��������� ������������ ������ � �������� ���� ���� ����� ������������ �� 9400
  {
    // ������ ����������� �������
    // ������� �������� ������������ ������� (��������)
    EEPROM_read(&ConfigDevice, ADR_ConfigDevice, sizeof(ConfigDevice));
    ErrDev = InvalidDevice();
    if (ErrDev)
    {
      CodeError |= ERR_DEVCFG; // ������ ������������ ������� (0x80)
      SPCTR_err=0x80; // ���� ������ ������, ������� ������������ ��������������
      InitDevice(ErrDev); 
      EEPROM_write(&ConfigDevice, ADR_ConfigDevice, sizeof(ConfigDevice));
    }
    
    // �������� ��������� ������������ ��������� �������
    EEPROM_read(&UserSet, ADR_UserMeasConfig, sizeof(UserSet));
    SPCTR_err = SPCTR_err + CheckUserGonfig ();  // �������� ���������������� �������� � ��� �� �����������
    if (SPCTR_err>=0x100)// ���� ������ �� ��� ��������� ���� ������ ������������ 
    {
      CodeError |= ERR_USERSET; // ������ ��������������� ��������� (0x200)
      //FixErrUserMC (ErrDev); //  ���������  (���������� ���������)
      EEPROM_write(&UserSet, ADR_UserMeasConfig, sizeof(UserSet));
    }
    // �������� ������� �������
    //EEPROM_read(&CoeffLW, ADR_CoeffPM, sizeof(CoeffLW)); //�������� ������� ������������� � ����������� ������� �������������
    EEPROM_read(&CoeffLW, ADR_CoeffPM, 12800); //�������� ������� ������������� � ����������� ������� �������������
    ErrDev = FindErrCoeff ();
    SPCTR_err = SPCTR_err + ErrDev;
    if (ErrDev)
    {
      CodeError |= ERR_SPECTR; // ������ ���������� ������! (0x100)
      //EEPROM_write(&CoeffLW, ADR_CoeffPM, sizeof(CoeffLW));
      EEPROM_write(&CoeffLW, ADR_CoeffPM,12800);
    }
    
  }
  //return (CodeError | (ErrDev<<3)); 
  return (CodeError); 
}



//������� ������ EEPROM
HAL_StatusTypeDef EEPROM_write(void *buff, uint16_t address, uint32_t byte)
{ 
#define size_pg 128
  uint8_t BufTX[140];
  uint8_t *ptr = buff;
  uint16_t n_pg = byte/size_pg ; // ����� �������
  uint16_t last_byte=byte - (n_pg*size_pg); // ������� �����
  
  
  HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(EEPROM24LC128<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
  //����� �� ����� 64 ����  �� ���
  for (int j=0; j<n_pg;j++)
  {
    BufTX[0] = (uint8_t)((address+(j*size_pg))>>8 & 0xFF);
    BufTX[1] = (uint8_t)((address+(j*size_pg)) & 0xFF);
    for (int i=0; i<size_pg; ++i)
    {
      BufTX[2+i] = *ptr++;
    }
    StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EEPROM24LC128<<1), BufTX, size_pg+2, 10000); // ���������� ������ ����� �� ����� 64 ����
     HAL_Delay(10);
  }
  
  BufTX[0] = (uint8_t)((address+(n_pg*size_pg))>>8 & 0xFF);
  BufTX[1] = (uint8_t)((address+(n_pg*size_pg)) & 0xFF);
  for (int i=0; i<last_byte; ++i)
  {
    BufTX[2+i] = *ptr++;
  }
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EEPROM24LC128<<1), BufTX, last_byte+2, 10000); // ���������� ������ ����� �� ����� 64 ����
  HAL_Delay(10);

  
  return StatusI2C2;
}

//������� ������ EEPROM
HAL_StatusTypeDef EEPROM_read(void *buff, uint16_t address, uint32_t byte)
{
  uint8_t BufTX[3];
  HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
  
    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c2, (uint16_t)(EEPROM24LC128<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
  // ������ ������ ����� ���� - 
  // ������� ���������
  // ������������� �����
  BufTX[0] = (uint8_t)(address>>8 & 0xFF);
  BufTX[1] = (uint8_t)(address & 0xFF);
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(EEPROM24LC128<<1), BufTX, 2, 1000);
  TOP_I2C_Master_Receive(&hi2c2, (uint16_t)(EEPROM24LC128<<1), buff, byte, 10000);

  return StatusI2C2;
  }

