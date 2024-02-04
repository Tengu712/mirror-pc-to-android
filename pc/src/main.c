#include "bitmap.h"
#include "socket.h"
#include "vsync.h"
#include "window.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>

int main() {
    // create a communicator
    Communicator com = createCommunicator(63200);
    if (com == NULL) {
        printf("[ error ] main(): failed to create a communicator.\n");
        return 1;
    }

    // show the connected status
    printf("[ info ] main(): connected.\n");

    // get a pid from user input
    DWORD pid;
    printf("PID: ");
    scanf_s("%lu", &pid);

    // get a window from user input pid
    const HWND window = getWindowHandleFromProcessID(pid);
    if (window == NULL) {
        printf("[ error ] main(): failed to get the window handle: PID=%lu\n", pid);
        deleteCommunicator(com);
        return 1;
    }

    // create a pixels getter
    PixelsGetter pg = createPixelsGetter(window);
    if (pg == NULL) {
        printf("[ error ] main(): failed to create a pixels getter.\n");
        deleteCommunicator(com);
        return 1;
    }

    // show the size of the window
    const int width = getWidth(pg);
    const int height = getHeight(pg);
    printf("[ info ] window width  = %d\n", width);
    printf("[ info ] window height = %d\n", height);

    // get previously the size of pixels
    const unsigned long pixelsSize = getPixelsSize(pg);
    printf("[ info ] pixels size = %lu\n", pixelsSize);

    // send the size of the window
    const int bufSizeOfWindow[2] = { width, height };
    if (!sendData(com, (const char *)bufSizeOfWindow, sizeof(bufSizeOfWindow))) {
        printf("[ error ] main(): failed to send the size of the window.\n");
        deletePixelsGetter(pg);
        deleteCommunicator(com);
        return 1;
    }

    // create a vsynchronizer
    VSynchronizer vsync = createVSynchronizer();
    if (vsync == NULL) {
        printf("[ error ] main(): failed to create a VSynchronizer.\n");
        deletePixelsGetter(pg);
        deleteCommunicator(com);
        return 1;
    }

    // get a pid from user input
    unsigned int interval;
    printf("frame interval: ");
    scanf_s("%u", &interval);
    if (interval == 0) {
        printf("[ error ] main(): interval must be greater than 0: %u\n", interval);
        deleteVSynchronizer(vsync);
        deletePixelsGetter(pg);
        deleteCommunicator(com);
        return 1;
    }

    // show the start status
    printf("[ info ] main(): start.\n");

    // loop
    unsigned int count = 0;
    while (1) {
        // wait vsync
        waitVSync(vsync);

        // skip
        if ((count % interval) != 0) {
            count += 1;
            continue;
        }
        count += 1;

        // check if the window closed
        if (!IsWindow(window)) {
            break;
        }

        // get pixels
        const uint32_t *pixels = getPixels(pg);
        if (pixels == NULL) {
            printf("[ warning ] main(): failed to get pixels.\n");
            continue;
        }

        // send pixels
        if (!sendData(com, (const char *)pixels, pixelsSize)) {
            printf("[ error ] main(): failed to send pixels.\n");
            break;
        }
    }

    // show the end status
    printf("[ info ] main(): end.\n");

    // finish
    deleteVSynchronizer(vsync);
    deletePixelsGetter(pg);
    deleteCommunicator(com);
    return 0;
}
