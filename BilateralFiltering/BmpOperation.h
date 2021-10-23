BmpImage ReadBMP(char *s);
void WriteBMP(BmpImage Im, char* s);
BmpImage BmpCpy(BmpImage Im);
void FullColor2Grayscale(BmpImage Im);
BmpImage BilateralFilting(BmpImage Im, int S, double Sigmas, double Sigmar);
void SelfAdaptingParameters(BmpImage Im, double* Sigmas, double* Sigmar);