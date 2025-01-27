// OTDR_Measure
// 

#ifndef __OTDR_MEAS_H__
#define __OTDR_MEAS_H__


#define PNTVERTICALSIZE 4 // ����� ������������ ���������

void SendSetALT ( WORD Data); // �������� ����� � �������
WORD CalkSetALT (unsigned int NumPnt, unsigned int CurrPos);
unsigned PosCursorMain (int Dir);//��������� � ��������� �������� ������� - ��������
unsigned SetPosCursorMain (unsigned Point);//���������  ������� - ��������
unsigned PosCursorSlave (int Dir);//��������� � ��������� �������� ������� - ��������
unsigned SetPosCursorSlave (unsigned Point);//���������  ������� - ��������
int GetNumAccumPerSec (void); //��������� �������� ���������� � ������ ������
void SetNumAccumPerSec (int Data);// ��������� �������� ����� ����������
unsigned GetVerticalSize (BYTE Index);//��������� ������������� ������� ����������� ��������������
//void  SetVerticalSize (unsigned Size);// ��������� ������������� ������� ����������� ��������������
unsigned GetSetHorizontScale (int Dir);//���������-��������� ��������������� �������� ����������� ��������������
//void ButtonsForOTDR (void);
float Calc_LSA (unsigned int start_P,unsigned int end_P);// ���������� ��������� ��������� ��� ������ 2


#endif
