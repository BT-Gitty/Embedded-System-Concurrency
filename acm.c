#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// decide what type ACM to use, look in the handout for pseudo-code

char slots[3][200];     // 3 slots; add the size of each slot instead of ...
int w=0, r=1, l=2;      // define and initialise the variables as in the handout

void reader_thread(){
                        // variable declarations/initialisation, if needed

        while(1){       // reader loop; includes a delay to reduce the output data rate
                while (l != r){
                r = l;
                printf ("%s", slots[r]);        // access slot; slots[i] is a pointer to slots[i][0]
                fflush(stdout);                 // (slots[i][0],slots[i][1],... is a 0-terminated string)
                usleep(6000000);                // limit output rate; calculate your own value (>500000)
                }
        }
}

char inp(){             // getchar() wrapper which checks for End Of File EOF
        char c;
        c=getchar();
        if(c==EOF) exit(0);             // exit the whole process if input ends
        return c;
}

int detW() {
  switch (l) {
  case 0:
    switch (r) {
    case 1: return 2;
    case 2: return 1;
    }
  case 1:
    switch (r) {
    case 0: return 2;
    case 2: return 0;
    }
  case 2:
    switch (r) {
    case 0: return 1;
    case 1: return 0;
    }
  }
  return w;
}

int main (){
        pthread_t reader;               // variable declarations/initialisation, if needed
        int j = 0;
        pthread_create(&reader, NULL, reader_thread, NULL);     // create reader thread

        while (1){      // writer loop


// access slot; modify this according to the output format of your game
// in this example I keep reading until '*' appears,
// then the symbol 0 is added, which is the sting terminator.
// The terminator is needed for printf("%s",...) of the reader.

                j = 0;
                while ((slots[w][j++] = inp()) != '*'); // the actual computation takes place inside the condition
                slots[w][j] = 0;                        // append the terminating symbol to the string

                l = w;
                w = detW();
        }

        return 0;
}