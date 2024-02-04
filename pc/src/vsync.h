#pragma once

typedef struct VSynchronizer_t *VSynchronizer;

void deleteVSynchronizer(VSynchronizer vsync);

VSynchronizer createVSynchronizer(void);

int waitVSync(VSynchronizer vsync);
