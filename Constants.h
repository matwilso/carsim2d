#pragma once

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

enum {
    TDC_LEFT = 0x1,
    TDC_RIGHT = 0x2,
    TDC_UP = 0x4,
    TDC_DOWN = 0x8
};
