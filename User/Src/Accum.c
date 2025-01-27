//===============================
#include "system.h"

// ���������� �������� �2, ���������� � ����� �������
volatile DWORD OldTimeAccum; // ���������� �������� ������� ����������
volatile DWORD CurrTimeAccum; // ������� ��������
uint8_t EnaTimerAccum =0; //������� ���������� ����� ������� ����������
int CntAccumulat=0; // ������� ����������



void RUN_SUM (DWORD* RawDataI);//


void SUMMER (DWORD* RawDataS)//
{
  //���� ��������� ��������� � ������������� �����������
      //memset(&BufADD, 0, sizeof(BufADD));
     //LED_KT(1); // ������ ������ ������
  
      memset(&BufNAK, 0, sizeof(BufNAK));
     //LED_KT(0); // ������ ������ ������

  // �������� ���������� 
  TIM1->CR1 |=TIM_CR1_CEN;

  // ��� ����� ���� ��������� � ������ ������������
         // ������ ���������� �� TIM1_CCR1 ���� ������ ����� ���1
      while(Ena_AVRG)
      {
        //CountWait=0;
        //LED_START(1);
        while(Ena_CFG) // ���� ��������� ��� ���������
        {
        //  CountWait++;
        asm("NOP");
        }
       // LED_START(0);
       // BufNAK[0] = CountWait;
       // CountWait=0;
       // LED_START(1);
        while(Ena_SUMM)
        {
       //   CountWait++;
        asm("NOP");
        }
       // LED_START(0);
       // BufNAK[1] = CountWait;
             ContNextAvrg();
      }
    //StopAllTIM(1); // stop all timers
  // � ������� BufNAK ����� ���� ���������� 
     //LED_KT(1); // ������ ������ ������

   for (int i=0;i<RAWSIZE;i++)                                  // !!!!!!!!!!! 
    RawDataS[i] += BufNAK[i];     // ������������ � ���� ������ ��������������� (���� ���� ��� ���) !!!!!!!!!!!
    // ������ ������ �����������
    //    memcpy( RawDataS, BufNAK, RAWSIZE * sizeof(DWORD) );
    // LED_KT(0); // ����� ����� ������

  //STARTPP(1); // ������������� � "1" START                            !!!!!!!!!!!
  //ADC_DATA_EN(1);// ������� �����                            !!!!!!!!!!!
  //PointInPeriod ++;    // ��������� ������������ 
      Sm_Shift = 0; // ������� �������� ������ ����.��������
      CountDMA = 0; //clear count DMA

      Ena_AVRG = 1; // ��� ����� ����������
      Ena_CFG = 1; // ��� ��������� ����� ������ ����������
      Ena_SUMM = 1; // ��� �������� ������ ������������ ����������

  asm("NOP");
}


void Averaging (int NumAccum,unsigned AddIndexLN, BYTE EnDrawGraph )// ������� ���������� � ���������� �������� ����� ����������
// ������������� ���������������: PointsPerPeriod - ����� ����� �� ������, ������� �� ����� ���������
// NumAccumPerSec - ����� ���������� � ������ ������
// AddIndexLN - ������ �� �������� ��������������� ������ ����� ��� ������ �������
// ������� 0 - ������������ ������ �������� ���������� ���������
// �� 0 - ������������ ������ ��������� ������� ��������� ����������� � StartRunFirst
{ 
  //    CntPointPick=0;

  //CurrTimeAccum = HAL_GetTick();
  OldTimeAccum = CurrTimeAccum;
  SetNumAccumPerSec (NumAccum);// ������ ����� ����������
  // ��� ��������������� ��������� ����� � ��������� ������������ �������� ��� ������������� �����
  // ��� ���� ���������� ���� �������� �������� ��� ���������, �������� �� ��������� �������� �������� ��� ����� ���
  // � ��������� ������ ��������� ( �� ���� ������� ���)
  // ������ �� ������� � ��������� ������ ���� �� �������� ��������� ����� ����������
  // � ���� ��� ������� 1- ���������� � ������ ������� ���������, StartRunFirst();
  // ��� ������������ �������� � ������������� �������, ����� �������� ��������
  // ��������������� ����� �������� (��������)���������� ������ ����� ������������� ���������� ���������� �� ����� ������������
  // !!! ����� ��������� ���������� ������ ���������� ��������, ����� ������ ���1 (� ��������� ����� 1 ���)
  // �� ���������� ��������� ����� ��� DCMI , ������ ����, ��������������� ������� ������ �������
   // ��� �� ���� SendSetALT (CalkSetALT(PointsPerPeriod, PointInPeriod=0)); // �������� ����� � �������, ������ ������� ������ PointInPeriod
  // ��������� ��������� ���������  ����� ��������
  // � �������� ��� ��������� (������������ ��������) ��� ���������
     //LED_KT(1); // ������ ������ ������
  StartRunFirst(AddIndexLN);// ����������� ������� ���������� ��������� ������ TIM1 - 
     //LED_KT(0); // ������ ������ ������

  if (CntAccumulat==0)CntAccumulat++;// ������ ��������� �� ����������
  //SetParamMeasure();
  while (CntAccumulat)
  {
    // ����� �������� ���� �� ������� ������ ����� ���������� ��� �� �������???
    // RUN_SUM ���� � ������� ����������\ ����������� ������������ - ����
    // SUMMER - ���� � ������������� �� ����������� ��������� ���, ����� �������������
    // ���� �������� ������������, �������� �������� ����������
    //( � ��� ����� ����� ����� ���� ���������� � �������������)
    // ������ ���������������� ������� - ���� �� ����!
    // ���� �������� SUMMER �� ��� ���� ������ ��������� ��� ������ �������
    // �� 
    // ����������� 

    RUN_SUM(RawData);//34-67-130-260-510-770-1540~ max 34*48=1632 uS 1632/1540=
    //CreatDelay (DelayAfterAvrg);
    //SendSetALT (CalkSetALT(PointsPerPeriod, PointInPeriod)); // (14 uS)�������� ����� � �������
  }
  MeasureNow =0; //��������� ����������� �� ���������� ������

  if (!RemoutCtrl)
  {
    // ������������ ������ ������� ����������
    EnaTimerAccum = 0;
  DrawPictureMeas (EnDrawGraph); // (28 mS) ��������� �������� ��� ���������
        EnaTimerAccum = 1;

  }
}

void DrawPictureMeas (BYTE EnDraw) // ��������� �������� ��� ���������
{ 
  char Str[32];
  Rect rct={0,0,128,56};
  GraphParams params = {GetVerticalSize(GetUserVerSize()),PosCursorMain (0),GetSetHorizontScale(0),0, GetUserTypeCentral ()};//PosCursorMain (0)
   //  LED_KT(1); // ������ �������
  
  if (EnDraw)
  {
    if(g_NeedScr)
    {
      CmdInitPage(17);// ������� ������� ������������ ���� �� DrawOTDR
      
      sprintf(Str,"t0.txt=\"%s\"���",MsgMass[30][CurrLang]); //���� ���������: XX� 
      NEX_Transmit((void*)Str);//
      // ����� ��������� ����� ����� � �������� ���������
      if ((GetIndexLN()==0)&&(IndexSmall==0))
        sprintf(Str,"t1.txt=\"0.5%s\"���",MsgMass[20][CurrLang]);//����� �����: XX��
      else
        sprintf(Str,"t1.txt=\"%d%s\"���",GetLengthLine(GetIndexLN()),MsgMass[20][CurrLang]);//����� �����: XX��
      //sprintf(Str,"t6.txt=\"%s\"���",MsgMass[35][CurrLang]); // 
      NEX_Transmit((void*)Str);//
      // ������ ������ ����� �����
      // ����� ��������� � ��������������� �� ����� �� 3 ���� ����,
      // � ������ ������������� ������ ����� ����� � ����� ������������ ������������
      // ���������� ���� ������ ����� �����
      if(g_AutoSave)
        sprintf(Str,"t3.bco=YELLOW���"); // ���������
      else
        sprintf(Str,"t3.bco=WHITE���"); // �����
      
      NEX_Transmit((void*)Str);//
      //if (GetSetModeLW(0)) 
      //sprintf(Str,"t3.txt=\"%d%s\"���",GetLengthWaveLS (GetPlaceLS(CURRENT)),MsgMass[21][CurrLang]); // auto
      //  sprintf(Str,"t3.txt=\"%s\"���",MsgMass[21][CurrLang]); // auto
      //else
      sprintf(Str,"t3.txt=\"%d%s\"���",GetLengthWaveLS (GetPlaceLS(CURRENT)),MsgMass[18][CurrLang]); // nm
      NEX_Transmit((void*)Str);//
      
      g_FirstScr = 0;
      g_NeedScr = 0;
    }
    
    //������� �������� �� UART2 ��� ���������� NEXTION
    // ����������(����������) ������ ��� ������ ����������
    if(GetIndexVRM()==4) // �������� �����
      sprintf(Str,"t2.txt=\"%s\"���",MsgMass[26][CurrLang]); //���� ���������:  ��������
    else
      
      sprintf(Str,"t2.txt=\"%d%s\"���",(WORD)(((float)GetCntNumAvrg()/(float)GetFinAvrg())*((float)GetTimeAvrg(GetIndexVRM()))),MsgMass[4][CurrLang]); //���� ���������: XX� 
    NEX_Transmit((void*)Str);// 
    if(TypeLCD)
    {
      rct.right=480;
      rct.bottom=255;// ����� ���� ������� 255, � � ���������� ������� 270/255 1.05
      params.scale=(params.scale>=16)?(8):(params.scale);
    }
    else
    {
      rct.right=400;
      rct.bottom = 210;
      params.scale=(params.scale>=16)?(10):(params.scale);
    }
    rct.left = 0;
    rct.top = 0;
    //rct={0,0,390,210};
    
    MakeGraphNext( &rct, LogData, POINTSIZE, &params );// ��� ������������� ������ ��� ������� (��� �����) � ����������� �� ����������
    
    
    SendDrawNex(NexData,2,rct.right);
    HAL_Delay (1);
    
  }
    //   LED_KT(0); // ����� �������

}

void RUN_SUM (DWORD* RawDataI)//
{
  //char Str[5];
  //static int CntAccumulat=0; // ������� ����������
  // DWORD* pRawData = RawData;
  // WORD* pDataADC = &DataADC;
  //--U0IER &= ~IER_RBR ;// | IER_THRE | IER_RLS/* Disable UART0 interrupt */
  //  TST_P1 (1); // set LED

  SUMMER(RawDataI); // 
  
  //SUMMER(RawDataI,   PointsPerPeriod-1-PointInPeriod); // ������� ������ ������ ������ ������� ��� ����� � �������������!!!!!!!!!!! 
  //---U0IER |= IER_RBR ;// | IER_THRE | IER_RLS/* Enable UART0 interrupt */
  //  TST_P1 (0); // off LED
  
  //  unsigned int i;
  //  unsigned int Step;
  //  //if (CntAccumulat==0)CntAccumulat++;// ������ ��������� �� ����������
  //  Step = PointsPerPeriod-1-PointInPeriod; // ������� ������ ������ ������ ������� ��� ����� � �������������!!!!!!!!!!! 
  //  ADC_DATA_EN(0); // ������������� ����� ��� ������ ���               !!!!!!!!!!!
  //  STARTPP(0); // ������������� � "0" START                            !!!!!!!!!!!  
  //   for (i=Step;i<RAWSIZE;i+=PointsPerPeriod)                                  // !!!!!!!!!!! 
  //    RawData[i] += DataADC;     // ������������ � ���� ������ ��������������� (���� ���� ��� ���) !!!!!!!!!!!
  //  STARTPP(1); // ������������� � "1" START                            !!!!!!!!!!!
  //  ADC_DATA_EN(1);// ������� �����                            !!!!!!!!!!!
  //  PointInPeriod ++;     // ��������� ������������ 
  //  PointInPeriod = 0;
    CntAccumulat ++;  // ��������� �������� ����������
    
//  if ((CurrTimeAccum - OldTimeAccum) > 2500) // ����� 2.5 ���
//  {
//  }
  //if (CntAccumulat > GetNumAccumPerSec())//��������� �������� ���������� � ������ ������ �� ����� ����������
  if ((CntAccumulat > GetNumAccumPerSec())||((CurrTimeAccum/1000)>GetTimeAvrg(GetIndexVRM())))//��������� �������� ���������� � ������ ������ �� ����� ����������
    
  { // ��������� ���� ���������� ����� �� ����� � ����� � �� �������!?
        StopAllTIM(1);

    OldTimeAccum = CurrTimeAccum;  
    //RawData[RAWSIZE-1]= GetTimer(2);
    DWORD Noise =0;
    DWORD NoiseAdd =0; // ��������� ���� �� ���������� ������ � ����� ����� ��� 64 � 128 ��
    int CntAddNoise= 0; // ������� ����� �������
    DWORD MaxNoise =0;
    //int j=GetCurrentBegShiftZone (); //��������� �������� �������� �� �������
    int j=0; //��������� �������� �������� �� �������
    DWORD LocalRaw;
    unsigned Avrgs = (GetCntNumAvrg()+CntAccumulat-1); // ������ ����� ����������, �� ������ ������
    SetCntNumAvrg(Avrgs); // ����������
    // filtr (WOW Super)
    //if (GetCntNumAvrg()>=GetFinAvrg())
    //processWindowMedian(RawData, RAWSIZE);
    //FiltrWOW(RawData, GetCntNumAvrg());
    //for (int i=0; i<j-1; ++i)
    // � ��� � ������ ������ 63 �����
    // ������ ������ �����
    for (int i=30; i<(55); ++i)
    {
      if (RawData[i]>MaxNoise) MaxNoise = RawData[i];
      Noise +=RawData[i];
    }
    Noise = (DWORD)1*(Noise/(25));
    if (GetIndexLN()>5)// ������� ����� -> ������� ����� �� ������� �����
    {
      for (int i=5390;i<5420;++i) // ����� 30 ����� � ����� ����������� ����
      {
        if (RawData[i] < (Noise + 100*Avrgs)) 
        {
          CntAddNoise++;
          NoiseAdd +=RawData[i];
        }
      }
      NoiseAdd += Noise;
      CntAddNoise++;
      Noise = (DWORD)1*(NoiseAdd/CntAddNoise);
    }
    //Noise = (DWORD)1*(Noise/(j-1));
    DWORD CurrentMaxLog =(DWORD)(5000.0*log10((double)Avrgs*1023)); // ������������ �������� ������� ����������
    // ������ ���������������� ���� (����� ���������)
    SetLogNoise((unsigned short)(CurrentMaxLog - (DWORD)(5000.0*log10((double)(MaxNoise-Noise))))) ;
    
    // 
    //Noise += Avrgs/400; // ������� ����� �������� � �����
    // ��������� ������� �����
    //SaveNoise (Noise/2);
    for (int i=0; i<OUTSIZE; i++)
    { 
      // ���������� �� �������� ������ ���� ������ 
      //if (PointsPerPeriod==48)   LocalRaw = (RawData[i+j-1]+RawData[i+j])>>1;// ���� ����� ������ �� ����������� ����� �� 2, ����� ����
      //else  LocalRaw = RawData[i+j];
      LocalRaw = RawData[i+j];
      // ������� ����� �������, �� ���� ����� ������� ���������� � ��������� ��������
      // ����������� 12 ������
      //LocalRaw += (DWORD)(RawData[i+j+(PointsPerPeriod/3)]*2.56e-5);
      
      if (LocalRaw<=Noise) LocalRaw=Noise+1;
      LocalRaw= LocalRaw-Noise;
      LogData[i] = (unsigned short)(CurrentMaxLog - (DWORD)(5000.0*log10((double)LocalRaw))) ;
        if (LogData[i] > CurrentMaxLog)
        { 
          //__no_operation();
          CurrentMaxLog+=1;
        }
      
    }
    CntAccumulat =  0;
    SetHeadFileRaw (GetCntNumAvrg()); // ����� ��������� ����� ������ ������������� ������
    
  }
}
// �������� ������ �������� ������
 void ContNextAvrg(void)
{
  // ������ ��������� � ����������, �� ������� �1, ���������� ����� ���� �����
  //StopAllTIM(0);// ������������� ������� ������� �������
    //TIM1->CR1 &=~TIM_CR1_CEN;
  // CountDMA ��������� �� �������� � ������� ���� ��������� ������� ����
  //Sm_Shift = (CountDMA)&(NumRepit-1);
  Cnt2Timer[CountDMA%128]=CurrCnt2Timer;
  //uint32_t PointDMA = (CountDMA&(NumRepit-1));
  uint32_t PointDMA = (CountDMA%(NumRepit));
  //LED_START(1);
  //LED_START(0);
  for(int i=0;i<SizeBlockNak; i++)
  {
    //BufNAK[NumRepit*i+PointDMA] +=BufADC[i]; 
    // for ADC MS9280 buffer BufADD
    //BufNAK[NumRepit*i+PointDMA] +=BufADD[i]; 
    BufNAK[NumRepit*i+PointDMA] +=BufADD[i]; 
   //BufNAK[NumRepit*i+PointDMA] +=(BufADC[i]+BufADD[i-1])/2; 
    // BufNAK[2*(NumRepit*i+PointDMA)+1] +=BufADC[i]; 
    //BufNAK[2*(NumRepit*i+PointDMA)] +=BufADD[i-1]; 
    //TIM1->CNT = TIM1->CCR1 - 5; // ��� ��� �� ����������
  }
  //LED_START(1);
    //TIM1->CR1 |=TIM_CR1_CEN;
  //LED_START(0);
  //LED_KT(1); // ��������� ���������� - ������������ ����� �������� �����
    StopAllTIM(0);// ������������� ������� ������� �������

  //if(++CountDMA<SumNumNak)
  //if((++CountDMA) >= SumNumNak)// ����� ����������� ����������
  if((++CountDMA) >= NumRepit)// ����� ����������� ����������
  {
    StopAllTIM(1);
    
    TIM1->CR1 &=~TIM_CR1_CEN;
    //  ������������� �������� ������ ��� ����������� ����������
    // ��������� ������� ��������� ��������� ��� ������ �����������
    // ����� ���������� ��� ��������� �������, � ���� ��������
    Ena_AVRG = 0;
    //EnaPrintRes = 1;
    
  }
  //��� �� ����������, ���� ������ ��� ���������� ����� ������
  // �� ������� �1 ������� ����������
  //        if(SW_TIM1>2)
  //        {
  //          TIM1->CNT = TIM1->CCR1 - 5;
  //          // ���������������� �������� ������ ����� � ��������� �����
  //          
  //        }
  //        // � ���������� ��� ��� ������� ���������� �����
  //        //TIM1->CNT =990;
  //        TIM1->CR1 |=TIM_CR1_CEN;
  
  //EnaNextAvrg = 0;
    Ena_SUMM = 1;
    Ena_CFG = 1;
  
}
