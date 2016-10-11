#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include "kmeans.h"
#include "RLE.h"
#include "ScanLines.h"
#include "Vec.h"
#include "blackflycamera.h"
#include "types.h"
#include "ros/ros.h"
#include "minho_team_ros/mirrorConfig.h"
#include "minho_team_ros/visionHSVConfig.h"
#include <iostream>

using namespace std;
using minho_team_ros::mirrorConfig;
using minho_team_ros::visionHSVConfig;

class ImageProcessor
{
public:
    ImageProcessor(bool begin);

    /* Detection Functions */
    void detectInterestPoints(); // Detects linePoints, ballPoints and obstaclePoints
    void detectBallPosition(); // Given the detected ballRLE find the optimal candidate

    /* Image Output Functions */
    Mat *getImage(bool *success); // Returns camera image pointer
    Mat *getOriginal(); // Returns original (clean camera image) pointer
    Mat *readStaticImage(); // RetuFucksrns image read from static system path
    void getBinary(Mat *in,  labelConfiguration labelconf); // Returns thresholded HSV image
    void getSegmentedImage(Mat *buffer); // Returns buffer's segmented image
    void paintPixel(int x, int y, int classifier, Mat *buf); // Paints a certain pixel in the image
    hsv rgbtohsv(rgb in); // Converts rgb to hsv

    /* Camera driver wrapping functions */
    bool frameAvailable(); // Returns true if a new camera frame is available
    bool isReady(); // Returns true if camera is ready
    bool startCamera(); // Returns true if camera is successfuly started
    bool startImaging(); // Returns true if imaging was successfuly started
    void printCameraInfo(); // Prints in stdout some camera information
    void closeCamera(); // Closes camera feed

    /* Look up Table fuctions */
    void updateLookUpTable(Mat* buffer, int x, int y, int label, int rad); // Updates LUT cell value
    bool readLookUpTable(); // Reads look up table from file
    bool writeLookUpTable(); // Writes look up table to file
    void resetLookUpTable(); // Resets defined look up table
    void generateLookUpTable(); // Generates look up table based on values' ranges
    void updateLabelLutConf(LABEL_t label,labelConfiguration lb_conf);
    int getClassifier(int x,int y); // Returns classifier given a pixel and LUT configuration

    /* Camera-Robot Information */
    void setCenter(int x,int y); // Sets robot center
    void setBuffer(Mat *buf); // Sets current buffer
    double getRobotHeight(); // Returns robot height
    Point getCenter(); // Returns robot center
    void setStaticImagePath(QString pth);// Sets path for static image

    /* Mathmatic Functions */
    int d2p(Point p1,Point p2); // Returns distance between two points
    double d2pWorld(int pixeis); // Returns world distance given pixel distance
    Point2d worldMapping(Point p); // Maps point to world (world dist, angle)

    /* Initializations */
    void variablesInitialization(); // Inits other variables
    bool initWorldMapping(); // Inits world mapping variables
    void generateMirrorConfiguration();
    bool writeMirrorConfig();
    void updateDists(double max, double step, vector<short unsigned int>pix_dists);
    mirrorConfig getMirrorConfAsMsg();
    visionHSVConfig getVisionConfAsMsg();
    bool initializeBasics();
    QString getField();
    
    /* WORLD STATE INFORMATION Buffers */
    vector<Point3d>ballCandidates;
    vector<Point>linePoints;
    vector<Point>obstaclePoints;
    vector<Point>ballCentroids;
    vector<Point>ballPoints;
    
private:
    /*Image Containers */
    Mat *processed, *buffer;
    Mat original, balls, mask, staticImg;

    /* Camera Driver */
    BlackFlyCamera *topCam;
    int centerX, centerY;

    /* Other Variables and Lut Variables*/
    int YUVLookUpTable[256*256*256];
    lutConfiguration lutconfig;
    
    /* Vision Variables */
    Mat element;
    double sizeRelThreshold, piThreshold;
    double MAX_DISTANCE,STEP;
    QString staticImgPath;

    /* World Mapping */
    double robotHeight;
    vector<vector<Point2d> >distLookUpTable;
    vector<double> distPix;
    vector<double> distReal;
    vector<double> ballPix;
    vector<double> ballReal;

    // Implementation USING RLE
    void rleModInitialization(); // Inits RLE Mode sensors and RLE's
    void preProcessIdx(); // Preprocesses image, labeling it for RLE scan
    ScanLines linesRad, linesCir;
    Mat idxImage;
    RLE rleBallRad,rleLinesRad,rleObs,rleBallCir,rleLinesCir;
    
    //ConfigFiles strings
    QString field;
    QString agent;
    QString mirrorParamsPath, imageParamsPath ,maskPath,lutPath;
};

#endif // IMAGEPROCESSOR_H
