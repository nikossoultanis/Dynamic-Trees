#include <iostream>
#include <stdlib.h>
#include <vector>

typedef struct node_str Node;
int id_counter = 1;
struct node_str
{
    Node *parent;
    Node *left;
    Node *right;
    int id;
    int cost; 
    bool solid;
    std::vector<Node *> path;
    Node *successor;
};

template <class T1, class T2>
Node* maketree(Node *u);
void makepath(Node *u);
Node* successor(Node *u);

bool is_root(Node *node)
{
    return (node->parent == NULL) ? true : false;
}

Node* maketree()
{
    Node *res = (Node *)calloc(sizeof(Node), 1);
    res->id = id_counter++;
    makepath(res);
    Node *suc = successor(res);
    return res;
}

// initialize a path for one vertex
void makepath(Node *u)
{
    (u->path).push_back(u);
}
// returns the parent of last vertex in the path
Node* successor(Node *u)
{
    Node *last = (u->path).back();
    if(!is_root(last))
        return NULL;
    else
        return last->parent;   
}

Node* findtail(std::vector<Node *> path)
{
    if(path.empty())
        return NULL;
    return path.back();
}

Node* findhead(std::vector<Node *> path)
{
    if(path.empty())
        return NULL;
    return path.front();
}


std::vector<Node *> join(std::vector<Node *> p, Node *u, std::vector<Node *> r)
{
    std::vector<Node *> final_path;
    p.push_back(u);
    final_path.reserve(p.size() + r.size());
    final_path.insert(final_path.end(), p.begin(), p.end());
    final_path.insert(final_path.end(), r.begin(), r.end());
    return final_path;
}

std::pair<std::vector<Node *>, std::vector<Node *> > split(Node *u)
{
    std::vector<Node *> p, q, r;

    std::vector<Node *> path = u->path;
    if(u == findhead(path) && u == findtail(path))
    {
        // p.push_back(NULL);
        // q.push_back(NULL);
        return ( std::pair<std::vector<Node *>,std::vector<Node *> >(p,q) );
    }
    else if (u == findhead(path))
    {
        q.reserve(path.size());
        p.push_back(NULL);
        q.insert(q.end(), path.begin(), path.end());
        return ( std::pair<std::vector<Node *>,std::vector<Node *> >(p,q) );
    }
    else if (u == findtail(path))
    {
        p.reserve(path.size());
        q.push_back(NULL);
        p.insert(p.end(), path.begin(), path.end());
        return ( std::pair<std::vector<Node *>,std::vector<Node *> >(p,q) );
    }

    else
    {
        bool u_encountered = false;
        for (std::vector<Node *>::iterator it = path.begin(); it != path.end(); ++it)
        {
            if (*it == u)
            {
                u_encountered = true;
            }
            if (!u_encountered)
                p.push_back(*it);
            if (u_encountered)
                q.push_back(*it);
        }
    return ( std::pair<std::vector<Node *>,std::vector<Node *> >(p,q) );

    }

}
std::vector<Node *> expose(Node *u)
{
    std::vector<Node *> p, q, r;
    std::pair<std::vector<Node *>,std::vector<Node *> > separated_paths;
    Node *w;
    while(u)
    {
        w = successor(u);
        separated_paths = split(u);
        q = separated_paths.first;
        r = separated_paths.second;
        if(!q.empty())
        {
                
        }
        u = w;
    }
    return p;
}


int main()
{
    Node* s = maketree();

    return 1;
}