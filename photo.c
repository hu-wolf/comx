#include"Picture.h"
//#define white 0xff
//#define black 0x00
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <pshpack2.h>
#include <poppack.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

#pragma pack(push)
#pragma pack(1)

/*����λͼ�ļ�ͷ*/
typedef struct
{
	WORD bfType;//�ļ�����,����Ϊ"BM",ASCIIֵΪ0x4d42
	DWORD bfSize;//�ļ��Ĵ�С
	WORD bfReserved1;//Ϊ0
	WORD bfReserved2;//Ϊ0
	DWORD bfOffBits;//�洢����������������ļ�ͷ��ƫ����(�ļ���ͷ����ɫ������)
} BIT_MAP_FILE_HEADER;

/*������Ϣͷ*/
typedef struct
{
	DWORD biSize;//��Ϣͷ��С
	DWORD biWidth;//λͼ���(����)
	DWORD biHeight;//λͼ�߶�(����)
	WORD biPlanes;//����Ϊ1
	WORD biBitCount;//ÿ�����ص�λ��,����������λ��24Ϊÿһɫ8λ(1,4,8,16,24,32)
	DWORD biCompression;//ѹ����ʽ(0��BI_RGB(δѹ��))
	DWORD biSizeImage;//ͼ���С(�ֽ�,��������Ϣͷ),������ѹ��λͼ
	DWORD biXPelsPerMeter;//��Ŀ���豸ÿ�׵���������˵��λͼ��ˮƽ�ֱ���
	DWORD biYPelsPerMeter; //��Ŀ���豸ÿ�׵���������˵��λͼ�Ĵ�ֱ�ֱ���
	DWORD biClrUsed;//��ɫ�����ɫ��,��Ϊ0��λͼʹ����biBitCountָ���������ɫ��
	DWORD biClrImportant;//��Ҫ��ɫ����Ŀ������ֵΪ0��������ɫ����Ҫ
} BIT_MAP_INFO_HEADER;

/*��ɫ��*/
typedef struct
{
	BYTE blue;
	BYTE green;
	BYTE red;
} RGB_QUAD;

#pragma pack(pop)
RGB_QUAD *bmp_data = NULL;
RGB_QUAD white = { 255, 255, 255 };
RGB_QUAD black = { 0, 0, 0 };
RGB_QUAD red = { 0, 0, 255 };

static void printWord(int s, int t,const unsigned char *word)
{
	int count;
	int x, y;
	for (y = t, count = 0; y > t - 16; y--, count++)
	{
		for (x = s; x < s + 8; x++)
		{
			if (word[count] & (1 << (s + 7 - x)))
			{
				bmp_data[y * 320 + x] = white;
			}
			else
			{
				bmp_data[y * 320 + x] = black;
			}
		}
		for (count++; x < s + 16; x++)
		{
			if (word[count] & (1 << (s + 15 - x)))
			{
				bmp_data[y * 320 + x] = white;
			}
			else
			{
				bmp_data[y * 320 + x] = black;
			}
		}
	}
}

static void printAllWord(int s, int t, int num, const unsigned char *word)
{
	int x, y;
	int count;
	for (x = s, y = t, count = 0; x<s + num * 16; x += 16, count += 32)
	{
		printWord(x, y, &word[count]);
	}
}

void CreatPIDPicture(int Data[],int DataNum,int Base,int max)
{
	FILE *bmp_file;
	int i, j;
	long width = PIDW;  //����ͼƬ���
	long height = PIDH; //����ͼƬ�߶�
	long date_size = width*height;
	BIT_MAP_FILE_HEADER bmp_map_file;
	BIT_MAP_INFO_HEADER bmp_map_info;

	if ((bmp_file = fopen("cubs.bmp", "wb+")) == NULL)
	{
		printf("����ͼƬʧ��!\n");
		system("pause");
		exit(1);
	}

	/*д��ͼλ�ļ�ͷ*/
	bmp_map_file.bfType = 0x4d42;
	bmp_map_file.bfSize =\
		sizeof(BIT_MAP_FILE_HEADER) + sizeof(BIT_MAP_INFO_HEADER) + width * height * sizeof(RGB_QUAD);
	bmp_map_file.bfReserved1 = 0;
	bmp_map_file.bfReserved2 = 0;
	bmp_map_file.bfOffBits = sizeof(BIT_MAP_FILE_HEADER) + sizeof(BIT_MAP_INFO_HEADER);
	fwrite(&bmp_map_file, sizeof(BIT_MAP_FILE_HEADER), 1, bmp_file);

	/*д����Ϣͷ*/
	bmp_map_info.biSize = sizeof(BIT_MAP_INFO_HEADER);
	bmp_map_info.biPlanes = 1;
	bmp_map_info.biHeight = height;
	bmp_map_info.biWidth = width;
	bmp_map_info.biBitCount = 24;
	bmp_map_info.biClrImportant = 0;
	bmp_map_info.biClrUsed = 0;
	bmp_map_info.biSizeImage = width * height * sizeof(RGB_QUAD);
	bmp_map_info.biCompression = 0;
	bmp_map_info.biXPelsPerMeter = 0;
	bmp_map_info.biYPelsPerMeter = 0;
	fwrite(&bmp_map_info, sizeof(BIT_MAP_INFO_HEADER), 1, bmp_file);

	/*��ͼ������ڴ�*/
	if ((bmp_data = (RGB_QUAD*)malloc(width * height * sizeof(RGB_QUAD))) == NULL)
	{
		printf("ͼƬ�����ڴ�ʧ�ܣ�\n");
		exit(0);
	}

	/*��ʼ��ͼ��*/
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			bmp_data[j * width + i] = black;
		}
	}

	/*��������*/
	double Wstep = width / (double)DataNum;
	double Hstep = height / (double)max;

	/*��������ֵ*/
	Base = (int)(Base*Hstep);
	for (i = 0; i < width; i++)
	{
		bmp_data[Base*width + i] = red;
		bmp_data[(Base + 1)*width + i] = red;
		bmp_data[(Base - 1)*width + i] = red;
	}

	/*���*/
	int count;
	int temp;
	for (i = 0, count = 0; i < width&&count <= DataNum; count++)
	{
		temp = (int)(Data[count] * Hstep);
		bmp_data[temp * width + i] = white;
		bmp_data[(temp + 1) * width + i] = white;
		bmp_data[(temp + 1) * width + i + 1] = white;
		bmp_data[temp * width + i + 1] = white;
		bmp_data[(temp + 2) * width + i] = white;
		bmp_data[(temp + 2) * width + i + 1] = white;
		bmp_data[(temp + 2) * width + i + 2] = white;
		bmp_data[(temp + 1) * width + i + 2] = white;
		bmp_data[temp * width + i + 2] = white;
		i = (int)(i + Wstep);
	}

	fwrite(bmp_data, sizeof(RGB_QUAD), date_size, bmp_file);
	printf("BMPͼ�����ɳɹ�\n");
	fclose(bmp_file);
	free(bmp_data);
	system("start cubs.bmp");
}