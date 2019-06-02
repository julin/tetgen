#include "tetgenext.h"

int  ConvexHullWithEdges(
  int npoint, std::function<void(int, bool, double*)> pointFun,
  int nedgePoint, int nedge, std::function<void(int, int*)> edgeFun,
  std::function<void(int, const double*)> pointOut,
  std::function<void(int, const int*)> elemOut)
{

  tetgenio::gPrintFun = MyPrintFun;

  std::vector<std::array<int,2> > edgesNew;
  auto  ImportMesh = [&npoint, &nedgePoint, &nedge, &edgeFun, &pointFun, &edgesNew](tetgenio& out) {
    out.numberofpoints = npoint + nedgePoint;
    out.pointlist = new REAL[out.numberofpoints * 3];
    for (int i = 0; i < npoint; i++) {
      pointFun(i, false, out.pointlist + i * 3);
    }
    int index0 = npoint * 3;
    for (int i = 0; i < nedgePoint; i++) {
      pointFun(i, true, out.pointlist + (index0 + i * 3));
    }

    out.numberofedges = nedge;
    out.edgelist = new int[out.numberofedges * 2];
    int edgeNode[2];
    edgesNew.resize(nedge);
    for (int i = 0; i < nedge; i++) {
      edgeFun(i, edgeNode);
      edgesNew[i][0] = edgeNode[0] + npoint;
      edgesNew[i][1] = edgeNode[1] + npoint;
      out.edgelist[i * 2] = edgesNew[i][0];
      out.edgelist[i * 2 + 1] = edgesNew[i][1];
    }
  };

  auto ExportMesh = [&pointOut, &elemOut, &edgesNew](const tetgenio& out) {
    for (int i = 0; i < out.numberofpoints; i++) {
      pointOut(i, out.pointlist + i * 3);
    }

    for (int i = 0; i < out.numberoftetrahedra; i++) {
      elemOut(304, out.tetrahedronlist + i * 4);
    }

    for (size_t i = 0; i < edgesNew.size(); i++) {
      elemOut(102, edgesNew[i].data());
    }
  };

  auto CheckRecoveryed = [&edgesNew](const tetgenio& in) {
    /*
    int index = 0;
    DInt2Array tetEdges;

    tetEdges.reserve(in.numberoftetrahedra * 6);
    for (size_t i = 0; i < in.numberoftetrahedra; i++) {
      auto atet = in.tetrahedronlist + i * 4;
      for (int j = 0; j < 6; j++) {
        tetEdges.push_back(MEXT_NS::Int2(atet[TET_EDGE_VERTEX[j][0]], atet[TET_EDGE_VERTEX[j][1]]));
      }
    }

    Int2HArray tetEdgeProxy(tetEdges);
    for (auto it : edgesNew) {
      int pos = tetEdgeProxy.find(it);
      if (pos < 0)
        return false;
    }
    */
    return true;
  };

  tetgenio in, out;
  {
    ImportMesh(in);
    int nnode0 = in.numberofpoints;
    tetgenbehavior b;
    //ScopedRedirect stdRedect;
    b.parse_commandline((char*)"YQpcCC");
    tetrahedralize(&b, &in, &out);
    if (!CheckRecoveryed(out)) {
      return -1;
    }
  }
  ExportMesh(out);
  return 0;
}

