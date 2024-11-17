#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <vector>

#include "btree_array_funcs.h"

using namespace std;




template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K())
    {
        key = k;
    }
    MPair(const K& k, const V& v)
    {
        key = k;
        //value = v;
    }
    MPair(const K& k, const vector<V>& vlist)
    {
        key = k;
        value_list = vlist;

    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        outs << "[" << print_me.key << ":" << print_me.value_list << "]";
        return outs;       }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key == rhs.key)
            return true;
        else    
            return false;
    }

    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key != rhs.key)
            return true;
        else    
            return false;
    }
    
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key < rhs.key)
            return true;
        else    
            return false;  
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key <= rhs.key)
        {
            return true;
        }
        else    
        {

            return false;
        }  
    }
    
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
       if(lhs.key > rhs.key)
            return true;
        else    
            return false;  
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        vector<V> concatenated_list = lhs.value_list; // Copy lhs value_list
        concatenated_list.insert(concatenated_list.end(), rhs.value_list.begin(), rhs.value_list.end()); // Concatenate rhs value_list

        return MPair<K, V>(lhs.key, concatenated_list); // Create a new MPair with the key from lhs and the concatenated value_list


    }
};


template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;

        Iterator(){}
        Iterator(typename map_base::Iterator it)
        {
            _it = it;
        }
        
        // post order
        Iterator operator ++(int unused)
        {
            _it++;
            return _it;

        }        
        
        
        // pre order
        Iterator operator ++()
        {
            
            ++_it;
            return _it;
        }        
        MPair<K, V> operator *()
        {
            return *_it;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            if(lhs._it == rhs._it)
                return true;
            else
                return false; 
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {

            if(lhs._it != rhs._it)
                return true;
            else
                return false;    
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap();

//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

    void print_lookup();


//  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);

    

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const ;
    vector<V> &get(const K& key);

    Iterator find(const K& key);
    int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);
    Iterator equal_range(const K& key);



    bool is_valid() {
        return true;
    }
    V& at(const K& key);


    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    
    
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

template <typename K, typename V>
MMap<K,V>::MMap()
{

}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin()
{
    Iterator temp = mmap.begin();
    return temp; 

}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end()
{
    Iterator temp;
    return temp; 

}

template <typename K, typename V>
int MMap<K,V>::size() const
{
    return 0;
}

template <typename K, typename V>
bool MMap<K,V>::empty() const
{
    return true;
}

template <typename K, typename V>
const vector<V>& MMap<K,V>::operator[](const K& key) const
{

    vector<V> temp;
    return temp;
}

template <typename K, typename V>
vector<V>& MMap<K,V>::operator[](const K& key)
{
    return mmap.get(MPair<K,V>(key)).value_list;
}

template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v)
{
    MPair<K,V> test(k,v);
    mmap.insert(test);
}

template <typename K, typename V>
void MMap<K,V>::erase(const K& key)
{
    
}

template <typename K, typename V>
void MMap<K,V>::clear()
{

}

template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const
{
    return true;
}

template <typename K, typename V>
vector<V> & MMap<K,V>::get(const K& key)
{
    MPair<K,V> temp(key, V());
    return mmap.get(temp).value_list; 
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key)
{
     Iterator temp = mmap.find(key); // if it doesnt find the key, it will add it!
    return temp;

}

template <typename K, typename V>
int MMap<K,V>::count(const K& key)
{
    return 0;
}



template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K& key)
{
    Iterator lowerBound = mmap.lower_bound(key);
    return lowerBound; 

}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K& key)
{

    Iterator upperBound = mmap.upper_bound(key);
    //cout << "Testing for seg fault2" << endl;

    //cout << *upperBound << endl;
   // cout << "Testing for seg fault2" << endl;

    return upperBound; 

}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::equal_range(const K& key)
{
    Iterator temp;
    return temp; 

}



template <typename K, typename V>
V& MMap<K,V>::at(const K& key)
{
    V *temp;
    return *temp;
}


template <typename K, typename V>
void MMap<K,V>::print_lookup()
{   
    cout << "print_lookup was called" << endl;
    for(MMap<K,V>::Iterator it = begin(); it != end(); it++)
    {
        cout << *it << endl;
    }

}



#endif