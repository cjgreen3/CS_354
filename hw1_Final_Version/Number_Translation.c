#include <stdio.h>
#include <math.h>
#include<string.h>
#include<stdlib.h>

#define BINARY_STRING_LENGTH 33
#define HEX_STRING_LENGTH 11

void GetBinaryFromUser(char *binary);
void GetHexFromUser(char *hex);
void GetDecimalFromUser(unsigned int *x);

void ConvertBinaryToDecimal(const char *binary, unsigned int *decimal);
void ConvertHexToDecimal(const char *hex, unsigned int *decimal);
void ConvertDecimalToBinary(const unsigned int decimal, char *binary); 
void ConvertDecimalToHex(const unsigned int decimal, char *hex); 

int main(){
    char binary[BINARY_STRING_LENGTH] = "11011";
    char hex[HEX_STRING_LENGTH] = "0X3F";
	unsigned int decimal = 31;
	
	enum MENU {binary_to_decimal=1, hex_to_decimal, decimal_to_binary, decimal_to_hex};
	while (1) {
		printf("Which conversion would you like to do?\n");
		printf("  %d. Binary to Decimal\n", binary_to_decimal);
		printf("  %d. Hex to Decimal\n", hex_to_decimal);
		printf("  %d. Decimal to Binary\n", decimal_to_binary);
		printf("  %d. Decimal to Hex\n", decimal_to_hex);
		printf("  q to Quit\n");
		int choice;
		if (!scanf("%d", &choice)) return 0;  // exits if user enters something other than a digit
		
		switch (choice) {
			case binary_to_decimal: {
				GetBinaryFromUser(binary); 
				ConvertBinaryToDecimal(binary, &decimal); 
				printf("Binary: %s\nDecimal: %u\n\n", binary, decimal);
			} break;
			case hex_to_decimal: {
				GetHexFromUser(hex); 
				ConvertHexToDecimal(hex, &decimal); 
				printf("Hex: %s\nDecimal: %u\n\n", hex, decimal);
			} break;
			case decimal_to_binary: {
				GetDecimalFromUser(&decimal); 
				ConvertDecimalToBinary(decimal, binary); 
				printf("Decimal: %u\nbinary: %s\n\n", decimal, binary);
			} break;
			case decimal_to_hex: {
				GetDecimalFromUser(&decimal); 
				ConvertDecimalToHex(decimal, hex); 
				printf("Decimal: %u\nHex: %s\n\n", decimal, hex);
			} break;
			default: return 0; // exit if user enters an invalid number
		}
	}
	return 0;
}

void GetBinaryFromUser(char * binary) {
    // write your code here to complete this function
  
     printf("Please enter binary string here: ");
      
     scanf("%s", binary);
    return;
}

void GetHexFromUser(char * hex) {
    // this function has been written for you
    printf("Please enter hex string here (do include the 0x prefix): ");
    scanf("%s", hex);
	for (char *p=hex;*p;p++) if(*p>='a') *p-='a'-'A';  // to upper case
    return;
}

void GetDecimalFromUser(unsigned int *x) {
    // write your code here to complete this function
 printf("Please enter decimal number here: ");
     scanf("%d", x);
    return;
}

void ConvertBinaryToDecimal(const char * binary, unsigned int *decimal) {
    // write your code here to complete this function
   // *decimal = 32; // This hardcoded answer will be incorrect for all but one input. Change this line!
   //https://www.programiz.com/c-programming/examples/binary-decimal-convert
   //I used the inner while loop alogrithm for this function
  int i = 0;
  int rem = 0;
  unsigned int dec = 0;
  int bin = atoi(binary);
  while(bin != 0){
    rem = bin % 10;
    (bin) /= 10;
    dec += rem * pow(2,i);
    ++i;
  } 
  *decimal =  dec;
	return;
}

void ConvertHexToDecimal(const char *hex, unsigned int *decimal) {
 

    // write your code here to complete this function
	// *decimal = 32; // This hardcoded answer will be incorrect for all but one input. Change this line!
   //https://codeforwin.org/2015/09/c-program-to-convert-hexadecimal-to-decimal-number-system.html
  //used alogrithm and converted it to this syntax
  
    long long dec;
    int i = 0, val, len;

    dec = 0;
    len = strlen(hex);
    len--;
    for(i=0; hex[i]!='\0'; i++){
        /* Find the decimal representation of hex[i] */
        if(hex[i]>='0' && hex[i]<='9')
        {
            val = hex[i] - 48;
        }
        else if(hex[i]>='a' && hex[i]<='f')
        {
            val = hex[i] - 97 + 10;
        }
        else if(hex[i]>='A' && hex[i]<='F')
        {
            val = hex[i] - 65 + 10;
        }

        dec += val * pow(16, len);
        len--;
    }


  *decimal =  dec;
	return;
}

void ConvertDecimalToBinary(const unsigned int decimal, char *binary) {
    // write your code here to complete this function
    // do not include leading zeros in the answer
    // if the input is 0 then the output should be just 0
   
   
   // binary = "11101"; // This hardcoded answer will be incorrect for all but one input. Change this line!

  //https://stackoverflow.com/questions/24657952/removing-leading-zeros-in-a-char-array
  //https://stackoverflow.com/questions/48321129/decimal-to-binary-in-c-char-array
  int dec = decimal; 
  char binTemp[33];

        for (int i = 31; i >= 0; i--) 
              {
            if (dec % 2 == 0) 
                  {
                    binTemp[i] = '0';
                  } else 
                  {
                    binTemp[i] = '1';
                  }
            dec =  dec / 2;
               }

      
      int size = 32;
      int i = 0;
      int j = 0; 
     for ( i = 0; i < size - 1; ++i)
    if (binTemp[i] != '0')
        break;


  for (; i < size; i++){
    binary[j] = binTemp[i]; 
    j++; 
    
  }
  binary[j] = '\0'; 

  
return;
}
void ConvertDecimalToHex(const unsigned int decimal, char *hex) {
    // write your code here to complete this function
    // do not include leading zeros in the answer
    // do include the prefix 0x
    // if the input is 0 then the output should be 0x0 
    //hex = "0xF3"; // This hardcoded answer will be incorrect for all but one input. Change this line!
    //https://www.geeksforgeeks.org/program-decimal-hexadecimal-conversion/
    char hexTemp[HEX_STRING_LENGTH]; 
    int i = 0;
    unsigned int dec = decimal;
    while(dec != 0){
      int temp = 0;
      temp = dec % 16;
      if(temp<10){
        hexTemp[i] = temp + 48;
        i++;
      }
      else{
        hexTemp[i] = temp + 55;
        i++;
      }
      dec = dec/16;
    }
    int counter = 2; 
     for(int j=i-1; j>=0; j--){
        //printf("%c",hexTemp[j]); 
        hex[counter] = hexTemp[j]; 
        counter++; 
     }
      hex[counter] = '\0'; 


    return;
}










