
#include "stats.h"

// Initialize the private vectors so that, for each color, entry
    // (x,y) is the cumulative sum of the the color values in the rectangle from (0,0)
    // to (x,y). Similarly, the sumSq vectors are the cumulative
    // sum of squares in the rectangle from (0,0) to (x,y).
stats::stats(PNG& im) {
    //initialize
    vector<long> column(im.height());
    sumRed.assign(im.width(), column);
    sumGreen.assign(im.width(), column);
    sumBlue.assign(im.width(), column);
    sumsqRed.assign(im.width(), column);
    sumsqGreen.assign(im.width(), column);
    sumsqBlue.assign(im.width(), column);
    int blueSum;
    int redSum;
    int greenSum;
    int blueSumSQ;
    int redSumSQ;
    int greenSumSQ;
    RGBAPixel* pixel;
    for (unsigned long xpos = 0; xpos < im.width(); xpos++) {
        for (unsigned long ypos = 0; ypos < im.height(); ypos++) {
            blueSum = 0;
            redSum = 0;
            greenSum = 0;
            blueSumSQ = 0;
            redSumSQ = 0;
            greenSumSQ = 0;
            for (unsigned long width = 0; width <= xpos; width++) {
                for (unsigned long height = 0; height <= ypos; height++) {
                    pixel = im.getPixel(width,height);
                    blueSum += pixel->b;
                    redSum += pixel->r;
                    greenSum += pixel->g;
                    blueSumSQ += pixel->b * pixel->b;
                    redSumSQ += pixel->r * pixel->r;
                    greenSumSQ += pixel->g * pixel->g;
                }
            }
            sumBlue[xpos][ypos] = blueSum;
            sumRed[xpos][ypos] = redSum;
            sumGreen[xpos][ypos] = greenSum;
            sumsqBlue[xpos][ypos] = blueSumSQ;
            sumsqRed[xpos][ypos] = redSumSQ;
            sumsqGreen[xpos][ypos] = greenSumSQ;
        }
    }
}

stats::stats(PNG& im) {
    //initialize
    vector<long> column(im.height());
    sumRed.assign(im.width(), column);
    sumGreen.assign(im.width(), column);
    sumBlue.assign(im.width(), column);
    sumsqRed.assign(im.width(), column);
    sumsqGreen.assign(im.width(), column);
    sumsqBlue.assign(im.width(), column);
    //previous values
    int blueSum;
    int redSum;
    int greenSum;
    int blueSumSQ;
    int redSumSQ;
    int greenSumSQ;
    //counters for vertical column
    int blueSumC;
    int redSumC;
    int greenSumC;
    int blueSumSQC;
    int redSumSQC;
    int greenSumSQC;
 
    for (unsigned long xpos = 0; xpos < im.width(); xpos++) {
    RGBAPixel* pixel;
    blueSumC = 0;
    redSumC = 0;
    greenSumC = 0;
    blueSumSQC = 0;
    redSumSQC = 0;
    greenSumSQC = 0;
    for (unsigned long ypos = 0; ypos < im.height(); ypos++) {
        if (xpos != 0) {
        blueSum = sumBlue[xpos-1][ypos];
        redSum = sumRed[xpos-1][ypos];
        greenSum = sumGreen[xpos-1][ypos];
        blueSumSQ = sumsqBlue[xpos-1][ypos];
        redSumSQ = sumsqRed[xpos-1][ypos];
        greenSumSQ = sumsqGreen[xpos-1][ypos];
        } else {
        blueSum = 0;
        redSum = 0;
        greenSum = 0;
        blueSumSQ = 0;
        redSumSQ = 0;
        greenSumSQ = 0; 
        }
        pixel = im.getPixel(xpos,ypos);
        blueSumC += pixel->b;
        redSumC += pixel->r;
        greenSumC += pixel->g;
        blueSumSQC += pixel->b * pixel->b;
        redSumSQC += pixel->r * pixel->r;
        greenSumSQC += pixel->g * pixel->g;
        sumBlue[xpos][ypos] = blueSum + blueSumC;
        sumRed[xpos][ypos] = redSum + redSumC;
        sumGreen[xpos][ypos] = greenSum +greenSumC;
        sumsqBlue[xpos][ypos] = blueSumSQ + blueSumSQC;
        sumsqRed[xpos][ypos] = redSumSQ + redSumSQC;
        sumsqGreen[xpos][ypos] = greenSumSQ + greenSumSQC;
        }
    }
}

    /*
    * Returns the sum of all pixel values in the given colour channel in the square defined by ul and dim
     * useful in computing the average of a square
     * PA3 function
     * @param channel is one of r, g, or b
     * @param ul is (x,y) of the upper left corner of the square
     * @param dim is log of side length of the square */
long stats::getSum(char channel, pair<int, int> ul, int dim) {
    int dimension = pow(2, dim) - 1;
    vector<vector<long>> vec;

    if (channel == 'g') {
        vec = sumGreen;
    }
    if (channel == 'r') {
        vec = sumRed;
    }
    if (channel == 'b') {
        vec = sumBlue;
    }
    if (ul.first == 0 && ul.second == 0) 
    return vec[dimension][dimension];

    int x;
    int y;

    if (ul.first == 0){
    y = ul.second - 1;
    return 
    vec[ul.first + dimension][ul.second+dimension]
    - vec[ul.first + dimension][y];
    }

    if (ul.second == 0) {
    x = ul.first - 1;
    return 
    vec[ul.first + dimension][ul.second+dimension]
    - vec[x][ul.second +dimension];
    }

    x = ul.first - 1;
    y = ul.second - 1;
    return 
    vec[ul.first + dimension][ul.second+dimension]
    - vec[x][ul.second +dimension]
    - vec[ul.first + dimension][y]
    + vec[x][y];
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    int dimension = pow(2, dim) - 1;
    vector<vector<long>> vec;

    if (channel == 'g') {
        vec = sumsqGreen;
    }
    if (channel == 'r') {
        vec = sumsqRed;
    }
    if (channel == 'b') {
        vec = sumsqBlue;
    }
     if (ul.first == 0 && ul.second == 0) 
    return vec[dimension][dimension];

    int x;
    int y;

    if (ul.first == 0){
    y = ul.second - 1;
    return 
    (vec[ul.first + dimension][ul.second+dimension]
    - vec[ul.first + dimension][y]);
    }

    if (ul.second == 0) {
    x = ul.first - 1;
    return 
    (vec[ul.first + dimension][ul.second+dimension]
    - vec[x][ul.second +dimension]);
    }

    x = ul.first - 1;
    y = ul.second - 1;
    return 
    (vec[ul.first + dimension][ul.second+dimension]
    - vec[x][ul.second +dimension]
    - vec[ul.first + dimension][y]
    + vec[x][y]);
}

long stats::rectArea(int dim) {
    int dimension = pow(2, dim);
    return dimension * dimension;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    double ret = 0;
    ret += getSumSq('g', ul, dim) - (pow((getSum('g', ul, dim)),2) / rectArea(dim));
    ret += getSumSq('r', ul, dim) - (pow((getSum('r', ul, dim)),2) / rectArea(dim));
    ret += getSumSq('b', ul, dim) - (pow((getSum('b', ul, dim)),2) / rectArea(dim));
    return ret;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    RGBAPixel avg;
    int divideby = rectArea(dim);
    avg.g = getSum('g', ul, dim) / divideby;
    avg.b = getSum('b', ul, dim) / divideby;
    avg.r = getSum('r', ul, dim) / divideby;
    return avg;
}
