#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define N 21000000

int main(void) {
  char ch;
  int byte = 0;

  // Input file & stream
  const char input_filename[10] = "source2";
  FILE *input_file = fopen(input_filename, "rb+");
  char *stream_buffer = (char*)calloc(N, sizeof(char));
  FILE *stream = fmemopen(stream_buffer, N * sizeof(char), "wb+");
  
  // (1)
  char buffer1[1];
  fseek(input_file, 0, SEEK_SET);
  fseek(stream, 0, SEEK_SET);
  while((byte = fread(buffer1, sizeof(char), 1, input_file)) != 0) {
    fwrite(buffer1, sizeof(char), byte, stream);
  }
  fseek(stream, 0, SEEK_SET);
  const char output_filename1[10] = "output1";
  FILE *output_file1 = fopen(output_filename1, "wb+");
  while((ch = fgetc(stream)) != EOF) {
    fputc(ch, output_file1);
  }
  fclose(output_file1);

  // (2)
  char buffer2[32];
  fseek(input_file, 0, SEEK_SET);
  fseek(stream, 0, SEEK_SET);
  while((byte = fread(buffer2, sizeof(char), 32, input_file)) != 0) {
    fwrite(buffer2, sizeof(char), byte, stream);
  }
  fseek(stream, 0, SEEK_SET);
  const char output_filename2[10] = "output2";
  FILE *output_file2 = fopen(output_filename2, "wb+");
  while((ch = fgetc(stream)) != EOF) {
    fputc(ch, output_file2);
  }
  fclose(output_file2);

  // (3)
  char buffer3[1024];
  fseek(input_file, 0, SEEK_SET);
  fseek(stream, 0, SEEK_SET);
  while((byte = fread(buffer3, sizeof(char), 1024, input_file)) != 0) {
    fwrite(buffer3, sizeof(char), byte, stream);
  }
  fseek(stream, 0, SEEK_SET);
  const char output_filename3[10] = "output3";
  FILE *output_file3 = fopen(output_filename3, "wb+");
  while((ch = fgetc(stream)) != EOF) {
    fputc(ch, output_file3);
  }
  fclose(output_file3);

  // (4)
  char buffer4[4096];
  fseek(input_file, 0, SEEK_SET);
  fseek(stream, 0, SEEK_SET);
  while((byte = fread(buffer4, sizeof(char), 4096, input_file)) != 0) {
    fwrite(buffer4, sizeof(char), byte, stream);
  }
  fseek(stream, 0, SEEK_SET);
  const char output_filename4[10] = "output4";
  FILE *output_file4 = fopen(output_filename4, "wb+");
  while((ch = fgetc(stream)) != EOF) {
    fputc(ch, output_file4);
  }
  fclose(output_file4);

  // (5)
  char buffer5[4096];
  fseek(input_file, 0, SEEK_SET);
  fseek(stream, 0, SEEK_SET);
  while((fgets(buffer5, 4096, input_file))) {
    fputs(buffer5, stream);
  }
  fseek(stream, 0, SEEK_SET);
  const char output_filename5[10] = "output5";
  FILE *output_file5 = fopen(output_filename5, "wb+");
  while((ch = fgetc(stream)) != EOF) {
    fputc(ch, output_file5);
  }
  fclose(output_file5);

  // (6)
  fseek(input_file, 0, SEEK_SET);
  fseek(stream, 0, SEEK_SET);
  while((ch = fgetc(input_file)) != EOF) {
    fputc(ch, stream);
  }
  fseek(stream, 0, SEEK_SET);
  const char output_filename6[10] = "output6";
  FILE *output_file6 = fopen(output_filename6, "wb+");
  while((ch = fgetc(stream)) != EOF) {
    fputc(ch, output_file6);
  }
  fclose(output_file6);
  
  fclose(input_file);
  fclose(stream);
  return 0;
}