#ifndef __MODES_H__
#define __MODES_H__

#include "rtc.h"
// ������ ������ ������� ( ��� ����������� ������� ��� ������ �����)
#define MODEMENU 0 // ����� ����
#define MODESETREFL 2 // ����� ��������� ������������
#define MODEREFL 1 // ����� �������������
#define MODEMEMR 3 // ����� ������ � ������� �������������
#define MODETEST 4 // ����� �������
#define MODESETUP 6 // ����� ��������� ������������
#define MODEMEASURE 7 // ����� ��������� ������������� (����������)
#define MODETESTMEM 8 // ����� ������� view memory
#define MODEERREFL 85 // ����� ������ ��������� ��������������������
#define MODEOTDRPRM 9 // ����� ��������� ���������� ������������������� ���������

#define MODEOTHER 255 // ������ ������


// ������������ ���������� ������������� ��� ����������
#define NOTMODE 0 // ������ ��� ����� ��� ������ �� �������
#define SETPOWER 1 // �������� �������, ����������� ��������� ����������
#define INPUTOTDR 2 // �������� ������� �� �����
#define QUALITYBEGIN 3 // �������� �������� ������� ( �������� ������� - ������ �����)
#define SEARCHENDLINE 4 // ����� ����� �����
#define LASTCHECK 5 // �������� ����� �������� ����������
#define AVERAGING 6 // ������ ���������� ������ ��������
//#define ONESUMMONLY 7 // ������ 1 ���������� 
#define CALKORLBEG 8 //  ������ ORL
//
#define POWMANUAL 1 // ������ ����� ����������
#define POWAUTO 2 // �������������� ����� ����������

#define PNTVERTICALSIZE 4 // ����� ������������ ���������

#define PRESS( a ) (KeyP & a)

WORD SetNumCellIzm (WORD Data);
float GetLastPower (void);

BYTE SetGetMonEna (BYTE Data); // ������/��������� �������� ������ ������� �� ���������� ���������
//
BYTE GetEnaMZ (void); // ��������� �������� �������� ���������� ������������� ������� ���
BYTE SetEnaMZ (BYTE Mode); //  ���������  �������� ���������� ������������� ������� ���

//
BYTE SetModeDevice (BYTE Mode); // �������������� ��������� ������ �������
BYTE GetCurrentModeDevice (void); // ��������� �������� ������ �������
BYTE GetModeDevice (void);// ��������� ������ �������
BYTE GetModePowerMeter (void); // ��������� �������� ������ ����������
      // ������������� ������ � ����������� �� ����������
void SetPlaceNew(int Mode);


void ModeFuncTmp(void);
void SetMode( void(f)(void) );

void SetBELCORE (void);// ��������� ������� ������� � BELCORE

void ModeSetOTDRprm(void);// ����� ��������� ���������� ������������� �������
void ModeWelcome(void); // ����� ��������
void ModeMainMenu(void); // ����� ��������� ����
void ModeSetupOTDR(void); // ����� ��������� �������������
void ModeStartOTDR(void); // ����� ���������� �������������
void ModeErrorOTDR(void); // ����� ����������� "��������� �� �����"
void ModeDrawOTDR(void); // ����� ����������� ��������������
void ModeEventsOTDR(void); // ����� ����������� ������� ��������������
void ModeKeyBoardOTDR(void); // ����� ����������� ���������� ��������� ������������ ��������������
void ModeMemoryOTDR(void); // ����� ����������� ����������� ������������� � ������ � ����
void ModeSelectOLT(void); // ����� ������ ���� �������
void ModeMeasManualOLT(void); // ����� ������ ������� � ������ ������
void ModeMeasAutoOLT(void); // ����� ������ ������� � �������������� ������
void ModeSourceOnly(void); // ����� ������ ������� ������ ��������
void ModeSaveOLT(void); // ����� ���������� ����������� ����������
void ModeKeyBoardOLT(void); // ����� ����������� ���������� ��������� ������������ ����������
void ModeKeyBoardPrefix(void); // ����� ����������� ���������� ��������� Prefix File Name
void ModeSelectMEM(void); // ����� ������ ������ � �������
void ModeClearMEM(void); // ����� ������������ ������ ����������
void ModeViewMemOLT(void); // ����� ��������� ������ ����������
void ModeSetting(void); // ����� ��������� �������
void Mode(void); // ����� ��������� �������
void ModeDateTimeSET(void); // ����� ��������� �������
void ModeCalibrate(void); // ����� ��������� ���������� ��������
void ModeFileMngDir(void); // ����� ���� ��������� ����������
void ModeFileMngFiles(void); // ����� ���� ��������� ������

//void FrBig(unsigned char SW,  char Add); //������� �����
//void FrSmall(unsigned char SW,  char Add);// ��������� �����
// ����� ������������� ������� � �����
//void FrFree(unsigned char SW ,  int X, int Size);

BYTE ChangeFrSet (BYTE FrSet, BYTE MaxSet, BYTE MinSet, BYTE DirSet);// ��������� ������� � ������ �������� ����������
// ������ �������� � ������� ����
BYTE DrawSourceVFL (BYTE* frameInx,  char* Str, DWORD KeyP, BYTE Lang, BYTE FrmMax, BYTE FrmMin);
BYTE Measuring ( void);
int SearchShiftBeg (int Size);// ����� ������� ���� 
void DrawCalibrating (DWORD Data);// ��������� ���� ����������
BYTE IndexSeek( int Data );// ����� ������� ����� ��� �������� ����� �����

// ��������� ������������ �� ����� ����� �����
DWORD Scan( DWORD* array, unsigned size, DWORD level );

void StartSettingBegShift (void); // ����� ��������� ������� ���

void InitAutoPM (void); // ��������� ��������� ���������� ��������

unsigned GetCntNumAvrg ( void);// ��������� �������� �������� ����������
unsigned GetFinAvrg ( void);// ��������� ��������� �������� ���������� (������������ �������� ����������)
unsigned SetCntNumAvrg ( unsigned Data);// ��������� �������� �������� ����������

float GetPosLine (unsigned Cursor); // ��������� ����� �� ������� �������

// ���������� ���������� � ������������� �������� �� ������
//void Draw_KeyBoard(unsigned char Cursor, char* Str_first, char* Str_edit, BYTE Shift ,BYTE Lang);
//void Knob(unsigned char X,unsigned char Y);
void SavePowerMeter(float Pow_mW);// ������� ���������� � ������ ���������
void ReturnToTester (BYTE EnClr); // ������� � ����������
void WriteMemPow(void); // ������ � ������ ���������������
void DrawLevelStr(char* Str); //???? ����� �������� � ��� ������� W/ dB/ dBm
void SetStringPM(char *Str, float mWatt) ;          // ������������� ������ ������ ���������
void ReLoadCommOLT (void); // ������������� ����������� ��� ����������

void KeybCntrl (void); // ������������� ��������� � ����������

WORD CheckLevelBattery (void); // �������� ������ ��������
//123 void SetContrast (float Data, WORD UserContr); // ��������� ���� ���������� � ������ ���������������� ���������
void BadBattery(void); // ������ ����������
// ������������ � ����� ���������������� ���������� (���� �� �����  � ������
void UploadFW_Nextion(void); //

BYTE CheckPONI (WORD TmpCellMem);  // �������� ������� ������ � �� ��������� ���� ��� �� ���
//void TestScreen (void); // 
// ���������� �������� �� �������������� ������
void CalkPreEvents (DWORD* array, unsigned short PII);
// ��������� ������� �����
void SaveNoise (DWORD Noise);

// ���������� �������������� (�� ������ �� ������ � �� ������� �� UART)
int SaveNewOTDRTrace (BYTE Mode);

// ��������� ������������� ��������
BYTE GetUserVerSize (void);
// ��������� ����������������� ������ ��������� �������
BYTE GetUserTypeCentral (void);
void PrintEventTbl (void); // ����� �� RS ������� �������
// ������ �������
unsigned short SpliceProg (unsigned short PII);
// ������� ��������� ������ DAC �� ������ P0.26
//123 void SetLvlDAC (unsigned int DAC); // �� �����!
// ��������� ��������� ����� �������� �������������� ������
void SetHeadFileRaw (DWORD NAV);
// ������� � ����� ��������� ���������� ������������� �� ���� ��� ��� ��������� ���� ��������
void SetMODESettingRef (void);
// ������ ORL �� ������� �������������� ������ �� ��������� � ������ 16�� 40 ��
float MeasORL(int NumAvrgThis, int EnaReport);
// ������� ������������� �������� �� ������ � NEXTION
void CmdInitPage(int Num);

void SlowON (void); // ��������� ��������� �������

void AlarmSignal(int type);

void CreatDelay (unsigned long a); // ��������� �� LCD.h

//void TetrisGame (void);
//void ArcanoidGame (void);
//void KeyTestGame (void);

//extern RTCTime TimeSaveOTDR; // ����� ����������� ��������������
//extern RTCTime TimeSaveOLT; // ����� ����������� ��������������
extern char CommentsOTDR[20]; //����������� �������������
extern char IndexCommOTDR ;
extern BYTE RSOptYes; // ������� ������ ������� �� ����������� UART
extern BYTE RemoutCtrl; // ������� ���� ����������

//���������� ��������� ��������� ORL 
extern float g_VolORL;
extern float g_UpGrdB ; // ���������� ��
extern float g_kLog; // ����������� ���������
extern float g_OffSetdB; // �������� � ������� dB



#endif  /* __MODES_H__ */

