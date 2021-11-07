#ifndef MYSTL_ALLOC_H_
#define MYSTL_ALLOC_H_

#include <cstddef>
#include <cstdlib>
#include <iostream>

union FreeList{
    union FreeList* next;
    char data[1];
};

const size_t max_bytes = 128;
const size_t free_list_length = 16;
const size_t num_add_list = 8;

class alloc{
private:
    static char* pool_start;
    static char* pool_end;
    static size_t pool_size;

    static FreeList* free_list[free_list_length];

public:
  static void* allocate(size_t n);
  static void  deallocate(void* p, size_t n);

private:
  static size_t find_list_index(size_t n);
  static size_t round_up(size_t n);
  static void*  refill(size_t n);
  static char*  chunk_alloc(size_t n, size_t &num);
};

char*  alloc::pool_start = nullptr;
char*  alloc::pool_end = nullptr;
size_t alloc::pool_size = 0;

FreeList* alloc::free_list[free_list_length] = {
  nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr
};

inline void* alloc::allocate(size_t n)
{
    if(n>max_bytes)
        return malloc(n);
    size_t index = find_list_index(n);
    FreeList* result = free_list[index];
    if(result){
        free_list[index] = free_list[index]->next;
        return result;
    }
    return refill(round_up(n));
}

inline void alloc::deallocate(void* p, size_t n)
{
    if(n>max_bytes){
        free(p);
        return;
    }
    size_t index = find_list_index(n);
    ((FreeList*)p)->next = free_list[index];
    free_list[index] = (FreeList*)p;
}

inline size_t alloc::find_list_index(size_t n){
    //std::cout<<"n = "<<n<<std::endl;
    if(n%8){
        //std::cout<<"index = "<<(int)n/8<<std::endl;
        return n/8;
    }
    else{
        //std::cout<<"index = "<<(int)(n/8-1)<<std::endl;
        return n/8-1;
    }
}

inline size_t alloc::round_up(size_t n){
    if(n%8)
        return 8*(n/8+1);
    else
        return n;
}

void* alloc::refill(size_t n){
    size_t num = num_add_list;
    char* block = chunk_alloc(n, num);
    if(num==1)
        return block;
    free_list[find_list_index(n)] = (FreeList*)(block + n);
    FreeList* now_list = nullptr;
    FreeList* next_list = (FreeList*)(block + n);
    for(size_t i=1;;++i){
        now_list = next_list;
        next_list = (FreeList*)((char*)now_list+n);
        if(num-1==i){
            now_list->next= nullptr;
            break;
        }
        else
            now_list->next = next_list;
    }
    return block;
}

char* alloc::chunk_alloc(size_t n, size_t &num){
    char* result;
    size_t need_bytes = n* num;
    size_t pool_bytes = pool_end - pool_start;
    if(pool_bytes>=need_bytes){
        result = pool_start;
        pool_start += need_bytes;
        return result;
    }
    else if(pool_bytes>=n){
        num = pool_bytes/n;
        result = pool_start;
        pool_start += num*n;
        return result;
    }
    else{
        if(pool_bytes){
            size_t index = find_list_index(pool_bytes);
            ((FreeList*)pool_start)->next = free_list[index];
            free_list[index] = (FreeList*)pool_start;
        }
        size_t bytes_to_get = need_bytes*8;
        pool_start = (char*)malloc(bytes_to_get);
        if(!pool_start){
            FreeList* temp_list;
            size_t index;
            for(size_t i=n;i<=max_bytes;i+=8){
                index = find_list_index(i);
                temp_list = free_list[index];
                if(temp_list){
                    free_list[index] = free_list[index]->next;
                    pool_start = (char*)temp_list;
                    pool_end = pool_start+i;
                    return chunk_alloc(n, num);
                }
            }
            std::cout<<"out of memory!"<<std::endl;
            pool_end = nullptr;
            throw std::bad_alloc();
        }
        else{
            pool_end = pool_start+bytes_to_get;
            pool_size += bytes_to_get;
            return chunk_alloc(n, num);
        }
    }
}

#endif
