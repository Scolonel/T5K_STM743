void GetHeaderBelcore (char* Name, unsigned short Block, unsigned short NumEvents) // ��������� ����� �������
{
  char Str[10];
  
  unsigned short DataShort ; // 
  unsigned long DataInt ;
  
      // ��� ������ ����������������� ��������� ������� (������� ������������� ����� ����� �� ������� ��������)
      switch (Block)
      {
      case 0: //0...55 byte
//        // ������ ���������� ���� ��������� ����
//      if (GetNumTrace()) // ���� �� ������� �� ������ �� �������
//        TraceREAD(GetNumTraceSaved(GetNumTrace()));//  ������ ���� ������� ���� ��������// 27/01/2011 ����������� ��������� ��������������
//      else  TraceREAD(0);
      memcpy( Name, HeaderBelcore, 56 );
        if (NumEvents)
        {
          //memcpy( &Name[5], 7, 1);
          Name[2]=0x62; // ������ ������ ������� Map ����� +16
          Name[6]=7; // ������ ������ ���������� ����� � ��� �����
          memcpy( &Name[56], &HeaderBelcore[225], 16 ); // �������� ���� ������� � ��� ����
          DataShort = (unsigned short)(NumEvents*32+24); // ��������� ������� ����� � ����������� �� ����� �������
          memcpy( &Name[68], &DataShort, 2);
          
        }
        break;
      case 1: //56...117 (72...133) 62 byte
      memcpy( Name, &HeaderBelcore[56], 62 );
        // ������������� ����� �����
      DataShort = (unsigned short)GetLengthWaveLS (GetPlaceLS(CURRENT)); // ��������� ����� ����� �� ������� ������������� �����
      // ���������� ������ ����� 1300 ��� �4 ��������!
      
      memcpy( &Name[86-56], &DataShort, 2);
      // ��������� Comments
      memcpy( &Name[98-56], CommentsOTDR, 19);
        break;
      case 2: //118...211 (134...227) 94 byte
      memcpy( Name, &HeaderBelcore[118], 94 );
      // ��������� �������������
      memcpy( &Name[118-118], IdnsBC[ConfigDevice.ID_Device], 14 );
      GetMFID( Str ); // for Belcore
      // Set MDIF
      memcpy( &Name[133-118], Str, 12 );
      // Number Device
      sprintf (Str,"%04d",ConfigDevice.NumDevice);
      memcpy( &Name[146-118], Str, 4 );
      // ����� ���������
      GetNumVer(Str);
      memcpy( &Name[153-118], Str, 4 );
      
      // ����� ���������� �������������� 
      // ������������� ���� ����������� ��������������
      DataInt = TotalSec (TimeSaveOTDR);
      memcpy( &Name[159-118], &DataInt, 4);
      // (164)AW ����� ����� � 0.1 �� (1310�� = 13100) 0x332c 0x3c8c (164)
      DataShort = (unsigned short)GetLengthWaveLS (GetPlaceLS(CURRENT))*10;
      memcpy( &Name[165-118], &DataShort, 2);
      // ����� ������������ �������� 
      DataShort = GetWidthPulse(GetIndexIM());
      memcpy( &Name[173-118], &DataShort, 2);
      // ������������� DS
      // ((10000points*10(in 1ns 100ps))/2 = 50000 , 333.333 ns - �������� ����� ����������
      DataInt = (unsigned long)((ADCPeriod*50000)/NumPointsPeriod[GetIndexLN()]); //  ������������� �������� DS ��� �������������� ������ ���������
      memcpy( &Name[175-118], &DataInt, 4);
      // ###(182) GI ����� �����������  146583 (1.46583)  
      DataInt = (unsigned long)( GetIndexWAV()*100000);
      memcpy( &Name[183-118], &DataInt, 4);
      // ###(186) BC = 800 ������ ��������� ���������
      DataInt =  ReflParam.BC;
      memcpy( &Name[187-118], &DataInt, 2);
      // ###(188) NAV ����� ���������� - ������� �� ������� ���������� � ����� ����� = FinAvrg
      DataInt = SettingRefl.NumAvrag;
      memcpy( &Name[189-118], &DataInt, 4);
      // ###(192) AR  ����� ����������� ������� (����� ����� ��������� �� ���) DS*NPPW/10000
      DataInt = (unsigned long)((ADCPeriod*5*4096)/NumPointsPeriod[GetIndexLN()]); //  ������������� �������� DS ��� �������������� ������ ���������
      memcpy( &Name[193-118], &DataInt, 4);
      // ###(200) NF ������ ������� ����� ����� 65535
      DataInt =  ReflParam.NF;
      memcpy( &Name[201-118], &DataInt, 2);
      // ###(206) LT ����������� �������� ��������� ��� ������� 200 � 0.001 ��
      DataInt =  ReflParam.LT;
      memcpy( &Name[207-118], &DataInt, 2);
      // ###(208) RT ����������� �������� ��������� ��� ������� 40000 � -0.001��
      DataInt =  ReflParam.RT;
      memcpy( &Name[209-118], &DataInt, 2);
      // ###(210) ET ����������� ��������� ��� ����� ����� 3000 � 0.001 ��
      DataInt =  ReflParam.ET;
      memcpy( &Name[211-118], &DataInt, 2);
        break;
      case 3: //212...223 (228...239) 12 byte
      memcpy( Name, &HeaderBelcore[213], 12 );
       break; 
      }

}