#pragma once
#include <string>

using namespace std;

#define VOS_OK 1;
#define VOS_FAIL 0;

#define VOS_TRUE 1;
#define VOS_FALSE 0;

#define TWO_SUM(a,b) (a)+(b);
#define THREE_SUM(a,b,c) (a)+(b)+(c);

struct RGB_POS
{
    double R, G, B;
    int X, Y;
};

struct HSV_POS
{
    double H, S, V;
    int X, Y;
};