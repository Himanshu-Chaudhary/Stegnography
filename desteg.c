/**********************************************/
/*Himanshu Chaudhary                          */
/*April 13, 2016                              */
/*CS 241L    Section #003                     */
/**********************************************/

#include <stdlib.h>
#include <stdio.h>

/* @param in, type FILE* in
 *  is the file that contain the image data
 * This function returns an encoded character from
 *  the image*/

char readC(FILE* in)
{
  char tempChar;
  int i =0;
  char newChar='\0';
  /* loops through 4 values and gets the last two bits 
   *  which constructs an character from encoded pixels*/
  for (i=0;i<=3;i++)
  {
    fread(&tempChar,1,1,in);
    /* gets the last two bits*/
    tempChar = tempChar & 3;
    /* construct a new character by placing the bits from
     * last to bottom*/
    newChar = newChar | (tempChar <<i*2);
  }
  return newChar;
}

int getIntFromArray(unsigned char bytes[])
{
  int n = 
    bytes[0] |
    bytes[1] << 8 | 
    bytes[2] << 16 |
    bytes[3] << 24; 
  return n;
}


int main(int argc, char** argv)
{
  char* infilename = argv[1];
  FILE* in = fopen(infilename, "rb");
  char newChar;
  unsigned char header[54];
  int i;
 
    /* read header into array */
  fread(header, 1, 54, in);

  /* is this really a bitmap? */
  if (header[0] != 'B' || header[1] != 'M')
  {
    printf("Input file is not a bitmap\n");
    exit(1);
  }

  /* Is the header size what we expect? */
  if(getIntFromArray(&header[10]) != 54)
  {
    printf("Unexpected header size\n");
    exit(1);
  }

  /* How many bits per pixel? Expecting 24*/
  if(!(header[28] == 24 && header[29] == 0))
  {
    printf("Unexpected number of bits/pixel\n");  
    exit(1);
  }
  fread (header,1,54,in);
  newChar = readC(in);
  /* prints the decoded message in a loop until the last 
   * character is reached*/
  i =0;
  while (newChar!= '\0')
  {
   printf("%c",newChar);
   newChar = readC(in);
   i++;
   }
  return 0;
}

