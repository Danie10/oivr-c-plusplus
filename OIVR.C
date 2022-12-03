
// **PROGRAM WRITTEN IN QUICK BASIC V4.5 BY DF V/D MERWE**
// **WRITTEN V1.0 ON 1991-06-01**
// **UPDATE V2.0 ON 1991-09-09**
// **UPDATE V2.1 ON 1991-10-22**
// **UPDATE V2.2 ON 1992-07-04**  save and retrieve data
// **UPDATE V2.3 ON 1992-07-05**  better modularization, interrupts for monitor detection
//								  interrupts used for file checking
// **Version 2.4 ON 93-09-10  Rewritten in C++ ver 3.1


#include <conio.h>
#include <iostream.h>
#include <fstream.h>
#include <dos.h>
#include <stdio.h>
#include <math.h>

// Declare prototypes for functions
void Alarm(); // Makes alarm noise for errors
void IntroScr(int secs); // Introduction screen
void DrawBox(int startx, int starty, int endx, int endy); // Draws box
void CheckComLine(); // Checks command line arguments
void DispHelp();  // Displays help screens
int  PrintRdy();  // Tests if the printer is ready
void ErrMsg(char Mess1[], char Mess2[]);
int  Prompt(char message[]);//Does a prompt for Y or N, returns true or false
void PrintHeading(); // Prints task name and intro
int  LoadYVals(float X1[],float Y1[]); // Loads X1 and Y1 and returns N number of pairs
int  LoadFile(float X1[],float Y1[]); // Loads X1 and Y1 from file, returns N
void CorrectError(int N, float X1[],float Y1[]); // Corrects incorrect pairs entered
void SaveData(int N, float X1[], float Y1[]); // Save pairs of data to a file
void CalcStart(int N,float X1[],float Y1[],float A[], float B[], float S1[],float S2[],float S3[],float S4[],float C1[],float C2[], int Eqn2, int Eqn3, int Eqn4);
	 // Starts off for each equation, then calls CalcMain to do actual working
void CalcMain(int N, float X[], float Y[], float A[], float B[], float S1[], float S2[], float S3[], float S4[], float C1[], float C2[], int J);
void DoPrintout(int N,float X1[],float Y1[],float A[], float B[], float S1[],float S2[],float S3[],float S4[],float C1[],float C2[], int Eqn2, int Eqn3, int Eqn4);
ofstream prnt("LPT1");  // Points to printer


//**************************************************************
main()
{

int N; // Number of pairs
float *X1 = new float[200]; // Reserve 200 pointers for dynamic memory
float *Y1 = new float[200]; // to add the X1 and Y1 values
float A[4], B[4], S1[4], S2[4], S3[4], S4[4], C1[4], C2[4];
int Eqn2 = 0,Eqn3 = 0,Eqn4 = 0; // These are flags for values <= 0 in CalcStart equations
								// If flag is 1, then it has errors


CheckComLine();  // Checks for command line arguments
IntroScr(3);  // Display intro screen for time secs
if (!PrintRdy())
   { cout << "Printer must be turned on and switched online!\n"; }
PrintHeading(); // Get task name and print intro

if (Prompt("Do you want to load Y(I) values from a saved file? "))
   { N = LoadFile(X1,Y1); }
else
   { N = LoadYVals(X1,Y1); }

if (Prompt("Where there any errors made? "))
   { CorrectError(N,X1,Y1); }  // Correct incorrect pairs


if (Prompt("Do you want to save data pairs to file? "))
	{ SaveData(N,X1,Y1); }

CalcStart(N, X1, Y1, A, B, S1, S2, S3, S4, C1, C2, Eqn2, Eqn3, Eqn4);

DoPrintout(N, X1, Y1, A, B, S1, S2, S3, S4, C1, C2, Eqn2, Eqn3, Eqn4);

cout << "\nCALCULATIONS COMPLETED\n";



prnt.close(); // Close printer
delete [] X1; // Deallocate dynamic arrays
delete [] Y1;
return 0;
}

//**************************************************************
void CheckComLine()
{
char text = _argv[1][1]; // _argv is global variable from DOS.H
						 // for arguments from the command line
if (text =='?')
		// Checks if /? was used to start OIVR
		{ DispHelp(); }  // Executes help


if ((text=='S') || (text=='s'))
		// Checks if /S was used to start OIVR
		{ cout << "Will do setup";   // Executes setup
		delay(1);
		clrscr(); }


return;
}

//*********************************************************************
void PrintHeading()
// Gets task name and prints intro to printer
{
char heading[15];


if (Prompt("Does your task have a name? (Y/N): "))
   { cout << "Enter task name (Max 14 characters): ";
	 cin.get(heading,14);
	 cout << "\n";
	 prnt << "Task name: " << heading << "\n\n"; }
else
	{ prnt << "Task name: None\n\n"; }


cout <<  "THIS PROGRAM CALCULATES A TABLE OF ALTERNATIVE REGRESSION\n";
prnt <<  "THIS PROGRAM CALCULATES A TABLE OF ALTERNATIVE REGRESSION\n";
cout <<  "COEFFICIENTS FOR LINEAR, EXPONENTIAL, LOGARITHMIC AND POWER\n";
prnt <<  "COEFFICIENTS FOR LINEAR, EXPONENTIAL, LOGARITHMIC AND POWER\n";
cout <<  "FUNCTION LEAST SQUARE REGRESSION LINES TO A GIVEN SET OF\n";
prnt <<  "FUNCTION LEAST SQUARE REGRESSION LINES TO A GIVEN SET OF\n";
cout <<  "N PAIRS OF (X(I),Y(I)) VALUES, X IS CONSIDERED THE INDEPENDANT\n";
prnt <<  "N PAIRS OF (X(I),Y(I)) VALUES, X IS CONSIDERED THE INDEPENDANT\n";
cout <<  "VARIABLE\n\n";
prnt <<  "VARIABLE\n\n";
cout <<  "IF A MISTAKE IS MADE WHILST ENTERING DATA, NOTE THE PAIR\n";
cout <<  "NUMBER AND AN OPPORTUNITY WILL BE GIVEN BEFORE THE CALCULATIONS\n";
cout <<  "ARE PERFORMED TO CORRECT DATA FOR THOSE PAIRS.\n\n";
prnt << flush; // Empties print buffer to printer

return;

}
//*********************************************************************

int LoadYVals(float X1[],float Y1[])  // Loads X1 and Y1 and returns N number of pairs

{
int I; // Counter and pair number
int N; // Number of pairs
char NoOfPair[] = "NUMBER OF PAIRS OF X1(I),Y1(I) TO BE ENTERED, N = ";

do {
	cout << NoOfPair;
	cin >> N;
	if (N>200)
		{  ErrMsg("N may not be greater than 200!!!","TRY AGAIN");
		   cout << "\n";
		   continue; }
   }
while (N>200); // Will repeat request if N > 200

prnt << NoOfPair << N << "\n\n";
prnt << flush;  // Empty buffer
clrscr();
cout << NoOfPair << N << "\n\n";
for(I = 0; I < N; I++)
	{ cout << "ENTER PAIR NO  " << (I+1) << "  X(I) Y1(I) WITH SPACE SEPARATING: ";
	  cin >> X1[I] >> Y1[I];
	  if ((X1[I]<0) || (Y1[I]<0)) // If less than 0 it retries
		 { ErrMsg("VALUES MUST BE GREATER THAN 0","PRESS A KEY");
		   I--;  // Decrement I to compensate for the continue
		   continue; // Loops again
		 }
	}



return N;
}

//*************************************************************************
int LoadFile(float X1[],float Y1[])
{
char FileName[16];
int key, N;
char NoOfPair[] = "NUMBER OF PAIRS OF X1(I),Y1(I) TO BE ENTERED, N = ";
ifstream FileIn; // Pointer for file output

do {
cout << "Filename can include drive letter (max 15 characters).\n";
cout << "Enter filename: ";
cin.getline(FileName,16);
FileIn.open(FileName, ios::in); // Opens for input
if (!FileIn) // Checks to make sure file was opened
	{ Alarm();
	  cout << "\nFile " << FileName << " could not be opened - it may not exist.";
	  cout << "\nPress any key to retry or 'Q' to quit operation: ";
	  key = getch();
	  if (key != 'q' && key != 'Q')  // User didn't press Q
		{ continue; }
	  else                           // User pressed Q
		{ break;    }
	}
else
	{ cout << "\nBusy reading from file ";
	  N = 0;
	  while ( FileIn >> X1[N] >> Y1[N] ) // Reads X1 and Y1 values
		  { cout << ".";                 // Displays progress in dots
			N++;         }               // Increments for next pair no


	  cout << "\nFinished reading from file\n";
	  FileIn.close();  // Close file
	  break; // Exit out of loop
	}

}  while ((key != 'q') && (key != 'Q'));

prnt << NoOfPair << N << "\n\n"; //Print to printer
prnt << flush;  // Empty buffer

return N;
}

//*************************************************************************

void CorrectError(int N,float X1[],float Y1[]) // Corrects incorrect pairs entered
{
int EditNr, cont;

do {
	cout << "\nENTER PAIR NO TO EDIT ";
	cin >> EditNr;
	if ((EditNr < 1) || (EditNr > N))
		{ ErrMsg("INVALID PAIR NUMBER","TRY AGAIN");
		  cout << "\n";
		  continue;  // Loop again
		}
	cout << "PAIR NO " << EditNr << " ENTERED AS " << X1[EditNr-1] << " ";
	cout << Y1[EditNr-1] << ":    NEW VALUES -> ";
	cin >> X1[EditNr-1] >> Y1[EditNr-1];
	if ((X1[EditNr-1]<0) || (Y1[EditNr-1]<0)) // If less than 0 it loops
		 { ErrMsg("VALUES MUST BE GREATER THAN 0","PRESS A KEY");
		   continue; // Loops again
		 }
	if (Prompt("Edit another pair? "))
		{  continue; } // Loop for another
	else
		{ cont = 0;    // Breaks out of DO WHILE loop
		  break; }

	}

while (cont > 0);




return;
}

//************************************************************************

void SaveData(int N, float X1[], float Y1[]) // Save pairs of data to a file
{
char FileName[16];
int I, key;
ofstream FileOut; // Pointer for file output

do {
cout << "Filename can include drive letter (max 15 characters).\n";
cout << "Enter filename: ";
cin.getline(FileName,16);
FileOut.open(FileName, ios::out | ios::noreplace); // Opens for output, no overwrite
if (!FileOut) // Checks to make sure file was created
	{ Alarm();
	  cout << "\nFile " << FileName << " could not be created - it may exist already.";
	  cout << "\nPress any key to retry or 'Q' to quit operation: ";
	  key = getch();
	  if (key != 'q' && key != 'Q')  // User didn't press Q
		{ continue; }
	  else                           // User pressed Q
		{ break;    }
	}
else
	{ cout << "\nBusy writing to file......\n";
	  for (I=0; I < N ; I++)
		  { FileOut << X1[I] << " " << Y1[I] << "\n";} // Write pairs to file
	  cout << "Finished writing to file\n";
	  FileOut.close();  // Close file
	  break; // Exit out of loop
	}

}  while ((key != 'q') && (key != 'Q'));


return;
}

//***********************************************************************

void CalcStart(int N,float X1[],float Y1[],float A[], float B[], float S1[],float S2[],float S3[],float S4[],float C1[],float C2[], int Eqn2, int Eqn3, int Eqn4)
// Starts off by copying X1 to array X, and Y1 to array Y
// Then it checks for values <= 0, if none, it calls main calculation function

{

int J;  // Number equals the equation number being worked on
int count;  // Used as a counter
float *X = new float[200]; // Reserve 200 pointers for dynamic memory
float *Y = new float[200]; // to add the X and Y values


// Start with equation 1 and J=0
J = 0;
for (count=0; count < N; count++)
	{ X[count] = X1[count];   // Copies X1 to X array
	  Y[count] = Y1[count];   // Copies Y1 to Y array
	}
CalcMain(N, X, Y, A, B, S1, S2, S3, S4, C1, C2, J); // Do main calculations

// Start with equation 2 and J=1
J = 1;
for (count=0; count < N; count++)
	{ X[count] = X1[count];  // Copies X1 to X array
	  if (Y1[count] <= 0)    // Checks if Y1 is <= 0
		{ Eqn2 = 1;          // If it is then set error flag for Eqn2
		  break;     }  	 // Go straight to equation 3
	  else                   // If Y1 > 0 then performs LOG
		{ Y[count] = log(Y1[count]); } // Copies LOG(Y1) to Y array
		  // Do main calculations
if ( Eqn2 != 1)
		 { CalcMain(N, X, Y, A, B, S1, S2, S3, S4, C1, C2, J);
		   A[1] = exp(A[1]);
		 }
	}

// Start with equation 3 and J=2
J = 2;
for (count=0; count < N; count++)
	{ Y[count] = Y1[count];  // Copies Y1 to Y array
	  if (X1[count] <= 0)    // Checks if X1 is <= 0
		{ Eqn3 = 1;          // If it is then set error flag for Eqn3
		  break;     }  	 // Go straight to equation 4
	  else                   // If X1 > 0 then performs LOG
		{ X[count] = log(X1[count]); }  // Copies LOG(X1) to X array

if ( Eqn3 != 1)
		{
		  // Do main calculations
		  CalcMain(N, X, Y, A, B, S1, S2, S3, S4, C1, C2, J);
		}
	}


// Start with equation 4 and J=3
J=3;
for (count=0; count < N; count++)
	{ if ((X1[count] <= 0) || (Y1[count] <= 0) ) // Checks if X1 or Y1 is <= 0
		{ Eqn4 = 1;          // If it is then set error flag for Eqn4
		  break;     }  	 // Miss main calculations
	  else                   // If X1 and Y1 > 0 then performs LOG
		{ Y[count] = log(Y1[count]);} // Copies LOG(Y1) to Y array
									  // X already LOGed in equation3
if ( Eqn4 != 1)
		  // Do main calculations
		{  CalcMain(N, X, Y, A, B, S1, S2, S3, S4, C1, C2, J);
		   A[3] = exp(A[3]);
		}
	}

// Main calculations done

delete [] X;  // Deallocates dynamic memory
delete [] Y;


return;
}

//**********************************************************************

void CalcMain(int N, float X[], float Y[], float A[], float B[], float S1[], float S2[], float S3[], float S4[], float C1[], float C2[], int J)

{
float Z2 = 0, Z3 = 0, Z4 = 0, Z5 = 0, Z6 = 0, Z7 = 0, Z8 = 0, Z9 = 0;
int count = 0;  // Counter

for (count = 0; count < N; count++)
	{ 	Z5 = Z5 + X[count] * Y[count];
		Z6 = Z6 + X[count] / N;
		Z7 = Z7 + Y[count] / N;
		Z9 = Z9 + pow(X[count],2);
		Z2 = Z2 + pow(Y[count],2);
	}

Z3 = Z6 * N;
Z4 = pow(Z3,2) / N;
Z8 = (Z6 * N) * (Z7 * N);
Z8 /= N;
B[J] = ((Z5 - Z8) / (Z9 - Z4));
A[J] = (Z7 - B[J] * Z6);
S3[J] = B[J] / (Z2 - (pow((Z7 * N),2) / N)) * (Z5 - Z8);
C1[J] = 1;
C1[J] /= (N - 1);
C1[J] *= (Z5 - Z8);
C2[J] = C1[J] / pow(((Z2 - ( pow((Z7 * N),2) / N)) / (N - 1)),.5);
C2[J] = C2[J] / pow(((Z9 - ( pow((Z6 * N),2) / N)) / (N - 1)),.5);

S4[J] = pow(((Z2 - A[J] * Z7 * N - B[J] * Z5) / (N - 2)), .5);
S2[J] = S4[J] /  ( pow( (Z9 - ( pow((Z6 * N),2) ) / N),.5));
S1[J] = pow( Z9/ ( N* ( Z9- pow((Z6*N),2) /N ) ) , .5  ) * S4[J];




return;

}

//***********************************************************************

void DoPrintout(int N,float X1[],float Y1[],float A[], float B[], float S1[],float S2[],float S3[],float S4[],float C1[],float C2[], int Eqn2, int Eqn3, int Eqn4)
{
char *EqnString[4] = { {"A+B*X"},{"A*EXP(B*X)"},{"A+B*LOG(X)"},{"A*X^B"} };
int counter;
float Y6, Y7, Y8, Y9;

prnt << "A              = REGRESSION COEFFICIENT\n";
prnt << "B              = REGRESSION COEFFICIENT\n";
prnt << "STA            = STANDARD ERROR OF REGRESSION COEFFICIENT A\n";
prnt << "STB            = STANDARD ERROR OF REGRESSION COEFFICIENT B\n";
prnt << "SEYONX         = STANDARD ERROR OF ESTIMATE OF Y ON X\n";
prnt << "COEFFDET       = COEFFICIENT OF DETERMINATION\n";
prnt << "COV            = COVARIANCE\n";
prnt << "CORRCOEFF      = CORRELATION COEFFICIENT\n\n";


for (counter=0;counter<4;counter++)
	{ if ((counter==1 && Eqn2==1) || (counter==2 && Eqn3==1) || (counter==3 && Eqn4==1) )
			{ continue; }  // If error in equation then loop to next equation
	  prnt << "Y = " << *(EqnString+counter) << "\tA = " << A[counter] << "\t\t\tB = " << B[counter] << "\n";
	  prnt << "\t\tSTA = " << S1[counter] << "\t\t\tSTB = " << S2[counter] << "\n";
	  prnt << "\t\tSEYONX = " << S4[counter] << "\t\tCOEFFDET = " << S3[counter] << "\n";
	  prnt << "\t\tCOV = " << C1[counter] << "\t\t\tCORRCOEFF = " << C2[counter] << "\n\n";
	}

prnt << "X(I)" << "\tY(I)" << "\tY=A+B*X" << "\t\tY=A*EXP(B*X)" << "\tY=A+B*LOG(X)" << "\tY=A*X^B\n\n";
for (counter =0; counter < N; counter++)
	{ Y6 = A[0] + B[0] * X1[counter];
	  Y7 = A[1] * exp(B[1] * X1[counter]);
	  if (Eqn2 != 1)
		{ Y8 = A[2] + B[2] * log(X1[counter]); }
	  if (Eqn3 != 1)
		{ Y9 = exp(log(A[3]) + B[3] * log(X1[counter])); }
	  prnt << X1[counter] << "\t" << Y1[counter] << "\t" << Y6 << "\t" << Y7 << "\t" << Y8 << "\t" << Y9 << "\n\n";
	}

prnt << "EXPONENTIAL, LOGARITHMIC OR POWER REGRESSIONS WILL NOT BE CALCULATED\n";
prnt << "IF NEGATIVE OR ZERO VALUES APPEAR ON THE DATA, LINEAR REGRESSION WILL\n";
prnt << "OF COURSE ALWAYS BE PERFORMED REGARDLESS OF NEGATIVE VALUES.\n";
prnt << "\f"; // formfeed to printer


return;
}

//*************************************************************************

