#include"App.h"
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

typedef void(*V_FP_V)(void);

void show(void)
{
	printf("��ӭʹ�ö๦�ܴ��ڵ�������(By ����ˮ��)\n"\
		"����ʹ��ǰ�Ķ�˵���ĵ�\n"\
		"1.�����ն�\n"\
		"2.���մ�������\n"\
		"3.����ATģʽ����\n"\
		"4.�������ݵ���ͼƬ\n"\
		"Q.�˳�����\n"
		);
}

int main(void)
{
	static V_FP_V const arpf[] =
	{ HyperTerminal, ReceivePicture, SetAT, ExportPicture };
	int option = 0;
	show();
	while (1)
	{
		option = _getch();
		if (option > '4' || option < '1')
		{
			if (option == 'Q' || option == 'q')
				break;
			printf("���ݴ���,����������!\n");
			continue;
		}
		option -= 49;
		system("cls");
		arpf[option]();
		system("pause");
		system("cls");
		show();
	}
	return EXIT_SUCCESS;
}