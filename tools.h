#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <cmath>
#include <tgmath.h>
#include <iomanip>
#include <algorithm>
#include <iomanip> 
#include <string.h>//strtok from C
//#define INF 999999999

int Round(double r);
double parseFloat(const std::string& input);	//string -> double
int parseInt(const std::string& input);	//string -> int
std::vector<std::string> split(const std::string &s, const std::string &seperator);	//split string into vector
int getdir (std::string dir, std::vector<std::string> &files);
double nodeRoadDistance(double nx, double ny, double rx1, double ry1, double rx2, double ry2);	//Distance from a node to a road
void GetFootOfPerpendicular(double nx, double ny, double rx1, double ry1, double rx2, double ry2, double& x, double& y);
bool nodeOnRoad(double nx, double ny, double rx1, double ry1, double rx2, double ry2);

int findCost(std::vector<int> &vCostT, std::vector<int> &vCostC, int t);
int hasIntersection(int x11, int y11, int x12, int y12, int x21, int y21, int x22, int y22, int& x, int& y);
double direction(std::pair<int, int> pi, std::pair<int, int> pj, std::pair<int, int> pk);
bool onSegment(std::pair<int, int> pi, std::pair<int, int> pj, std::pair<int, int> pk);
bool lineCoincide(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p3, std::pair<int, int> p4, std::pair<int, int>& pr1, std::pair<int, int>& pr2);
double lineGradient(int x1, int y1, int x2, int y2);
double min(double x, double y);
double max(double x, double y);

int hasIntersectionDouble(double x11, double y11, double x12, double y12, double x21, double y21, double x22, double y22, double& x, double& y);
double direction(std::pair<double,double> pi, std::pair<double, double> pj, std::pair<double, double> pk);
bool findFile(std::string filename);
void strToVec(std::vector<int> &key, std::string &ss);
#endif
