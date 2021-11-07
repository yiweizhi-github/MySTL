#ifndef MYSTL_VECTOR_H_
#define MYSTL_VECTOR_H_

#include "alloc.h"
#include <cstring>

template<class T>
class vector{
public:
    typedef T* iterator;
private:
    iterator start;
    iterator finish;
    iterator end_of_storage;
public:
    vector();
    vector(size_t n, T val);
    vector(const vector& vec);
    vector& operator=(const vector& vec);
    vector(vector&& vec);
    vector& operator=(vector&& vec);
    ~vector();

    T& operator[](int i);
    const T& operator[](int i) const;
public:
    iterator begin();
    iterator end();
    size_t size();
    size_t capacity();
    void push_back(const T& x);
    iterator insert(iterator pos, const T& x);
    iterator insert(iterator pos, int n, const T& x);
    iterator erase(iterator pos);
    iterator erase(iterator pos_start, iterator pos_end);
};

template<class T>
vector<T>::vector(){
    start = nullptr;
    finish = nullptr;
    end_of_storage = nullptr;
    std::cout<<"construct null"<<std::endl;
}

template<class T>
vector<T>::vector(size_t n, T val){
    T* p = (T*)alloc::allocate(sizeof(T)*n);
    start = p;
    finish = p+n;
    end_of_storage = p+n;
    for(size_t i=0;i<n;i++)
        new (p++) T(val);
    std::cout<<"construct"<<std::endl;
}

template<class T>
vector<T>::vector(const vector& vec){
    size_t n = vec.end_of_storage-vec.start;
    T* p = (T*)alloc::allocate(sizeof(T)*n);
    start = p;
    end_of_storage = p+n;
    n = vec.finish-vec.start;
    memcpy(start, vec.start, sizeof(T)*n);
    finish = p+n;
    std::cout<<"construct copy"<<std::endl;
}

template<class T>
vector<T>& vector<T>::operator=(const vector& vec){
    if(this!=&vec){
        size_t n = end_of_storage-start;
        alloc::deallocate(start, sizeof(T)*n);
        n = vec.end_of_storage-vec.start;
        T* p = (T*)alloc::allocate(sizeof(T)*n);
        start = p;
        end_of_storage = p+n;
        n = vec.finish-vec.start;
        memcpy(start, vec.start, sizeof(T)*n);
        finish = p+n;
        std::cout<<"construct copy="<<std::endl;
    }
    return *this;
}

template<class T>
vector<T>::vector(vector&& vec){
    start = vec.start;
    finish = vec.finish;
    end_of_storage = vec.end_of_storage;
    vec.start = nullptr;
    vec.finish = nullptr;
    vec.end_of_storage =nullptr;
    std::cout<<"construct move"<<std::endl;
}

template<class T>
vector<T>& vector<T>::operator=(vector&& vec){
    if(this!=&vec){
        size_t n = end_of_storage-start;
        alloc::deallocate(start, sizeof(T)*n);
        start = vec.start;
        finish = vec.finish;
        end_of_storage = vec.end_of_storage;
        vec.start = nullptr;
        vec.finish = nullptr;
        vec.end_of_storage =nullptr;
        std::cout<<"construct move="<<std::endl;
    }
    return *this;
}

template<class T>
vector<T>::~vector(){
    size_t n = end_of_storage-start;
    if(n)
        alloc::deallocate(start, sizeof(T)*n);
    start = nullptr;
    finish = nullptr;
    end_of_storage =nullptr;
    std::cout<<"destroy"<<std::endl;
}

template<class T>
T& vector<T>::operator[](int i){
    return *(start+i);
}

template<class T>
const T& vector<T>::operator[](int i) const{
    return *(start+i);
}

template<class T>
inline T* vector<T>::begin(){
    return start;
}

template<class T>
inline T* vector<T>::end(){
    return finish;
}

template<class T>
inline size_t vector<T>::size(){
    return finish-start;
}

template<class T>
inline size_t vector<T>::capacity(){
    return end_of_storage-start;
}

template<class T>
void vector<T>::push_back(const T& x){
    if(finish!=end_of_storage){
        new (finish) T(x);
        //memcpy(finish, &x, sizeof(T));
        finish++;
    }
    else{
        size_t n = finish-start;
        size_t old_size = end_of_storage-start;
        size_t new_size = old_size!=0? 2*old_size:1;
        T* new_start = (T*)alloc::allocate(sizeof(T)*new_size);
        if(start){
            memcpy(new_start, start, sizeof(T)*n);
            std::cout<<"start = "<<(int)start<<", new_start = "<<(int)new_start<<std::endl;
            alloc::deallocate(start, sizeof(T)*old_size);
        }
        start = new_start;
        finish = start+n;
        end_of_storage = start+new_size;
        new (finish) T(x);
        finish++;
    }
}

template<class T>
T* vector<T>::insert(iterator pos, const T& x){
    if(finish!=end_of_storage){
        memcpy(pos+1, pos, sizeof(T)*(finish-pos));
        new (pos) T(x);
        finish++;
        return pos;
    }
    else{
        size_t old_size = end_of_storage-start;
        size_t new_size = old_size!=0? 2*old_size:1;
        T* new_start = (T*)alloc::allocate(sizeof(T)*new_size);
        std::cout<<"start = "<<(int)start<<", new_start = "<<(int)new_start<<std::endl;
        if(start){
            size_t length = finish-start;
            T* new_pos = new_start+(pos-start);
            memcpy(new_start, start, sizeof(T)*(pos-start));
            new (new_pos) T(x);
            memcpy(new_pos+1, pos, sizeof(T)*(finish-pos));
            alloc::deallocate(start, sizeof(T)*old_size);
            start = new_start;
            finish = start+length+1;
            end_of_storage = start+new_size;
            return new_pos;
        }
        else{
            start = new_start;
            new (start) T(x);
            finish = start+1;
            end_of_storage = start+new_size;
            return start;
        }
    }
}

template<class T>
T* vector<T>::insert(iterator pos, int n, const T& x){
    for(int i=0;i<n;i++){
        pos = insert(pos, x);
        pos++;
    }
    return --pos;
}

template<class T>
T* vector<T>::erase(iterator pos){
    if(pos>=finish){
        std::cout<<"The position you want to delete is out of range!"<<std::endl;
        return pos;
    }
    finish--;
    memcpy(pos, pos+1, sizeof(T)*(finish-pos));
    return pos;
}

template<class T>
T* vector<T>::erase(iterator pos_start, iterator pos_end){
    if(pos_start>=finish){
        std::cout<<"The position you want to delete is out of range!"<<std::endl;
        return pos_start;
    }
    if(pos_end>finish)
        pos_end = finish;
    size_t n = pos_end-pos_start;
    memcpy(pos_start, pos_end, sizeof(T)*(finish-pos_end));
    finish -= n;
    return pos_start;
}


#endif
