typedef struct node_str Node;

struct node_str {
   Node *left;
   Node *right;
   int id;
    Node *parent;
   // Node *path_parent; /* We can in the end take this off (or not). Leave it for now, for simplicity. Here there is a tradeoff space/time. */
};

Node *make_tree(int id) { // int id not needed 100%
    Node *res = calloc(sizeof(Node), 1);
    res->id = id;
    return res;
}

Node *parent(Node *node) {
    return node ? node->parent : NULL;
}

bool is_root(Node *node) {
    return (node->parent == NULL) ? true : false;
}

Node *find_root(Node *node){
    while(parent(node) != NULL)
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
