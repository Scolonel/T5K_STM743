#ifndef DRAWFUNCTIONS_H
#define DRAWFUNCTIONS_H

#include "system.h"

#define NOCHANGE 0
#define UPPICS 1
#define DOWNPICS 2
#define UP10PICS 3
#define DOWN10PICS 4
#define MEDIUM 5
#define BYCUR 6
#define MEMDRAW 7
#define FIXSIZE 8

#define ScrH_N  (210)  // Nexion
#define ScrW_N  (390) // Nexion


// ��������� ��� �������� - ������������� � ������� ��������� ������
typedef struct tagRect
{
    long    left;
    long    top;
    long    right;
    long    bottom;
} Rect;

// ��������� �������� ���������� ���������
typedef struct tagGraphParams
{
  unsigned maxy;      // �������� �� Y
  unsigned cursor;     // �������������� ������� [0, 4095]
  unsigned scale;     //  ��������: 1 - ����� 1 ����� ������� ������� ����� �� 1 ����� 
  BYTE numcur;     // ����� �������� 0- ���,1- �������� 2- ��� ��������������
  BYTE typecentral;     // ����� ��������� ���������� 0- ��� (�������),1- ������������ ���1 2- ��� ��������������
} GraphParams;



//unsigned short MakeGraph( Rect* rct, unsigned short* array, unsigned size, GraphParams* prms );
unsigned MakeGraphNext( Rect* rct, unsigned short* array, unsigned size, GraphParams* prms );
// ��������� �������� �� ������������������ ������, ������� ������������ ����. �������� ����,
// ������ �����. ������������ ����� ������� ����� � ���������� ����.
//void DrawPicture (Rect* rct, const unsigned char* file);

unsigned GetLevelCurs (int Dir);

//void DecrBinDec(unsigned char *nm);
//void InitRect (Rect* rct, long left, long top, long right, long bottom);
//void InitParams (GraphParams* prms, unsigned cursor, unsigned scale, unsigned maxy, BYTE numcur, BYTE typecentral);
//void DrawDigitScore(BYTE Digit, int x, int y);
unsigned MinMax( unsigned short* array, unsigned nums );
unsigned MinMaxNEX( unsigned short* array, unsigned nums );
unsigned short Adjust4Nex( int val, int center, int range );
//unsigned short Adjust2( int center, int min, int max, int rng );


typedef struct 
{
  unsigned int x;
  unsigned int y;
}Magnifier;

//void DrawMgn(Magnifier M,char whereto[1024]);

//void MoveTo(Magnifier M,int x,int y,char whereto[1024]);

typedef struct
{
  Magnifier M;
  unsigned int start;
  unsigned int fixcur;// ���������� �������������� �������
  char mode;
  char *statusbar;
}Desktop;

//void DrawDskt(Desktop D,char whereto[1024], short unsigned int func[OUTSIZE]);//short int

float Calc_LSA (unsigned int start_P,unsigned int end_P);// ���������� ��������� ��������� ��� ������ 2
void SendDrawNex (unsigned char* array, int ID_lcd, int Points);

#endif
