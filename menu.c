#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

#include"common.h"
#include"App.h"
#include"comx.h"

typedef void(*V_FP_V)(void);

void show(int Choose)
{
	switch (Choose)
	{
	default:printf("fatal error!!!\n");
		CASE MainMenu :
		printf("��ӭʹ�ö๦�ܴ��ڵ�������(By ����ˮ��)\n"\
			"����ʹ��ǰ�Ķ�˵���ĵ�\n"\
			"1.�����ն�\n"\
			"2.���մ�������\n"\
			"3.����ATģʽ����\n"\
			"4.�������ݵ���ͼƬ\n"\
			"Q.�˳�����\n"
			);
		CASE SetATMenu :
		printf("���ȽӺ���·,ȷ����������\n");
		system("pause");
		system("cls");
		printf("��ѡ��оƬ�ͺ�\n1.HC-05\t2.HC-06\nQ.�˳���ǰ����\n");
		CASE ExportPictureMenu :
		printf("��ѡ����������\n1.ͼƬ\n2.PID����\nQ.�˳���ǰ����\n");
	}
}

void Menu(int Choose, int Num)
{
	static V_FP_V const arpf[][9] =
	{
		{ HyperTerminal, ReceivePicture, SetAT, ExportPicture },
		{ Set05AT, Set06AT },
		{ ExPic, PIDtest }
	};
	int option = 0;
	show(Choose);
	while (1)
	{
		option = _getch();
		if (option > Num + 48 || option < '1')
		{
			if (option == 'Q' || option == 'q')
			{
				break;
			}
			else
			{
				printf("���ݴ���,����������!\n");
				continue;
			}
		}
		option -= 49;
		system("cls");
		arpf[Choose][option]();
		system("pause");
		system("cls");
		show(Choose);
	}
}