#include <stdio.h>

int* f1(int *val){
  int *address = val;
  return address;
}

int main(){
  int val = 5;
  int *address = f1(&val);
  printf("Value %d is at %p\n", *address, (void*)address);
  return 0;
}
