#include "BmpDefine.h"
#include<math.h>
#include<memory.h>
#include<string.h>

//////////////////////////////////////////////////
/////////////////////��������/////////////////////
//////////////////////////////////////////////////
BmpImage ReadBMP(char *s)
{
	FILE *fp;
	BmpImage Im = (BmpImage)malloc(sizeof(struct tagBMPIMAGE));
	Im->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im->InfoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));
	if ((fp = fopen(s, "rb")) == NULL)		//��HelloWorld.bmp��ʧ�ܷ��ش�����Ϣ
	{
		printf("Opening occurs Error!\n");
	}
	fseek(fp, 0, SEEK_SET);									//����fread�����ļ�ͷ����Ϣͷ
	fread(Im->FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(Im->InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if (Im->InfoHeader->biBitCount < 24)
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		Im->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*num);
		for (int i = 0; i < num; i++)
		{
			fread(&(Im->Pallete[i].rgbBlue), sizeof(BYTE), 1, fp);
			fread(&(Im->Pallete[i].rgbGreen), sizeof(BYTE), 1, fp);
			fread(&(Im->Pallete[i].rgbRed), sizeof(BYTE), 1, fp);
			fread(&(Im->Pallete[i].rgbReversed), sizeof(BYTE), 1, fp);
		}
	}
	else Im->Pallete = NULL;

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���
	BYTE *pColorData = (BYTE *)malloc(height*l_width);
	long nData = height * l_width;
	fread(pColorData, 1, nData, fp);

	Im->RgbData = (RGBQUAD)malloc(width*height * sizeof(struct tagRGBQUAD));
	if (Im->InfoHeader->biBitCount < 24)//�е�ɫ�壬��λͼΪ�����ɫ
	{
		int k;
		int index = 0;
		if (Im->InfoHeader->biBitCount == 1)
		{
			int k;
			int index = 0;
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 8;//k:ȡ�ø�������ɫ������ʵ�����������е����
					mixIndex = pColorData[k];//mixIndex:��ȡ��ǰ���ص���ɫ�ľ���ֵ
					if (k == 2755)
					{
						printf("Hello!");
					}
					switch (j % 8)
					{
					case 7:
						mixIndex = mixIndex << 7;
						mixIndex = mixIndex >> 7;
						break;
					case 6:
						mixIndex = mixIndex << 6;
						mixIndex = mixIndex >> 7;
						break;
					case 5:
						mixIndex = mixIndex << 5;
						mixIndex = mixIndex >> 7;
						break;

					case 4:
						mixIndex = mixIndex << 4;
						mixIndex = mixIndex >> 7;
						break;
					case 3:
						mixIndex = mixIndex << 3;
						mixIndex = mixIndex >> 7;
						break;

					case 2:
						mixIndex = mixIndex << 2;
						mixIndex = mixIndex >> 7;
						break;
					case 1:
						mixIndex = mixIndex << 1;
						mixIndex = mixIndex >> 7;
						break;

					case 0:
						mixIndex = mixIndex >> 7;
						break;
					}
					//���������ݱ��浽�����ж�Ӧ��λ��
					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;

				}
		}
		if (Im->InfoHeader->biBitCount == 2)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 4;//k:ȡ�ø�������ɫ������ʵ�����������е����
					//j:��ȡ��ǰ���ص���ɫ�ľ���ֵ
					mixIndex = pColorData[k];
					switch (j % 4)
					{
					case 0:
						mixIndex = mixIndex << 6;
						mixIndex = mixIndex >> 6;
						break;
					case 1:
						mixIndex = mixIndex << 4;
						mixIndex = mixIndex >> 6;
						break;
					case 2:
						mixIndex = mixIndex << 2;
						mixIndex = mixIndex >> 6;
						break;
					case 3:
						mixIndex = mixIndex >> 6;
						break;
					}

					//���������ݱ��浽�����ж�Ӧ��λ��
					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;


				}
		}
		if (Im->InfoHeader->biBitCount == 4)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 2;
					mixIndex = pColorData[k];
					if (j % 2 == 0)
					{//��
						mixIndex = mixIndex << 4;
						mixIndex = mixIndex >> 4;
					}
					else
					{//��
						mixIndex = mixIndex >> 4;
					}

					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;

				}

		}
		if (Im->InfoHeader->biBitCount == 8)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j;
					mixIndex = pColorData[k];
					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;
				}
		}
		if (Im->InfoHeader->biBitCount == 16)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					WORD mixIndex = 0;

					k = i * l_width + j * 2;
					WORD shortTemp;
					shortTemp = pColorData[k + 1];
					shortTemp = shortTemp << 8;

					mixIndex = pColorData[k] + shortTemp;

					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;
				}
		}
	}
	else//λͼΪ24λ���ɫ
	 {
		 int k;
		 int index = 0;
		 for (int i = 0; i < height; i++)
			 for (int j = 0; j < width; j++)
			 {
				 k = i * l_width + j * 3;
				 Im->RgbData[index].rgbRed = pColorData[k + 2];
				 Im->RgbData[index].rgbGreen = pColorData[k + 1];
				 Im->RgbData[index].rgbBlue = pColorData[k];
				 Im->RgbData[index].rgbReversed = 0;
				 index++;
			 }
	 }
	 fclose(fp);
	 return Im;
}

YUVQUAD RGB2YUV(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	YUVQUAD YUV = (YUVQUAD)malloc(width*height * sizeof(struct tagYUVQUAD));
	for (int i = 0; i < width*height; i++)
	{
		YUV[i].yuvY = (BYTE)(0.114*Im->RgbData[i].rgbBlue + 0.587*Im->RgbData[i].rgbGreen + 0.299*Im->RgbData[i].rgbRed);		//Ϊÿ����Y��ֵ
		YUV[i].yuvU = (BYTE)(0.437*Im->RgbData[i].rgbBlue - 0.289*Im->RgbData[i].rgbGreen - 0.148*Im->RgbData[i].rgbRed);		//Ϊÿ����U��ֵ
		YUV[i].yuvV = (BYTE)(-0.100*Im->RgbData[i].rgbBlue - 0.515*Im->RgbData[i].rgbGreen + 0.615*Im->RgbData[i].rgbRed);
	}
	return YUV;
}
int OTSU(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	YUVQUAD yuv = RGB2YUV(Im);
	//ֱ��ͼHistogram����ͳ��ÿ���Ҷ�������Ŀ
	int Histogram[256] = { 0 };
	for (int i = 0; i < num; i++)
	{
		Histogram[(int)yuv[i].yuvY * 256 / 256]++;
	}
	float pt[256], w[256], u[256], o[256], Ut;
	//ptͳ�ƻҶȼ�Ϊi�����ظ���ռ�����صı���
	pt[0] = (float)Histogram[0] / num;
	//wΪ�Ҷȼ�С����i��ͼ����������ռ�������ۼ�
	w[0] = pt[0];
	//uΪ�Ҷȼ�С����i��ͼ������i*pt[i]���ۼ�
	u[0] = w[0];
	for (int i = 1; i < 256; i++)
	{
		pt[i] = (float)Histogram[i] / num;
		w[i] = w[i - 1] + pt[i];
		u[i] = u[i - 1] + i * pt[i];
	};
	Ut = u[255];//����ͼ��ƽ���Ҷ�
	for (int i = 0; i < 256; i++)
	{
		o[i] = w[i] * (1 - w[i])*(u[i]/w[i] - (u[255]-u[i])/(1- w[i]))*(u[i] / w[i] - (u[255] - u[i]) / (1 - w[i]));//����
	};
	int maxi = 0;
	float maxo = 0;
	for (int i = 0; i < 256; i++)
	{
		if (o[i] != 0x7FC0000)
			if (o[i] > maxo) { maxo = o[i]; maxi = i; }//����0~255���ҽף����㲢Ѱ����䷽���ֵ�����ҵ�ʱ��iΪ��Ӧ��䷽��ĻҶ�ֵ

	}
	return maxi * 256 / 256;
}

void FullColor2Binary(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	Im->InfoHeader->biBitCount = 1;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);	//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���
	int num = width * height;
	YUVQUAD yuv = RGB2YUV(Im);
	int threshold = OTSU(Im);	//�ô���㷨��ȡ��ֵ
	for (int i = 0; i < width*height; i++)	//�ж���ֵ��ϵ
	{
		if (yuv[i].yuvY > threshold)
		{
			Im->RgbData[i].rgbBlue = 255;
			Im->RgbData[i].rgbGreen = 255;
			Im->RgbData[i].rgbRed = 255;
		}
		else
		{
			Im->RgbData[i].rgbBlue = 0;
			Im->RgbData[i].rgbGreen = 0;
			Im->RgbData[i].rgbRed = 0;
		}
	}
	//Ϊת��Ϊ��ֵͼ��ı���Ӧͷ��Ϣ
	Im->FileHeader->bfOffset = 62;
	Im->FileHeader->bfSize = 62 + l_width * height;
	Im->InfoHeader->biBitCount = 1;
	Im->InfoHeader->biSizeImage = l_width * height;
	Im->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*2);
	Im->Pallete[0].rgbBlue = Im->Pallete[0].rgbRed = Im->Pallete[0].rgbGreen = 0;
	Im->Pallete[1].rgbBlue = Im->Pallete[1].rgbRed = Im->Pallete[1].rgbGreen = 255;
	Im->Pallete[0].rgbReversed = Im->Pallete[1].rgbReversed = 0;
}

int Niblack_Threshold(BmpImage Im,int i,int j, YUVQUAD yuv)
{
	int r = 7;
	int k = -0.1;
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int threshold;

	int m = 0, s = 0, n = 0;
	for (int p = i - r; p <= i + r; p++)	//�����ֵ
	{
		for (int q = j - r; q <= j + r; q++)
		{
			int temp;
			if (p < 0 || q < 0||p>=width||q>=height)	temp = 0;
			else
			{
				temp = yuv[p + q * width].yuvY;
				n++;
			}
			m += temp;
		}
	}
	m = m / n;
	for (int p = i - r; p <= i + r; p++)	//�����׼��
	{
		for (int q = j - r; q <= j + r; q++)
		{
			int temp;
			if (p < 0 || q < 0 || p >= width || q >= height)	temp = 0;
			else
			{
				temp = yuv[p + q * width].yuvY;
				s += (temp - m)*(temp - m);
			}
		}
	}
	s = (int)sqrt(s / n);
	threshold = m + 1.0*k*s;
	return threshold;
}
void Niblack(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	Im->InfoHeader->biBitCount = 1;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���
	int num = width * height;
	YUVQUAD yuv = RGB2YUV(Im);
	int* result = (int*)malloc(sizeof(int)*width*height);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int threshold = Niblack_Threshold(Im,i,j,yuv);
			if (yuv[i + j * width].yuvY < threshold)
				result[i + j * width] = 1;
			else
				result[i + j * width] = 0;
			//printf("%d %d ||", threshold, result[i + j * width]);
		}
	}
	for (int i = 0; i < width*height; i++)
	{
		if (result[i]==0)
		{
			Im->RgbData[i].rgbBlue = 255;
			Im->RgbData[i].rgbGreen = 255;
			Im->RgbData[i].rgbRed = 255;
		}
		else
		{
			Im->RgbData[i].rgbBlue = 0;
			Im->RgbData[i].rgbGreen = 0;
			Im->RgbData[i].rgbRed = 0;
		}
	}
	Im->FileHeader->bfOffset = 62;
	Im->FileHeader->bfSize = 62 + l_width * height;
	Im->InfoHeader->biBitCount = 1;
	Im->InfoHeader->biSizeImage = l_width * height;
	Im->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE) * 2);
	Im->Pallete[0].rgbBlue = Im->Pallete[0].rgbRed = Im->Pallete[0].rgbGreen = 0;
	Im->Pallete[1].rgbBlue = Im->Pallete[1].rgbRed = Im->Pallete[1].rgbGreen = 255;
	Im->Pallete[0].rgbReversed = Im->Pallete[1].rgbReversed = 0;
}

void WriteBMP(BmpImage Im, char* s)
{
	FILE* fp = fopen(s, "wb");
	fwrite(Im->FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(Im->InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���
	BYTE *pColorData = (BYTE *)malloc(height*l_width);
	memset(pColorData, 0, height*l_width);
	long nData = height * l_width;

	if (Im->Pallete != NULL)
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		for (int i = 0; i < num; i++)
		{
			fwrite(&(Im->Pallete[i].rgbBlue), sizeof(BYTE), 1, fp);
			fwrite(&(Im->Pallete[i].rgbGreen), sizeof(BYTE), 1, fp);
			fwrite(&(Im->Pallete[i].rgbRed), sizeof(BYTE), 1, fp);
			fwrite(&(Im->Pallete[i].rgbReversed), sizeof(BYTE), 1, fp);
		}
	}

	if (Im->InfoHeader->biBitCount < 24)
	{
		int k;
		int index = 0;
		if (Im->InfoHeader->biBitCount == 1)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 8;//k:ȡ�ø�������ɫ������ʵ�����������е����
					if (Im->RgbData[index].rgbRed == 255)
						mixIndex = 1;
					switch (j % 8)
					{
					case 0:
						mixIndex = mixIndex << 7;
						break;
					case 1:
						mixIndex = mixIndex << 6;
						break;
					case 2:
						mixIndex = mixIndex << 5;
						break;

					case 3:
						mixIndex = mixIndex << 4;
						break;
					case 4:
						mixIndex = mixIndex << 3;
						break;

					case 5:
						mixIndex = mixIndex << 2;
						break;
					case 6:
						mixIndex = mixIndex << 1;
						break;

					case 7:
						mixIndex = mixIndex;
						break;
					}
					pColorData[k] += mixIndex;
					index++;
				}
		}
		
	}
	else//λͼΪ24λ���ɫ
	{
		int k;
		int index = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				k = i * l_width + j * 3;
				pColorData[k + 2] = Im->RgbData[index].rgbRed;
				pColorData[k + 1] = Im->RgbData[index].rgbGreen;
				pColorData[k] = Im->RgbData[index].rgbBlue;
				index++;
			}
			/*for (int j= i * l_width + width * 3; j < i * l_width+l_width; j++)
			{
				pColorData[j] = 0;
			}*/
		}
	}
	fwrite(pColorData, 1, nData, fp);
}

BmpImage BmpCpy(BmpImage Im)
{
	BmpImage Im_out = (BmpImage)malloc(sizeof(struct tagBMPIMAGE));

	Im_out->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im_out->FileHeader->bfOffset = Im->FileHeader->bfOffset;
	Im_out->FileHeader->bfReserved1 = Im->FileHeader->bfReserved1;
	Im_out->FileHeader->bfReserved2 = Im->FileHeader->bfReserved2;
	Im_out->FileHeader->bfSize = Im->FileHeader->bfSize;
	Im_out->FileHeader->bfType = Im->FileHeader->bfType;

	Im_out->InfoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));
	Im_out->InfoHeader->biBitCount = Im->InfoHeader->biBitCount;
	Im_out->InfoHeader->biClrImportant = Im->InfoHeader->biClrImportant;
	Im_out->InfoHeader->biClrUsed = Im->InfoHeader->biClrUsed;
	Im_out->InfoHeader->biCompression = Im->InfoHeader->biCompression;
	Im_out->InfoHeader->biHeight = Im->InfoHeader->biHeight;
	Im_out->InfoHeader->biPlanes = Im->InfoHeader->biPlanes;
	Im_out->InfoHeader->biSize = Im->InfoHeader->biSize;
	Im_out->InfoHeader->biSizeImage = Im->InfoHeader->biSizeImage;
	Im_out->InfoHeader->biWidth = Im->InfoHeader->biWidth;
	Im_out->InfoHeader->biXPelsPerMeter = Im->InfoHeader->biXPelsPerMeter;
	Im_out->InfoHeader->biYPelsPerMeter = Im->InfoHeader->biYPelsPerMeter;

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;

	if (Im->Pallete != NULL)
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		Im_out->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*num);
		memcpy(Im_out->Pallete, Im->Pallete, sizeof(struct tagPALLATE)*num);
	}
	else Im_out->Pallete = NULL;

	Im_out->RgbData = (RGBQUAD)malloc(width*height * sizeof(struct tagRGBQUAD));
	memcpy(Im_out->RgbData, Im->RgbData, width*height * sizeof(struct tagRGBQUAD));

	return Im_out;
}

//////////////////////////////////////////////////
//////////////////////��̬ѧ//////////////////////
//////////////////////////////////////////////////
StrElem Cross()
{
	StrElem Cross;
	Cross.x = (int*)malloc(5 * sizeof(int));
	Cross.y = (int*)malloc(5 * sizeof(int));
	Cross.x[0] = -1; Cross.x[1] = 0; Cross.x[2] = 0; Cross.x[3] = 0; Cross.x[4] = 1;
	Cross.y[0] = 0; Cross.y[1] = -1; Cross.y[2] = 0; Cross.y[3] = 1; Cross.y[4] = 0;
	Cross.len = 5;
	Cross.r = 1;
	return Cross;
}

StrElem Square(int r)
{
	StrElem Square;
	Square.len = (2 * r + 1)*(2 * r + 1);
	Square.x = (int*)malloc(Square.len * sizeof(int));
	Square.y = (int*)malloc(Square.len * sizeof(int));
	int idx = 0;
	for (int i = -r; i <= r; i++) {
		for (int j = -r; j <= r; j++) {
			Square.x[idx] = i;
			Square.y[idx] = j;
			idx++;
		}
	}
	Square.r = r;
	return Square;
}

void Erosion(BmpImage Im)
{
	StrElem strelem=Cross();

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;

	int* out = (int*)malloc(sizeof(int)*num);
	memset(out, 0, sizeof(int)*num);
	
	for(int i= strelem.r;i<width- strelem.r;i++)	//�����Ǳ�Ե��ÿһ����
		for (int j = strelem.r; j < height - strelem.r; j++)
		{
			int flag = 1;
			for (int k = 0; k < strelem.len; k++)
			{
				if (Im->RgbData[(j+ strelem.y[k])*width + i+ strelem.x[k]].rgbBlue != 0)
				{
					flag = 0; break;	//�ж�B�Ƿ������A
				}
			}
			if (flag == 1)
				out[j*width + i] = 1;	//��ȡ��ֵ�����
		}
	for (int i = 0; i < num; i++)
	{
		if (out[i] == 1)
		{
			Im->RgbData[i].rgbBlue = 0;
			Im->RgbData[i].rgbRed = 0;
			Im->RgbData[i].rgbGreen = 0;
		}
		else
		{
			Im->RgbData[i].rgbBlue = 255;
			Im->RgbData[i].rgbRed =255;
			Im->RgbData[i].rgbGreen = 255;
		}
	}
}

void Dilation(BmpImage Im)
{
	StrElem strelem = Cross();

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;

	int width_l = width + 2 * strelem.r;
	int height_l = height + 2 * strelem.r;
	int r = strelem.r;

	int* in = (int*)malloc(sizeof(int)*width_l*height_l);
	int* out = (int*)malloc(sizeof(int)*width_l*height_l);
	memset(in, 0, sizeof(int)*width_l*height_l);
	memset(out, 0, sizeof(int)*width_l*height_l);

	for(int i= 0;i< width;i++)//����ÿһ���㣬�����������
		for (int j =0; j < height; j++)
		{
			if (Im->RgbData[i + j * width].rgbBlue == 0)
				in[(i + strelem.r) + (j+r) * width_l] = 1;
		}


	for(int i=r;i<width_l-r;i++)
		for (int j = r; j < height_l-r; j++)
		{
			int flag = 0;
			for (int k = 0; k < strelem.len; k++)
			{
				if (in[(i + strelem.x[k]) + width_l * (j + strelem.y[k])] == 1)	//�ж�B��A�Ľ��Ƿ�
				{
					flag = 1;
					break;
				}
			}
			if(flag==1)
			out[i + width_l * j] = 1;
		}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			if (out[(i + r) + (j + r)*width_l] == 1)
			{
				Im->RgbData[i + j * width].rgbBlue = 0;
				Im->RgbData[i + j * width].rgbRed = 0;
				Im->RgbData[i + j * width].rgbGreen = 0;
			}
			else
			{
				Im->RgbData[i + j * width].rgbBlue = 255;
				Im->RgbData[i + j * width].rgbRed = 255;
				Im->RgbData[i + j * width].rgbGreen = 255;
			}
		}

	free(in);
	free(out);
}

void Opening(BmpImage Im)
{
	Erosion(Im);
	Dilation(Im);
}

void Closing(BmpImage Im)
{
	Dilation(Im);
	Erosion(Im);
}