#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cassert>


//TODO
//RETURN COST (V, PARENT(V))
//mincost
//
typedef struct node_str Node;
int id_counter = 1;
struct node_str
{
    Node *left;
    Node *right;
    int id;
    Node *parent;
    int cost; // from node to parent(node)
    bool solid;
    Node* successor;
};
template <class T1, class T2>

std::vector<Node *> expose(Node *u);
Node *find_head(std::vector<Node *> path);
Node *find_tail(std::vector<Node *> path);
Node *successor(std::vector<Node *> path);
std::pair<std::vector<Node *>, std::vector<Node *> > split(Node *u);

Node *make_tree()
{
    Node *res = (Node *)calloc(sizeof(Node), 1);
    res->id = id_counter++;
    return res;
}

Node *parent(Node *node)
{
    return node ? node->parent : NULL;
}

Node *grand_parent(Node *node)
{
    return parent(parent(node));
}

bool is_root(Node *node)
{
    return (node->parent == NULL) ? true : false;
}

Node *find_root(Node *node)
{
    while (parent(node) != NULL)
        node = parent(node);
    return node;
}

Node *rotate_right(Node *node)
{
    Node *temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}

Node *rotate_left(Node *node)
{
    Node *temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

std::vector<Node *> find_path(Node *v)
{
    std::vector<Node *> upper_path;
    std::vector<Node *> down_path;
    std::vector<Node *> final_path;
    upper_path.push_back(v);
    // go up to the tree
    Node *temp = v->parent;
    while (temp) // if parent exists
    {
        if (temp->solid)
        {
            upper_path.push_back(temp);
            temp = temp->parent;
        }
    }
    // checking nodes under the node v
    Node *left_child, *right_child;
    left_child = (v->left);
    right_child = (v->right);
    while (left_child || right_child)
    {
        if (left_child->solid)
            down_path.push_back(left_child);

        else
            down_path.push_back(right_child);

        left_child = left_child->left;
        right_child = right_child->right;
    }
    final_path.reserve(down_path.size() + upper_path.size());
    final_path.insert(final_path.end(), down_path.begin(), down_path.end());
    final_path.insert(final_path.end(), upper_path.begin(), upper_path.end());
    return final_path;
}


//TODO: implement join()
std::vector<Node *> expose(Node *u)
{
    std::cout << "e" << std::endl;
    std::vector<Node *> p, q, r;
    Node *w = make_tree();
    Node *s = make_tree();
    std::pair<std::vector<Node *>,std::vector<Node *> > separated_paths;

    while (u)
    {
        w = successor(find_path(u));
        separated_paths = split(u);
        q = separated_paths.first;
        r = separated_paths.second;
        if(!q.empty())
        {
            successor(q);
            s->successor = u;
        }
        u = w;
    }
    // Node *s = successor(p);
    // s->successor = NULL;
    return p;
}
// Procedure makepath p.71
void makepath(Node *u)
{
    u->parent = NULL;
    u->left  = NULL;
    u->right = NULL;
    u->cost  = 0;
}

Node *find_head(std::vector<Node *> path)
{
    return path.back();
}
Node *find_tail(std::vector<Node *> path)
{
    return path.front();
}
Node *successor(std::vector<Node *> path)
{
    Node *last = find_tail(path);
    std::cout << "found tail" << std::endl;
    if (last->parent == NULL){
        std::cout << "null parent" << std::endl;
        return NULL;
    }
    else
        return last->parent;
}
std::pair<std::vector<Node *>, std::vector<Node *> > split(Node *u)
{
    std::vector<Node *> p, q, r;

    std::vector<Node *> path = find_path(u);
    if(u == find_head(path) && u == find_tail(path))
    {
        p.push_back(NULL);
        q.push_back(NULL);
        return ( std::pair<std::vector<Node *>,std::vector<Node *> >(p,q) );
    }
    else if (u == find_head(path))
    {
        q.reserve(path.size());
        p.push_back(NULL);
        q.insert(q.end(), path.begin(), path.end());
        return ( std::pair<std::vector<Node *>,std::vector<Node *> >(p,q) );
    }
    else if (u == find_tail(path))
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
int main()
{
    Node *s;
    s = make_tree();
    //std::pair<std::vector<Node *>,std::vector<Node *> > splitted_path = split(s);
    // for (std::vector<Node *>::iterator it = splitted_path.first.begin(); it != splitted_path.first.end(); ++it)
    // {
    //     std::cout << *it <<" " << std::endl;
    // }
    //     for (std::vector<Node *>::iterator it = splitted_path.second.begin(); it != splitted_path.second.end(); ++it)
    // {
    //     std::cout << *it <<" " << std::endl;
    // }
    std::vector<Node *> p;
    p = expose(s);
    
    Node *x;
    x = make_tree();
    // x->parent = NULL;
    x->parent = NULL;
    // Node *u;
    // Node *w;
    // for(int i=0; i<3; i++){
    // w = successor(find_path(s));

    // u = w;
    // }



    // std::vector<int> x;
    // std::vector<int> y;
    // x.push_back(1);
    // x.push_back(2);
    // y.push_back(3);
    // y.push_back(4);
    // std::pair<std::vector<int>, std::vector<int> > test (x,y);
    // std::cout << test.first.size() << " " <<test.second.size()<< std::endl;
    // for (std::vector<int>::iterator it = test.first.begin(); it != test.first.end(); ++it)
    // {
    //     std::cout << *it <<" " << std::endl;
    // }
    
}