// ������������ ������� ������� SRRNXN4MGXT310 
#include "system.h"

#define BC_def 800 // ������� ��������� �������� � ����������� �� ����� ������ �� 80-60 ��
#define FPO_def 0
#define NF_def 65535
#define NFSF_def 1000
#define PO_def 0
#define LT_def 200  // 0.2dB ����� ����������� ������� �� ���������
#define RT_def 65000  // 65dB ����� ����������� ������� �� ��������� �� 0 �� 65 ��
#define ET_def 3000  // 3dB ����� ��������� ��� ����������� ����� �����
// ������� � ������ ����������� ������ ��� ����������� ������ �����
#define ADDTONOISE 10000 // ������� � ������ ��� ����������� ������ �����


EVENTS EvenTrace[SizeTableEvnt];
// ��������� ��������� ����� ������� �������������� (�������� ���������)
ENDBLKEVENTS EndEvenBlk;
// ��������� ��������������� ������� ��� ������� � ������
MY_EVNT MyEvenTrace[3*SizeTableEvnt];

//unsigned short PosEndEvenBlkELMP; // ������� ������������ ����� ����� � �������� (���� �������������� ��� ��������� ������

static unsigned short CntKeyEvents = 0;
static unsigned short EnaKeyEvents = 0; //������� ���������� ������ ������� � ���� �������

void InitMyEventsTable (void) // ������������� �������� ���������� �������
{
  for (int i=0;i<3*SizeTableEvnt;++i)
  {
    MyEvenTrace[i].EN = i+1;
    MyEvenTrace[i].EPT = 0;
    MyEvenTrace[i].Volume = 0;
    MyEvenTrace[i].PointInImp = 0;
    MyEvenTrace[i].Type_Evnt = 0;
    MyEvenTrace[i].CompBell = 0;
  }
}

void InitEventsTable (void) // ������������� �������� �������
{
  for (int i=0;i<SizeTableEvnt;++i)
  {
    EvenTrace[i].EN = i+1;
    EvenTrace[i].EPT = 0;
    EvenTrace[i].ACI = 0;
    EvenTrace[i].EL = 0;
    EvenTrace[i].ER = 0;
    EvenTrace[i].EC[0] = '2';
    EvenTrace[i].EC[1] = 'O';
    EvenTrace[i].EC[2] = '9';
    EvenTrace[i].EC[3] = '9';
    EvenTrace[i].EC[4] = '9';
    EvenTrace[i].EC[5] = '9';
    EvenTrace[i].LMT[0] = '2';
    EvenTrace[i].LMT[1] = 'P';
  for (int j=0;j<9;++j)
  {
    EvenTrace[i].COMM_EVN[j] = ' ';
  }
    EvenTrace[i].COMM_EVN[9] = 0;
    
  }
  // ������������� ����� ����� �������
  EndEvenBlk.EEL = 0;
  EndEvenBlk.ELMP[0] = 0;
  EndEvenBlk.ELMP[1] = 1;
  EndEvenBlk.ORL = (unsigned short)(g_VolORL*1000);
  EndEvenBlk.RLMP[0] = 0;
  EndEvenBlk.RLMP[1] = 1;
  //PosEndEvenBlkELMP = 1;
}

DWORD CheckReflParam (void)
{
  DWORD err = 0;
  if ((ReflParam.BC>850)||(ReflParam.BC<450)) {err++; ReflParam.BC = BC_def;}
  if (ReflParam.FPO!=0){err++; ReflParam.FPO = FPO_def;}
  if (ReflParam.NF<40000) {err++;ReflParam.NF = NF_def;}
  if (ReflParam.NFSF!=1000) {err++;ReflParam.NFSF = NFSF_def;}
  if (ReflParam.PO!=0) {err++;ReflParam.PO = PO_def;}
  if ((ReflParam.ET>20000)||((ReflParam.ET<100))) {err++; ReflParam.ET = ET_def;} //end line 0.1 --- 20.0
  if ((ReflParam.LT>5000)||(ReflParam.LT<50)) {err++; ReflParam.LT = LT_def;}  //styk  0.05 --- 5.0
  if ((ReflParam.RT>RT_def)||(ReflParam.RT<10000)) {err++; ReflParam.RT = RT_def;} // refl 10 ... 65
  return err;

}

unsigned short GetNumEvents (void) // ���������� ����� ������� (
{
  return CntKeyEvents;
}

unsigned short GetSetEnaEvents (unsigned Dir) // ������������� ���������� ������� ���������� �������
{
  if (Dir) // �������� �������
  {
    if (EnaKeyEvents) EnaKeyEvents=0;
    else EnaKeyEvents=1;
  }
  
  return EnaKeyEvents;
}

// ������ �������� ���������� ��������� ������� � ������ ����� � �������
float CalkSCO (unsigned short* array, unsigned short BeginPoint, unsigned short Size)// , float* Base
{
  float b1;
  float Sy=0.0,Sy2=0.0;
  for (int i=BeginPoint; i<BeginPoint+Size; ++i)
  {
    Sy = Sy + array[i];
    Sy2 = Sy2 + array[i]*array[i];
  }
  
  b1 = (Size*Sy2)- Sy*Sy;
  b1 = b1/(Size*(Size-1));
  b1 = sqrtf(b1);
  //Base = (Sy - b1*Sx)/Size;
  return b1;
  
}

// ������ ������ �� ������ ����� ��������� ������� � ������ ����� � �������
float CalkTrend (unsigned short* array, unsigned short BeginPoint, unsigned short Size, unsigned short TrLvlDwn, float b0)// , float* Base
{
  float b1,  bx;//b0,
  float Sx=0.0,Sy=0.0,Sxy=0.0,Sx2=0.0, Smin=0.0;
  for (int i=BeginPoint; i<BeginPoint+Size; ++i)
  {
    if (array[i]>Smin) Smin = array[i];
    Sx = Sx + i;
    Sy = Sy + array[i];
    Sxy = Sxy + (i*array[i]);
    Sx2 = Sx2 + i*i;
  }
  
  b1 = ((Sx*Sy) - (Size*Sxy))/((Sx*Sx)-(Size*Sx2));
  // ������ �������� ��� �������� �������� ������
  if (b0 != 0.0)
  {
  //b0 = GetLSACoef();// ���� ������� �������������� �� ����������
  b1 = (b1 - b0)*(Size-1);
  // ������������� ����� ���������� ��������
  if (b1 < (float)ReflParam.LT)// ����� ��������������� �������
  {
    // ������������� �������
    bx = -b1;
    if ((bx < (float)ReflParam.LT)||(bx > 1500)||(TrLvlDwn<Smin)) // ����� -1.5 ��
    // ���������� ������ ������������� �� ����������� ������ �������� ��������� � �����
    b1 = 0.0; 
  }
  else // ������� ��������� ���� ����� ����, ������ ��� � ����������� �� ������ �������
  {
    if ((array[BeginPoint] > TrLvlDwn)&&(b1 <= (0.3*(Smin-TrLvlDwn)+200.0)))
      b1 = 0.0;

  }
  }
  
  //Base = (Sy - b1*Sx)/Size;
  return b1;
  
}
// ������ ������� �� �������� ������� (�� ��������� ������������ �������� �� ��������� ��� � ����������,
// ������� ������� �� ����� � ���� ��������� ����� �� ������� �������� �������)
unsigned short CalkDownJump (unsigned short* array, unsigned short BeginPoint, unsigned short Size)
{
  float MedBeg = 0, MedEnd = 0;
  unsigned short SizePlusAdd= 2;
  if (Size<10) SizePlusAdd = 5;
  else SizePlusAdd = Size>>1; // ������� ����� ��������� (�� ����� �� ������ ���������)
  // ������ ��������������������� ��� ������ �� ����� ��������� ���������
  for (int i=0; i<SizePlusAdd; ++i)// ���� �� ����������� ����� (��������� �� �����)
  {
    MedBeg += array[i+BeginPoint];
    MedEnd += array[i+Size+SizePlusAdd+BeginPoint];
  }
  MedBeg = MedBeg/SizePlusAdd;
  MedEnd = MedEnd/SizePlusAdd;
  if ((MedEnd - MedBeg)>(float)(ReflParam.LT))
  {
    return (unsigned short)(MedEnd - MedBeg);
  }
  
return (0);
}
// ������ ���������  �� ������ ����� ��������� ������� � ������ ����� � �������
float CalkCorelation32 (DWORD* array, unsigned short BeginPoint, unsigned short Size, BYTE Type)// , float* Base
{
  
  // ����������� ������ ���� �� 3 ����� , ����� ���������� ���� ��� ������������� ����������� � ����������
  // ��� ��� ��� ��� ��������� 166 �� � ������ 750 �� �������� ��� 3 ����� ����������
  double Sxy=0.0,Sx2=0.0,Sy2=0.0 ,Sx=0.0,Sy=0.0;//
  double Xi = 0.0, Correl = 0.0;
  double tmp;
  //static double  Alt_Cor = 1.0,Yi = 0.0 ;
  //double Ymed = 0.0;
  double Dxy = 0.0,Dx = 0.0, Dy = 0.0;// 
  double ImExpMax = 0; // ��������� �������� �������� ��� ������� �����
 // unsigned short Ymin=65535, Ymax=0; // �������� ��������� � ���������� �� ���������
 // int Imin=0, Imax=0; //������� ��������� � ����������
//  unsigned short SizePlus= 2;
//  if (Size<10) SizePlus = 5;
//  else SizePlus = Size>>1; // ������� ����� ��������� (�� ����� �� ������ ���������)
  // ������ ��������������������� ��� ������ �� �������� ���������
  for (int i=0; i<Size+SizePlus; ++i)// ���� �� ����������� ����� (������������ �������� ���� ��������� �� �����)
  {
    // ������������ Yi
    tmp = (double)array[i+BeginPoint];
    Sy += tmp; 
    Sy2 += pow(tmp,2);
    // ��������� Xi
    switch(Type)
    {
    case 1:
   // �������� ��������  ---\____
//    if ( i<SizePlus || Size+SizePlus<i )
//      Xi = ((i<SizePlus)?(1.0):(-1.0));
//    else 
//      Xi = 1.0-2.0*(i-SizePlus)/(Size+1);
      Xi = 0;
      break;
    default:
// �������� ��������  ____/---\____
     if ( i<=Size )
     {
       Xi = 1000.0*(1.0 - exp(TauImp()*i));
       ImExpMax = Xi;
     }
    else 
      Xi = 1000.0*ImExpMax*(exp(TauImp()*(i-Size)));
      break;
    }
    // ������������ Xi
    Sx += Xi;
    Sx2 += pow(Xi,2);
    Sxy += Xi*tmp;
  }
  Dxy = Sxy - (Sx*Sy)/(Size+SizePlus);
  
  Dx = sqrtf(Sx2 - ((Sx*Sx)/(Size+SizePlus)));
  Dy = sqrtf(Sy2 - ((Sy*Sy)/(Size+SizePlus)));

   Correl = Dxy/(Dx*Dy);
  
  return (float)Correl;
  //return (float)Alt_Cor;
  
}


unsigned short CalkEventsKeys (unsigned short* array, unsigned short PII, BYTE Type) // ���������� ������� ��� ������� ��������������
{
#define DELTACHNG 400 // ��������� ������� 0.4 ��
#define MORED 8 // ������� � PII ��� ����������� ������� �������������� ������� ����� ���������
  
  unsigned short CurLvlVol = DELTACHNG; // ������� ������� ������� 
  unsigned short NewMinLvlVol = DELTACHNG; // ������� ����������� ������� ������� 
  unsigned short IndexCurLvlVol = 0; // ������ �������� ������� ������� 
  unsigned short IndexCurBeg = 0; // ������ ������� �t �������� ������� ������� 
  unsigned short NumEvents = 0; // ������� �������
  //long ReflEvents = 0; // �������� ��������� � ���������� �������
  static unsigned short Count = 0; // ������ ������� - ������ ������� ������� 
  unsigned short EventsType1 = 0; // ������� ������� ����������� �������
  //unsigned short EventsType2 = 0; // ������� ������� ����������� �������
  unsigned short PosEventsType1 = 0; // ������� ����������� ������� ����� �� ���������� � ������ �����
  //unsigned short PosEventsType2 = 0; // ������� ����������� ������� ����� �� ���������� � ������ �����
  unsigned short PointsShift = GetPointsShift(); // ����� ��������� 
  unsigned short LvlTrendUp = GetLvlTrend(); // ��������� ������� ������ ����� ��� ������� ������ 
  float LSACoefIN = GetLSACoef();
  int FindBeg = (int)(LSACoefIN*PII); //+ (DELTACHNG/2) �������� ��������� � ����������� �� ���. ���������
  int ChkLvlUp ; // ��������� ������� �������� ����������� �������
  long BegTemp; // �������� ������ �������
  
  //float AutoRef = 200;
  float Trend = 1000;
  //float CurrTrend = 1000;
  int temp=0, temp1 = 0;
  //char UartStr[40];
  //float SumBuff=0;
  //float Correlations;
  float Buff[160];
  memset( Buff, 0, 160 * sizeof(int) );
  //  InitEventsTable (); // ������������� �������� �������
  Count = 0;
  int CurrBig = 0;
  //int YesEvents = 0; // ������� ����������� �������
  //int YesCorrFix = 0; // ������� ������������� ��������
  //int CorrFix = 0; // �������� ����������
  //int CountEvents = 0; // ������� ������������ �������
  //unsigned short Sobytie;
  unsigned short LevlRefl; // ������� ����������� ������� ��� ���������
  volatile unsigned short i;
  unsigned short DeadPoint = 0; // ����� �� ������� �� ������� ����� ���� ���� ���������
  unsigned short DeadPointUp = 0; // ����� �� ������� �� ������� ��������� ���������� �������
//  unsigned short EndDeadLine = CalkEndRefl ( array,  PII,   LvlTrendUp);

  // �������� ����� ���������� ���������
  //    sprintf (UartStr, "PII=%d Shift=%d \n",PII,PointsShift);
  //   UARTSend0 ((BYTE*)UartStr, strlen (UartStr));
  float TrOld=0.0, TrCurr=0.0, TrNew=0.0;
  // ����� ����� ����� , � ����� ��������������

  for (i = PII; i<OUTSIZE-2*PII; i++)
  
  //for (i = PII; i<=EndDeadLine; i++)
  {
    // ���� ����� ��� �������� ������
    //  sprintf (UartStr, "%d %.3f \n",array[i],CalkTrend(array, i, 2*PII, LvlTrendUp, 0.0));
    // UARTSend0 ((BYTE*)UartStr, strlen (UartStr));
    
    // �������� ������������ �������� �������
    if ((array[i] < 230)) // &&(i<4*PII)0.23 �� � ������ 4 ������������� ��������
    {
      if ((i-CurrBig)>1) // ����� ���������� ������ 3�� 
      {
        CurrBig = i;
        Count = 0;
      }
      else // ����������� ����������... �������
      {
        CurrBig = i;
        Count++;
      }
      if (Count > PointsInImpulse(1)) // 
      {// ������� ��������� ��������� �� ����� ������
        NumEvents++;    
        //if ((GetIndexLN()==0)&&(IndexCurLvlVol>0)) IndexCurLvlVol--; // ��������� ������� ������ �������// ��� 2 �� ����� ����������
        
        EvenTrace[NumEvents-1].EPT = IndexCurLvlVol;
        EvenTrace[NumEvents-1].EC[0] = '2';//2
        EvenTrace[NumEvents-1].EC[1] = 'E';//E
        EndEvenBlk.ELMP[1] = IndexCurLvlVol;//���������� �������� ����� ����� ����� ... ���� ����������� � ��������

        memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "BigOvrEnd\0",10);
        if ((NumEvents>1)&&((EvenTrace[NumEvents-1].EPT>=EvenTrace[NumEvents-2].EPT-2)&&(EvenTrace[NumEvents-1].EPT<=EvenTrace[NumEvents-2].EPT+2)))// ���� ��������� - ����������� ����� �������
        {
          NumEvents--;    
          //EvenTrace[NumEvents-1].EC[0] = '2';//����� ������� �� ������������� ��� ������� �������
          EvenTrace[NumEvents-1].EC[1] = 'E';//E
          EvenTrace[NumEvents-1].EL = 0;  
          memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "LineEnd_0\0",10);
          // �������� ER ��������� ������...
        }
        EndEvenBlk.EEL = array[EndEvenBlk.ELMP[1]]-EndEvenBlk.EEL;
        
        if (IndexCurLvlVol == 0)// �� ���� �������� ������� ����� ��� �� 0.4 ��
        {
          // ������ ����� ��� ����� ������� ��������� � ������
          // ��� ��� ��������� ��������� ��� ���������
          EvenTrace[NumEvents-1].EPT=0;
          EvenTrace[NumEvents-1].EC[0] = '2';
          EvenTrace[NumEvents-1].EC[1] = 'E';
          //EvenTrace[NumEvents-1].ER =  CalkEventER (array[0] - CalkMINMAX (&array[i], PII+1, 0)); // ���������� ���������
          EvenTrace[NumEvents-1].EL =  0; // ���������� ���������
          
          //EndEvenBlk.ELMP[1] = i;//���������� �������� ����� ����� ����� ... ���� ����������� � ��������
          memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "NotFiber1\0",10);
          NumEvents=0;    // ��� �������!!!
          
        }
        
        break; //������� ��������� �������
      }
    }
    
    //  �������� ������ �� ����� �����
    if (array[i]> (LvlTrendUp+ADDTONOISE))//GetNoiseLvl()
    {
      //IndexCurLvlVol = i;
      NumEvents++;
      
      EvenTrace[NumEvents-1].EPT=IndexCurLvlVol;
      EvenTrace[NumEvents-1].EC[0] = '0';//0
      EvenTrace[NumEvents-1].EC[1] = 'O';//O - out of range finding end of fiber
      // ������ ���� ������ �������������� �����
      // ��� �������� 18.5 ��, ��� ��������� 25 ��
      EndEvenBlk.ELMP[1] = IndexCurLvlVol;//���������� �������� ����� ����� ����� ... ���� ����������� � ��������
      EvenTrace[NumEvents-1].EL=0;
      memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "OutOfRng \0",10);
      // ���� ��������� - ���������� �������  ������� 1E, 
      if (i<DeadPoint+3) // ���� ������� ���������� �������
      {
        NumEvents--;
        EndEvenBlk.ELMP[1] = PosEventsType1;
        EvenTrace[NumEvents-1].EL = 0; //array[i]-array[PosEventsType1];// Found by software
        EvenTrace[NumEvents-1].EC[0] = '1';// reflective events with end fiber
        EvenTrace[NumEvents-1].EC[1] = 'E';// Found by software
        memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "FbEndRefl\0",10);
      }
      EndEvenBlk.EEL = array[EndEvenBlk.ELMP[1]]-EndEvenBlk.EEL;
      break;// ���� � ���� - ������� (���������� ������� ����� �����)
    }
    // ��������� ������ �������� �������
    temp = (int)(array[i])-(int)(array[i-1]);  // ������� ������� ������� ����� � ����������
    // �������� �� ������ ������ ������ ������ � ���� ������ �������� ����� � 15.000 ��
    // ����� ������ ������ ��� �� �������� �����, �� �������� ������ � ��� ���� ����� ��� ���� 15 ��
    if ((temp>0)&&(temp>ReflParam.ET)&&(array[i]>CurLvlVol)&&(CurLvlVol>15000))
    {// ��� ������������� ����� - ������ ���� �� ������� ������ ������
      NumEvents++;
      memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "BigJmpDwn\0",10);
      if ((i - IndexCurLvlVol)> (2*PII))
      {
        IndexCurLvlVol = i; // ���� ����� ����� ���������� ��������� ��� �� 2 ����. �������� 
      memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "BigJmpDnN\0",10);
        // ��������� ����� �����
      }
        
      EvenTrace[NumEvents-1].EPT = IndexCurLvlVol;
      EvenTrace[NumEvents-1].EC[0] = '0';//0
      EvenTrace[NumEvents-1].EC[1] = 'E';//E
      EvenTrace[NumEvents-1].EL=0;//array[IndexCurLvlVol]-array[IndexCurLvlVol-1];
      EndEvenBlk.ELMP[1] = IndexCurLvlVol;//���������� �������� ����� ����� ����� ... ���� ����������� � ��������
      if (i<DeadPoint+3) // ���� ������� ���������� �������
      {
        NumEvents--;
        EndEvenBlk.ELMP[1] = PosEventsType1;
        EvenTrace[NumEvents-1].EL = 0;//array[i]-array[PosEventsType1];// Found by software // ������� ��������� � �������
        EvenTrace[NumEvents-1].EC[0] = '1';// reflective events with end fiber
        EvenTrace[NumEvents-1].EC[1] = 'E';// Found by software
        memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "FbEndJmp \0",10);
        
      }
      EndEvenBlk.EEL = array[EndEvenBlk.ELMP[1]]-EndEvenBlk.EEL;
      
      break;
    }
    // �������� ���� ��� �� ������ �� �������� ������,
    // ������������ ��������� ��������� �������
    // ��� �� ����� �� ������� �� �������� �� "��������"
    if ((array[i]>(CurLvlVol-DELTACHNG))&&(array[i]>(NewMinLvlVol-DELTACHNG))&&(abs(temp)<DELTACHNG)&&(temp!=0))
    {
      if (NewMinLvlVol<array[i])
      {// ����� ������� ��������� ���� ������� ���� ����� ������������ ��������???
        NewMinLvlVol=array[i];
      }
      CurLvlVol = array[i]; // ��������� ����� ��������
      IndexCurLvlVol = i;
    }
    // �������� ������ ������� ��� �� �������� �������������� �������
    // 18 ��, ������������� ������� ������ ����� ��� ������ ��������������� ������� ����� ��������
      if ((array[i]>18000)&&(IndexCurBeg == 0)) // not fiber - ��� �������
      {
      NumEvents++;    
      EvenTrace[NumEvents-1].EPT=0;
      EvenTrace[NumEvents-1].EC[0] = '0';
      EvenTrace[NumEvents-1].EC[1] = 'E';
      EvenTrace[NumEvents-1].EL = 0;// Found by software // ������� ��������� � �������
          //EndEvenBlk.ELMP[1] = i;//���������� �������� ����� ����� ����� ... ���� ����������� � ��������
          memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "NotFiber2\0",10);
      NumEvents = 0;   // ��� �������!!!!
        
        break; //��� ����� - �������
        
      }
    
    // ����� ������ ����� ����� �������� ������������ ��������
    temp1 = array[i+PII] - array[i];
    // ���� ���� ��� ����� ������������� ������ ��� �� 0.2 �� � �� ���� ����� ��� 
    if ((temp1<FindBeg)&&(IndexCurBeg == 0)&&(array[i] > 1000))//&&(temp1>0)
      //if ((temp>DELTACHNG/2)&&(IndexCurBeg == 0)&&(array[i] > 3000))
    {
      // �������� ������� ������� �� �������������� �������
      IndexCurBeg = i;
      NumEvents++;    
      //EvenTrace[NumEvents-1].EPT=IndexCurLvlVol;
      EvenTrace[NumEvents-1].EPT = 0;
      EvenTrace[NumEvents-1].EC[0] = '2';// 3 -������ ����� ��� �������� !!!!
      EvenTrace[NumEvents-1].EC[1] = 'F';// Found by software
      EndEvenBlk.ELMP[0] = 0;//���������� �������� ����� ... ���� ����������� � ��������
      // ��������� ��������� � ������ �� ���� �������
      // (unsigned short)(((unsigned int)((640*i)/NumPointsPeriod[GetIndexLN()]))<<6)
      // ��������������� ������ �������� � ������ �����, �������� �������� � ������ �����,
      // 
      
      BegTemp = (long)(i*LSACoefIN); // ��������� �� ������� ������� �������
      //BegTemp =  BegTemp/NumPointsPeriod[GetIndexLN()];
      //BegTemp =  BegTemp>>6;
      
      EndEvenBlk.EEL = (long)(array[i])- BegTemp ;//���������� �������� ����� .� ������ �����
      array[0] = (unsigned short)(EndEvenBlk.EEL);// �� ������������ ��� ��� ��� ��������� � EndEvenBlk.EEL
      EvenTrace[NumEvents-1].EL =  array[0]; // ��������� ��������� � �����
      
      // ����� ���������
      //unsigned short tempMIN = CalkMINMAX (&array[0], i, 0);
      EvenTrace[NumEvents-1].ER =  CalkEventER (EndEvenBlk.EEL - CalkMINMAX (&array[0], i, 0));
      memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "FirstBeg \0",10);
      NumEvents--;    
      
    }
    // ����� ������ ������� 
    if ((Type)&&(IndexCurBeg))
    {
      unsigned short PIIM = PointsInImpulse(0);
      // �������� ������ ������ �� �������
      // �������� ������ ���������� ��������� �� �������
      if ((i > DeadPoint)&&(i > DeadPointUp))
      {
        Trend = CalkTrend(array, i, PIIM, LvlTrendUp, LSACoefIN);
      }
      else
        Trend = 0.0;
      
      
      // ������ ������ ��������������� ���������� �� ����������� �������  
      //sprintf (UartStr, "%d %d    %.3f \n",PIIM,i,Trend);
      //UARTSend0 ((BYTE*)UartStr, strlen (UartStr));
      
      // ����������� ���������� �� 3-� ������
      TrOld = TrCurr;
      TrCurr = TrNew;
      TrNew = Trend;
      if ((TrOld>0)&&(TrCurr>0)&&(TrNew>0)) // ����� �� ������� �������� ������, ��������� ���������
      {
        if((TrCurr>=TrOld)&&(TrCurr>=TrNew)&&(NumEvents<(SizeTableEvnt-1))&&(TrCurr<ReflParam.ET))// ����� ������������� ���������, �� ������ ������ ����� �����
        {
          // ������ ������� �������
          //if ((LvlTrendUp > array[i-1])||(TrCurr >= (0.3*(array[i-1]-LvlTrendUp)+200.0)))
            // ���� ������ ���� ������� ������ ������ - ��������� ������� (������� ���������)
          //{
            NumEvents++;    
          EvenTrace[NumEvents-1].EPT = i-1;// �������� ������� �������
          EvenTrace[NumEvents-1].EC[0] = '0';// NO reflective events
          EvenTrace[NumEvents-1].EC[1] = 'F';// Found by software
          EvenTrace[NumEvents-1].EL = (short int)TrCurr;// Found by software          
          EvenTrace[NumEvents-1].ER = 0;
          memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "NoReflEvt\0",10);
          DeadPoint = i + PII;
          //}
        }
      }
      // ����������� ������������� ������� ��� ������������� ���������
      if ((TrOld<0)&&(TrCurr<0)&&(TrNew<0)) // ����� �� ������� �������� ������, ��������� ���������
      {
        if((TrCurr<=TrOld)&&(TrCurr<=TrNew)&&(NumEvents<(SizeTableEvnt-1))&&(-TrCurr<ReflParam.ET))// ����� ������������� ���������, �� ������ ������ ����� �����
        {
          //if (LvlTrendUp > array[i-1])// ���� ������ ���� ������� ������ ������ - ��������� ������� (������� ���������)
          //{
          NumEvents++;    
          EvenTrace[NumEvents-1].EPT = i-1;// �������� ������� �������
          EvenTrace[NumEvents-1].EC[0] = '0';// NO reflective events
          EvenTrace[NumEvents-1].EC[1] = 'F';// Found by software
          EvenTrace[NumEvents-1].EL = (short int)TrCurr;// Found by software          
          EvenTrace[NumEvents-1].ER = 0;
          memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "NpReflEvt\0",10);
          DeadPoint = i + PII;
          //}
        }
      }
      
      /*
      Trend = (array[i+PointsShift]-array[i])/1.0;
      int a = i - PII;
      // 50*�(0.0002*level) - ���� ����� ������ ����������� �������
      AutoRef = -50*exp(0.0002*array[i]) - ReflParam.LT;
      //
      SumBuff = SumBuff + Trend;  
      if( a - PII >= 0 ) SumBuff = SumBuff - Buff[ a % PII ];
      Buff[ a % PII ] = Trend;
      
      Correlations = CalkCorelation(array, i, PII); // ������ ����������� ������������� ���������
      // ������ ������� ������ �� PII �����, ���� ���������� ������� ����� ������ ������� ������������� � �������� ������
      // ��� ������ ������� �������� 0.2 ��, ����������� ������ ����� 20 ������ ����������,
      // ��� ������ � 1�� ������� ����� 100������, � ��� �����
      Sobytie = CalkDownJump(array, i, PII);
      CorrFix = 0;
      if (Correlations<-0.7)
      {
      if (!YesEvents)// ���� ������� begin
      {
      YesEvents = 1;
      CountEvents = 0;
    }
      else
      {
      CountEvents++;
      if (CountEvents>PII)// ������� ����������
      {
      YesCorrFix = 1; // ���������� ������������� �������
    }
    }
    }
      else
      {
      YesEvents = 0;
      if (YesCorrFix)
      {
      YesCorrFix = 0;
      CorrFix = i - PII;
    }
    }
      
      
      if ((Trend < (AutoRef))&&(!EventsType2)) // ����� ��������� 
      {
      EventsType2 = 1;
    }
      if ((EventsType2) && (Trend < AutoRef))
      {
      if (Trend<CurrTrend) 
      {
      CurrTrend = Trend;
      PosEventsType2 = i;
    }
    }
      if ((Trend > 0) && (EventsType2) && (NumEvents<(SizeTableEvnt-1)))
      {
      NumEvents++;    
      EvenTrace[NumEvents-1].EPT = PosEventsType2;// �������� ������� �������
      EvenTrace[NumEvents-1].EC[0] = '1';// reflective events
      EvenTrace[NumEvents-1].EC[1] = 'F';// Found by software
      CurrTrend = 1000;
      EventsType2  = 0;
      memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "NoRefEvnt\0",10);
    } 
      */
      // �������� ����������� �������
      // ����� �������� ������ ����������� �������
      ChkLvlUp = (int)array[i] - (int)LvlTrendUp;
      if (ChkLvlUp>0) ChkLvlUp =  (ChkLvlUp>>1) + DELTACHNG;
      else ChkLvlUp =  DELTACHNG;
      
      if (((array[i]-array[i+2])>ChkLvlUp)&&(!EventsType1)&&(NumEvents<(SizeTableEvnt-1))&&(i>DeadPointUp))// ������ ����� ����� ��� �� 0.4 ��
      {
        EventsType1=1;
        if (GetIndexLN()) IndexCurLvlVol = i; // ��������� ������� ������ �������
        else IndexCurLvlVol = i-1; // ��� 2 �� ����� ����������
        PosEventsType1 = IndexCurLvlVol;
        DeadPoint = i + PII + MORED;  // ������������� ��������� ����� ����� ����������� �������
        // ���� ��������� ��� �� ������� ����� ������� �� ���������� ������� ����
        
      }
      // ��������� �������....
      if (EventsType1) // ����� ����� �������
      {
//        if (EventsType1<(PII+1))
//          EventsType1++;
//        else // ��������� ������� 
        {
          if (NumEvents<(SizeTableEvnt-1))
          {
            // ����� ���������� ������� ����������������� �������� ������������� ������
            EventsType1 = 0;
            // ����� ���� ��������� ��� �� �������������� �������������� ��������� ��� "�����" ���������
            // 01.03.2022 ��� ����� ��������� ��������� ����� �� �������� ������������� ���������!
            // ��������� ������
            //if( (EvenTrace[NumEvents-1].EL < 0)&&(((EvenTrace[NumEvents-1].EPT)>(PosEventsType1-PII/2-2))&&((EvenTrace[NumEvents-1].EPT)<(PosEventsType1-PII/2+2))))
            //{
            //  NumEvents--; // ������� ������� - ���� ���� �������� ������� � ������������� ����������....
            //}
            
            NumEvents++;    
            EvenTrace[NumEvents-1].EPT = PosEventsType1;// �������� ������� �������
            EvenTrace[NumEvents-1].EC[0] = '1';// reflective events
            EvenTrace[NumEvents-1].EC[1] = 'F';// Found by software
            int step = 0;
            // 28.02.2022 �� �������� ����� ���������  ���������� ������� � "�������������" ����������
            // ����� ���� ���������� ������� - ��� ����� ���� � ������������� ����������
            // �.�. ��������� �������� ����� ������ ��� ������� ������ ������� ������� ���� ���� ����� ������ �������
            // �������� �� 1 ��
            // ����� ������� ����� ��������� ���� ������ �� ���������, ��� ���� � ���� ����!
            do
            { // ���� ����� ���� ������� � ������� ���. ������� �� 2 ����� �� �����
              int trew = array[PosEventsType1+ PII + step+1]-array[PosEventsType1+ PII + step];
              //if (((array[PosEventsType1+ PII + step]-array[PosEventsType1])>0)&&(trew<(DELTACHNG>>2))&&(trew>0)) break;
              // ������� � ������ �����  1 �� ��� ������ ������ �� �������������� �������
              if (((array[PosEventsType1+ PII + step]-(array[PosEventsType1]+1000))>0)&&(trew<(DELTACHNG>>2))&&(trew>0)) break;
            }
            while (step++ < (4095-(PosEventsType1 + 2*PII))); 
            //  if (step>50)
            //  {
            DeadPointUp = PosEventsType1 + PII + step;
            EvenTrace[NumEvents-1].EL = array[PosEventsType1+PII+step+1]-array[PosEventsType1];// Found by software - ������� ���������
            // �������� �� ������������� �� �������
            if(EvenTrace[NumEvents-1].EL < 0)
            {
          EvenTrace[NumEvents-1].EC[0] = '0';// NO reflective events
          EvenTrace[NumEvents-1].EC[1] = 'F';// Found by software
          EvenTrace[NumEvents-1].ER = 0;
          memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "NegEvt   \0",10);
             
            }
            // �������� �������� ������ �� ������� ������������ �������� ��� ����������� ����� �����
             if( (array[PosEventsType1+3*PII+step+1]-array[PosEventsType1+PII+step+1]) > ReflParam.ET)// ����� 3 ��
             {
               memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "ReflEvEnd\0",10);
               EvenTrace[NumEvents-1].EPT = IndexCurLvlVol;
               EvenTrace[NumEvents-1].EC[0] = '2';//2
               EvenTrace[NumEvents-1].EC[1] = 'E';//E
               EvenTrace[NumEvents-1].EL = 0;
               EndEvenBlk.ELMP[1] = PosEventsType1;//���������� �������� ����� ����� ����� ... ���� ����������� � ��������
               EndEvenBlk.EEL = array[EndEvenBlk.ELMP[1]]-EndEvenBlk.EEL;
               break;
             }
             
            //ReflEvents = array[PosEventsType1];
            memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "ReflEvtS \0",10);
            LevlRefl = CalkMINMAX (&array[PosEventsType1], PII+1, 0);
            if (LevlRefl < 100)
            {
              EvenTrace[NumEvents-1].EC[0] = '2';// reflective events - � �����������
              memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "ReflEvtB \0",10);
            }
            
            EvenTrace[NumEvents-1].ER =  CalkEventER (array[PosEventsType1] - LevlRefl); // ���������� ���������
            //EvenTrace[NumEvents-1].ER=array[PosEventsType1]-ReflEvents;
            // �������� ��� ������� �� ��������� � ��� �� ����� ����� (������ ��� ����������)
          }
          
        }
      }
    }
  }
  // �������� ���������� ������� 
//  if (EvenTrace[NumEvents-1].EPT < EndDeadLine) // ��������� ������� ������ , �������� �����
//  {
//    // ��������� ���� ������� ����� �����
//               memcpy (&EvenTrace[NumEvents-1].COMM_EVN, "FindEnd  \0",10);
//               EvenTrace[NumEvents-1].EPT = IndexCurLvlVol;
//               EvenTrace[NumEvents-1].EC[0] = '2';//2
//               EvenTrace[NumEvents-1].EC[1] = 'E';//E
//               EvenTrace[NumEvents-1].EL = 0;
//               EndEvenBlk.ELMP[1] = PosEventsType1;//���������� �������� ����� ����� ����� ... ���� ����������� � ��������
//               EndEvenBlk.EEL = array[EndEvenBlk.ELMP[1]]-EndEvenBlk.EEL;
//  }
//     

  // 
  return NumEvents;
}

// �������� �� ��������� ��������� ��� �������������� �������
// 
void CalkNonReflDwn (void)
{
  
}
// ����� ����� ����� , � ����� ��������������
unsigned short CalkEndRefl (unsigned short* array, unsigned short PII,  unsigned short TrLvlDwn)
{
  
  unsigned short EndLine = OUTSIZE-1;
  float Fir = 0.0, Two = 0.0, Tre = 0.0; 
  float Tst = GetLSACoef()*PII ;
  for (int i=OUTSIZE-1; i>PII; --i)
  {
    Tre = Two;
    Two = Fir;
    Fir = 0.0;
    if (array[i]<(TrLvlDwn) )// ���� ��������� ������ - ��������� ����� �� �������
    {
      Fir = CalkTrend(array, i-PII, PII, TrLvlDwn, 0.0);
      if ((Fir>0) && (Fir < (Tst+100.0))&& (Fir > (Tst-100.0)))
      EndLine = (unsigned short)i;
      else Fir = 0.0;
    }
    if (Fir>0 && Two>0 && Tre>0) 
    {
      EndLine = (unsigned short)(i+2);
      break;
    }
  }
  return EndLine;
}



// ������ �������� EPT ��� ������� �� ��������� �������
long CalkEPT (unsigned long Position)
{
        float NowDS;
        float EvntEPT;
            // �������������� ��������� �������� (��� ����������� ��������������)
        PointsPerPeriod = NumPointsPeriod[GetIndexLN()]; // SetPointsPerPeriod( ... );
        //NowDS = (ADCPeriod*50000)/PointsPerPeriod; //  ������������� �������� DS ��� �������������� ������ ���������
        NowDS = GetValueDS(); //  ������������� �������� DS ��� �������������� ������ ���������
        EvntEPT = Position*NowDS;
        EvntEPT = EvntEPT/10000;
        return (long)(EvntEPT);

}

unsigned short GetLvlTrend (void) // ������ ���������� ������ � ������� �������
{
  double Data = (5000.0*log10((double)(sqrt((float)(GetNumAverag()/8770.0)))))+16000.0; //������ ��������� ����� ��������
  if (GetApdiSet ())// c ��������� (���� ������)
  {
    if (GetIndexIM()>4) // filtr ON
    {
      Data = Data + 8000.0;
    }
  }
  else
      Data = Data - 3000.0;
  if ( Data <0) Data = 1000.0;
  return (unsigned short)(Data);
    
    
}

// ������ ��������� ��������� �� ������ ��������������

// ���������� ��������� � ����������� �� �������� ������� � ������������ ��������
// � ������� �1000 - �� ���� �������� ��������
// �������� ������ BC - 80 ��(800),�������� �������� � ��*1000, ������������ ��������
long CalkEventER (long LevelER)
//
{
  if (LevelER<=0.0) LevelER = 1.0;
  float CalkR = 10.0*log10(pow(10, (LevelER/5000.0))-1); // 10^(f/5)
  float Calk = GetWidthPulse(GetIndexIM());// �������� ������������ �������� � ��
  Calk = (ReflParam.BC/-10.0)+10.0*log10(Calk); // �������� Q � ����������� �� ������. ��������
  Calk = Calk + CalkR;
  return (long)(Calk*1000);
}

unsigned short CalkMINMAX (unsigned short* array, int size, BYTE type)
{
  unsigned short MINi = 65535;
  unsigned short MAXi = 0;
  for (int j = 0; j< size; ++j)
  {
    if (array[j] < MINi) MINi = array[j];
    if (array[j] > MAXi) MAXi = array[j];
    
  }
  if (type) return MAXi;
  else return MINi;
}

