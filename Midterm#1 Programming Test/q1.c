#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#define M 1000
#define N 1000

int parse_argument(int argc, char *argv[], \
                   int *cflag, int *iflag, int *uflag, \
                   char **input_filename, char **output_filename) {
  // Get options and filename
  int input_id = -1, output_id = -1;
  for(int i = 1; i < argc; i++){
    if(input_id == -1) {
      if(!strcmp(argv[i], "-c")) *cflag = 1;
      else if(!strcmp(argv[i], "-i")) *iflag = 1;
      else if(!strcmp(argv[i], "-u")) *uflag = 1;
      else if(argv[i][0] == '-') {
        printf("Invalid option %s.", argv[i]);
        return -1;
      }
      else *input_filename = argv[i], input_id = i;
    }
    else if(output_id == -1) {
      *output_filename = argv[i], output_id = i;
    }
    else {
      printf("Too many argument.\n");
      return -1;
    }
  }
  // Return error if no input file
  if(*input_filename == NULL) {
    printf("No input file.\n");
    return -1;
  }
  return 0;
}

int open_file(char *input_filename, char *output_filename, \
              FILE **input_file, FILE **output_file) {
  // Open input file
  *input_file = fopen(input_filename, "r");
  if(*input_file == NULL) {
    printf("Error opening input file.\n");
    return -1;
  }
  // Open output file
  if(output_filename != NULL) {
    *output_file = fopen(output_filename, "w");
    if(*output_file == NULL) {
      printf("Error opening output file.\n");
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

int equal_str(char *s1, char *s2, int iflag) {
  if(iflag) return strcasecmp(s1, s2) == 0;
  else return strcmp(s1, s2) == 0;
}

int get_count(int num_line, char **contents, int iflag, int **count) {
  int *cnt = (int*)malloc(num_line * sizeof(int));
  cnt[num_line - 1] = 1;
  for(int i = num_line - 2; i >= 0; i--) {
    if(equal_str(contents[i], contents[i + 1], iflag)) 
      cnt[i] = cnt[i+1] + 1;
    else cnt[i] = 1;
  }
  *count = cnt;
  return 0;
}

int output_result(FILE *output_file, int num_line, char **contents, \
                  int *count, int cflag, int uflag) {
  for(int i = 0; i < num_line; i++) {
    char buffer[N] = {};
    // Decide which content should be output based on uflag
    if(uflag && (count[i] != 1 || (i != 0 && count[i-1] != 1))) continue;
    if(!uflag && (i != 0 && count[i]+1 == count[i-1])) continue;

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

int compare_str( const void *a, const void *b) {
    char *const *s1 = a;
    char *const *s2 = b;
    return strcmp(*s1, *s2);
}

int main(int argc, char *argv[]) {
  // Get options and filename
  int cflag = 0, iflag = 0, uflag = 0;
  char *input_filename = NULL, *output_filename = NULL;
  if(parse_argument(argc, argv, &cflag, &iflag, &uflag, \
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

  // Sort the contents
  qsort(contents, num_line, sizeof(char*), compare_str);

  // Get count array to perform uniq operation
  int *count = NULL;
  if(get_count(num_line, contents, iflag, &count) == -1) {
    return -1;
  }

  // See how is count array doing
  // for(int i = 0; i < num_line; i++)
  //   printf("%d %s\n", count[i], contents[i]);

  // Output result
  if(output_result(output_file, num_line, contents, count, cflag, uflag) == -1) {
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