#ifndef BTREE_ARRAY_FUNCS_H
#define BTREE_ARRAY_FUNCS_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

using namespace std;


template <class T>
T maximal(const T& a, const T& b)                      //return the larger of the two items
{
    if(a > b)
        return a;
    else    
        return b;

}

template <class T>
void swap(T& a, T& b)  //swap the two items
{
    T tempValue = a;
    a = b;
    b = tempValue;

}

template <class T>
int index_of_maximal(T data[ ], int n)                 //return index of the largest item in data
{
    int largestIndex = 0;
    for(int i = 1; i < n; i++)
    {
        if(data[i] > data[largestIndex])
            largestIndex = i;
    }

    return largestIndex;
}

template <class T>
void ordered_insert(T data[ ], int& n, T entry)       //insert entry into the sorted array
{                                                        //data with length n
    int i = n-1;
    while(data[i] > entry)
    {
        data[i+1] = data[i];
        i--;
    }
    data[i+1] = entry;
    n++; // increase the given length of your array
}

template <class T>
int first_ge(const T data[ ], int n, const T& entry)   //return the first element in data that is
{                                                        //not less than entry
    for(int i = 0; i < n; i++)
    {

        if(entry <= data[i])
        {
            //cout << "for first_ge, we are returning " << i << endl;
            return i;
        }
    }
    return n;
}

template <class T>
void attach_item(T data[ ], int& n, const T& entry)    //append entry to the right of data
{
    data[n] = entry;
    n++;

}

template <class T>
void insert_item(T data[ ], int i, int& n, T entry)    //insert entry at index i in data
{
    for(int j = n; j > i; j--)
        data[j] = data[j-1];

    data[i] = entry;


    n++; // increase the given length of your array

}

template <class T>
void detach_item(T data[ ], int& n, T& entry)          //remove the last element in data and place
{                                                        //it in entry
    entry = data[n-1];
    n--;

}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry)   //delete item at index i and place it in entry
{
    entry = data[i];
    for(int j = i; j < n-1; j++)
        data[j] = data[j+1];

    n--;
}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)   //append data2 to the right of data1
{
    int j = 0;
    for(int i = n1; i < (n1 + n2); i++)
    {
        data1[i] = data2[j];
        j++;
    }
    
    n1+=n2;
    
    
    n2 = 0;


}

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)   //move n/2 elements from the right of data1
{                                                        //and move to data2
    int mid = (n1)/2;

    int j;
    if(n1 % 2 == 0)
        j = 0;
    
    else
        j = 1;

    for(int i = n2; i < (n2+mid); i++)
    {
        data2[i] = data1[mid + j];
        j++;
    }
    
    n2 += mid;

    n1 -= mid;
    
    //cout << "n2 is " << n2 << endl;

}

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size)              //copy src[] into dest[]
{
    for(int i = 0; i < src_size; i++)
        dest[i] = src[i];

    dest_size = src_size;
    

}

template <class T>
void print_array(const T data[], int n, int pos = -1)  //print array data
{
    for(int i = pos; i < n; i++)
      cout << data[i] << " ";   
}

template <class T>
bool is_gt(const T data[], int n, const T& item)       //item > all data[i]
{
    for(int i = 0; i < n; i++)
    {
        if(item < data[i])
            return false;

    }

    return true;
}

template <class T>
bool is_le(const T data[], int n, const T& item)       //item <= all data[i]
{
    for(int i = 0; i < n; i++)
    {
        if(item > data[i])
            return false;

    }

    return true;
}

//-------------- Vector Extra operators: ---------------------

// FOR B+ TREES

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) //print vector list
{   
    cout << "[ ";
    for(int i = 0; i < list.size(); i++)
        outs << list[i] << " ";

    outs << "]";

    return outs;
}


template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) //list.push_back addme
{
    list.push_back(addme);
    return list;

}

#endif