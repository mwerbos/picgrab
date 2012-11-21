#include <Magick++.h>
#include <iostream>

using namespace Magick;
using namespace std;

/*Function declarations*/
Image magickRead(char* name);
void getClusters(Image, int*, int, double);
void improveCluster(Image, int*, int);
double getError(int*, int*, int);
int findClosestCluster(ColorRGB color, int* clusterCenters, int numClusters);
double getColorDistance(ColorRGB, ColorRGB);
int run(int, char**);
