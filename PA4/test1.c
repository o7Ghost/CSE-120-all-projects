#include "aux.h"
#include "umix.h"
#include "mycode4.h"

#define NUMYIELDS 5

static int square, cube; // global variables, shared by threads

void Main ()
{
 int i, t, me;
 void printSquares (), printCubes (), justPrint ();

 MyInitThreads ();

 me = MyGetThread ();
 
 t = MyCreateThread (justPrint, 1);
 t = MyCreateThread (justPrint, 2);
 t = MyCreateThread (justPrint, 3);
 t = MyCreateThread (justPrint, 4);
 t = MyCreateThread (justPrint, 5);
 t = MyCreateThread (justPrint, 6);
 t = MyCreateThread (justPrint, 7);
 t = MyCreateThread (justPrint, 8);
 t = MyCreateThread (justPrint, 9);
 
 t = MyCreateThread (justPrint, 9);
 t = MyCreateThread (justPrint, 9);
 t = MyCreateThread (justPrint, 9);
 t = MyCreateThread (justPrint, 9);
 t = MyCreateThread (justPrint, 9);
 t = MyCreateThread (justPrint, 9);
 t = MyCreateThread (justPrint, 9);

 MyYieldThread(5);

 t = MyCreateThread (justPrint, 11);
 t = MyCreateThread (justPrint, 22);
 t = MyCreateThread (justPrint, 33);
 t = MyCreateThread (justPrint, 44);
 t = MyCreateThread (justPrint, 55);
 t = MyCreateThread (justPrint, 66);
 t = MyCreateThread (justPrint, 77);
 t = MyCreateThread (justPrint, 88);
 t = MyCreateThread (justPrint, 99);

 MyYieldThread(2);

 t = MyCreateThread (justPrint, 111);
 t = MyCreateThread (justPrint, 222);
 t = MyCreateThread (justPrint, 333);
 t = MyCreateThread (justPrint, 444);
 t = MyCreateThread (justPrint, 555);
 t = MyCreateThread (justPrint, 666);
 t = MyCreateThread (justPrint, 777);
 t = MyCreateThread (justPrint, 888);
 t = MyCreateThread (justPrint, 999);

 MyYieldThread(4); 
 
 t = MyCreateThread(justPrint, 1111);
 t = MyCreateThread(justPrint, 2222);
 t = MyCreateThread(justPrint, 3333);
 MySchedThread(); 
 
 t = MyCreateThread(justPrint, 11111);
 t = MyCreateThread(justPrint, 22222);
 t = MyCreateThread(justPrint, 33333);
 MySchedThread(); 
 
 /* Basic Test */
 t = MyCreateThread (printCubes, t);
 t = MyCreateThread (printSquares, t);
 t = MyCreateThread (printCubes, t);
 t = MyCreateThread (printSquares, t);
 t = MyCreateThread (printCubes, t);
 t = MyCreateThread (printSquares, t);
 t = MyCreateThread (printCubes, t);
 t = MyCreateThread (printSquares, t);
 t = MyCreateThread (printCubes, t);
 
 for (i = 0; i < NUMYIELDS; i++) {
  int tmp = MyYieldThread (t);
  Printf ("T%d: square = %d, cube = %d\n", me, square, cube);
 }

 MyExitThread ();
}

void printSquares (t)
 int t;    // thread to yield to
{
 int i;

 for (i = 0; i < NUMYIELDS; i++) {
  square = i * i;
  Printf ("\nT%d: %d squared = %d\n", MyGetThread (), i, square);
  int tmp = MyYieldThread (t);
 }
}

void printCubes (t)
 int t;    // thread to yield to
{
 int i;

 for (i = 0; i < NUMYIELDS; i++) {
  cube = i * i * i;
  Printf ("T%d: %d cubed = %d\n", MyGetThread (), i, cube);
  int tmp = MyYieldThread (t);
 }
}

void justPrint(t)
 int t;
{
 Printf("\n%d with parameter %d\n", MyGetThread(), t);
}
