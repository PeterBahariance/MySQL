#ifndef MAP_H
#define MAP_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <vector>

#include "bplustree.h"


using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V())
    {
        key = k;
        value = v;
    }
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me)
    {
        
        outs << "[" << print_me.key << ":" << print_me.value << "]";
        return outs;    
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key == rhs.key)
            return true;
        else    
            return false;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key < rhs.key)
            return true;
        else    
            return false;  
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key > rhs.key)
            return true;
        else    
            return false;      
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key <= rhs.key)
            return true;
        else    
            return false;      
    }
    
    // add the two
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        K newKey = lhs.key + rhs.key;
        V newValue = lhs.value + rhs.value;

        return Pair<K,V>(newKey, newValue)();
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator()
        {

        }
        Iterator(typename map_base::Iterator it)
        {
            _it = it;
        }

        Iterator operator ++(int unused)
        {
            Iterator temp = *this;
             ++(*this);
            return temp;
        } 

        Iterator operator ++()
        {
             ++(*this);
             return *this;
           
        }
        Pair<K, V> operator *()
        {
            return *_it;
            
            //Pair<K, V> temp;
            //return temp;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            if(lhs == rhs)
                return true;
            else
                return false;        
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            if(lhs != rhs)
                return true;
            else
                return false;          
        }
    private:
        typename map_base::Iterator _it;

    };

    Map();
//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;

    // I have not writtent hese yet, but we will need them:
       int count(const K& key);
       Iterator lower_bound(const K& key);
       Iterator upper_bound(const K& key);
       Iterator equal_range(const K& key);

    bool is_valid(){
        
        return true;
        //return map.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }



private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};

template <typename K, typename V>
Map<K,V>::Map()
{
    key_count = 0;
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin()
{
    Iterator temp = map.begin();
    return temp; 
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end()
{
    Iterator temp;
    return temp; 
}

template <typename K, typename V>
int Map<K,V>::size() const
{
    return 0;
}

template <typename K, typename V>
bool Map<K,V>::empty() const
{
    return true;
}

template <typename K, typename V>
V& Map<K,V>::operator[](const K& key)
{   
    //cout << "[] was called: " << endl;

    V& getItem = map.get(Pair<K,V>(key)).value;

    //cout << "We now have our current value, which is " << getItem << endl;
    //cout << endl;
    //V& getItem = map.get(key).value;
    return getItem;


}


template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const
{
    const V *temp;
    return *temp;
}

template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v)
{
    Pair<K,V> test(k,v);
    map.insert(test);
}

template <typename K, typename V>
void Map<K,V>::erase(const K& key)
{

}

template <typename K, typename V>
void Map<K,V>::clear()
{

}

template <typename K, typename V>
V Map<K,V>::get(const K& key)
{
    Pair<K,V> temp(key, V());
    return map.get(temp).value; // Return a reference to the value
    //return map.get(key);
    

}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key)
{
    
    Iterator temp = map.find(key); // if it doesnt find the key, it will add it!
    return temp;


}




template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) const
{
    //Pair<K,V> temp(target, V());
    return map.contains(target);
}

template <typename K, typename V>
int count(const K& key)
{
    return 0;
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::lower_bound(const K& key)
{
    Iterator temp;
    return temp;     
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::upper_bound(const K& key)
{
    Iterator temp;
    return temp; 
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::equal_range(const K& key)
{
    Iterator temp;
    return temp;     
}

template <typename K, typename V>
V& Map<K,V>::at(const K& key)
{
    V *temp;
    return *temp;
}


#endif