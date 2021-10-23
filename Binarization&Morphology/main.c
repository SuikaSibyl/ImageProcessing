#include "BmpDefine.h"
#include "BmpOperation.h"

#define BinaryC 0
#define Choice 4

int main(void)
{
#if BinaryC==0
	BmpImage Im = ReadBMP("DSC06448.bmp");
	FullColor2Binary(Im);	//≤‚ ‘¥ÛΩÚ∂˛÷µªØ
	WriteBMP(Im, "Otsu.bmp");
#endif

#if BinaryC==1
	BmpImage Im = ReadBMP("DSC07917.bmp");
	Niblack(Im);	//≤‚ ‘Niblack∂˛÷µªØ
	WriteBMP(Im, "Niblack.bmp");
#endif

#if Choice==1
	Erosion(Im);//≤‚ ‘Erosion
	WriteBMP(Im, "Erosion.bmp");
#endif

#if Choice==2
	Dilation(Im);//≤‚ ‘Dilation
	WriteBMP(Im, "Dilation.bmp");
#endif

#if Choice==3
	Opening(Im);//≤‚ ‘Opening
	WriteBMP(Im, "Opening.bmp");
#endif

#if Choice==4
	Closing(Im);//≤‚ ‘Closing
	WriteBMP(Im, "Closing.bmp");
#endif
}