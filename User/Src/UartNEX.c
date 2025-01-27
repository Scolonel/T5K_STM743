/*****************************************************************************
 *   
 *  ��� STM32H743 - ��������� ���� �� uart.c T7kAR ������ ��� 
 *  NEXTION 
 *  ���� UART2 ������ uart7  
 *
 *  
 *   2024.11.05  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/

#include "system.h"

//  
volatile BYTE UART2TxEmpty = 1;
volatile BYTE UART2Buffer[BUFSIZEUART2];
volatile DWORD UART2Count = 0;
volatile DWORD RecievNEX = STOP_UART;
uint8_t RX_BufNEX[BUFSIZEUART2];
volatile DWORD CntRXNEX = 0;
volatile BYTE Uart2DecYes = 0;
volatile BYTE ReadyNEX=0; // ���������� ���������� NEXTION
volatile BYTE g_WtRdyNEX = 1;
volatile BYTE ProgFW_LCD =0; // ������� ���������������� ���������� Nexion
volatile WORD CountTimerUart2 = 0;
char wrd[10];
BYTE RSOptYes = 0;
volatile BYTE g_GetStr; // ������� �������� ��������� ������ �� ���������. 



void CheckStrNEX (void) // �������� �������� ������
{
     if(g_GetStr==1)//����� ��� ���� ������� ������, � ������������ ����� 
   {
     if((CntRXNEX>1)&&(RX_BufNEX[CntRXNEX-1]==0xa7)&&(RX_BufNEX[0]==0xa4))
     g_GetStr=2;
   }
}
//#pragma optimize=size  
char GetStringNEX(char* NEXStr, int Size)
{

    char Indx =(CntRXNEX<Size+2)?(CntRXNEX-2):(Size-1);
    memcpy(NEXStr,(void*)&RX_BufNEX[1],Indx); 
    NEXStr[Size-1]=0;
    return Indx;
}

// ��� � ����������

void StartRecievNEX (uint32_t TimeOut) // ���������� �������� ������ �� Nextion� �������� �������, � ���������� ������� ��������
{
  RX_BufNEX[0] = 0; // �������� ������ �������� �� NEXTION
  CntRXNEX=0; // ������ ������� �������� ����
  RecievNEX=STOP_UART;// ��������� � ����� ������� ������� �����
  g_WtRdyNEX = 0;// ������ ������� ��������� �������� �� ��������
  ReadyNEX = 0; // ������ ������� �������� ������� � ����������� ( ���� ���������� ����� ������������� ��� �����)
  CountTimerUart2 = TimeOut;// ������������ ����� �������� � mS 
  uint16_t  Dummy = huart7.Instance->RDR ; // ������ ������ ������ �� NEXTION
  HAL_UART_Receive_IT(&huart7, RX_BufNEX,1); // ���� �������� ������� ����� �� �������� ����

}





// ����� ����� �� UART2 LCD
void RS_LCD (BYTE temp)
{
  //     CDC_Transmit(0, (uint8_t*)(TRD), 1); // echo back on same channel
  
  //GetRstTMUart2(1); // ������� ������ Uart2
  CountTimerUart2 = 50;// ������������ ����� �������� � mS �������� ���������� �����
  if(ProgFW_LCD)
  {
    RX_BufNEX[CntRXNEX]=temp;
    CntRXNEX++;
  }
  else // ������� ������
  {
    switch (RecievNEX)
    {
    case STOP_UART:                    //��������� ���������� � ����� ����� �������
      if (temp)  // ���-�� ������� ��������� ������ �����
      {
        CntRXNEX=0;
        RX_BufNEX[CntRXNEX]=temp;
        RecievNEX=START_UART;
        CntRXNEX++;
        if(temp == 0xa4) // ��� �� ������ ������������ ��
          g_NeedChkAnsvNEX=1; // ������ ������ ������ �� ���������� �� �� 
      }
      break;
    case START_UART:                   //��������� ����� ������� ������� (���������� V <-> W) 02/09/03
      {
        switch (temp)
        {
        case 0xa7:                 //����� ������� 
          RX_BufNEX[CntRXNEX++]=temp;
          if(g_NeedChkAnsvNEX == 1) // ��������� ����� �� ���������
          {
            g_NeedChkAnsvNEX = 2; // ����� ������ ����� ����������
          }
          break;
        case '�':                 //����� ������� 
          //CntRXOpt=0;
          RX_BufNEX[CntRXNEX++]=temp;
          if ((CntRXNEX>3)&&(!memcmp ((void*)&RX_BufNEX[CntRXNEX-3], "���",3)))
          {
            RecievNEX=END_UART;
            RX_BufNEX[CntRXNEX]=0x0;
            Uart2DecYes = 1;            //������� ������� ���������, ������� ���� ���������� ��������� �������RSDecode();
            switch(RX_BufNEX[0])
            {
            case 0xFD: // ��������� �������� �����!
              ReadyNEX = 1;
              break;
            case 0xFE: // ����� ������� ����
              ReadyNEX = 2;
              break;
            case 0x1A: // error
              ReadyNEX = 3;
              break;
            case 0x70: // text block
              ReadyNEX = 4;
              break;
            case 'c': // text block ������ �� connect
              ReadyNEX = 5;
              break;
            default:
              break;
            }
          }//
          
          break;
        default:                  //����������� ����� �������
          RX_BufNEX[CntRXNEX]=temp;
          CntRXNEX++;
          if (CntRXNEX > 80)
          {
            CntRXNEX=0;
            RecievNEX=STOP_UART;
          }
          break;
        }
      }
    case END_UART:                     //��������� �������� ��������� �������
      break;                      //���������� ������� �� ���������� - ������� ������ �� ������
    }
  }
}





/******************************************************************************
**                            End Of File
******************************************************************************/
