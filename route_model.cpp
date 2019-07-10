#include <vector>
#include <iostream>
#include <vector>
#include "route_model.h"


using namespace std;

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  int count = 0;
  vector<Model::Node> vectorGivenByThisNodes = this -> Nodes();
  for(int i = 0; i < vectorGivenByThisNodes.size(); i++) {
    m_Nodes.push_back(RouteModel::Node(count, this, vectorGivenByThisNodes[i]));//create a routemodel node for each of the model nodes (m_nodes) and push counter, this, and node back into the vector of m_nodes.
    count++;
  }
  CreateNodeToRoadHashmap();
}

  /*
  for(Model::Node node: this->Nodes()) { //iterates over all the nodes in this->Nodes
    m_Nodes.push_back(Node(counter, this, node)); //create a routemodel node for each of the model nodes (m_nodes) and push counter, this, and node back into the vector of m_nodes.
    counter++;
  }
  CreateNodeToRoadHashmap();
}
*/
void RouteModel::CreateNodeToRoadHashmap() {
  for (const Model::Road &road: Roads()) { //iterate over the const road reference in the vector of model Roads
    if (road.type != Model::Road::Type::Footway) { //if the road is not a footway
      for (int node_idx : Ways()[road.way].nodes) {
        if (node_to_road.find(node_idx) == node_to_road.end()) {
         node_to_road[node_idx] = vector<const Model::Road*> ();
        }
         node_to_road[node_idx].push_back(&road);
       }
    }
  }
}

//Declare the FindNeighbor Function in the Routemodel Class. Node is a subclass of routemodel, so will need 2 scope resolution operators; searches through node indices to find closest node to the current node
RouteModel::Node *RouteModel::Node::FindNeighbor(vector<int> node_indices) {
 Node *closest_node = nullptr; //create a pointer|assign it to Null
 Node node; //Temporary node variable node
 for (int node_index : node_indices) { //takes current node indexes in node_indices and loops through them
   node = parent_model -> SNodes()[node_index]; //sets temporary node var equal to the node_index sub-class in the parent_model
   if (this-> distance(node) != 0 && !node.visited) { //conditions for the if statement
     if (closest_node == nullptr || this-> distance(node) < this->distance(*closest_node)) { //this -> references the node being looked at by the for loop
       closest_node = &parent_model-> SNodes()[node_index]; //set the address of closest_node to the actual address of the parent model's SNodes vector
     }
   }
   return closest_node;
 }
}

void RouteModel::Node::FindNeighbors() {
  for (auto &road : parent_model->node_to_road[this->index]) { //for each index in the current parent model object:
    RouteModel::Node *new_neighbor = this->FindNeighbor(parent_model -> Ways()[road->way].nodes);
    //returns all of the indices of the nodes for the current road AND returns a pointer to that node stored as new_neighbor
    if(new_neighbor) {  //check if new_neighbor is a null pointer
      this -> neighbors.push_back(new_neighbor);
    }
  }
}

RouteModel::Node &RouteModel::FindClosestNode(float x, float y) { //accepts two floats and returns a reference to a RouteModel::Node Object
  Node input; //create a temporary node
  input.x = x;
  input.y = y;

  float min_dist = std::numeric_limits<float>::max(); //find minimum distance in your search
  float dist;
  int closest_idx; //will store the index of the closest node

  for (const Model::Road &road : Roads()) { //search through nodes that are roads and not footways
    if (road.type != Model::Road::Type::Footway) {
      for (int node_idx : Ways()[road.way].nodes) { //for each index in the node_index
        dist = input.distance(SNodes()[node_idx]); //check distance to the current node
        if (dist < min_dist) {
          closest_idx = node_idx;
          min_dist = dist;
        }
      }
    }
  }
  return SNodes()[closest_idx];
};
