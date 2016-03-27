#ifndef __ROUTE_H__
#define __ROUTE_H__
#include <vector>
#include <set>
#include <iostream>
/*
#define START_PHENO 100
#define ALPHA 1
#define BETA 2
#define START_MUL 1
#define Q_PHENO 100.0
#define Q_DIST 400.0
#define PHENO_DECREASE_AFTER 0.3
#define PHENO_DECREASE_BEFORE 0.9
*/
using namespace std;

struct ConstSeg {
    double START_PHENO,ALPHA,BETA,START_MUL,Q_PHENO,Q_DIST,PHENO_DECREASE_BEFORE,PHENO_DECREASE_AFTER;
};

class DirectedEdge {
public:
    DirectedEdge() {
        this->exist = false;
    }
    DirectedEdge(int destNode, int edgeNum, int edgeCost);
    void printEdge(int sourceNode) {
        cout << sourceNode << "," << this->destNode << "," << this->edgeNum << "," << this->edgeCost << endl;
    }
    ~DirectedEdge(){}
    int destNode;
    int edgeNum;
    int edgeCost;
    float pheno;
    bool exist;
};

class Ant {
public:
    Ant(const int sourcePosition,const int destPosition) {
        _position=sourcePosition;
        _destPosition=destPosition;
        _visitedEdge.clear();
        _visitedPoint.clear();
        _visitedPoint.insert(sourcePosition);
        _cost=0;
        arrived=false;
    }
    ~Ant() {}
    void travel();
    void update();
    static void releasePheno();
    static void printPheno();
    static vector<vector<DirectedEdge> > originMap;
    static set<int> demandSet;
    static vector<DirectedEdge*> currentAnswer;
    static int currentCost;
    bool arrived;
private:
    int _position,_destPosition;
    set<int> _visitedPoint;
    vector<DirectedEdge*> _visitedEdge;
    DirectedEdge *selectEdge();
    int _cost;
};
void search_route(char *graph[5000], int edge_num, char *condition);

#endif