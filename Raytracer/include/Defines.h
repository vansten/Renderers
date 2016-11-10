#pragma once

#define ORTHO 0
#define ORTHO_SIZE 4.0f


#define BLOCKS 1
#if BLOCKS
#define MULTITHREADED 1
#endif

#if MULTITHREADED
#define PC 1
#endif

#define ANTI_ALIASING 0
#define MAX_STEPS 4

#define USE_CONSOLE 1

#define LIGHTS 1

#define USE_HILBERT_CURVE 1