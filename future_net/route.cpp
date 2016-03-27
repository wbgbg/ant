#include "route.h"
#include "lib/lib_record.h"
#include "lib/util.cpp"
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <iterator>
#include <set>
#include <deque>
#include <string>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <complex>
#include <cmath>
#include <fstream>
#include <exception>
using namespace std;
//你要完成的功能总入口

vector<vector<DirectedEdge> > Ant::originMap(601);
set<int> Ant::demandSet;
vector<DirectedEdge *> Ant::currentAnswer;
int Ant::currentCost;
ConstSeg constSeg;
DirectedEdge::DirectedEdge(int destNode, int edgeNum, int edgeCost) {
    this->destNode = destNode;
    this->edgeNum = edgeNum;
    this->edgeCost = edgeCost;
    if (Ant::demandSet.find(destNode)!=Ant::demandSet.end()) {
        this->pheno = constSeg.START_PHENO*constSeg.START_MUL;
    } else {
        this->pheno = constSeg.START_PHENO;
    }
    this->exist = true;
}
int tooAlpha=0,tooBeta=0;

DirectedEdge* Ant::selectEdge() {
    float sum=0.0;
    vector<pair<DirectedEdge *, float> > partial;
    partial.clear();
    for (int edge=0;edge<originMap[_position].size();edge++) {
        DirectedEdge *nextEdge = &originMap[_position][edge];
        if (_visitedPoint.find(nextEdge->destNode)==_visitedPoint.end()) {
            float answer = pow(nextEdge->pheno,constSeg.ALPHA)*pow(constSeg.Q_DIST/nextEdge->edgeCost,constSeg.BETA);
            if (pow(nextEdge->pheno,constSeg.ALPHA)>pow(constSeg.Q_DIST/nextEdge->edgeCost,constSeg.BETA)*10) {
                tooAlpha++;
            }
            if (10*pow(nextEdge->pheno,constSeg.ALPHA)<pow(constSeg.Q_DIST/nextEdge->edgeCost,constSeg.BETA)) {
                tooBeta++;
            }
            //cout << "edge " << nextEdge->edgeNum << " takes less than " << answer << " towards " << nextEdge->destNode << endl;
            partial.push_back(pair<DirectedEdge *, float>(nextEdge, answer));
            sum+=answer;
        }
    }
    float answer = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/sum));
    //cout << "total percentage is " << sum << endl;
    //cout << "this time we get " << answer << endl;
    for (int edge=0;edge<partial.size();edge++) {
        DirectedEdge *nextEdge =partial[edge].first;
        double percentage = partial[edge].second;
        //cout << "edge " << nextEdge->edgeNum << " takes less than " << percentage << " towards " << nextEdge->destNode << endl;
        if (percentage > answer) {
            return nextEdge;
        } else {
            answer -= percentage;
        }
    }
    return nullptr;
}
int maxlen=0;
void Ant::travel() {
    if (_position==_destPosition) {
        arrived=true;
        set<int> answer;
        answer.clear();
        //set_intersection(_visitedPoint.begin(),_visitedPoint.end(),demandSet.begin(),demandSet.end(),insert_iterator<set<int> >(answer, answer.begin()));
        for (auto edge : _visitedEdge) {
            if (demandSet.find(edge->destNode)!=demandSet.end()) {
                answer.insert(edge->destNode);
            }
        }
        if (answer.size()>maxlen) {
            for (auto routine : _visitedEdge) {
                cout << routine->edgeNum << " towards " << routine->destNode << endl;
            }
            //sleep(1);
            maxlen=answer.size();
            cout << maxlen << endl;
        }
        if (answer==demandSet) {
            if (currentCost>_cost) {

                for (auto routine : _visitedEdge) {
                    cout << routine->edgeNum << " towards " << routine->destNode << endl;
                }
                //sleep(1);
                cout << "cost:" << _cost << endl;
                currentCost = _cost;
                currentAnswer = _visitedEdge;
            }
        }
        return;
    } else {
        DirectedEdge *selectedEdge=selectEdge();
        if (selectedEdge!= nullptr) {
            _position = selectedEdge->destNode;
            _visitedPoint.insert(selectedEdge->destNode);
            _visitedEdge.push_back(selectedEdge);
            _cost+=selectedEdge->edgeCost;
            travel();
        }
    }
}

void Ant::update() {
    int sumPoint=0,sumDist=0;
    for (DirectedEdge* passEdge : _visitedEdge) {
        sumDist+=passEdge->edgeCost;
        if (demandSet.find(passEdge->destNode)!=demandSet.end()) {
            sumPoint++;
        }
    }
    for (DirectedEdge* passEdge : _visitedEdge) {
        if (maxlen-sumPoint<=2) {
            if (maxlen-sumPoint+1==0) {
                cout << "maxlen:" << maxlen << "sumPoint:" << sumPoint << endl;
                exit(1);
            }
            passEdge->pheno += constSeg.Q_PHENO/sumDist/(maxlen-sumPoint+1);
        }
    }
}

void Ant::releasePheno(){
    for (int i=0;i<originMap.size();i++) {
        for (DirectedEdge &edge : originMap[i]) {
            if (maxlen==demandSet.size()) {
                edge.pheno *= constSeg.PHENO_DECREASE_BEFORE;
            } else {
                edge.pheno *= constSeg.PHENO_DECREASE_BEFORE;
            }
            if (edge.pheno<10) {
                edge.pheno=10;
            }
            if (edge.pheno>1000) {
                edge.pheno=1000;
            }
        }
    }
    //if  (tooAlpha>0 || tooBeta>0)
    //    cout << "too_Alpha:" << tooAlpha << " too_Beta:" << tooBeta << endl;
}

void Ant::printPheno(){
    set<int> xSet = {1,9,14,75,156,111,171,106,29,25,54,50,64,142,159,81,170,84,107,141};
    double sum=0;
    int num=0;
    for (int i=0;i<originMap.size();i++) {
        for (auto edge : originMap[i]) {
            sum += edge.pheno;
            num++;
            if (xSet.find(edge.edgeNum)!=xSet.end()) {
                cout << edge.edgeNum << " has pheno " << edge.pheno << endl;
            }
        }
    }
    cout << "avarage pheno " << sum/num << endl;
}


void timeOver(int x) {
    ofstream outputs("./results");
    outputs << Ant::currentCost << endl;
    outputs.close();
    throw range_error("timeOver");
    //timeCount++;
    //cout << timeCount << "s has passed " << maxSize << endl;
}

ConstSeg getConst() {
    ConstSeg tempSeg;
    ifstream inputs("./seg");
    inputs
        >> tempSeg.ALPHA
        >> tempSeg.BETA
        >> tempSeg.Q_PHENO
        >> tempSeg.PHENO_DECREASE_BEFORE
        //>> tempSeg.PHENO_DECREASE_AFTER
        >> tempSeg.START_PHENO
        >> tempSeg.START_MUL
        >> tempSeg.Q_DIST;
    inputs.close();
    cout << tempSeg.ALPHA
        << tempSeg.BETA
        << tempSeg.Q_PHENO
        << tempSeg.PHENO_DECREASE_BEFORE
        //<< tempSeg.PHENO_DECREASE_AFTER
        << tempSeg.START_PHENO
        << tempSeg.START_MUL
        << tempSeg.Q_DIST << endl;
}

void search_route(char *topo[5000], int edge_num, char *demand)
{
    srand(time(0));
    signal(SIGALRM, timeOver);
    struct itimerval tick;
    tick.it_value.tv_sec=3;
    tick.it_value.tv_usec=0;
    tick.it_interval.tv_sec=0;
    tick.it_interval.tv_usec=0;
    constSeg = getConst();
    int res=setitimer(ITIMER_REAL, &tick, NULL);
    if (res!=0) {
        cout << "set timer fail errno:" << errno << endl;
    }
    cout << "search_route" << endl;
    for (int nodeI=0; nodeI<edge_num; nodeI++) {
        vector<int> topoVec;
        strToVector(topo[nodeI], ",", &topoVec);
        DirectedEdge edgeNow(topoVec[2], topoVec[0], topoVec[3]);
        Ant::originMap[topoVec[1]].push_back(edgeNow);
    }
    vector<int> demandVec;
    strToVector(demand, ",|", &demandVec);
    const int sourceNode = demandVec[0];
    const int destNode = demandVec[1];
    for (int i=2;i<demandVec.size();i++) {
        Ant::demandSet.insert(demandVec[i]);
    }
    Ant::currentCost=99999;
    Ant::currentAnswer.clear();
    vector<Ant> antTeam;
    //cout << "we will start from" << sourceNode << " towards " << destNode << endl;
    try {
        int round=0;
        while (true) {
            for (int i = 0; i < 50; i++) {
                antTeam.push_back(Ant(sourceNode, destNode));
                //cout << "send a new ant" << endl;
                antTeam[i].travel();
            }
            Ant::releasePheno();
            for (int i = 0; i < 50; i++) {
                if (antTeam[i].arrived) {
                    antTeam[i].update();
                }
            }
            //Ant::printPheno();
            //sleep(1);
            //cout << "new round" << endl;
            antTeam.clear();
        }
    } catch (...) {
            cout << "over" << endl;
        }
    if (Ant::currentAnswer.size()!=0) {
        for (auto answerIter : Ant::currentAnswer) {
            record_result(answerIter->edgeNum);
        }
    }
}
