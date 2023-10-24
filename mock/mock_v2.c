#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#define M 100
#define N 100

int get_num_from_str(char *s) {
  int res = 0;
  for(int i = 0; s[i]; i++) {
    if(!isdigit(s[i])) return -1;
    res = 10 * res + (s[i] - '0');
  }
  return res;
}

int parse_argument(int argc, char *argv[], \
                   int *cflag, int *iflag, int *uflag, \
                   int *dflag, int *Dflag, int *sflag, int *fflag, int *wflag, \
                   char **input_filename, char **output_filename) {
  // Get options and filename
  int input_id = -1, output_id = -1;
  for(int i = 1; i < argc; i++){
    if(input_id == -1) {
      if(!strcmp(argv[i], "-c")) *cflag = 1;
      else if(!strcmp(argv[i], "-i")) *iflag = 1;
      else if(!strcmp(argv[i], "-u")) *uflag = 1;
      else if(!strcmp(argv[i], "-d")) *dflag = 1;
      else if(!strcmp(argv[i], "-D")) *Dflag = 1;
      else if(!strcmp(argv[i], "-s")) {
        if(i == argc - 1 || (*sflag = get_num_from_str(argv[i + 1])) == -1) {
          printf("Error: Invalid number of bytes to skip.\n");
          return -1;
        }
        i++;
      }
      else if(!strcmp(argv[i], "-f")) {
        if(i == argc - 1 || (*fflag = get_num_from_str(argv[i + 1])) == -1) {
          printf("Error: Invalid number of fields to skip.\n");
          return -1;
        }
        i++;
      }
      else if(!strcmp(argv[i], "-w")) {
        if(i == argc - 1 || (*wflag = get_num_from_str(argv[i + 1])) == -1) {
          printf("Error: Invalid number of bytes to compare.\n");
          return -1;
        }
        i++;
      }
      else if(argv[i][0] == '-') {
        printf("Error: Invalid option %s.\n", argv[i]);
        return -1;
      }
      else *input_filename = argv[i], input_id = i;
    }
    else if(output_id == -1) {
      *output_filename = argv[i], output_id = i;
    }
    else {
      printf("Error: Too many argument.\n");
      return -1;
    }
  }
  // Set dflag to false if Dflag is true
  if(*Dflag) *dflag = 0;

  // Return error if no input file
  if(*input_filename == NULL) {
    printf("Error: No input file.\n");
    return -1;
  }
  // Return error if uflag && (dflag || Dflag) is true
  if(*uflag && (*dflag || *Dflag)) {
    printf("Error: Can't set -u and %s at the same time.\n", (*dflag?"-d":"-D"));
    return -1;
  }
  // Return error if Dflag && cflag is true
  if(*Dflag && *cflag) {
    printf("Error: Printing all duplicated lines and repeat counts is meaningless.\n");
    return -1;
  }
  return 0;
}

int open_file(char *input_filename, char *output_filename, \
              FILE **input_file, FILE **output_file) {
  // Open input file
  *input_file = fopen(input_filename, "r");
  if(*input_file == NULL) {
    printf("Error: Can't open input file.\n");
    return -1;
  }
  // Open output file
  if(output_filename != NULL) {
    *output_file = fopen(output_filename, "w");
    if(*output_file == NULL) {
      printf("Error: Can't open output file.\n");
      fclose(*input_file);
      return -1;
    }
  }
  return 0;
}

int get_contents(FILE *input_file, int *num_line, char ***contents) {
  char buffer[N] = {};
  int idx = 0;
  char **data = (char**)calloc(M, sizeof(char*));
  // Read file contents
  while(fgets(buffer, N, input_file) != NULL) {
    // Replace '\n' with '\0'
    int len = strlen(buffer);
    if(buffer[len - 1] == '\n') buffer[len - 1] = '\0';
    // Copy buffer to data[idx]
    data[idx] = (char*)calloc(N, sizeof(char));
    strcpy(data[idx], buffer);
    idx++;
  }
  *num_line = idx;
  *contents = data;
  return 0;
}

int equal_str(char *s1, char *s2, int iflag, int sflag, int fflag, int wflag) {
  // Move to next space
  for(int i = 0; i < fflag; i++) {
    while(*s1 == ' ') s1++;
    while(*s2 == ' ') s2++;
    while(*s1 != '\0' && *s1 != ' ') s1++;
    while(*s2 != '\0' && *s2 != ' ') s2++;
  }
  // Move to next char
  for(int i = 0; i < sflag; i++) {
    if(*s1 == '\0' && *s2 == '\0') break;
    if(*s1 != '\0') s1++;
    if(*s2 != '\0') s2++;
  }
  // Compare
  if(iflag) {
    if(wflag != -1) return strncasecmp(s1, s2, wflag) == 0;
    else return strcasecmp(s1, s2) == 0;
  }
  else {
    if(wflag != -1) return strncmp(s1, s2, wflag) == 0;
    else return strcmp(s1, s2) == 0;
  }
}

int get_count(int num_line, char **contents, \
              int iflag, int sflag, int fflag, int wflag, int **count) {
  int *cnt = (int*)malloc(num_line * sizeof(int));
  cnt[num_line - 1] = 1;
  for(int i = num_line - 2; i >= 0; i--) {
    if(equal_str(contents[i], contents[i + 1], iflag, sflag, fflag, wflag)) 
      cnt[i] = cnt[i+1] + 1;
    else cnt[i] = 1;
  }
  *count = cnt;
  return 0;
}

int output_result(FILE *output_file, int num_line, char **contents, \
                  int *count, int cflag, int uflag, int dflag, int Dflag) {
  for(int i = 0; i < num_line; i++) {
    char buffer[N] = {};
    // Decide which content should be output based on the flag
    if(uflag && (count[i] != 1 || (i != 0 && count[i-1] != 1))) continue;
    else if(dflag && (count[i] == 1 || (i != 0 && count[i]+1 == count[i-1]))) continue;
    else if(Dflag && (count[i] == 1 && (i == 0 || count[i-1] == 1))) continue;
    else if(!uflag && !dflag && !Dflag && (i != 0 && count[i]+1 == count[i-1])) continue;

    // Concate the content
    if(cflag) sprintf(buffer, "   %d ", count[i]);
    strcat(buffer, contents[i]);
    strcat(buffer, "\n");

    // Output to standard output or output file
    if(output_file) fputs(buffer, output_file);
    else printf("%s", buffer);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  // Get options and filename
  int cflag = 0, iflag = 0, uflag = 0, dflag = 0, Dflag = 0;
  int sflag = 0, fflag = 0, wflag = -1;
  char *input_filename = NULL, *output_filename = NULL;
  if(parse_argument(argc, argv, &cflag, &iflag, &uflag, \
                    &dflag, &Dflag, &sflag, &fflag, &wflag, \
                    &input_filename, &output_filename) == -1) {
    return -1;
  }

  // Open input file and output file
  FILE *input_file = NULL, *output_file = NULL;
  if(open_file(input_filename, output_filename, \
               &input_file, &output_file) == -1) {
    return -1;
  }

  // Get contents from input file
  int num_line = 0;
  char **contents = NULL;
  if(get_contents(input_file, &num_line, &contents) == -1) {
    return -1;
  }

  // Get count array to perform uniq operation
  int *count = NULL;
  if(get_count(num_line, contents, iflag, sflag, fflag, wflag, &count) == -1) {
    return -1;
  }

  // See how is count array doing
  // for(int i = 0; i < num_line; i++)
  //   printf("%d %s\n", count[i], contents[i]);

  // Output result
  if(output_result(output_file, num_line, contents, count, \
                   cflag, uflag, dflag, Dflag) == -1) {
    return -1;
  }
  
  // Close file
  fclose(input_file);
  if(output_file != NULL) fclose(output_file);

  // Free contents and count
  for(int i = 0; i < num_line; i++) free(contents[i]);
  free(contents);
  free(count);

  return 0;
}