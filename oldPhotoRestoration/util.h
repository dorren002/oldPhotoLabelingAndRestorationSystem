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

//struct networkCfg
//{};
struct UsrCfg
{
    int mask_flag;
    int channel_num;
    string root_dir;
    string input_dir;
    string mask_dir;
    string output_dir;
    string img_format;
    // networkCfg serverCfg;
};