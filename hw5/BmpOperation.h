BmpImage ReadBMP(char *s);
void WriteBMP(BmpImage Im, char* s);
BmpImage BmpCpy(BmpImage Im);
void FullColor2Grayscale(BmpImage Im);
BmpImage Filtering(Filter fl, BmpImage Im);
Filter ConstructFilter(enum FilterType ft);
BmpImage Filtering_yuv(Filter fl, BmpImage Im);