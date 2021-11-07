#ifndef MYSTL_RBtree_H_
#define MYSTL_RBtree_H_

enum RBtree_color{red = 0, black = 1};

template <class T_Key, class T_Value>
class RBtree_node{
public:
    T_Key key;
    T_Value value;
    RBtree_node* parent;
    RBtree_node* left_child;
    RBtree_node* right_child;
    int color;
public:
    RBtree_node():key(0), value(0), parent(nullptr), left_child(nullptr), right_child(nullptr), color(red){};
    RBtree_node(T_Key key_in, T_Value value_in):key(key_in), value(value_in), parent(nullptr), left_child(nullptr), right_child(nullptr), color(red){};
    RBtree_node(const RBtree_node& node):key(node.key), value(node.value), parent(nullptr), left_child(nullptr), right_child(nullptr), color(red){}
    RBtree_node& operator=(const RBtree_node& node){
        if(this!=&node){
            key = node.key;
            value = node.value;
            parent = node.parent;
            left_child = node.left_child;
            right_child = node.right_child;
            color = node.color;
        }
        return *this;
    }
    ~RBtree_node(){};
};

template <class T_Key, class T_Value>
class RBtree{
public:
    RBtree_node<T_Key, T_Value>* root_node;
    RBtree_node<T_Key, T_Value>* end_node;
public:
    RBtree();
	RBtree(RBtree_node<T_Key, T_Value> current_node);
	~RBtree();
	void insert(RBtree_node<T_Key, T_Value> node);
	void fix_tree_insert(RBtree_node<T_Key, T_Value>* current_node);
	void erase(T_Key erase_key);
	void fix_Tree_delete(RBtree_node<T_Key, T_Value>* current_node, RBtree_node<T_Key, T_Value>* parent);
	void left_rotate(RBtree_node<T_Key, T_Value>* current_node);
	void right_rotate(RBtree_node<T_Key, T_Value>* current_node);
	void DFS_delete(RBtree_node<T_Key, T_Value>* node);
};

template <class T_Key, class T_Value>
RBtree<T_Key, T_Value>::RBtree(){
    root_node = nullptr;
    end_node = new RBtree_node<T_Key, T_Value>();
}

template <class T_Key, class T_Value>
RBtree<T_Key, T_Value>::RBtree(RBtree_node<T_Key, T_Value> current_node){
    root_node = new RBtree_node<T_Key, T_Value>(current_node);
    root_node->color = black;
    end_node = new RBtree_node<T_Key, T_Value>();
    end_node->parent = root_node;
    end_node->left_child = root_node;
    end_node->right_child = root_node;
    root_node->parent = end_node;
}

template <class T_Key, class T_Value>
RBtree<T_Key, T_Value>::~RBtree(){
    if(root_node)
        DFS_delete(root_node);
    delete end_node;
}

template <class T_Key, class T_Value>
void RBtree<T_Key, T_Value>::insert(RBtree_node<T_Key, T_Value> node){
    RBtree_node<T_Key, T_Value>* insert_node = new RBtree_node<T_Key, T_Value>(node);
    if(root_node==nullptr){
        root_node = insert_node;
        root_node->color = black;
        end_node = new RBtree_node<T_Key, T_Value>();
        end_node->parent = root_node;
        end_node->left_child = root_node;
        end_node->right_child = root_node;
        root_node->parent = end_node;
        return;
    }
    RBtree_node<T_Key, T_Value> *temp_node = root_node, *parent = nullptr;
    while(temp_node){
        parent = temp_node;
        if(temp_node->key>insert_node->key)
            temp_node = temp_node->left_child;
        else
            temp_node = temp_node->right_child;
    }
    insert_node->parent = parent;
    //std::cout<<parent->key<<" "<<insert_node->key<<std::endl;
    if(parent->key>insert_node->key)
        parent->left_child = insert_node;
    else
        parent->right_child = insert_node;
    insert_node->color = red;
    fix_tree_insert(insert_node);
    return;
}

//插入后调整，由于插入节点为根节点的情况已在insert函数中处理(直接设为黑色然后返回),所以本函数只处理后两种情况。
template <class T_Key, class T_Value>
void RBtree<T_Key, T_Value>::fix_tree_insert(RBtree_node<T_Key, T_Value>* current_node){
    RBtree_node<T_Key, T_Value> *parent, *gparent, *uncle, *tmp;
    while((parent=current_node->parent)!=end_node&&parent->color==red){
        gparent = parent->parent;
        if(parent==gparent->left_child){
            uncle = gparent->right_child;
            if(uncle!=nullptr&&uncle->color==red){
                std::cout<<"1"<<std::endl;
                parent->color = black;
                uncle->color = black;
                gparent->color = red;
                current_node = gparent;
                continue;
            }
            if(current_node==parent->right_child){
                std::cout<<"2"<<std::endl;
                left_rotate(parent);
                tmp = parent;
                parent = current_node;
                current_node = tmp;
            }
            std::cout<<"3"<<std::endl;
            parent->color = black;
            gparent->color = red;
            right_rotate(gparent);
        }
        else{
            uncle = gparent->left_child;
            if(uncle!=nullptr&&uncle->color==red){
                std::cout<<"4"<<std::endl;
                parent->color = black;
                uncle->color = black;
                gparent->color = red;
                current_node = gparent;
                continue;
            }
            if(current_node==parent->left_child){
                std::cout<<"5"<<std::endl;
                right_rotate(parent);
                tmp = parent;
                parent = current_node;
                current_node = tmp;
            }
            std::cout<<"6"<<std::endl;
            parent->color = black;
            gparent->color = red;
            left_rotate(gparent);
        }
    }
    root_node->color = black;
}

template <class T_Key, class T_Value>
void RBtree<T_Key, T_Value>::erase(T_Key erase_key){
    RBtree_node<T_Key, T_Value>* temp_node = root_node;
    while(temp_node!=nullptr&&temp_node->key!=erase_key){
        std::cout<<temp_node->key<<" "<<erase_key<<std::endl;
        if(temp_node->key>erase_key)
            temp_node = temp_node->left_child;
        else
            temp_node = temp_node->right_child;
    }
    if(temp_node==nullptr){
        std::cout<<"The element you want to delete is not exist!"<<std::endl;
        return;
    }
    std::cout<<temp_node->key<<" "<<erase_key<<std::endl;
    RBtree_node<T_Key, T_Value> *child, *parent;
    int erase_color;
    if(temp_node->left_child!=nullptr&&temp_node->right_child!=nullptr){
        RBtree_node<T_Key, T_Value> *successor_node = temp_node->right_child;//寻找中序后继节点
        while(successor_node->left_child!=nullptr)
            successor_node = successor_node->left_child;
        temp_node->key = successor_node->key;
        temp_node->value = successor_node->value;

        std::cout<<"successor_node = "<<successor_node->key<<std::endl;

        child = successor_node->right_child;
        parent = successor_node->parent;
        erase_color = successor_node->color;
        if(child!=nullptr)
            child->parent = parent;
        if(successor_node==parent->left_child)
            parent->left_child = child;
        else
            parent->right_child = child;
        if(erase_color==black)
            fix_Tree_delete(child, parent);
//        successor_node->parent = nullptr;
//        successor_node->left_child = nullptr;
//        successor_node->right_child = nullptr;
//        successor_node->color = red;
        delete successor_node;
        return;
    }
    if(temp_node->left_child!=nullptr)
        child = temp_node->left_child;
    else
        child = temp_node->right_child;
    parent = temp_node->parent;
    erase_color = temp_node->color;
    if(child!=nullptr)
        child->parent = parent;
    if(parent!=end_node){
        if(temp_node==parent->left_child)
            parent->left_child = child;
        else
            parent->right_child = child;
    }
    else{
        root_node = child;
        end_node->parent = root_node;
        end_node->left_child = root_node;
        end_node->right_child = root_node;
        if(root_node)
            root_node->parent = end_node;
    }
    if(erase_color==black)
        fix_Tree_delete(child, parent);
//    temp_node->parent = nullptr;
//    temp_node->left_child = nullptr;
//    temp_node->right_child = nullptr;
//    temp_node->color = red;
    delete temp_node;
}

template <class T_Key, class T_Value>
void RBtree<T_Key, T_Value>::fix_Tree_delete(RBtree_node<T_Key, T_Value>* current_node, RBtree_node<T_Key, T_Value>* parent){
    RBtree_node<T_Key, T_Value>* other = nullptr;
    while((current_node==nullptr||current_node->color==black)&&current_node!=root_node){
        if(current_node)
            std::cout<<"current_node->key = "<<current_node->key<<std::endl;
        std::cout<<"parent->key = "<<parent->key<<std::endl;
        if(current_node==parent->left_child){
            other = parent->right_child;
            if(other)
                std::cout<<"other->key = "<<other->key<<std::endl;
            if(other->color==red){
                other->color = black;
                parent->color = red;;
                left_rotate(parent);
                other = parent->right_child;
                std::cout<<"a"<<std::endl;
            }
            if((other->left_child==nullptr||other->left_child->color==black)&&(other->right_child==nullptr||other->right_child->color==black)){
                other->color = red;
                current_node = parent;
                parent = current_node->parent;
                std::cout<<"b"<<std::endl;
            }
            else{
                if(other->right_child==nullptr||other->right_child->color==black){
                    other->left_child->color = black;
                    other->color = red;
                    right_rotate(other);
                    other = parent->right_child;
                    std::cout<<"c"<<std::endl;
                }
                other->color = parent->color;
                parent->color = black;
                other->right_child->color = black;
                left_rotate(parent);
                current_node = root_node;
                std::cout<<"d"<<std::endl;
            }
        }
        else{
            other = parent->left_child;
            if(other)
                std::cout<<"other->key = "<<other->key<<std::endl;
            if(other->color==red){
                other->color = black;
                parent->color = red;;
                right_rotate(parent);
                other = parent->left_child;
                std::cout<<"e"<<std::endl;
            }
            if((other->left_child==nullptr||other->left_child->color==black)&&(other->right_child==nullptr||other->right_child->color==black)){
                other->color = red;
                current_node = parent;
                parent = current_node->parent;
                std::cout<<"f"<<std::endl;
            }
            else{
                if(other->left_child==nullptr||other->left_child->color==black){
                    other->right_child->color = black;
                    other->color = red;
                    left_rotate(other);
                    other = parent->left_child;
                    std::cout<<"g"<<std::endl;
                }
                other->color = parent->color;
                parent->color = black;
                other->left_child->color = black;
                right_rotate(parent);
                current_node = root_node;
                std::cout<<"h"<<std::endl;
            }
        }
    }
    if(current_node!=nullptr){
        if(current_node->color==red)
            std::cout<<"red become black!"<<std::endl;
        current_node->color = black;
    }
}

template <class T_Key, class T_Value>
void RBtree<T_Key, T_Value>::left_rotate(RBtree_node<T_Key, T_Value>* current_node){
    RBtree_node<T_Key, T_Value> *right_child = current_node->right_child;
    current_node->right_child = right_child->left_child;
    if(right_child->left_child!=nullptr)
        right_child->left_child->parent = current_node;
    right_child->parent = current_node->parent;
    if(current_node->parent==end_node){
        root_node = right_child;
        end_node->parent = root_node;
        end_node->left_child = root_node;
        end_node->right_child = root_node;
        root_node->parent = end_node;
    }
    else{
        if(current_node==current_node->parent->left_child)
            current_node->parent->left_child = right_child;
        else
            current_node->parent->right_child = right_child;
    }
    right_child->left_child = current_node;
    current_node->parent = right_child;
}

template <class T_Key, class T_Value>
void RBtree<T_Key, T_Value>::right_rotate(RBtree_node<T_Key, T_Value>* current_node){
    //std::cout<<current_node->left_child->key<<std::endl;
    RBtree_node<T_Key, T_Value> *left_child = current_node->left_child;
    current_node->left_child = left_child->right_child;
    if(left_child->right_child!=nullptr)
        left_child->right_child->parent = current_node;
    left_child->parent = current_node->parent;
    if(current_node->parent==end_node){
        root_node = left_child;
        end_node->parent = root_node;
        end_node->left_child = root_node;
        end_node->right_child = root_node;
        root_node->parent = end_node;
    }
    else{
        if(current_node==current_node->parent->left_child)
            current_node->parent->left_child = left_child;
        else
            current_node->parent->right_child = left_child;
    }
    left_child->right_child = current_node;
    current_node->parent = left_child;
}

template <class T_Key, class T_Value>
void RBtree<T_Key, T_Value>::DFS_delete(RBtree_node<T_Key, T_Value>* node){
    RBtree_node<T_Key, T_Value> *left = node->left_child, *right = node->right_child;
    //std::cout<<node->key<<std::endl;
    delete node;
    if(left)
        DFS_delete(left);
    if(right)
        DFS_delete(right);
}

#endif
