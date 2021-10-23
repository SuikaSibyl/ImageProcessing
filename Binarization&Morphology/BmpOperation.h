//¶ÁÐ´BMP
BmpImage ReadBMP(char *s);
void WriteBMP(BmpImage Im, char* s);
BmpImage BmpCpy(BmpImage Im);

void FullColor2Binary(BmpImage Im);
void Niblack(BmpImage Im);

void Erosion(BmpImage Im);
void Dilation(BmpImage Im);
void Opening(BmpImage Im);
void Closing(BmpImage Im);