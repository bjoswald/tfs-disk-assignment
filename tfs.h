// defines the "byte" type as an unsigned 8-bit integer, valued 0 to 255
typedef unsigned char byte;

// this is the TFS currently in memory, globally available to all functions
// the first index is that of the 16 blocks of the TFS
// the second index is that of the 16 bytes within a block
byte tfs[16][16];

int rootNameHolderLoc;
int rootLocationHolder;


void init();

void display();

// TODO: many more functions

//commands

void import(char arg1[], char arg2[]);

void mkdir(char arg[]);

void export(char arg[], char arg2[]);

void ls(char arg[]);

//helpers


int findDirectory(char arg[], int startingLocation);

void inputName(int loc, char arg[], int argSize);

void inputLocationPointer(int loc);

byte lowBits(byte a);

byte highBits(byte a);

void hexToChar1(int x);

void hexToChar2(int c, int d);

void decimalToHex(int a);

char getLocation();