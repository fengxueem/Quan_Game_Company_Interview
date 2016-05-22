#include "stdio.h"
#include "stdlib.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
void read8bitBMP(char* fileName, int* mix, int* mxx, int* miy,int* mxy)
{
	int i;
	FILE* f = fopen(fileName, "rb");
	unsigned char info[(54+1024)];
	fread(info, sizeof(unsigned char), (54+1024), f); // read the 54-byte header
	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	int size = width * height;
	//read the datas
	unsigned char* data = (unsigned char*)malloc(size*sizeof(unsigned char));
	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);
	int transparent_color = data[0];
	int flag = 0;
	int count = 0;
	int c = 0;
	int n = 0;
	int m = 0;
	int minX = 0;
	int minY = 0;
	int maxY = height;
	int maxX = width;
	//getting the points value for each peak
	for (int t = 0; t < size; t++) {
		/* code */
		if(t % width == 0)
		{
			minY++;
		}
		if(data[t] != transparent_color)
		{
				t = size;
		}
		data[t] = 255;
	}

	for(int r = size-1;  r>= 0; r--)
	{
		 if(r % width == 0)
		 {
			 maxY--;
		 }
		 if(data[r] != transparent_color)
		 {
			 r=0;
		 }
		 data[r] = 255;
	}

	for(int l = 0; l < width ; l++)
	{
		for(int a = minY ; a < maxY ; a++)
		{
			int tmp = a * width;
		  if(data[l+tmp] != transparent_color)
			{
				//printf("Game over !! n = %d\n ", m);
				l =width;
				a = maxY;
			}
			data[l+tmp] = 255;
		}
		minX++;
	}

	for(int e = width-1 ; e > 0 ; e--)
	{
		for(int a = minY-1 ; a < maxY-1 ; a++)
		{
			int tmp = a * width;
		  if(data[e+tmp] != transparent_color)
			{
				e =0;
				a = maxY;
			}
			data[e+tmp] = 255;
		}
		maxX--;
	}
	FILE* ec = fopen("out.bmp", "wb");
	fwrite(info, 1078 * sizeof(unsigned char),1,ec);
	fwrite(data, (size) * sizeof(unsigned char),1,ec);
	fclose(ec);
	free(data);
	printf("finished crops, the minY is %d maxY is %d\n",minY,maxY);
	printf("finished crops, the minX is %d maxX is %d\n",minX,maxX);
	*mix = minX;
	*mxx = maxX;
	*miy = minY;
	*mxy = maxY;

	FILE* f1 = fopen(fileName, "rb");
	unsigned char info1[(54+1024)];
	fread(info1, sizeof(unsigned char), (54+1024), f1); // read the 54-byte header
	//read the datas
	unsigned char* data1 = (unsigned char*)malloc(size*sizeof(unsigned char));
	fread(data1, sizeof(unsigned char), size, f1); // read the rest of the data at once
	fclose(f1);
	unsigned char bmppad[3] = { 0, 0, 0 };
	int newHeight = maxY - minY + 1;
	int newWidth = maxX - minX + 1;
	int newImageSize = newWidth * newHeight;
	int newfileSize = newImageSize + 1078;
	FILE *R = fopen("fozzie-out.bmp", "wb");
	*(DWORD*)&info1[2] = newfileSize;//file size = image size + header size
	*(LONG*)&info1[18] = newWidth;//width
	*(LONG*)&info1[22] = newHeight;//height
	*(DWORD*)&info1[34] = newImageSize;//image size = file size - header size = width * height
	fwrite(info1, 1078 * sizeof(unsigned char), 1, R);
	for (int p = minY; p <= maxY; p++)
	{
		for (int q = minX; q <= maxX; q++)
		{
			fwrite(&data1[width * p + q], sizeof(unsigned char), 1, R);
		}
		fwrite(bmppad, 1, (4 - ((maxX - minX + 1) % 4))%4, R);
	}
	fclose(R);
	free(data1);
}

int main()
{
	int mix;
	int mxx;
	int miy;
	int mxy;
  printf("Hey Quan is fucking genius!\n");
  //FILE *a = fopen ("b.bmp", "r");
	read8bitBMP("b.bmp",&mix,&mxx,&miy,&mxy);
	printf("%d,%d,%d,%d \n",mix,mxx,miy,mxy);
  return 0;
}
