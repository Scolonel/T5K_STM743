#ifndef __PM_H__
#define __PM_H__

// ��������� ������ ��� - AD7782
#define FREEADC 0 // ��� ����� ���������
#define READYDATA 1 // ������ ������
#define BUSYADC 2 // ��� ������


// ��� "������������" �������������"
int GetPMData(void);              // ������ ������ �� ��� ����������
float GetCoeffSpctr(unsigned int Lambda);   // ��������� ������������� ������������ � ����������� �� ����� �����
                                            // ��������� 800-900, 1210-1390, 1410-1650 
float GetCoefPointKlb(unsigned int Lambda);              // ���������� �������������� ����������� ��� ������������� ���� ����


// "�������" �������
void SetSwitchPMMode(BYTE SwMode);          // ������������� ����� ������������ 
                                            // ������������� �������� ���������� �������� (������ ��� ��������������)
void SetRange(unsigned char Rn);            // ��������� ������ �������� ���������� ��������� (��� ������� ������)
BYTE GetRange(void);                        // ���������� ������� �������� ������ �������������� ���������
float GetPower(unsigned int Lambda);       // �������� �������� � ��� � ����������� �� ����� �����
unsigned int GetPMWavelenght(signed char DeltaLambda); //����������, ���� �������� ������� ����� ����
unsigned int SetPMWavelenght (unsigned int CurrPMWavelenght); // �������������� ��������� ������� ����� �����
float Watt2dB(char *str, float mWatt,BYTE TypeRslt);    // ����������� ��� � ���� (���), �� ��� ���
int GetTypeRslt();                          // ���������� ������� �������� ���� ��. ��������
int SetTypeRslt(BYTE type);                 // ������������� ����� �������� ���� ��.���������
int AcquireShZeroRng(void);                 // ��������� ������� ��������
int AcquireShZeroLowRng(void);                          // ��������� ������� �������� �� ������ ��������� (������������� ����)
int SetDataADC (int Data); // ��������� ������ ��� � ����������
void SetStateADC (BYTE State); // ��������� ������ ���
BYTE GetStateADC (void); // ��������� ������ ���
int AcquireCoefStykRange(BYTE Rng, float* PrevRng, float* CurrRng);         // ��������� ����������� ����������� �������� ��������� � ����������
float GetCoefSpctr(WORD Lambda, float RealPower);     // ���������� ������������ ����������� ��� ������� ����� �����
float GetCoefSpctrKlb(WORD index, float RealPower);    // ���������� ������������ ����������� ��� ������������� ���� ����
// ��������� ������ ����� ���� ������� Wide � JDSU ��������� ?????????? ���������
void SetAutoLamdaJDSU (DWORD Freq);
// ����� �������� ������ ����� �� �������
unsigned int GetLambdaJDSUFreq (unsigned int Freq);


#endif  /* __PM_H__ */
