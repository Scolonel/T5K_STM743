#include "system.h"

//#define AUTO      1
//#define MANUAL    0

static BYTE Range = 0;                  // �������� ������ ��������� (����������� ��������)
static char SwitchRangeMode=AUTO;       // ����� ������������ ������������� ��������: �������������� ��� ������
static BYTE TypeRslt=0;                 // ������� ���������: ��� - 0, �� - 1, ��-2
static DWORD Watchdog;
volatile BYTE StateADC = FREEADC;
static int DataADC_PM=0x00;
//static BOOL Ready=TRUE;
static int FreeCodeADC_PM=0x00;       // ����������� �������� ����� ��� ��� �������� ���������


static int Res_Old=0;                 // ���������� ����������� �������� ��� � ������ ��������
static int Level[40];         // ����������� �������� ��� � ������ ��������
static unsigned int PMWavelenght=1310;  // ������� ����� �����

unsigned int GetPMWavelenght(signed char DeltaLambda) //����������, ���� �������� ������� ����� �����
{
  BYTE i=0;     // ������
  if(!DeltaLambda) return PMWavelenght;         // ���� DeltaLambda ����,  �� �������� ������� ����� �����
  
  if(DeltaLambda==13)                           // ���� DeltaLambda ����� 13, �� ������������ �� ������������ ������ ����
  {
    for(i=0;i<WAVE_LENGTHS_NUM;i++)
      if(PMWavelenght<CoeffPM.PointKalib[i])
      {
        PMWavelenght=CoeffPM.PointKalib[i];
        return CoeffPM.PointKalib[i];
      }
      
      PMWavelenght=CoeffPM.PointKalib[0];
      
    return  PMWavelenght;    
  };
  // ���� ��� ������� �����������
  if (WIDE_VER)  // ���� ��� ��������� ��� ������������ ���������
  {
  if((PMWavelenght+DeltaLambda)<800) 
  {
    PMWavelenght=1650;
    return  PMWavelenght;    
  }
  
  if((PMWavelenght+DeltaLambda)>1650) 
  {
    PMWavelenght=800;
    return  PMWavelenght;    
  }
  
  if(((PMWavelenght+DeltaLambda>900)&&(PMWavelenght+DeltaLambda<1210))&&(PMWavelenght+DeltaLambda<PMWavelenght))
  {
    PMWavelenght=900;
    return PMWavelenght;
  }

  if(((PMWavelenght+DeltaLambda>900)&&(PMWavelenght+DeltaLambda<1210))&&(PMWavelenght+DeltaLambda>PMWavelenght))
  {
    PMWavelenght=1210;
    return PMWavelenght;
  }
  }
  else // ����������� ��������
  {
      if((PMWavelenght+DeltaLambda)<800) 
  {
    PMWavelenght=1700;
    return  PMWavelenght;    
  }
  if((PMWavelenght+DeltaLambda)>1700) 
  {
    PMWavelenght=800;
    return  PMWavelenght;    
  }

  }
  
  PMWavelenght+=DeltaLambda; 
  
  return PMWavelenght;

  
}
// ��������� ������ ����� ���� ������� Wide � JDSU ���������
void SetAutoLamdaJDSU (DWORD Freq)
{
    if (!(WIDE_VER))  // ����  ����������� ��������
    {
      if (SetJDSU.CompMode)
      {
        PMWavelenght = GetLambdaJDSUFreq(Freq);
        GetCurrLvldB(PMWavelenght);
      }
    }
}
// ����� �������� ������ ����� �� �������
unsigned int GetLambdaJDSUFreq (unsigned int Freq)
{
  for (int i=0;i<6;++i)
  {
    if ((Freq > SetJDSU.FreqLambda[i]-20)&&(Freq < SetJDSU.FreqLambda[i]+20))
    {
      return SetJDSU.LambdaNum[i];
    }
  }
  return PMWavelenght;
}



unsigned int SetPMWavelenght (unsigned int CurrPMWavelenght) // �������������� ��������� ������� ����� �����
{
  if (WIDE_VER)  // ���� ��� ��������� ��� ������������ ���������
  {
  if ((CurrPMWavelenght<800)||(CurrPMWavelenght>1650)||((CurrPMWavelenght>900)&&(CurrPMWavelenght<1210))) CurrPMWavelenght = 1310;
  }
  else // ����������� ��������
  {
  if ((CurrPMWavelenght<800)||(CurrPMWavelenght>1700)) CurrPMWavelenght = 1310;
  }
  PMWavelenght = CurrPMWavelenght;
  return PMWavelenght;
}

// ������������� ����� ������������ ������������� �������� 
// ���������� �������� (������ ��� ��������������)
void SetSwitchPMMode(BYTE SwMode)       
{
  SwitchRangeMode=SwMode;
  return;
}

void MyDe_t (int lin) // ������������ ������������ ����������� ��������
{
  for (int i=0;i<lin;i++);
}


// ������������� ������ ������ �� ���
uint32_t ReadDataADC7782 (void)
{
  uint32_t Data=0;
  for (int i=0; i<24; i++)
  {
    PM_CLK(0);
    MyDe_t(2);
    if ((GET_PM_DATA)!=0) Data++;
    PM_CLK(1);  
    Data = Data<<1;
    MyDe_t(4);
  }
  return Data>>1;
}

// ������ ������ �� ��� ����������
// ������ ������ �� ��� ����������
// � �������� ����� ��������� ��������� ���
// ���� ���������� ������ (�� ����� ������  ������ ������) ��������� �� �����������!!!
// �� ������� ������ �������������  ���������� � ����������� ���������
int GetPMData(void)       
{
  
  
  switch (StateADC)
  {
  case FREEADC: 
    PM_CS(0);               // ��������� ���� ��������� ������ CS
    Watchdog=0; // �������� ������� �� ��������� (25 ��)
    StateADC = BUSYADC;
    break;
  case BUSYADC: // 
    Watchdog++; // ���� ���������� ���
    
    if (Watchdog > 10) // �� ��������� ����� 250 ��
    {
      // ������ ����� �� ������ ������ (������ ������ ������
      ReadDataADC7782 ();
      StateADC = FREEADC;
      PM_CS(1);
    }
    else
    {
      if (!(GET_PM_DATA)) // ������ ������ ����� ���������
      {
        DataADC_PM =  ReadDataADC7782 ();
        Watchdog=0; //
        StateADC = READYDATA;
      }
    }
    
    break;
  case READYDATA: // ������ ������ (����� ��������� ����������� ��)
    Watchdog=0; // 
    PM_CS(1);
    //CurrRange = Range;
    //PMCurrValue = GetPower(PMWavelenght);      // �������� �������� � ��� �� �������� �� ����� �����
    StateADC = FREEADC;
    break;
  }
  
  //FreeCodeADC_PM = 0x800000-DataADC_PM;
  //FreeCodeADC_PM = 0xFFFFFF-DataADC_PM; // ��� ��������� ��������
//  if(DeviceConfig.CfgRE>>1) // ���� ����� ����� ���������� 03.03.2023
//    FreeCodeADC_PM = 0x800000 - DataADC_PM;
//  else // ������ ���������� �����
    FreeCodeADC_PM = DataADC_PM-0x800000;
  return  FreeCodeADC_PM;                           // 
}
void SetStateADC (BYTE State) // ��������� ������ ���
{
 StateADC = State; 
}

BYTE GetStateADC (void) // ��������� ������ ���
{
 return StateADC; 
}

int SetDataADC (int Data) // ��������� ������ ��� � ����������
{
  PM_CS(1);
  DataADC_PM = Data;
  Watchdog=0; // �������� ������� ����������� ������� (���� ����� 30 ��)
  StateADC = READYDATA;
  return DataADC_PM;
  
}

float GetPower(unsigned int Lambda)      // �������� �������� � ��� � ����������� �� ����� �����
{
//  
//  char M_PON;
//  float clck_K;
//  clkP++;
  
  int i;                          // �������
  int Res_Now;                    // ������� �������� ��� � ������ ��������
  float Lev_x;                    // ����������� �������� ���: ������� �������� Level[0..39]
  static float Lev_W=1e-9;
  
  
  
  Res_Now = GetPMData() - CoeffPM.ShZeroRng[Range];        // �������� ��������� � ������ ��������
  
  if (StateADC == READYDATA)                               // ���� ������ ������
  {
    if (abs(Res_Old - Res_Now)>10000)                      // ����������� ������ ��� ������ ��������� ���������
    {
      for (i=0; i<=39; i++)
        Level[i]=Res_Now;    
    }
    
    for (i=39; i>=1; i--)
    Level[i]=Level[i-1];
    Level[0] = Res_Now; 
    
    
    Lev_x = 0;                                            // ������� ������� �������� ��������� ���
    for (i=0; i<40; i++)Lev_x +=Level[i];
    Lev_x = Lev_x/40.0;
                             // ������ ����� �������, ������� ��������� ����� �������������
    Res_Old = (int)Lev_x;                                      // ��������� ��������, ��� "������"
    if (Lev_x<=0)     Lev_x = 1;                          // ���� ����� �������� ������ ��� ����� ����
    Lev_W = ( 7.9261495712e-7 * Lev_x  * CoeffPM.CoefStykRange[Range] * GetCoeffSpctr(Lambda)) / pow( 100, 3-Range ); // ���������� ������ �������� � ���������� 
    // ������������ ���� ����� ����� 800-900 � ������� ��������, � ����� ����� �� ����� 0.0)
    if(( Lambda>=800)&&( Lambda<900)&&(ConfigDevice.CfgPM==1)&&(NameDB.ph_A[0]!=0.0))
      Lev_W = pow(Lev_W, 1.0+NameDB.ph_A[0]);
    if (SwitchRangeMode == AUTO)                          // �������������� ������������ ����������
    {
      // ������� ����������� ������������
      // ����� ������� � 270 �� ���� ������� ������� ������������ �� 6000000 = 5B8000
      if ((Lev_x>=0x5B8000) && (Range<3))                     //����� ������������ �� �� ����� ������ ���������, (����� 8000000 � ������� ��������)
      {
        Range++;                                          //������������� �� ����� �������������� ��������
        SetRange(Range);
        //CreatDelay (5000000);
        
      }
      if (Lev_x < 40000 && Range>0)                       //����� �������� ������� ����� �������� �� �� ����� �������������� �������,
      {
        Range--;                                          //������������� �� ����� �������������� ��������
        SetRange(Range);
        //CreatDelay (5000000);
      }
    }
    
    
    
    
    
//    switch(GetCfgPM())
//    {
//    case 1:
//      if (Lev_W<1e-9)Lev_W = 1e-9;                          //����������� ��������� �� ������ 1pW 
//      break;
//    case 2:
//      if (Lev_W<1e-6)Lev_W = 1e-6;                          //����������� ��������� �� ������ 1pW 
//      break;
//    }
    StateADC = FREEADC;
  }
  return Lev_W;

}

void SetRange(unsigned char Rn)                            // ��������� ������ �������� ���������� ���������
{
   if (Rn>3) Rn = 0;
   Range=Rn;
  switch(Rn)
  {
  case 0:
    KEY1(0);    KEY2(1);    KEY3(0);
    break;
  case 1:
    KEY1(1);    KEY2(1);    KEY3(0);
    break;
  case 2:
    KEY1(1);    KEY2(1);    KEY3(1);
    break;
  case 3:
    KEY1(1);    KEY2(0);    KEY3(0);
    break;
  default:
    KEY1(0);    KEY2(1);    KEY3(0);
    break;
  }

  return;
}

BYTE GetRange(void)                                        // ���������� ������� �������� ������ �������������� ���������
{
  return Range;
}


// ��������� ������������� ������������ � ����������� �� ����� �����
// ��������� 800-900, 1210-1390, 1410-1650 

float GetCoeffSpctr(unsigned int Lambda)
{
  float k_c =1.0;    // ������������ �����������
  unsigned int i;    // ������
  double a,b;        // ������� �� ������� ����������� �����.  a*x+b=y
  
    if (WIDE_VER)  // ���� ��� ��������� ��� ������������ ���������
  {

  if((Lambda<800) || ((Lambda>900)&&(Lambda<1210)) || (Lambda>1650)) // ���� ������� �������� ����� �����    
    return k_c;
  
  if(Lambda<=900)                      // ������ �������� L-�������� 800...900��
  {
    i=(int)((Lambda-800)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrL[i+1]-CoeffPM.CoefSpctrL[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrL[i]-a*(800+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(850);
    
    return k_c;
  }
  
  if(Lambda<=1390)                     // ������ �������� M-�������� 1210...1390��
  {
    i=(int)((Lambda-1210)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrM[i+1]-CoeffPM.CoefSpctrM[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrM[i]-a*(1210+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(1310);
    
    return k_c;
  }

  if(Lambda<1410)                       // "����" 1391...1409 ��
  {
    a = (CoeffPM.CoefSpctrH[0]-CoeffPM.CoefSpctrM[9])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrM[9]-a*1390;                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(1310);

    return k_c;
  }  

  if(Lambda<=1650)                     // ������ �������� H-�������� 1410...1650��
  {
    i=(int)((Lambda-1410)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrH[i+1]-CoeffPM.CoefSpctrH[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrH[i]-a*(1410+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(1550);
    
    return k_c;
  }
  }
else // ����������� ��������
{
    if((Lambda<800) || (Lambda>1700)) // ���� ������� �������� ����� �����    
    return k_c;
   // 800 ... 900 
  if(Lambda<=900)                      // ������ �������� L-�������� 800...900��
  {
    i=(int)((Lambda-800)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrL[i+1]-CoeffPM.CoefSpctrL[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrL[i]-a*(800+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(850);
    
    return k_c;
  }
  //�� ������������ �������� ��������� �� ����� 900 - 1210 ����� �����������
  // 900... 960
  if(Lambda<=960)                     // 
  {
    i=(int)(Lambda-900);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = ((CoeffPM.CoefSpctrL[5]/1.8) - CoeffPM.CoefSpctrL[5])/60;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrL[5];                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*i+b;
    
    return k_c;
    
  }
  // 960... 1000
  
  if(Lambda<=1000)                     // �� ������������ �������� ��������� �� ����� 960 - 1000 ������ �������������
  {
    
    k_c = CoeffPM.CoefSpctrL[5]/1.8;
    
    return k_c;
    
  }
  // 1000... 1210
  
  if(Lambda<=1210)                     // �� ������������ �������� ��������� �� ����� 900 - 1210 ������ �������������
  {
    i=(int)(Lambda-1000);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrM[0]-(CoeffPM.CoefSpctrL[5]/1.8))/210;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrL[5]/1.8;                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*i+b;
    
    return k_c;
    
  }
  // 1210.... 1390
  if(Lambda<=1390)                     // ������ �������� M-�������� 1210...1390��
  {
    i=(int)((Lambda-1210)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrM[i+1]-CoeffPM.CoefSpctrM[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrM[i]-a*(1210+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(1310);
    
    return k_c;
  }
  // 1390... 1410
  if(Lambda<1410)                       // "����" 1391...1409 ��
  {
    a = (CoeffPM.CoefSpctrH[0]-CoeffPM.CoefSpctrM[9])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrM[9]-a*1390;                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(1310);

    return k_c;
  }  
    // 1410... 1650
  if(Lambda<=1650)                     // ������ �������� H-�������� 1410...1650��
  {
    i=(int)((Lambda-1410)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrH[i+1]-CoeffPM.CoefSpctrH[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrH[i]-a*(1410+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*GetCoefPointKlb(1550);
    
    return k_c;
  }
  // 1650... 1700
  if(Lambda<=1700)                     // �� ������������ �������� ��������� �� ����� 1650 - 1700(1650*1.1) ������ �������������
  {
    i=(int)(Lambda-1650);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = ((CoeffPM.CoefSpctrH[12]*1.1) - CoeffPM.CoefSpctrH[12])/50;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrH[12];                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*i+b;
    
    return k_c;
    
  }
}
  

  return k_c;
} 



float GetCoefPointKlb(unsigned int Lambda)                                  // ���������� �������������� ����������� ��� ������������� ���� ����
{
  BYTE i=0;                 // ������ 
  
  for(i=0;i<WAVE_LENGTHS_NUM;i++)   
    if(Lambda==CoeffPM.PointKalib[i]) return CoeffPM.CoefPointKlb[i];     // ���� ����� ����������, �� ���������� ������������ �����������
  
  return 1;                                                               // ���� Lambda 
  
}

float Watt2dB(char *str, float mWatt,BYTE TypeRslt)           // ����������� ��� � ���� (���), �� ��� ���
{
    float Data;
    
    switch(GetCfgPM())
    {
    case 1:
      if (mWatt<1e-9)mWatt = 5e-10;                          //����������� ��������� �� ������ 1pW ������� ��������
      break;
    case 2:
    if (WIDE_VER)  // ���� ��� ��������� ��� ������������ ���������
      if (mWatt<1e-6)mWatt = 5e-7;                          //����������� ��������� �� ������ 1nW  �����
    else 
      if (mWatt<1e-8)mWatt = 5e-9;                          //����������� ��������� �� ������ 1nW  �����
      
      break;
    }
    
  switch(TypeRslt)
  {
  case 0:// ���
    Data = 10*log10(mWatt);
    sprintf(str,"% 7.3f",Data);
    break;   
  case 1:// ��
    Data = 10*log10(mWatt)-GetCurrLvldB(0);
    sprintf(str,"% 7.3f",Data);
    break;
  case 2:// ��
    Data = mWatt;
    sprintf(str,"% 6.2f",Data);
    break;
  }
  return Data;
}

int GetTypeRslt()                                    // ���������� ������� �������� ���� ��. ��������
{
  return TypeRslt;
}

int SetTypeRslt(BYTE type)                           // ������������� ����� �������� ���� ��.���������
{
  if(type>2) type=0;
  TypeRslt=type;
  return TypeRslt;
}

int AcquireShZeroLowRng(void)                          // ��������� ������� �������� �� ������ ��������� (������������� ����)
{
  BYTE i=0;                                     // ��������
 // static unsigned long data;                             // ��� ����������� ������
  
  
  int AverageData=0;                              // ���������� ������
  
    if (GetRange()==0)
      // ���������� �������� 0  ?
    {
    CreatDelay(1.2e6);    //~ 0.1 S
    for(i=0;i<3;i++)
    {    
      if(StateADC==FREEADC) GetPMData();
      MyDe_t(1); // ����� ��������
      while(StateADC!=READYDATA) 
      {
      MyDe_t(1); // ����� ��������
      }     

      SetStateADC(FREEADC);
    }     
    //AverageData=0;
    
    
    
    for(i=0;i<20;i++)
    {    
      if(StateADC==FREEADC) GetPMData();
      MyDe_t(1); // ����� ��������
      while(StateADC!=READYDATA) 
      {
      MyDe_t(1); // ����� ��������
      }
      
      AverageData+=GetPMData();
      SetStateADC(FREEADC);
    }
    AverageData = (AverageData/20);
    if ( AverageData < 300000)
    CoeffPM.ShZeroRng[0]=(int)(AverageData);
    
   
  CreatDelay(1e7);  // ~ 0.8 S
    }
  return 1;
}

int AcquireShZeroRng(void)                          // ��������� ������� ��������
{
  BYTE i=0, j=0;                                    // ��������
 // static unsigned long data;                             // ��� ����������� ������
  
  
  float AverageData=0;                              // ���������� ������
  char Str[20];
  
  //ClearScreen();
  
  for(j=0;j<4;j++)
  {
    SetRange(j);                      // ���������� �������� 0  
    CreatDelay(1.2e6);    //~ 0.1 S
    for(i=0;i<3;i++)
    {    
      if(StateADC==FREEADC) GetPMData();
      
      while(StateADC!=READYDATA) 
      {
                        
      }     
    CreatDelay(2e5);    //~ 0.02 S

      SetStateADC(FREEADC);
    }     
    sprintf(Str,"t%d.txt=\"�������...\"���",2*j+2); //
    //UARTSend2((BYTE*)Str, strlen(Str));    //
    NEX_Transmit((void*)Str);// 

    AverageData=0;
    
    
    
    for(i=0;i<20;i++)
    {    
      if(StateADC==FREEADC) GetPMData();
      
      while(StateADC!=READYDATA) 
      {
        
        CreatDelay(2e5);    //~ 0.02 S
      }
      
      AverageData+=GetPMData();
      SetStateADC(FREEADC);
    } 
    
    CoeffPM.ShZeroRng[j]=(int)(AverageData/20);
    
    sprintf(Str,"t%d.txt=\"%d\"���",2*j+1,CoeffPM.ShZeroRng[j]); //
    //UARTSend2((BYTE*)Str, strlen(Str));    //
      NEX_Transmit((void*)Str);// 

  }
  
  
  CreatDelay(1e7);  // ~ 0.8 S
  return 1;
}


int AcquireCoefStykRange(BYTE Rng, float* PrevRng, float* CurrRng)         // ��������� ����������� ����������� �������� ��������� � ����������
{
  // ���������� ������ � ������ UART - ��������� ����������, ������ �� ����� �� �����
  //float PrevRng=0;                         // �������� ����� ��� �� ���������� ���������
  //float CurrRng=0;                         // �������� ����� ��� �� ������� ���������
  BYTE i;                                    // �������
  //char Str[20];                              // ������ ��� ������ �� �����
  
  if((Rng<1)||(Rng>3)) return 0;             // ���� ������ �������� ��������
  
  //ClearScreen();
  SetRange(Rng-1);                            // ���������� ���������� ��������
  
  //sprintf(Str,"����. �����.");       
  //putString(0,0,Str,1,1);

  //sprintf(Str,"�������� %d", Rng-1); 
  //putString(2,18,Str,1,0);
  //sprintf(Str,"�������..."); 
  //putString(82,18,Str,1,0);
  
  //PaintLCD();
  HAL_Delay(100);
  //CreatDelay(1.2e6);  // ~ 0.1 S

  // ������ ������
  for(i=0;i<3;i++)
  {    
    if(StateADC==FREEADC) GetPMData();
    while(StateADC!=READYDATA);
    SetStateADC(FREEADC);
  }     

  // ��������   
  for(i=0;i<20;i++)
  {    
    if(StateADC==FREEADC) GetPMData();
    
    while(StateADC!=READYDATA) 
    {
      //sprintf(Str,"%d",0x800000-DataADC_PM); // nm
      //putString(82,54,Str,1,0);
      //PaintLCD();  
      HAL_Delay(10);
    }
    
    *PrevRng+=GetPMData();
    SetStateADC(FREEADC);
  } 
      MyDe_t(1); // ����� ��������
  *PrevRng=*PrevRng/20.0;
  *PrevRng= *PrevRng - (float)CoeffPM.ShZeroRng[Rng];
  
  SetRange(Rng);
  //sprintf(Str,"%.0f",*PrevRng); 
  //putString(82,18,Str,1,0);
 
  //sprintf(Str,"�������� %d",Rng); // �����
  //putString(2,30,Str,1,0);
  
  //sprintf(Str,"�������...");
  //putString(82,30,Str,1,0);
  HAL_Delay(100);

  //PaintLCD();   
  // ������ ������
  for(i=0;i<3;i++)
  {    
    if(StateADC==FREEADC) GetPMData();
    while(StateADC!=READYDATA);
    SetStateADC(FREEADC);
  }     
  
  // ��������
  for(i=0;i<20;i++)
  {    
    if(StateADC==FREEADC) GetPMData();
    
    while(StateADC!=READYDATA) 
    {
      //sprintf(Str,"%d",0x800000-DataADC_PM); // nm
      //putString(82,54,Str,1,0);
      //PaintLCD(); 
      HAL_Delay(10);
    }
    
    *CurrRng+=GetPMData();
    SetStateADC(FREEADC);
  } 
  
      MyDe_t(1); // ����� ��������
  
  *CurrRng=*CurrRng/20.0;
  *CurrRng =*CurrRng - (float)CoeffPM.ShZeroRng[Rng];
  CoeffPM.CoefStykRange[0] = 1.0;
  CoeffPM.CoefStykRange[Rng]=CoeffPM.CoefStykRange[Rng-1]*(*PrevRng)/(*CurrRng*100);

  //sprintf(Str,"%.0f",*PrevRng);
  //putString(82,30,Str,1,0);
  
  //sprintf(Str,"�����������"); // �����
  //putString(2,42,Str,1,0);
  //sprintf(Str,"%.2f",CoeffPM.CoefStykRange[Rng]); // nm
  //putString(82,42,Str,1,0);
      HAL_Delay(100);
  
  
  return 1;
}

float GetCoefSpctr(WORD Lambda, float RealPower)     // ���������� ������������ ����������� ��� ������� ����� �����
{
  BYTE i=0;                             // �������
  float AcqPower=1e-9;                  // ���������� ��������
  char Str[64];                         // ������ ��� ������
  float Coef=1.0;                       // ������������ �����������
  
  
  if((Lambda<800) || ((Lambda>900)&&(Lambda<1210)) || (Lambda>1650)) // ���� ������� �������� ����� �����    
    return 1.0;
  
  if(Lambda<=900)                      // ������ �������� L-�������� 800...900��
  {
    i=(int)((Lambda-800)/20);          // ��� ����� ������������� ������������� - 20 ��  
    CoeffPM.CoefSpctrL[i]=1.0;         // �������, ��� ����������� ����� 1
  }
  else if(Lambda<=1390)                // ������ �������� M-�������� 1210...1390��
  {
    i=(int)((Lambda-1210)/20);         // ��� ����� ������������� ������������� - 20 ��  
    CoeffPM.CoefSpctrM[i]=1.0;         // �������, ��� ����������� ����� 1
  }
  else                                 // ������ �������� H-�������� 1410...1650��
  {
    i=(int)((Lambda-1410)/20);         // ��� ����� ������������� ������������� - 20 ��  
    CoeffPM.CoefSpctrH[i]=1.0;         // �������, ��� ����������� ����� 1
  }
  
  // �������� ���� ��������� ��������� ��� �������� ������  
  //CreatDelay(1e6);   // ~ 
  
  //CurStateADC=GetStateADC();
  if (GetStateADC()==BUSYADC)
    while(GetStateADC()!=BUSYADC);
  // ����, ����� ��� �����������
  
  //if(CurStateADC==READYDATA) GetPower(Lambda);    // �������, ��� ��� ������ � ����� ����������, ������ ������
  
  SetSwitchPMMode(MANUAL);                // ����������� ���������� � ������ ����� , ����� ����� �� ������������� 
  // ���������� �.�. 30.10.2015
  
  
  for(i=0;i<60;i++) // 40*30 �� = 1.8���
  {
    AcqPower=GetPower(Lambda);          // ������ ������
    // ������� ����� ���� � ����������
    // �� ����������� ��������� ��������� ������������ ����������
    sprintf(Str,"t1.txt=\"%2.2e\"���",AcqPower); // � ���� ������ dBm REF
    //UARTSend2((BYTE*)Str, strlen(Str));    //
      NEX_Transmit((void*)Str);// 
    
    sprintf(Str,"t3.txt=\"%d  %d\"���",i,GetRange() ); // � ����  �������� �������� nm
    //UARTSend2((BYTE*)Str, strlen(Str));    //
      NEX_Transmit((void*)Str);// 
    
    if (GetStateADC()==BUSYADC)
      while(GetStateADC()!=BUSYADC);
    // ����, ����� ��� �����������
    
    CreatDelay(3e5);   // ~ 
    
  }
  
  Coef=RealPower/AcqPower;              // ������������ �����������
  sprintf(Str,"t5.txt=\"%.4f\"���",Coef);
      NEX_Transmit((void*)Str);// 
  SetSwitchPMMode(AUTO);                // ����������� ���������� � �������������� �����
  CreatDelay(3e5);   // ~ 
  
  return Coef;
}

float GetCoefSpctrKlb(WORD index, float RealPower)     // ���������� ������������ ����������� ��� ������������� ���� ����
{
  BYTE i=0;                             // �������
  float AcqPower=1e-9;                  // ���������� ��������
  //char Str[20];                         // ������ ��� ������
  CoeffPM.CoefPointKlb[index]=1.0;
  if(index>3) return 1.0;
  
  if (GetStateADC()==BUSYADC)
  while(GetStateADC()!=BUSYADC);
               // ����, ����� ��� �����������
 
  //if(CurStateADC==READYDATA) GetPower(Lambda);    // �������, ��� ��� ������ � ����� ����������, ������ ������
  
  SetSwitchPMMode(MANUAL);                // ����������� ���������� � ������ ����� , ����� ����� �� ������������� 
  // ���������� �.�. 30.10.2015
  
    
  for(i=0;i<60;i++) // 40*30 �� = 1.8���
  {
    AcqPower=GetPower(CoeffPM.PointKalib[index]);          // ������ ������
      CreatDelay(3e5);   // ~ 
      
  }
  SetSwitchPMMode(AUTO);                // ����������� ���������� � �������������� �����
    
  
  CoeffPM.CoefPointKlb[index]=RealPower/AcqPower;              // ������������ �����������
  
  return CoeffPM.CoefPointKlb[index];

  
}
