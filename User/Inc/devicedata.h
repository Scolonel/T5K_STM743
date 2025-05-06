#ifndef __DEVICEDATA_H__
#define __DEVICEDATA_H__

#define WAVE_LENGTHS_NUM 5
#define LENGTH_LINE_NUM 7
#define WIDTH_PULSE_NUM 8
#define TIME_AVR_NUM 6
#define TIME_LIGHT_NUM 3
#define LSPLACENUM 4
#define CURRENT 0
#define NEXT 1
#define MODELSNUM 5
#define MODERENUM 3



// ��������� ������������� ��������� � ����� �������� �� 0.05 ��
typedef struct
{
float BatControl[1024];// �������� ������� ������� 
} BAT_CNTR;




// ��������� ������������� ��������� � ����� �������� �� 0.05 ��
typedef struct
{
uint16_t SetCoefLW[4][2048];// ����� ��������� ��������� �� ������ ����
} TAB_SET;

// ��������� ������������ �������
typedef struct
{
  uint16_t NumDevice;  // (9999) � �������,
  uint16_t PlaceLW[LSPLACENUM]; //  ������������ ������������� ������������ �� ������ ���� 
  uint8_t ID_Device; // ������ 0-����� ��� 1-MOT_700
  uint8_t Ena_DB;  // ���������� ��������������� �����( 0- ���������, 1-���������)
  unsigned char AltName[20]; //  �������������� ���
} CFG_DEV;



typedef struct  // �������� ���������������� ��������� ������� ������ ������
{
  uint8_t CurrLang;  // ������� ���� Rus, Eng, Cze
  char iCurrLW ; // ������ ������� ����� ����� 
  char iFixLvl[4] ; // ������ �������������� ������, ��������� �� ����� (��� ����� ����� �����)
  uint16_t iLvlCurrLW[4]; // ������� �����. ��� ������ ����� �����
  uint16_t iLvlFixLW[4][2]; // ���������� ������������� �����. ��� ������ ����� �����
  
} CFG_USR;


unsigned InvalidDevice(); //boolean actually
void InitDevice(unsigned Err);
DWORD CheckUserGonfig (void);  // �������� ���������������� �������� 
DWORD FindErrCoeff (void); // �������� ������� �������������
void SetLang( BYTE lang );
BYTE GetID_Dev(void); // ��������� �������������� ��� ���� ������
BYTE SetID_Dev(BYTE Data); // ��������� �������������� ��� ���� ������
BYTE GetLang(int Dir); // ��������� �������� �����/ ��� ������������
WORD GetNumDevice(void); // ��������� ������  �������
WORD SetNumDevice(WORD Num); // ��������� ������  �������

// ��������� ������ � ���������, ����� �� �������� ������ ������ �� ���������(0...1023)
extern BAT_CNTR LvlBatSav;
extern uint32_t CountBat; // ������� ������� �� �������

// ��������� ������������� ���������� ��������
extern TAB_SET CoeffLW;
// ������������ �������
extern CFG_DEV ConfigDevice;
// ���������������� ��������� (�������) ��� ������������� ����� ���������
extern CFG_USR UserSet;


#endif  /* __DEVICEDATA_H__ */



