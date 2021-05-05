#include <Rcpp.h>
#include <string>

using namespace std;
using namespace Rcpp;
const char baseMap[2][2] = {'0', '1', '2', '3'};

// [[Rcpp::export]]
CharacterVector getTitle(int x, int y, int level)
{
    // check bundary
    if (level < 1)
    {
        Rcerr << "level must be unsigend int" << endl;
        return "error";
    }
    if (x < 1 || x > (1 << level))
    {
        Rcerr << "x and y must be in [1, 2^level]" << endl;
        return "error";
    }

    if (y < 1 || y > (1 << level))
     {
        Rcerr << "x and y must be in [1, 2^level]" << endl;
        return "error";
    }   
    // make index start with 0
    int _x = x - 1;
    int _y = y - 1;
    string title;
    
    for (int i = level; i > 0; i--)
    {
        int temple = 1 << (i-1);
        int _xRage = _x / temple;
        int _yRange = _y / temple;

        title.push_back(baseMap[_xRage][_yRange]);

        _x %= temple;
        _y %= temple;
    }
    return title;
}
