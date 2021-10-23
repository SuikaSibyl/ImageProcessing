#include "BmpDefine.h"
#include "BmpOperation.h"

#define TEST -1
#define Pic -1
int main(void)
{
	BmpImage Im1 = ReadBMP("Mine.bmp");
	BmpImage Im2 = ReadBMP("Test3.bmp");
	Histogram_Fitting(Im1, Im2);
	WriteBMP(Im1, "Result.bmp");

#if Pic==2
	Histogram_Fitting(Im1, Im3);
	WriteBMP(Im1, "Fitting2.bmp");
#endif
	//Test Logarithmic_gray
#if TEST==0
	BmpImage Im1 = ReadBMP("Lenna.bmp");
	BmpImage Im2 = ReadBMP("TEST1.bmp");
	BmpImage Im3 = ReadBMP("TEST2.bmp");
	BmpImage Im4 = ReadBMP("TEST5.bmp");
	FullColor2Grayscale(Im1);
	FullColor2Grayscale(Im2);
	FullColor2Grayscale(Im3);
	FullColor2Grayscale(Im4);
	WriteBMP(Im1, "Gray1.bmp");
	WriteBMP(Im2, "Gray2.bmp");
	WriteBMP(Im3, "Gray3.bmp");
	WriteBMP(Im4, "Gray4.bmp");
	logarithmic_gray(Im1);
	logarithmic_gray(Im2);
	logarithmic_gray(Im3);
	logarithmic_gray(Im4);
	WriteBMP(Im1, "LogGray1.bmp");
	WriteBMP(Im2, "LogGray2.bmp");
	WriteBMP(Im3, "LogGray3.bmp");
	WriteBMP(Im4, "LogGray4.bmp");
#endif
	//Test Logarithmic
#if TEST==1
	BmpImage Im1 = ReadBMP("Lenna.bmp");
	BmpImage Im2 = ReadBMP("TEST1.bmp");
	BmpImage Im3 = ReadBMP("TEST2.bmp");
	BmpImage Im4 = ReadBMP("TEST5.bmp");
	logarithmic(Im1);
	logarithmic(Im2);
	logarithmic(Im3);
	logarithmic(Im4);
	WriteBMP(Im1, "Log1.bmp");
	WriteBMP(Im2, "Log2.bmp");
	WriteBMP(Im3, "Log3.bmp");
	WriteBMP(Im4, "Log4.bmp");
#endif
	//Test Histogram_Equalization_Gray
#if TEST==2
	BmpImage Im1 = ReadBMP("Lenna.bmp");
	BmpImage Im2 = ReadBMP("TEST1.bmp");
	BmpImage Im3 = ReadBMP("TEST4.bmp");
	BmpImage Im4 = ReadBMP("TEST5.bmp");
	FullColor2Grayscale(Im1);
	FullColor2Grayscale(Im2);
	FullColor2Grayscale(Im3);
	FullColor2Grayscale(Im4);
	WriteBMP(Im1, "Gray1.bmp");
	WriteBMP(Im2, "Gray2.bmp");
	WriteBMP(Im3, "Gray3.bmp");
	WriteBMP(Im4, "Gray4.bmp");
	Histogram_Equalization_Gray(Im1);
	Histogram_Equalization_Gray(Im2);
	Histogram_Equalization_Gray(Im3);
	Histogram_Equalization_Gray(Im4);
	WriteBMP(Im1, "EqualGray1.bmp");
	WriteBMP(Im2, "EqualGray2.bmp");
	WriteBMP(Im3, "EqualGray3.bmp");
	WriteBMP(Im4, "EqualGray4.bmp");
#endif
	//Test Histogram_Equalization
#if TEST==3
	BmpImage Im1 = ReadBMP("Lenna.bmp");
	BmpImage Im2 = ReadBMP("TEST1.bmp");
	BmpImage Im3 = ReadBMP("TEST4.bmp");
	BmpImage Im4 = ReadBMP("TEST5.bmp");
	Histogram_Equalization(Im1);
	Histogram_Equalization(Im2);
	Histogram_Equalization(Im3);
	Histogram_Equalization(Im4);
	WriteBMP(Im1, "Equal1.bmp");
	WriteBMP(Im2, "Equal2.bmp");
	WriteBMP(Im3, "Equal3.bmp");
	WriteBMP(Im4, "Equal4.bmp");
#endif
	//Test Histogram_Equalization_Union
#if TEST==4
	BmpImage Im1 = ReadBMP("Lenna.bmp");
	BmpImage Im2 = ReadBMP("TEST1.bmp");
	BmpImage Im3 = ReadBMP("TEST4.bmp");
	BmpImage Im4 = ReadBMP("TEST5.bmp");
	Histogram_Equalization_Union(Im1);
	Histogram_Equalization_Union(Im2);
	Histogram_Equalization_Union(Im3);
	Histogram_Equalization_Union(Im4);
	WriteBMP(Im1, "Equal_Union1.bmp");
	WriteBMP(Im2, "Equal_Union2.bmp");
	WriteBMP(Im3, "Equal_Union3.bmp");
	WriteBMP(Im4, "Equal_Union4.bmp");
#endif
	//Test Histogram_Fitting_Gray
#if TEST==5
	BmpImage Im1 = ReadBMP("Lenna.bmp");
	BmpImage Im2 = ReadBMP("TEST0.bmp");
	BmpImage Im3 = ReadBMP("TEST1.bmp");
	BmpImage Im4 = ReadBMP("TEST3.bmp");
	FullColor2Grayscale(Im1);
	FullColor2Grayscale(Im2);
	FullColor2Grayscale(Im3);
	FullColor2Grayscale(Im4);
	WriteBMP(Im1, "Gray1.bmp");
	WriteBMP(Im2, "Gray2.bmp");
	WriteBMP(Im3, "Gray3.bmp");
	WriteBMP(Im4, "Gray4.bmp");
#if Pic==1
	Histogram_Fitting_Gray(Im1,Im2);
	WriteBMP(Im1, "Fitting_Gray1.bmp");
#endif
#if Pic==2
	Histogram_Fitting_Gray(Im1, Im3);
	WriteBMP(Im1, "Fitting_Gray2.bmp");
#endif
#if Pic==3
	Histogram_Fitting_Gray(Im1, Im4);
	WriteBMP(Im1, "Fitting_Gray3.bmp");
#endif
#endif
#if TEST==6
	BmpImage Im1 = ReadBMP("Lenna.bmp");
	BmpImage Im2 = ReadBMP("TEST0.bmp");
	BmpImage Im3 = ReadBMP("TEST1.bmp");
	BmpImage Im4 = ReadBMP("TEST3.bmp");
#if Pic==1
	Histogram_Fitting(Im1, Im2);
	WriteBMP(Im1, "Fitting1.bmp");
#endif
#if Pic==2
	Histogram_Fitting(Im1, Im3);
	WriteBMP(Im1, "Fitting2.bmp");
#endif
#if Pic==3
	Histogram_Fitting(Im1, Im4);
	WriteBMP(Im1, "Fitting3.bmp");
#endif
#endif
}