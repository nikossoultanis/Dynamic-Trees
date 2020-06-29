#include <iostream>
#include <bits/stdc++.h> 
#include <stdlib.h>
#include <vector>
#include <cassert>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace boost;

typedef struct node_str Node;
int id_counter = 0;
int path_counter = 0;

struct node_str
{
    Node *parent;
    Node *left;
    Node *right;
    int data;
    int cost; 
    bool solid;
    int in_path;
    Node *successor;    
};

typedef adjacency_list<vecS, vecS, bidirectionalS, Node, int> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::vertex_iterator vertex_iterator;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef graph_traits<Graph>::edge_iterator edge_iterator;
typedef graph_traits<Graph>::out_edge_iterator out_edge_iterator;

static std::map<int, std::vector<Vertex>> paths;
static std::map<int, std::vector<Vertex>>::iterator map_it;

Graph makepath(Graph &g) // works
{
    id_counter++;
    path_counter++;
    Vertex u1 = add_vertex(g);
    g[u1].data = id_counter;
    g[u1].left = NULL;
    g[u1].right = NULL;
    g[u1].parent = NULL;
    g[u1].successor = NULL;
    g[u1].cost = 0;
    g[u1].in_path = path_counter;
    std::vector<Vertex> new_path;
    new_path.push_back(u1);
    paths[path_counter] = new_path;

    return g;
}

Vertex findtail(std::vector<Vertex> path) // works
{
    if(path.size() > 0)
        return path.back();
    return NULL;
}

Vertex findhead(std::vector<Vertex> path) // works
{
    if(path.size() > 0)
        return path.front();
    return NULL;
}


int findpathcost(std::vector<Vertex> path, Graph g) // works
{
    int cost = 0;
    for(std::vector<Vertex>::iterator it = path.begin(); it != path.end(); ++it)
        cost += g[*it].cost;
    return cost;
}

void addpathcost(std::vector<Vertex> path, Graph &g, int cost) // works
{
    for(std::vector<Vertex>::iterator it = path.begin(); it != path.end(); ++it)
        g[*it].cost += cost;
}

//TODO change successor, add 3rd parameter to map

std::vector<Vertex> join(std::vector<Vertex> p1, Vertex u, std::vector<Vertex> p2, Graph &g)
{
    adjacency_list<vecS, vecS, bidirectionalS, Node, int>::vertex_descriptor v2 = u;
    Vertex p1_tail = findtail(p1);
    Vertex p2_head = findhead(p2);
    boost::add_edge(p1_tail, u, g);
    boost::add_edge(u, p2_head, g);
    for(std::vector<Vertex>::iterator it = p2.begin(); it != p2.end(); ++it)
    {   
        g[*it].in_path = g[p1_tail].in_path;
    }

    p1.push_back(u);
    p1.insert( p1.end(), p2.begin(), p2.end() );
    
    map_it=paths.find(g[p2_head].in_path);
    paths.erase(map_it);
    return p1;

}

int main(){
    Graph g;

    std::pair<vertex_iterator, vertex_iterator> vp;
    makepath(g);
    makepath(g);
    makepath(g);
    makepath(g);

    // for(vp=vertices(g); vp.first!=vp.second; ++vp.first){
	// 	//std::cout << g[*vp.first].in_path << " ";
    //     std::cout << paths[g[*vp.first].in_path].size() << std::endl;
    // }
    
    std::vector<Vertex> p1 = paths[1];
    std::vector<Vertex> p2 = paths[2];
    Vertex x;
    std::vector<Vertex> final_p;
    final_p = join(p1, x, p2, g);
    std::cout << final_p.size() << std::endl;

    // for(std::vector<Vertex>::iterator it = final_p.begin(); it != final_p.end(); ++it)
    // {
    //     std::cout << g[*it].cost << std::endl;
    // }

    // adjacency_list<vecS, vecS, bidirectionalS, Node, int>::vertex_descriptor v1 = boost::add_vertex(g);
    // adjacency_list<vecS, vecS, bidirectionalS, Node, int>::vertex_descriptor v2 = boost::add_vertex(g);

    // std::pair<boost::adjacency_list<vecS, vecS, bidirectionalS, Node, int>::edge_descriptor, bool> p = boost::add_edge(v1, v2, g);
    // std::cout << boost::edge(v2,v1,g).second <<std::endl;

    // addpathcost(paths[g[1].in_path], g, 666);
    // int cost = findpathcost(paths[g[1].in_path], g);

    // Vertex u = findtail(paths[i]);
    // std::cout << g[u].data << std::endl;
    // for(std::vector<Vertex>::iterator it = paths[g[1].in_path].begin(); it != paths[g[1].in_path].end(); ++it)
    // {
    //     std::cout << g[*it].cost << std::endl;    
    // }
    // for (int i = 1; i <= paths.size(); i++)
    // {

    // }


    return 1;
}