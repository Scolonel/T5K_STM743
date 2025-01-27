#include "system.h"
//#include "msgs.h"

// TODO: to Utils.c

int intabs( int a )
{
  //return a & 0x7FFFFFFF;
  return (a > 0 ?  a : -a);
}

extern unsigned long Sum_sm;
//static volatile unsigned char screen[ ScrW * ScrH / PIX_IN_BYTE ];
static unsigned   CursLevelMin;
static unsigned   cMM;

// ��������� ��� �������� - ������������� � ������� ��������� ������
//static  Rect rct;
// ��������� �������� ���������� ���������
//static  GraphParams params;


int WMax (int a, int b)
{
 return a > b ? a : b;
}

int WMin (int a, int b)
{
 return a > b ? b : a;
}



 unsigned short central; // ��������� ��������� 




/*void PutPixel(int x, int y,char mas[1024])
{
	PutPixel(x, y,mas,true);
}*/
inline void Swap( int* _1, int* _2 ) //��� ��������?
{
  int tmp = *_1;
  *_1 = *_2;
  *_2 = tmp;
}







// ���� ����� ������� ���� ������, ��:
// coeffx = (size-first)/(rct->right-rct->left);
// SCR_WIDTH = ������ ������ 
// - �� ���� ������ �� �� ������ ������ ������ �����
unsigned MinMax( unsigned short* array, unsigned nums )
{
  unsigned short min = 0xFFFF, max = 0;
  for( unsigned i = 0; i < nums; ++i )
  {
    if( min > array[i] )
      min = array[i];
    if( max < array[i] )
      max = array[i];
  }
  return ((max << 16) + min);
}

unsigned MinMaxNEX( unsigned short* array, unsigned nums )
{
  unsigned short min = 0xFFFF, max = 0;
  unsigned OutData;
  int Grad = 0;
  for( unsigned i = 0; i < nums; ++i )
  {
    if( min > array[i] )
    {
      min = array[i];
      Grad++;
    }
    if( max < array[i] )
    {
      max = array[i];
      Grad--;
    }
  }
  if (Grad<0)
  {
    OutData = ((max << 16) + min);
  }
  else
  {
    OutData = ((min << 16) + max);
  }
  return OutData;
}


unsigned short Adjust4Nex( int val, int center, int range )
{
  if( val > (center + range/2) )
    val = center + range/2; 
  if( val < (center - range/2) )
    val = center - range/2;
  return val + range/2 - center;//
}


//unsigned short Adjust2( int center, int min, int max, int rng )
//{
//  if( center > max - rng/2 )
//   center = max - rng/2;
//  if( center < min + rng/2 )
//    center = min + rng/2;
//  return center;
//}

//unsigned short MakeGraph( Rect* rct, unsigned short* array, unsigned size, GraphParams* prms)
//{
// //const unsigned coeffx = 1; // ��������� ������ �� X; ���� Scale
//  unsigned char points[ ScrW*2 ]; // ������ ��� ������� ������ - ����� ������� �� ������� � ����������������, ���� ����
//  unsigned width = rct->right - rct->left; // ������ ������� ����������� � ��������
//  int height = rct->bottom - rct->top; // ������ ������� ����������� � ��������
//  int pseudoLast = prms->cursor + (width * prms->scale) / 2; // ���������� ��������� ����� (� ��������) ��� ������ � ���������
//  int pseudoFirst = prms->cursor - (width * prms->scale) / 2; // ���������� ������ ����� (� �������) ��� ������ � ���������
//  unsigned first = (unsigned)WMax( pseudoFirst, 0 ); //  �������� ���
//  if( pseudoLast > size ) //�����, ����� �������� ����-?
//    first = size - width * prms->scale; // ��� ����� ���� +-�������
//  /**/unsigned short locmax = 0, locmin = 0xFFFF;
//  for( unsigned i = first; i < first + width * prms->scale; ++i ) // ����� ��������� ��� � ��� �� ������������ �������
//  {
//    if( array[i] > locmax )
//      locmax = array[i];
//    if( array[i] < locmin )
//      locmin = array[i];
//  }
//  /**/cMM = MinMax( &array[ prms->cursor ], prms->scale ); // ��� ��� �� ������� ���� ��������� ������ 
//    CursLevelMin = MinMax( &array[PosCursorSlave(0) ], prms->scale );// ��� ��� ��� ������� �������
//    CursLevelMin = (CursLevelMin << 16);
//    CursLevelMin += (cMM&0xFFFF);
//  
//  //(�� ���� ���������� ����� ��� ��� �������� �� ������
//  // ������ � ������������� ������������ �� �����������
//  unsigned short steppics = prms->maxy/height; //������ ����������� (22000) �� ����� �������� �����������
//  switch (prms->typecentral)
//  {
//  case NOCHANGE:
//    break;
//  case UPPICS:
//    if ((central)<(locmax-prms->maxy/2-steppics)) central += steppics; // ����� �������� ����� �� 1 ������
//    break;
//  case DOWNPICS:
//    if (central > (prms->maxy/2+steppics)) central -= steppics; // ����� �������� ���� �� 1 ������
//    break;
//  case UP10PICS:
//    if ((central+7*steppics)<(GetVerticalSize (0) - prms->maxy/2)) central += 7*steppics; // ����� �������� ����� �� 7 ��������
//    
//    //if ((central-locmin)>(prms->maxy/2-7*steppics)) central += 7*steppics; // ����� �������� ����� �� 7 ��������
//    break;
//  case DOWN10PICS:
//    if ((central-7*steppics)>(prms->maxy/2)) central -= 7*steppics; // ����� �������� ����� �� 7 ��������
//    //if ((locmax - central)>(prms->maxy/2+7*steppics)) central -= 7*steppics; // ����� �������� ���� �� 7 ��������
//    break;
//  case FIXSIZE:
//  /**/ //central =  (locmin + locmax)>>2; // ���������� ������ ����������� �� �������� ������������ ������
//  /**/ central = GetVerticalSize (0)>>1; // ����� �� ���������� ��������
//    break;
//  case MEDIUM:
//  /**/ //central =  (locmin + locmax)>>1; // ���������� ������ ����������� �� �������� ������������ ������
//  /**/ //central = GetVerticalSize(GetUserVerSize())>>1; // ���������� ������ ����������� �� �������� ������������ ������
//  /**/ central = GetVerticalSize (0)>>1; // ����� �� ���������� ��������
//    break;
//  case BYCUR:
//  /*unsigned short */central = ((cMM&0xFFFF)+(cMM>>16))/2; // ���������� ������ ����������� �� ��� ���������� ������
//    break;
//  case MEMDRAW: // ��������� ��� ��������� ������
//  /**/ //central =  (locmin + locmax)>>2; // ���������� ������ ����������� �� �������� ������������ ������
//  /**/ //central = GetVerticalSize (0)>>1; // ���������� ������ ����������� �� �������� ������������ ������
//  /**/ central = prms->maxy/2; // ���������� ������ ����������� �� �������� ������������ ������
//    break;
//  }
//  /* central = Adjust2((cMM&0xFFFF+cMM>>16)/2, locmin, locmax, prms->maxy );*/ // ���������� ������ ����������� �� ��� ���������� ������
//  /*unsigned short *///central = (locmin + locmax)/2; // ���������� ������ ����������� �� ��� ���������� ������
//  for( unsigned i = 0; i < width; ++i ) // ���������� ������� ������
//  {
//    /**/unsigned minmax = MinMax(&array[ first + i*prms->scale ], prms->scale ); // ���� ��� ������ ������������� �����
//    // ����������� ���� � ���
//    unsigned short min = (minmax&0xFFFF);
//    unsigned short max = (minmax>>16);
//    /**/min = Adjust3((int)min, (int)central,(int)prms->maxy );
//    max = Adjust3((int)max, (int)central,(int)prms->maxy);
//    //��������� � �������, 1.����� ��������, 2. ����������� ������.3. ��� ����� �� ���� ��������
//    
//    points[2*i] =  (unsigned char)(height*min / prms->maxy );
//    //NexData[2*(width-i)-1] = 2*(56-points[2*i]);//NexData[2*i] = points[2*i]>>16;
//    points[2*i+1] =  (unsigned char)(height*max / prms->maxy );
//    //NexData[2*(width-i)] = 2*(56-points[2*i+1]);//NexData[2*i+1] = points[2*i+1]>>16;
//  }
////  // ��������� ������� ��� ������� �� ��������� ��� ����� � ���� ������ ��� NEXTION
////  for( unsigned i = 0; i < 195; ++i ) //195 ���������� ������� ������ ��� NEXTION ���� �� ������ �����
////  {
////    /**/unsigned minmax = MinMax(&array[i*21], 21 ); // ���� ��� ������ ������������� ����� �� 21 �����
////    int Mi=(210-(minmax&0xFFFF)/167);
////    int Ma=(210-(minmax>>16)/167);
////    if(Mi<0) Mi=0;
////    if(Ma<0) Ma=0;
////    NexData[2*(195-i)-1] = (unsigned char)(Mi);//NexData[2*i] = points[2*i]>>16;
////    //NexData[2*(195-i)-1] = (unsigned char)((Ma+Mi)>>1);//NexData[2*i] = points[2*i]>>16;
////    NexData[2*(195-i)] = (unsigned char)(Ma);//NexData[2*i+1] = points[2*i+1]>>16;
////    //NexData[3*(195-i)] = (unsigned char)(Ma);//NexData[2*i+1] = points[2*i+1]>>16;
////  }
//  // ��������� ������� ��� ������� �� ��������� ������ 390 ����� �� 0 �� 10�� ��� NEXTION
//  // ����������� 210 ������������ �������� ��� ����������� ������� ����� ������ 0 ��� �����������
//  // �������� �� 0�� �� 15�� 
////  for( unsigned i = 0; i < 390; ++i ) //195 ���������� ������� ������ ��� NEXTION ���� �� ������ �����
////  {
////    int Shft = (prms->cursor>390)?(prms->cursor-390+i):(i);
////    int Mi=array[Shft];
////    if (Mi>10000) Mi=10000;
////    if (Mi<6865) Mi=6865;
////    int Ma=(209-(Mi-6865)/15);
////    NexData[390-i] = (unsigned char)(Ma);//NexData[2*i] = points[2*i]>>16;
////  }
//  DrawGraph( rct, points );
//  
//  // ������: ���� ������ ������������ ������, ������ ����� ���������, ��� ������ =)
//  int crsM = rct->left+(prms->cursor-first)/prms->scale;
//  int crsS = rct->left+(PosCursorSlave(0)-first)/prms->scale;; 
//  char Str[2] = {'>',0};
//  const int lineNum = 4;
//  switch (prms->numcur)
//  {
//  case 2:// ���� ������ ������
//    DrawLine( crsS, rct->top, crsS, rct->bottom, 1, 1 );
//    if (PosCursorSlave(0)>=first + prms->scale*rct->right)
//    {
//      putString(120,10,Str,1,0);
//    }
//    if (PosCursorSlave(0)<first)
//    {
//      Str[0] = '<';
//      putString(0,10,Str,1,0);
//    }
//  case 1:// ������ ���� ������
//    for (int i=0;i<lineNum;i++)
//    {
//      DrawLine( crsM, rct->top + (height/lineNum)*i, crsM, rct->top + (height/lineNum)*(i+1)-height/(lineNum*3), 1, 1 );
//    }
//  default:// �������� ���
//    break;
//  }
//  return central;
//}
// ������ ��� Nextion
unsigned MakeGraphNext( Rect* rct, unsigned short* array, unsigned size, GraphParams* prms) // ������ ���� ������
{
 //const unsigned coeffx = 1; // ��������� ������ �� X; ���� Scale
  unsigned char points[ ScrW_N*2 ]; // ������ ��� ������� ������ - ����� ������� �� ������� � ����������������, ���� ����
  // NexData[390] - ��� �������!
  unsigned width = rct->right - rct->left; // ������ ������� ����������� � ��������
  int height = rct->bottom - rct->top; // ������ ������� ����������� � ��������
  //int ScaleN=(prms->scale>16)?(10):(prms->scale);
  int ScaleN=prms->scale;
  int pseudoLast = prms->cursor + (width * ScaleN) / 2; // ���������� ��������� ����� (� ��������) ��� ������ � ���������
  int pseudoFirst = prms->cursor - (width * ScaleN) / 2; // ���������� ������ ����� (� �������) ��� ������ � ���������
  unsigned first = (unsigned)WMax( pseudoFirst, 0 ); //  �������� ���
  if( pseudoLast > size ) //�����, ����� �������� ����-?
    first = size - width * ScaleN; // ��� ����� ���� +-�������
  /**/unsigned short locmax = 0, locmin = 0xFFFF;
  for( unsigned i = first; i < first + width * ScaleN; ++i ) // ����� ��������� ��� � ��� �� ������������ �������
  {
    if( array[i] > locmax )
      locmax = array[i];
    if( array[i] < locmin )
      locmin = array[i];
  }
  /**/cMM = MinMax( &array[ prms->cursor ], ScaleN ); // ��� ��� �� ������� ���� ��������� ������ 
    CursLevelMin = MinMax( &array[PosCursorSlave(0) ], ScaleN );// ��� ��� ��� ������� �������
    CursLevelMin = (CursLevelMin << 16);
    CursLevelMin += (cMM&0xFFFF);
  
  //(�� ���� ���������� ����� ��� ��� �������� �� ������
  // ������ � ������������� ������������ �� �����������
  unsigned short steppics = prms->maxy/height; //������ ����������� (22000) �� ����� �������� �����������
  switch (prms->typecentral)
  {
  case NOCHANGE:
    break;
  case UPPICS:
    if ((central)<(locmax-prms->maxy/2-steppics)) central += steppics; // ����� �������� ����� �� 1 ������
    break;
  case DOWNPICS:
    if (central > (prms->maxy/2+steppics)) central -= steppics; // ����� �������� ���� �� 1 ������
    break;
  case UP10PICS:
    if ((central+7*steppics)<(GetVerticalSize (0) - prms->maxy/2)) central += 7*steppics; // ����� �������� ����� �� 7 ��������
    
    //if ((central-locmin)>(prms->maxy/2-7*steppics)) central += 7*steppics; // ����� �������� ����� �� 7 ��������
    break;
  case DOWN10PICS:
    if ((central-7*steppics)>(prms->maxy/2)) central -= 7*steppics; // ����� �������� ����� �� 7 ��������
    //if ((locmax - central)>(prms->maxy/2+7*steppics)) central -= 7*steppics; // ����� �������� ���� �� 7 ��������
    break;
  case FIXSIZE:
  /**/ //central =  (locmin + locmax)>>2; // ���������� ������ ����������� �� �������� ������������ ������
  /**/ central = GetVerticalSize (0)>>1; // ����� �� ���������� ��������
    break;
  case MEDIUM:
  /**/ //central =  (locmin + locmax)>>1; // ���������� ������ ����������� �� �������� ������������ ������
  /**/ //central = GetVerticalSize(GetUserVerSize())>>1; // ���������� ������ ����������� �� �������� ������������ ������
  /**/ central = GetVerticalSize (0)>>1; // ����� �� ���������� ��������
    break;
  case BYCUR:
  /*unsigned short */central = ((cMM&0xFFFF)+(cMM>>16))/2; // ���������� ������ ����������� �� ��� ���������� ������
    break;
  case MEMDRAW: // ��������� ��� ��������� ������
  /**/ //central =  (locmin + locmax)>>2; // ���������� ������ ����������� �� �������� ������������ ������
  /**/ //central = GetVerticalSize (0)>>1; // ���������� ������ ����������� �� �������� ������������ ������
  /**/ central = prms->maxy/2; // ���������� ������ ����������� �� �������� ������������ ������
    break;
  }
  /* central = Adjust2((cMM&0xFFFF+cMM>>16)/2, locmin, locmax, prms->maxy );*/ // ���������� ������ ����������� �� ��� ���������� ������
  /*unsigned short *///central = (locmin + locmax)/2; // ���������� ������ ����������� �� ��� ���������� ������
  for( unsigned i = 0; i < width; ++i ) // ���������� ������� ������
  {
    /**/unsigned minmax = MinMaxNEX(&array[ first + i*ScaleN ], ScaleN*2 ); // ���� ��� ������ ������������� �����
    // ����������� ���� � ���
    unsigned short min = (minmax&0xFFFF);
    unsigned short max = (minmax>>16);
    /**/min = Adjust4Nex((int)min, (int)central,(int)prms->maxy );
    max = Adjust4Nex((int)max, (int)central,(int)prms->maxy);
    //��������� � �������, 1.����� ��������, 2. ����������� ������.3. ��� ����� �� ���� ��������
    unsigned char  Mi = (unsigned char )(height*min / prms->maxy );
    unsigned char  Ma = (unsigned char )(height*max / prms->maxy );
    if(Mi>height-1) Mi=height-1;
    if(Ma>height-1) Ma=height-1;
    if(ScaleN>1)
    NexData[width-(i++)] =  height-1-Mi;
    //NexData[2*(width-i)-1] = 2*(56-points[2*i]);//NexData[2*i] = points[2*i]>>16;
    NexData[width-(i)] =  height-1-Ma;
    //NexData[2*(width-i)] = 2*(56-points[2*i+1]);//NexData[2*i+1] = points[2*i+1]>>16;
  }
//  // ��������� ������� ��� ������� �� ��������� ��� ����� � ���� ������ ��� NEXTION
//  for( unsigned i = 0; i < 195; ++i ) //195 ���������� ������� ������ ��� NEXTION ���� �� ������ �����
//  {
//    /**/unsigned minmax = MinMax(&array[i*21], 21 ); // ���� ��� ������ ������������� ����� �� 21 �����
//    int Mi=(210-(minmax&0xFFFF)/167);
//    int Ma=(210-(minmax>>16)/167);
//    if(Mi<0) Mi=0;
//    if(Ma<0) Ma=0;
//    NexData[2*(195-i)-1] = (unsigned char)(Mi);//NexData[2*i] = points[2*i]>>16;
//    //NexData[2*(195-i)-1] = (unsigned char)((Ma+Mi)>>1);//NexData[2*i] = points[2*i]>>16;
//    NexData[2*(195-i)] = (unsigned char)(Ma);//NexData[2*i+1] = points[2*i+1]>>16;
//    //NexData[3*(195-i)] = (unsigned char)(Ma);//NexData[2*i+1] = points[2*i+1]>>16;
//  }
//  DrawGraph( rct, points );
  
  // ������: ���� ������ ������������ ������, ������ ����� ���������, ��� ������ =)
  int crsM = rct->left+(prms->cursor-first)/ScaleN;
  int crsS = rct->left+(PosCursorSlave(0)-first)/ScaleN; 
  
    if (PosCursorSlave(0)>=first + ScaleN*rct->right) crsS = width; // 400 (������ ������ ������)
    if (PosCursorSlave(0)<first) crsS = width+1; // ������ �����
      
  return (unsigned)((crsS<<16) + crsM);
}


unsigned GetLevelCurs (int Dir)
{
  if (Dir)// ������ ������
  {
  return CursLevelMin;
  }
  else
  return cMM;
    
}
// ��������� �������� �� ������������������ ������, ������� ������������ ����. �������� ����,
// ������ �����. ������������ ����� ������� ����� � ���������� ����.
//void DrawPicture (Rect* rct, const unsigned char* file)
//{
//  unsigned int size =0;
//  for (int i = (int)rct->top; i< (int)rct->bottom; ++i)
//  {
//    for (int j = 0; j < (rct->right-rct->left)>>3; ++j)
//    {
//      
//      for (int k=0; k<8; ++k)
//      {
//         PutPixel(rct->left+j*8+k, i ,((file[size]<<k)&0x80)>>7);
//      }
//      size++;
//    }
//  }
//  
//}
//
//void DrawDigitScore(BYTE Digit, int x, int y)
//{ BYTE Num;
//  if (Digit>9) Digit = 0;
//  for (int j=0; j<3; ++j)
//  {
//    Num = scordigit[Digit][j];
//  for (int i=0; i<5; i++)
//  {
//   PutPixel(x-i, y+j ,(Num>>i)&0x01);
//
//  }
//  }
//}

//void InitRect (Rect* rct, long left, long top, long right, long bottom)
//
//{
//  //Rect rct;
//  rct->left = left;
//  rct->right = right;
//  rct->top = top;
//  rct->bottom = bottom;
//  //GraphParams params;
//}
//
//void InitParams (GraphParams* prms, unsigned cursor, unsigned scale, unsigned maxy, BYTE numcur, BYTE typecentral)
//{
//  prms->cursor = cursor;
//  prms->scale = scale;
//  prms->maxy = maxy;
//  prms->numcur = numcur;
//  prms->typecentral = typecentral;
//}
//MakeGraph( &rct, array, 4096, &params );

//�������� �������� � NEXTION
//115200, 256 ���� ������ ���-���
void SendDrawNex (unsigned char* array, int ID_lcd, int Points)
{
  char CmdBuf[30];
 // sprintf( CmdBuf,"t1.txt=\"Engl\"���"); // 0xff
 // UARTSend2 ((BYTE*)CmdBuf, strlen (CmdBuf));// 
 // sprintf( CmdBuf,"t2.txt=\"����\"���"); // 0xff
//  UARTSend2 ((BYTE*)CmdBuf, strlen (CmdBuf));// 
  //  CreatDelay (30000); // 33 �� - ���� ��� ������ (�������������)
  StartRecievNEX (80);
  sprintf( CmdBuf,"addt %d,1,%d���",ID_lcd, Points+1); // 0xff �������� ������� �� ������� �������� ���� ��������� (��������� 3 ��� lcd
//  g_WtRdyNEX = 0;
//  ReadyNEX = 0;
//  ClearRS2();
//  GetRstTMUart2(1); // ������� ������ Uart2
//  UARTSend2 ((BYTE*)CmdBuf, strlen (CmdBuf));//
  NEX_Transmit((void*)CmdBuf);//
  while(!((g_WtRdyNEX)||(ReadyNEX==2)));
  // ���� ����� ��������� ������
  //array[390]=255; 
  //  CreatDelay (400000); // 300000-������� �� 3.5
  HAL_Delay(40);
  array[Points+1]=255;  
  array[Points+2]=255;  
  array[Points+3]=255; 
  StartRecievNEX (80);

//  g_WtRdyNEX = 0;
//  ReadyNEX = 0;
//  ClearRS2();
//  GetRstTMUart2(1); // ������� ������ Uart2
  // � ��� ���� �������� ������ ������ ��� ���� ��������
  //UARTSend2 ((BYTE*)array, Points+1);//
   HAL_UART_Transmit(&huart7,(void*)array,Points+1,(uint32_t)((Points+1)/8));  //
  // ����� ����� ��������� ������
  while(!((g_WtRdyNEX)||(ReadyNEX==1)));
  
   // CreatDelay (800000); // 70 �� - ���� ��� ������ (�������������)
  HAL_Delay(80);
}
