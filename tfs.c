#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "tfs.h"
#include "io.h"
#include <fcntl.h>

// initializes the TFS in memory
void init() {

  // first block is root directory
  // - there is free space, and first free block is 01
  tfs[0][0] = 1;
  // - root dir contents and their locations are all 0
  for (int i = 1; i < 16; i++)
    tfs[0][i] = 0;

  // remaining blocks are free
  // each free block points to the next one,
  // except the last block which stores 255 in first byte
  for (int i = 1; i < 16; i++)
    tfs[i][0] = i == 15 ? 0xFF : i + 1;
    // free block data (beyond byte 0) is undefined (don't need to set)
}
// display the TFS as columns of hex digits
void display() {
 
 for (int x=0 ; x < 16; x++)
{
	hexToChar1(x);
  for (int y=0 ; y < 16; y++)
  {
	
	int c = highBits(tfs[x][y]);
	int d = lowBits(tfs[x][y]);

	hexToChar2(c,d);
  }
  prints("\n");
}

}
// TODO: many more functions, to handle other commands

byte highBits(byte a){
	int highBits = a>>4;
	return highBits;
}

byte lowBits(byte a){
	int lowBits = a & 0x0F;
	return lowBits;
}

void import(char arg[], char arg2[]){

int arg2Size = 0;
int dirFound = 0;

int freeBlock = tfs[0][0];

int i = 0;
while (arg2[i] != '\0'){
	if (arg2[i] == '/'){
		++dirFound;
	}
	++i;arg2Size++;
}

	//find directory
		int loc = 0;

		for(int i = 0; i < dirFound - 1; ++i){
			loc = findDirectory(arg2,loc);
		}
		//input name 
		inputName(loc, arg2, arg2Size);
		// input location
		inputLocationPointer(loc);

		// input raw data
		char c;

	int fileSize = 0;

	int b= 1; // start writing data here

	FILE *file;
	file = fopen(arg, "r");
	if (file) {
		while ((c = getc(file)) != EOF){
			++fileSize;
		}
	}
	fclose(file);

if(fileSize > 10){
	
	tfs[freeBlock][b] = tfs[0][0] << 4;
	tfs[0][0]++;
	if(fileSize > 20){
		tfs[freeBlock][b] += tfs[0][0]; tfs[0][0]++;
		}
	b = b + 7;
}
	file = fopen(arg, "r");
	if (file) {
		while ((c = getc(file)) != EOF){
			int a = c;
			tfs[freeBlock][b] = a;
			b++;
		}
	}
		//store filesize
		tfs[freeBlock][0] = fileSize;


	fclose(file);
	prints("Import Successful!");
	println();


}

void mkdir(char arg[]){

int argSize = 0;
int dirFound = 0;
int i = 0;

while (arg[i] != '\0'){
	if (arg[i] == '/'){
		++dirFound;
	}
	++i;argSize++;
}
	// check for / absolute path
	if(dirFound > 0){
		// for each / found go find that and at end return the very last directory location
		int loc = 0;

		for(int i = 0; i < dirFound - 1; ++i){
			loc = findDirectory(arg,loc);
		}

		//find free name spot and input
		inputName(loc, arg, argSize);
		// Find free location spot and input.
		inputLocationPointer(loc);
		
		prints("Directory Succesfully Created\n");

	}else{
			prints("Invalid directory. Remember to put exact path, capital letters only for directory names, and no duplicate names.\n");
		}

	

}


void ls(char arg[]){

int dirFound = 0;
int i = 0;

while (arg[i] != '\0'){
	if (arg[i] == '/'){
		++dirFound;
	}
	++i;
}

	int loc = 0;
	if(dirFound < 0){
		loc = findDirectory(arg,loc);
	}	
	
	int count = 0;
	for(int i = 1; i < 11; i++){
		if(tfs[loc][i] != 00){
			++count;
		}
	}
	if(count > 0){
		printc((char) count + '0'); 
		prints(" Directories and files total.");
		for(i = 0; i < count +1;i++){
		char c =tfs[loc][i];
		printc(c);
		println();println();
		}	
	} else{prints("this directory is empty.\n");}
	

}

void export(char arg[], char arg2[]){

	int loc = 0;
	
	loc = findDirectory(arg,loc);

	

	if(loc == -1){
		prints("directory/file not found\n");
	}else{
		
	char c[100];

		int size = tfs[loc][0];

		int i = 0;

		if(size > 10){
			int n = 8;
			while(i < size){
				printf("%i",loc);
				c[i] = tfs[loc][n];
				++n;
				++i;
				if (n > 15){
					loc++;
					n=0;
				}
			}
			
		}else{
			int n = 1;
			while (i < size){
			c[i] = tfs[loc][n];
			++n;
			++i;
			}
			
		}

		int filePtr = open(arg2,O_WRONLY|O_CREAT,0755);


		write(filePtr, c, size);


		close(filePtr);
	}


	}

void inputName(int loc, char arg[], int argSize){
	
		int n = 1;
		while (tfs[loc][n] != 00){
			n++;
		}
		if(tfs[loc][n] == 00){
			tfs[loc][n] = arg[argSize-1];
		}

}

void inputLocationPointer(int loc){
int q = 11;

	while(highBits(tfs[loc][q]) != 0 && lowBits(tfs[loc][q]) != 0){
			++q;
		}
		if(highBits(tfs[loc][q]) == 0){
			tfs[loc][q] = tfs[0][0] << 4;
		} else{
			tfs[loc][q] += tfs[0][0];
		}


		int freeBlock = tfs[0][0];
		tfs[freeBlock][0] = tfs[0][0] << 4;

		tfs[0][0]++;

}


int findDirectory(char arg[], int loc){

/*
int i = 0;
int dirFound =0;
int dirIndexs[10];

while (arg[i] != '\0'){
	if (arg[i] == '/'){
		dirIndexs[dirFound] = i;
		++dirFound;
	
	}
	++i;
}

//we running this code as many times as there are dirs. tihs code starts from root every time.then follows. 
//example find /b/c/d assume its good
if(arg[0] != '/'){prints("invalid path remember to include /.");goto notFound;}

loc = 0;
int d =0;

// run as many times as there is dirs 
// example /a/b/c run three time, find a, find b in a, find c in b
for(i = 0; i < dirFound;i++){
	
	int tLoc = 11;
	int l = 1;
		while(tfs[loc][l] != arg[dirIndexs[d] + 1])
		{
			++l;
			if( l > 16){goto notFound;
			}
		}
		//nameFound = at l;
		//now go to that names location
		//brute force find that loc from name i loc
		while(highBits(tfs[loc][tLoc]) != l && lowBits(tfs[tLoc][tLoc]) != l){
			++tLoc;
			if(tLoc > 16){goto notFound;
			prints("country");}
		}
		++d;
		if(highBits(tfs[loc][tLoc]) == l){
			loc = highBits(tfs[loc][tLoc]);
		}else{
			loc = lowBits(tfs[loc][tLoc]);
		}
}

return loc;

notFound:
prints("not found");
return loc = -1;
*/


int argSize = 0;
int dirFound = 0;
int dirIndexs[10];
int dirBlockLocation;

int b = 1;
int d = 1;
int j = 11;
int i = 0;



while (arg[i] != '\0'){
	if (arg[i] == '/'){
		dirIndexs[dirFound] = i;
		++dirFound;
	
	}
	++i;argSize++;
}

		if(dirFound == 1){
			dirIndexs[1] = 2;
		}

		//find the directory
		while (tfs[loc][b] != arg[dirIndexs[d] - 1]){
			++b;
			if(b > 11){goto notFound;}
		}
		if (tfs[loc][b] == arg[dirIndexs[d] - 1]){
			// directory exists
			// b = block location of dir 
				

			while(highBits(tfs[loc][j]) != b && lowBits(tfs[loc][j]) != b){
				++j;
			}
			if (highBits(tfs[loc][j]) == b){
				dirBlockLocation = highBits(tfs[loc][j]);
			}
			if (lowBits(tfs[loc][j]) == b)
			{
				dirBlockLocation = lowBits(tfs[loc][j]);
			}
			}
		
return dirBlockLocation;

notFound:
return dirBlockLocation = -1;
}
	

void hexToChar1(int x){

	char a;
	if (x <= 9){
	a = (char) x + '0';
	}else{
		int i = 0;
		while (x > 9){
			a = 'A' + i;
			x--;
			i++;
		}
	}
	printc(a);
	prints("x: ");
}

void hexToChar2(int c, int d){
	
	char a;
	char b;

	if (c <= 9){
	a = (char)c + '0';
	}
	else{
		int i = 0;
		while(c > 9){
			a = 'A' + i;
			c--;
			i++;
		}
	}
	
	if (d <= 9){
	b = (char)d + '0';
	}
	else{
		int i = 0;
		while(d > 9){
			b = 'A' + i;
			d--;
			i++;
		}
	}
	
	printc(a);
	printc(b);
	printc(' ');

}


void decimalToHex(int a){

// char array to store
	// hexadecimal number
	char hexaDeciNum[100];


			// counter for hexadecimal number array
			int i = 0;
			while (a != 0) {
			// temporary variable to
			// store remainder
			
			int temp = 0;
			// storing remainder in temp
			// variable.
			temp = a % 16;
		
			// check if temp < 10
			if (temp < 10) {
				hexaDeciNum[i] = (char)(temp + 48);
				i++;
			}
			else {
				hexaDeciNum[i] = (char)(temp + 55);
				i++;
			}
		
			a = a / 16;
			
			}
			
			// print hexDeciNums in reverse for final hex answer
			for (int j = i - 1; j >= 0; j--){
			printc(hexaDeciNum[j]);
			}

}
