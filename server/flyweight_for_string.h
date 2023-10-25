

#ifndef SERVER_FLYWEIGHT_FOR_STRING_H
#define SERVER_FLYWEIGHT_FOR_STRING_H

#include <iostream>
#include "allocator_border_descriptor.h"
#include "AVL.h"
class flyweight_for_string final{
private:
    class string_comparer{
    public:
        string_comparer() = default;
        int operator()(std::string str1, std::string str2){
            return str1.compare(str2);
        }
    };
    AVL_tree<std::string, int, string_comparer>* _pool_string;
    memory* _alloc;
    flyweight_for_string(){
        _alloc = new allocator_border_descriptor(400000000);
        _pool_string = new AVL_tree<std::string, int, string_comparer>(_alloc);
    }

public:
    static flyweight_for_string& get_instance(){
        static flyweight_for_string _instance;
        return _instance;
    }

    std::string& get_flyweight(const std::string& str){
        if (_pool_string->find_key(str) == 0){
            _pool_string->insert(str, 1);
//            std::cout<<1<<std::endl;
        }else{
            int count = _pool_string->get(str);
            count++;
            _pool_string->update_key(str, count);
//            std::cout<<count<<std::endl;
        }
        return _pool_string->ref_key(str);
    }

    void remove_flyweight(const std::string& str){
        if (_pool_string->find_key(str) == 0){
            return;
        }
        int count = _pool_string->get(str);
        if (count <= 1){
            _pool_string->remove(str);
//            std::cout<<0<<std::endl;
        }else{
            count--;
            _pool_string->update_key(str, count);
//            std::cout<<count<<std::endl;
        }
    }

    ~flyweight_for_string(){
        if (_pool_string != nullptr){
            delete _pool_string;
        }
        if (_alloc != nullptr){
            delete _alloc;
        }
    }

    flyweight_for_string(const flyweight_for_string& other) = delete;
    flyweight_for_string& operator=(const flyweight_for_string& other) = delete;
    flyweight_for_string(const flyweight_for_string&& other) = delete;
    flyweight_for_string& operator=(const flyweight_for_string&& other) = delete;

};

#endif //SERVER_FLYWEIGHT_FOR_STRING_H
