#pragma once

#include<functional>
#include <vector>
#include "tetgen.h"
#include <array>

int  ConvexHullWithEdges(
  int npoint, std::function<void(int, bool, double*)> pointFun,
  int nedgePoint, int nedge, std::function<void(int, int*)> edgeFun,
  std::function<void(int, const double*)> pointOut,
  std::function<void(int, const int*)> elemOut);
