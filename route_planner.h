#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"
using namespace std;

class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    float GetDistance() const {return distance;}
    // Add public variables or methods declarations here.
    void AStarSearch();

  private:
    // Add private variables or methods declarations here.
    RouteModel &m_Model;
    RouteModel::Node *start_node;
    RouteModel::Node *end_node;
    float distance;
    vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *);
    float CalculateHValue(RouteModel::Node *node);
    vector<RouteModel::Node*> open_list;
    RouteModel::Node *NextNode();
    void AddNeighbors(RouteModel::Node *current_node);
};

#endif
