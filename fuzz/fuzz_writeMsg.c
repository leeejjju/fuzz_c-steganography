#include <stdio.h>
#include "../stego.h"
#include <stdint.h>

void copy_header(FILE *, int, FILE *);
int get_message_length(char[]);
int message_fits(int, int, int);
int count_new_lines(FILE *);
void encode_length(FILE *, FILE *, int);
void encode_message(FILE *, FILE *, int, char *, int, int);
// #define IMAGE
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
  
  FILE *fp;
  #ifdef IMAGE // mutate image data
  char imagepath[] = "img/temp.ppm";
  if ((fp = fopen(imagepath, "w+")) == NULL)
  {
    printf("Could not open file to write %s.\nAborting\n", imagepath);
    return 1;
  }
  fwrite(data, 1, size, fp);
  fflush(fp);
  fclose(fp);
  if ((fp = fopen(imagepath, "r+")) == NULL)
  {
    printf("Could not open file to read %s.\nAborting\n", imagepath);
    return 1;
  } 
  char *myMessage = "test message";
  int message_length = get_message_length(myMessage);
  #else // mutate message data
  
  if ((fp = fopen("img/hackny.ppm", "r+")) == NULL)
  {
    printf("Could not open file %s.\nAborting\n", argv[2]);
    return 1;
  }
  char *myMessage = (char *)data;
  int message_length = (int)size;
  #endif


  if (read_ppm_type(fp))
  {
    skip_comments(fp);


    int w = get_width(fp);
    int h = get_height(fp);

    if (message_fits(message_length, w, h))
    {
      if (read_color_depth(fp))
      {
        FILE *fp_t = fopen("out.ppm", "w");
        int i = count_new_lines(fp);
        copy_header(fp, i, fp_t);
        encode_length(fp, fp_t, (message_length - 8) / 8);
        encode_message(fp, fp_t, (message_length - 8), myMessage, w, h);
        printf("Encoding Process Complete. Take a look at out.ppm\n");

        fclose(fp);
        fclose(fp_t);
      }
      else
      {
        printf("\nError: Image color depth invalid. Must be 255.\n");
        return 1;
      }
    }
    else
    {
      printf("\nError: Image file is not large enough to hold the message.\nAborting.\n");
      return 1;
    }
  }
  else
  {
    printf("Error: Wrong file format.\nAborting\n");
    return 1;
  }

  return 0;
}

void copy_header(FILE *fp1, int numLines, FILE *fp2)
{
  int i;
  char temp;

  rewind(fp1); // Goes back to the beginning of the file

  for (i = 0; i < numLines; i++)
  {
    while ((temp = fgetc(fp1)) != EOF && temp != '\n')
    {
      fputc(temp, fp2);
    }
    fputc('\n', fp2);
  }

  return;
}
int get_message_length(char my_msg[])
{
  int i = 0;
  while (my_msg[i] != '\0')
  {
    i++;
  }
  return i * 8 + 8;
}
int message_fits(int length, int w, int h)
{
  return length < w * h * 3;
}
int count_new_lines(FILE *fp)
{
  char temp;
  int count = 0;

  rewind(fp);

  while ((temp = fgetc(fp)) != EOF)
    if (temp == '\n')
      count++;

  return count;
}

void encode_length(FILE *in, FILE *out, int length)
{
  char temp;
  int i, l;
  for (i = 0; i < 8; i++)
  {
    temp = fgetc(in);
    l = length;
    l >>= 7 - i;
    if ((l & 1) == 1)
    {
      if ((temp & 1) == 0)
      {
        temp++;
      }
    }
    else
    {
      if ((temp & 1) == 1)
      {
        temp--;
      }
    }
    fputc(temp, out);
  }

  return;
}

void encode_message(FILE *in, FILE *out, int num_to_encode, char *my_message, int w, int h)
{
  int encoded = 0;
  unsigned char temp;
  int idx = 0, num_coppied = 0;
  char current;

  int fileSize = (w * h * 3) - 8; // Number of bytes after first 8
  int i;

  for (i = 0; i < fileSize; i++)
  {
    temp = fgetc(in);
    current = my_message[idx];

    current >>= 7 - num_coppied;
    num_coppied++;

    if (encoded <= num_to_encode)
    {
      encoded++;
      if ((current & 1) == 1)
      {
        if ((temp & 1) == 0)
        {
          temp++;
        }
      }
      else
      {
        if ((temp & 1) == 1)
        {
          temp--;
        }
      }
      if (num_coppied == 8)
      {
        idx++;
        num_coppied = 0;
      }
    }

    fputc(temp, out);
  }

  return;
}
