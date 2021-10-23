#include "BmpDefine.h"
#include "BmpOperation.h"

int main(void)
{
	BmpImage Im = ReadBMP("CASE1.bmp");
	double Sigmas, Sigmar;
	SelfAdaptingParameters(Im, &Sigmas, &Sigmar);
	BmpImage Im_out= BilateralFilting(Im,15, Sigmas,Sigmar);
	WriteBMP(Im_out, "TEST.bmp");
}