#include"comx.h"

void HyperTerminal(void)
{
	HANDLE Hcom;
	Hcom = HyperTerminalInit();
	CheckComHandle(Hcom);

	HANDLE Wait[2];
	Wait[0] = (HANDLE)_beginthreadex(NULL, 0, WriteInfo, Hcom, 0, NULL);
	Wait[1] = (HANDLE)_beginthreadex(NULL, 0, ReadInfo, Hcom, 0, NULL);
	WaitForMultipleObjects(2, Wait, TRUE, INFINITE);
	CloseHandle(Hcom);
	CloseHandle(Wait[0]);
	CloseHandle(Wait[1]);
	fExit = 0;
}

void ReceivePicture(void)
{
	HANDLE Hcom;
	Hcom = ReceivePictureInit();
	CheckComHandle(Hcom);
	/*׼���洢�ļ�*/
	FILE *pSave;
	char FileName[MAX_PATH];
	printf("������洢�ļ��������ļ���,��enterȷ��\n");
	scanf("%s", FileName);
	FileName[MAX_PATH - 1] = '\0';
	pSave = fopen(FileName, "w");
	if (pSave == NULL)
	{
		printf("�洢�ļ�����ʧ��\n");
		system("pause");
		exit(EXIT_FAILURE);
	}
	/*��������*/
	printf("����׼������,�뿪����λ����ʼ�������ݴ���\n��Ҫ���������밴ESC\n");
	_beginthreadex(NULL, 0, ExitMonitor, NULL, 0, NULL);

	/*����������ʼ��,����λ������λ��ͳһ*/
	const char *DataStart = "tsD";
	/*����������ֹ��,����λ������λ��ͳһ*/
	const char *DataEnd = "deD";
	/*�ν�����,����λ������λ��ͳһ*/
	const char SectionEnd = '#';
	char SEbuf[4];
	DWORD ReadNum;
	memset(SEbuf, 0, sizeof(char)* 4);
	while (1)
	{
		if (ReadFile(Hcom, &SEbuf[0], 1, &ReadNum, NULL) == FALSE)
		{
			printf("���ڿ����ѱ��γ�\n����߳���ֹ\n");
			system("pause");
		}
		if (ReadNum != 0)
		{
			if (STRCMP(SEbuf, == , DataStart))
				break;
			SEbuf[2] = SEbuf[1];
			SEbuf[1] = SEbuf[0];
		}
	}
	printf("��⵽������ʼ��,��ʼ�����������ļ�\n");
	memset(SEbuf, 0, sizeof(char)* 3);
	while (1)
	{
		if (ReadFile(Hcom, &SEbuf[0], 1, &ReadNum, NULL) == FALSE)
		{
			printf("���ڿ����ѱ��γ�\n����߳���ֹ\n");
			system("pause");
		}
		if (ReadNum != 0)
		{
			fprintf(pSave, "%c", SEbuf[0]);
			if (SEbuf[0] == SectionEnd&&fExit == 1)
			{
				printf("ͬʱ��⵽����ֹ�����˳�\n");
				break;
			}
			if (STRCMP(SEbuf, == , DataEnd))
			{
				printf("��⵽������ֹ��\n");
				break;
			}
			SEbuf[2] = SEbuf[1];
			SEbuf[1] = SEbuf[0];
		}
	}

	CloseHandle(Hcom);
	fclose(pSave);
	fExit = 0;
}

void SetAT(void)
{
	printf("���ȽӺ���·,ȷ����������\n");
	system("pause");
	system("cls");
	printf("��ѡ��оƬ�ͺ�\n1.HC-05\t2.HC-06\n");
	switch (_getch())
	{
		default:printf("�������!\n"); system("pause"); return;
		CASE '1':Set05AT();
		CASE '2':Set06AT();
	}
	fExit=0;
}

void ExportPicture(void)
{
	printf("��ѡ����������\n1.ͼƬ\n2.PID����\n");
	switch (_getch())
	{
		default:printf("�������!\n"); system("pause"); return;
		CASE '1':ExPic();
		CASE '2':PIDtest();
	}
}