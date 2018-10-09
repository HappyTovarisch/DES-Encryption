//Jake Bernstein (with credits to Edvin Kuc and David Chou for their help)
//9/4/2018
//Reads and input file, encrypts it (or decrypts it) and prints it to the
//output file

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <rpc/des_crypt.h>
#define bufferSize 80

int main( int argc, char* argv[] ){

  char buffer[ bufferSize ]; //Set size of buffer, make it variable
  char key[8];
  //  char key[] = "abcd1234" For early tests, not useful in final product
  //  const char fname[] = "new.txt"; Also for early testing

  //These three lines of code open the input and output files and makes
  //key something we can use.
  int inputFile = open(argv[2], O_RDONLY | S_IRUSR); //Open input file
  int outputFile = open(argv[3], O_CREAT | O_WRONLY | S_IWUSR, 0666); //Open/create output file
  strcpy(key, argv[1]);

  //Checking for key
  if(sizeof(key) != 8){
    printf("Key is 8 characters long.");
    return 1;
  }

  //printf("Before setparity: %s\n", key);
  des_setparity(key); //Apparently this is important
  //printf("After setparity %s\n", key);

  //Checking for correct input
  if(argc != 5){
    printf("Invalid Input. Enter ./encrypt key 'InputFile' 'OutputFile' 'Mode'\n");
    return 2;
  }

  //Checking for mode
  if((argv[4][0] != '0') && (argv[4][0] != '1')){
      printf("Mode is an integer 0 or 1.\n");
      return 3;
    }

  //Encrypt
  if(argv[4][0] == '0'){
    while(read(inputFile, buffer, bufferSize)){
      if(buffer == 0){
	break;
      }
      else{
	//printf("The encryptions happens now!\n"); Debugging
	ecb_crypt( key, buffer, bufferSize, DES_ENCRYPT);
      }
    }
    printf("Successful encryption!\n");
    write(outputFile, buffer, bufferSize);
  }

  //Decrypt
  if(argv[4][0] == '1'){
    while(read(inputFile, buffer, bufferSize)){
      if(buffer == 0){
	break;
      }
      else{
	ecb_crypt( key, buffer, bufferSize, DES_DECRYPT);
      }
    }
    printf("Successful decryption!\n");
    write(outputFile, buffer, bufferSize);
  }  

  close(inputFile);
  close(outputFile);
  return 0;
}
