#ifndef __EVENTS_H
#define __EVENTS_H

#define SizeTableEvnt 20

// ��������� ������� � KeyEvents
#pragma pack(2)
typedef struct   // Size 32 Byte
{
  unsigned short EN;      // EN - Event Number
  unsigned int EPT;       // EPT Event Propagation Time 0
  short int ACI ;    // ACI Attenuation Coefficient Lead-in-Fiber 0
  short int EL ;     // EL Events Loss 0
  long ER ;               // ER Events Reflectance -45000 = -45.0 dB 
  char EC[6] ;            // EC Event Code 0/1/2-A/M/E/F/O-9999  
  char LMT[2];            // 2P/LS 2-point LSA 
  char COMM_EVN[10] ;     // ������������ � �������
} EVENTS;  // ��������� ��������������

// ��������� ��������� (������� , LOSS � ORL) ������� � KeyEvents

#pragma pack(2)
typedef struct   // Size 22 Byte
{
  long EEL;     // EEL - End-to_End Loss
  long ELMP[2];          // End-to_End Marker Positions
  unsigned short ORL ;   // optical Return Loss
  long RLMP[2];          // optical Return Loss Marker Positions
} ENDBLKEVENTS;  // ����� ����� �������

// ��������� ������ ������� ����������, �� ��� ������� ������� ��� �������
// ������ ������� � 3 ���� 
#pragma pack(2)
typedef struct   // Size 12 Byte
{
  unsigned short EN;      // EN - Event Number (2)
  unsigned int EPT;       // ��������� ������� � ������ - ����� ��� ����������� ��� ������� (4)
  unsigned short Volume ;    // �������� ������� �� LOGDATA (2)
  unsigned short PointInImp ;     // ����� ����� � �������� PII (2)
  char Type_Evnt ;               // ��� ������� (1)
  char CompBell ;            // ������� ������������� � �������� ������ ��� ������� ����� ��������� � ������ (1)
} MY_EVNT;  // ���������� ������ �������

// ��������� ������� �������������� ( �������� ���������)
extern EVENTS EvenTrace[SizeTableEvnt];
// ��������� ��������� ����� ������� �������������� (�������� ���������)
extern ENDBLKEVENTS EndEvenBlk;
// ��������� ������� �������������� ( �������� ���������) ��������� ���������
extern MY_EVNT MyEvenTrace[3*SizeTableEvnt];



DWORD CheckReflParam (void);
unsigned short GetNumEvents (void); // ���������� ����� ������� (
unsigned short GetSetEnaEvents (unsigned Dir); // ������������� ���������� ������� ���������� �������
void InitEventsTable (void); // ������������� �������� �������
unsigned short CalkEventsKeys (unsigned short* array, unsigned short PII, BYTE Type); // ���������� ������� ��� ������� ��������������
// ������ ������� �� �������� ������� (�� ��������� ������������ �������� �� ��������� ��� � ����������,
// ������� ������� �� ����� � ���� ��������� ����� �� ������� �������� �������)
unsigned short CalkDownJump (unsigned short* array, unsigned short BeginPoint, unsigned short Size);
// ������ �������� ���������� ��������� ������� � ������ ����� � �������
float CalkSCO (unsigned short* array, unsigned short BeginPoint, unsigned short Size);// , float* Base
// ������ ������ �� ������ ����� ��������� ������� � ������ ����� � �������
float CalkTrend (unsigned short* array, unsigned short BeginPoint, unsigned short Size, unsigned short TrLvlDwn, float b0); //, float* Base
// ������ ���������  �� ������ ����� ��������� ������� � ������ ����� � �������
float CalkCorelation32 (DWORD* array, unsigned short BeginPoint, unsigned short Size, BYTE Type);// , float* Base
// ������ �������� EPT ��� ������� �� ��������� �������
long CalkEPT (unsigned long Position);

unsigned short GetLvlTrend (void); // ������ ���������� ������ � ������� �������

// ����� ����� ����� , � ����� ��������������
unsigned short CalkEndRefl (unsigned short* array, unsigned short PII,  unsigned short TrLvlDwn );

// ���������� ��������� � ����������� �� �������� ������� � ������������ ��������
// � ������� �1000 - �� ���� �������� ��������
// �������� ������ BC - 80 ��(800),�������� �������� � ��*1000, ������������ ��������
long CalkEventER (long LevelER);
unsigned short CalkMINMAX (unsigned short* array, int size, BYTE type);

#endif
