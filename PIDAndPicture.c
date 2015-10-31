#include"Picture.h"
#include"comx.h"

void ExPic(void)
{
	printf("Expic not open\n");
}

void PIDtest(void)
{
	system("cls");
	printf("�������������ļ���\n");
	char FileName[MAX_PATH];
	scanf("%s", FileName);
	FileName[MAX_PATH - 1] = '\0';

	FILE *ReFile;
	ReFile = fopen(FileName, "r");
	if (ReFile == NULL)
	{
		printf("�ļ���ʧ��!\n");
		system("pause");
		return;
	}

	StepResponseData SRD;
	SRD.td = SRD.tr = SRD.tp = SRD.ts = 0;
	SRD.Overshoot = 0;

	printf("�����뱾�β��Ե�����ֵ\n");
	scanf("%d", &SRD.MaxOutput);
	printf("�����뱾�β��Ե�����ֵ\n");
	scanf("%d", &SRD.base);
	printf("�����뱾�β��ԵĲ�������(ms)\n");
	scanf("%d", &SRD.T);

	printf("������...\n");
	int ch;
	for (SRD.DataNum = 0, SRD.MaxData = 0; SRD.DataNum < PIDW / 2; SRD.DataNum++)
	{
		/*���ݶ�ȡ*/
		fscanf(ReFile, "%d", &SRD.Data[SRD.DataNum]);

		/*���㳬����,��ֵʱ��*/
		if (SRD.Data[SRD.DataNum]>SRD.MaxData)
		{
			SRD.MaxData = SRD.Data[SRD.DataNum];
			if (SRD.MaxData > SRD.base)
			{
				SRD.tp = (SRD.DataNum - 1) * SRD.T;
				SRD.Overshoot = (SRD.MaxData - SRD.base) / (double)SRD.base;
			}
		}

		/*�������ʱ��*/
		if (SRD.ts == 0 && abs(SRD.Data[SRD.DataNum] - SRD.base) / (double)SRD.base <= 0.02)
		{
			SRD.ts = (SRD.DataNum - 1) * SRD.T;
		}
		if (SRD.ts != 0 && abs(SRD.Data[SRD.DataNum] - SRD.base) / (double)SRD.base > 0.02)
		{
			SRD.ts = 0;
		}

		/*�����ӳ�ʱ��*/
		if (SRD.td == 0 && SRD.Data[SRD.DataNum] >= SRD.base / 2)
		{
			SRD.td = (SRD.DataNum - 1) * SRD.T;
		}

		/*��������ʱ��*/
		if (SRD.tr == 0 && SRD.Data[SRD.DataNum] >= SRD.base)
		{
			SRD.tr = (SRD.DataNum - 1) * SRD.T;
		}

		/*��ȡ����ֵ��(����ͨ��Э�����)���ж�EOF*/
		ch = getc(ReFile);
		ch = getc(ReFile);
		if (ch == EOF)
			break;
		else
			ungetc(ch, ReFile);
	}

	printf("�������!\n");
	printf("td=%dms\ntr=%dms\ntp=%dms\nts=%dms\n��=%g\n", SRD.td, SRD.tr, SRD.tp, SRD.ts, SRD.Overshoot);
	printf("���������ʼ�������β��ԵĽ�Ծ��Ӧͼ��\n");
	_getch();

	printf("��������ͼƬ...\n");
	CreatPIDPicture(SRD.Data, SRD.DataNum, SRD.base, SRD.MaxOutput);
}