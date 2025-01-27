//#define LANG_NUM 3
#define ENADRAWORL 1
//#define ENAOLDLCD 1 //1 - ���������� ���������� �� ������ LCD, 0 - ��� ������� ������


#define NO_PARENT 0xFFFFFFFF
#define MAXWIDESMBL 17 // ������������ ����� ������� �������� � ������ 
#define MAXSMALLSMBL 22
#define WAITWELCOME 100
#define TIMEDELAYINPUT 17000 // �������� ��� ��������� �������� ������� (�������� �������-������� �����)

#define PNTSNUM 7 // ����� ����� �������� ����� �����
#define PNTVERTICALSIZE 4
#define b_MENU 0
#define b_OK 1
#define b_RIGHT 2
#define b_LEFT 3
#define b_DOWN 4
#define b_UP 5

#define PLUS 1
#define MINUS 0

#define VIEWER 0
#define SETSCALE 1

#define WAITING 0
#define MEASUR 1

#define INCR 1
#define DECR -1
// ��������� ��������� ������������� � ������
#define VIEWSAVED 7  // ������ �������� 
#define SAVEDTRACE 1 // ��������� � ����������
#define MEMFULL 2 // ��������� � ������������� ����������
#define VIEWNEXT 3 // �������� ��������� ��������������
// �������� �������� �� ��������� �������������� � ��������� ��� � ������������ 
#define SETPARAM 0
#define VIEWMEM 1
#define VIEWEVNT 2

#define BEGZONEPNT 12 //����� ����� ����� ������� ������� ������ (��� ����� AR ����� ������ ������ � ��� �������, ������� ������ �������)30.04.2021
#define BEGZONEAVRG 10 //����� ���������� ��� �������� �������� �������


#define DELTRACE 4 // ������ ������������� �������� ��������������
#define DELETEDTRACE 5

// #define NumFiltrs 4

#define Pic_Tx 40 // X - �������� �������� ��� ������ ��������� �������
#define Pic_Ty 2 // Y - �������� �������� ��� ������ ��������� �������

#include "system.h"
// ���� ��������
//static const  unsigned char LOGOS[] = {
//  0xaa, 0xaa, 0xaa, 0xaa, 
//  0x01, 0x00, 0x00, 0x80, 
//  0x02, 0x00, 0x00, 0x40, 
//  0x01, 0xaa, 0x55, 0x80, 
//  0x02, 0xaa, 0x55, 0x40, 
//  0x01, 0x00, 0x00, 0x80, 
//  0x02, 0x00, 0x00, 0x40, 
//  0xaa, 0xaa, 0xaa, 0xaa 
//};

// ������ �����
const unsigned char BegLine [40] = {
 #include "BegB.pic"
}
;
// ���������� �������
const unsigned char EvntRefl [40] = {
 #include "ReflEvnt.pic"
}
;
// ������� ��������� - "����������" � ����� �����
const unsigned char BigEnd [40] = {
 #include "BigOvrEnd.pic"
}
;
// ��� ������������� ����� �����
const unsigned char NoRefEnd [40] = {
 #include "NoReflEnd.pic"
}
;
// ��� �������������� �������
const unsigned char NoRefEvn [40] = {
 #include "NoReflEvnt.pic"
}
;
//����� � ���� (OutOfRange)
const unsigned char OutOfNoise [40] = {
 #include "OutNoise.pic"
}
;




const int KeyPoints[PNTSNUM] = { 96, 172, 344, 688, 1366, 2048, 4608 }; // ����� ����������� ������� ������������� ����� 4096
 // ���� ���� ���� ����� �� ���������� ����� � �� �� ������� �������, ��� ��� ������������� �� �������� �����
//const int KeyPoints[PNTSNUM] = { 96, 192, 384, 768, 1536, 2304, 4608 }; // ����� ����������� ������� ������������� ����� ��� ����� �������
//const int VerticalSize[PNTVERTICALSIZE] = { 22000, 16000, 8000, 4000 }; // ������������ ��������  ��� ���������
// ����� �������� � uS � ����������� �� ����� � ������������
const int TimeRepitOfLN[PNTSNUM] = { 105, 120, 200, 350, 450, 750, 1500 }; // ����� ����������� ������� ������������� ����� 4096

const DWORD DelayBadLength [LENGTH_LINE_NUM-1][LENGTH_LINE_NUM]= //����� �������������,����� ����������
{{0,1250,4600,11000,18000,12000,26000},
{0,0,3300,9700,16800,10700,24600},
{0,0,0,6400,13500,7400,21350},
{0,0,0,0,7100,950,14900},
{0,0,0,0,0,0,7700},
{0,0,0,0,0,0,13900}};

struct tag_PON PONI;


char CommentsOTDR[20] = {"                   \0"}; //����������� �������������
char FileNameOTDR[20] = {"                   \0"}; //����������� �������������
char PrefixFileNm[11] = {"Fiber     \0"}; //����������� �������������
char IndexPrefix = 0;
static unsigned int NumFiber = 1; // ���� ������� ����� ���
char IndexCommOTDR = 0;
char CommentsOLT[16] = {"               \0"}; //����������� ����������
char CommentsOLTOld[16] = {"               \0"}; //����������� ����������
char IndexCommOLT = 0;
char VerFW_LCD[25] = {"No version LCD          \0"}; //������ �� ���������� NEXION
char txtout[64];
static void (*ModeFunc)(void);

static unsigned CntWelcome = 0;
static BYTE PosCurr = 6; // ��������� ������� ��������������� ������� � ���������� �����
static BYTE SubModeMeasOTDR = NOTMODE; // ��������� ��������� �������������
static DWORD CurrentSumDelay; // �������� ��������� �������� ��� ��������� ����� ����� ��� ������ ����������
static unsigned NumAvrg; // ����� ���������� (����� �� �������� �� 3 ���)
static unsigned FinAvrg; // ����� ���������� ����� �� �������
static unsigned CntNumAvrg; // ������� ����� ���������� ()
static unsigned ValueDS; // �������� DS ���������� ������ ����� (��� ������� �������� ����� � ������ � ������)
static char KbPosX=11; // ������� � � ����������
static char KbPosY=0; // ������� Y � ����������
static BYTE IndexVerSize = 0;
volatile BYTE IndexSmall = 0; // ������������� ��� 2 �� ,0 - 0.5 ��, 1 - 2�� 
static BYTE ModeMemDraw = VIEWSAVED;
static BYTE ModeDevice = MODEMENU;
static BYTE ModePowerMeter = NOTMODE;
static BYTE ReturnModeViewRefl = SETPARAM;
// ���������� ���������� ������������� ������� ���
static BYTE EnaCorrMZ = 0;
int CntPointPick;
unsigned short PointsPick[16];
unsigned short PointsPickIndx[16];

// ���������� ������ ��������� ��������������
  static char ViewMode = VIEWER;//SETCSALE
  static BYTE TypeCentral = MEDIUM;
// ���������� ��������������� ����������
static float RPON[5]; // �������� ��� ������ � ������ PON 0-850,1-1310,2-1490,3-1550,4-1625 
static unsigned short LWPON[5]; //����� ���� �������� ��� �� �� ������������� � �������������
static unsigned short LWCNT[5]; //������� ������� �� �������� ���������� �����
static BYTE ModAutoPM = WAITING; // ����� ���������� ���� �������
static float LastPower; // �������� ��� ������ � ������ manual
//static BYTE Pow_Yes; //������� ������� ����� ����� ����� �������� ������ ��������� �����
static BYTE IndWavePON; //������ ����� ����� � ������� ������������� ����
static WORD TimerAutoPM=0; // ������� �������
//static BYTE NeedSaveCell = 0;//������� ���������� ��������� ����������
static BYTE FrClearMEM = 1; // ��������� �� ������
static WORD NumCellIzm ;// ������� �������� ������ ��� ���������
static WORD ProcBat , ProcBatInd; // ������� ��������
static int ADCData; // ���� ��� ��������
static float Ubat; // ���������� ������� � �������
BYTE NeedDrawCRC=0;
static volatile DWORD MeagNoise;
// ������ ����������
static DWORD ErrOLT;
// ��� �������
static unsigned short NumCalkEvents; // ����� ����������� �������
static unsigned short IndexEvents; // ��������� �� ������� 
static unsigned short IndexEventsOld; // ��������� �� ������� 
    // ������ �������� ������ � ������ Y=0..210
static unsigned CurrRed;
static unsigned CurrBlue;
static BYTE MonitorEna; // ������� ������������� �� ���������� ��������� ������ ������� �������
static BYTE CntLS=0; // ������� ������� ����������� ������� (����� 1 ��� ��� ��� ��� �� �� ���� ������� ��� 1 ��� �������)
// 17.11.2022 � ����� ��������, ��� ������� ��������� ��� ���� ����������
// ������� �������������� ���������� (��� �������� � ����� ���������, ������������� ��� ������ ������� ���������� ����� ����������)
 unsigned long TimeMeasure3S;
 BYTE RemoutCtrl=0; // ������� ���� ����������
 float CursorScale =1.0;
//���������� ��������� ��������� ORL 
float g_VolORL = 0.0 ;
float g_UpGrdB = 10.0; // ���������� �� ��� ����???
float g_kLog = 10.0; // ����������� ���������
float g_OffSetdB = 75.0; // �������� � ������� dB
BYTE g_mem_param = 0; // ��� ������� ��� ��������� ������������� � ������ , ��������� ��������� ��� ���������� �������� �����(����� ����� orl)
//static BYTE CompJDSU = 0;
volatile BYTE g_GetStr=0; // ������� �������� ��������� ������ �� ���������. 
volatile BYTE g_NeedScr=1; // ����������� ������ ! ���������! ���� ��������� 
volatile BYTE g_FirstScr=1; // ������ ���� � �����, ����� ������ ����������� (����������), ����� �������� ������ ������ ����, ��� NEXTION
volatile BYTE NeedKeyB = 0; // ������������� ������������ � ����������
volatile BYTE TypeLCD = 0; // ��� ���������� � �������������� v-3.2(=0) s-3.5(=1) 
volatile BYTE g_NeedChkAnsvNEX=0; // ������� ��������� ������ �� ���������.� �� ��������
volatile BYTE g_AutoSave=0; // ������� ������ ���� ���������� ��������� �� �������� ������ ����
volatile int NeedReturn = 0; // ������������� ��������� � ���� ����������
static BYTE ReturnMemView = 0; // ������� ���� ������������ �� Esc �� ��������� ������, 1- � ������, ��� � �������������
// 0 - �� ������ ��������� 



      // ������������� ������ � ����������� �� ����������
void SetPlaceNew(int Mode)
      {  
        if(Mode)
        g_AutoSave = 0; // ����� ���� ������
          
          
      switch (GetCombLS(SettingRefl.SetModeLW))
      {
      case 1:
        SettingRefl.SW_LW = 0; // ��������� ������� ���������� �������� ������
          GetSetModeLW(-1); // ���������� ���� �����
        break;
      case 2:
        SettingRefl.SW_LW = 1; // ��������� ������� ���������� �������� ������
          GetSetModeLW(-1); // ���������� ���� �����
        break;
      case 3: // ����������
        g_AutoSave = 1; // ��. ���� ��������������� ��������
        SettingRefl.SW_LW = 0; // ��������� ������� ���������� �������� ������
          GetSetModeLW(1); // ������������� ��������� �����, ����������� ����
        break;
      case 4:
        SettingRefl.SW_LW = 2; // ��������� ������� ���������� �������� ������
          GetSetModeLW(-1); // ���������� ���� �����
        break;
      case 5:
        g_AutoSave = 1; // ��. ���� ��������������� ��������
        SettingRefl.SW_LW = 0; // ��������� ������� ���������� �������� ������
          GetSetModeLW(1); // ������������� ��������� �����, ����������� ����
        break;
      case 6:
        g_AutoSave = 1; // ��. ���� ��������������� ��������
        SettingRefl.SW_LW = 1; // ��������� ������� ���������� �������� ������
          GetSetModeLW(1); // ������������� ��������� �����, ����������� ����
        break;
      case 7:
        g_AutoSave = 1; // ��. ���� ��������������� ��������
        SettingRefl.SW_LW = 0; // ��������� ������� ���������� �������� ������
          GetSetModeLW(1); // ������������� ��������� �����, ����������� ����
        break;
      default: // ��������� ����� ����
        g_AutoSave = 0; // ����� ����� ��������� ����� ���� ����������/
        SettingRefl.SW_LW = 0; // ��������� ������� ���������� �������� ������
          GetSetModeLW(-1); // ���������� ���� �����
        break;
      }
      SettingRefl.K_pr = GetWAV_SC(SettingRefl.SW_LW); // � ������� ����������� �� ����� ������
      GetPlaceLS(CURRENT); // ������������� ������ ������� �����
    }

// �������� ������ ���� ������ (�����) 04.04.2024
void AlarmSignal(int type)
{
   HAL_Delay(2);
 for (int i=0; i<type; i++)
 {
   LED_START(1);//On  LED
   myBeep(10);
   HAL_Delay(250);
   LED_START(0);//Off  LED
   myBeep(0);
   HAL_Delay(250);
 }
}


//void SlowON (void) // ��������� ��������� �������
//{
//  POWREF (ON);
//  CreatDelay (10000000); // 0.7 � (� ���� ��������� ����� �������� �� USB, ��� ���)
//  POWDET(ON);
//  CreatDelay (4000000); // 0.3 �
//  POWALT(ON);
//  CreatDelay (4000000); // 0.3 ��
//}


BYTE SetGetMonEna (BYTE Data) // ������/��������� �������� ������ ������� �� ���������� ���������
{
  if(Data != 255)
  {
    MonitorEna = Data;
  }
  return MonitorEna;
}

WORD SetNumCellIzm (WORD Data)
{
  if (Data > GetCellMem(0)-1)NumCellIzm=0;
  else NumCellIzm = Data;
  return NumCellIzm;
}

BYTE GetModeDevice (void)// ��������� ������ �������
{
  return ModeDevice;
}

unsigned GetCntNumAvrg ( void)// ��������� �������� �������� ����������
{
  return CntNumAvrg;
}
unsigned GetFinAvrg ( void)// ��������� ��������� �������� ���������� (������������ �������� ����������)
{
  return FinAvrg;
}

unsigned SetCntNumAvrg ( unsigned Data)// ��������� �������� �������� ����������
{
  return (CntNumAvrg = Data);
}



void ModeFuncTmp(void)
{
  ModeFunc();
  if(KeyP)
    KeyP = 0;
}

void SetMode( void(f)(void) )
{
  ModeFunc = f;
}

void ModeWelcome(void)// ����� ��������
{
  char Str[32];
  char StrN[32];
  char St[5];
  CntWelcome++;
  static DWORD SecOld=100;
  //static DWORD BatOld=150;
  current_time = RTCGetTime();
  CurrLang=GetLang(CURRENT);
  CheckLevelBattery ();
  // ������� ���� ��� ���������� ���� LCD �� ��������
  // ����������� ���� ��� NEXTION
  if(g_FirstScr)
  {
    
    // ������������� �� ���������� �������
    // ������������ �������
    GetDeviceLabel( Str, NameDB.Ena_DB  );
    sprintf (StrN,"t0.txt=\"%s\"���",Str); // 
    NEX_Transmit((void*)(void*)StrN);//
    // �������������
    if (GetID_Dev())
    {
      sprintf (Str,"t1.txt=\"%s\"���",MsgMass[12][CurrLang]); // ����
    }
    else
    {
      sprintf (Str,"t1.txt=\"%s\"���",MsgMass[75][CurrLang]); // ��
    }
    NEX_Transmit((void*)Str);//
    // Date
    sprintf (Str,"t2.txt=\"%s\"���",MsgMass[13][CurrLang]); // Date
    NEX_Transmit((void*)Str);//
    // Time
    sprintf (Str,"t4.txt=\"%s\"���",MsgMass[15][CurrLang]); // time
    NEX_Transmit((void*)Str);//
    // Battery
    sprintf (Str,"t6.txt=\"%s\"���",MsgMass[16][CurrLang]); // Battery
    NEX_Transmit((void*)Str);//
    // Version
    GetNumVer(St);
    sprintf (Str,"t8.txt=\"v%s(%X)\"���",St,CheckErrMEM); // Version
    NEX_Transmit((void*)Str);//
    // Number
    sprintf (Str,"t9.txt=\"�%d\"���",GetNumDevice()); // Number
    NEX_Transmit((void*)Str);//
    
  }
  // �������� ��������� ������� ��� ��������� (����� � �����������)
  // time
  if(SecOld != current_time.RTC_Sec)
  {
    //����������� �������
    SecOld = current_time.RTC_Sec ; 
    sprintf(Str,"t5.txt=\"%02d:%02d:%02d\"���",current_time.RTC_Hour,current_time.RTC_Min,current_time.RTC_Sec);
    NEX_Transmit((void*)Str);//
    // ����������� ����
    sprintf (Str,"t3.txt=\"%02d.%02d.%02d\"���",current_time.RTC_Mday,current_time.RTC_Mon,current_time.RTC_Year);//,MsgMass[14][CurrLang] ����� ��� 25.12.2013
    NEX_Transmit((void*)Str);//
    // ������� � ��� ������
    if (!(EXT_POW))
    {
      sprintf(Str,"t7.txt=\"%s\"���",MsgMass[11][CurrLang]);// ������� �������
    }
    else
    {
      sprintf(Str,"t7.txt=\"%d%%\"���", ProcBatInd);
    }
    NEX_Transmit((void*)Str);// 
  }
  g_FirstScr=0; // ��������� ������� ������� �������
  
  if ((HAL_GetTick() - TimeBegin) > 4000) //4s
    //if (CntWelcome > WAITWELCOME)
  {
    SetMode(ModeMainMenu);
    CmdInitPage(1);// ������� ������� ������������ ���� �� MainMenu � ��������� �������� ������� �����
    
    myBeep(10);
    ModeDevice = MODEMENU;
    SetHeadFileRaw (0); // ����� ��������� ����� ������ ������������� ������
    
    //ADCData++;
    //ClearScreen();
  }   
  
}

void ModeMainMenu(void) // ����� ��������� ����
{ 
  static volatile BYTE FrSetMainMenu = 1; // ��������� �� ������
  static WORD CntInd = 0;
  //static WORD ProcBatNow = 55;
  static BYTE OnlyBat = 1;
  static BYTE KnowLCD = 0;

  char Str[32];
  char StrN[32];
  //  static long tutu=0;
  //  Rect rct_pic={tutu,0,tutu+128,64};
  //  ������� �������� �������  
  //  if (BUTTON_DOWN(BTN_UP)/*(KeyP & (1<<b_UP)*/)&&(getStateButtons(b_UP)==SHORT_PRESSED))
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1;
    FrSetMainMenu = ChangeFrSet (FrSetMainMenu, 4, 1, MINUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1;
    FrSetMainMenu = ChangeFrSet (FrSetMainMenu, 4, 1, PLUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_DOWN);
  }
  
  //  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
  //  {
  //    tutu++;
  //    //ClrKey (BTN_LEFT);
  //  }
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
  {
    myBeep(10);
    while (SetIndexLight(1)!=1);
    //Light(GetTimeLight(SetIndexLight(0)));
    //ClrKey (BTN_LEFT);
  }
  if ((CntInd++%10)==0)
  {
    //ProcBatNow=ProcBatInd;
    OnlyBat = 1;
  }
  
  // ������� ���� ��� ���������� ���� LCD �� ��������
  // ���������� ������ ����������
  if(g_FirstScr) // ���������� ����������� �����
  {
    // ������������ �������
    GetDeviceLabel( Str, NameDB.Ena_DB  );
    sprintf (StrN,"t5.txt=\"%s\"���",Str); // 
    NEX_Transmit((void*)StrN);//
    // refl
    sprintf(Str,"t1.txt=\"%s\"���",MsgMass[7][CurrLang]);
    NEX_Transmit((void*)Str);//
    GetTesterName(Str ); // Tester
    sprintf (StrN,"t2.txt=\"%s\"���",Str); // 
    NEX_Transmit((void*)StrN);//
    sprintf(Str,"t3.txt=\"%s\"���",MsgMass[9][CurrLang]);
    NEX_Transmit((void*)Str);//
    sprintf(Str,"t4.txt=\"%s\"���",MsgMass[10][CurrLang]);
    //LCD_LIGHT(ON);
    NEX_Transmit((void*)Str);//
    //CreatDelay (3000000); // 168 �� - ���� ��� ������ (�������������) 83nS*30000 ���� �������������� 2 ��
    HAL_Delay(300);    // ��������� ��������� ��� ������ ����������
    // �������� ����� ���������� ������ 
     TypeLCD=1;
     
#if 0
     StartRecievNEX (500);
    sprintf(Str,"get t6.txt���");
    NEX_Transmit((void*)Str);//
    //NEX_Transmit((void*)CmdBuf);//
    while(!((g_WtRdyNEX)||(ReadyNEX==4)));
    // ����� ������ ����� ��������� �� ���������� ������� �� ����������
    // ��� ����� ��� ������ �����������
    // ���� ����� ��������� ������
    //CreatDelay (2000000); // 168 �� - ���� ��� ������ (�������������) 83nS*30000 ���� �������������� 2 ��
    if(RX_BufNEX[0] == 0x70) // ���� �����! ��������� ������
    {
      for(int i=0;i<25;++i)VerFW_LCD[i]=RX_BufNEX[i+1];
      VerFW_LCD[23]=0;
      // ����� ������� ������������� ���������� (���� ��� �������)
      // �� ����� ��� ��������� ����������� ��� ��������� ������������� � � ������
      switch(VerFW_LCD[3])
      {
      case '2':
        TypeLCD=0;
        KnowLCD = 1;
        break;
      case '5':
        TypeLCD=1;
        KnowLCD = 1;
        break;
      default:
        TypeLCD=0;
        KnowLCD = 0;
        break;
      }
    }
    if(!KnowLCD)
      // �������� ��������� "�������" ���� ������������������ � ���������� �������� ������
      AlarmSignal(3);
#endif

      //���� ��������� ��������� QR code
        if(TypeLCD)
    sprintf(Str,"qr0.pco=BLACK���"); // QR ������
    else
    sprintf(Str,"qr0.pco=BLUE���"); // QR �����
      
    NEX_Transmit((void*)Str);// 

    g_FirstScr = 0;
    g_NeedScr = 1;
    OnlyBat=1;
  }
  if(g_NeedScr) // ����������� ������ ! ���������! ���� ��������� 
  {
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str,"t1.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t3.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t4.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str,"t%d.bco=GREEN���",FrSetMainMenu); // �������
    NEX_Transmit((void*)Str);// 
//    if(TypeLCD)
//    sprintf(Str,"qr0.pco=BLACK���"); // QR ������
//    else
//    sprintf(Str,"qr0.pco=BLUE���"); // QR �����
//      
//    NEX_Transmit((void*)Str);// 
    
    g_NeedScr=0;
  }
  if (OnlyBat)
  {
    // ������� � ��� ������
    if (!(EXT_POW))
    {
      sprintf(Str,"t0.txt=\"%s\"���",MsgMass[11][CurrLang]);// ������� �������
    }
    else
    {
      sprintf(Str,"t0.txt=\"%d%%\"���", ProcBatInd);
    }
    NEX_Transmit((void*)Str);// 
    OnlyBat=0;
  }
  // ������ "��" ��� ������� ���� �������� ������� � ������ ����, ������� �� ����� ��������� ����!
  if (PRESS(BTN_OK))
  {
    myBeep(10);
    switch (FrSetMainMenu)
    {
    case 1: // ������������
      // ������� � ����� ��������� ���������� ������������� �� ���� ��� ��� ��������� ���� ��������
      SetMODESettingRef ();
      // ������������� ������ ���������� ��������� �������
      SetSeqCombLW();
      
      CmdInitPage(2);// ������� ������� ������������ ���� �� OTDR
      
      //     SetIndexLN(GetIndexLN());//������������� ������� ��������� �� ������ � ���������
      //     SetMode(ModeSetupOTDR);
      //     SetIndexWAV(GetWAV_SC(GetPlaceLS(CURRENT))); // ������������� �����. ����������� ��������� ����� �����
      //
      //     GetSetModeLW(-1); // ���������� ������� ���� ���� ����������
      //     CntLS = 1;
      //     PosCursorMain (-4100); // ����� ������� � ������
      //     IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
      //     GetSetHorizontScale (5); // ����� �������� ��� ����� �������
      //     if (IndexSmall==0)GetSetHorizontScale (-2);
      //     // ������������� ��� 2 �� 
      //     ModeDevice = MODESETREFL;
      //     SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
      //     
      //     PWM_LMOD_Init (OFF); // ��������� LMOD �� PWM 0 - OFF, 1- 270Hz 2-2kHz  
      break;
    case 2: // ������
      
      if (GetCfgPM ()) // ���� ����������
      {
        SetMode(ModeSelectOLT); 
        ErrOLT = CheckErrCoeff ();
        
        
        ModeDevice = MODEOTHER; // ����� ��� �������� � ����� ���������� �� ����. ��������
        CmdInitPage(3);// ������� ������� ������������ ���� �� Tester (����� ���� ��� ���)
        
      }
      else
      {
        SetMode(ModeSourceOnly); // ����� ������ (������ ��������)
        ModeDevice = MODETEST;
        CmdInitPage(6);// ������� ������� ������������ ���� �� ������ ������
        
      }
      POWDET(ON); // �������� ������� ����������
      // ������� "����� ��������" ����� ��������� �������, ��� ��� �� ��������� ��������
      // �� ������� ������������ ���������, ��� ���� �� 2529 � 3.2 �����
      // 
      HAL_Delay (40); //
      SetupSource ( SetModeLS (Str, CURRENT, CurrLang)); // ���������� ����� ������ ���������, � ����� ������ ��� ����������� �� ����������
      
      break;
    case 3: // ������
//123      PWM_LMOD_Init (OFF); // ��������� LMOD �� PWM 0 - OFF, 1- 270Hz 2-2kHz  
      SetMode(ModeSelectMEM);
      ModeDevice = MODEOTHER;
//123      SSPInit_Any(MEM_FL1); // ������������� SSP ��� ���������� FLASH (���� 1 �� ��� �� ����� ����������)
      CmdInitPage(4);// ������� ������� ������������ ���� �� MemoryMenu
      
      break;
    case 4: // ���������
      SetMode(ModeSetting);
//123      PWM_LMOD_Init (OFF); // ��������� LMOD �� PWM 0 - OFF, 1- 270Hz 2-2kHz  
      ModeDevice = MODESETUP;
//123      SSPInit_Any(MEM_FL1); // ������������� SSP ��� ���������� FLASH (���� 1 �� ��� �� ����� ����������)
      CmdInitPage(5);// ������� ������� ������������ ���� �� Setting
      break;
    }
    //ClrKey (BTN_OK);
  }
  // ������� "����� ��������"
  HAL_Delay (20); //
  
  //ClrKey (BTN_MENU + BTN_LEFT + BTN_RIGHT);
}

void ModeSetupOTDR(void) // ����� ��������� ������������� CHECK_OFF
{
  static volatile BYTE FrSetSetupOTDR = 0; // ��������� �� ������
  char Str[64];
  int NeedReSaveIndex=0; // ������� �������������� �������� ������� � ������ ����������� � ����� ������
  
  //BYTE CurrLang=GetLang(CURRENT);
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetSetupOTDR = ChangeFrSet (FrSetSetupOTDR, 4, 0, MINUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetSetupOTDR = ChangeFrSet (FrSetSetupOTDR, 4, 0, PLUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_DOWN);
  }
  switch (FrSetSetupOTDR) // ��������� �������� ����� ���������
  {
  case 0: //����� �����
    // ������ �� ����������� ����!
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
    {
      myBeep(10);
//      if(g_AutoSave) g_AutoSave = 0;
//      else g_AutoSave = 1;
//      if(GetIndexVRM()>3) // ����� �������� ��� ������������ �� �������� ���� ����������
//      g_AutoSave = 0; 
      if((g_AutoSave)||(GetIndexVRM()>3)) g_AutoSave = 0;
      else g_AutoSave = 1;
      // ������������� ������ � ����������� �� ���������� 
      // ���� ����� �� ���������� �� ����� �������� ����
      SetPlaceNew(0);
      g_FirstScr = 1; // Need reDraw Screen
      
    }
    // ����� <---
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_FirstScr = 1; // Need reDraw Screen
      if(GetIndexVRM()>3) // ����� �������� ��� ������������ �� �������� ���� ����������
      {
      if (SettingRefl.SetModeLW>0)SettingRefl.SetModeLW--;
       else
       SettingRefl.SetModeLW =GetSetNumLS(0)-1  ;
      }
      else
      {
      if (SettingRefl.SetModeLW>0)SettingRefl.SetModeLW--;
      else
      {
        SettingRefl.SetModeLW = (1<<GetSetNumLS(0))-2;
      }
      }
      // ������������� ������ � ����������� �� ����������
      SetPlaceNew(1);
    }
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_FirstScr = 1; // Need reDraw Screen
      // 22.11.2022 ����������� � �������������� ���� ���� � ����������� �� ���������� ������� ���������
      if(GetIndexVRM()>3) // ����� �������� ��� ������������ �� �������� ���� ����������
      {
        if(SettingRefl.SetModeLW<(GetSetNumLS(0)-1)) SettingRefl.SetModeLW++;
        else         SettingRefl.SetModeLW = 0;

      }
      else
      {
      if ((SettingRefl.SetModeLW+1)<((1<<GetSetNumLS(0))-1))SettingRefl.SetModeLW++;
      else
      {
        SettingRefl.SetModeLW = 0;
      }
      }
      // ������������� ������ � ����������� �� ����������
      SetPlaceNew(1);
    }
    //    if (((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))||((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED)))
    //    {
    //      myBeep(10);
    //      g_FirstScr = 1; // Need reDraw Screen
    //      if ((CntLS) < GetSetNumLS(0))
    //      {
    //        GetSetModeLW(-1); // ���������� ���� �����
    //        CntLS++;
    //        GetPlaceLS(NEXT);
    //      }
    //      else
    //      {
    //        if (GetIndexVRM()<4)
    //          GetSetModeLW(1); // ������������� ���� �����, ���� �������� ������� ���������� 15,30,60,180�
    //        else GetPlaceLS(NEXT);
    //        CntLS=0; // ��� �������� ����� ��� ���� 0
    //      }
    //      SetIndexWAV(GetWAV_SC(GetPlaceLS(CURRENT))); // ������������� �����. ����������� ��������� ����� �����
    //      
    //      //ClrKey (BTN_RIGHT);
    //    }
    break; //����� �����
  case 1: //����� �����
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
//123      PosCursorMain (-4100); // ����� ������� � ������
      IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
//123      GetSetHorizontScale (5); // ����� �������� ��� ����� �������
      if (!GetSubModRefl ())// ������
      { 
        if ((GetIndexLN()==0)&&(IndexSmall==0))
        {
          IndexSmall=1;
        }
        else
          SetIndexLN(GetIndexLN()+1);
      }
      else // 
      {
        SetSubModRefl (MANUAL);
        SettingRefl.SubModeRef = MANUAL;
        SetIndexLN(0);
        IndexSmall=0;
//123        GetSetHorizontScale (-2); // ������� ��� 0.5 ��
      }
      //ClrKey (BTN_RIGHT);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
//123      PosCursorMain (-4100); // ����� ������� � ������
      IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
//123      GetSetHorizontScale (5); // ����� �������� ��� ����� �������
      if (!GetSubModRefl ())// ������
      {
        if (GetIndexLN()>0)
        {
          
          SetIndexLN(GetIndexLN()-1);
        }
        else
        {
          if ((GetIndexLN()==0)&&(IndexSmall==1))
          {
            IndexSmall=0;
//123            GetSetHorizontScale (-2); // ������� ��� 0.5 ��
          }
          else
          {
            SetSubModRefl (AUTO);
            SettingRefl.SubModeRef = AUTO;
            SetIndexLN(0);
            IndexSmall=1;
          }
        }
      }
      else // 
      {
        SetIndexLN(0);
      }
      //ClrKey (BTN_LEFT);
    }
    break; //����� �����
  case 2: //������������ �������� 
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      if (!GetSubModRefl ())// ������
      {
        SetIndexIM(GetIndexIM()+1);
      }
      else // 
      {
        SetIndexIM(0);
      }
      //ClrKey (BTN_RIGHT);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      if (!GetSubModRefl ())// ������
      {
        if (GetIndexIM()>0)
        {
          SetIndexIM(GetIndexIM()-1);
        }
        else
        {
          SetIndexIM(0);
        }
      }
      else // 
      {
        SetIndexIM(0);
      }
      //ClrKey (BTN_LEFT);
    }
    break; //������������ ��������
  case 3: //����� ��������� ��� ��� �������� ����� �������
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //if (GetSetModeLW(0))
    if (g_AutoSave)
      {
        if (NumFiber<999) NumFiber++;
        else NumFiber=1;
      }
      else
      {
        SetIndexVRM(GetIndexVRM()+1);
        if (GetIndexVRM()>3)// ���������� ������� ���� ������
        {
          GetSetModeLW(-1); // ���������� ���� �����
          //CntLS=1;
        }
      }
      //ClrKey (BTN_RIGHT);
    }
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //if (GetSetModeLW(0))
    if (g_AutoSave)
      {
        if (NumFiber<989) NumFiber=NumFiber+10;
        else NumFiber=1;
      }
      //ClrKey (BTN_RIGHT);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //if (GetSetModeLW(0))
    if (g_AutoSave)
      {
        if (NumFiber>10) NumFiber=NumFiber-10;
        else NumFiber=990;
      }
      ClrKey (BTN_LEFT);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //if (GetSetModeLW(0))
    if (g_AutoSave)
      {
        if (NumFiber>1) NumFiber--;
        else NumFiber=999;
      }
      else
      {
        if (GetIndexVRM()>0)
        {
          SetIndexVRM(GetIndexVRM()-1);
        }
        else
        {
          SetIndexVRM(0xFF);
        }
        if (GetIndexVRM()>3)// ���������� ������� ���� ������
        {
          GetSetModeLW(-1); // ���������� ���� �����
          //CntLS=1;
        }
      }
      //ClrKey (BTN_LEFT);
    }
    break; //����� ���������
  case 4: //�����.����������� 
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //if (GetSetModeLW(0))
    if (g_AutoSave)
    {
      if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
      {
        g_NeedScr = 1; // Need reDraw Screen
        myBeep(10);
        for (int Ind =ARRAY_SIZE(PrefixFileNm)-2; Ind>=0; Ind--)
        {
          if (PrefixFileNm[Ind]<0x20) PrefixFileNm[Ind]=' ';
          else if (PrefixFileNm[Ind]!=' ' && IndexPrefix == 0) IndexPrefix = Ind;
          //Index_Comm --;
        }
        if (PrefixFileNm[IndexPrefix]!=' ')IndexPrefix ++;
        KbPosX = 11;
        KbPosY = 2;
        SetMode(ModeKeyBoardPrefix);
        ModeDevice = MODEOTHER;
        NeedKeyB =1; // ���� ������������ � ����������
        //ClrKey (BTN_OK);
      }
    }
    else
    {
      //            if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED)) // �������� ��������� �����. �����������
      //      {
      //        myBeep(20);
      //        SetWAV_SC(GetIndexWAV(),GetPlaceLS(CURRENT));
      //        SSPInit_Any(MEM_FL1); // ������������� SSP ��� ������ FLASH 
      //        SaveUserConfig();
      //        //SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA 
      //
      //      }
      
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetIndexWAV(GetIndexWAV()+0.0001);
        NeedReSaveIndex = 1;
        //ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetIndexWAV(GetIndexWAV()-0.0001);
        NeedReSaveIndex = 1;
        //ClrKey (BTN_LEFT);
      }
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
      {
        //myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetIndexWAV(GetIndexWAV()+0.0001);
        NeedReSaveIndex = 1;
        ////ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
      {
        //myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetIndexWAV(GetIndexWAV()-0.0001);
        NeedReSaveIndex = 1;
        //ClrKey (BTN_LEFT);
      }
    }
    break; //�����.�����������
  }
  // ���� �� ����� ����������� 
  if ((FrSetSetupOTDR == 4) && (NeedReSaveIndex))
  {
    ReSaveWAV_SC (); // ������������� ���� ���� ��������� �������
    NeedReSaveIndex = 0;
  }
  // ��������� ����� ��������� ���� � ������ ��� � �������� �� ���������� ����
  if(g_FirstScr)
  {
    
    // ������� �������� 
    // ������������� ������ � ����������� �� ����������
    
    switch (GetCombLS(SettingRefl.SetModeLW))
    {
    case 1: // ��������� ����� ����
    case 2:
    case 4:
    sprintf(Str,"t0.txt=\"%s\"���", MsgMass[17][CurrLang]); // ����� �����
      break;
    default: // ����������
    sprintf(Str,"t0.txt=\"%s\"���", MsgMass[125][CurrLang]); // ��. ���� ����
      
      break;
    }
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t2.txt=\"%s\"���", MsgMass[19][CurrLang]); // ����� ����� 
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t4.txt=\"%s\"���", MsgMass[22][CurrLang]); //����.���.
    NEX_Transmit((void*)Str);// 
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //sprintf(Str,"t6.txt=\"%s\"���", (GetSetModeLW(0))?(MsgMass[61][CurrLang]):(MsgMass[24][CurrLang])); //"����� ���."//"� �������"
    sprintf(Str,"t6.txt=\"%s\"���", (g_AutoSave)?(MsgMass[61][CurrLang]):(MsgMass[24][CurrLang])); //"����� ���."//"� �������"
    NEX_Transmit((void*)Str);// 
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //sprintf(Str,"t8.txt=\"%s\"���", (GetSetModeLW(0))? (PrefixFileNm):(MsgMass[27][CurrLang])); //"������� �����.������."
    sprintf(Str,"t8.txt=\"%s\"���", (g_AutoSave)? (PrefixFileNm):(MsgMass[27][CurrLang])); //"������� �����.������."
    NEX_Transmit((void*)Str);// 
    
    
    
    g_NeedScr = 1; // ��� ������ ���������� ��������
    g_FirstScr = 0;
  }
  // ���� ��� �� �������� � ����� ��������� 
  if(g_NeedScr)
  {
    //FIO1PIN |=LEDSTART;//On  LED
    // ������ ������ ����� �����
    switch (GetCombLS(SettingRefl.SetModeLW))
    {
    case 1: // ��������� ����� ����
      sprintf(Str,"t1.txt=\"%d%s\"���",GetLengthWaveLS (0),MsgMass[18][CurrLang]); // nm
      break;
    case 2:
      sprintf(Str,"t1.txt=\"%d%s\"���",GetLengthWaveLS (1),MsgMass[18][CurrLang]); // nm
      break;
    case 4:
      sprintf(Str,"t1.txt=\"%d%s\"���",GetLengthWaveLS (2),MsgMass[18][CurrLang]); // nm
      break;
    case 3:
      sprintf(Str,"t1.txt=\"%02d,%02d\"���",(GetLengthWaveLS(0)%1000)/10,(GetLengthWaveLS(1)%1000)/10); // 1 � 2
      break;
    case 5:
      sprintf(Str,"t1.txt=\"%02d,%02d\"���",(GetLengthWaveLS(0)%1000)/10,(GetLengthWaveLS(2)%1000)/10); // 1 � 3
      break;
    case 6:
      sprintf(Str,"t1.txt=\"%02d,%02d\"���",(GetLengthWaveLS(1)%1000)/10,(GetLengthWaveLS(2)%1000)/10); // 2 � 3
      break;
    case 7:
      sprintf(Str,"t1.txt=\"%02d,%02d,%02d\"���",(GetLengthWaveLS(0)%1000)/10,(GetLengthWaveLS(1)%1000)/10,(GetLengthWaveLS(2)%1000)/10); // ��� 3
      break;
    default: // ����������
      sprintf(Str,"t1.txt=\"?NON?\"���"); // test
      
      break;
    }
    
    // ������ ������ ����� �����
//    if (GetSetModeLW(0)) 
//     // sprintf(Str,"t1.txt=\"%s\"���",MsgMass[21][CurrLang]); // auto
//      sprintf(Str,"t1.txt=\"13,15,85\"���"); // test
//    else
//      sprintf(Str,"t1.txt=\"%d%s\"���",GetLengthWaveLS (GetPlaceLS(CURRENT)),MsgMass[18][CurrLang]); // nm
    NEX_Transmit((void*)Str);// 
    // ������ ������ - ��������
    if (!GetSubModRefl ()) // manual
    {
      if ((GetIndexLN()==0)&&(IndexSmall==0))
        sprintf(Str,"t3.txt=\"0.5%s\"���", MsgMass[20][CurrLang]); // km
      else 
        sprintf(Str,"t3.txt=\"%d%s\"���", GetLengthLine(GetIndexLN()),MsgMass[20][CurrLang]); // km
    }
    else 
    {
      sprintf(Str,"t3.txt=\"%s\"���", MsgMass[21][CurrLang]);
      SetIndexLN (0); // ��������� ������� ����� �����
    }
    NEX_Transmit((void*)Str);// 
    
    // ������ ������ - ������������ ��������
    if (!GetSubModRefl ()) sprintf(Str,"t5.txt=\"%d%s\"���", GetWidthPulse(GetIndexIM()),MsgMass[23][CurrLang]);//ns
    else 
    {
      sprintf(Str,"t5.txt=\"%s\"���", MsgMass[21][CurrLang]);
      SetIndexIM (0); // ��������� ������� ����� ��������
    }
    NEX_Transmit((void*)Str);// 
    // ��������� ������ � ����������� �� ��������� ����� �����
    // ������ ����������� ������� ��������� ��� ������ �������
    // 22.11.2022 ��������� ��������� ����� 4-5 ���� ��������� � ������ � �������������� �����������
    //if (GetSetModeLW(0))
    if (g_AutoSave)
    {
      sprintf(Str,"t7.txt=\"%03d\"���", NumFiber);//c
    }
    else
    {
      sprintf(Str,"t7.txt=\"%d%s\"���", GetTimeAvrg(GetIndexVRM()),MsgMass[4][CurrLang]);//c
      if (GetIndexVRM() == 5)
        sprintf(Str,"t7.txt=\"%s\"���", MsgMass[25][CurrLang]);// "����."
      if (GetIndexVRM() == 4)
        sprintf(Str,"t7.txt=\"%s\"���", MsgMass[26][CurrLang]);//"�����."
    }
    NEX_Transmit((void*)Str);// 
    // ����� ������ - ������ ��������� ��� ������� ��� ����
    // ������ ����������� ����� ����������� ��� �������� ����� �����
    //if (GetSetModeLW(0))
    //123
    // ����� ���������� ������� ��� ���������� ������ � ������ �������
    // ���� ����������� , ���� ���� ����
    if (g_AutoSave)
    {
      sprintf(Str,"t9.txt=\"%03d/%03d\"���",GetNumTraceSaved(0),MAXMEMALL);// ������� ������
    }
    else
    {
      sprintf(Str,"t9.txt=\"%.4f\"���",GetIndexWAV ());
    }
    //  sprintf(Str,"t9.txt=\"����\"���");// ���� ������
    NEX_Transmit((void*)Str);// 
    // �������� ����������  � ��������� ���������
    sprintf(Str,"t0.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t4.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t6.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t8.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str,"t%d.bco=GREEN���",FrSetSetupOTDR<<1); // ������� ?
    NEX_Transmit((void*)Str);// 
    // �������� ���� t1 � ����������� �� ������
    if(g_AutoSave)
        sprintf(Str,"t1.bco=YELLOW���"); // ���������
    else
        sprintf(Str,"t1.bco=WHITE���"); // �����

    NEX_Transmit((void*)Str);//
    
    //FIO1PIN &=~LEDSTART;//Off  LED
    g_NeedScr = 0;
  }
  // ������ �������� � ����
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    SetMode(ModeMainMenu);
    myBeep(10);
    ReSaveWAV_SC (); // ������������� ���� ���� ���������
    ModeDevice = MODEMENU;
    CmdInitPage(1);// ������� ������� ������������ ���� �� MainMenu (�������)
  }
  if (rawPressKeyS) // START Measure �� ������ ��������� �������������
  {        
    myBeep(10);
    // ��������� ��������� ��������� ���� ���������
    EEPROM_write(&SettingRefl, ADR_ReflSetting, sizeof(SettingRefl));
    ReSaveWAV_SC (); // ������������� ���� ���� ���������
    LSEL0(0);
    LSEL1(0);
    SlowON();
    ReturnMemView = 0; // ������� ������� �������� � ����� ��������� ������������� ���� ������ ������
    //POWALT(ON);
    //POWREF (ON);
    //POWDET(ON);
    SetMode(ModeStartOTDR);
    ModeDevice = MODEMEASURE;
    //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
    SubModeMeasOTDR = SETPOWER;
//123    SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
    rawPressKeyS=0;
    CntLS=0;
    //CreatDelay (30000); // 3.3 ��
  HAL_Delay(3);
  CmdInitPage(16);// ������� ������� ������������ ���� �� ������ ��������� Measuring
  }
  if(NeedKeyB ) // ������������� ������������ � ���������� ��� �������������� PreFix
  {
    // �������� ���������� ����� ������� ����������
    if (CurrLang) 
      // ������� ������� ������������ ���� �� EnglishPreF Keyboard 
      CmdInitPage(31); //(22)
    else
      // ������� ������� ������������ ���� �� RussianPreF Keyboard 
      CmdInitPage(32); //(23)
    NeedKeyB=0; 
  }
  
  
}

void ModeErrorOTDR(void) // ����� ����������� "��������� �� �����" CHECK_OFF
{
  char Str[32];
  //BYTE CurrLang;
  
  //CurrLang=GetLang(CURRENT);
  //CreatDelay(300000);
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
    myBeep(1);
  }
  //ClrKey (BNS_MASK); // ����� ������� ������
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[36][CurrLang]); // ��������
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[37][CurrLang]); // ��������� �� ����� [��]
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[38][CurrLang]); // ��� �����������
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[39][CurrLang]); // ������� <S> 
    NEX_Transmit((void*)Str);    //
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
    //sprintf(Str,"t3.bco=RED���"); // �����
    //NEX_Transmit((void*)Str);//
    
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }
  // ������ �������� ���������� (������� ������)
  
  if (rawPressKeyS)
  { 
    myBeep(10);
    //g_NeedScr = 1; // Need reDraw Screen
    SetMode(ModeSetupOTDR);
    GetSetModeLW(-1); // ����� �������� ���� ���� ����������, ��� ��� ������
    ModeDevice = MODESETREFL;
    SubModeMeasOTDR = NOTMODE;
    GetPlaceLS(CURRENT);
    rawPressKeyS=0;
    // ������� ������� �������� �� OTDR (��������� �������������)
    CmdInitPage(2);
    
  }
  //CreatDelay (400000); // 3.3 ��
    HAL_Delay(40);

}

#pragma optimize=none  
void ModeStartOTDR(void) // ����� ���������� �������������
{
  DWORD Mean=0;
  DWORD Noise ;
  //static DWORD LengthLine = 0;
  
  DWORD LocalMax = 0;
  static BYTE SizeRepit = 0;// ����� ���������� ��� ������� ������ ��������� � ������������
  static BYTE InputOK = YES;
  static BYTE LengthOK = YES;
  static BYTE IndxAddBad = 0; //�������� ������� ����� ��� ���������� ����� ���������� ����� �����

  static BYTE ShadowIndexLN; //
  static BYTE ShadowIndexIM;
  static BYTE NeedResetIM = 0; // ������� ������������� ������������� ������������ �������� ����� ������� ���������
  //static DWORD CurrentSumDelay; // �������� ��������� �������� ��� ��������� ����� ����� ��� ������ ����������
  //static unsigned CntAvrg; // ������� ���������� (����� �� �������� �� 3 ���)
  char Str[22];
  //BYTE CurrLang;
  //CurrLang=GetLang(CURRENT);
  switch (SubModeMeasOTDR)
  {
  case SETPOWER: // ��������� ������� (������ ������������� ���������) (�������� ���)
    // ���� ����� ����������� ������ ���������
    //sprintf(Str,"ooo\r");//c
    //UARTSend0 ((BYTE*)Str, strlen (Str));
    
    LSEL0(0);
    LSEL1(0);
    if(g_FirstScr) // ������ ����� ���������
    {
      sprintf(Str,"t0.txt=\"%s\"���", MsgMass[28][CurrLang]); // Wait
      NEX_Transmit((void*)Str);// 
      // ��������� ��������
      sprintf(Str,"t1.txt=\"\"���"); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t2.txt=\"\"���"); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t3.txt=\"\"���"); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t4.txt=\"\"���"); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t5.txt=\"\"���"); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t6.txt=\"\"���"); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t7.txt=\"\"���"); //
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t8.txt=\"\"���"); //
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t9.txt=\"\"���"); //
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t10.txt=\"\"���"); //
      NEX_Transmit((void*)Str);// 
      g_FirstScr = 0;
      g_NeedScr = 1;
    }
    SubModeMeasOTDR = INPUTOTDR;
    ShadowIndexLN = GetIndexLN(); // ���������� ������������� �����, ���������� ������� �����
    ShadowIndexIM = GetIndexIM();
    PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
    myBeep (0);
    //123    disable_timer ( 0 );
    LED_START(1);//On  LED
    CntNumAvrg = 0; // �������� ������� ����������
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(ON); // ON HIGH VOLT
    // �������� ������ 2 � ��������� �����
    //123    reset_timer(2);
    //123    enable_timer(2);
    CurrTimeAccum = 0; // ������� ����� ���������� 1 ���
    EnaTimerAccum = 1; // ��������� ������� ����� ����������
    
    Averaging (5,0,0);//������ ������������� ���������
    break;
  case INPUTOTDR:
    LSEL0(0);
    LSEL1(0);
    if(g_NeedScr) //
    {
      sprintf(Str,"t1.txt=\"%s\"���",MsgMass[34][CurrLang]); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t6.txt=\"???\"���"); // 
      NEX_Transmit((void*)Str);// 
      g_NeedScr = 0;  
    }
    PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
    CntNumAvrg = 0; // �������� ������� ����������
    Averaging (200,0,0);
    for (int i=0; i<RAWSIZE; ++i)
    {
      Mean +=RawData[i];
    }
    Mean = Mean/RAWSIZE;
    Mean = Mean/200;
    //Mean = 200; // ���� ��� �������� ���������
    if (Mean > 2000)
    {
      POWDET(OFF);
      POWREF (OFF);
      SetMode(ModeErrorOTDR);
      ModeDevice = MODEERREFL;
      SubModeMeasOTDR = NOTMODE;
      //123      SSPInit_Any(MEM_FL1); // �������������� ������������� SSP ��� ���������� ������� FLASH (���� 1 �� ��� �� ����� ����������)
      //123      enable_timer ( 0 );
      myBeep(20);
      LED_START(0);//Off  LED
      // ������� ������� ������������ ���� �� ��������� �� ��������� �� ����� ErrPowerInput
      //CmdInitPage(19);
      NeedReturn = 19;
      //g_FirstScr=1;
    }
    else
    {
      HV_LOW(OFF); //ON  HIGH VOLT FULL
      HV_SW(ON); // ON HIGH VOLT
      SubModeMeasOTDR = CALKORLBEG;
    }
    break;
    // ������� ��������� ORL (� ���� ������ )
    // 40 nS 16 km, �� ������ �� ���������
  case CALKORLBEG:
    // ����� ������� ��������� ORL 
    g_VolORL = MeasORL (1000, 0);
    LED_START(1);//On  LED - ���������� ���������
    // �������� ��� ��� ��������� �� �����
    sprintf(Str,"t6.txt=\"%s\"���",MsgMass[35][CurrLang]); // 
    NEX_Transmit((void*)Str);// 
    
    g_NeedScr =1;
    SubModeMeasOTDR = QUALITYBEGIN;
    break;
  case QUALITYBEGIN:
    SetIndexWAV(GetWAV_SC(GetPlaceLS(CURRENT))); // ������������� �����. ����������� ��������� ����� �����
    
    //GetPlaceLS(CURRENT);
    SetIndexLN(0);
    SetIndexIM(0);
    if(g_NeedScr)
    {
      //sprintf(Str,"t6.txt=\"%s\"���",MsgMass[35][CurrLang]); // 
      //NEX_Transmit((void*)Str);// 
      sprintf(Str,"t2.txt=\"%s\"���",MsgMass[32][CurrLang]); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t7.txt=\"%s\"���",MsgMass[33][CurrLang]); // 
      NEX_Transmit((void*)Str);// 
      
      g_NeedScr = 0;
    }
    PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    InputOK = YES; // �������� ������� ������� �������
    memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
    CntNumAvrg = 0; // �������� ������� ����������
    Averaging (BEGZONEAVRG,6,0); // �������� 2�� ����� (128��)
    LocalMax = 0;
    Mean = (DWORD)(5000.0*log10(1023*BEGZONEAVRG)); // ������������ ������� ��� ������ ����������
    for (int i=0; i<BEGZONEPNT; ++i)
    {
      LocalMax +=LogData[i];
    }
    LocalMax=LocalMax/BEGZONEPNT; // ������� ������� �� 10 ����� ������
    if (LocalMax<800) 
      InputOK = NO; // ����� ����� ���� -3�� = ������ ������
    g_NeedScr = 1;
    SubModeMeasOTDR = SEARCHENDLINE;
    break;
  case SEARCHENDLINE:
    SetIndexLN(6); //128km
    SetIndexIM(4); //1000ns // ����� ��� ������ ���� (���� 5) ??? 31.08.2012
    if(g_NeedScr)
    {
      sprintf(Str,"t7.txt=\"%s\"���",((InputOK)?("OK"):("???"))); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t3.txt=\"%s\"���",MsgMass[31][CurrLang]); // ����� �����
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t8.txt=\"???\"���"); //  ��������� - ������������
      NEX_Transmit((void*)Str);// 
      
      g_NeedScr = 0;
    }
    PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
    CntNumAvrg = 0; // �������� ������� ����������
    Averaging (500,0,0);
    CurrentSumDelay = 0;
    Noise = 0;
    LocalMax = 0;
    // ������� ������� � ��� � �������� ������� �������� ������� �������  ������ �� 3��
    for (int i = 0;i<100;++i)
    {
      if (RawData[RAWSIZE-100+i]>LocalMax) LocalMax = RawData[RAWSIZE-100+i];
      Noise += RawData[RAWSIZE-100+i];
    }
    Noise = Noise/100;
    RawData[0] = Noise + (LocalMax-Noise)*2;
    // ������� ��������
    /*
    for (int i= 2; i<RAWSIZE; ++i)
    {
    if (RawData[i]<Noise)RawData[i]=Noise+1;
    RawData[i] = 5000.0*log10((double)(RawData[i]));
    
  }
    */
    LengthOK = YES;
    CurrentSumDelay = 1;
    Mean = Scan( RawData, RAWSIZE , RawData[0] );// ����� ��������� , ��� �� ���� ����� � ������ 128 ��
    //LengthLine = Mean;
    //for (Mean=RAWSIZE-1; ((Mean>90) && (RawData[Mean]<RawData[0])); --Mean);
    IndxAddBad = IndexSeek(Mean); // ����� ������ ���� ��������� ��� ���� ���������
    if (IndxAddBad>ShadowIndexLN) 
    {
      LengthOK = NO;
      // ���� ����������� ���������� �� ������ �����
      // �� ��� ���� �� ������� ������� �����
      // ������� � ������� � ����� ����� (83.33��)
      DWORD tmp = RAWSIZE/NumPointsPeriod[ShadowIndexLN];
      if (Mean>tmp) CurrentSumDelay = (Mean-tmp)*4;
    }
    else 
      {// ����� ������� ���������
      Mean = KeyPoints[ShadowIndexLN]-1;
      IndxAddBad = 0; // ����� ��� ��������� ������� ��������� ��� ����������
      }
    // Mean - ������ ������ ���������� �������� ����� �����
    // ����� ����������� �� 3 ��� ����� ��������� ��� 
    // ����� �������������� ������ �� ����� 28 ��
    // ����� ������ ������� � ������� ����� Mean*333.33 + 14000*NumPointsPeriod[ShadowIndexLN]
    TimeMeasure3S  = 2750000; // uS
    if (RemoutCtrl) TimeMeasure3S  = 3000000; //uS
    // ���������� ��������������� ����� ����������
    if(LengthOK) // ����� ����������,  
    NumAvrg = (unsigned)(TimeMeasure3S/((NumPointsPeriod[ShadowIndexLN]+1)*TimeRepitOfLN[ShadowIndexLN]+260));//*NumPointsPeriod[ShadowIndexLN])
    // ����� ������
    else
    NumAvrg = (unsigned)(TimeMeasure3S/((NumPointsPeriod[ShadowIndexLN]+1)*TimeRepitOfLN[IndxAddBad]+260));//*NumPointsPeriod[ShadowIndexLN])
      
      
                         // ����� ���������� ��� ������� ������� 2.75 ��� �� ����������� ����� � ����� ����� 333ns 
    //  NumAvrg = (unsigned)(7300000L/(Mean+10)); // ���� +30
    // ���������� �����. �������� ��� ���������� ����� 
    if (GetSubModRefl()) // �������������� - ������� �� ������
    {
      ShadowIndexLN = IndexSeek(Mean);
      ShadowIndexIM = IndexSeek(Mean);
     // NumAvrg = (unsigned)(8250000L/(KeyPoints[ShadowIndexLN]+50)); // ����� ���������� , ���� +30
    //NumAvrg = (unsigned)(TimeMeasure3S/(NumPointsPeriod[ShadowIndexLN]*TimeRepitOfLN[ShadowIndexLN]));//*NumPointsPeriod[ShadowIndexLN])
    NumAvrg = (unsigned)(TimeMeasure3S/((NumPointsPeriod[ShadowIndexLN]+1)*TimeRepitOfLN[ShadowIndexLN]+260));//*NumPointsPeriod[ShadowIndexLN])
      CurrentSumDelay = 1;
    }
    SetIndexLN(ShadowIndexLN);
    SetIndexIM(ShadowIndexIM);
    PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    //ValueDS = (unsigned)(MultIndex[GetIndexLN()]*(ADCPeriod*50000)/PointsPerPeriod); //  ������������� �������� DS ��� �������������� ������ ���������
    ValueDS = GetValueDS(); //  ������������� �������� DS ��� �������������� ������ ���������
    // ������������� ���������� � ������������ � ������ ����� �� ������
    //NumAvrg = NumAvrg/PointsPerPeriod; // ��������� ����� ���������� �� 1 �������
    // ��� ������ ����������
    //CurrentSumDelay += DelayPeriod[GetIndexLN()];
    g_NeedScr = 1;
    SubModeMeasOTDR = LASTCHECK;
    break;
  case LASTCHECK:
    // New LCD
    if(g_NeedScr)
    {
      if ((GetIndexLN()==0)&&(IndexSmall==0))
        sprintf(Str,"t8.txt=\"0.5%s%s\"���",MsgMass[20][CurrLang],((LengthOK)?(" "):("!")));//����� �����: XX��
      else
        sprintf(Str,"t8.txt=\"%d%s%s\"���",GetLengthLine(GetIndexLN()),MsgMass[20][CurrLang],((LengthOK)?(" "):("!")));//����� �����: XX��
      //sprintf(Str,"t6.txt=\"%s\"���",MsgMass[35][CurrLang]); // 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t4.txt=\"%s\"���",MsgMass[29][CurrLang]); // ����.��������: XX��
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t9.txt=\"%d%s\"���",GetWidthPulse(GetIndexIM()),MsgMass[23][CurrLang]); // ����.��������: XX��
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t5.txt=\"%s\"���",MsgMass[30][CurrLang]); //���� ���������: XX� 
      NEX_Transmit((void*)Str);// 
      sprintf(Str,"t10.txt=\"%d%s\"���",GetTimeAvrg(GetIndexVRM()),MsgMass[4][CurrLang]); //���� ���������: XX� 
      NEX_Transmit((void*)Str);// 
      
      g_NeedScr = 0;
    }
    
    memset( RawData, 0, RAWSIZE * sizeof(DWORD) ); // �������� ������ ����������
    FinAvrg = (GetTimeAvrg(GetIndexVRM())*NumAvrg)/3;
    CntNumAvrg = 0; // �������� ������� ����������
    if (GetIndexVRM()==4) NumAvrg = NumAvrg-10; // ������� ~1,5 cek
    ShadowIndexIM = GetIndexIM(); // ���������� ������ �������������� ��������
    if ((ShadowIndexIM > 4) && (GetIndexVRM()!=4))
    {
      SetIndexIM(4); //1000ns ��� ������� ��������� ������������� ������� ���� ����� ������� 3...10���
      //SetIndexShadowIM (ShadowIndexIM); // ��������� ������� ���� ��� �������� ��������
      SW_FLTR(ON);
      NeedResetIM = 1;
    }
    // ����������� ������� DAC
    //p0.26 (PHLD) 
    //PINSEL1 &=~0x00300000; //set P0.26 us GIO
    //PINSEL1 |=0x00200000; // set P0.26 us AOUT
    //SetLvlDAC (0);
    //123
    // ����� �����-�� �������� ����� �� "������" �� ������������ ������
    // �� ������� ��� 1 ����������, ����� �� �����
    //    if (!(FIO2PIN & BTN_DOWN))
    //    {
    //      Averaging (1,CurrentSumDelay,0);// ����� 3 ��� ������ ���������
    //      for (int i = 0; i<RAWSIZE;++i)
    //      {
    //        sprintf(Str,"%u\n", RawData[i]);//c
    //        UARTSend0 ((BYTE*)Str, strlen (Str));
    //      }
    //      sprintf(Str,"\r");//c
    //      UARTSend0 ((BYTE*)Str, strlen (Str));
    //      
    //      memset( RawData, 0, RAWSIZE * sizeof(DWORD) ); // �������� ������ ����������
    //      CntNumAvrg = 0; // �������� ������� ����������
    //      
    //    }
    // �������� ������ 2 � ��������� �����
    //123    reset_timer(2);
    //123    enable_timer(2);
    CurrTimeAccum = 0;
    EnaTimerAccum = 1;
    
    // ����� ������ 3000 ��� ��� �� ����� ���������� �� 3 ���
    //    CmdInitPage(17);// ������� ������� ������������ ���� �� DrawOTDR
    //    sprintf(Str,"t0.txt=\"%s\"���",MsgMass[30][CurrLang]); //���� ���������: XX� 
    //    NEX_Transmit((void*)Str);// 
    
    g_NeedScr = 1; // ���������� ��� ��� ������������ � ����� ����������� ����������
    // ���������� ���� ��� ������ ��������� � ������� �������
    Averaging (NumAvrg,IndxAddBad,1);// ����� 3 ��� ������ ���������
    // ��������� ������ ���������� 
    SubModeMeasOTDR = AVERAGING;
    // ����� ���������� ���� � ���������������
    
    //PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    //memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
    // ������ �������� ���������� � ����������� �� �������������� ������� ����������
    break;
    //  case ONESUMMONLY:
    //    
    //    break;
  case AVERAGING:
    //sprintf(Str,"xxx\r");//c
    //UARTSend0 ((BYTE*)Str, strlen (Str));
    
    // ���� �������� ����� ������� ��������� (������ 3 ���)
    // ���� ���� �������, ��������� �������������� ��� �������
    //123    
    //    if (NeedResetIM) // ����������� ������� � �������� ��������������
    //    {
    //      SetIndexIM(ShadowIndexIM);
    //      SSPInit_Any(MEM_FL1); // �������������� ������������� SSP ��� ���������� ������� FLASH (����� ��������)
    //      SpliceWRITE(250); 
    //      LogData[12] = FlashReadSpliceData(12);
    //      SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (����������� ������� ���������� )
    //      memset( RawData, 0, RAWSIZE * sizeof(DWORD) ); // �������� ������ ����������
    //      CntNumAvrg = 0; // �������� ������� ����������
    //      NeedResetIM = 0;
    //    }
    if (GetIndexVRM()==4) // ������� ~1,5 cek ... ����� ���� ����
    {
      CntNumAvrg = 0; // �������� ������� ����������
      memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
      //123      reset_timer(2); //04.02.2014
      //123      enable_timer(2);
      CurrTimeAccum = 0;
      EnaTimerAccum = 1;
      
    }
    Averaging (NumAvrg,IndxAddBad,1); //������ �������� ����������
    if ((GetCntNumAvrg() >= FinAvrg)||((CurrTimeAccum/1000)>GetTimeAvrg(GetIndexVRM()))) //��������� ���������� ������ ��������������
    {
      
      RawData[RAWSIZE-1]= CurrTimeAccum;// ���-�� ����� ��������� � ������ �������������� (����� �����)
      EnaTimerAccum = 0;
      
      HV_LOW(ON); //ON LOW HIGH VOLT
      HV_SW(OFF); // OFF HIGH VOLT
      POWDET(OFF);
      POWREF (OFF);
      // ���������� ��� ���������� ��������
      //PINSEL1 &=~0x00300000; //set P0.26 us GIO
      
      SetMode(ModeDrawOTDR);
      // ������� ���� ��������� ��������������  
      // ������� ������� ������������ ���� �� DrawOTDRview (�����)  
      CmdInitPage(18);
      ModeDevice = MODEREFL;
      ReturnModeViewRefl = SETPARAM;//VIEWMEM -  ����� ��������� � � ���������
      SubModeMeasOTDR = NOTMODE;
      GetPlaceLS(CURRENT);
      rawPressKeyS=0;
      //123      SSPInit_Any(MEM_FL1); // �������������� ������������� SSP ��� ���������� ������� FLASH (���� 1 �� ��� �� ����� ����������)
      //123      enable_timer ( 0 );
      myBeep(30);
      LED_START(0);//Off  LED
      //Light(GetTimeLight(SetIndexLight(0))); // �������� ���� ���� ���������
      //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
      //GetSetHorizontScale (5); // ��������� ��� �������� �� �����
      // ��������� ������ ���������� ... ��������� ����� (���� ����������� "������" ��������)
      // ���������� � ������� ��� �������������������� ����� ����������
      if(EnaCorrMZ)// ���� ��������� ������������� ������� ���!
      {
        // ����� ����� ����������� �������� ������� ���� �� �������, ��������� ��� ��������
        // ������ ��� (PointsPerPeriod==48), �������� ��������� 
        // ������� ������ ����� 18000 �� �� ����� 700 ( �.�. �� � �����, ����� ���� 17000)
        // ����� ������ 20-
        // ���� ���, ������� �������� 7 �����
        CntPointPick=0;
        if((GetIndexIM()==0)&&(PointsPerPeriod==48)) // ������ �� ������ ��������
        {
          for (int i=0; i<OUTSIZE; i++)
          { 
            if((LogData[i]<16000)&&(LogData[i]>3700)&&(i>20)) // ������ � ������ ���������
            {
              unsigned short aLD,bLD,cLD,dLD,eLD,fLD,gLD;
              aLD = LogData[i-6];
              bLD = LogData[i-5];
              cLD = LogData[i-4];
              dLD = LogData[i-3];// pick
              eLD = LogData[i-2];
              fLD = LogData[i-1];
              gLD = LogData[i];
              if((CntPointPick<15)&&(dLD<cLD)&&(dLD<eLD))// ���� ��������,
              {
                if((cLD<bLD)&&(eLD<fLD))// ��������� ���� �� ������ �� ��������
                {
                  if((bLD<aLD)&&(fLD<gLD))// ��������� ���� ����?
                  {
                    if(((gLD-dLD)>1000)&&((aLD-dLD)>1000))// ����� "�������" � � ���� ���� ������, ��������� ��� �������������
                    {
                      PointsPick[CntPointPick]=dLD;
                      PointsPickIndx[CntPointPick]=i-3;
                      CntPointPick++;
                      // ������������� ������� �������� �� 0.75 ��
                      LogData[i-3]-=753;
                      // ����� ����� ������� �������� 
                    }
                  }
                }
              }
            }
          }
          // ���� ����� ����� �����
          //          if(CntPointPick)
          //          {
          //            char BufOut[32];
          //            for(int j=0;j<CntPointPick;j++)
          //            {
          //              sprintf(BufOut,"%d. %d\r",PointsPickIndx[j],PointsPick[j]);//c
          //              UARTSend0 ((BYTE*)BufOut, strlen (BufOut));
          //            }
          //          }
        }
      }
      
      if (GetIndexIM()>4)
      {
        SpliceProg (PointsInImpulse(0));
      }
      TimeSaveOTDR = RTCGetTime(); // ��������� ����� �����
      SetNumAverag (FinAvrg); // ������ ����� ���������� � ������ �������������
      SaveLogNoise(); // ��������� ������� �����(����� ��������� � �� (NF)
      //123      TraceERASE(0); // �������� ������ (0) � ������ ��� ��������� ���������� ���������, ��� ��
      //123      TraceWRITE(0); // ������ ������ (0) ��� ��������� ��������� ����������� ��������� �� ���� (�������)
      
      // ��������������, � ����� ���� �� �������...
      SaveFileSD(0); // ��������� � �������
      IndexEvents=0; // ��������� �� ������� (�������������� �����)
      if (SetGetMonEna(255)) PrintEventTbl(); // ���� ���������� ������� ������ �� RS ������� �������
      // ���� ���������� ������� ��������������� ���������, ���� ��������� ��������� �� ����� ������ ����� � ��������� �������
      //if (GetSetModeLW(0)) // �������������� ����� ��������� �� ������ ���� (������)
      //
      if (g_AutoSave) //17.11.2022 �������������� ����� ���������� ��������� 
        //�� ������ ���� ��� �� �����
        
      { 
        // �������������� ��� ����� � ����������
        memcpy( FileNameOTDR, PrefixFileNm, 10 );
        sprintf(Str,"_%03d_%04d",NumFiber,GetLengthWaveLS (GetPlaceLS(CURRENT))); // ����� ������� � ������ ����� �� ������� ���������� (8 ����)
        memcpy( &FileNameOTDR[10], &Str, 9 ); // 
        memcpy( CommentsOTDR,FileNameOTDR, 20 );
        // ��������� �������������� � ������ 
        //123 !!!!        if (SaveTrace())
        SaveFileSD(1);
        if (1)
        {
          CntLS++;
          // ��������� �� ���������� �� ���� ��������� ������ ���������
          switch(GetCombLS(SettingRefl.SetModeLW))
          {
          case 1:
          case 2:
          case 4:
            SizeRepit=1;
            break;
          case 3:
            SizeRepit=2;
            SettingRefl.SW_LW = 1; // ��������� ���������� ������� �������� ������
            
            break;
          case 5:
            SizeRepit=2;
            SettingRefl.SW_LW = 2; // ��������� ���������� ������� �������� ������
            break;
          case 6:
            SizeRepit=2;
            SettingRefl.SW_LW = 2; // ��������� ���������� ������� �������� ������
            break;
          case 7:
            SizeRepit=3;
            if (CntLS<2)
              SettingRefl.SW_LW = 1; // ��������� ���������� ������� �������� ������
            else
              SettingRefl.SW_LW = 2; // ��������� ���������� ������� �������� ������
            
            break;
          default:
            SizeRepit=0;
            break;
          }
          
          // ��� �������������� ��������� ����� ��������� �� �������������� ��������
          // ��������� ����� ����� ��� ����������
          //if (++CntLS<GetSetNumLS(0)) // ��� ���� ����� ����� �� ������� ����� �������� (������)
          if (CntLS<SizeRepit) // 17.11.2022 (�����) ��� ���� ����� ����� �� ������� ����� ��������
          {
            // ����� ����� �����, ��� ������������ �� �������, ��� ��������� ���������� 
            //������ ��� ����������� ������
            //
            GetPlaceLS(CURRENT); // �������� ��������� ����� ����� �� ������ �����������
            myBeep(10);
            LSEL0(0);
            LSEL1(0);
            SlowON();
            //POWALT(ON);
            //POWREF (ON);
            //POWDET(ON);
            SetMode(ModeStartOTDR);
            ModeDevice = MODEMEASURE;
            CmdInitPage(16);// ������� ������� ������������ ���� �� ������ ��������� Measuring
            //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
            SubModeMeasOTDR = SETPOWER;
            //123 SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
          }
          else // ����� � ����� ��������� ������������� �� ��������� ����� ���������
          {
            if (NumFiber<999) NumFiber++;
            else NumFiber =1;
            CntLS = 0;// ??? ��� ��� ������� �� ���� �������� 0
            SetMode(ModeSetupOTDR);  //  ������� � ����� ��������� ������������� (����)
            ModeDevice = MODESETREFL;
            SubModeMeasOTDR = NOTMODE;
            // ������������� ������ � ����������� �� ����������
            SetPlaceNew(0); // ��� ������ ��������� ���� ����������
            
            CmdInitPage(2);// ������� ������� ������������ ���� �� OTDR (��������� �������������)
          } 
        }
        else // ��� ����� ��� ������������� - ������� � �������� � ������ �������
        {
          SetMode(ModeMemoryOTDR);
          ModeDevice = MODEMEMR;
          myBeep(10);
          ModeMemDraw = MEMFULL;
          // ������� ������� ������������ ���� �� Mem_OTDR_garaph (�����)  
          CmdInitPage(13);
          
        }
      }
      if (RemoutCtrl) // ������ ��������� ����� ���� ��������� ������������
      {
        sprintf(Str,"END\r");//c
        RemoutCtrl = 0;
        UARTSendExt ((BYTE*)Str, strlen (Str));
        ClearRS();
      }
      
    }
    break;
  }
  // ���������� �� ������ S ������� � ��������� �������������
  if (rawPressKeyS)
  { 
    myBeep(10);
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(OFF); // OFF HIGH VOLT
    POWDET(OFF);
    POWREF (OFF);
    // ���������� ��� ���������� ��������
    //PINSEL1 &=~0x00300000; //set P0.26 us GIO
    
    GetPlaceLS(CURRENT);
    rawPressKeyS=0;
    //123 SSPInit_Any(MEM_FL1); // �������������� ������������� SSP ��� ���������� ������� FLASH (���� 1 �� ��� �� ����� ����������)
//123    enable_timer ( 0 );
    myBeep(20);
    LED_START(0);//Off  LED
    //SetVerticalSize (22000);// ��������� ������������� ������� ����������� ��������������
//123    Light(GetTimeLight(SetIndexLight(0))); // �������� ���� ���� ���������- not need
    if ((SubModeMeasOTDR==AVERAGING)&&(GetSetModeLW(0)!=1))
    {
      SetMode(ModeDrawOTDR);
      ModeDevice = MODEREFL;
      SubModeMeasOTDR = NOTMODE;
      ReturnModeViewRefl = SETPARAM;//VIEWMEM -  ����� ��������� � � ���������
      
      //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
      //GetSetHorizontScale (5); // ��������� ��� �������� �� �����
      /*  ���� ���� �� ��������� ���������� ����� ����������� �� ��� ������ ��������*/
      TimeSaveOTDR = RTCGetTime(); // ��������� ����� �����
      SetNumAverag (GetCntNumAvrg()); // ������ ����� ���������� � ������ �������������
      SaveLogNoise(); // ��������� ������� �����(����� ��������� � �� (NF)
      SetHeadFileRaw (GetNumAverag()); // ����� ��������� ����� ������ ������������� ������
      
//123      TraceERASE(0); // �������� ������ (0)
//123      TraceWRITE(0); // ������ ������ (0)
      SaveFileSD(0); // ��������� ������� ������

      CmdInitPage(18);// ������� ������� ������������ ���� �� ��������� �������������� DrawOTDRView
      
    }
    else
    {
      SetMode(ModeSetupOTDR);  //  ������� � ����� ��������� ������������� (����)
      ModeDevice = MODESETREFL;
      SubModeMeasOTDR = NOTMODE;
      CmdInitPage(2);// ������� ������� ������������ ���� �� OTDR (��������� �������������)
    }
    if (RemoutCtrl) // ������ ��������� ����� ���� ��������� ������������
    {
      sprintf(Str,"END\r");//c
      RemoutCtrl = 0;
      UARTSendExt ((BYTE*)Str, strlen (Str));
    }
  }
  // ����� ������ ����, ���� ���������� (�������� ��� ������ �� �����)
  if(NeedReturn)
  {
    CmdInitPage(NeedReturn);
    NeedReturn = 0;
  }
  
  //  if (g_FirstScr)
  //  {
  //    // ����� ��������� ������� ���� ������ ����������
  //    // �� �������� ��������� ��� ��������� �������������
  //    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[1][CurrLang]);
  //    NEX_Transmit((void*)Str);    // 
  //
  //    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[1][CurrLang]);
  //    NEX_Transmit((void*)Str);    // 
  //
  //    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[32][CurrLang]);
  //    NEX_Transmit((void*)Str);    // ������� ������:
  //
  //    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[31][CurrLang]);
  //    NEX_Transmit((void*)Str);    // Range
  //
  //    sprintf(Str, "t4.txt=\"%s\"���", MsgMass[29][CurrLang]);
  //    NEX_Transmit((void*)Str);    // Pulse width
  //
  //    sprintf(Str, "t5.txt=\"%s\"���", MsgMass[30][CurrLang]);
  //    NEX_Transmit((void*)Str);    // ���� ���������
  //
  //    sprintf(Str, "t6.txt=\" \"���");
  //    NEX_Transmit((void*)Str);    // ������ ������
  //
  //    sprintf(Str, "t7.txt=\" \"���");
  //    NEX_Transmit((void*)Str);    // ������ ������
  //
  //    sprintf(Str, "t8.txt=\" \"���");
  //    NEX_Transmit((void*)Str);    // ������ ������
  //
  //    sprintf(Str, "t9.txt=\" \"���");
  //    NEX_Transmit((void*)Str);    // ������ ������
  //
  //    sprintf(Str, "t10.txt=\" \"���");
  //    NEX_Transmit((void*)Str);    // ������ ������
  //    g_FirstScr = 0;
  //    g_NeedScr = 1;
  //  }
  //  if (g_NeedScr)
  //  {
  //    // ����� ��������� ������� ���� ������ ����������
  //    // �� ����������� ��������� ��������� ������������ ����������
  //
  //    // ������������� ���� ������ 
  //    // �������� ����������  � ��������� ���������
  //    sprintf(Str, "t1.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t1.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);// 
  //    sprintf(Str, "t2.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);// 
  //    sprintf(Str, "t3.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);// 
  //    sprintf(Str, "t4.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t5.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t6.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t7.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t8.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t9.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t10.bco=WHITE���"); // �����
  //    NEX_Transmit((void*)Str);//
  //    sprintf(Str, "t%d.bco=GREEN���", SubModeMeasOTDR); // ������� - ���� ���! ���� ���������
  //    NEX_Transmit((void*)Str);// 
  //                                       // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
  //    g_NeedScr = 0;
  //  }
  
  //ClrKey (BNS_MASK); // ����� ������� ������
}

void ModeDrawOTDR(void) // ����� ����������� ��������������
{
  char Str[48];
  char Stra[24];
  unsigned InitScale; // ������� ������������� � ����������� �� ����������
  //static char ViewMode = VIEWER;//SETCSALE
  //static BYTE TypeCentral = MEDIUM;
  static BYTE NumCursors = 1;
  float fCD; // ���������� ��� ������ ��
  static float LvlLSA; // ������� ��������� ���������
  BYTE NeedCalkLSA = 0;
  unsigned CursorMain = PosCursorMain (0);
  //unsigned CursorMain = (0);
  float TmpACI;

  //float PosLine;// = ((ValueDS*LIGHTSPEED*1e-14)/GetIndexWAV())*CursorMain;
  //BYTE CurrLang;
  //CurrLang=GetLang(CURRENT);
  switch (ViewMode)
  {
  case VIEWER: // ������� ��������
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==UP_SHORT_PRESSED)) // ��������� ������ ������� ��������� 3 (UP_)
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      //      switch (NumCursors) // ����� �� ������������ �������
      //      {
      //      case 2: // ��� ������� .. slave �������
      //        NumCursors = 1;
      //        break;
      //      default :                 
      //        NumCursors = 2;
      //        NeedCalkLSA = 1;
      //        SetPosCursorSlave (CursorMain);
      //        break;
      //      }
      // � ��� ��� �� ������ 2 �������, �� ��� �� ��������� �������....
      NumCursors = 2;
      NeedCalkLSA = 1;
      SetPosCursorSlave (CursorMain);//123
      
      //TypeCentral = BYCUR;
      //ClrKey(BTN_OK);
    }
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==LONG_PRESSED)) // ������� � ����� ����
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      ViewMode = SETSCALE;
      //      if (NumCursors != 2)
      //      switch (NumCursors) // ����� �� ������������ �������
      //      {
      //      case 2:
      //        NumCursors = 1;
      //        break;
      //      default :
      //        NumCursors = 2;
      //        SetPosCursorSlave (CursorMain);
      //        break;
      //      }
    }
    // ����� ��������������� �������� ��� ������������ ������� ���� 
    //    if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==INF_PRESSED))
    //    {
    //
    //      ClrKey (BTN_DOWN);
    //    }
    // ����������� ������� � �����
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED)) 
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      PosCursorMain (GetSetHorizontScale (0));//123
      NeedCalkLSA = 1;
    }
    // ��������� ����� ������� ������
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==UP_INF_PRESSED)) 
    {
      g_NeedScr = 1; // Need reDraw Screen
      NeedCalkLSA = 1; // ��������� ����� ������� ������, ���� �����������
    }
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
    {
     PosCursorMain (GetSetHorizontScale(0)*5);//123 
      NeedCalkLSA = 1;
      g_NeedScr = 1; // ����������� ������ ! ���������! ���� ��������� ������ ��� ��� �����
    } 
    // ����������� ������� �����
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      PosCursorMain (-GetSetHorizontScale(0));//123
      NeedCalkLSA = 1;
    }
    // ��������� ����� ������� ������
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==UP_INF_PRESSED)) 
    {
      g_NeedScr = 1; // Need reDraw Screen
      NeedCalkLSA = 1; // ��������� ����� ������� ������, ���� �����������
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
    {
     PosCursorMain (-GetSetHorizontScale(0)*5);//123 
      NeedCalkLSA = 1;
      g_NeedScr = 1; // ����������� ������ ! ���������! ���� ��������� 
    }
    if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      TypeCentral = UP10PICS;
    }
    if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      TypeCentral = UP10PICS;
    }
    if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      TypeCentral = DOWN10PICS;
    }
    // ����� ������� - ����� ���� ����
    if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==LONG_PRESSED))
    {
      /*  */ 
      if (IndexVerSize==0)// ����. ������� ����� ���������� ������� �������
      {
        InitEventsTable (); // ������������� �������� �������
        SetMode(ModeEventsOTDR);
        // ������� ������� ������������ ���� �� ���������� ������� ������� (�����)  
        // ���� ������� ������� ����
        NeedReturn = 12; // ���������� �������� � ���� 12
        // CmdInitPage(12);
        // ���������� �������� �� �������������� ������
        // CalkPreEvents (RawData, PointsInImpulse(1));
        // ���� ������� - ������ � ����������� ����� ������� � ������ �����
        NumCalkEvents =  (CalkEventsKeys (LogData, PointsInImpulse(0), 1)); 
        IndexEvents = IndexEventsOld = 0;
      }
      else 
      {
        g_NeedScr = 1; // Need reDraw Screen
        TypeCentral = DOWN10PICS;
      }
    }
    break;
  case SETSCALE: // ��������� �������� 
    // ������� ����
    //  sprintf(Str,"<ZOOM>");// ������� ���������� �������������� �����
    //  putString(82,44,Str,1,0);
    // ��������� ��������������� ��������
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))//&&(getStateButtons(BTN_DOWN)==INF_PRESSED)
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSetHorizontScale (-1);//123
      //ClrKey (BTN_RIGHT);
      KeyP &=~BTN_RIGHT;
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))//&&(getStateButtons(BTN_DOWN)==INF_PRESSED)
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSetHorizontScale (1);//123
      //ClrKey (BTN_LEFT);
      KeyP &=~BTN_LEFT;
      
    }
    // ������������ ������������� ��������
    if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      if (IndexVerSize < PNTVERTICALSIZE-1)IndexVerSize++;
      else IndexVerSize = PNTVERTICALSIZE-1;
      TypeCentral = BYCUR;
      //ClrKey (BTN_UP);
    }
    if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      if (IndexVerSize > 0)IndexVerSize--;
      else IndexVerSize = 0;
      TypeCentral = BYCUR;
      if (IndexVerSize == 0)TypeCentral = MEDIUM; // ��� �������� � ���� �������� �������� �� ��������
      //ClrKey (BTN_DOWN);
    }
    
    if (((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==LONG_PRESSED))||((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==LONG_PRESSED))) // ������� � ������ �������
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSetHorizontScale (5);//123
      IndexVerSize = 0;
      TypeCentral = MEDIUM;
      ViewMode = VIEWER;
      
    }
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==UP_SHORT_PRESSED)) // ������� � ����� ���������
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      //TypeCentral = MEDIUM;
      ViewMode = VIEWER;
    }
    break;
    
  } // �� ������������� ������� ���������
  // ������������ ��������� �����������
  GraphParams params = {GetVerticalSize(IndexVerSize),CursorMain,GetSetHorizontScale(0),NumCursors, TypeCentral};//PosCursorMain (0)//123
  //GraphParams params = {22000,CursorMain,1<<3,NumCursors, TypeCentral};//PosCursorMain (0)
  //UserParams = {GetVerticalSize(IndexVerSize),CursorMain,GetSetHorizontScale(0),NumCursors, TypeCentral};//PosCursorMain (0)
  
  Rect rct={0,0,128,56};
  // ������ �����������
  
  // �������� ������ � ����� ���������(������ ��� ��������� � ����������)
  // �������� ������ ����  � ���������� ���� ����
  // ��������� ����� ��������� ���� � ������ ��� � �������� �� ���������� ����
  if(g_FirstScr)
  {
    
    //sprintf( Str,"xstr 0,208,27,16,3,RED,WHITE,0,1,1,\"A:\"���"); // 0
    //NEX_Transmit((void*)Str);// 
    //sprintf( Str,"xstr 0,224,27,16,3,BLUE,WHITE,0,1,1,\"B:\"���"); // 0
    //NEX_Transmit((void*)Str);// 
    sprintf(Str,"t4.txt=\"%s\"���", MsgMass[118][CurrLang]);// Slope ������
    NEX_Transmit((void*)Str);// 
    
    g_NeedScr = 1; // ��� ������ ���������� ��������
    g_FirstScr = 0;
    NeedCalkLSA = 1;// ���� ����������� ������!
  }
  // ����� ����� ������� ������!
    // ���� ��� ������� � ������� ����������� ������� ���� �� ������� LSA
    //if ((NeedCalkLSA)&&(NumCursors == 2))
    // ���� ������� ����������� ������� ���� �� ������� LSA
    if (NeedCalkLSA)
    {
      // ������ ��������� ���������, ���� �������� �������
      NeedCalkLSA = 0;
      unsigned ptA = PosCursorMain (0);//123!!!
      unsigned ptB = PosCursorSlave (0);//123!!!
      //unsigned ptA = 0;
      //unsigned ptB = 128;
      if(ptA!=ptB)
      {
      TmpACI = (GetPosLine(ptA)-GetPosLine(ptB)); // ���������� ����� �������
      LvlLSA = (LogData[ptA]-LogData[ptB])/(1000.0*TmpACI);
      }
      else
        LvlLSA  = 0.0;
      //LvlLSA = Calc_LSA(PosCursorMain (0),PosCursorSlave (0));
    }
  
  // ���� ��� �� �������� � ����� ��������� 
  if(g_NeedScr)
  {
    
    // ������������ ��������� �����������
    CursorMain = PosCursorMain (0); //123!!!// ������������� ������ ����� ���������
    //CursorMain =  (0); // ������������� ������ ����� ���������
    // ������ ���������� �������, � �������, ���� ������� ���� �������� ��������!?
    GraphParams paramsN = {GetVerticalSize(IndexVerSize),CursorMain,GetSetHorizontScale(0),NumCursors, TypeCentral};//123!!!//PosCursorMain (0)
    //GraphParams paramsN = {22000,CursorMain,1<<3,NumCursors, TypeCentral};//PosCursorMain (0)
    // ����������(����������) ������ ��� ������ ���������� ����� ��������� ������
    // ����� ���� �������������� ��������� ��� ������ ������������ ����������� 400*240(3.2") ��� 480*320(3.5")
    if(TypeLCD)
    {
      rct.right=480;
      rct.bottom=255;// ����� ���� ������� 255, � ���������� ������� 270/255 1.058
      InitScale = 8; // 4608/InitScale>480
      // ��������� �������
      CursorScale = 1.06;
    }
    else
    {
      rct.right=400;
      rct.bottom=210;
      InitScale = 10; // 4608/InitScale>480
      CursorScale = 1.0;

    }
    
    rct.left=0;
    rct.top=0;
    paramsN.scale=(paramsN.scale>=16)?(InitScale):(paramsN.scale);
    unsigned CurrSM = MakeGraphNext( &rct, LogData, POINTSIZE, &paramsN );// ��� ������������� ������ ��� ������� (��� �����) � ����������� �� ����������
    // ������ �������� ������ � ������ Y=0..210
    CurrRed = (CurrSM & 0xffff);
    CurrBlue = (CurrSM>>16);
    //CreatDelay (20000);
      HAL_Delay(2);

    SendDrawNex(NexData,2, rct.right);
    
    //CreatDelay (30000); // �� ������� ���������� � 30000 "�������"
      HAL_Delay(3);
// Main Cursor
    if (GetIndexLN()) // �� 2 �� ( ������ � ��)
    {
      if (GetIndexLN()==6) // 128 km? 2 sign
        sprintf(Str,"t0.txt=\"A:%6.2f%s\"���", GetPosLine(CursorMain) ,MsgMass[20][CurrLang]);//��
      else
        sprintf(Str,"t0.txt=\"A:%6.3f%s\"���", GetPosLine(CursorMain) ,MsgMass[20][CurrLang]);//��
    }
    else
      sprintf(Str,"t0.txt=\"A:%.1f%s\"���", GetPosLine(CursorMain)*1000.0 ,MsgMass[78][CurrLang]);//� - �����
    NEX_Transmit((void*)Str);// 
    // Slave Cursor  
    if (GetIndexLN()) // �� 2 �� ( ������ � ��)
    {
      if (GetIndexLN()==6) // 128 km? 2 sign
        sprintf(Str,"t1.txt=\"B:%6.2f%s\"���", GetPosLine(PosCursorSlave (0)) ,MsgMass[20][CurrLang]);//��
      else
        sprintf(Str,"t1.txt=\"B:%6.3f%s\"���", GetPosLine(PosCursorSlave (0)) ,MsgMass[20][CurrLang]);//��
    }
    else
      sprintf(Str,"t1.txt=\"B:%.1f%s\"���", GetPosLine(PosCursorSlave (0))*1000.0 ,MsgMass[78][CurrLang]);//� - �����
    NEX_Transmit((void*)Str);// 
    
    if (GetIndexLN()) // �� 2 �� ( ������ � ��)
    {
      if (GetIndexLN()==6) // 128 km? 2 sign
        sprintf(Str,"t2.txt=\"A-B:%.2f%s\"���",fabs(GetPosLine(CursorMain)-GetPosLine(PosCursorSlave (0))) ,MsgMass[20][CurrLang]);//��
      else
        sprintf(Str,"t2.txt=\"A-B:%.3f%s\"���",fabs(GetPosLine(CursorMain)-GetPosLine(PosCursorSlave (0))) ,MsgMass[20][CurrLang]);//��
    }
    else
      sprintf(Str,"t2.txt=\"A-B:%.1f%s\"���",fabs((GetPosLine(CursorMain)-GetPosLine(PosCursorSlave (0)))*1000.0) ,MsgMass[78][CurrLang]);//� - �����
    NEX_Transmit((void*)Str);// 
    // ������� �� ����� ���������
    if ((int)(PosCursorSlave(0) - PosCursorMain(0))>0)
    {
      fCD=((unsigned short)(GetLevelCurs(1)>>16)-(unsigned short)(GetLevelCurs(1)&0xFFFF))/1000.0;
      if((fCD>-10.0)&&(fCD<10.0))
        sprintf(Str,"t3.txt=\"L:%.3f%s\"���",fCD,MsgMass[47][CurrLang]);//��
      else
        sprintf(Str,"t3.txt=\"L:%.2f%s\"���",fCD,MsgMass[47][CurrLang]);//��
      
    }
    else
    {
      fCD= ((unsigned short)(GetLevelCurs(1)&0xFFFF)-(unsigned short)(GetLevelCurs(1)>>16))/1000.0;
      if((fCD>-10.0)&&(fCD<10.0))
        sprintf(Str,"t3.txt=\"L:%.3f%s\"���",fCD,MsgMass[47][CurrLang]);//��
      else
        sprintf(Str,"t3.txt=\"L:%.2f%s\"���",fCD,MsgMass[47][CurrLang]);//��
    }
    NEX_Transmit((void*)Str);// 
    // ������ � ��������� ��������� ���������
//    if(LvlLSA<0.0)
//      sprintf(Str,"t5.txt=\"<0 %s/%s\"���",MsgMass[47][CurrLang],MsgMass[20][CurrLang]);//��/��
//    else if(LvlLSA<10.0)
    if((LvlLSA<10.0)&&(LvlLSA>-10.0)) // ��� �����
      sprintf(Str,"t5.txt=\"%.3f%s/%s\"���",LvlLSA,MsgMass[47][CurrLang],MsgMass[20][CurrLang]);//��/��
    else if((LvlLSA<=30.0)&&(LvlLSA>-30.0))
      sprintf(Str,"t5.txt=\"%.2f%s/%s\"���",LvlLSA,MsgMass[47][CurrLang],MsgMass[20][CurrLang]);//��/��
    else 
      sprintf(Str,"t5.txt=\">< %s/%s\"���",MsgMass[47][CurrLang],MsgMass[20][CurrLang]);//��/��
    
    NEX_Transmit((void*)Str);// 
    
    // �������, ��������� ���������� ��������� ���� ��� ������
    
    if(CurrBlue==CurrRed)
    {
      sprintf( Str,"line %d,0,%d,%d,63519���",CurrRed,CurrRed,(int)(rct.bottom*CursorScale)-1); // 0
      NEX_Transmit((void*)Str);// 
    }
    else
    {
      sprintf( Str,"line %d,0,%d,%d,RED���",CurrRed,CurrRed,(int)(rct.bottom*CursorScale)-1); // 0
      NEX_Transmit((void*)Str);// 
      //CreatDelay (20000);
      HAL_Delay(2);
      if(CurrBlue<rct.right)
      {
        sprintf( Str,"line %d,0,%d,%d,BLUE���",CurrBlue,CurrBlue,(int)(rct.bottom*CursorScale)-1); // 0
      }
      else
      {
        if(CurrBlue==rct.right) // ������
        {
          if(TypeLCD)
            sprintf( Str,"xstr 440,192,26,40,2,BLUE,BLACK,0,1,3,\">\"���"); // 03_5(2-40) (� ������-������) 0
          else
            sprintf( Str,"xstr 387,192,16,24,2,BLUE,BLACK,0,1,3,\">\"���"); // 03_2(2-24) (� ������-������) 0
        }
        else
        {
          if(TypeLCD)
            sprintf( Str,"xstr 1,192,26,40,2,BLUE,BLACK,0,1,3,\"<\"���"); // 03_5(2-40) (� ������-������) 0
          else
            sprintf( Str,"xstr 1,192,16,24,2,BLUE,BLACK,0,1,3,\"<\"���"); // 03_2(2-24) (� ������-������) 0
        }
      }
      NEX_Transmit((void*)Str);// 
      
    }
    // ������ ������� ��������� ������� (���� ��� 1 ��� 2)
    //   if(NumCursors==1)
    //        sprintf( Str,"xstr 387,0,12,16,3,RED,BLACK,0,1,1,\"A\"���"); // 0
    //    else 
    //        sprintf( Str,"xstr 387,0,12,16,3,BLUE,BLACK,0,1,1,\"B\"���"); // 0
    //    NEX_Transmit((void*)Str);
    // ��������� ���������
    sprintf(Stra,"Y-%.1f%s/div",(float)GetVerticalSize(IndexVerSize)/7000.0,MsgMass[47][1] );//��
    
    if(TypeLCD)
      sprintf( Str,"xstr 360,0,120,24,3,GREEN,BLACK,0,1,3,\"%s\"���",Stra); // 3_5(3-24) (� ������-������) 0
    else
      sprintf( Str,"xstr 260,0,140,24,2,GREEN,BLACK,0,1,3,\"%s\"���",Stra); // 3_2(2-24) (� ������-������) 0
    NEX_Transmit((void*)Str);// 
    //CreatDelay (20000);
      HAL_Delay(2);

    sprintf(Stra,"X-1:%d",GetSetHorizontScale(0));// ������� ���������� �������������� �����
    if(TypeLCD)
      sprintf( Str,"xstr 360,24,120,24,3,GREEN,BLACK,0,1,3,\"%s\"���",Stra); // 3_5(24)  0
    else
      sprintf( Str,"xstr 260,16,140,24,2,GREEN,BLACK,0,1,3,\"%s\"���",Stra); // 3_2(24)  0
    NEX_Transmit((void*)Str);// 
    
    // ���� ���, ��������� ���� �� ������
    if(ViewMode == SETSCALE)
    {
      sprintf(Stra,"<ZOOM>");// ������� ���������� �������������� �����
      //sprintf(Stra,"<Z1:%d>",GetSetHorizontScale(0));// ������� ���������� �������������� �����
      if(TypeLCD)
        sprintf( Str,"xstr 240,0,100,40,3,2047,BLACK,0,1,3,\"%s\"���",Stra); //3_5(40) 0 ��������� ���� ��� ����������, 0- �������, 1- ������� , 2 -��������, 3 -��� ����������
      else
        sprintf( Str,"xstr 160,0,100,24,2,2047,BLACK,0,1,3,\"%s\"���",Stra); //3_2(24) 0 ��������� ���� ��� ����������, 0- �������, 1- ������� , 2 -��������, 3 -��� ����������
      NEX_Transmit((void*)Str);// 
    }
    
    g_NeedScr = 0; 
  }
  
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    switch (ReturnModeViewRefl)
    {
    case VIEWMEM:
      SetMode(ModeMemoryOTDR);
      ModeDevice = MODEMEMR;
      ModeMemDraw = VIEWNEXT;
      // ������� ������� ������������ ���� �� Mem_OTDR_graph (�������)  
      CmdInitPage(13);
      //���� ����� ��� �� ������������
      //CreatDelay(1000000);
        HAL_Delay(100);

      break;
    case VIEWEVNT:
      SetMode(ModeEventsOTDR);
      // ������� ������� ������������ ���� �� Event_window (�������)  
      CmdInitPage(12);
      break;
    default:
      SetMode(ModeSetupOTDR);
      GetSetModeLW(-1); // ����� ��������, ��� ��� �� ��������� 
      ClrKey (BTN_MENU);
      ModeDevice = MODESETREFL;
      ViewMode = VIEWER; // ���� ���� ���� �� ��������� ���
      // ������� ������� ������������ ���� �� OTDR (�������)  
      CmdInitPage(2);
      break;
    }
  }
  //ClrKey (BNS_MASK);
  if (rawPressKeyS)// ������� � �������� ������������ ����������
  { 
    myBeep(10);
    SetMode(ModeKeyBoardOTDR);
    ModeDevice = MODEOTHER;
    myBeep(10);
    CheckCommOTDR ();// �������� � ������������� ������ ������������ OTDR

    // ���������� ������ ����������� ��� ��������
    IndexCommOTDR = 0;
    CommentsOTDR[ARRAY_SIZE(CommentsOTDR)-1]=0; // ��������� ������� � ������� ����� 0
    for (int Ind =ARRAY_SIZE(CommentsOTDR)-2; Ind>=0; Ind--) // ���� � ����������
    {
      if (CommentsOTDR[Ind]<0x20) CommentsOTDR[Ind]=' '; //���� �����������, �� ������ �� "��������"
      else if (CommentsOTDR[Ind]!=' ' && IndexCommOTDR == 0)IndexCommOTDR = Ind; // ��������� ����� ������ �� ������� ��"������"
      //Index_Comm --;
    }
    if ((CommentsOTDR[IndexCommOTDR]!=' ')&&(IndexCommOTDR!=18))IndexCommOTDR ++;// ���� ��������� �� �� ������ � ��� �� ���������, �� ����������� ���������
    KbPosX = 11;
    KbPosY = 2;
    // ���� ����� ������� ����� �������� ����������
    //        POWALT(ON);
    //        POWREF (ON);
    //        POWDET(ON);
    //        Light(0); // ��������� ���������
    //        SetMode(ModeStartOTDR);
    //        ModeDevice = MODEMEASURE;
    //        //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
    //        SubModeMeasOTDR = SETPOWER;
    //        SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
    //        rawPressKeyS=0;
    // ������� ������� ������������ ���� �� ���������� �������������� Keyboard (�����)  OTDR
    if(CurrLang)// �� �������
    {
      CmdInitPage(22);
    }
    else
    {
      CmdInitPage(23);
    }
    
    
  }
  // ����� ������ ����, ���� ����������
  if(NeedReturn)
  {
    CmdInitPage(NeedReturn);
    NeedReturn = 0;
  }
  
  //SetVerticalSize(VerticalSize[IndexVerSize]);
}


void ModeEventsOTDR(void) // ����� ����������� ������� �������������� CHECK_OFF
{
  char Str[32];
  char s_orl[16];
  //Rect rct_pic={77,17,109,27};
  int type_pic=0;
  //static unsigned short IndexEventsOld = 0; // ��������� �� ������� ��� ������������ ��������������� ����������

  if (IndexEvents == 0) // ������ ��������� �������������
  {
  }
  else
  {
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==UP_SHORT_PRESSED)) // ������� � ����� ��������� � ������������� ����
    {
      myBeep(10);
      GetSetHorizontScale (-6); // �������� �������
      GetSetHorizontScale (0); // ������������� ������� = 4
      //ClrKey (BTN_LEFT);
      // ������������ ������������� ��������
      // if (IndexVerSize < PNTVERTICALSIZE-1)IndexVerSize++;
      // else IndexVerSize = PNTVERTICALSIZE-1;
      IndexVerSize=1;
      SetPosCursorMain (EvenTrace[IndexEvents-1].EPT);     
      TypeCentral = MEDIUM;
      ViewMode = VIEWER;
      SetMode(ModeDrawOTDR);
      // � 22.11.2022 ���� ������������ ������� ��������
      ReturnModeViewRefl = VIEWEVNT;//VIEWMEM -  ����� ��������� � � ���������
      KeyP &=~BTN_OK;
    // ������� ������� ������������ ���� �� DrawOTDRview (�������)  
      // ����� ������� 
      // ���� ������� ������� ����
      NeedReturn = 18; // ���������� �������� � ���� 18
      //CmdInitPage(18);
      
    }
    
    
  }
  // ����������� ������� � �����
  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED)) 
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    if (IndexEvents<NumCalkEvents)IndexEvents++;
    else 
      IndexEvents=0; 
    if((IndexEvents!=0) && (IndexEventsOld==0))g_FirstScr=1;
    if((IndexEvents==0) && (IndexEventsOld!=0))g_FirstScr=1;
    IndexEventsOld = IndexEvents;
  }
  // ����������� ������� �����
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    if (IndexEvents>0)IndexEvents--;
    else IndexEvents=NumCalkEvents; 
    if((IndexEvents!=0) && (IndexEventsOld==0))g_FirstScr=1;
    if((IndexEvents==0) && (IndexEventsOld!=0))g_FirstScr=1;
    IndexEventsOld = IndexEvents;
  }
  // �������� ��������� IndexEvents
  
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"< %s >\"���", MsgMass[83][CurrLang]); // < ������e >
    NEX_Transmit((void*)Str);    //
    // ����� � ����������� �� ���������
    if(IndexEvents) // ���������� �������
    {
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[84][CurrLang]); //Type Event
    NEX_Transmit((void*)Str);    // 
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[85][CurrLang]); // ���������
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[86][CurrLang]); //���������
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t8.txt=\"%s\"���", MsgMass[87][CurrLang]); // ���������
    NEX_Transmit((void*)Str);    //
    }
    else // ��������� ������� ������� (��������� �����)
    {
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[31][CurrLang]); //������ �����
    NEX_Transmit((void*)Str);    // 
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[93][CurrLang]); // ������ ������
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t3.txt=\"ORL\"���" ); //ORL, ���� ����������! 
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t8.txt=\" \"���" ); //Empty str
    NEX_Transmit((void*)Str);    //
      
    }
       



    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
    // ������� ��������� �������
    sprintf(Str, "t7.txt=\"%d/%d \"���", IndexEvents,NumCalkEvents); //������� �������
    NEX_Transmit((void*)Str);    //
    // ����� � ����������� �� ���������
    type_pic = 0;
          sprintf(Str,"p0.pic=%1d���",type_pic);//�������� ��������
      //sprintf(Str,"pic 330,70,%1d���",type_pic);//��� ��������
      NEX_Transmit((void*)Str);    // 
      //CreatDelay(5000);
  HAL_Delay(1);

    if(IndexEvents) // ���������� �������
    {
      // ������� ���������� ���� ������� ���� ������ �����!
      //sprintf(Str,"t4.txt=\"%c%c\"���",EvenTrace[IndexEvents-1].EC[1],EvenTrace[IndexEvents-1].EC[0]);// event Code
      // ��������� ��������� ������� ��� ������ � ���� �4 �������� ��� ��������?
          switch (EvenTrace[IndexEvents-1].EC[1])
    {
    case 'F': // 
      switch (EvenTrace[IndexEvents-1].EC[0])
      {
      case '0':
      sprintf(s_orl,"nRef");//N - ������������ �������
      type_pic = 1;
        break;
      case '1':
      sprintf(s_orl,"Ref");//R - ���������� �������
      type_pic = 2;
        break;
      case '2':
      sprintf(s_orl,"Ref");//S - ������ �����
        break;
    default:
    sprintf(s_orl,"%s ",EvenTrace[IndexEvents-1].COMM_EVN);//�����������
      break;
        
      }
      break;
    case 'E':
      switch (EvenTrace[IndexEvents-1].EC[0])
      {
      case '0':
      sprintf(s_orl,"EOFn");//E - ����� ����� ��� ���������
      type_pic = 4;
        break;
      case '2':
      sprintf(s_orl,"EOFr");//E - ����� ����� ��� ���������� �� ����������� �������
      type_pic = 5;
        break;
    default:
    sprintf(s_orl,"%s ",EvenTrace[IndexEvents-1].COMM_EVN);//�����������
      break;
      }
      break;
    case 'O':
      switch (EvenTrace[IndexEvents-1].EC[0])
      {
      case '0':
      sprintf(s_orl,"ENF");//E - ����� ����� ��� ���������
      type_pic = 6;
        break;
//      case '2':
//      DrawPicture (&rct_pic, BigEnd);
//      sprintf(Str,"E");//E - ����� ����� ��� ���������� �� ����������� �������
//      putString(120,18,Str,1,0);
//        break;
    default:
    sprintf(s_orl,"%s ",EvenTrace[IndexEvents-1].COMM_EVN);//�����������
      break;
      }
      break;
    default:
    sprintf(s_orl,"%s ",EvenTrace[IndexEvents-1].COMM_EVN);//�����������
      break;
    }
    // ������� ����� ����� ��������
      sprintf(Str,"t4.txt=\"%s\"���",s_orl);//��
      NEX_Transmit((void*)Str);    // 
      if(type_pic)
      {
      //CreatDelay(50000);
  HAL_Delay(5);
      // � ������ ��������
      sprintf(Str,"p0.pic=%1d���",type_pic);//��� ��������
      //sprintf(Str,"pic 330,70,%1d���",type_pic);//��� ��������
      NEX_Transmit((void*)Str);    // 
      //CreatDelay(50000);
  HAL_Delay(5);
      // ����� ���� ��������!
      }
      // ����� ������ �����
      if (GetIndexLN()) // �� 2 �� ( ������ � ��)
        sprintf(Str,"t5.txt=\"%.3f%s\"���",fabs(GetPosLine(EvenTrace[IndexEvents-1].EPT)) ,MsgMass[20][CurrLang]);//��
      else
        sprintf(Str,"t5.txt=\"%.1f%s\"���",fabs((GetPosLine(EvenTrace[IndexEvents-1].EPT))*1000.0) ,MsgMass[78][CurrLang]);//� - �����
      NEX_Transmit((void*)Str);    // 
      // ���������
      sprintf(Str, "t6.txt=\"%.2f%s\"���",(EvenTrace[IndexEvents-1].EL)/1000.0,MsgMass[47][CurrLang]); // ��
      NEX_Transmit((void*)Str);    //
      // ���������
      // 16/04/2015 �������� ��� ������������ �������
      if (EvenTrace[IndexEvents-1].ER !=0)
      {
        sprintf(Str, "t9.txt=\"%.2f%s\"���",EvenTrace[IndexEvents-1].ER/1000.0,MsgMass[47][CurrLang]); // ��
      }
      else
      {
        sprintf(Str, "t9.txt=\"------\"���"); // ��
      }
      NEX_Transmit((void*)Str);    //
      
    }
    else // ��������� ������� ������� (��������� �����)
    {
      // �������� ������� ������ ��������
     // sprintf(Str,"pic 330,70,0���");//������ ��������
     //NEX_Transmit((void*)Str);    // 
      // ����� ���� ��������!
     // CreatDelay(1000);
    if (GetIndexLN()) // �� 2 �� ( ������ � ��)
      sprintf(Str,"t4.txt=\"%.3f%s\"���",fabs(GetPosLine(EndEvenBlk.ELMP[1])) ,MsgMass[20][CurrLang]);//��
    else
      sprintf(Str,"t4.txt=\"%.1f%s\"���",fabs((GetPosLine(EndEvenBlk.ELMP[1]))*1000.0) ,MsgMass[78][CurrLang]);//� - �����
    NEX_Transmit((void*)Str);    // 
    sprintf(Str, "t5.txt=\"%.2f%s\"���", (LogData[EndEvenBlk.ELMP[1]]-LogData[EndEvenBlk.ELMP[0]])/1000.0,MsgMass[47][CurrLang]); // ��
    NEX_Transmit((void*)Str);    //
    if(g_VolORL!=0)sprintf(s_orl,"%.1f",g_VolORL);
    else sprintf(s_orl,"???");
      sprintf(Str,"t6.txt=\"%s%s\"���",s_orl,MsgMass[47][CurrLang]);////ORL, ���� ����������!
    NEX_Transmit((void*)Str);    // 
    sprintf(Str, "t9.txt=\" \"���" ); //Empty str
    NEX_Transmit((void*)Str);    //
    }
    
                                           // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }

  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    SetMode(ModeDrawOTDR);
// 22.11.2022 ���� �������� ���� ������ ������ �� ����� ����������  ReturnMemView
    if(ReturnMemView)
      ReturnModeViewRefl = VIEWMEM;//SETPARAM -  ����� ��������� � ������������
      else
      ReturnModeViewRefl = SETPARAM;//-  ����� ��������� � ������������
    myBeep(10);
    // ������� ������� ������������ ���� �� DrawOTDRview (�������)  
      CmdInitPage(18);
  }
  // ����� ������ ����, ���� ����������
  if(NeedReturn)
  {
   CmdInitPage(NeedReturn);
   NeedReturn = 0;
  }
  //ClrKey (BNS_MASK); // ����� ������� ������
  
}

void ModeKeyBoardOTDR(void) // ����� ����������� ���������� ��������� ������������ ��������������
{
  char Str[32];
  char StrI[32];
  //BYTE CurrLang=GetLang(CURRENT);
  //static BYTE Shift = 0; // ������� 
  BYTE NeedSaveTr = 0;
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
  SetIndexCommOTDR (IndexCommOTDR);
    
    // ���� �������� �� ���������� �������....
    memcpy(StrI,CommentsOTDR,IndexCommOTDR+1); 
    StrI[IndexCommOTDR+1]=0;// No more 20 byte size
    sprintf(Str, "t1.txt=\"%s\"���",StrI); 
    NEX_Transmit((void*)Str);    // 1 ������ �����������
    g_GetStr=1; // ������� �������  ������������� ��������� �������! �� ����������.
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
  sprintf(Str,"%02d%02d%02d_%02d%02d%01d.sor",TimeSaveOTDR.RTC_Year%100,
          TimeSaveOTDR.RTC_Mon,
          TimeSaveOTDR.RTC_Mday,
          TimeSaveOTDR.RTC_Hour,
          TimeSaveOTDR.RTC_Min,
          TimeSaveOTDR.RTC_Sec/10 );
    sprintf(StrI, "t0.txt=\"%s\"���", Str);
    NEX_Transmit((void*)StrI);    // Date/Time ������
    g_NeedScr=0;
  }
    if(g_GetStr==2) // ���! ��� �� ������� �����, ����� ���������� � �������� �������
  {
    // ����� ���������� ������ �� ������
    IndexCommOTDR = GetStringNEX(CommentsOTDR, ARRAY_SIZE(CommentsOTDR));
    //memcpy(CommentsOLT,&RX_BufNEX[1],(CntRXNEX<18)?(CntRXNEX-2):(15)); 
    //CommentsOLT[15]=0;
    for(int i=IndexCommOTDR; i<ARRAY_SIZE(CommentsOTDR); i++) CommentsOTDR[i]=' ';
    CommentsOTDR[ARRAY_SIZE(CommentsOTDR)-1]=0;
    // �������� �����������
   memcpy(NameDB.UserComm,CommentsOTDR,20); 
    //for(int i=0; i<20; i++) 
    //NameDB.UserComm[i]=CommentsOTDR[i];

    WriteNeedStruct(0x10);
  
    //CommentsOTDR[ARRAY_SIZE(CommentsOTDR)-1]=0xd;
    //    memcpy (PONI.CommUserPM, CommentsOLT,16);
    //    RX_BufNEX[31]=0xd;
    //    UARTSend0 ((BYTE*)RX_BufNEX, 32);
    //    UARTSend0 ((BYTE*)CommentsOTDR, strlen (CommentsOTDR));

    NeedSaveTr=1; // ���� ����������
  }
  // ���������� �� ������ OK �� ���������� ������� (�����)
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
    // ��������� ������ ��
  {
    myBeep(7);
    //  sprintf(Str, "click brok,1���"); // ���� ������ �� �� ����������
    //NEX_Transmit((void*)Str);    //
      sprintf(Str, "click bok,1���"); // ���� ������ �� �� ���������� ENGLISH
    NEX_Transmit((void*)Str);    //
    // ����� ������� �������� ����� �� ����� 2 ��
    // StartRecievNEX (10);// ����� �������� ������ ������� �� ����������

  }
  // ����� ��� ����������
  if (((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))||(NeedReturn))
  {
    SetMode(ModeDrawOTDR);
    ReturnModeViewRefl = SETPARAM;//VIEWMEM -  ����� ��������� � � ���������
    ModeDevice = MODEREFL;
    myBeep(10);
    // ������� ������� ������������ ���� �� DrawOTDRview (�������)  
      CmdInitPage(18);
      NeedReturn = 0;
  }
  if ((rawPressKeyS)||(NeedSaveTr))// ���������� ���� � ��������� �� ������ S  ����� ������ ������
  {
    if ((!((KbPosY == 2)&&(KbPosX == 11)))||(NeedSaveTr))  SaveNewOTDRTrace (0);
    rawPressKeyS = 0;
    NeedSaveTr = 0;
  }
}

// �������� ������ SD Card ��� ������ ����������, ���������� ������
// "����������" ����������, ������������� ������� ���� ��� �� ����������,
// ���� �� ��������� � ����������� ��������� ���������� � ������
// �������� ���� 33
void ModeFileMngDir(void) // ����� ���� ��������� ����������
{
  char Str[32];
  
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED)) 
  {
    myBeep(10);
    if(IndexNameDir>0)IndexNameDir--;
    g_NeedScr=1;
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    if((IndexNameDir+1)<NumNameDir)IndexNameDir++;
    g_NeedScr=1;
  }
  if (g_FirstScr)
  {
    SDMMC_SDCard_DIR(); // ��������� �����������
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"0:/_OTDR\"���"); // < ������e >
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t14.txt=\"%d\"���", NumNameDir); // < ������� ����� ����� >
    NEX_Transmit((void*)Str);    //
    
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    sprintf(Str, "t13.txt=\"%d\"���", IndexNameDir+1); // < ����� ����� ������� >
    NEX_Transmit((void*)Str);    //
    if (IndexNameDir > NumNameDir) IndexNameDir = 0; 
    // ��� ����� ������� ������� ��������� �� ����� � ���������
    // �� ���������� ������� ������� ������ ����� ������ ���� ������
    // ����� �����,� ������������� ������ ������ ��������������� � ������������ � 
    // ������� �������� ��������� �����
    IndexLCDNameDir = IndexNameDir%12; // ��� ��� � ��� 12 �����
    PageDir = IndexNameDir/12; // ������� �������� ������� ���������� ������ ����� ����������
    // �������� ���� ���������� ������� ����������
    for (int i=0; i<12; i++)
    {
      
      sprintf(Str, "t%d.txt=\"%s\"���",i+1 ,NameDir[PageDir*12+i]); // < ������e >
      NEX_Transmit((void*)Str);    //
      
    }
    for (int i=0; i<12; i++)
    {
      // �������� ����������  � ��������� ���������
      sprintf(Str,"t%d.bco=WHITE���",i+1); // �����
      NEX_Transmit((void*)Str);// 
    }
    sprintf(Str,"t%d.bco=GREEN���",IndexLCDNameDir+1); // GREEN
    NEX_Transmit((void*)Str);    //
    // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }
  // ��������� ������ "OK"
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==UP_SHORT_PRESSED)) // ������� � ����� ��������� � ������������� ����
  {
    myBeep(10);
      SetMode(ModeFileMngFiles);
      ModeDevice = MODEMEMR;
      ModeMemDraw = VIEWNEXT;
      ReturnMemView = 1; // ���� ��������� ���� �� �� ESC
         // ������� ������� ������������ ���� �� Mem_OTDR_garaph (�����)  
      //KeyP = 0;
      ClrKey(BTN_OK);
      CmdInitPage(34); // ����� ���� ���� ���� ������� ����������
       //CreatDelay(1000000);
      HAL_Delay(100);
  }

  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    // ����� ��� �������� � ����������� �� �������� ������ ������
    if(ReturnMemView)
    {
      //DeleteTrace = 0;
      SetMode(ModeSelectMEM);
      ModeDevice = MODEOTHER;
      myBeep(10);
      // ������� ������� ������������ ���� �� Memory (�������)  
      CmdInitPage(4);
    }
  }
}

// �������� ������ SD Card ��� ������ ������ , ���������� ������
// "����������" ������, ������������� ������� ���� ��� �� ����������,
// ���� �� ��������� � ����������� ��������� ���������� � ������
// �������� ���� 34
void ModeFileMngFiles(void) // ����� ���� ��������� ������
{
  char Str[32];
  char FilPath[64];
  uint32_t BlkSz; // ������ ����� ���������
  UINT RWC;
  FATFS FatFs;

  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==UP_SHORT_PRESSED)) // ������� � ����� ��������� � ������������� ����
  {
    myBeep(10);
  }
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED)) 
  {
    myBeep(10);
    if(IndexNameFiles>0)IndexNameFiles--;
    g_NeedScr=1;
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    if((IndexNameFiles+1)<NumNameFiles)IndexNameFiles++;
    g_NeedScr=1;
  }
  if (g_FirstScr)
  {
    SDMMC_SDCard_FILES(); // ��������� �����, 
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"0:/_OTDR/%s\"���",NameDir[IndexNameDir]); // < ������e >
    NEX_Transmit((void*)Str);    //
    sprintf(Str, "t14.txt=\"%d\"���", NumNameFiles); // < ������� ������ ����� >
    NEX_Transmit((void*)Str);    //
    
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    sprintf(Str, "t13.txt=\"%d\"���", IndexNameFiles+1); // < ����� ���� ������ >
    NEX_Transmit((void*)Str);    //
    if (IndexNameFiles > NumNameFiles) IndexNameFiles = 0; 
    // ��� ����� ������� ������� ��������� �� ����� � ���������
    // �� ���������� ������� ������� ������ ����� ������ ���� ������
    // ����� �����,� ������������� ������ ������ ��������������� � ������������ � 
    // ������� �������� ��������� �����
    IndexLCDNameFiles = IndexNameFiles%12; // ��� ��� � ��� 12 �����
    PageFiles = IndexNameFiles/12; // ������� �������� ������� ���������� ������ ����� ����������
    // �������� ���� ���������� ������� ����������
    for (int i=0; i<12; i++)
    {
      
      sprintf(Str, "t%d.txt=\"%s\"���",i+1 ,NameFiles[PageFiles*12+i]); // < ������e >
      NEX_Transmit((void*)Str);    //
      
    }
        FR_Status = f_mount(&FatFs, SDPath, 1);

    // ����� ����� ��������� ���� �� ������ ��������� � ��������� ���
    sprintf(FilPath, "0:/_OTDR/%s/%s",NameDir[IndexNameDir],NameFiles[IndexNameFiles]); // ���� � �����
    // ������� ���� � ��������� ������ �����
        FR_Status = f_open(&Fil, FilPath, FA_READ);
    if(FR_Status == FR_OK)
    {
     f_lseek (&Fil, 2); // ������������ �� 2 �����
     f_read (&Fil, (void*)&BlkSz, 4, &RWC);
     f_lseek (&Fil, BlkSz); // ������������ ��  �����
     f_read (&Fil, (void*)&F_SOR, 142, &RWC);

    }
    f_close(&Fil);
  FR_Status = f_mount(NULL, "", 0);
    
    for (int i=0; i<12; i++)
    {
      // �������� ����������  � ��������� ���������
      sprintf(Str,"t%d.bco=WHITE���",i+1); // �����
      NEX_Transmit((void*)Str);// 
    }
    sprintf(Str,"t%d.bco=GREEN���",IndexLCDNameFiles+1); // GREEN
    NEX_Transmit((void*)Str);    //
    // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
        sprintf(Str, "t15.txt=\"%d\"���", BlkSz); // < ����� ���� ������ >
    NEX_Transmit((void*)Str);    //
        sprintf(Str, "t16.txt=\"%dnm\"���", F_SOR.AW/10); // < ����� ���� ������ >
    NEX_Transmit((void*)Str);    //
        sprintf(Str, "t17.txt=\"%d\"���", F_SOR.NPPW); // < ����� ���� ������ >
    NEX_Transmit((void*)Str);    //
        sprintf(Str, "t18.txt=\"%d\"���", F_SOR.AR); // < ����� ���� ������ >
    NEX_Transmit((void*)Str);    //
        sprintf(Str, "t19.txt=\"%d\"���", F_SOR.NAV); // < ����� ���� ������ >
    NEX_Transmit((void*)Str);    //

    g_NeedScr = 0;
  }
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    // ����� ��� �������� � ����������� �� �������� ������ ������
    if(ReturnMemView)
    {
      SetMode(ModeFileMngDir);
      ModeDevice = MODEMEMR;
      ModeMemDraw = VIEWNEXT;
      ReturnMemView = 1; // ���� ��������� ���� �� �� ESC
         // ������� ������� ������������ ���� �� Mem_OTDR_garaph (�����)  
      //KeyP = 0;
      ClrKey(BTN_MENU);
      CmdInitPage(34); // ����� ���� ���� ���� ������� ����������
       //CreatDelay(1000000);
      HAL_Delay(100);
    }
  }
}

void ModeMemoryOTDR(void) // ����� ����������� ����������� ������������� � ������ � ����
{
  
  char Str[64];
  char Stra[32];
  //BYTE CurrLang=GetLang(CURRENT);
  unsigned Trace = GetNumTrace();
  static BYTE DeleteTrace = 0; //������� �������� ��������� ������
  static BYTE MsgDel = 0; //��������� ��� �������� ��������� ������
  BYTE NeedCallView = 0; //������� ������������� ������� ���� ��������� ��������������
  //BYTE NeedReStartMeasure = 0; //������� ������������� ����� ��������� ��������� �� ���� ������������ �������������
  // ��������� ����������  � �����������
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED)&&!DeleteTrace)
    // ��������� ������ �� (������������� �������� � ���� ��������� ��������������
  {
      TraceERASE(0);
      TraceWRITE(0); // ���������� �������������� � ����������� �����  � 0 ������
      DeleteTrace = 0;
      GetSetHorizontScale (5);// ��� �������� �� �����
      SetMode(ModeDrawOTDR);
      if(ReturnMemView)
      ReturnModeViewRefl = VIEWMEM;//SETPARAM -  ����� ��������� � ������������
      else
      ReturnModeViewRefl = SETPARAM;//-  ����� ��������� � ������������
        
      ModeDevice = MODEREFL;
      myBeep(10);
      NeedCallView = 1;
    }
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED)&&DeleteTrace)
    // ��������� ������ �� ��� ��������
  {
      // ������� ��������� ��������������
      Trace = DeletingTrace (Trace);
      SetNumTrace(Trace);
      g_FirstScr = 1; // ���� ���������� ������
      ModeMemDraw = VIEWNEXT;

      //ModeMemDraw = VIEWSAVED;
      MsgDel = 2;
      g_NeedScr = 1;
      DeleteTrace = 0;
      myBeep(20);
  }
  
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    if ( ModeMemDraw == VIEWSAVED) // ����� ������� ���� ������� ��������������
    {
      myBeep(10);
      if ((Trace != 0)&&(GetNumTraceSaved(0)>0))
      {
        if (DeleteTrace == 0)
        {
          DeleteTrace = 1;
          MsgDel = 1;
          g_NeedScr = 1;
        }
      }
    }
  }
  
  if ((PRESS(BTN_UP))&&((getStateButtons(BTN_UP)==SHORT_PRESSED)||(getStateButtons(BTN_UP)==INF_PRESSED)))
  {
    
    myBeep(5);
    DeleteTrace = 0;
    if(g_mem_param) g_mem_param = 0; // ����������� ��� ������ ��� ���������
    else g_mem_param = 1;
    // ���������� ��������!
    g_FirstScr = 1; // ���� ���������� ������
    DrawMemoryRefl(Trace, CurrLang, g_mem_param);
    ModeMemDraw = VIEWSAVED;
   
  }
  if ((PRESS(BTN_LEFT))&&((getStateButtons(BTN_LEFT)==SHORT_PRESSED)||(getStateButtons(BTN_LEFT)==INF_PRESSED)))
  {
    
    myBeep(5);
    DeleteTrace = 0;
    if (Trace > 0) Trace--;
    else Trace = GetNumTraceSaved(0);
    SetNumTrace(Trace);
    ModeMemDraw = VIEWNEXT;
    g_FirstScr = 1; // ���� ���������� ������
  }
  if ((PRESS(BTN_RIGHT))&&((getStateButtons(BTN_RIGHT)==SHORT_PRESSED)||(getStateButtons(BTN_RIGHT)==INF_PRESSED)))
  {
    myBeep(5);
    DeleteTrace = 0;
    if (Trace < GetNumTraceSaved(0)) Trace++;
    else Trace =0;
    SetNumTrace(Trace);
    ModeMemDraw = VIEWNEXT;
    g_FirstScr = 1; // ���� ���������� ������
  }
// ������������� ����������� �� �������
  switch (ModeMemDraw)
  {
  case SAVEDTRACE:
    //ClearScreen(); ������ � ������� ���������� ����
    DrawMemoryRefl(Trace, CurrLang, g_mem_param);
    ModeMemDraw = VIEWSAVED;
    break;
  case MEMFULL:
    //ClearScreen();
    DrawMemoryRefl(0, CurrLang, g_mem_param); //�������� ������� �������������� � ����� ��������� � ������ ������ 
    sprintf(Stra,"%s    %d/%d",MsgMass[63][CurrLang], GetNumTraceSaved(0), MAXMEMALL);//  "��� ������" 
    sprintf( Str,"xstr 200,150,200,40,2,2047,BLACK,0,1,3,\"%s\"���",Stra); // 0��� ����� ����������� 
    NEX_Transmit((void*)Str);//

    ModeMemDraw = VIEWSAVED;
    break;
  case VIEWNEXT:
    //ClearScreen();
    DrawMemoryRefl(Trace, CurrLang, g_mem_param);
    // �������������� ��������� �������� (��� ����������� ��������������)
    PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    //ValueDS = (unsigned)((ADCPeriod*50000)/PointsPerPeriod); //  ������������� �������� DS ��� �������������� ������ ���������
    ValueDS = GetValueDS(); //  ������������� �������� DS ��� �������������� ������ ���������
    ModeMemDraw = VIEWSAVED;
    
    break;
  case DELTRACE:
    
    break;
  case VIEWSAVED:
    // ����� 14.11.2022 ����� ��������� ��������� � �������������� ����������� �� ���� ��������� ������������� �� ������ "S"
    //NeedReStartMeasure = 1;
    //ClearScreen();
    //DrawMemoryRefl(Trace, CurrLang, g_mem_param);
    break;
  }
  //
  if(g_FirstScr)
  {
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if(g_NeedScr)
  {
    switch(MsgDel)
    {
    case 1:
      sprintf(Stra,"%s (Ok)", MsgMass[66][CurrLang]);//"�������?"
      sprintf( Str,"xstr 200,150,200,40,2,2047,BLACK,0,1,3,\"%s\"���",Stra); // 0��� ����� �����������
      NEX_Transmit((void*)Str);// 
      MsgDel = 0;
      break;
    case 2:
      sprintf(Stra,"%s    %d/%d",MsgMass[67][CurrLang], GetNumTraceSaved(0), MAXMEMALL);
      sprintf( Str,"xstr 200,130,200,40,2,2047,BLACK,0,1,3,\"%s\"���",Stra); // 0��� ����� �����������
      NEX_Transmit((void*)Str);//
      MsgDel = 0;
      break;
    default:
      break;
    }
    // ���� �������� ������: - ������ ��� ��������, - ������������� ��������
    //      sprintf(Stra,"<ZOOM>");// ������� ���������� �������������� �����
      //sprintf(Stra,"<Z1:%d>",GetSetHorizontScale(0));// ������� ���������� �������������� �����

    g_NeedScr = 0;
  }
  if(NeedCallView)
  {
    // ������� ������� ������������ ���� �� DrawOTDRview (�������)  
    CmdInitPage(18);
         //���� ����� ��� �� ������������
    // �������� ����� ��������� �����... 13.02.2024
    // ������ � 10 ��� ������ �� ������
    //CreatDelay(1000000); //  �� 173
    //CreatDelay(100000); //  173
    //CreatDelay(800000);// 176
    CreatDelay(100000);// 177  ������ � 173

    NeedCallView = 0;
  }
  
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    // ����� ��� �������� � ����������� �� �������� ������ ������
    if(ReturnMemView)
    {
   DeleteTrace = 0;
   SetMode(ModeSelectMEM);
   ModeDevice = MODEOTHER;
   myBeep(10);
    // ������� ������� ������������ ���� �� Memory (�������)  
      CmdInitPage(4);
    }
    else
    {// ���� ��������� ��������������
      TraceERASE(0);
      TraceWRITE(0); // ���������� �������������� � ����������� �����  � 0 ������
      DeleteTrace = 0;
      GetSetHorizontScale (5);// ��� �������� �� �����
      SetMode(ModeDrawOTDR);
      ReturnModeViewRefl = SETPARAM;//SETPARAM -  ����� ��������� � ������������
      ModeDevice = MODEREFL;
      myBeep(10);
    // ������� ������� ������������ ���� �� DrawOTDRview (�������)  
    CmdInitPage(18);
      
    }
  }
 // if (((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))||
 //    ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))) // ���� ��������������� ������� Left Right
    //ClrKey (BNS_MASK-BTN_RIGHT-BTN_LEFT);
 // else
    //ClrKey (BNS_MASK);
  //CalkEventsKeys (LogData, PointsInImpulse(0), 1)
  // � ������ 154 ������ ���������
//  if(NeedReStartMeasure)
//  {
    if (rawPressKeyS) // 17.11.2022 ����� ��������� ���������
    {        
      myBeep(10);
      // ���� �������� ��������� ��������� ����� ����� � ������������ � ������
      SetIndxSeqLS();
       //g_SetModeLW = SettingRefl.SW_LW;
      g_AutoSave = 0; // ����� ����� ��������� ����� ���� ����������/
        
      ReSaveWAV_SC (); // ������������� ���� ���� ���������
      LSEL0(0);
      LSEL1(0);
      SlowON();
      //POWALT(ON);
      //POWREF (ON);
      //POWDET(ON);
      SetMode(ModeStartOTDR);
      ModeDevice = MODEMEASURE;
      ReturnMemView = 0; // ��� �� ����������� �� ���������
      ReturnModeViewRefl = SETPARAM;//SETPARAM -  ����� ��������� � ���������
      //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
      SubModeMeasOTDR = SETPOWER;
      //123 SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
      rawPressKeyS=0;
      
      //CreatDelay (30000); // 3.3 ��
  HAL_Delay(4);
      CmdInitPage(16);// ������� ������� ������������ ���� �� ������ ��������� Measuring
    }
//    NeedReStartMeasure = 0;
//  }
  
      // ������� "����� ��������"
    // ������ �������� ������� ������, ���������� ������� �� UART 13.02.2024?
    // ������ 20000 - �� ����������
    //CreatDelay (20000); //173 � 176,177
  HAL_Delay(3);

}


void ModeSelectOLT(void) // ����� ������ ���� ������� CHECK_OFF
{
  static volatile BYTE FrSelectOLT = 0; // ��������� �� ������
  BYTE RedEye = GetCfgRE(); // �������� ���� �� ��. ����
  char Str[32];
  char StrI[32];
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    FrSelectOLT = ChangeFrSet (FrSelectOLT, 1+RedEye, 0, MINUS);// ��������� ������� � ������ �������� ����������
    g_NeedScr = 1; // Need reDraw Screen
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    FrSelectOLT = ChangeFrSet (FrSelectOLT, 1+RedEye, 0, PLUS);// ��������� ������� � ������ �������� ����������
    g_NeedScr = 1; // Need reDraw Screen
    //ClrKey (BTN_DOWN);
  }
  if((RedEye)&&(FrSelectOLT==1+RedEye)) //���� ������� ���� � ����� �� ���
  {
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      SetModeRE(Str,INCR,CurrLang );
      //ClrKey (BTN_RIGHT);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      SetModeRE(Str,DECR,CurrLang );
      //ClrKey (BTN_LEFT);
    }
  }
  if(g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[49][CurrLang]); // manual
    NEX_Transmit((void*)Str);    //
    
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[50][CurrLang]); // auto
    NEX_Transmit((void*)Str);    // 
    // ��� ������� ����
    if(RedEye)
    {
      sprintf(Str,"t2.txt=\"VFL,    0.65 \"���"); // Red Eye
      NEX_Transmit((void*)Str);    // 
      // ��������� � ����������� �� ���� ����������
      if(TypeLCD) //3.5
      {
      sprintf(Str,"draw 272,113,384,156,BLACK���"); // Red Eye
      NEX_Transmit((void*)Str);    // 
      sprintf(Str,"draw 271,112,385,157,BLACK���"); // Red Eye
      NEX_Transmit((void*)Str);    // 
        
      }
      else
      {
      sprintf(Str,"draw 270,87,382,118,BLACK���"); // Red Eye
      NEX_Transmit((void*)Str);    // 
      sprintf(Str,"draw 269,86,383,119,BLACK���"); // Red Eye
      NEX_Transmit((void*)Str); 
      }   // 
    }
    else
    {
      sprintf(Str,"t2.txt=\" \"���"); // ������ ������
      NEX_Transmit((void*)Str);    // 
      sprintf(Str,"t6.txt=\" \"���"); // ������ ������
      NEX_Transmit((void*)Str);    // 
    }
    // ��������� ������ �����������
    if (ErrOLT)  //
    {
      sprintf(Str,"t5.txt=\"%s :(%2X)\"���", MsgMass[115][CurrLang], ErrOLT); // 
    }
    else
      sprintf(Str,"t5.txt=\" \"���"); // ������ ������
    NEX_Transmit((void*)Str);    // 
    
    
    // ������������� � ����������������� ���������
    // ��������� ������������ ���������
    if (!(WIDE_VER))  // ���� ���� ��������� ��� ������������ ���������
    {
      // ����������� �������������
      if (((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))||((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED)))
      {
        myBeep(10);
        if(SetJDSU.CompMode) SetJDSU.CompMode = 0;
        else SetJDSU.CompMode = 1;
      }
      sprintf(Str,"t3.txt=\"%s: %c\"���", MsgMass[117][CurrLang],(SetJDSU.CompMode)?('J'):('T')); // 
      NEX_Transmit((void*)Str);    // 
      
      sprintf(Str,"t4.txt=\"W\"���"); // 
      NEX_Transmit((void*)Str);    // 
      
    }
    else
    {
      sprintf(Str,"t3.txt=\" \"���"); // ������ ������
      NEX_Transmit((void*)Str);    // 
      sprintf(Str,"t4.txt=\" \"���"); // 
      NEX_Transmit((void*)Str);    // 
    }
    
    
    g_FirstScr=0;
    g_NeedScr=1;
    
  }
  if(g_NeedScr)
  {
    // �������� ����������  � ��������� ���������
    sprintf(Str,"t0.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t1.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t%d.bco=GREEN���",FrSelectOLT); // �������
    NEX_Transmit((void*)Str);// 
    
    // ��� ������� ����
    if(RedEye)
    {
      SetModeRE (Str, CURRENT, CurrLang);
      sprintf(StrI,"t6.txt=\"%s\"���",Str); //�����
      NEX_Transmit((void*)StrI);    // 
    }
    
    g_NeedScr=0;
    
  }
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    SetMode(ModeMainMenu);
    POWDET(OFF); 
    SetupSource (0); // ��������� �������� 
    ModeDevice = MODEMENU;
    //123 SSPInit_Any(MEM_FL1); // �������������� ������������� SSP ��� ���������� ������� FLASH (���� 1 �� ��� �� ����� ����������)
    // ������� ������� ������������ ���� �� MainMenu (�������)  
    CmdInitPage(1);
    
  }
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
    switch (FrSelectOLT)
    {
    case 0:
      myBeep(10);
      //POWDET(ON);
      SetMode(ModeMeasManualOLT);
      //123 enable_timer(3);  /* Enable Interrupt Timer3 */
      //123 enable_timer(1);  /* Enable  Timer1 JDSU */
      ModeDevice = MODETEST;
      ModePowerMeter = POWMANUAL;
      //123 SSPInit_Any(SPI_PM);      //�������� SPI �� ���
      // ������� ������� ������������ ���� �� Test_manual (�����)  
      CmdInitPage(7);
      
      break;
    case 1: // �������
      myBeep(10);
      //POWDET(ON);
      //UARTInit1 (1200);
      //VICINTENCLEAR | = 1 << UART1_INT; /* Disable Interrupt */
//123      VICINTENABLE = 1 << UART1_INT;  /* Enable Interrupt */
      SetMode(ModeMeasAutoOLT);
//123      enable_timer(3);  /* Enable Interrupt Timer3 */
//123      enable_timer(1);  /* Enable  Timer1 JDSU */
      ModeDevice = MODETEST;
      ModePowerMeter = POWAUTO;
      InitAutoPM (); // ��������� ��������� ���������� ��������
//123      SSPInit_Any(SPI_PM);      //�������� SPI �� ���
      // ������� ������� ������������ ���� �� Test_auto (�����)  
      CmdInitPage(8);
      break;
    }
//123    P08_Init();
  }
  
}

float GetLastPower (void)// ����������� ���������� �������� 
{
  return LastPower;
}

void ModeMeasManualOLT(void) // ����� ������ ������� � ������ ������ CHECK_OFF
{
  static BYTE FrManualOLT = 1; // ��������� �� ������
  static BYTE MemIndexLW;
  BYTE NowIndexLW;

  //BYTE AddRed;
  char Str[32];
  char Stra[32];
  char Strb[32];
  char Strc[32];
  BYTE RedEye = GetCfgRE();
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  static float tmp=0;
  static BYTE i=0;                 //������� ��� ������� �� �����
  
  if(i==0)
  {
    tmp =  GetPower(GetPMWavelenght(0)); // �������� �������� � ���
    LastPower = tmp;
    i=10;
   //      g_NeedScr = 1; // Need reDraw Screen
  }
  else
  {
   // LastPower = GetPower(GetPMWavelenght(0)); // �������� �������� � ���
 
  }
    
  i--;
  
//      switch(GetCfgPM())
//    {
//    case 1:
//      if (tmp<1e-9)tmp = 1e-9;                          //����������� ��������� �� ������ 1pW 
//      break;
//    case 2:
//      if (tmp<1e-6)tmp = 1e-6;                          //����������� ��������� �� ������ 1pW 
//      break;
//    }
//
  
  //����������� ������ :
  
  //������ "����"  
  switch(FrManualOLT)
  {
  case 1:   // ����� REF
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      SetTypeRslt(GetTypeRslt()+1);
    }
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
    {
     
    }
    
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))// ������������ �� ��������� ����� �����
    {
      myBeep(15);
      SetCurrLvldB(GetPMWavelenght(0),10*log10(tmp)); // ����� ���������� � ������
      SetTypeRslt(1); // set dB
      g_NeedScr = 1; // Need reDraw Screen
    }
    
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==LONG_PRESSED))
    {
      if (GetRange() == 0)
      {
      myBeep(85);
      AcquireShZeroLowRng();                 // ����� ������������� ���� �� �������������� ���������
      }
      //AcquireCoefStykRange(1);
    }
    
    break;
  case 2:// ��������� ����� �����
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetCurrLvldB(GetPMWavelenght(-1));
    };
    
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
    { 
      g_NeedScr = 1; // Need reDraw Screen
      GetCurrLvldB(GetPMWavelenght(-1));
    };
    
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {    
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetCurrLvldB(GetPMWavelenght(1));
    }
    
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
    {    
      g_NeedScr = 1; // Need reDraw Screen
      GetCurrLvldB(GetPMWavelenght(1));
    }
    
    
    
    if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
    {    
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetCurrLvldB(GetPMWavelenght(13));
    }
    
    break;
    
  }
  
  // ��������� ������ ����� ���� ������� Wide � JDSU ���������
  SetAutoLamdaJDSU (TimerValueJDSU);
  // ��� ��������� � �������� ������
//AddRed =  DrawSourceVFL (&FrManualOLT, Str, KeyP, CurrLang, 3, 1);
DrawSourceVFL (&FrManualOLT, Str, KeyP, CurrLang, 3, 1);
  
  // ������ ������  
  
    SetStringPM(Str, tmp);           // ������������� ������ ������ ���������
    sprintf(Stra, "t0.txt=\"%s\"���", Str);
    NEX_Transmit((void*)Stra);    // ORL


  
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    
    sprintf(Str, "t2.txt=\"REF, %s\"���", MsgMass[48][CurrLang]);
    NEX_Transmit((void*)Str);    // REF
    
    sprintf(Str, "t4.txt=\"%s\"���", MsgMass[17][CurrLang]);
    NEX_Transmit((void*)Str);    // ����� �����
    
    sprintf(Str, "t6.txt=\"%s\"���", MsgMass[68][CurrLang]);
    NEX_Transmit((void*)Str);    // ��������
    
    sprintf(Str, "t8.txt=\"ORL, %s\"���", MsgMass[47][CurrLang]);
    NEX_Transmit((void*)Str);    // ORL
    // ��������
    // �������� �� ������� ����
        sprintf(Str, "t10.txt=\"\"���");
    NEX_Transmit((void*)Str);    // 
        sprintf(Str, "t13.txt=\"\"���");
    NEX_Transmit((void*)Str);    // 
    // ����� ���������� ������
  sprintf(Str,"t9.txt=\"� %04d\"���",GetCellMem(0));
  NEX_Transmit((void*)Str);    // � ������
    
    // ��������� ������������ ���������
    if (!(WIDE_VER))  // ���� ���� ��������� ��� ������������ ���������
    {
      sprintf(Str,"t12.txt=\"%c\"���",(SetJDSU.CompMode)?('J'):('T')); // 
    }
    else
    {
      sprintf(Str, "t12.txt=\" \"���"); // ������ ������
    }
    NEX_Transmit((void*)Str);    // W
    
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str, "t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t4.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t6.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t8.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t10.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t%d.bco=GREEN���", (FrManualOLT)<<1); // �������
    NEX_Transmit((void*)Str);// 
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
  sprintf(Str,"t1.txt=\"%2.3f\"���",GetCurrLvldB(0)); // dBm REF
    NEX_Transmit((void*)Str);    //

  sprintf(Str,"t3.txt=\"%d%s\"���",GetPMWavelenght(0),MsgMass[18][CurrLang]); // nm
    NEX_Transmit((void*)Str);    //
  
    if(g_VolORL!=0)sprintf(Str,"t7.txt=\"%.2f\"���",g_VolORL);
    else sprintf(Str,"t7.txt=\"???\"���");
    NEX_Transmit((void*)Str);    // �������� ORL
    
    // ������ ��� ��������� ()
        sprintf(Str, "t6.txt=\"%s\"���", MsgMass[68][CurrLang]);// source
        SetModeLS (Strb, CURRENT, CurrLang); // �������� ����� ���������
        sprintf(Strc,"t5.txt=\"%s\"���",Strb);
    NEX_Transmit((void*)Str);    // ��������
    
    NEX_Transmit((void*)Strc);    // ��������
    
    // �������� ������ ����� ����� ���������
    MemIndexLW = GetPlaceLS(CURRENT);
    //NowIndexLW = MemIndexLW;
        sprintf(Stra,"t11.txt=\"%.2f\"���",GetLengthWaveLS (MemIndexLW)/1000.0);
    NEX_Transmit((void*)Stra);    // ��������
    
                                       // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }

    // �������� ����� ���� ��������� ���� ����������
  NowIndexLW = GetPlaceLS(CURRENT);
  if(MemIndexLW != NowIndexLW)
  {
    sprintf(Stra,"t11.txt=\"%.2f\"���",GetLengthWaveLS (NowIndexLW)/1000.0);
    NEX_Transmit((void*)Stra);    // ��������
    MemIndexLW = NowIndexLW;
  }

  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
//123    P08_IRQ_EN(OFF); // ��������� ���������� �� ���������� ���
    myBeep(10);
    SetMode(ModeSelectOLT);
    ModeDevice = MODEOTHER;
    ModePowerMeter = NOTMODE;
          // ������� ������� ������������ ���� �� Tester (�������)  
      CmdInitPage(3);
  }
  if (rawPressKeyS) // key S ������ ����� � �������� ����.
  {        
    myBeep(10);
    SetMode(ModeSaveOLT);
    SavePowerMeter(tmp);
    ReLoadCommOLT (); // ������������� ����������� ��� ����������
    ModeDevice = MODEOTHER;
    rawPressKeyS=0;
          // ������� ������� ������������ ���� �� Tester (�������)  
      CmdInitPage(24);
  }
  
}

void SetStringPM(char *Str, float mWatt)           // ������������� ������ ������ ���������
{
  BYTE TypeRslt = GetTypeRslt();
  switch (TypeRslt)
  {
  case 1: //dB
    Watt2dB(Str, mWatt,1);
    sprintf(Str,"%s %s", Str,MsgMass[47][CurrLang]); // �������� dB
    break;
  case 2: // mW
    if(mWatt>1.0)                                  // ���� �������� ������ 1���     
      sprintf(Str,"% 6.2f %s", mWatt, MsgMass[97][CurrLang]);            // �������� � ���
    else if(mWatt*1e3>1.0)                        // ���� ������ 1 ����
      sprintf(Str,"% 6.2f %s", mWatt*1e3, MsgMass[96][CurrLang]);       // �������� � ����
    else if(mWatt*1e6>1.0)                        // ���� ������ 1 ���
      sprintf(Str,"% 6.2f %s", mWatt*1e6, MsgMass[95][CurrLang]);       // �������� � ���
    else 
      // ��������� � ����������� �� �����
          switch(GetCfgPM())
    {
    case 1:
      if (mWatt<1e-9)mWatt = 1e-9;                          //����������� ��������� �� ������ 1pW 
      sprintf(Str,"% 6.2f %s", mWatt*1e9, MsgMass[94][CurrLang]);       // �������� � ���
      break;
    case 2:
          if (WIDE_VER)  // ���� ��� ��������� ��� ������������ ���������
          {
      if (mWatt<=1e-6)mWatt = 1e-6;                          //����������� ��������� �� ������ 1nW 
      sprintf(Str,"<%6.2f %s", mWatt*1e6, MsgMass[95][CurrLang]);       // �������� � ���
          }
          else
          {
      if (mWatt<=1e-8)mWatt = 1e-8;                          //����������� ��������� �� ������ 10pW 
      sprintf(Str,"<%6.2f %s", mWatt*1e9, MsgMass[95][CurrLang]);       // �������� � ���
          }
          
      break;
    }

    break;
  default: // ��������� � ���...
    Watt2dB(Str, mWatt,0);
          switch(GetCfgPM())
    {
    case 1:
      if (mWatt<=1e-9)
      {
        mWatt = 1e-9;                          //����������� ��������� �� ������ 1pW  -90 dBm
      sprintf(Str,"<-90.00 %s", MsgMass[48][CurrLang]); // �������� dBm
      }
      else
      sprintf(Str,"%s %s", Str,MsgMass[48][CurrLang]); // �������� dBm
      break;
    case 2:
          if (WIDE_VER)  // ���� ��� ��������� ��� ������������ ���������
          {
            if (mWatt<=1e-6)
            {
              mWatt = 1e-6;                          //����������� ��������� �� ������ 1nW -60 dBm
              sprintf(Str,"<-60.00 %s", MsgMass[48][CurrLang]); // �������� dBm
            }
            else
              sprintf(Str,"%s %s", Str,MsgMass[48][CurrLang]); // �������� dBm
          }
          else
          {
            if (mWatt<=1e-8)
            {
              mWatt = 1e-8;                          //����������� ��������� �� ������ 10pW -80 dBm
              sprintf(Str,"<-80.00 %s", MsgMass[48][CurrLang]); // �������� dBm
            }
            else
              sprintf(Str,"%s %s", Str,MsgMass[48][CurrLang]); // �������� dBm
          }
      break;
    }
    break;
  }
}

void ModeMeasAutoOLT(void) // ����� ������ ������� � �������������� ������
{
  
  static BYTE FrAutoOLT = 3; // ��������� �� ������
  char Str[32];
  char Stra[32];
  char Strb[32];
  char Strc[32];
  static WORD NumWave=1310;
  static WORD TempNumWave=1310;
  static float tmpPOW;
  static BYTE MemIndexLW;
  //char wrd[10];
  BYTE NowIndexLW;
  //static BYTE NeedSetNewLVL = 0;
  BYTE RedEye = GetCfgRE();
  //BYTE AddRed;
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  TimerAutoPM = TimerPA(0); // ~100mS ������
  //AddRed = DrawSourceVFL (&FrAutoOLT, Str, KeyP, CurrLang, 3, 3);
  DrawSourceVFL (&FrAutoOLT, Str, KeyP, CurrLang, 3, 3);
  /* */  
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==LONG_PRESSED))
  {
    myBeep(10);
    for (int i=0; i<5; i++)
    {
      if ( RPON[i]>-100.0)
      {
        SetNewLeveldB (RPON[i], GetIndxPON(LWPON[i])); // ��������� ������ ������ �������� �� �������� ������
      }
    }
    //123 SSPInit_Any(MEM_FL1); // ������������� SSP ��� ���������� FLASH (���� 1 �� ��� �� ����� ����������)
    WriteNeedStruct (0x04); // ���������� �������� ��������� ������������ �������
    //FlashErasePage(CFG_USER); // ������ �������� ��������� ������������ �������
    //FlashWritePageSM(CFG_USER, StructPtr(CFG_USER), StructSize(CFG_USER), 0);
    //123 SSPInit_Any(SPI_PM); // ������������� SSP ��� ���������� PM (���� 1 �� ��� �� ����� ����������)
  }
  //SetRange(1);
  if (RSOptYes) // �������� ����� ����� �o����� ������ ����� ����������
  {
//123!!!    TempNumWave = GetNumWaveOpt (); // �������� ����� ����� �����
    TempNumWave = 1310; // �������� ����� ����� �����
    if (TempNumWave)
    { 
      // 1. ��������� � ������ ��������� ���������� ����� 
       IndWavePON=GetIndxPON(TempNumWave);
       LWPON[IndWavePON]=TempNumWave; // ������� ����� ����� � ������ (��� ���������)
       LWCNT[IndWavePON] = 0; // clear cnt ������� �����
      // myBeep(5); //���� ������������� ������
      for (int i=0; i<5; i++)
      {
       if (LWPON[i] != 0) // ���� ���� ������ � ��������� � ���������
       {
          if(LWCNT[i]++>3)
            {
              RPON[i]=-100.0;
              LWPON[i] = 0; // �������� ������ ���� ����
              LWCNT[i] = 0; // clear cnt 
            }
       }
       else
       {
              RPON[i]=-100.0;
              LWPON[i] = 0; // �������� ������ ���� ����
              LWCNT[i] = 0; // clear cnt 
       
       }
      }
//      for (int i=0; i<5; i++)
//      {
//        if ((LWPON[i] != 0)&&(TempNumWave==LWPON[i])) // ���� ���� ������ � ��������� � ���������
//          // if (TempNumWave == CoeffPM.PointKalib[i])
//        {
//          IndWavePON = i;// ������ ������ ������ RPON ��� �������� ������������� ����������
//          LWCNT[i] = 0; // clear cnt 
//          break;
//        }// ��������� ������� �������� ����� ������������� �������� ���� ���� ����������
//        else
//        {
//          if(LWPON[i] == 0) // ��� ����� ������� �����
//          {
//            LWPON[i]=TempNumWave;
//            IndWavePON = i;
//            break;
//          }
//          // ����� ���� �� ����� �� �����������
//          else
//          {
//            if(LWCNT[i]++>3)
//            {
//              RPON[i]=-100.0;
//              LWPON[i] = 0; // �������� ������ ���� ����
//              LWCNT[i] = 0; // clear cnt 
//            }
//          }
//        }
//      }
      
      NumWave = TempNumWave;
      GetCurrLvldB(NumWave);
      //SetPMWavelenght (NumWave); // �������������� ��������� ������� ����� �����
      
      ModAutoPM = MEASUR;
      TimerAutoPM = TimerPA(1); // �������� ������
      SetRange(3); // ����� ������
      SetSwitchPMMode(AUTO);  // ������������� ����� ������������ ���������� �������
    }

  }
        // test code
    //sprintf(Str, "t8.txt=\"%s\"���",wrd);
    //sprintf(Str, "t8.txt=\"%d %d\"���",TimerAutoPM,ModAutoPM);
    //NEX_Transmit((void*)Str);    // 
  
  tmpPOW =  GetPower(NumWave); // �������� �������� � ���
  switch (ModAutoPM)
  {
  case WAITING:
    if (TimerAutoPM > 1000) //~10S
    {
      myBeep(10);
      InitAutoPM();
    }
    
    break;
    // ��������� � ��������
    // �������� �����  ������ ��� ����, ��� ��� ����� �������� � �������� ��������� 06.06.2011
  case MEASUR:
    if (TimerAutoPM >= 150) //~1.5S
    {
      RPON[IndWavePON] = Watt2dB(Str, tmpPOW , 1);
      ModAutoPM = WAITING; // ����� ���������� ���� �������
      TimerAutoPM = TimerPA(1); // ������� ������� (��������)
      SetSwitchPMMode(MANUAL);  // ������������� ����� ������������ ���������� � ������
      SetRange(1);
    }
    
    break;
  }
  BYTE   M_PON =0;
  // ������ �������� � ����� ���������
 // ������ ���!? ����� ���� ������� ����
    // ������ ������ ���������
  M_PON = 0;
  for (int i=0; i<5; ++i)
  {
    if ((RPON[i]>-100.0)&&(M_PON<3)) // ���� �������� ��� ���������
    {
      //DrawLine(2,12*M_PON+5,114,12*M_PON+5,11,0); //������ ���������� ���������
      sprintf(Str,"t%d.txt=\"%6.3f %s\"���",M_PON+1, RPON[i], MsgMass[47][CurrLang]);//��
    NEX_Transmit((void*)Str);    // ��������
      //sprintf(Str,"t%d.txt=\"%d%s\"���",M_PON+4,CoeffPM.PointKalib[i], MsgMass[18][CurrLang]);//�� 1300
      sprintf(Str,"t%d.txt=\"%d%s\"���",M_PON+4,LWPON[i], MsgMass[18][CurrLang]);//��
    NEX_Transmit((void*)Str);    // ����� �����
      M_PON++;
    }
  }
  for(int i=M_PON; i<3;++i)
  {
    //DrawLine(2,i*12+5,114,i*12+5,11,0); //������ ���������� ���������
    sprintf(Str,"t%d.txt=\"--------\"���",i+1);
    NEX_Transmit((void*)Str);    // ��������
    sprintf(Str,"t%d.txt=\"----\"���",i+4);
    NEX_Transmit((void*)Str);    // ����� �����
  }
  
  
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    
 //   sprintf(Str, "t2.txt=\"REF, %s\"���", MsgMass[48][CurrLang]);
 //   NEX_Transmit((void*)Str);    // REF
    
 //   sprintf(Str, "t4.txt=\"%s\"���", MsgMass[17][CurrLang]);
 //   NEX_Transmit((void*)Str);    // ����� �����
    
    sprintf(Str, "t7.txt=\"%s\"���", MsgMass[68][CurrLang]);
    NEX_Transmit((void*)Str);    // ��������
    
    sprintf(Str, "t8.txt=\"ORL, %s\"���", MsgMass[47][CurrLang]);
    NEX_Transmit((void*)Str);    // ORL
    // ��������
    // �������� �� ������� ����
        sprintf(Str, "t9.txt=\"\"���");
    NEX_Transmit((void*)Str);    // 
    // test code
    //memcpy(wrd ,RX_BufOpt,9);
        sprintf(Str, "t13.txt=\"\"���");
   //wrd[9]=0;
    //    sprintf(Str, "t13.txt=\"%s\"���",wrd);
    NEX_Transmit((void*)Str);    // 
    // ����� ���������� ������
  sprintf(Str,"t12.txt=\"� %04d\"���",GetCellMem(0));
  NEX_Transmit((void*)Str);    // � ������
    
    // ��������� ������������ ���������
    if (!(WIDE_VER))  // ���� ���� ��������� ��� ������������ ���������
    {
      sprintf(Str,"t0.txt=\"%c\"���",(SetJDSU.CompMode)?('J'):('T')); // 
    }
    else
    {
      sprintf(Str, "t0.txt=\" \"���"); // ������ ������
    }
    NEX_Transmit((void*)Str);    // W
    
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
 // sprintf(Str,"t1.txt=\"%2.2f\"���",GetCurrLvldB(0)); // dBm REF
 //   NEX_Transmit((void*)Str);    //

 // sprintf(Str,"t3.txt=\"%d%s\"���",GetPMWavelenght(0),MsgMass[18][CurrLang]); // nm
 //   NEX_Transmit((void*)Str);    //
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str, "t7.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t8.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t9.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t%d.bco=GREEN���", (FrAutoOLT)+4); // �������
    NEX_Transmit((void*)Str);// 
                                       // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
  
    // ������ ��� ��������� ()
        sprintf(Str, "t7.txt=\"%s\"���", MsgMass[68][CurrLang]);// source
        //sprintf(Stra,"t14.txt=\"%.2f\"���",GetLengthWaveLS (GetPlaceLS(CURRENT))/1000.0);
        SetModeLS (Strb, CURRENT, CurrLang); // �������� ����� ���������
        sprintf(Strc,"t10.txt=\"%s\"���",Strb);
    NEX_Transmit((void*)Str);    // ��������
    
    //NEX_Transmit((void*)Stra);    // ��������
    NEX_Transmit((void*)Strc);    // ��������
  
    if(g_VolORL!=0)sprintf(Str,"t11.txt=\"%.2f\"���",g_VolORL);
    else sprintf(Str,"t11.txt=\"???\"���");
    NEX_Transmit((void*)Str);    // �������� ORL
    // �������� ������ ����� ����� ���������
    MemIndexLW = GetPlaceLS(CURRENT);
    //NowIndexLW = MemIndexLW;
        sprintf(Stra,"t14.txt=\"%.2f\"���",GetLengthWaveLS (MemIndexLW)/1000.0);
    NEX_Transmit((void*)Stra);    // ��������
    
    
    g_NeedScr = 0;
  }
    //sprintf(Str,"%d",TimerValueJDSU);
    //putString(82,12*2+6,Str,1,0);
    // �������� ����� ���� ��������� ���� ����������
  NowIndexLW = GetPlaceLS(CURRENT);
  if(MemIndexLW != NowIndexLW)
  {
    sprintf(Stra,"t14.txt=\"%.2f\"���",GetLengthWaveLS (NowIndexLW)/1000.0);
    NEX_Transmit((void*)Stra);    // ��������
    MemIndexLW = NowIndexLW;
  }
  
  
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    //UARTInit1(1200);//, OFF
//123    VICINTENCLEAR  = 1 << UART1_INT; /* Disable Interrupt */
//123    disable_timer(3); /* Disable Interrupt Timer3*/
//123    disable_timer(1); /* Disable  Timer1 JDSU*/
    
    SetMode(ModeSelectOLT);
    ModeDevice = MODEOTHER;
    ModePowerMeter = NOTMODE;
    SetSwitchPMMode(AUTO);  // ������������� ����� ������������ ���������� � ������
          // ������� ������� ������������ ���� �� Tester (�������)  
      CmdInitPage(3);
  }
  
  if (rawPressKeyS) // key S 
  {        
    myBeep(10);
    SetMode(ModeSaveOLT);
    SavePowerMeter(tmpPOW);
    ReLoadCommOLT (); // ������������� ����������� ��� ����������

    ModeDevice = MODEOTHER;
    //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
    rawPressKeyS=0;
          // ������� ������� ������������ ���� �� Tester (�������)  
      CmdInitPage(24);
  }
}

void ModeSourceOnly(void) // ����� ������ ������� ������ �������� CHECK_OFF
{
  static BYTE FrSourceOnly = 1; // ��������� �� ������
  BYTE RedEye = GetCfgRE();
  static BYTE MemIndexLW;
  BYTE NowIndexLW;
  char Str[32];
  char Stra[32];
  char Strb[32];
  char Strc[32];  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  DrawSourceVFL (&FrSourceOnly, Str, KeyP, CurrLang, 1, 1);
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    GetDeviceLabel( Str, NameDB.Ena_DB  );
    sprintf(Stra, "t0.txt=\"%s\"���", Str);
    NEX_Transmit((void*)Stra);    // ������������ �������
    
    sprintf(Str, "t2.txt=\"ORL, %s\"���", MsgMass[47][CurrLang]);
    NEX_Transmit((void*)Str);    // ORL
    
    // ������� ����
    if(RedEye)
    {
      sprintf(Str,"t3.txt=\"VFL,    0.65 \"���"); // Red Eye
      NEX_Transmit((void*)Str);    // 
      sprintf(Str,"draw 284,159,383,188,BLACK���"); // Red Eye
      NEX_Transmit((void*)Str);    // 
      sprintf(Str,"draw 285,160,382,187,BLACK���"); // Red Eye
      NEX_Transmit((void*)Str);    // 
    }
    else
    {
      sprintf(Str,"t3.txt=\" \"���"); // ������ ������
      NEX_Transmit((void*)Str);    // 
      sprintf(Str,"t5.txt=\" \"���"); // ������ ������
      NEX_Transmit((void*)Str);    // 
    }
    
    
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  { 
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str, "t1.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t3.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t%d.bco=GREEN���", (FrSourceOnly)); // �������
    NEX_Transmit((void*)Str);// 
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
    // ������ ��� ��������� ()
    // �������� ������ ����� ����� ���������
    MemIndexLW = GetPlaceLS(CURRENT);
    NowIndexLW = MemIndexLW;
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[68][CurrLang]);// source
    sprintf(Stra,"t4.txt=\"%.2f\"���",GetLengthWaveLS (MemIndexLW)/1000.0);
    SetModeLS (Strb, CURRENT, CurrLang); // �������� ����� ���������
    sprintf(Strc,"t6.txt=\"%s\"���",Strb);
    NEX_Transmit((void*)Str);    // ��������
    
    NEX_Transmit((void*)Stra);    // ��������
    NEX_Transmit((void*)Strc);    // ��������
    
    if(RedEye) // Red eye
    {
      SetModeRE (Strb, CURRENT, CurrLang);
      sprintf(Strc,"t5.txt=\"%s\"���",Strb);
      NEX_Transmit((void*)Strc);    // Red Eye
    }
    
    
    if(g_VolORL!=0)sprintf(Str,"t7.txt=\"%.2f\"���",g_VolORL);
    else sprintf(Str,"t7.txt=\"???\"���");
    NEX_Transmit((void*)Str);    // �������� ORL
    
    
    // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }
  // �������� ����� ���� ��������� ���� ����������
  NowIndexLW = GetPlaceLS(CURRENT);
  if(MemIndexLW != NowIndexLW)
  {
    sprintf(Stra,"t4.txt=\"%.2f\"���",GetLengthWaveLS (NowIndexLW)/1000.0);
    NEX_Transmit((void*)Stra);    // ��������
    MemIndexLW = NowIndexLW;
  }
  
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    SetMode(ModeMainMenu);
    ModeDevice = MODEMENU;
    POWDET(OFF); 
    SetupSource (0); // ��������� �������� 
    // ������� ������� ������������ ���� �� MainMenu (�������)  
    CmdInitPage(1);
  }
}

void ModeSaveOLT(void) // ����� ���������� ����������� ���������� CHECK_OFF
{
  static BYTE FrSaveOLT = 1; // ��������� �� ������
  char Str[32];
  static BYTE ErrMemOlt = 0; // ��������� �� ������
//  static BYTE NeedKeyB = 0; // ������������� ������������ � ����������
  
  if (GetCellMem(0) <MaxMemPM)
  {
    if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      FrSaveOLT = ChangeFrSet (FrSaveOLT, 4, 1, MINUS);// ��������� ������� � ������ �������� ����������
      //ClrKey (BTN_UP);
    }
    if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      FrSaveOLT = ChangeFrSet (FrSaveOLT, 4, 1, PLUS);// ��������� ������� � ������ �������� ����������
      //ClrKey (BTN_DOWN);
    }
    switch (FrSaveOLT) // ��������� �������� ����� ���������
    {
    case 1: //����� ��������� �����������
      if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetMode(ModeKeyBoardOLT);
        ModeDevice = MODEOTHER;
        NeedKeyB = 1; // ������������� ������������ � ����������
      }
      break;
    case 2: //��������� �������� �������
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        if (PONI.NumFix<9999)PONI.NumFix++;
        else PONI.NumFix = 0;
        //ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        if (PONI.NumFix>0)PONI.NumFix--;
        else PONI.NumFix = 9999;
        //ClrKey (BTN_LEFT);
      }
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        if (PONI.NumFix<9999)PONI.NumFix++;
        else PONI.NumFix = 0;
        //ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        if (PONI.NumFix>0)PONI.NumFix--;
        else PONI.NumFix = 9999;
        //ClrKey (BTN_LEFT);
      }
      break;
    case 3: //�������� ��������� ��������������
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        GetEnIncFiber(1);
        //ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        GetEnIncFiber(1);
        //ClrKey (BTN_LEFT);
      }
      break;
    case 4: //����� �������� 
      if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        PONI.NumFix = 0;
        //ClrKey (BTN_OK);
      }
      break;
    }
  }
  else
  {
    if(!ErrMemOlt)
    {
      ErrMemOlt = 1;
      // ���������� � ����� ���� �� ������ � �������� ��� ��!
      // ������� ������� ������������ ���� �� MainMenu (�������)  
      CmdInitPage(25);
      if (g_FirstScr)
      {
        
        sprintf(Str, "t0.txt=\"%s\"���", MsgMass[36][CurrLang]);
        NEX_Transmit((void*)Str);    // � � � � � � � �
        sprintf(Str, "t1.txt=\"%s\"���", MsgMass[59][CurrLang]);
        NEX_Transmit((void*)Str);    // " ������ ����������"
        sprintf(Str, "t2.txt=\"%s\"���", MsgMass[60][CurrLang]);
        NEX_Transmit((void*)Str);    // "��� ��������� ������"
        sprintf(Str, "t3.txt=\"%s\"���", MsgMass[38][CurrLang]);
        NEX_Transmit((void*)Str);    // "  ��� �����������  "
        sprintf(Str, "t4.txt=\"%s < S >\"���", MsgMass[39][CurrLang]);
        NEX_Transmit((void*)Str);    // "    ������� \"S\"   "
        g_FirstScr = 0;
      }
      
    }
  }
  
  //ClrKey (BNS_MASK);
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    
    sprintf(Str, "t0.txt=\"%s\"���",PONI.CommUserPM);
    NEX_Transmit((void*)Str);    // 1 ������ �����������
    
    sprintf(Str, "t1.txt=\"%04d\"���", PONI.NumFix);
    NEX_Transmit((void*)Str);    // ����� ������
    
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[54][CurrLang]);
    NEX_Transmit((void*)Str);    // ������������
    
    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[61][CurrLang]);
    NEX_Transmit((void*)Str);    // ����� �������
    
    sprintf(Str, "t4.txt=\"%s\"���", MsgMass[55][CurrLang]);
    NEX_Transmit((void*)Str);    // ��� �������� ��������
    
    sprintf(Str, "t5.txt=\"%s\"���", MsgMass[58][CurrLang]);
    NEX_Transmit((void*)Str);    // ����� ��������
    
    sprintf(Str, "t8.txt=\"� %04d\"���", GetCellMem(0));
    NEX_Transmit((void*)Str);    // ����� ��������
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
    
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str, "t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str); //
    sprintf(Str, "t3.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t4.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t5.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t%d.bco=GREEN���", FrSaveOLT+1); // �������
    NEX_Transmit((void*)Str);// 
    
    sprintf(Str, "t1.txt=\"%04d\"���", PONI.NumFix);
    NEX_Transmit((void*)Str);    // ����� ������
    
    sprintf(Str, "t6.txt=\"%04d\"���",PONI.NumFix);
    NEX_Transmit((void*)Str);    ////������� �������
    
    if (!GetEnIncFiber(0))  sprintf(Str,"t7.txt=\"%s\"���", MsgMass[56][CurrLang]);//������
    else   sprintf(Str,"t7.txt=\"%s\"���", MsgMass[57][CurrLang]);//����
    NEX_Transmit((void*)Str);    //
    
    // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    
    
    g_NeedScr = 0;
  }
  
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    ReturnToTester (0); // ������� � ����������
  }
  if (rawPressKeyS) // key S
  {        
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    memcpy (PONI.CommUserPM, CommentsOLT,16);
    WriteMemPow(); // �� ���� �� ������������ � ����������
    rawPressKeyS=0;
    ErrMemOlt=0;
  }
  if(NeedKeyB ) // ������������� ������������ � ���������� OLT
  {
    // �������� ���������� ����� ������� ����������
    if (CurrLang) 
      // ������� ������� ������������ ���� �� EnglishOLT Keyboard 
      CmdInitPage(20); //(22)
    else
      // ������� ������� ������������ ���� �� RussianOLT Keyboard 
      CmdInitPage(30); //(23)
    NeedKeyB=0; 
  }
}

void ModeKeyBoardOLT(void) // ����� ����������� ���������� ��������� ������������ ����������
{
  char Str[32];
  char StrI[32];
  //char StrOld[22]; // ������ ������ �����������
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  //static BYTE Shift = 0; // ������� 
//  static BYTE NeedReturn = 0; // ������������� ��������� � ���� ����������
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str,"%02d/%02d/%02d %02d:%02d:%02d",TimeSaveOLT.RTC_Year%100,
            TimeSaveOLT.RTC_Mon,
            TimeSaveOLT.RTC_Mday,
            TimeSaveOLT.RTC_Hour,
            TimeSaveOLT.RTC_Min,
            TimeSaveOLT.RTC_Sec );
    
    sprintf(StrI, "t0.txt=\"%s\"���", Str);
    NEX_Transmit((void*)StrI);    // Date/Time ������
    // ���� �������� �� ���������� �������....
    memcpy(StrI,CommentsOLT,IndexCommOLT+1); 
    StrI[IndexCommOLT+1]=0;// No more 16 byte size
    sprintf(Str, "t1.txt=\"%s\"���",StrI); 
    NEX_Transmit((void*)Str);    // 1 ������ �����������
    g_GetStr=1; // ������� �������  ������������� ��������� �������! �� ����������.
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    g_NeedScr=0;
  }
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    SetMode(ModeSaveOLT);
    ModeDevice = MODEOTHER;
    NeedReturn = 1;
    myBeep(10);
  }
  if(g_GetStr==2) // ���! ��� �� ������� �����, ����� ���������� � �������� �������
  {
    // ����� ���������� ������ �� ������
    IndexCommOLT = GetStringNEX(CommentsOLT, ARRAY_SIZE(CommentsOLT));
    //memcpy(CommentsOLT,&RX_BufNEX[1],(CntRXNEX<18)?(CntRXNEX-2):(15)); 
    //CommentsOLT[15]=0;
    for(int i=IndexCommOLT; i<ARRAY_SIZE(CommentsOLT); ++i) CommentsOLT[i]=' ';
    CommentsOLT[ARRAY_SIZE(CommentsOLT)-1]=0;
        memcpy (PONI.CommUserPM, CommentsOLT,16);
        SetMode(ModeSaveOLT);
        
    NeedReturn=1;
  }
  if(NeedReturn)
  {
    // ������� ������� ������������ ���� �� SaveOLT (�������) 
    // ���� ���������� �� ��� ��������� � ��������� !!!
    // ����� ��� 16 ���� � ��������� �������
    g_GetStr=0; // ������� �������  ������������� ��������� �������! �� ����������.
    CmdInitPage(24);
    
    NeedReturn = 0;
  }
  
  
  
}

// ��������� ���������� ��� ��������� �������� ����� �����
void ModeKeyBoardPrefix(void) // ����� ����������� ���������� ��������� prefixNameFile
{
  char Str[32];
  char StrI[32];
  //static BYTE Shift = 0; // ������� 
  // ��� ��� ������ ����������
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // ���� �������� �� ���������� �������....
    memcpy(StrI,PrefixFileNm,IndexPrefix+1); 
    StrI[IndexPrefix+1]=0;// No more 11 byte size
    sprintf(Str, "t1.txt=\"%s\"���",StrI); 
    NEX_Transmit((void*)Str);    // 1 ������ �����������
    g_GetStr=1; // ������� �������  ������������� ��������� �������! �� ����������.
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    g_NeedScr=0;
  }
  
  if(g_GetStr==2) // ���! ��� �� ������� �����, ����� ���������� � �������� �������
  {
    // ����� ���������� ������ �� ������
    IndexPrefix = GetStringNEX(PrefixFileNm, ARRAY_SIZE(PrefixFileNm));
    //memcpy(CommentsOLT,&RX_BufNEX[1],(CntRXNEX<18)?(CntRXNEX-2):(15)); 
    //CommentsOLT[15]=0;
    for(int i=IndexPrefix; i< ARRAY_SIZE(PrefixFileNm); ++i) PrefixFileNm[i]=' ';
    PrefixFileNm[ARRAY_SIZE(PrefixFileNm)-1]=0;
    NeedReturn=1;
  }
  
  if (((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))||(NeedReturn))
  {
    myBeep(10);
    SetMode(ModeSetupOTDR);  //  ������� � ����� ��������� ������������� (����)
    ModeDevice = MODESETREFL;
    SubModeMeasOTDR = NOTMODE;
    // �������� � ����
    // ����� ��� 11 ���� � ��������� �������
    g_GetStr=0; // ������� �������  ������������� ��������� �������! �� ����������.
    // �������� � ���� �������� �������������
    CmdInitPage(2);
    NeedReturn = 0;
  }
  
  
  
}


void ModeSelectMEM(void) // ����� ������ ������ � ������� CHECK_OFF
{
  static volatile BYTE FrSelectMEM = 1; // ��������� �� ������
  char Str[32];
  BYTE PowerMeter=0;
  if (GetCfgPM()) PowerMeter=1;
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSelectMEM = ChangeFrSet (FrSelectMEM, 2+PowerMeter, 1, MINUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSelectMEM = ChangeFrSet (FrSelectMEM, 2+PowerMeter, 1, PLUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_DOWN);
  }
  
  
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    // ����� ���������� ��� ���� 
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[42][CurrLang]);
    NEX_Transmit((void*)Str);    // ������
    
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[43][CurrLang]);
    NEX_Transmit((void*)Str);    // ��������
    
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[7][CurrLang]);
    NEX_Transmit((void*)Str);    // ������������
    
    sprintf(Str, "t3.txt=\"%4d\"���", MAXMEMALL-GetNumTraceSaved(0));
    NEX_Transmit((void*)Str);    // ������� ��������
    
    if (PowerMeter) // ���� ����������
    {
      
      sprintf(Str, "t4.txt=\"%s\"���", MsgMass[74][CurrLang]);
      NEX_Transmit((void*)Str);    // ����������
      
      sprintf(Str, "t5.txt=\"%4d\"���",MaxMemPM-GetCellMem(0));
      NEX_Transmit((void*)Str);    // ���������� (����� �����)
      
      sprintf(Str, "t6.txt=\"%s\"���", MsgMass[44][CurrLang]);
      NEX_Transmit((void*)Str);    // �������
    }
    else  // ���������� ���
    {
      sprintf(Str, "t4.txt=\"%s\"���", MsgMass[44][CurrLang]);
      NEX_Transmit((void*)Str);    // �������
      sprintf(Str, "t5.txt=\"\"���");
      NEX_Transmit((void*)Str);    // ������
      
      sprintf(Str, "t6.txt=\"\"���");
      NEX_Transmit((void*)Str);    // ������
      
    }
    
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
    
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str, "t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t4.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t6.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t%d.bco=GREEN���", FrSelectMEM<<1); // �������
    NEX_Transmit((void*)Str);// 
    // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }
  
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
    switch (FrSelectMEM) // ����� �� ������ "��"
    {
    case 1: // ������� � ������ �������������
      myBeep(10);
      
      //SetMode(ModeMemoryOTDR);
      SetMode(ModeFileMngDir);
      ModeDevice = MODEMEMR;
      ModeMemDraw = VIEWNEXT;
      ReturnMemView = 1; // ���� ��������� ���� �� �� ESC
      // ������� ������� ������������ ���� �� Mem_OTDR_garaph (�����)  
      ClrKey(BTN_OK);
      CmdInitPage(33); // ����� ���� ���� ���� ������� ����������
      //CreatDelay(1000000);
      HAL_Delay(100);
      
      
      break;
    case 2: // ������� �  ������ ����������
      if (PowerMeter) // ���� ����������
      {
        // go to wiev memPM
        if (GetCellMem(0))
        {
          //123!!!       ReadCellIzm(GetCellMem(0)-1,(unsigned char*)&PONI);//  ������ �� ������(flash) � PONI ������ ���������� ����������
          
          Sec2Date (PONI.TotalTimeCell, &TimeSaveOLT);
          myBeep(10);
          NumCellIzm = GetCellMem(0)-1;
          SetMode(ModeViewMemOLT);
          SetModeDevice(MODETESTMEM);
          // ������� ������� ������������ ���� �� Mem_OLT_view (�����)  
          CmdInitPage(20);
        }
      }
      else
      {
        myBeep(10);
        SetMode(ModeClearMEM);
        FrClearMEM = 2 + PowerMeter;
        // ������� ������� ������������ ���� �� Select_MEM_Clr(�����)  
        CmdInitPage(21);
        //NeedReturn = 4; // ��� �� �������� ���� ��
      }
      break;
    case 3: // ������� � ����� ������ �������� ������
      myBeep(10);
      SetMode(ModeClearMEM);
      FrClearMEM = 2 + PowerMeter;
      // ������� ������� ������������ ���� �� Select_MEM_Clr(�����)  
      CmdInitPage(21);
      //NeedReturn = 4; // ��� �� �������� ���� ��
      break;
    }
    
  }
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    SetMode(ModeMainMenu);
    ModeDevice = MODEMENU;
    // ������� ������� ������������ ���� �� MainMenu (�������)  
    CmdInitPage(1);
    
  }
}

void ModeClearMEM(void) // ����� ������������ ������ ���������� CHECK_OFF
{
  char Str[32];
  //(InputOK)?("OK"):("???")
    BYTE PowerMeter=((GetCfgPM())?(1):(0));
  //static BYTE FrClearMEM = 1; // ��������� �� ������
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
    switch (FrClearMEM)
    {
    case 1: // refl
      DeletingAllTrace (); // "��������"���� ��������������, ������ �������� ������� ����������
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      NeedReturn = 4; // ������ ���� ��������
      //SetMode(ModeSelectMEM);
      break;
    case 2:
      if (PowerMeter)
      {
        DeletedAllCell (); // �������� ���� ������� ����������
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        NeedReturn = 4; // ������ ���� ��������
        //SetMode(ModeSelectMEM);
      }
      else
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        NeedReturn = 4; // ������ ���� ��������
       // SetMode(ModeSelectMEM);
      }
      break;
    case 3:
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      NeedReturn = 4; // ������ ���� ��������
      //SetMode(ModeSelectMEM);
      break;
    }
  }
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrClearMEM = ChangeFrSet (FrClearMEM, 2+PowerMeter, 1, MINUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrClearMEM = ChangeFrSet (FrClearMEM, 2+PowerMeter, 1, PLUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_DOWN);
  }

  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������     
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[45][CurrLang]); 
    NEX_Transmit((void*)Str);    // ������� ������
                                                             
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[7][CurrLang]);
    NEX_Transmit((void*)Str);    // ������������ 

          if (PowerMeter)
          {
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[74][CurrLang]);
    NEX_Transmit((void*)Str);    // ����������     

    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[46][CurrLang]); 
    NEX_Transmit((void*)Str);    // ������
          }
          else
          {
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[46][CurrLang]); 
    NEX_Transmit((void*)Str);    // ������
    sprintf(Str, "t3.txt=\"\"���");
    NEX_Transmit((void*)Str);    // Empty string    

          }
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������

    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str, "t1.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t3.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//

    sprintf(Str, "t%d.bco=GREEN���", FrClearMEM); // �������
    NEX_Transmit((void*)Str);// 
                                       // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }

  if (((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))||(NeedReturn))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    SetMode(ModeSelectMEM);
    if(!NeedReturn) NeedReturn = 4;
            // ������� ������� ������������ ���� �� MainMenu (�������)  
    CmdInitPage(NeedReturn);
    NeedReturn = 0;
    
    //ModeDevice = MODEMENU;
  }
}

void ModeViewMemOLT(void) // ����� ��������� ������ ���������� CHECK_OFF
{
  char Str[32];
  char StrO[64];
  if ((PRESS(BTN_LEFT))&&((getStateButtons(BTN_LEFT)==SHORT_PRESSED)||(getStateButtons(BTN_LEFT)==INF_PRESSED)))
      {
        if (NumCellIzm > 0) NumCellIzm--;
        else NumCellIzm = GetCellMem(0)-1;
//123!!!       ReadCellIzm(NumCellIzm,(unsigned char*)&PONI);//  ������ �� ������(flash) � PONI ������ ���������� ����������
       Sec2Date (PONI.TotalTimeCell, &TimeSaveOLT);
       g_NeedScr = 1; // ��� ������ ���������� ��������
       myBeep(10);
        
      }
  if ((PRESS(BTN_RIGHT))&&((getStateButtons(BTN_RIGHT)==SHORT_PRESSED)||(getStateButtons(BTN_RIGHT)==INF_PRESSED)))
      {
        if (NumCellIzm < GetCellMem(0)-1) NumCellIzm++;
        else NumCellIzm =0;
//123!!!       ReadCellIzm(NumCellIzm,(unsigned char*)&PONI);//  ������ �� ������(flash) � PONI ������ ���������� ����������
       Sec2Date (PONI.TotalTimeCell, &TimeSaveOLT);
       g_NeedScr = 1; // ��� ������ ���������� ��������
       myBeep(10);
      }
  
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������   
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[0][CurrLang]); // 
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[0][CurrLang]); // 
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[17][CurrLang]); // ����� �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[0][CurrLang]); //  
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t4.txt=\"%s\"���", MsgMass[0][CurrLang]); //
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t5.txt=\"%s\"���", MsgMass[52][CurrLang]); // ������  
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t6.txt=\"%s\"���", MsgMass[24][CurrLang]); // ��
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t7.txt=\"%s\"���", MsgMass[0][CurrLang]); //
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t8.txt=\"%s\"���", MsgMass[0][CurrLang]); //
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t9.txt=\"%s\"���", MsgMass[47][CurrLang]); // ��
    NEX_Transmit((void*)Str);// 


    g_NeedScr = 1; // ��� ������ ���������� ��������
    g_FirstScr = 0;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
  // ������ ������� ���� � ����� ����� � ��� ����� � ������
  sprintf(Str,"%02d.%02d.%02d_%02d:%02d:%02d (%04d)",TimeSaveOLT.RTC_Year%100,
          TimeSaveOLT.RTC_Mon,
          TimeSaveOLT.RTC_Mday,
          TimeSaveOLT.RTC_Hour,
          TimeSaveOLT.RTC_Min,
          TimeSaveOLT.RTC_Sec,
          NumCellIzm+1); // ���������� 23.04.2013 - ����� � ������ � ���������� ����� ���������� 
    sprintf(StrO, "t0.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
  sprintf(Str,"%s",PONI.CommUserPM);
    sprintf(StrO, "t1.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
  sprintf(Str,"%04d",PONI.NumFix);
    sprintf(StrO, "t10.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
  switch (PONI.Rez) // ����� ������ � ����������� �� ���� ����� ��������
  {
  case MANUAL:
    sprintf(Str,"%s", MsgMass[17][CurrLang]);//����� �����
    sprintf(StrO, "t2.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"%d%s",PONI.LenWaveMeas,MsgMass[18][CurrLang]);//��
    sprintf(StrO, "t6.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"%s", MsgMass[51][CurrLang]);//�������, 
    sprintf(StrO, "t3.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    DrawLevelStr(Str); //???? ����� �������� � ��� ������� W/ dB/ dBm
    sprintf(StrO, "t7.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"REF,     %s", MsgMass[48][CurrLang]);//���
    sprintf(StrO, "t4.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"%.2f",PONI.BaseLvl[0]);
    sprintf(StrO, "t8.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"%s,  %s", MsgMass[52][CurrLang], MsgMass[47][CurrLang]);//������,  ��
    sprintf(StrO, "t5.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"%.2f",PONI.PowLevel[1]);
    sprintf(StrO, "t9.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    
    break;
  case AUTO:
    sprintf(Str,"%s", MsgMass[53][CurrLang]);//��.�����    
    sprintf(StrO, "t2.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"%s", MsgMass[52][CurrLang]);//  ������
    sprintf(StrO, "t6.txt=\"%s\"���", Str); // 
   NEX_Transmit((void*)StrO);// 
        for (int i=0; i<3; i++)
        {
          if (PONI.PowLevel[i]>-100.0)
          {
    sprintf(Str,"%04d %s  ",PONI.LenWaveKlb[i], MsgMass[18][CurrLang]);
    sprintf(StrO, "t%01d.txt=\"%s\"���",3+i, Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"%.2f %s",PONI.PowLevel[i], MsgMass[47][CurrLang]);
    sprintf(StrO, "t%01d.txt=\"%s\"���",7+i, Str); // 
   NEX_Transmit((void*)StrO);// 
          }
    else 
    {
    sprintf(Str,"-------");
    sprintf(StrO, "t%01d.txt=\"%s\"���",3+i, Str); // 
   NEX_Transmit((void*)StrO);// 
    sprintf(Str,"--------");
    sprintf(StrO, "t%01d.txt=\"%s\"���",7+i, Str); // 
   NEX_Transmit((void*)StrO);// 
    }      

        }
    break;
  }

    // ������������� ���� ������ 

    g_NeedScr = 0;
  }


    if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    SetMode(ModeSelectMEM);
    ModeDevice = MODEOTHER;
    // ������� ������� ������������ ���� �� Memory (�������)  
      CmdInitPage(4);
  }
  //ClrKey (BNS_MASK);

}

void ModeSetting(void)// ����� ��������� ������� CHECK_IN
{
  static BYTE FrSetting = 0; // ��������� �� ������
  char Str[32];
  char SetNewWinIfOut = 0; // �������������� ������� ������� � ������ ���� ���� ���� �����
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetting = ChangeFrSet (FrSetting, (ENAOLDLCD)?(4):(3), 0, MINUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetting = ChangeFrSet (FrSetting, (ENAOLDLCD)?(4):(3), 0, PLUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_DOWN);
  }
  switch (FrSetting) // ������� ������������ �����
  {
  case 0: // Data_Time_Set
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    PosCurr = 6;
    SetMode(ModeDateTimeSET);
    NeedDrawCRC = 1;
    ModeDevice = MODEOTHER;
 
        // ������� ������� ������������ ���� �� Set_datetime (�����)  
    // ������� �����!
       SetNewWinIfOut = 9; // �������������� ������� ������� � ������ ���� ���� ���� �����
  //CmdInitPage(9);
    //ClrKey (BTN_OK);
  }
  break;
  case 4: // BlackLight (1)
  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    //ClrKey (BTN_RIGHT);
  }
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    //ClrKey (BTN_LEFT);
  }
  break;// BlackLight
  case 1: // Language (2)
  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_FirstScr = 1; // Need reDraw Screen
    CurrLang=GetLang(INCR);
    //ClrKey (BTN_RIGHT);
  }
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_FirstScr = 1; // Need reDraw Screen
    CurrLang=GetLang(DECR);
    //ClrKey (BTN_LEFT);
  }
  break;// Language
  case 2: //������������ (3)
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
    myBeep(10);
    SetMode(ModeSetOTDRprm);
    NeedDrawCRC = 1;
    ModeDevice = MODEOTDRPRM;
        // ������� ������� ������������ ���� �� Set_OTDRparams (�����)  
       SetNewWinIfOut = 10; // �������������� ������� ������� � ������ ���� ���� ���� �����
    //CmdInitPage(10);
    //  StartSettingBegShift (); // ����� ��������� ������� ���
        //ClrKey (BTN_OK);
  }
  break;// ������������
  case 3: //Contrast (4)
  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
//xxx    ChangeUserContr (1); // �������� ���������������� �������������
    //ClrKey (BTN_RIGHT);
  }
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
//xxx    ChangeUserContr (-1); // �������� ���������������� �������������
    //ClrKey (BTN_LEFT);
  }
  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
//xxx    ChangeUserContr (1); // �������� ���������������� �������������
    //ClrKey (BTN_RIGHT);
  }
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
//xxx    ChangeUserContr (-1); // �������� ���������������� �������������
    //ClrKey (BTN_LEFT);
  }
//  if (((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==LONG_PRESSED))&&(ChangeUserContr (0)==66))
//  {
//    myBeep(10);
//    SetMode(TetrisGame);
//    InitTetris();
//  }
//  if (((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==LONG_PRESSED))&&(ChangeUserContr (0)==67))
//  {
//    myBeep(10);
//    SetMode(ArcanoidGame);
//    InitArcanoid();
//  }
//  if (((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==LONG_PRESSED))&&(ChangeUserContr (0)==68))
//  {
//    myBeep(10);
//    SetMode(KeyTestGame);
//    
//  }
  break;// Contrast
  }

  
  if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[1][CurrLang]);
    NEX_Transmit((void*)Str);    // ���� / �����
  
  
    sprintf(Str, "t1.txt=\"Language\"���"); //!
    NEX_Transmit((void*)Str);    // ����
  
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[100][CurrLang]); //!
    NEX_Transmit((void*)Str);    // OTDR Params
  
    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[121][CurrLang]); //!
    NEX_Transmit((void*)Str);    // �������
  
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
    sprintf(Str, "t6.txt=\"%s\"���", MsgMass[0][CurrLang]);
    NEX_Transmit((void*)Str);    // English
  
    sprintf(Str, "t7.txt=\"%d\"���", 55);
    NEX_Transmit((void*)Str);    // ???
  
    // ������������� ���� ������ 
    // �������� ����������  � ��������� ���������
    sprintf(Str, "t0.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t1.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str, "t3.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t4.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);//
    sprintf(Str, "t%d.bco=GREEN���", FrSetting); // �������
    NEX_Transmit((void*)Str);// 
  
  									 // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }
    
 if(SetNewWinIfOut) // �������������� ������� ������� � ������ ���� ���� ���� �����
 {
        // ������� ������� ������������ ���� �� Set_datetime (�����)  
    CmdInitPage(SetNewWinIfOut);
    
 }
    if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    WriteNeedStruct(0x04);//SaveUserConfig();
    SetMode(ModeMainMenu);
    ModeDevice = MODEMENU;
        // ������� ������� ������������ ���� �� MainMenu (�������)  
    CmdInitPage(1);
  }
  
}

void ModeSetOTDRprm(void)// ����� ��������� ���������� ������������� ������� CHECK_IN
{
  static BYTE FrSetOTDRprm = 0; // ��������� �� ������
  char Str[32];
  int IfJumpNewWin = 0;
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetOTDRprm = ChangeFrSet (FrSetOTDRprm, 2, 0, MINUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetOTDRprm = ChangeFrSet (FrSetOTDRprm, 2, 0, PLUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_DOWN);
  }
  switch (FrSetOTDRprm)
  {
  case 0: // ��������� ���������� ������������ � ����� �������
  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    GetSetEnaEvents(1);
    //ClrKey (BTN_RIGHT);
  }
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    GetSetEnaEvents(1);
    //ClrKey (BTN_LEFT);
  }
  break;// Events
  case 1: // ��������� ������� �������
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
    myBeep(10);
    SetMode(SetBELCORE);
    ModeDevice = MODEOTHER;
        // ������� ������� ������������ ���� �� Set_OTDR_event(�����)  
    IfJumpNewWin = 11;
    //CmdInitPage(11);
    //ClrKey (BTN_OK);
  }
  break;
  case 2: //����������
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
        // ������� ������� ������������ ���� �� Calibrate (�����)  
    StartSettingBegShift (); // ����� ��������� ������� ���
        //ClrKey (BTN_OK);
        // ������� ������� ������������ ���� �� Set_OTDRparams (�������)  
    IfJumpNewWin = 14;
    //CmdInitPage(10);
  }
  break;// ����������
//  case 3:
//  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
//  {
//    myBeep(10);
//    if (FltrMod<NumFiltrs) FltrMod++;
//    else FltrMod=0;
//    //ClrKey (BTN_RIGHT);
//  }
//  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
//  {
//    myBeep(10);
//    if (FltrMod>0) FltrMod--;
//    else FltrMod=NumFiltrs;
//    //ClrKey (BTN_LEFT);
//  }
//    
//    break; // filtr
  }

  if (g_FirstScr)
  {
	  // ����� ��������� ������� ���� ������ ����������
	  // �� �������� ��������� ��� ��������� �������������
	  sprintf(Str, "t0.txt=\"%s\"���", MsgMass[111][CurrLang]);
	  NEX_Transmit((void*)Str);    // ������ �������

	  sprintf(Str, "t1.txt=\"%s\"���", MsgMass[112][CurrLang]);
	  NEX_Transmit((void*)Str);    // ��������� ������

	  sprintf(Str, "t2.txt=\"%s\"���", MsgMass[5][CurrLang]);
	  NEX_Transmit((void*)Str);    // ����������

	  g_FirstScr = 0;
	  g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
	  // ����� ��������� ������� ���� ������ ����������
	  // �� ����������� ��������� ��������� ������������ ����������
  if (GetSetEnaEvents(0)) 
	  sprintf(Str, "t3.txt=\"%s\"���", MsgMass[113][CurrLang]);
  else
	  sprintf(Str, "t3.txt=\"%s\"���", MsgMass[114][CurrLang]);
	  NEX_Transmit((void*)Str);// 

	  // ������������� ���� ������ 
	  // �������� ����������  � ��������� ���������
	  sprintf(Str, "t0.bco=WHITE���"); // �����
	  NEX_Transmit((void*)Str);// 
	  sprintf(Str, "t1.bco=WHITE���"); // �����
	  NEX_Transmit((void*)Str);// 
	  sprintf(Str, "t2.bco=WHITE���"); // �����
	  NEX_Transmit((void*)Str);// 
	  sprintf(Str, "t%d.bco=GREEN���", FrSetOTDRprm); // �������
	  NEX_Transmit((void*)Str);// 
		// ��� ��������� ��������� ������ ���� ���� ���� ������ ������
	  g_NeedScr = 0;
  }

  if(IfJumpNewWin)
  {
       CmdInitPage(IfJumpNewWin);
    IfJumpNewWin =0;
  }
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))// ������� � ���������
  {
    myBeep(10);
    SetMode(ModeSetting);
    ModeDevice = MODESETUP;
        // ������� ������� ������������ ���� �� Settings (�������)  
    CmdInitPage(5);
  }
  //ClrKey (BNS_MASK);
}
// ��������� ������� ������� � BELCORE CHECK_IN
void SetBELCORE (void)
{
  float OutSign;                         // ���������� ��� ������ ����������
  static volatile BYTE FrSetBelcore = 0; // ��������� �� ������
  char Str[25];
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetBelcore = ChangeFrSet (FrSetBelcore, 3, 0, MINUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // Need reDraw Screen
    FrSetBelcore = ChangeFrSet (FrSetBelcore, 3, 0, PLUS);// ��������� ������� � ������ �������� ����������
    //ClrKey (BTN_DOWN);
  }
  switch (FrSetBelcore) // ��������� �������� ����� ���������
  {
  case 0: // ����������� ��������� ���������
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_BC (0.1);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_BC (-0.1);
    }
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_BC (0.3);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_BC (-0.3);
    }
    //OutSign = GetSet_BC(0.0)/10.0;
    //sprintf(Str,"%2.1f%s",OutSign, MsgMass[47][CurrLang]);//��
    break;
    // ����.��������� ���������
  case 1: // ����� �������� ��������� � ����������
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_LT (0.1);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_LT (-0.1);
    }
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_LT (0.3);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_LT (-0.3);
    }
    //OutSign = GetSet_LT(0.0)/1000.0;
    //sprintf(Str,"%2.2f%s",OutSign, MsgMass[47][CurrLang]);//��
    break;
  case 2: // ����� �������� ����������� �������
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_RT (1.0);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_RT (-1.0);
    }
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_RT (3.0);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_RT (-3.0);
    }
    //OutSign = GetSet_RT(0.0)/1000.0;
    //sprintf(Str,"%2.2f%s",-OutSign, MsgMass[47][CurrLang]);//��
    break;
  case 3: // ����� �������� ����������� ����� �����
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_ET (0.1);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_ET (-0.1);
    }
    if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_ET (0.3);
    }
    if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==INF_PRESSED))
    {
      g_NeedScr = 1; // Need reDraw Screen
      GetSet_ET (-0.3);
    }
    //OutSign = GetSet_ET(0.0)/1000.0;
    //sprintf(Str,"%2.2f%s",OutSign, MsgMass[47][CurrLang]);//��
    break;
  }
if (g_FirstScr)
{
  // ����� ��������� ������� ���� ������ ����������
  // �� �������� ��������� ��� ��������� �������������
  sprintf(Str, "t0.txt=\"%s\"���", MsgMass[103][CurrLang]);
  NEX_Transmit((void*)Str);                         // (BC) �������� ���������
  
  sprintf(Str, "t1.txt=\"%s\"���", MsgMass[105][CurrLang]);
  NEX_Transmit((void*)Str);                         // (LT) ������ � �����
  
  sprintf(Str, "t2.txt=\"%s\"���", MsgMass[107][CurrLang]);
  NEX_Transmit((void*)Str);                         // (RT) �����. ��������� ���������
  
  sprintf(Str, "t3.txt=\"%s\"���", MsgMass[109][CurrLang]);
  NEX_Transmit((void*)Str);                         // (ET) ����� �����
  
  
  g_FirstScr = 0;
  g_NeedScr = 1;
}
if (g_NeedScr)
{
  // ����� ��������� ������� ���� ������ ����������
  // �� ����������� ��������� ��������� ������������ ����������

  OutSign = GetSet_BC(0.0)/10.0;
  sprintf(Str, "t4.txt=\"%2.1f%s\"���",OutSign, MsgMass[47][CurrLang]);
  NEX_Transmit((void*)Str);                         // ...
  
  OutSign = GetSet_LT(0.0)/1000.0;
  sprintf(Str, "t5.txt=\"%2.2f%s\"���", OutSign, MsgMass[47][CurrLang]);
  NEX_Transmit((void*)Str);                         // ...
  
  OutSign = GetSet_RT(0.0)/1000.0;
  sprintf(Str, "t6.txt=\"%2.1f%s\"���",-OutSign, MsgMass[47][CurrLang]);
  NEX_Transmit((void*)Str);                         // ..
  
  OutSign = GetSet_ET(0.0)/1000.0;
  sprintf(Str, "t7.txt=\"%2.2f%s\"���",OutSign, MsgMass[47][CurrLang]);
  NEX_Transmit((void*)Str);                         // ..
  // ������������� ���� ������ 
  // �������� ����������  � ��������� ���������
  sprintf(Str, "t0.bco=WHITE���"); // �����
  NEX_Transmit((void*)Str);// 
  sprintf(Str, "t1.bco=WHITE���"); // �����
  NEX_Transmit((void*)Str);// 
  sprintf(Str, "t2.bco=WHITE���"); // �����
  NEX_Transmit((void*)Str);// 
  sprintf(Str, "t3.bco=WHITE���"); // �����
  NEX_Transmit((void*)Str);//
  sprintf(Str, "t%d.bco=GREEN���", FrSetBelcore); // �������
  NEX_Transmit((void*)Str);// 
  
  								   // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
  g_NeedScr = 0;
}
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))// ������� � ���������
  {
    myBeep(10);
    CheckReflParam ();  // �������� ���������������� �������� 
    
    //FlashErasePage(EVEN_SET); // ������ �������� ��������� ������������ �������
    //FlashWritePageSM(EVEN_SET, StructPtr(EVEN_SET), StructSize(EVEN_SET), 0);
    WriteNeedStruct(0x08);
    SetMode(ModeSetOTDRprm);
    ModeDevice = MODEOTDRPRM;
        // ������� ������� ������������ ���� �� Set_OTDRparams (�������)  
    CmdInitPage(10);
    //myBeep(10); // �����, ��������� 3
  }
  
}


void ModeDateTimeSET(void) // ����� ��������� ������� CHECK_IN
{
  //static BYTE PosCurr = 6; // ��������� ������� ��������������� �������
  static RTCTime NowTime;
  char Str[32];
  char St[5];
  static DWORD old_sec;
  static DWORD new_sec;
  //BYTE CurrLang=GetLang(CURRENT);
  //DWORD KeyP = SetBtnStates( KEYS_REG, 1 );
  if (PosCurr == 6)  NowTime=RTCGetTime(); // ��������� �����
  new_sec = NowTime.RTC_Sec;
  if(new_sec != old_sec)
  {
   old_sec = new_sec;
   g_NeedScr = 1;
  }

  if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // ���� ������������ ��� �� �� ����� ����������
    if (PosCurr>0) PosCurr--;
    else PosCurr=5;
    //ClrKey (BTN_RIGHT);
  }
  if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
  {
    myBeep(10);
    g_NeedScr = 1; // ���� ������������ ��� �� �� ����� ����������
    if (PosCurr<5) PosCurr++;
    else PosCurr=0;
    //ClrKey (BTN_LEFT);
  }
  // ��������� �������
  if ((PRESS(BTN_UP))&&((getStateButtons(BTN_UP)==SHORT_PRESSED)||(getStateButtons(BTN_UP)==INF_PRESSED)))
  {
    myBeep(10);
    g_NeedScr = 1; // ���� ������������ ��� �� �� ����� ����������
    switch (PosCurr)
    {
    case 0:// �������� ������
      if (NowTime.RTC_Sec<59) NowTime.RTC_Sec++;
      else NowTime.RTC_Sec = 0;
      break;
    case 1:// �������� �����
      if (NowTime.RTC_Min<59) NowTime.RTC_Min++;
      else NowTime.RTC_Min = 0;
      break;
    case 2:// �������� �����
      if (NowTime.RTC_Hour<23) NowTime.RTC_Hour++;
      else NowTime.RTC_Hour = 0;
      break;
    case 3:// �������� ���� ������
      if (NowTime.RTC_Mday<31) NowTime.RTC_Mday++;
      else NowTime.RTC_Mday = 1;
      break;
    case 4:// ��������  ������
      if (NowTime.RTC_Mon<12) NowTime.RTC_Mon++;
      else NowTime.RTC_Mon = 1;
      break;
    case 5:// ��������  ����
      if (NowTime.RTC_Year<2050) NowTime.RTC_Year++;
      else NowTime.RTC_Year = 2024;
      break;
    }
    //ClrKey (BTN_UP);
  }
  if ((PRESS(BTN_DOWN))&&((getStateButtons(BTN_DOWN)==SHORT_PRESSED)||(getStateButtons(BTN_DOWN)==INF_PRESSED)))
  {
    myBeep(10);
    g_NeedScr = 1; // ���� ������������ ��� �� �� ����� ����������
    switch (PosCurr)
    {
    case 0:// �������� ������
      if (NowTime.RTC_Sec>0) NowTime.RTC_Sec--;
      else NowTime.RTC_Sec = 59;
      break;
    case 1:// �������� �����
      if (NowTime.RTC_Min>0) NowTime.RTC_Min--;
      else NowTime.RTC_Min = 59;
      break;
    case 2:// �������� �����
      if (NowTime.RTC_Hour>0) NowTime.RTC_Hour--;
      else NowTime.RTC_Hour = 23;
      break;
    case 3:// �������� ���� ������
      if (NowTime.RTC_Mday>1) NowTime.RTC_Mday--;
      else NowTime.RTC_Mday = 31;
      break;
    case 4:// ��������  ������
      if (NowTime.RTC_Mon>1) NowTime.RTC_Mon--;
      else NowTime.RTC_Mon = 12;
      break;
    case 5:// ��������  ����
      if (NowTime.RTC_Year>2025) NowTime.RTC_Year--;
      else NowTime.RTC_Year = 2050;
      break;
    }
    //ClrKey (BTN_DOWN);
  }
  
  if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
  {
  myBeep(10);
  g_NeedScr = 1; // ���� ������������ ��� �� �� ����� ����������
  RTCSetTime( NowTime );
//  if (GetID_Dev()==0)
//  {
//  sprintf(Str,"(%04X)", CalkCheckSum()); // 
//  putString(2,42,Str,1,0);
//  }
  PosCurr = 7;
  //ClrKey (BTN_OK);
   // 18.10.2011 - ��������� CRC ��� ����� � �������� �����
    if (NeedDrawCRC)
  {

  if (GetID_Dev()==0)
  {
  GetNumVer(St);
  }
  NeedDrawCRC=0;
  }
 
  }
  // ���������� ������ ����������
if(g_FirstScr)
{
  // ����� ��������� ������� ���� ������ ����������
  // �� �������� ��������� ��� ��������� �������������
      sprintf(Str,"t0.txt=\"%s\"���", MsgMass[65][CurrLang]); // "����"" 
    NEX_Transmit((void*)Str);// 
      sprintf(Str,"t1.txt=\"%s\"���", MsgMass[15][CurrLang]); // "�����"
    NEX_Transmit((void*)Str);// 
      sprintf(Str,"t2.txt=\"/\"���"); // "�����"
    NEX_Transmit((void*)Str);// 
      sprintf(Str,"t3.txt=\"/\"���"); // "�����"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t4.txt=\":\"���"); // "���������"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t5.txt=\":\"���"); // "���������"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t6.txt=\" \"���"); // "������ ������ (������)"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t7.txt=\" \"���"); // "������ ������ (������)"
    NEX_Transmit((void*)Str);// 

  g_FirstScr=0;
  g_NeedScr=1;
}
if(g_NeedScr)
{
   // ����� ��������� ������� ���� ������ ����������
  // �� ����������� ��������� ��������� ������������ ����������
    sprintf(Str,"n5.val=%d���",NowTime.RTC_Year%100); // "���"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n4.val=%d���",NowTime.RTC_Mon); // "�����"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n3.val=%d���",NowTime.RTC_Mday); // "����"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n2.val=%d���",NowTime.RTC_Hour); // "����"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n1.val=%d���",NowTime.RTC_Min); // "������"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n0.val=%d���",NowTime.RTC_Sec); // "�������"
    NEX_Transmit((void*)Str);// 
 
    
 
  // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    // �������� ����������  � ��������� ���������
  // �������� ���� ��������������
    sprintf(Str,"n0.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n1.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n2.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n3.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n4.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"n5.bco=WHITE���"); // �����
    NEX_Transmit((void*)Str);// 
  if(PosCurr<6)
  {
    sprintf(Str,"n%d.bco=GREEN���",PosCurr); // �������
    NEX_Transmit((void*)Str);// 
  }
  g_NeedScr = 0;
  if(PosCurr==7)
  {
      GetNumVer(St);
    sprintf(Str,"t6.txt=\"v%s (%04X)\"���",St, CalkCheckSum()); // "������ � ����������� �����"
    NEX_Transmit((void*)Str);// 
    sprintf(Str,"t7.txt=\"%s\"���", MsgMass[64][CurrLang]); // Up-to DATE"
    NEX_Transmit((void*)Str);// 

  }
}
  
  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
  {
    myBeep(10);
    SetMode(ModeSetting);
    ModeDevice = MODESETUP;
        // ������� ������� ������������ ���� �� Settings (�������)  
    CmdInitPage(5);
  }
  
  
}

void ModeCalibrate(void) // ����� ��������� ���������� ��������
{
  SetIndexIM (3); // ������������� ������ ������������ �������� � �������������� �� ������ (500��
  SetIndexLN (0); // �������� ����� (2��)
  PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
  PointInPeriod = 0;
  memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
  myBeep(0);
//123  disable_timer ( 0 );
//123    reset_timer(2);
//123    enable_timer(2);
  CurrTimeAccum = 0;
  EnaTimerAccum = 1;
  
  Averaging(200,0,0);// ��������� ������� � ������ ������������� ���������� 
  for (int i=0; i<NUMSHIFTZONE; ++i)
  {
    if (i<7) SetIndexLN (i);
    if ((i>6)&&(i<12))
    {
      SetIndexLN (i-5); 
      SetIndexIM (5);
    }
    if (i==12)
    {
      SetIndexLN (6); 
      SetIndexIM (6);
    }
    PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
    memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
//123    reset_timer(2); // ���������� �������������� ������!
    //123 enable_timer(2);
    CurrTimeAccum = 0;
    EnaTimerAccum = 1;
    
    Averaging(300,0,0);// ��������� ������� � ������ ������������� ���������� 
    DWORD Data = SetBegShiftZone (i, SearchShiftBeg (500));// ������ ������� ���� �� �������
    DrawCalibrating(Data);
    g_NeedScr=1;
    
  }
  SetIndexLN (0); 
  SetIndexIM (0);
  HV_LOW(ON); //ON LOW HIGH VOLT
  HV_SW(OFF); // OFF HIGH VOLT
  POWDET(OFF);
  POWREF (OFF);
  //123 SSPInit_Any(MEM_FL1); // �������������� ������������� SSP ��� ���������� ������� FLASH (���� 1 �� ��� �� ����� ����������)
  WriteNeedStruct (0x04);
//123  FlashErasePage(CFG_USER); // ������ �������� ��������� ������������ �������
//123  FlashWritePageSM(CFG_USER, StructPtr(CFG_USER), StructSize(CFG_USER), 0);
  SetMode(ModeSetOTDRprm);
  ModeDevice = MODEOTDRPRM;
  myBeep(20);
  SubModeMeasOTDR = NOTMODE;
  //123 enable_timer ( 0 );
  // ������� � ���� ��������� ��� �������������
          // ������� ������� ������������ ���� �� Set_datetime (�����)  
  CmdInitPage(10);

  //ClrKey (BNS_MASK); // ����� ������� ������
}

//// ������� �����
//void FrBig(unsigned char SW,  char Add)
//{
//    //  char CmdBuf[30];
//    //  DrawLine(0,0+SW*12,76,0+SW*12,12,0,screen); // ������� ���� ,��������
//    // DrawLine(80,0+SW*12,124,0+SW*12,12,0,screen);
//    if (((76+Add)>127)||((76+Add)<0)) Add = 0;
//
//    DrawLine(0,4+SW*12,76+Add,4+SW*12,1,1);
//    DrawLine(0,4+SW*12,0,15+SW*12,1,1);
//    DrawLine(0,15+SW*12,76+Add,15+SW*12,1,1);
//    DrawLine(76+Add,4+SW*12,76+Add,15+SW*12,1,1);
//    // �� ������� �������� ����� �� ����������
//    //1. ������� 2. �������� �����
//  //sprintf( CmdBuf,"cls WHITE���"); // 0xff
//  //NEX_Transmit((void*)CmdBuf);// 
//  //sprintf( CmdBuf,"page 0���"); // 0xff
//  //NEX_Transmit((void*)CmdBuf);// 
//  //sprintf( CmdBuf,"draw %d,%d,%d,%d,RED���",0,4+SW*12,76+Add,15+SW*12); // 0xff
//  //NEX_Transmit((void*)CmdBuf);// 
//
//}
//// ��������� �����
//void FrSmall(unsigned char SW ,  char Add) // �� ������ ���� (09.10.2013)
//{
//    if (((80+Add)>127)||((80+Add)<0)) Add = 0;
//    DrawLine(80+Add,4+SW*12,127,4+SW*12,1,1);
//    DrawLine(80+Add,4+SW*12,80+Add,15+SW*12,1,1);
//    DrawLine(80+Add,15+SW*12,127,15+SW*12,1,1);
//    DrawLine(127,4+SW*12,127,15+SW*12,1,1);
//}
//// ����� ������������� ������� � �����
//void FrFree(unsigned char SW ,  int X, int Size)
//{
//    if (((X+Size)>0)&&((X+Size)<128))
//    {
//    DrawLine(X,4+SW*12,X+Size,4+SW*12,1,1);
//    DrawLine(X,4+SW*12,X,15+SW*12,1,1);
//    DrawLine(X,15+SW*12,X+Size,15+SW*12,1,1);
//    DrawLine(X+Size,4+SW*12,X+Size,15+SW*12,1,1);
//    }
//  
//
//}
//
BYTE ChangeFrSet (BYTE FrSet, BYTE MaxSet, BYTE MinSet, BYTE DirSet)// ��������� ������� � ������ �������� ����������
{
  if (DirSet) // �����
  {
    if (FrSet < MaxSet) FrSet++;
    else FrSet = MinSet;
  }
  else
  {
    if (FrSet > MinSet) FrSet--;
    else FrSet = MaxSet;
  }
   return FrSet; 
}

// ������ �������� � ������� ����
BYTE DrawSourceVFL (BYTE* frameInx, char* Str, DWORD KeyP, BYTE Lang, BYTE FrmMax, BYTE FrmMin)
{
    BYTE RedEye = GetCfgRE();
#if !ENADRAWORL
  {
    switch (*frameInx-FrmMax)
    {
    case 0: // Source
      if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        GetPlaceLS(NEXT);
        //ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetModeLS(Str,INCR,Lang );
        //ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetModeLS(Str,DECR,Lang );
        //ClrKey (BTN_LEFT);
      }
      break;
    case 1: // VFL
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetModeRE(Str,INCR,Lang );
        //ClrKey (BTN_RIGHT);
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
      {
        myBeep(10);
        g_NeedScr = 1; // Need reDraw Screen
        SetModeRE(Str,DECR,Lang );
        //ClrKey (BTN_LEFT);
      }
      break;
    }
    if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      *frameInx = ChangeFrSet (*frameInx, FrmMax+RedEye, FrmMin, MINUS);// ????????? ??????? ? ?????? ???????? ??????????
      //ClrKey (BTN_UP);
    }
    if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      *frameInx = ChangeFrSet (*frameInx, FrmMax+RedEye, FrmMin, PLUS);// ????????? ??????? ? ?????? ???????? ??????????
      //ClrKey (BTN_DOWN);
    }
    

  }
  
#endif
  
#if ENADRAWORL
  // c ORL
  {
    switch (*frameInx-FrmMax)
    {
    case 0: // Source
      if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
      {
        myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
      GetPlaceLS(NEXT);
      }
      if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
      {
        myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
          SetModeLS(Str,INCR,Lang );
      }
      if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
      {
        myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
          SetModeLS(Str,DECR,Lang );
      }
      break;
    case 1: // ������ ��� �� ������ ORL �����
      if ((PRESS(BTN_OK))&&(getStateButtons(BTN_OK)==SHORT_PRESSED))
      {
        myBeep(20);
        GetPlaceLS(CURRENT);
        PreSetModeLS(0); // ����� ��� ��������� ��������!
        SetModeLS(Str,0,Lang ); // ������������� ���������
        SetupSource (GetModeLS()); // ��������� ������ ������ ������  
        // ����� ������� ��������� ORL 
        g_VolORL = MeasORL (1000, 1);
      g_NeedScr = 1; // Need reDraw Screen
        myBeep(20);
      }
      //    }
      break;
    case 2: // VFL ����� ������ �������� ?
      if(FrmMax==1)
      {
        if ((PRESS(BTN_RIGHT))&&(getStateButtons(BTN_RIGHT)==SHORT_PRESSED))
        {
          myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
          SetModeRE(Str,INCR,Lang );
        }
        if ((PRESS(BTN_LEFT))&&(getStateButtons(BTN_LEFT)==SHORT_PRESSED))
        {
          myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
          SetModeRE(Str,DECR,Lang );
        }
      }
      break;
    }
    if ((PRESS(BTN_UP))&&(getStateButtons(BTN_UP)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
        if(FrmMax==1)
   *frameInx = ChangeFrSet (*frameInx, FrmMax+1+RedEye, FrmMin, MINUS);// ????????? ??????? ? ?????? ???????? ??????????
    else
   *frameInx = ChangeFrSet (*frameInx, FrmMax+1, FrmMin, MINUS);// ????????? ??????? ? ?????? ???????? ??????????
    }
    if ((PRESS(BTN_DOWN))&&(getStateButtons(BTN_DOWN)==SHORT_PRESSED))
    {
      myBeep(10);
      g_NeedScr = 1; // Need reDraw Screen
        if(FrmMax==1)
   *frameInx = ChangeFrSet (*frameInx, FrmMax+1+RedEye, FrmMin, PLUS);// ????????? ??????? ? ?????? ???????? ??????????
    else
   *frameInx = ChangeFrSet (*frameInx, FrmMax+1, FrmMin, PLUS);// ????????? ??????? ? ?????? ???????? ??????????
    }

    
  }
#endif
  return 1;
}

BYTE Measuring ( void)
{
  return SubModeMeasOTDR;
}

int SearchShiftBeg (int Size)// ����� ������� ���� 
{
  DWORD LocalMax=0;
  static int i;
  int IndexLocMax=0;
  char EndCalc = 0;
  for ( i= 0; i<Size; i++)// ����� ���������� ��������
  {
    if( RawData[i]>LocalMax)
    {
      LocalMax = RawData[i];
      IndexLocMax = i;
    }
  }
  for ( i = IndexLocMax ;((i>0)&&(!EndCalc));i--)
  {
    if ((LocalMax-RawData[i]) > 6*RawData[i]) // ��������� ����� ������� ������� �� "�������" ��������
      
      EndCalc = 1;
  }
    return (i+2);// ���������� �.�. ��������� �������� ������ ������!
  
}

//void (*ModeFunc)(void);

void DrawCalibrating (DWORD Data)// ��������� ���� ����������
{
  char Str[22];
  //BYTE CurrLang;
  //CurrLang=GetLang(CURRENT);
  // ����� �������� ��� ������ ���������
    if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[28][CurrLang]);
    NEX_Transmit((void*)Str);    // 
  
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[40][CurrLang]);
    NEX_Transmit((void*)Str);    // 
  
    sprintf(Str, "t2.txt=\"%s\"���", MsgMass[19][CurrLang]); //!
    NEX_Transmit((void*)Str);    // 
  
    sprintf(Str, "t3.txt=\"%s\"���", MsgMass[22][CurrLang]); //!
    NEX_Transmit((void*)Str);    // 
  
  
    g_FirstScr = 0;
    g_NeedScr = 1;
  }
  if (g_NeedScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� ����������� ��������� ��������� ������������ ����������
    
    sprintf(Str, "t4.txt=\"%d%s\"���", GetLengthLine(GetIndexLN()),MsgMass[20][CurrLang]); //!
    NEX_Transmit((void*)Str);    // 
    
     sprintf(Str, "t5.txt=\"%d%s\"���", GetWidthPulse(GetIndexIM()),MsgMass[23][CurrLang]); //!
    NEX_Transmit((void*)Str);    // 
  
    sprintf(Str, "t6.txt=\"%d\"���",Data);
    NEX_Transmit((void*)Str);    // 
  
  
  									 // ��� ��������� ��������� ������ ���� ���� ���� ������ ������
    g_NeedScr = 0;
  }

}

BYTE IndexSeek( int Data )// ����� ������� ����� ��� �������� ����� �����
{
  int out = 0;
  for(;(Data >= KeyPoints[out])&&(out<PNTSNUM-1); ++out);
  return out;
}

// ��������� ������������ �� ����� ����� �����
DWORD Scan( DWORD* array, unsigned size, DWORD level )
{
  const unsigned step = 10;
  const unsigned inStep = 3;
  //unsigned cntr;
  //unsigned short curPart;
  for( unsigned i = size-1; i >= 90;)
  {
    if( array[i] > level )
    {
      unsigned second = 0;
      unsigned cntr = 1;
      for( unsigned j = 1; j < step; ++j )
      {
        if( array[i-j] > level )
        {
          if(second == 0) second = j;
          if( ++cntr == inStep )
            return i;
        } 
      }  
      i -= (second ? second : step);
    }
    else
      i -= 1;
  } 
  return 90;
}

float GetPosLine (unsigned Cursor) // ��������� ����� �� ������� �������
{
  float PosLine;
  // ��������� ����� �������� ���������� ����� (��� ������ �����)
  PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
 //ValueDS = (unsigned)((ADCPeriod*50000)/PointsPerPeriod); //  ������������� �������� DS ��� �������������� ������ ���������
  ValueDS = GetValueDS(); //  ������������� �������� DS ��� �������������� ������ ���������

  PosLine = (float)(LIGHTSPEED*1.0E-14);
  PosLine = PosLine*ValueDS;
  PosLine = PosLine/GetIndexWAV();
  PosLine = PosLine*Cursor;
  PosLine = PosLine/1000.0;
  return PosLine;
  
}

// off 02/11/2023
// ���������� ���������� � ������������� �������� �� ������
//void Draw_KeyBoard(unsigned char Cursor, char* Str_first, char* Str_edit, BYTE Shift ,BYTE Lang)
//{
//  char CommScr[20] = "                   \0"; // ������ ��������� ��� ������ �� �����
//  unsigned char i;
//  ClearScreen();
//  //Str_edit[ARRAY_SIZE(Str_edit)-1]=0; // ��������� ������� � ������� ����� 0
//  strcpy(CommScr,Str_edit);
//  if (CommScr[Cursor+1] != 0) // �� ����� ������
//  {
//    CommScr[Cursor]=0x5F; // ������ �� ��������� �����
//  }
//  else
//  {   
//  if (CommScr[Cursor] == ' ')CommScr[Cursor]=0x5F; // ������ �� ��������� �����
//  }
//  putString(2,10,CommScr,1,0);
//  //  sprintf(F_name,"%02X%02X%02X_%02X%02X%X.sor",ds1337[6],ds1337[5],ds1337[4],ds1337[2],ds1337[1],ds1337[0]>>4);
//  putString(0,2,Str_first,1,0);
//
//  
//  
//  for (i=0; i<48; i++)
//  {
//    putChar((i%12)*10, 24+((int)(i/12))*10,Tab_Kb[i+Shift*48+Lang*96],0,1,0);
//  }
//  Knob(KbPosX,KbPosY);
//}
//
//void Knob(unsigned char X,unsigned char Y)
//{
//  
//    DrawLine(X*10,Y*10+23,X*10+10,Y*10+23,1,1);
//    DrawLine(X*10,Y*10+23,X*10,Y*10+33,1,1);
//    DrawLine(X*10,Y*10+33,X*10+10,Y*10+33,1,1);
//    DrawLine(X*10+10,Y*10+23,X*10+10,Y*10+33,1,1);
// 
//}
//
BYTE GetEnaMZ (void) // ��������� �������� �������� ���������� ������������� ������� ���
{
  return EnaCorrMZ;
}
BYTE SetEnaMZ (BYTE Mode) //  ���������  �������� ���������� ������������� ������� ���
{
  if(Mode)
    EnaCorrMZ=1;
  else
    EnaCorrMZ=0;
  return EnaCorrMZ;
}
//
BYTE GetCurrentModeDevice (void) // ��������� �������� ������ �������
{
  return ModeDevice;
}

BYTE GetModePowerMeter (void) // ��������� �������� ������ ����������
{
  return ModePowerMeter;
}

BYTE SetModeDevice (BYTE Mode) // �������������� ��������� ������ �������
{
  switch (Mode)
  {
  case MODESETREFL: // ����� ��������� ������������
    SetIndexLN(GetIndexLN());//������������� ������� ��������� �� ������ � ���������
    PosCursorMain (-4100); // ����� ������� � ������
    IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
    GetSetHorizontScale (5); // ����� �������� ��� ����� �������
    // ��������� �������� ���. ��������
    PreSetModeLS(0);
    SetupSource (GetModeLS()); // ��������� ������ ������ ������  
    SetMode(ModeSetupOTDR);
    GetSetModeLW(-1); // ����� �������� ��� ��� �������������� ���������
    ModeDevice = MODESETREFL;
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(OFF); // OFF HIGH VOLT
    POWREF (OFF);
    POWDET(OFF);
//123    SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
      CmdInitPage(2);// ������� ������� ������������ ���� �� OTDR
    
    break;
  case MODEREFL :// ����� �������������
    //TraceREAD(0);// ����� �������� ����������� 0 �������������� �� � ������
    memset( LogData, 65535, OUTSIZE * sizeof(unsigned short) ); // ZERO_ARRAY( array ) memset( array, 0, sizeof(array) )
    SetMode(ModeDrawOTDR);
    ReturnModeViewRefl = SETPARAM;//VIEWMEM -  ����� ��������� � � ���������
    // ��������� �������� ���. ��������
    PreSetModeLS(0);
    SetupSource (GetModeLS()); // ��������� ������ ������ ������  
    ModeDevice = MODEREFL;
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(OFF); // OFF HIGH VOLT
    POWREF (OFF);
    POWDET(OFF);
//123    SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
    CmdInitPage(18);// ������� ������� ������������ ���� �� OTDR ��������
    break;
  case MODEMEMR:// ����� ������ � ������� �������������
    SetMode(ModeMemoryOTDR);
    // ��������� �������� ���. ��������
    PreSetModeLS(0);
    SetupSource (GetModeLS()); // ��������� ������ ������ ������  
    ModeDevice = MODEMEMR;
    ModeMemDraw = VIEWNEXT;
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(OFF); // OFF HIGH VOLT
    POWREF (OFF);
    POWDET(OFF);
    //123 SSPInit_Any(MEM_FL1); // ������������� SSP ��� FLASH (���� 1 �� ��� �� ����� ����������)
      CmdInitPage(13);// ������� ������� ������������ ���� �� OTDR ������
    break;
  case MODETEST:// ����� �������
    POWDET(ON); // �������� ������� ����������
    if (GetCfgPM ())
    {
      SetMode(ModeMeasManualOLT);
          CmdInitPage(7);// ������� ������� ������������ ���� �� ������
    }
    else 
    {
      SetMode(ModeSourceOnly); // ����� ������ (������ ��������)
          CmdInitPage(7);// ������� ������� ������������ ���� �� ������
    }
    ModeDevice = MODETEST;
    //123 SSPInit_Any(SPI_PM); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
    break;
  case MODETESTMEM:// ����� ������� ������
    if (GetCfgPM ())
    {
      POWDET(ON); // �������� ������� ����������
      SetMode(ModeViewMemOLT);
      //123 SSPInit_Any(MEM_FL1); // ������������� SSP ��� FLASH (���� 1 �� ��� �� ����� ����������)
      ModeDevice = MODETESTMEM;
      CmdInitPage(20);// ������� ������� ������������ ���� �� ������ �������
    }
    else 
    {
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(OFF); // OFF HIGH VOLT
    POWREF (OFF);
    POWDET(OFF);
    SetMode(ModeMainMenu);
    ModeDevice = MODEMENU;
      CmdInitPage(1);// ������� ������� ������������ ���� �� MainMenu
    }
    break;
  case MODESETUP:// ����� ��������� ������������
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(OFF); // OFF HIGH VOLT
    POWREF (OFF);
    POWDET(OFF);
    SetMode(ModeSetting);
    ModeDevice = MODESETUP;
    //123 SSPInit_Any(MEM_FL1); // ������������� SSP ��� FLASH (���� 1 �� ��� �� ����� ����������)
      CmdInitPage(5);// ������� ������� ������������ ���� �� Setting

    break;
  case MODEMEASURE:// ����� ��������� ������������� (����������)
    CmdInitPage(16);// ������� ������� ������������ ���� �� ������ ��������� Measuring
    SlowON();
    //POWALT(ON);
    //POWREF (ON);
    //POWDET(ON);
    //IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
    SubModeMeasOTDR = SETPOWER;
    SetMode(ModeStartOTDR);
    ModeDevice = MODEMEASURE;
    //123 SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
    
    break;
  default:// ����� ����
    HV_LOW(ON); //ON LOW HIGH VOLT
    HV_SW(OFF); // OFF HIGH VOLT
    POWREF (OFF);
    POWDET(OFF);
    SetMode(ModeMainMenu);
    ModeDevice = MODEMENU;
    //123 SSPInit_Any(MEM_FL1); // ������������� SSP ��� FLASH (���� 1 �� ��� �� ����� ����������)
    //  CmdInitPage(1);// ������� ������� ������������ ���� �� MainMenu
    CmdInitPage(1);// ������� ������� ������������ ���� MainMenu
    break;
  }
  return ModeDevice;
}

void StartSettingBegShift (void) // ����� ��������� ������� ���
{
        myBeep(10);
        SlowON();
        HV_LOW(ON); //ON LOW HIGH VOLT
        HV_SW(ON); // ON HIGH VOLT
        //POWALT(ON);
        //POWREF (ON);
        //POWDET(ON);
        //123 SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
        SetIndexIM (0);
        SetIndexLN (0);
        PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
        PointInPeriod = 0;
        memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
        SetNumAccumPerSec (200);// ��������� �������� ����� ����������
        SubModeMeasOTDR = AVERAGING;
        for (int i = 0; i< NUMSHIFTZONE; ++i)
        {
         SetBegShiftZone (i, 0);// ������ ������� ���� = 0
        }
        SetMode(ModeCalibrate);
        ModeDevice = MODEOTHER;
}

void InitAutoPM (void) // ��������� ��������� ���������� ��������
{
      ModAutoPM = WAITING; // ����� ���������� ���� �������
      IndWavePON = 0; //������ ����� ����� � ������� ������������� ����
      TimerAutoPM = TimerPA(1); // ������� ������� (��������)
      for(int i=0; i<5; i++) 
      {
        RPON[i]=-100.0;
        LWPON[i] = 0; // �������� ������ ���� ����
        LWCNT[i] = 0; // clear cnt 
      }
      SetSwitchPMMode(MANUAL);  // ������������� ����� ������������ ���������� � ������
      SetRange(1);
}

void SavePowerMeter(float Pow_mW)// ������� ���������� � ������ ���������
{
  char Str[20];
  int j=0;
  for (int i=0; i<3; i++)
  {
    PONI.LenWaveKlb[i]=0;
    PONI.PowLevel[i]=-100.0;
    PONI.BaseLvl[i]=-100.0;
  }
    PONI.CellMod=0;
    TimeSaveOLT = RTCGetTime(); // ��������� ����� �����
    PONI.TotalTimeCell = TotalSec( TimeSaveOLT); // ������� �������// ������� ������ ������� � ���
    PONI.CommUserPM[ARRAY_SIZE(PONI.CommUserPM)-1]=0; // ��������� ������� � ������� ����� 0
      for (int Ind =ARRAY_SIZE(PONI.CommUserPM)-2; Ind>=0; Ind--)
      {
        if (PONI.CommUserPM[Ind]<0x20) PONI.CommUserPM[Ind]=' ';
        //if (PONI.CommUserPM[Ind]!=' ')break;
        //Index_Comm --;
      }
  switch (ModePowerMeter)
  {
case POWMANUAL:
  // ��������� ��������� �������� ���������
    PONI.LenWaveMeas =  GetPMWavelenght(0);
    PONI.CellMod = GetTypeRslt();
    PONI.PowLevel[0]= Watt2dB(Str, Pow_mW,0); //dBm
    PONI.PowLevel[1]= Watt2dB(Str, Pow_mW,1); //dB
    PONI.PowLevel[2]= Pow_mW;
    PONI.BaseLvl[0] = GetCurrLvldB(0);
    //if (!En_Inc)  PONI.NumFix++;  // ��� ���� ������ ������ ����� ���������
    PONI.Rez = MANUAL;
    /*
  if (Cell_Mem<MaxMem) Cell_Mem++;
  else 
  {
    Cell_File = Cell_File + Cell_Mem;
    Cell_Mem = 0;
  }
    */
  break;
  case POWAUTO:
    // ������ � ������
    PONI.Rez = AUTO;
        for (int i=0; i<5; i++)
        {
          if ((RPON[i]>-100.0)&&(j<3)) // ���� �������� ��� ���������
          {
              PONI.PowLevel[j]=RPON[i];
              PONI.BaseLvl[j]=GetCurrLvldB(CoeffPM.PointKalib[i]) ;
              //PONI.LenWaveKlb[j]=CoeffPM.PointKalib[i]; // LWPON[i]
              PONI.LenWaveKlb[j]=LWPON[i]; // LWPON[i] 1300

              j++;
          }
        }

    break;
  default:
    break;
    
  }
    //123 SSPInit_Any(MEM_FL1); // ������������� SSP ��� ���������� FLASH (���� 1 �� ��� �� ����� ����������)
    //123 FlashErasePage(CFG_USER); // ������ �������� ��������� ������������ �������
    //123 FlashWritePageSM(CFG_USER, StructPtr(CFG_USER), StructSize(CFG_USER), 0);
    //123 SSPInit_Any(SPI_PM); // ������������� SSP ��� ���������� PM (���� 1 �� ��� �� ����� ����������)
  WriteNeedStruct(0x04);
}

void WriteMemPow(void) // ������ � ������ ���������������
{
  WORD TmpCellMem = GetCellMem(0);
    //123 SSPInit_Any(MEM_FL1); // ������������� SSP ��� ���������� FLASH (���� 1 �� ��� �� ����� ����������)
  
if (TmpCellMem < MaxMemPM)
      {
//123!!!      WriteCellIzm(TmpCellMem, (unsigned char*)&PONI);
      if (TmpCellMem < MaxMemPM) GetCellMem(1);
      if (GetEnIncFiber(0))    // ��� ���� ������ ������ ����� ���������
      {
        if (PONI.NumFix<9999) PONI.NumFix++;
        else PONI.NumFix=0;
      }
    //FlashErasePage(CFG_USER); // ������ �������� ��������� ������������ �������
    //FlashWritePageSM(CFG_USER, StructPtr(CFG_USER), StructSize(CFG_USER), 0);
    WriteNeedStruct(0x04);
      }
    else myBeep(20);          
    ReturnToTester (1); // ������� � ����������
    //123 SSPInit_Any(SPI_PM); // ������������� SSP ��� ���������� PM (���� 1 �� ��� �� ����� ����������)
  
}

void ReturnToTester (BYTE EnClr) // ������� � ����������( ����� �� ������� ���� ������ ���������� ���� ���������)
{
      switch (ModePowerMeter)
    {
    case POWMANUAL:
    SetMode(ModeMeasManualOLT);
          // ������� ������� ������������ ���� �� Tester (�������)  
      CmdInitPage(7);
      break;
    case POWAUTO:
      if (EnClr) InitAutoPM (); // ��������� ��������� ���������� ��������
    SetMode(ModeMeasAutoOLT);
    TimerPA (1); // ����� �������1
          // ������� ������� ������������ ���� �� Tester (�������)  
      CmdInitPage(8);
      break;
      // ���� ������ � ������ �� ������� UART �� ��� �� �����
    default:
    SetMode(ModeMeasManualOLT);
          // ������� ������� ������������ ���� �� Tester (�������)  
      CmdInitPage(7);
      break;
      
    }
    ModeDevice = MODETEST;
}

void  DrawLevelStr(char* Str) //???? ����� �������� � ��� ������� W/ dB/ dBm
{
  //char Watt[5];
         switch (PONI.CellMod)
    {
    case 0: // ���
    case 1: // ��
  switch ( GetCfgPM ()) // ��������� ��������� ����������
  {
    case 1:
      sprintf(Str,"%s  %.2f", MsgMass[48][CurrLang],PONI.PowLevel[0]);//���
      if (PONI.PowLevel[0] <= -85.0)
      {
      sprintf(Str,"%s  <%.2f", MsgMass[48][CurrLang], -85.0);//���
      }
      break;
    case 2:
      sprintf(Str,"%s  %.2f", MsgMass[48][CurrLang],PONI.PowLevel[0]);//���
      if (PONI.PowLevel[0] <= -60.0)
      {
      sprintf(Str,"%s  <%.2f", MsgMass[48][CurrLang],-60.0);//���
      }
      break;
    default:
      sprintf(Str,"%s  %.2f", MsgMass[48][CurrLang],PONI.PowLevel[0]);//���
      break;
  }
      //putWideString(20,2,screen,Str2,1);
      break;
    case 2: // ��������
      if (PONI.PowLevel[2] >= 1) // ���������
      {
        sprintf(Str,"%s  %.2f",MsgMass[97][CurrLang],PONI.PowLevel[2]);
        //putWideString(20,2,screen,Str2,1);
      }
      if ((PONI.PowLevel[2] >= 0.001)&&(PONI.PowLevel[2] < 1.0)) // ����������
      {
        sprintf(Str,"%s  %.2f",MsgMass[96][CurrLang],PONI.PowLevel[2]*1.0e3);
        //putWideString(20,2,screen,Str2,1);
      }
      
      if ((PONI.PowLevel[2] >= 1e-6)&&(PONI.PowLevel[2] < 0.001)) // ���������
      {
        sprintf(Str,"%s  %.2f",MsgMass[95][CurrLang],PONI.PowLevel[2]*1.0e6);
        //putWideString(20,2,screen,Str2,1);
      }
      
      if (PONI.PowLevel[2] < 1e-6) // ������ 1 ���
      {
        if (PONI.PowLevel[2] < 1e-9)PONI.PowLevel[2] = 1e-9;
        sprintf(Str,"%s  %.2f",MsgMass[94][CurrLang],PONI.PowLevel[2]*1.0e9);
        //putWideString(20,2,screen,Str2,1);
      }
      break;
    }
}

void ReLoadCommOLT (void) // ������������� ����������� ��� ����������
{
      memcpy ( CommentsOLT,PONI.CommUserPM,16); // ����� 
      CommentsOLT[ARRAY_SIZE(CommentsOLT)-1]=0; // ��������� ������� � ������� ����� 0
      IndexCommOLT = 0;
      
      for (int Ind =ARRAY_SIZE(CommentsOLT)-2; Ind>=0; Ind--)
      {
        if (CommentsOLT[Ind]<0x20) CommentsOLT[Ind]=' ';
        else if (CommentsOLT[Ind]!=' ' && IndexCommOLT == 0) IndexCommOLT = Ind;
        //Index_Comm --;
      }
      if (CommentsOLT[IndexCommOLT]!=' ')IndexCommOLT ++;
      KbPosX = 11;
      KbPosY = 2;
}

WORD CheckLevelBattery (void) // �������� ������ ��������
{
  static volatile WORD BufProcBat[10]={100,100,100,100,100,100,100,100,100,100};
  WORD ProcBatMed, ProcBatSum;
//123  ADCData = ADC0Read(BAT_ADC);//����� ������� ��������� ������ ��� ���������
  ADCData = 2048;
  Ubat = ADCData * GetSetBatStep(0)* 3 ; // ��� ��� �������� �� 3, ����� �� ���� -0.3- 0.3
    if (Ubat > 5.1) ProcBat = 100;
  //else if (Ubat >= 4.0) ProcBat = (WORD)((Ubat-4.0)*90.91);//  1.1� = 100%
  //else if (Ubat >= 4.3) ProcBat = (WORD)((Ubat-4.3)*125);//  0.8� = 100%
  else if (Ubat >= 4.15) ProcBat = (WORD)((Ubat-4.15)*105.26);//  0.95� = 100%
  if (Ubat<4.15)
  {
    ProcBat = 0; // ������ �������
  }
  if (!(EXT_POW)) ProcBat = 100; // ���� ������ ������� ��������� ��������

//123  SetContrast (Ubat, GetUserContr()); // �������� �� �����!
  ProcBatSum=0;
  for (int i=9;i>0;--i)
  {
    BufProcBat[i]=BufProcBat[i-1];
    ProcBatSum += BufProcBat[i];
  }
  
  BufProcBat[0]=ProcBat;
  ProcBatSum += BufProcBat[0];
  ProcBatMed = (WORD)(ProcBatSum/10);
  // ��� ���������
  if (ProcBatMed > 20) ProcBatInd = (WORD)(1.25*ProcBatMed - 25);
  else ProcBatInd = 0;
  //ProcBatInd = ProcBatMed;
  return ProcBatMed;
}

int GetBateryADC (void)
{
  return  ADCData;
}

//123
//void SetContrast (float Data, WORD UserContr) // ��������� ���� ���������� � ������ ���������������� ���������
//{
//  static unsigned int NewContrast = 0;
//  if (Data < 5.0) NewContrast = PeriodPWM/2;
//  else if (Data < 5.8) NewContrast = (unsigned int)(PeriodPWM/2 + (Data - 5.0)*6000);
//  else  NewContrast = 15000; 
//    ChangeLenPWM (NewContrast + ((50-UserContr)*USERSTEPONTR));
//}

void BadBattery(void) // ������ ���������� CHECK_OFF
{ 
  char F_name[32];
  WORD CntOff = 0;
  WORD LevelBat;
  REDEYE(0); // ��������� ������� �����
  HV_LOW(ON); //ON LOW HIGH VOLT
  HV_SW(OFF); // OFF HIGH VOLT
  POWREF (OFF);
  POWDET(OFF);
  SubModeMeasOTDR = NOTMODE;
  SetMode(ModeMainMenu);
  ModeDevice = MODEMENU;
    CmdInitPage(27);// MainMenu

  // ������� ���� ��� ���������� ���� LCD �� ��������
  // �������� ��������� Nextion
    sprintf (F_name,"t0.txt=\"%s\"���",MsgMass[71][CurrLang]); // 
    NEX_Transmit((void*)F_name);    sprintf (F_name,"t1.txt=\"%s\"���",MsgMass[72][CurrLang]); // 
    NEX_Transmit((void*)F_name);//
    sprintf (F_name,"t2.txt=\"%s\"���",MsgMass[73][CurrLang]); // 
    NEX_Transmit((void*)F_name);//
  
  LevelBat = CheckLevelBattery();
  while((LevelBat < 60)&&(EXT_POW)) // ���� ���������� ������� ������� �� ������������  � ���� �����
  {
    if (CntOff++%60 == 0) myBeep(3);
     // ������ ��������� �������
    LevelBat = CheckLevelBattery();
    // ProcBatInd
    //sprintf(F_name,"BAT = %d%%", LevelBat );
    sprintf (F_name,"t3.txt=\"BAT = %d%%(%1.2f)\"���",ProcBatInd,Ubat); // 
    NEX_Transmit((void*)F_name);//
    
  }
  CmdInitPage(1);// MainMenu
  myBeep(25);

}

// ������������ � ����� ���������������� ���������� (���� �� �����  � ������
void UploadFW_Nextion(void) // ���������� ���������� NEXTION
{ 
  char Str[32];
  REDEYE(0); // ��������� ������� �����
  HV_LOW(ON); //ON LOW HIGH VOLT
  HV_SW(OFF); // OFF HIGH VOLT
  POWREF (OFF);
  POWDET(OFF);
  //123 enable_timer ( 0 );
    // ����� �������� ��� ������ ���������
    if (g_FirstScr)
  {
    // ����� ��������� ������� ���� ������ ����������
    // �� �������� ��������� ��� ��������� �������������
    sprintf(Str, "t0.txt=\"%s\"���", MsgMass[119][CurrLang]);
    NEX_Transmit((void*)Str);    // 
  
    sprintf(Str, "t1.txt=\"%s\"���", MsgMass[120][CurrLang]);
    NEX_Transmit((void*)Str);    // 
  
    //CreatDelay(500000);// ���� �������
  HAL_Delay(50);
    ProgFW_LCD = 1; // ���������� ����� ������ UART ������ ����� ����� ��� �������

    g_FirstScr = 0;
    g_NeedScr = 0;
  }

  if(ProgFW_LCD==2) // ���� ��������� ������� ���������������� �� ������������  � ���� �����
  {
  //123 enable_timer( 0 );
  GetSysTick(1); // ��������� ����� 10 ��. 0 - ��������� �������� �� ����������� ������ 1- �����
  ProgFW_LCD=0;  
  SubModeMeasOTDR = NOTMODE;
  SetMode(ModeMainMenu);
  ModeDevice = MODEMENU;
      // ������� ������� ������������ ���� �� MainMenu (�������)  
  // ������� ����� ����!
  //CreatDelay(5000000);
  SetModeDevice (MODEMENU); // �������������� ��������� ������ �������
  CmdInitPage(1);
  HAL_Delay(500);// ��������� ����� ������, ����� �� �������!
  myBeep(125);
  }

}

void KeybCntrl (void) // ������������� ��������� � ����������
{
     if ((PRESS(BTN_UP))&&((getStateButtons(BTN_UP)==SHORT_PRESSED)||(getStateButtons(BTN_UP)==INF_PRESSED)))
      {
        myBeep(7);
        if (KbPosY > 0) KbPosY--;
        else KbPosY =3;
      }
    if ((PRESS(BTN_DOWN))&&((getStateButtons(BTN_DOWN)==SHORT_PRESSED)||(getStateButtons(BTN_DOWN)==INF_PRESSED)))
      {
        myBeep(7);
        if (KbPosY < 3) KbPosY++;
        else KbPosY =0;
      }
    if ((PRESS(BTN_LEFT))&&((getStateButtons(BTN_LEFT)==SHORT_PRESSED)||(getStateButtons(BTN_LEFT)==INF_PRESSED)))
      {
        myBeep(7);
        if (KbPosX > 0) KbPosX--;
        else KbPosX =11;
      }
    if ((PRESS(BTN_RIGHT))&&((getStateButtons(BTN_RIGHT)==SHORT_PRESSED)||(getStateButtons(BTN_RIGHT)==INF_PRESSED)))
      {
        myBeep(7);
        if (KbPosX < 11) KbPosX++;
        else KbPosX =0;
      }
}

BYTE CheckPONI (WORD TmpCellMem) // �������� ������� ������ � �� ��������� ���� ��� �� ���
{
  char Str[16] = {"Error S a v e !\0"};
  BYTE Err = 0;
  if (PONI.TotalTimeCell>0x80000000)Err = 1;
  for (int i=0; i<3; i++)
  {
    if (PONI.LenWaveKlb[i]>1700)Err = 1;
    if (isnan(PONI.PowLevel[i]))Err = 1;
    if (isnan(PONI.BaseLvl[i]))Err = 1;
  }
  if (PONI.CellMod>2) Err=1;
  if (PONI.LenWaveMeas>1700) Err=1;
  if (PONI.NumFix>9999) Err=1;
  if (PONI.Rez>1) Err=1;
  if (PONI.CommUserPM[15]!=0) Err =1;
  if (Err)
  {
    memcpy(&PONI.CommUserPM, Str,16);
  for (int i=0; i<3; i++)
  {
    PONI.LenWaveKlb[i] = 0;
    PONI.PowLevel[i] = -100.0;
    PONI.BaseLvl[i] = -100.0;
  }
  PONI.CellMod=0;
  PONI.LenWaveMeas=1310;
  PONI.NumFix=0;
  PONI.Rez=0;
  PONI.TotalTimeCell = 10*YearSecV-1;
//123!!!  WriteCellIzm(TmpCellMem, (unsigned char*)&PONI);
  
  }
  return Err;
}

// off 02/11/2023
//void TestScreen (void)
//{
//    ClearScreen();
//for (int z = 1; z>=0; z--)
//{
//  for (int x = 0; x<128; ++x)
//  {
//    for (int y = 0; y<64; ++y)
//    {
//      PutPixel(x, y, z);
//    }
//    PaintLCD();
//  }
//}
//}

// ���������� �������������� (�� ������ �� ������ � �� ������� �� UART)
int SaveNewOTDRTrace (BYTE Mode)
{
  char BufStrOut[25];
  int Ret=0;
  // �������� ����� �����, � ��������� ����� � 0 � ���������
  
  SetMode(ModeMemoryOTDR);
  ModeDevice = MODEMEMR;
  TimeSaveOTDR = RTCGetTime(); // ��������� ����� �����
  myBeep(10);
  
  Ret = 1;
  //  SaveTrace(); // ��������� ������� ������(�� ����������
    SaveFileSD(1); // ��������� ������� ������( on SD Card
  //sprintf(txtout,"%s %d\r",CommentsOTDR,Ret);//c
  //      UARTSend0 ((BYTE*)txtout, strlen (txtout));

  if (Ret) ModeMemDraw = SAVEDTRACE;
  else ModeMemDraw = MEMFULL;
  if (Mode) // ���������� �� UART
  {
    if (Ret) sprintf(BufStrOut,"Trace saved %d\r",Ret);//c
    else sprintf(BufStrOut,"Mem full\r");//c
    UARTSendExt ((BYTE*)BufStrOut, strlen (BufStrOut));
    
  }
      // ������� ������� ������������ ���� �� Mem_OTDR_graph (�������)  
  // �������� �� ��� ���� �� ��������
  // 07.11.2022 (148)
     // CreatDelay(100000);
  HAL_Delay(10);
      CmdInitPage(13);
//  myBeep(10);
//      CreatDelay(200000);
//      CmdInitPage(13);
//  myBeep(10);

 return Ret; 
}
// ��������� ������� �����
void SaveNoise (DWORD Noise)
{
  MeagNoise = Noise;
}
// ���������� �������� �� �������������� ������
void CalkPreEvents (DWORD* array, unsigned short PII)
{
  float Correlations;
  char OutText[20];

    for (int i = 0 ; i<RAWSIZE-PII-20; i++)
  {
          Correlations = CalkCorelation32(array, i, PII, 0); // ������ ����������� ������������� ���������
      // ������ ������ ��������������� ���������� �� ����������� �������  
      sprintf (OutText, "%d %d %0.3f %0.6f\n",i,array[i],log10(array[i]),Correlations);
      UARTSendExt ((BYTE*)OutText, strlen (OutText));
      
  }
}
// ��������� ���������������� �������� ����������� ��������������
// ��������� ������������� ��������
BYTE GetUserVerSize (void)
{
  return IndexVerSize;
}
// ��������� ����������������� ������ ��������� �������
BYTE GetUserTypeCentral (void)
{
  return TypeCentral;
}

void PrintEventTbl (void) // ����� �� RS ������� �������
{
    char OutText[20];
   unsigned short  NumEventOut =  (CalkEventsKeys (LogData, PointsInImpulse(0), 1)); 
   
   sprintf(OutText,"%.1f %s; ",fabs((GetPosLine(EndEvenBlk.ELMP[1]))*1000.0) ,MsgMass[78][CurrLang]);//� - �����
      UARTSendExt ((BYTE*)OutText, strlen (OutText));
    sprintf(OutText,"%.2f %s; ",(LogData[EndEvenBlk.ELMP[1]]-LogData[EndEvenBlk.ELMP[0]])/1000.0,MsgMass[47][CurrLang]);//��
      UARTSendExt ((BYTE*)OutText, strlen (OutText));
      //
      for (int i=1; i<=NumEventOut;++i)
      {
        if(EvenTrace[i-1].EC[1] == 'F')// Found by software
        {
      //sprintf(OutText,"%s ",EvenTrace[i-1].COMM_EVN);//�����������
      //UARTSend0 ((BYTE*)OutText, strlen (OutText));
      sprintf(OutText,"Evnt %d %.1f ; ",i,fabs((GetPosLine(EvenTrace[i-1].EPT))*1000.0) );//� - �����
      UARTSendExt ((BYTE*)OutText, strlen (OutText));
      sprintf(OutText,"%.2f ; ",(EvenTrace[i-1].EL)/1000.0);//��
      UARTSendExt ((BYTE*)OutText, strlen (OutText));
      sprintf(OutText,"%.2f ; ",EvenTrace[i-1].ER/1000.0);//��
      UARTSendExt ((BYTE*)OutText, strlen (OutText));
        }
      }
      sprintf(OutText,"\n");//
      UARTSendExt ((BYTE*)OutText, strlen (OutText));
      SetGetMonEna(0);
}
// ��������� ������ �������������� ��� ����������
// ���� ����� �������� �������� ����� ���������� (�������� ������� � 0) �� 
// ���� ��������� N ����� ���� ������������ ���� �� ����� �� 0.05 �� � 
// ������� �������� �� 15 ��, �� ������ 0.5 ��
// ��������� ������ �������� ��� ���������� �� "�������" ��������� 3 � 10 ���
// ��������� ����� �� ����������� ���� � ����� �����
unsigned short SpliceProg (unsigned short PII)
{
  unsigned short EnaChange=0; // ������� ���������� ������ ������ ( ������������� ����� ���������� ���� 40 ��)
  //int Difrn=0;
  //float MedLog=0;
  //unsigned long Summer=0;
  unsigned short AddSh=0; // ������� � ��������
  unsigned short Index=0;
  static unsigned short I_min=0, I_max=0;;
  static unsigned short min = 0xFFFF, max = 0;
  unsigned short deltaU = 150;
  unsigned long medium =0;
  if(GetLengthWaveLS (GetPlaceLS(CURRENT)) == 850) deltaU = 500;
  for (int i = (int)(PII+(PII>>1)) ; ((i<OUTSIZE)&&(LogData[i]<20000)&&(!Index)); ++i) // 26/02/2014 ������ �������� ����� �� 1.5 ��������
  {
    if (LogData[i]>750)
    {
      min = 0xFFFF;
      max = 0;
      medium =0;
      I_min = i;
      I_max = i;
      for( int j = i; j <i+5; ++j )
      {
        medium = medium + LogData[j];
        if( min > LogData[j] )
        {
          I_min = j;
          min = LogData[j];
        }
        if( max < LogData[j] )
        {
          I_max = j;
          max = LogData[j];
        }
      }
      // ��� 850 �� �� 5 ������ ���128�� �������� ����� 0.366 ��
      
      if (((max-min)<deltaU)&&(I_max>I_min))
      {
        Index = i+5;
        //medium = medium/5;
        medium = (unsigned long)FlashReadSpliceData(Index)- (unsigned long)LogData[Index];
        for (int o=1; o<Index; ++o) // ���������� �� ��������� ��������
        {
          LogData[o] = FlashReadSpliceData(o);
        }
        for (int o=Index; o<OUTSIZE; ++o) // ��������� "������"
        {
          // ����� ������ ����������� ������ ( ����� ������������� � ��������)
          if ((GetIndexLN()==6)&&(GetIndexIM()>=6)&&(GetIndexVRM()==3)&&GetApdiSet()) // ���� ����������� ����� ���������� 180�, ����� ����� 128 ��, � ������������ �������� 10 ���
          {
//          case 1:
//            if (LogData[o]>35000)
//            {
//              EnaChange++;
//              Summer +=(unsigned long)LogData[o];
//            }
//            if (EnaChange>2) // ����� ������ ������
//            {
//              //if (++AddSh > 700) AddSh = 700;
//              AddSh = (unsigned short)((double)Summer/(double)EnaChange); // ��������������� �������
//              if (LogData[o]<35000)
//              {
//                LogData[o] = (unsigned short)(((float)LogData[o] + (float)AddSh)/2);
//              }
//            }
//            break;
//          case 2:
//            if (LogData[o]>40000)
//            {
//              EnaChange++;
//            }
//            if (EnaChange>4)
//            {
//              if (LogData[o]<37000)
//              {
//                Difrn = LogData[o-1] - LogData[o];
//                if (Difrn>0)
//                  LogData[o] = LogData[o] + (unsigned short)(Difrn>>1);
//              }
//            }
//            break;
//          case 3:
//            if (LogData[o]>30000)
//            {
//              EnaChange++;
//              Summer +=(unsigned long)LogData[o];
//            }
//            if (EnaChange>4) // ����� ������ ������
//            {
//              //if (++AddSh > 700) AddSh = 700;
//              AddSh = (unsigned short)((double)Summer/(double)EnaChange); // ��������������� �������
//              if (LogData[o]<37000)
//              {
//                LogData[o] = AddSh;
//              }
//            }
//            break;
            if (LogData[o]>27000)
            {
              EnaChange++;
            }
            if (EnaChange>2) // ����� ������ ������
            {
              if (LogData[o]<24000) AddSh = 700;
              if (++AddSh > 700) AddSh = 700;
              if (LogData[o]<32000)
              {
                LogData[o] = LogData[o] + (unsigned short)(AddSh*3);
              }
            }
          }
          LogData[o] = LogData[o] + (unsigned short)medium;
          
        }
      }
    }
    
  }
  
  //if (Index) LogData[Index]=0;
  return Index;
}

// ������� ��������� ������ DAC �� ������ P0.26
//123
//void SetLvlDAC (unsigned int DAC)
//{
//  DACR = (unsigned long)((DAC&0x7)<<9);
//}

// ��������� ��������� ����� �������� �������������� ������
void SetHeadFileRaw (DWORD NAV)
{
  DWORD NumBt = RAWSIZE*sizeof(DWORD) + 24; // ����� ���� � ������� ������� 24 -32 ���������� ������ 24
  WORD Index;
  WORD siZe = sprintf ((void*)Head_RAW.Head,"%d",NumBt);
  // ���������
        for (Index=0 ; Index<siZe; Index++)
        {
          Head_RAW.Head[7-Index] =  Head_RAW.Head[siZe-Index-1];
        }
        Head_RAW.Head[7 - Index++] = 0x30 +  siZe;
        Head_RAW.Head[7 - Index++] = '#';
        while (Index < 8) Head_RAW.Head[7 - Index++]=0x20;
// ���������� DS
       //Head_RAW.ValDS = (unsigned int)((ADCPeriod*50000)/(NumPointsPeriod[GetIndexLN()])); //  ������������� �������� DS ��� �������������� ������ ���������
       //Head_RAW.ValDS = (unsigned int)((ADCPeriod*50000)/(NumRepit)); //  ������������� �������� DS ��� �������������� ������ ���������
       Head_RAW.ValDS = (unsigned int)GetValueDS(); //  ������������� �������� DS ��� �������������� ������ ���������
// ����� ���������� NAV
       Head_RAW.NumAvrg = NAV;
// ����� ����� ���������, ������������ �������� ������������ AW PWU
       Index = GetLengthWaveLS (GetPlaceLS(CURRENT)); // �������� ����� ����� ���������
          Head_RAW.AW_PWU = Index<<16; // ���������� ����� ����� ���������
          Index  = GetWidthPulse(GetIndexIM());
          Head_RAW.AW_PWU += Index; // ���������� ������������ ��������
// ������ ������, ��� ������������ (����� �� ����� �� ���� = 0) �������� (������ �����) �� ��� ��������� � ����������
          Head_RAW.FormatData = GetCurrentBegShiftZone();
          // ������ ���� ����������, ������ ����� ����� �� ������ 
            Head_RAW.SizeFrm = (NumRepit);
          //����� �������� NPPW (�� ��������� �������, �� � ��� ����)
              Head_RAW.NumPtsMain = RAWSIZE;//0x1200;
}

// 26.02.2014 
// ������� � ����� ��������� ���������� ������������� �� ���� ��� ��� ��������� ���� ��������
void SetMODESettingRef (void)
{
     SetIndexLN(GetIndexLN());//������������� ������� ��������� �� ������ � ���������
     SetMode(ModeSetupOTDR);
     SetIndexWAV(GetWAV_SC(GetPlaceLS(CURRENT))); // ������������� �����. ����������� ��������� ����� �����

     GetSetModeLW(-1); // ���������� ������� ���� ���� ����������
     CntLS = 1;
     PosCursorMain (-4100); // ����� ������� � ������
     IndexVerSize  = 0;// ��������� ������������� ������� ����������� �������������� ( ����� �������)
     GetSetHorizontScale (5); // ����� �������� ��� ����� �������
     // 25.11.2022 ������������ ��� ����� � ���� ��������� �������������, 
     //�� ��������������� ������� �����, �� ���� �� ����������� �������
        if ((GetIndexLN()!=0)&&(IndexSmall==0))
        {
          IndexSmall=1;
        }
     if (IndexSmall==0)GetSetHorizontScale (-2);
     // ������������� ��� 2 �� 
     ModeDevice = MODESETREFL;
     //123 SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
     
//123     PWM_LMOD_Init (OFF); // ��������� LMOD �� PWM 0 - OFF, 1- 270Hz 2-2kHz  

}
// ������ ORL �� ������� �������������� ������ �� ��������� � ������ 16�� 40 �� (03/05/2017)
float MeasORL(int NumAvrgThis, int EnaReport)
{
#define NumPnoise 40 // ����� ����� ��� �������� ����
//#define NumAvrgThis 5000 // ����� ����� ����������
  //char BufStr[48];
  double SumNoise = 0.; // "����" - ��������
  double SumDatRefl = 0.0; // ����� ������ �������������� 4096 �����
  float Result;
  double CurrDat, MaxNoise=0.;
  double SumBeg = 0, Mnozhitel = 1.0; // ������������� ���������
  int NumPereg = 0, pSu = 0; // ������� ����������, ����� ����� ������������, AllPereg = 0
  LED_START(1);//On  LED
  //myBeep(10);
  SlowON();
  HV_LOW(OFF); //ON HIGH VOLT
  HV_SW(ON); // ON HIGH VOLT
  //POWALT(ON);
  //POWREF (ON);
  //POWDET(ON);
  //123
  //������ ��� � ���� �������������
        if (EnaReport)//���� �������� �� ��� ����������
        {
//  SSPInit_Any(SPI_ALT); // ������������� SSP ��� ���������� ALTERA (���� 1 �� ��� �� ����� ����������)
//  disable_timer ( 0 );
//  reset_timer(2);
//  enable_timer(2);
          CurrTimeAccum = 0;
          EnaTimerAccum = 1;
        }
  SetIndexIM (1); // 40 ns
  SetIndexLN (3); // 16 km
  PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
  PointInPeriod = 0;
  memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
//123  SetNumAccumPerSec (NumAvrgThis);// ��������� �������� ����� ����������
  //CreatDelay (9000000*EnaReport); // 33 ��
  HAL_Delay(900*EnaReport);
  //        ModeDevice = MODEOTHER;
  //
  Averaging(100,0,0);// ��������� ������� � ������ ���������� 
   SetNumAccumPerSec (NumAvrgThis);// ��������� �������� ����� ����������
  LED_START(0);//Off  LED
  //        ModeDevice = MODEOTHER;
  //
  //disable_timer ( 0 );
  myBeep(0); // ����. ����
  //CreatDelay (9000000*EnaReport); // 33 ��
  HAL_Delay(900*EnaReport);
  CntNumAvrg = 0; // �������� ������� ����������
  memset( RawData, 0, RAWSIZE * sizeof(DWORD) );
//123  reset_timer(2);
//123  enable_timer(2);
  CurrTimeAccum = 0;
  EnaTimerAccum = 1;
  LED_START(0);//On  LED
  Averaging(NumAvrgThis,0,0);// ��������� ������� � ������ ���������� c ����������� ����������
  //g_UpGrdB, g_kLog, g_OffSetdB)
  //  SetMode(ModeSetOTDRprm);
  //  ModeDevice = MODEOTDRPRM;
  // ������ ����� ��������� ORL
  for(int i=0; i<NumPnoise; ++i) 
  {
    SumNoise +=(double)RawData[i];
    if(MaxNoise<(double)RawData[i]) MaxNoise = (double)RawData[i];
  }
  SumNoise = SumNoise/(NumAvrgThis*NumPnoise); // � ������ ���� ���� �������� �������������
  MaxNoise = MaxNoise/NumAvrgThis;  // �������������� �������� ������������� ����  � ����
  // ������ ���������
  // �������� ������� ������� ����� ��������
  SumBeg = (RawData[43]+RawData[44]+RawData[45])/(3.0*NumAvrgThis)-SumNoise;
  if(SumBeg>1.8 && SumBeg<400.)
    Mnozhitel = 10.24/SumBeg;
  else
    Mnozhitel = 1.0;
  // �������� �������� � ����������� �� ��� ������������
  for(int i=0; i<POINTSIZE; ++i) //4096
  {
    CurrDat = (double)RawData[i+41]/NumAvrgThis; // ��������� ���������
    if(CurrDat > 1020) // ����������! 4090
    {
      NumPereg++; // ��������� ������������ ����������
      //AllPereg++;
      CurrDat = CurrDat*pow(g_UpGrdB,NumPereg)/Mnozhitel; // �������� ������� (���� ���)
    }
    else // ��� ����������, ������� ������� ����������
    {
      if(NumPereg )
      {
#if 0
//        {
          sprintf(BufStr,"--p-%d\n", NumPereg);// ORL
  UARTSendExt ((BYTE*)BufStr, strlen (BufStr));
//        }
#endif
    NumPereg=0;
      }
    }
    // ������� ���������� , ���� ������ ������ ������������� ���� ���� �� �������
    if(CurrDat>(MaxNoise))
    {
    SumDatRefl +=CurrDat-SumNoise;
    pSu++;
    }
  }
  if(SumDatRefl<=10) SumDatRefl = 10; // �� ����� 65 ��
  // ����������� ����� �� ���������, "�����������" �������� �����
  SumDatRefl = SumDatRefl*Mnozhitel ;
  // ����������� �� ������ 14.8
  if(SumDatRefl>1047000.)SumDatRefl=1047000.;
  Result = g_OffSetdB - g_kLog*log10(SumDatRefl);
        if (EnaReport)//���� �������� �� ��� ����������
        {
          SetIndexLN (0); 
          SetIndexIM (0);
          HV_LOW(ON); //ON LOW HIGH VOLT
          HV_SW(OFF); // OFF HIGH VOLT
          //POWDET(OFF);
          POWREF (OFF);
          POWDET(ON); // �������� ������� ����������
//123          enable_timer(3);  /* Enable Interrupt Timer3 */
//123          enable_timer(1);  /* Enable  Timer1 JDSU */
//123          SSPInit_Any(SPI_PM);      //�������� SPI �� ���
//123          SetupSource (GetModeLS()); // �������������� ����� ������ ������  
          // �� ������ ������ 175, ����� ���� ������� ��������� ��������
          //CreatDelay (350000); //
  HAL_Delay(35);
          
        }
  // ���������� ����
#if 0
  //{
  sprintf(BufStr,"O=%.2f\n", Result);// ORL
  UARTSend0 ((BYTE*)BufStr, strlen (BufStr));
  sprintf(BufStr,"N=%.3f\n", SumNoise);// Noise Shift
  UARTSend0 ((BYTE*)BufStr, strlen (BufStr));
  sprintf(BufStr,"M=%.3f\n", MaxNoise);// MaxNoise
  UARTSend0 ((BYTE*)BufStr, strlen (BufStr));
  sprintf(BufStr,"S=%.3f\n", SumDatRefl);// Summa 
  UARTSend0 ((BYTE*)BufStr, strlen (BufStr));
  sprintf(BufStr,"P=%d\n", pSu);// ����� ����� ������������
  UARTSend0 ((BYTE*)BufStr, strlen (BufStr));
  sprintf(BufStr,"Mn=%.3f\n", Mnozhitel);// ���������
  UARTSend0 ((BYTE*)BufStr, strlen (BufStr));
  sprintf(BufStr,"Sb=%.3f\r", SumBeg);// ����� � ������
  UARTSend0 ((BYTE*)BufStr, strlen (BufStr));
  SubModeMeasOTDR = NOTMODE;
  //}
#endif
  LED_START(0);//Off  LED
  return Result;
}

    
    
//void TetrisGame (void)
//{
//     ClearScreen();
//    OnTimerTetris();
//
//  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
//  {
//    SetMode(ModeMainMenu);
//    myBeep(10);
//    ModeDevice = MODEMENU;
//  }
//    PaintLCD();
//
//}
// ������� ������������� �������� �� ������ � NEXTION
void CmdInitPage(int Num)
{
  char str[44];
  sprintf(str, "page %d���",Num); // < ������e >
  NEX_Transmit((void*)str);    //
  
  //NEX_Transmit((void*)CmdNextion[Num]);
  g_FirstScr=1;
  //CreatDelay(20000);// 177 ��� � 173
  HAL_Delay(30);
}

void SlowON (void) // ��������� ��������� �������
{
  // ������ � ��������� ��� ����������, ��� ���������...
  
  POWREF (ON);
  HAL_Delay (700); // 0.7 � (� ���� ��������� ����� �������� �� USB, ��� ���)
  POWDET(ON);
  // � ��� �� �������
//  POWALT(ON);
//  CreatDelay (40000); // 3.3 ��
//  POWDET(ON);
//  CreatDelay (40000); // 3.3 ��
//  POWREF (ON);
//  CreatDelay (80000); // 3.3 ��
  
}

#pragma optimize=none

// �������� 83.33 ��
void CreatDelay (unsigned long a) { while (a--!=0); }


//void ArcanoidGame (void)
//{
//     ClearScreen();
//     OnTimerArcanoid();
//  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==SHORT_PRESSED))
//  {
//    SetMode(ModeMainMenu);
//    myBeep(10);
//    ModeDevice = MODEMENU;
//  }
//    PaintLCD();
//
//}

//void KeyTestGame (void)
//{
//     ClearScreen();
//     OnTimerKeyTest();
//  if ((PRESS(BTN_MENU))&&(getStateButtons(BTN_MENU)==LONG_PRESSED))
//  {
//        POWALT(OFF);
//        POWREF (OFF);
//        POWDET(OFF);
//
//    SetMode(ModeMainMenu);
//    myBeep(10);
//    ModeDevice = MODEMENU;
//  }
//    PaintLCD();
//
//}
