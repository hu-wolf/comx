#include"comx.h"

BOOL OpenCom(HANDLE *Hcom, char *ComName,DWORD DesAcc,DWORD Flags)
{
	*Hcom = CreateFileA(
		ComName,//�����߼���
		DesAcc,//��������,GENERIC_READΪ�ɶ�,GENERIC_WRITEΪ��д
		0,//�Ƿ���Թ���,��Ϊ���ڲ��ɹ���,����Ϊ0
		NULL,//���ð�ȫ���Խṹ,ȱʡΪNULL
		OPEN_EXISTING,//������־,�Դ��ڱ���ΪOPEN_EXISTING(�򿪶����Ǵ���)
		Flags,//�Ƿ�����첽����,0Ϊͬ��,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPEDΪ�첽
		NULL//ѡ��ģ���ļ�,�Դ��ڱ���ΪNULL
		);
	if (*Hcom == (HANDLE)-1)
	{
		printf("�򿪴���ʧ��!\n�ô��ڲ����ڻ�������ڱ�ռ��\n");
		return FALSE;
	}
	return TRUE;
}

BOOL SetBufAndTimeOut(HANDLE Hcom)
{
	/*���ô���*/
	//����I/O������(����ͨ�����ʵ���)
	if (SetupComm(Hcom,//�豸���
		1024,//���뻺������С(�ֽ�)
		1024 //�����������С(�ֽ�)
		) == FALSE)
	{
		printf("I/O����������ʧ��\n");
		return FALSE;
	}
	//�������ó�ʱ�Ľṹ��  ע:��ʱ���������������õĳ�ʱʱ����δ��ɶ�/д����,��/д�����Ի����
	COMMTIMEOUTS TimeOuts;
	/*
	1.��/д�ܳ�ʱ=ϵ��*�ַ���+����
	2.ϵ���볣����Ϊ0���ʾ��ʹ�øó�ʱ
	3.���������ʱΪMAXDWORD�Ҷ�ϵ���ͳ�����Ϊ0,��ô�����ͻ���������,�����Ƿ����Ҫ����ַ�
	4.�첽ͨ��ʱ,��/д���ܻ�����ɲ���ǰ����,��ʱ��ʱ�涨���ǲ��������ʱ��
	5.ʱ�䵥λ��Ϊms
	*/
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = MAXDWORD;//�������ʱ
	TimeOuts.ReadTotalTimeoutMultiplier = 1;//��ʱ��ϵ��
	TimeOuts.ReadTotalTimeoutConstant = 10;//��ʱ�䳣��
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 50;//дʱ��ϵ��
	TimeOuts.WriteTotalTimeoutConstant = 1000;//дʱ�䳣��
	//д�볬ʱ����,��ȡ��ǰ��ʱ����ʹ��GetCommTimeouts()
	if (SetCommTimeouts(Hcom, &TimeOuts) == FALSE)
	{
		printf("д�볬ʱ����ʧ��\n");
		return FALSE;
	}
	return TRUE;
}

BOOL SetDcb(HANDLE Hcom, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits)
{
	//��������
	DCB dcb;
	//��ȡ��ǰDCB��Ϣ
	if (GetCommState(Hcom, &dcb) == FALSE)
	{
		printf("��ȡ���ڳ�ʼDCB��Ϣʧ��\n");
		return FALSE;
	}
	dcb.BaudRate = BaudRate;//���ò�����,��ʽΪCBR_xxx
	dcb.ByteSize = ByteSize;//�ֽڳ���(4-8)
	dcb.Parity = Parity;//��żУ�鷽��,ODDPARITY ��У�� EVENPARITY żУ�� MARKPARITY ���У�� NOPARITY ��У��
	dcb.StopBits = StopBits;//ֹͣλλ��,ONESTOPBIT 1λֹͣλ TWOSTOPBITS 2λֹͣλ ONE5STOPBITS 1.5λֹͣλ
	if (Parity == NOPARITY)
		dcb.fParity = 0;
	else
		dcb.fParity = 1;
	//д��DCB��Ϣ
	if (SetCommState(Hcom, &dcb) == FALSE)
	{
		printf("д��DCB��Ϣʧ��\n");
		return FALSE;
	}
	return TRUE;
}

static char* GetComName(void)
{
	static char ComName[7] = { 'C', 'O', 'M' };
	int ComNum;
	printf("��������Ҫ�򿪵Ĵ��ں�(enter��������)\n");
	scanf("%d", &ComNum);
	sprintf(&ComName[3], "%d", ComNum);
	return ComName;
}

HANDLE HyperTerminalInit(void)
{
	HANDLE Hcom = NULL;
	/*�򿪴���*/
	char *ComName = NULL;
	char InfoBuf[30];
	ComName = GetComName();
	if (OpenCom(&Hcom, ComName, GENERIC_READ | GENERIC_WRITE, 0) == FALSE)
	{
		return NULL;
	}
	/*���û������볬ʱ*/
	if (SetBufAndTimeOut(Hcom) == FALSE)
	{
		return NULL;
	}
	/*����DCB�ṹ��*/
	DWORD BaudRate;
	BYTE ByteSize;
	BYTE Parity;
	BYTE StopBits;
	printf("�밴�ĵ���ʽ���벨����,У�鷽ʽ,����λ,ֹͣλ\n");
	scanf("%ld-%c-%c-%c", &BaudRate, &Parity, &ByteSize, &StopBits);
	sprintf(InfoBuf, ComName);
	sprintf(&InfoBuf[strlen(ComName)], ",����:%ld-%c-%c-%c\n", BaudRate, Parity, ByteSize, StopBits);
	switch (StopBits - 48)
	{
		default:StopBits = ONE5STOPBITS;
		CASE 1:StopBits = ONESTOPBIT;
		CASE 2:StopBits = TWOSTOPBITS;
	}
	switch (Parity)
	{
		default:Parity = NOPARITY;
		CASE 'N':Parity = NOPARITY;
		CASE 'E':Parity = EVENPARITY;
		CASE 'O':Parity = ODDPARITY;
	}
	ByteSize -= 48;
	if (SetDcb(Hcom, BaudRate, ByteSize, Parity, StopBits) == FALSE)
	{
		return NULL;
	}
	//��������
	/*
	��������������(��һ��ʹ��):
	PURGE_TXABORT         �ж�����д�������������أ���ʹд������û����ɡ�
	PURGE_RXABORT         �ж����ж��������������أ���ʹ��������û����ɡ�
	PURGE_TXCLEAR         ������������
	PURGE_RXCLEAR         ������뻺����
	*/
	if (PurgeComm(Hcom, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE)
	{
		printf("��ʼ��������ʧ��\n");
		return NULL;
	}
	system("cls");
	printf(InfoBuf);
	return Hcom;
}

HANDLE ReceivePictureInit(void)
{
	HANDLE Hcom = HyperTerminalInit();
	return Hcom;
}

HANDLE Set05ATInit(void)
{
	HANDLE Hcom = NULL;
	char *ComName = NULL;
	ComName = GetComName();
	if (OpenCom(&Hcom, ComName, GENERIC_READ | GENERIC_WRITE, 0) == FALSE)
	{
		return NULL;
	}
	if (SetBufAndTimeOut(Hcom) == FALSE)
	{
		return NULL;
	}
	if (SetDcb(Hcom, 38400, 8, NOPARITY, ONESTOPBIT) == FALSE)
	{
		return NULL;
	}
	if (PurgeComm(Hcom, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE)
	{
		printf("��ʼ��������ʧ��\n");
		return NULL;
	}
	system("cls");
	printf("��ʼ�����\n���ں�:%s,����:38400-N-8-1\n", ComName);
	return Hcom;
}

const DWORD BAUD[] = {
	0, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
};

static BOOL TryBAUD(HANDLE Hcom)
{
	int count;
	char *SendBuf = "AT";
	char ReBuf[3];
	DWORD WriteNum;
	DWORD ReadNum;
	ReBuf[2] = '\0';
	for (count = 1; count <= 8; count++)
	{
		if (SetDcb(Hcom, BAUD[count], 8, NOPARITY, ONESTOPBIT) == FALSE)
		{
			return FALSE;
		}
		if (WriteFile(Hcom, SendBuf, 2, &WriteNum, NULL) == FALSE)
		{
			printf("��Ϣ����ʧ��,���ڿ����ѱ��γ�\n");
			return FALSE;
		}
		Sleep(800);
		if (ReadFile(Hcom, ReBuf, 2, &ReadNum, NULL) == FALSE)
		{
			printf("��Ϣ��ȡʧ��,���ڿ����ѱ��γ�\n");
			return FALSE;
		}
		if (ReadNum != 0)
		{
			if (STRCMP(ReBuf, == , "OK"))
			{
				system("cls");
				printf("��⵽��ʼ������Ϊ%ld\n", BAUD[count]);
				return TRUE;
			}
		}
	}
	printf("�����ʸ���115200,�޷�����!\n");
	return FALSE;
}

HANDLE Set06ATInit(void)
{
	HANDLE Hcom = NULL;
	char *ComName = NULL;
	ComName = GetComName();
	if (OpenCom(&Hcom, ComName, GENERIC_READ | GENERIC_WRITE, 0) == FALSE)
	{
		return NULL;
	}
	if (SetBufAndTimeOut(Hcom) == FALSE)
	{
		return NULL;
	}
	if (PurgeComm(Hcom, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE)
	{
		printf("��ʼ��������ʧ��\n");
		return NULL;
	}
	system("cls");
	printf("���ڲ��Գ�ʼ������...\n");
	if (TryBAUD(Hcom) == FALSE)
	{
		return NULL;
	}
	return Hcom;
}