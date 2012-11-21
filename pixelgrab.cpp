#include <climits>
#include <cmath>
#define DIM 2
#define COLORDIM 3
#include <Magick++.h>
#include <iostream>

using namespace Magick;
using namespace std;

/*Function declarations*/
Image magickRead(char* name);
void getClusters(Image*, int*, int, double);
void improveCluster(Image*, int*, int);
double getError(int*, int*, int);
int findClosestCluster(ColorRGB color, int* clusterCenters, int numClusters);
double getColorDistance(ColorRGB, ColorRGB);
int run(int, char**);

/*Functions*/
//MAIN!
int main(int argc, char **argv) {
  InitializeMagick(*argv);
  Image image = magickRead("test_wallpaper.jpg");
  int a=0;
  for(a=0;a<20;a++)
    cout << "ahaha" << endl;
  ColorRGB c = image.pixelColor(0,0);
  cout << (double)(c.red()) << endl;
  cout << image.baseRows() << endl;
  cout << image.baseColumns() << endl;
  int* k = (int*) malloc(sizeof(int)*6);
  int i=0;
  for(i=0;i<6;i++)
    *(k+i) = i*2;
  getClusters(&image,k,3,1.0);
  image.write("test_output.png" );
  return 0;
}

//Main clustering functions
void getClusters(Image* image, int* clusterCenters, 
  int numClusters, double tolerance) {
    cout << "getting clusters" << endl;
    double error = 0;
    int* lastClusterCenters = (int*) malloc(sizeof(int)*DIM*numClusters);
    int i;
    for(i=0;i<DIM*numClusters;i++)
      *(lastClusterCenters+i) = *(clusterCenters+i);
    do {
     improveCluster(image, clusterCenters, numClusters);
     error = getError(clusterCenters, lastClusterCenters, numClusters);
    } while(error>tolerance);
}

void improveCluster(Image* image, int* clusterCenters, int numClusters) {
  cout << "improving cluster" << endl;
  int rows = image->baseRows();
  int columns = image->baseColumns();
  //allocate memory for the clustered pixels
  int pixels[rows][columns];
  int i, j, k;
  double minColorDistance;
  double colorDistance;
  int closestCluster;
  //TODO: refactor this into its own function
  //loop over each pixel in the image to classify it
  for(j=0;j<rows;j++) {
    for(i=0;i<columns;i++) {
      minColorDistance=INT_MAX;
      //loop over each cluster point
      //and find the one this pixel is closest to in color
      for(k=0;k<numClusters;k++) {
        colorDistance =
          getColorDistance(image->pixelColor(i,j),
              image->pixelColor(
                *(clusterCenters+DIM*i),
                *(clusterCenters+DIM*i+DIM-1)
                )
              );
        //if this cluster is closer, it is the closest so far
        if(colorDistance<minColorDistance) {
          minColorDistance = colorDistance;
          closestCluster = k;
        } //end if
      } //end for cluster points
      pixels[i][j] = closestCluster;
    } //end for columns
  } //end for rows
  cout << "improveCluster: pixels classified" <<endl;
  //Now we have each pixel classified into clusters.
  //Now to find the center of each cluster.
  //Set each cluster center to 0
  for(i=0;i<DIM*numClusters;i++) {
    *(clusterCenters+i) = 0;
  }
  int* clusterCount = (int*) malloc(sizeof(int)*numClusters);
  for(i=0;i<numClusters;i++)
    *(clusterCount+i) = 0;

  for(i=0;i<rows;i++) {
    for(j=0;j<columns;j++) {
      //The cluster index of each pixel is k
      k = pixels[i][j];
     //Add this pixel to the appropriate cluster average
     *(clusterCenters+k) += i;
     //NOTE: Assumed that the dimension is 2 here.
     *(clusterCenters+k+1) += j;
     *(clusterCount+k) += 1;
    }//end for columns
  } //end for rows
  cout << "improveCluster: totals calculated" << endl;
  //Now actually take the average
  for(i=0;i<numClusters;i++) {
    *(clusterCenters+i) /= *(clusterCount+i);
    *(clusterCenters+i+1) /= *(clusterCount+i);
  }
  //Now clusterCenters has the new clusters!
  return;
}

int findClosestCluster(ColorRGB color, int* clusterCenters, int numClusters);

double getError(int* cluster1, int* cluster2, int numCenters) {
  cout << "getting error" << endl;
  int i;
  double error=0;
  for(i=0;i<numCenters*DIM;i++) {
    error += pow(*(cluster1+i)-*(cluster2+i),2);
  }
  cout << "got error: " << error << endl;
  return error;
}

double getColorDistance(ColorRGB color1, ColorRGB color2) {
  cout << "getting color distance" << endl;
  double distance = pow(color1.red()-color2.red(),2);
  distance += pow(color1.blue()-color2.blue(),2);
  distance += pow(color1.green()-color2.green(),2);
  cout << "distance is " << distance << endl;
  return distance;
}

/*Helper functions*/
Image magickRead(char* name) {
  Image image;
  try {
    image.read(name);
    return image;
  }
  catch (Exception &error_ ) {
    cout << "Caught exception: " << error_.what() << endl;
    exit(1);
  }
}
