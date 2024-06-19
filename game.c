#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>

#define width  40
#define height 18
#define buf_length (width*height)
#define fox_init_x (width*3/4)
#define fox_init_y (height*3/4)
#define fox_init_dir ('l')
#define rabbit_init_x (width/4)
#define rabbit_init_y (height/4)
#define rabbit_init_dir ('u')


//---- set keyboard mode -- please copy and use this function
struct termios tty_prepare(){
        struct termios tty_attr_old, tty_attr;
        tcgetattr (0, &tty_attr);
        tty_attr_old = tty_attr;
        tty_attr.c_lflag &= ~(ECHO | ICANON);
        tty_attr.c_cc[VMIN] = 1;
        tcsetattr (0, TCSAFLUSH, &tty_attr);
        return tty_attr_old;
}

//---- restore keyboard mode -- please copy and use this function
void tty_restore(struct termios tty_attr){
        tcsetattr (0, TCSAFLUSH, &tty_attr);
}

//---- fox direction
char fox_dir = fox_init_dir;

//---- keyboard thread function
void keys_thread(){
// Use getchar() to read the keyboard
        while(1){
                if (getchar() == 27){
                        getchar();

// Each arrow key generates a sequence of 3 symbols
// the first two are always 0x1b and 0x5b
// the third is
// 0x41 -- up
// 0x42 -- down
// 0x43 -- right
// 0x44 -- left
// Update the global variable fox_dir appropriately

                        switch(getchar()){
                                case 65: fox_dir = 'u'; break;
                                case 68: fox_dir = 'l'; break;
                                case 66: fox_dir = 'd'; break;
                                case 67: fox_dir = 'r'; break;
                        }
                }
        }
}

//---- update x and y coord-s according to direction; used in main()
void update_coord (int *x_ptr, int *y_ptr, char dir){ // call by reference to x and y
        switch (dir){
                case 'u': if (*y_ptr > 1) (*y_ptr)--; break;    // *y_ptr is called "dereference",
                                                                // which is the target pointed at by the pointer
                case 'l': if (*x_ptr > 1) (*x_ptr)--; break;

                case 'd': if (*y_ptr < height) (*y_ptr)++; break;

                case 'r': if (*x_ptr < width) (*x_ptr)++; break;
    }
}

//---- the program starts its execution from here
int main (){
        // variable declarations and initialisation
        pthread_t keys_th;
        int fox_x = fox_init_x;
        int fox_y = fox_init_y;
        char rabbit_dir = rabbit_init_dir;
        int rabbit_x = rabbit_init_x;
        int rabbit_y = rabbit_init_y;
        int random = 0;

        struct termios term_back = tty_prepare();

        pthread_create(&keys_th, NULL, keys_thread, NULL); // create the keyboard thread

        while (1){
                usleep (500000);

                update_coord (&fox_x, &fox_y, fox_dir);

                random = rand() % 4; // generate the rabbit direction at random
                switch (random){
                        case 0: rabbit_dir = 'u'; break;
                        case 1: rabbit_dir = 'r'; break;
                        case 2: rabbit_dir = 'd'; break;
                        case 3: rabbit_dir = 'l'; break;
                }

                update_coord (&rabbit_x, &rabbit_y, rabbit_dir);

                printf ("\033[2J\033[%d;%dH@\033[%d;%dH*", fox_y, fox_x, rabbit_y, rabbit_x);
                fflush (stdout);

                if (fox_y == rabbit_y && fox_x == rabbit_x) break; // add the condition(s) of game termination
        }

        pthread_cancel(keys_th);
        tty_restore(term_back);

        return 0;
}