#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
  #define EXPORT __declspec(dllexport)
#elif defined(__linux__)
  #define EXPORT __attribute__((visibility("default")))
#else
  #define EXPORT 
#endif

int length = 0;
char hello[] = "Hello World!";
int i;

EXPORT int init(){
    for(i = 0; hello[i] != '\0'; i++);
    length = i;
    i = 0;
    return 2;
}

EXPORT void input(char data, int pos){

}

EXPORT char output(int pos){
    char x;
    switch(pos){
        case 0:
            return (char)(length - i);
        break;
        case 1:
            x = hello[i];
            i++;
            return x;
    }
    return 0;
}
