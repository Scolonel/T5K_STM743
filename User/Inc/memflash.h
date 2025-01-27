#ifndef __MEMFLASH_H__
#define __MEMFLASH_H__



#define NUMPAGESFORREF 16
#define NUMWORDINPAGE 256
#define CHECKNUMTRACE Trace<=((CheckIDMEM)?(0x100+MaxMemOTDRExt):(MaxMemOTDR))

extern DWORD CheckMemFlash (void);
BYTE CheckMemID (void);
//extern CFG_DEV ConfigDevice;
void SaveConfigDevice (void);
void SaveUserConfig (void); // ���������� ���������������� ��������

// ������� ��������������
void TraceERASE(unsigned int Trace);
void TraceWRITE(unsigned int Trace); // ������ ������
// ������ ��������������
void TraceREAD(unsigned int Trace);
// ������ �������� ����������� ��������������
unsigned long FlashReadCommTrace (unsigned int Num, unsigned char *adress);
// ������ ������� ����������� ��������������(���� �����)
unsigned long FlashReadTimeTrace (unsigned int Num);

unsigned short SetNumTrace (unsigned short Num); // ��������� ������ ������
unsigned short GetNumTrace (void); // ��������� ������ ������
void CheckTableMemOTDR (BYTE MemErr); // �������� ������� ���������� ������������ 0 - �������� 1 - �������
unsigned short GetNumTraceSaved (unsigned short Index); // ������ �������� ������� ������������ �� �������
// �������� ������ - ������ �������� ������ �������������
void DrawMemoryRefl(unsigned short Trace, BYTE CurrLang, BYTE Param); // ������� ������������� ������ ����� ��������� � ORL
unsigned short SaveTrace(void); // ���������� ��������������
unsigned short DeletingTrace (unsigned short Trace); // "��������" ��������������, ������ ��������� 
unsigned short ReanimateTrace (unsigned short ReanT); // "����������" ��������� ����� �����

unsigned short DeletingAllTrace (void); // "��������"���� ��������������, ������ �������� ������� ����������
// ������� ������ �� ������������ ��������
// �������� ������� ����� Splice �������������
unsigned short FlashReadSpliceData (unsigned int NumData);
// ����� ��� �� � 250 �������������� (4001 pg...4016)
void SpliceWRITE(unsigned int Trace); 
// ��������� ���������� ������� ���������������
void AlarmReWriteTrace (void);

void SDMMC_SDCard_DIR(void); // ��������� �����������
void SDMMC_SDCard_FILES(void); // ��������� files
void SaveFileSD(int Mod); // ������ 0.sor �� �������� � ������� �����������
uint32_t StartInitSDcard (void); 



#endif  /* __MemFlash_H__ */

