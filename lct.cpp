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



typedef adjacency_list<vecS, vecS, bidirectionalS, Node, int> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::vertex_iterator vertex_iterator;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef graph_traits<Graph>::edge_iterator edge_iterator;
typedef graph_traits<Graph>::out_edge_iterator out_edge_iterator;

struct node_str
{
    Vertex parent;
    int data;
    int cost; 
    int in_path;
    bool is_root;
};

struct vertex_path
{
    std::vector<Vertex> path;
    Vertex successor;
};

static std::map<int, vertex_path> paths;
static std::map<int, vertex_path>::iterator map_it_new;
static std::map<int, vertex_path>::iterator map_it_old;

Vertex makepath(Graph &g) // works
{
    id_counter++;
    path_counter++;
    Vertex u1 = add_vertex(g);
    g[u1].data = id_counter;
    g[u1].parent = NULL;
    g[u1].cost = 0;
    g[u1].in_path = path_counter;
    vertex_path new_path;
    new_path.path.push_back(u1);
    new_path.successor = NULL;
    paths[path_counter] = new_path;

    return u1;
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

std::vector<Vertex> join(std::vector<Vertex> p1, Vertex u, std::vector<Vertex> p2, Graph &g)
{
    if(p1.size() > 0)
    {
        adjacency_list<vecS, vecS, bidirectionalS, Node, int>::vertex_descriptor v2 = u;
        Vertex p1_tail = findtail(p1);
        Vertex p2_head = findhead(p2);
        Vertex p1_succ = g[p1_tail].parent;
        boost::add_edge(p1_tail, u, g);
        boost::add_edge(u, p2_head, g);
        for(std::vector<Vertex>::iterator it = p2.begin(); it != p2.end(); ++it)
        {
            g[*it].in_path = g[p1_tail].in_path;
            g[u].in_path = g[p1_tail].in_path;
        }

        p1.push_back(u);
        p1.insert( p1.end(), p2.begin(), p2.end() );
        // need to update the map after the join
        map_it_old=paths.find(g[p2_head].in_path);
        paths.erase(map_it_old);
        vertex_path new_path;
        new_path.path = p1;
        new_path.successor = p1_succ;
        int index = g[p1_tail].in_path;
        paths[index] = new_path;

        return paths[index].path;
    }
    else
    {
        Vertex p2_head = findhead(p2); // index for the map
        
        if(!g[p2_head].in_path == g[u].in_path)
        {
            boost::add_edge(u, p2_head, g);
            vertex_path new_path;
            std::vector<Vertex> full_path;
            full_path.push_back(u);
            full_path.insert( full_path.end(), paths[g[p2_head].in_path].path.begin(), paths[g[p2_head].in_path].path.end() );
            new_path.path = full_path;
            new_path.successor = paths[g[p2_head].in_path].successor;
            g[u].in_path = g[p2_head].in_path;
            int index = g[p2_head].in_path;
            paths[index] = new_path;
            
            return paths[index].path;
        }
    return p2;
    }
    
}

// split must update paths after the path cut

std::pair<std::vector<Vertex>, std::vector<Vertex> > split(Vertex u, Graph &g)
{
    std::vector<Vertex> origin, p, q;
    origin = paths[g[u].in_path].path;
    std::vector<Vertex>::iterator index;
    if(g[u].data < 1 )
        return ( std::pair<std::vector<Vertex>,std::vector<Vertex> >(p,q) );

    for(std::vector<Vertex>::iterator it = origin.begin(); it != origin.end(); ++it)
    {
        if(*it == u)
        {
            index = it; 
            break;
        }
    }
    for(std::vector<Vertex>::iterator it1 = origin.begin(); it1 != index; ++it1)
        p.push_back(*it1);
    for(std::vector<Vertex>::iterator it2 = index+1; it2 != origin.end(); ++it2) 
         q.push_back(*it2);

    map_it_old = paths.find(g[u].in_path);
    paths.erase(map_it_old);

    if(!p.empty())
    {
        vertex_path new_path;
        path_counter++;
        for(std::vector<Vertex>::iterator it1 = p.begin(); it1 != p.end(); ++it1)
            g[*it1].in_path = path_counter;

        new_path.path = p;
        new_path.successor = g[findtail(p)].parent;
        paths[path_counter] = new_path;
    }
    if(!q.empty())
    {
        vertex_path new_path;
        path_counter++;
        for(std::vector<Vertex>::iterator it1 = q.begin(); it1 != q.end(); ++it1)
            g[*it1].in_path = path_counter;
        new_path.path = q;
        new_path.successor = g[findtail(q)].parent;
        paths[path_counter] = new_path;
    }
    return ( std::pair<std::vector<Vertex>,std::vector<Vertex> >(p,q) );
}




std::vector<Vertex> expose(Vertex u, Graph &g)
{
    std::pair<std::vector<Vertex>,std::vector<Vertex> > cutted_paths; // cutted paths are p,q
    std::vector<Vertex> p,q,r;
    Vertex w;
    while(g[u].data != 0)
    {   
        w = paths[g[u].in_path].successor;
        cutted_paths = split(u, g);
        q = cutted_paths.first;
        r = cutted_paths.second;
        if(!q.empty())
        {
            paths[g[findtail(p)].in_path].successor = u;
        }
        p = join(p, u, r, g);

        if(g[u].parent == NULL && g[u].is_root)
        {
            paths[g[findtail(p)].in_path].successor = NULL;
            return  p;
        }
    }
}

void cut(Vertex u, Graph &g) // needs expose
{
    std::pair<std::vector<Vertex>,std::vector<Vertex> > cutted_paths; // cutted paths are p,q
    std::vector<Vertex> arg = expose(u, g);
    cutted_paths = split(u, g);
    paths[g[u].in_path].successor =  NULL;
    Vertex temp = findtail(cutted_paths.second);
    paths[g[temp].in_path].successor = NULL;
}

// TODO when i join two paths, check if a vertex already exists

int main(){
    Graph g;

    std::pair<vertex_iterator, vertex_iterator> vp;
    Vertex x1 = makepath(g);
    g[x1].is_root = 1; // my root
    g[x1].parent = NULL;

    makepath(g);
    Vertex x = makepath(g);
    makepath(g);
    makepath(g);

    // for(vp=vertices(g); vp.first!=vp.second; ++vp.first)
    // {
	// 	std::cout << g[*vp.first].in_path << " ";
    //     std::cout << (paths[g[*vp.first].in_path].successor) << std::endl;
    // }
    std::vector<Vertex> p1 = paths[1].path;
    std::vector<Vertex> p2 = paths[2].path;
    std::vector<Vertex> final_p;
    std::vector<Vertex> p3;
    final_p = join(p1, x, p2, g);

    // std::cout << g[0].data << std::endl;
    // std::cout << final_p.size() << std::endl;
    // std::cout << "final path is: ";
    // for(std::vector<Vertex>::iterator it = paths[g[x].in_path].path.begin(); it != paths[g[x].in_path].path.end(); ++it)
    // {
    //     if(paths[g[*it].in_path].successor != NULL)
    //         std::cout << paths[g[x].in_path].successor <<" ";
    // }

    // std::cout << g[findtail(paths[g[x1].in_path].path)].data << std::endl;
    // std::cout << g[x1].data << std::endl;
    // std::vector<Vertex> p = expose(x1, g);


    // cut(x1,g);
    expose(x1,g);
    // split(x1, g);
    // std::cout << g[x1].in_path << std::endl;
    for(std::vector<Vertex>::iterator it = paths[6].path.begin(); it != paths[6].path.end(); ++it)
    {
        std::cout << g[*it].data << std::endl;
    }


    // std::cout << paths[g[x].in_path].path.size();
    // std::cout << paths[findtail(paths[g[x].in_path].path)].successor << std::endl;
    // std::cout << g[x].data << std::endl;
    // std::cout << "" << std::endl;

    // std::vector<Vertex> p;
    // std::cout << "x path is: ";
    // for(std::vector<Vertex>::iterator it = paths[g[x].in_path].path.begin(); it != paths[g[x].in_path].path.end(); ++it)
    // {
    //     std::cout << g[*it].data << " ";
    // }
    // std::cout << "" << std::endl;
    // std::cout << "cutting path finished" << std::endl;
    // std::pair<std::vector<Vertex>,std::vector<Vertex> > cutted_paths;

    // cutted_paths = split(x, g);
    // std::cout << "splitted path: ";
    // for(std::vector<Vertex>::iterator it = (cutted_paths.first).begin(); it != (cutted_paths.first).end(); ++it)
    // {
    //     std::cout << g[*it].data << " ";
    // }
    // std::cout << "" << std::endl;



    // for(std::vector<Vertex>::iterator it = final_p.begin(); it != final_p.end(); ++it)
    // {
    //    std::cout << paths[g[*vp.first].in_path].successor << std::endl;
    // }

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