#include "bitmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct PixelsGetter_t {
    int left;
    int top;
    int width;
    int height;
    HDC orgDC;
    HDC subDC;
    HBITMAP bitmap;
    uint32_t *pixels;
    unsigned long pixelsSize;
} *PixelsGetter;

void deletePixelsGetter(PixelsGetter pg) {
    if (pg == NULL) {
        return;
    }
    if (pg->pixels != NULL) {
        free((void *)pg->pixels);
    }
    free((void *)pg);
}

PixelsGetter createPixelsGetter(HWND window) {
    PixelsGetter pg = (PixelsGetter)malloc(sizeof(struct PixelsGetter_t));
    memset(pg, 0, sizeof(struct PixelsGetter_t));

    // get the location at screen
    POINT point = { 0, 0 };
    ClientToScreen(window, &point);
    pg->left = point.x;
    pg->top = point.y;

    // get the size
    RECT rect;
    GetClientRect(window, &rect);
    pg->width = rect.right - rect.left;
    pg->height = rect.bottom - rect.top;

    // get the dc of the primary screen
    pg->orgDC = GetDC(NULL);

    // create the compatible dc
    pg->subDC = CreateCompatibleDC(pg->orgDC);

    // create the compatible bitmap and bind it with compatible dc
    pg->bitmap = CreateCompatibleBitmap(pg->orgDC, pg->width, pg->height);
    SelectObject(pg->subDC, pg->bitmap);

    // create pixels array
    pg->pixelsSize = sizeof(uint32_t) * pg->width * pg->height;
    pg->pixels = (uint32_t *)malloc(pg->pixelsSize);
    memset(pg->pixels, 0, pg->pixelsSize);

    return pg;
}

int getWidth(PixelsGetter pg) {
    if (pg != NULL) {
        return pg->width;
    } else {
        printf("[ warning ] getWidth(): tried to get the width of the window before creating a pixels getter.\n");
        return 0;
    }
}

int getHeight(PixelsGetter pg) {
    if (pg != NULL) {
        return pg->height;
    } else {
        printf("[ warning ] getHeight(): tried to get the height of the window before creating a pixels getter.\n");
        return 0;
    }
}

unsigned long getPixelsSize(PixelsGetter pg) {
    if (pg != NULL) {
        return pg->pixelsSize;
    } else {
        printf("[ warning ] getPixelsSize(): tried to get the size of the pixels before creating a pixels getter.\n");
        return 0;
    }
}

uint32_t *getPixels(PixelsGetter pg) {
    // copy pixels
    if (!BitBlt(
            pg->subDC,
            0,
            0,
            pg->width,
            pg->height,
            pg->orgDC,
            pg->left,
            pg->top,
            SRCCOPY
    )) {
        printf("[ warning ] getPixels(): failed to copy pixels.\n");
        return NULL;
    }

    // create a bitmap info
    BITMAPINFO info;
    memset(&info, 0, sizeof(BITMAPINFO));
    info.bmiHeader.biSize = sizeof(info.bmiHeader); 
    info.bmiHeader.biWidth = pg->width;
    info.bmiHeader.biHeight = -pg->height; 
    info.bmiHeader.biPlanes = 1; 
    info.bmiHeader.biBitCount = 32; 
    info.bmiHeader.biCompression = BI_RGB; 

    // get pixels
    if (!GetDIBits(
            pg->orgDC,
            pg->bitmap,
            0,
            pg->height,
            pg->pixels,
            &info,
            DIB_RGB_COLORS
    )) {
        printf("[ warning ] getPixels(): failed to get the pixel data of the window.\n");
        return NULL;
    }

    // finish
    return pg->pixels;
}
