/**********************************************/
/*Himanshu Chaudhary                          */
/*April 26, 2016                              */
/*CS 241L    Section #003                     */
/**********************************************/

#include <stdlib.h> 
#include <stdio.h>


int getIntFromArray(unsigned char bytes[])
{
  int n = 
    bytes[0] |
    bytes[1] << 8 | 
    bytes[2] << 16 |
    bytes[3] << 24; 
  return n;
}

/* @param c, type char
 *  the character that needs to be encoded
 * @param in, type FILE* in
 *  is the file that contain the image data
 * @param out, type FILE* out
 *  is the file that contains the encoded message
 * This function encodes a character from out image
 *  by masking 2 bits of c to last 2 bits of the color in
 *  in each pixel*/
void writeC(char c, FILE* in, FILE* out)
{
  unsigned char tempChar;
  unsigned char newChar;
  int i =3; 
  while (i>=0)
  {
    fread(&newChar,1,1,in);
    /* brings the 2 bits that needs to be encoded
       in the first*/
    tempChar = c << i*2;
    /* brings those two bits in the end*/
    tempChar = tempChar>>6;
    /* zereos out the last two bits*/
    newChar = 252 & newChar;
    /* places tempChar in the last two bits of newChar*/
    newChar = newChar|tempChar;

    fwrite(&newChar,1,1,out);
    i--;
  }
}


  int main(int argc, char** argv)
{
  char* infilename = argv[1];
  char* outfilename = argv[2];
  char tempChar;
  unsigned char header[54];
  int isEnd =0;

  FILE* in = fopen(infilename, "rb");
  FILE* out = fopen(outfilename, "wb");

  int pixelDataSize;

  int i;

  /* read header into array */
  fread(header, 1, 54, in);\

  pixelDataSize = getIntFromArray(&header[34]);

  /* is this really a bitmap? */
  if (header[0] != 'B' || header[1] != 'M')
  {
    printf("Input file is not a bitmap\n");
  }

  /* Is the header size what we expect? */
  if(getIntFromArray(&header[10]) != 54)
  {
    printf("Unexpected header size\n");
  }

  /* How many bits per pixel? Expecting 24*/
  if(!(header[28] == 24 && header[29] == 0))
  {
    printf("Unexpected number of bits/pixel\n");  
  }
  
  
  /* writes header to the output file*/
  fwrite(header, 1, sizeof(header), out);
  /* loops through the each color in pixel and writes the
   * encoded pixel to the output file*/
  for (i =0; i<pixelDataSize; i++)
  {
    /* encodes the data until the data is finished*/
    while (isEnd==0)
    {
      tempChar = getchar();
      if (tempChar != EOF) writeC(tempChar, in, out);
      /* puts '\0' as the last character in the message*/
      else
      {
         tempChar = '\0';
         writeC(tempChar,in,out);
         isEnd =1;
      }
    }
    /* copies the imgaes as it is after the message is finished*/
    if (isEnd ==1)
    {
      fread(&tempChar,1,1,in);
      fwrite(&tempChar,1,1,out);\
    }
  }
  return 0;
}
