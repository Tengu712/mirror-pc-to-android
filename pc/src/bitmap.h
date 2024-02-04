#pragma once

#include <stdint.h>
#include <Windows.h>

typedef struct PixelsGetter_t *PixelsGetter;

void deletePixelsGetter(PixelsGetter pg);

PixelsGetter createPixelsGetter(HWND window);

int getWidth(PixelsGetter pg);

int getHeight(PixelsGetter pg);

unsigned long getPixelsSize(PixelsGetter pg);

uint32_t *getPixels(PixelsGetter pg);
