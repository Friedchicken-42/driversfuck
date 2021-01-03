#include "drivers.h"

#include <stdio.h>
#include <stdlib.h>

int load_dll(Driver* driver, const char* filename) {
#if defined(_WIN32)
    HMODULE hDll = LoadLibrary(filename);

    if (hDll == NULL) return 1;

    driver->init = (INIT)GetProcAddress(hDll, "init");
    driver->input = (_IN)GetProcAddress(hDll, "input");
    driver->output = (_OUT)GetProcAddress(hDll, "output");

#elif defined(__linux__)

    void* so = dlopen(filename, RTLD_NOW);

    if (so == NULL) return 1;

    driver->init = (INIT)dlsym(so, "init");
    driver->input = (_IN)dlsym(so, "input");
    driver->output = (_OUT)dlsym(so, "output");
#endif
    if (!driver->init || !driver->input || !driver->output) return 2;

    return 0;
}

int append_driver(Drivers* drivers, const char* filename) {
    Driver* driver = (Driver*)malloc(sizeof(Driver));
    int status = load_dll(driver, filename);
    if (status) {
        free(driver);
        return -1;
    }

    if (drivers->pos >= drivers->length) {
        drivers->length *= 2;
        drivers->list = (Driver**)realloc(drivers->list, drivers->length);
    }

    driver->offset = driver->init();
    drivers->offset += driver->offset;
    drivers->list[drivers->pos] = driver;
    drivers->pos++;

    return 0;
}

int find_driver(Drivers* drivers, int pos, int* driver_pos) {
    int offset = 0;
    int i;
    for (i = 0; i < drivers->pos; i++) {
        if (offset + drivers->list[i]->offset > pos) {
            break;
        }
        offset += drivers->list[i]->offset;
    }

    if (i == drivers->pos) {
        return -1;
    }

    return offset;
}

void driver_to_tape(int pos, char* tape, Drivers* drivers) {
    int driver_pos = 0;
    int offset = find_driver(drivers, pos, &driver_pos);

    if (offset == -1) {
        tape[pos] = getchar();
    } else {
        char x = drivers->list[driver_pos]->output(pos - offset);
        tape[pos] = x;
    }
}

void tape_to_driver(int pos, char* tape, Drivers* drivers) {
    int driver_pos = 0;
    int offset = find_driver(drivers, pos, &driver_pos);

    if (offset == -1) {
        fprintf(stderr, "%c", tape[pos]);
    } else {
        drivers->list[driver_pos]->input(tape[pos], pos - offset);
    }
}
