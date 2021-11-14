#include "vector.h"
#include "list.h"
#include "RBtree.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

void vector_test();
void list_test();
void RBtree_test1();
void RBtree_test2();

int main(){
    RBtree_test2();
}

void vector_test(){
    vector<int> a(4, 5);
    a.insert(a.end(), 1);
    for(auto i=a.begin();i!=a.end();i++)
        std::cout<<*i<<" ";
    std::cout<<std::endl;
    a[2] = 3;
    a.push_back(7);
    for(size_t j=0;j<a.size();j++)
        std::cout<<a[j]<<" ";
    std::cout<<std::endl;
    vector<vector<int>> b;
    b.insert(b.begin(), 2, a);
    b.push_back(a);
    for(auto i=b.begin();i!=b.end();i++){
        for(size_t j=0;j!=(*i).size();j++)
            std::cout<<(*i)[j]<<" ";
        std::cout<<std::endl;
    }
    b[1][2]=0;
    for(auto i=b.begin();i!=b.end();i++){
        for(size_t j=0;j!=(*i).size();j++)
            std::cout<<(*i)[j]<<" ";
        std::cout<<std::endl;
    }
    a[3] = 16266;
    b.insert(b.begin()+1, 10, a);
    for(auto i=b.begin();i!=b.end();i++){
        for(size_t j=0;j!=(*i).size();j++)
            std::cout<<(*i)[j]<<" ";
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    b.erase(b.begin()+1, b.begin()+100);
    for(auto i=b.begin();i!=b.end();i++){
        for(size_t j=0;j!=(*i).size();j++)
            std::cout<<(*i)[j]<<" ";
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    b.erase(b.begin()+1);
    for(auto i=b.begin();i!=b.end();i++){
        for(size_t j=0;j!=(*i).size();j++)
            std::cout<<(*i)[j]<<" ";
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    a.erase(a.begin()+1, a.begin()+3);
    b.insert(b.begin()+1, 4, a);
    for(auto i=b.begin();i!=b.end();i++){
        for(size_t j=0;j!=(*i).size();j++)
            std::cout<<(*i)[j]<<" ";
        std::cout<<std::endl;
    }
}

void list_test(){
    list<int> a(6, 999);
    list<int> b(3, 12);
    list<int> c(6, 64);
    list<int> e(10, 500);
    list<list<int>> d;
    d.insert(d.begin(), 2, a);
    d.insert(d.end()+94, 2, a);
    d.insert(d.end()+13, 2, b);
    d.insert(d.end()+10003, 2, c);
    for(auto it1=d.begin();it1!=d.end();++it1){
        for(auto it2=(*it1).begin();it2!=(*it1).end();++it2){
            std::cout<<*it2<<" ";
        }
        std::cout<<it1.node;
        std::cout<<std::endl;
    }
    auto it1 = d.begin();
    std::cout<<it1.node<<std::endl;
    it1 = it1+4;
    std::cout<<it1.node<<std::endl;
    auto it2 = (*it1).begin();
    it2 = it2+3;
    *it2 = 1234;
    d.erase(it1+3, it1+6);
    std::cout<<(it1+10)-(it1+3)<<std::endl;
    std::cout<<it1.node<<std::endl;
    std::cout<<d.begin().node<<" "<<d.end().node<<std::endl;
    int i=0;
    for(auto it1=d.begin();i<20;++it1,++i){
        for(auto it2=(*it1).begin();it2!=(*it1).end();++it2){
            std::cout<<*it2<<" ";
        }
        std::cout<<it1.node;
        std::cout<<std::endl;
    }
    std::cout<<a.begin().node<<" "<<a.end().node<<std::endl;
    auto it = a.begin();
    std::cout <<*it<<" "<<it.node<<std::endl;
    ++it;
    while (it!=a.end())
    {
        std::cout<<*it<<" "<<it.node<<std::endl;
        ++it;
    }
    a.insert(a.begin()+2, 2, 7);
    std::cout<<std::endl;
    it = a.begin();
    std::cout<<*it<<" "<<it.node<<std::endl;
    ++it;
    while (it!=a.end())
    {
        std::cout<<*it<<" "<<it.node<<std::endl;
        ++it;
    }
    a.erase(a.begin()+1, a.begin()+3);
    std::cout<<std::endl;
    it = a.begin();
    std::cout<<*it<<" "<<it.node<<std::endl;
    ++it;
    while (it!=a.end())
    {
        std::cout<<*it<<" "<<it.node<<std::endl;
        ++it;
    }
    a.insert(a.begin()+2, 1234);
    std::cout<<std::endl;
    it = a.begin();
    std::cout<<*it<<" "<<it.node<<std::endl;
    ++it;
    while (it!=a.end())
    {
        std::cout<<*it<<" "<<it.node<<std::endl;
        ++it;
    }
    a.erase(a.begin()+2);
    std::cout<<std::endl;
    it = a.begin();
    std::cout<<*it<<" "<<it.node<<std::endl;
    ++it;
    while (it!=a.end())
    {
        std::cout<<*it<<" "<<it.node<<std::endl;
        ++it;
    }
    it = a.begin()+3;
    *it = 567;
    std::cout<<std::endl;
    it = a.begin();
    std::cout<<*it<<" "<<it.node<<std::endl;
    ++it;
    while (it!=a.end())
    {
        std::cout<<*it<<" "<<it.node<<std::endl;
        ++it;
    }
    std::cout<<std::endl;
    std::cout<<a.front()<<" "<<a.back()<<std::endl;
}


void RBtree_test1(){
    RBtree_node<int, int> *temp[100000];
    RBtree_node<int, int> *node = new RBtree_node<int, int>(199, 500);
    RBtree_node<int, int> *test1 = new RBtree_node<int, int>(9354, 500);
    RBtree_node<int, int> *test2 = new RBtree_node<int, int>(3454, 500);
    RBtree_node<int, int> *test3 = new RBtree_node<int, int>(34, 500);
    temp[0] = new RBtree_node<int, int>(1, 1);
    RBtree<int, int> tree(*temp[0]);
	for (int i=2;i<=100000;i++){
		temp[i-1] = new RBtree_node<int, int>(i, i);
		tree.insert(*temp[i-1]);
	}
	tree.insert(*test1);
	tree.erase(33);
	tree.erase(34);
	tree.erase(35);
	tree.insert(*temp[33]);
	tree.insert(*node);
	tree.erase(356);
	tree.erase(798);
	tree.erase(1198);
	tree.insert(*test2);
	tree.insert(*test3);
	tree.erase(155);
	tree.erase(10000);
//	for(int i=1;i<=10000;i++){
//	    std::cout<<temp[i-1]->key<<" color = "<<temp[i-1]->color<<std::endl;
//        if(temp[i-1]->parent)
//            std::cout<<"parent = "<<temp[i-1]->parent->key<<std::endl;
//        if(temp[i-1]->left_child)
//            std::cout<<"left_child = "<<temp[i-1]->left_child->key<<std::endl;
//        if(temp[i-1]->right_child)
//            std::cout<<"right_child = "<<temp[i-1]->right_child->key<<std::endl;
//	}
//	std::cout<<node->key<<" color = "<<node->color<<std::endl;
//    if(node->parent)
//        std::cout<<"parent = "<<node->parent->key<<std::endl;
//    if(node->left_child)
//        std::cout<<"left_child = "<<node->left_child->key<<std::endl;
//    if(node->right_child)
//        std::cout<<"right_child = "<<node->right_child->key<<std::endl;
	for(int i=100000;i>=1;i--){
		tree.erase(i);
		std::cout<<"erase"<<i<<std::endl;
	}
	std::cout<<"root = "<<tree.root_node->key<<std::endl;
}

void RBtree_test2(){
    srand((int)time(0));
    RBtree<int, int> tree;
    RBtree_node<int, int>* temp[100000];
    int record[100000];
    for(int i=0;i<100000;++i){
        record[i] = rand()%10000;
        temp[i] = new RBtree_node<int, int>(record[i], i);
        tree.insert(*temp[i]);
    }
    for(int i=0;i<100000;++i)
        tree.erase(record[i]);
    for(int i=0;i<100000;++i)
        delete temp[i];

}
