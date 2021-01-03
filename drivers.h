#if !defined(DRIVERS)
#define DRIVERS

#if defined(_WIN32)
#include <windows.h>
typedef int(__cdecl* INIT)(void);
typedef int(__cdecl* _IN)(char, int);
typedef int(__cdecl* _OUT)(int);
#elif defined(__linux__)
#include <dlfcn.h>
typedef int (*INIT)(void);
typedef int (*_IN)(char, int);
typedef int (*_OUT)(int);
#endif

typedef struct {
    INIT init;
    _IN input;
    _OUT output;
    int offset;
} Driver;

typedef struct {
    Driver** list;
    int offset;
    int pos;
    int length;
} Drivers;

int load_dll(Driver* driver, const char* filename);
int append_driver(Drivers* drivers, const char* filename);
int find_driver(Drivers* drivers, int pos, int* driver_pos);
void driver_to_tape(int pos, char* tape, Drivers* drivers);
void tape_to_driver(int pos, char* tape, Drivers* drivers);

#endif
