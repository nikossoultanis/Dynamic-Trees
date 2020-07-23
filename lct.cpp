#include <iostream>
#include <cstdio>
#include <bits/stdc++.h> 
#include <stdlib.h>
#include <vector>
#include <cassert>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>

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

Vertex makepath(Graph &g) 
{
    id_counter++;
    path_counter++;
    Vertex u1 = add_vertex(g);
    g[u1].data = id_counter;
    g[u1].parent = NULL;
    g[u1].cost = rand()% 100 + 1;
    g[u1].in_path = path_counter;
    vertex_path new_path;
    new_path.path.push_back(u1);
    new_path.successor = u1;
    paths[path_counter] = new_path;

    return u1;
}

Vertex findtail(std::vector<Vertex> path)
{
    if(path.size() > 0)
        return path.back();
    return NULL;
}

Vertex findhead(std::vector<Vertex> path)
{
    if(path.size() > 0)
        return path.front();
    return NULL;
}


int findpathcost(std::vector<Vertex> path, Graph g)
{
    int cost = 0;
    for(std::vector<Vertex>::iterator it = path.begin(); it != path.end(); ++it)
        cost += g[*it].cost;
    return cost;
}

void addpathcost(std::vector<Vertex> path, Graph &g, int cost)
{
    for(std::vector<Vertex>::iterator it = path.begin(); it != path.end(); ++it)
        g[*it].cost += cost;
}

std::vector<Vertex> join(std::vector<Vertex> p1, std::vector<Vertex> v, std::vector<Vertex> p2, Graph &g)
{
    Vertex u;
    if(v.size() > 0)
    {
        u = v.front();
    }
    if(p1.size() < 1 && p2.size() < 1)
    {
        return paths[g[u].in_path].path;
    }
    if(p1.size() > 0)
    {
        adjacency_list<vecS, vecS, bidirectionalS, Node, int>::vertex_descriptor v2 = u;
        Vertex p1_tail = findtail(p1);
        Vertex p2_head = findhead(p2);
        Vertex p1_succ = g[p1_tail].parent;
        if(g[p1_tail].data != g[u].data && !boost::edge(p1_tail,u,g).second && !boost::edge(u,p1_tail,g).second)
            boost::add_edge(p1_tail, u, g[p1_tail].cost,g);
        if(g[u].data != g[p2_head].data && !boost::edge(u,p2_head,g).second && !boost::edge(p2_head,u,g).second)   
            boost::add_edge(u, p2_head, g[u].cost, g);

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
        Vertex p2_head;
        if(p2.empty())
            p2_head = u;
        else
            p2_head = findhead(p2); // index for the map
        if(g[p2_head].in_path != g[u].in_path)
        {  
            if(!boost::edge(u,p2_head,g).second && !boost::edge(p2_head,u,g).second)
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
    // remove_edge(u, v, g);
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

    if(paths[g[u].in_path].path.size() > 1) // if it's not a solo Vertex
    {
        map_it_old = paths.find(g[u].in_path);
        paths.erase(map_it_old);
    }

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
            paths[g[findtail(q)].in_path].successor = u;
        }
        p = join(p, paths[g[u].in_path].path, r, g);
        if(g[u].parent == 0)
        {
            paths[g[findtail(p)].in_path].successor = NULL;
            return p;
        }
    }
return p;
}
// void link(Vertex u, Vertex w)
// {

// }

void cut(Vertex u, Graph &g)
{
    std::pair<std::vector<Vertex>,std::vector<Vertex> > cutted_paths; // cutted paths are p,q
    cutted_paths = split(u, g);
    if(!cutted_paths.first.empty())
    {
        remove_edge((Vertex)cutted_paths.first.front(), u, g);
    }
    if(!cutted_paths.second.empty())
    {   
        remove_edge(u, (Vertex)cutted_paths.second.back(), g);
    }
    paths[g[u].in_path].successor =  NULL;
    Vertex temp = findtail(cutted_paths.second);
    paths[g[temp].in_path].successor = NULL;
}

bool visualize(Graph &g, bool term)
{
    if(!term){
        if( remove( "graph.dot" ) != 0 )
            perror( "Error deleting file" );
        else
            puts( "File successfully deleted" );
        freopen("graph.dot","w",stdout);
        write_graphviz(std::cout, g, default_writer(), 
            make_label_writer(boost::get(edge_bundle,g))); 
    }
    else
    {
        write_graphviz(std::cout, g, default_writer(), 
            make_label_writer(boost::get(edge_bundle,g)));
    }
    
    return true;
}
// TODO when i join two paths, check if a vertex already exists

int main(){
    Graph g;

    std::pair<vertex_iterator, vertex_iterator> vp;

    Vertex x0 = makepath(g);
    g[x0].is_root = 1;
    g[x0].parent = NULL;
    Vertex x1 = makepath(g);
    Vertex x2 = makepath(g);
    Vertex x3 = makepath(g);
    Vertex x4 = makepath(g);
    Vertex x5 = makepath(g);
    Vertex x6 = makepath(g);

    Vertex allNodes[100000];
    allNodes[0] = x0;
    for (int i=1; i<100000; i++)
        allNodes[i] = makepath(g);
    
    for (int i=0; i<50000; i++)
    {
        join(paths[g[allNodes[i]].in_path].path,paths[g[allNodes[i+1]].in_path].path,paths[g[allNodes[i+2]].in_path].path, g);
    }
    for (int i=0; i<25000; i++)
    {
        cut(allNodes[i], g);
    }
    return 1;
}