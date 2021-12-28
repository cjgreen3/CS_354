#include <stdio.h>
#define N 4
int Is_Number_Correct(int n, int a, int c){
 if(n==a){
  printf("Number %d: Correct. Nice Job.\n", c);
  return 1;
 }

 printf("Number:%d Incorrect.\n", c);
 return 0;
}

void f3helper(int *a);

int f0(){
// return 0x554a0c;
  return(5589516);
}
int f1(){
 int ebp4 = 0x61c3;
 int ebp8 = 0xc882;
 int ebpc = 0x107dc;
 int ebp10 = 0x76c4;
 int ebp14 = 0x31;
 int ebp18 = 0x11;

// int ebp4 = 25027;
// int ebp8 = 51330;
// int ebpc = 67548;
// int ebp10 = 30404;
// int ebp14 = 49;
// int ebp18 = 17;

 int eax =  ebpc;
 int edx =  ebp8;
 eax = eax + edx;
 if(ebp4<=eax) {
  eax = ebp4;
  eax = ebp8 * eax; 
 }
 else{
  ebp4 = ebp4 >> 4;
  eax = ebp4;
  return eax;
 }
 // eax = ebp4*ebp8;
  if(ebp10>=eax ){
 //convert sighed long into signed double long
  eax = ebp10;
  eax = eax /ebp18;
  edx = eax % ebp18;
  ebp10 = eax;
  eax = ebp10;
  return eax;
 // return ebp10 % ebp18;
 }
 else{
  eax = ebpc;
  edx = eax % ebp14;

  eax = eax /ebp14;
  ebpc = edx;
  eax = ebpc;

  return eax;
 }
}
int f2(int i){
 int ebp4 = 0;
 int ebp8 = 0;
 int eax = 0;
 int edx = 0;
 while(ebp4<=27){
  eax = ebp4;
  eax = eax*eax;
  edx = eax;
  eax = i;
  eax = eax + edx;
  ebp8 = eax + ebp8;
  ebp4 = ebp4 + 1;
 }
 eax = ebp8;
 return eax;
}
void f3(int a,int *pa){
 // int*(ip) = pa;
 // &pa = 8(a+6)+a;
 *pa = 9*a +48;

  return;
}
//void f3helper(int *a){
//  int ebp4 = 3;
//  int*eax =a;
//  int edx =*eax;
//  eax = ebp4;
//  edx = eax + edx;
//  eax = &a;
//  eax = edx;
//}

int main() {
    int i;
    int n[N];
    int a[N];
    int c = 0;

    // get numbers from user
    printf("Enter four numbers: ");
    for (i = 0; i<N; i++) scanf("%d", &n[i]);
    printf("\nYou have entered: %d, %d, %d, %d\n",n[0], n[1], n[2], n[3]);
    // get answers
    a[0] = f0(); 
    a[1] = f1();
    a[2] = f2(i);
    a[3] = 4046;  
    f3(a[3], &a[3]);

    // print answers
    // for (i = 0; i< N; i++) printf("answers #%d = %d\n",i,a[i]);

    // test 
    c += Is_Number_Correct(n[0], a[0], 0);
    c += Is_Number_Correct(n[1], a[1], 1);
    c += Is_Number_Correct(n[2], a[2], 2);
    c += Is_Number_Correct(n[3], a[3], 3);

    // report results
    if (c==0) printf("You didn't get any correct numbers. Please try again.\n");
    if (c > 0 && c < N) printf("You got %d correct numbers.  Please try again.\n", c);
    if (c==N) printf("All numbers are correct! Nice work!\n");

    return 0;
}
