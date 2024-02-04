#include "vsync.h"

#include <combaseapi.h>
#include <dxgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct VSynchronizer_t {
    IDXGIOutput *output;
} *VSynchronizer;

void deleteVSynchronizer(VSynchronizer vsync) {
    if (vsync == NULL) {
        return;
    }
    free((void *)vsync);
}

VSynchronizer createVSynchronizer(void) {
    VSynchronizer vsync = (VSynchronizer)malloc(sizeof(struct VSynchronizer_t));
    memset(vsync, 0, sizeof(struct VSynchronizer_t));

    const IID iid = {
        0x7b7166ec,
        0x21c7,
        0x44ae,
        { 0xb2, 0x1a, 0xc9, 0xae, 0x32, 0x1a, 0xe3, 0x69 },
    };

    IDXGIFactory *factory;
    if (FAILED(CreateDXGIFactory(&iid, &factory))) {
        printf("[ warning ] createVSynchronizer(): failed to create a VSynchronizer.\n");
        deleteVSynchronizer(vsync);
        return NULL;
    }

    IDXGIAdapter *adapter;
    if (FAILED(factory->lpVtbl->EnumAdapters(factory, 0, &adapter))) {
        printf("[ warning ] createVSynchronizer(): failed to get an adapter.\n");
        deleteVSynchronizer(vsync);
        return NULL;
    }

    if (FAILED(adapter->lpVtbl->EnumOutputs(adapter, 0, &vsync->output))) {
        printf("[ warning ] createVSynchronizer(): failed to get an output.\n");
        deleteVSynchronizer(vsync);
        return NULL;
    }

    return vsync;
}

int waitVSync(VSynchronizer vsync) {
    if (FAILED(vsync->output->lpVtbl->WaitForVBlank(vsync->output))) {
        printf("[ warning ] waitVSync(): failed to wait for vsync.\n");
        return 0;
    } else {
        return 1;
    }
}
