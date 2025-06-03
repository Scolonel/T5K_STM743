#ifndef __ATT_MODE_H__
#define __ATT_MODE_H__

#define MAXWIDESMBL 17 // ������������ ����� ������� �������� � ������ 
#define MAXSMALLSMBL 22
#define WAITWELCOME 100 // ����� ������ �������� ��������
#define PLUS 1
#define MINUS 0
#define Pic_Tx 67 // X - �������� �������� ��� ������ ��������� �������
#define Pic_Ty 5 // Y - �������� �������� ��� ������ ��������� �������

//char GetModeDev (void); // ���������� ������� �����
void CmdInitPage(int Num);

void ModeFuncTmp(void);
void SetMode( void(f)(void) );


void ModeWelcome(void); // ��������� ��������
void ModeMain(void); // ���������� � ���������� ��������� ����
// ������������ � ����� ���������������� ���������� (���� �� �����  � ������
void UploadFW_Nextion(void); // ���������� ���������� NEXTION
void BadBattery(void); // ������ ���������

//void SF_MainT4000(void); // ���������� � ���������� ��������� ����
//void SF_MainT5xxx(void); // ���������� � ���������� ��������� ����
//void SF_ResultTable(void); // ���������� � ���������� ���������� ����������� �����������
//void SF_ResultGraph(void); // ���������� � ���������� ������������ ����������� �����������
//void SF_OnOffMode (void); // ����� ��������� ������� (������ �����???)
//void DeviceAllOff (void);// ���������� ������� (���� �������) ��� ����������
//void SF_MemoryMain (void); // ���� ������ � �������
//void SF_MainSetting (void); // ���� ������ ��������� (������, ����/�����, ����)
//void SF_SettingDev (void); // ���� ��������� �������
//void SF_SettingDaTime (void); // ���� ��������� ���� � �������
//void SF_SettingFile (void); // ���� ��������� ���� ()
//void SF_ErrMsg (void); // ��������� ����������� ������ ������� 
////void vSetCHKey(char* Dims);
//
//// ���������� ���������� � ������������� �������� �� ������
//void KeybCntrl (void); // ������������� ��������� � ����������
//void Draw_KeyBoard(unsigned char Cursor, char* Str_first, char* Str_edit, unsigned char Shift ,unsigned char Lang);
//void Knob(unsigned char X,unsigned char Y);
//unsigned char SearchEndStr(char* EditStr, unsigned char FindEnd); // ���� ����� ������� � ������������� ������
//void FrBig(unsigned char SW,  char Add); //������� �����
//void FrLamda9(unsigned char SW);
//void FrLamda18(unsigned char SW);
//void FrSmall(unsigned char SW,  char Add);// ��������� �����
//void FrFree(unsigned char SW ,  int X, int Size);// ����� ������������� ������� � �����
//void FrdB(unsigned char SW ,  int X, int Size); //����� ��� ��������� �������� �������� �������� ���������
//void FrVal(unsigned char SW ,  int X, int Size);
//void FrFreePx(int X, int Y ,  char* PrntStr);
//unsigned char ChangeFrSet (unsigned char FrSet, unsigned char MaxSet, unsigned char MinSet, unsigned char DirSet);// ��������� ������� � ������ �������� ����������

#endif  /* __ATT_MODE_H__ */

