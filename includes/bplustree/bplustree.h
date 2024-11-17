#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <vector>

#include "../../includes/bplustree/btree_array_funcs.h"


using namespace std;

template <class T>
class BPlusTree
{
public:
    // done and tested
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0): it(_it), key_ptr(_key_ptr){}

        T operator *()
        {
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used)
        {

            // dont utilize un_used??
            //  In C++, the post-increment operator must have an int parameter, but its value is not typically used within the operator's logic.
            Iterator copyOriginal(this->it, this->key_ptr); // this is always a pointer to the current object, thats why we use the -> to dereference it 
            key_ptr++; 
            if(key_ptr == (this->it->data_count))
            {
                this->it = this->it->next;
                key_ptr = 0;
            }            
            return copyOriginal;

        }
        Iterator operator++()
        {
            key_ptr++;
            
            if(key_ptr == (this->it->data_count)) // I dont do data_count - 1, since increment key_ptr before
            {
                //cout << "this is called: " << key_ptr << endl;

                this->it = this->it->next;
                key_ptr = 0;
            }

            return *this;
  
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            if(lhs.it == rhs.it)
            {
                //cout << "Our iterators BPlusTrees are equal!!!" << endl;
                return true;
            }
            else
                return false;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            if(rhs.it == nullptr && lhs.it != nullptr)
                return true;
            else if(rhs.it == nullptr && lhs.it == nullptr)
                return false;

            if(lhs.it != rhs.it || lhs.key_ptr != rhs.key_ptr)
            {
                return true;
            }

            else    
                return false;

        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: " << endl;
                cout <<  *it << endl;
                // print_array(it->data, it->data_count, key_ptr);
                cout << endl;
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){
            return !it; // returns true if our BPlusTree pointer isnt pointing to anything
        }
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false); // done and tested
    BPlusTree(T *a, int size, bool dups = false); // done and tested

    //big three:
    BPlusTree(const BPlusTree<T>& other); // done and tested
    ~BPlusTree(); // done and tested
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS); // done and tested

    void copy_tree(const BPlusTree<T>& other);  //copy other into this object  // done and tested
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node); // done and tested

    void insert(const T& entry);   //insert entry into the tree  // done and tested
    
    void remove(const T& entry);   //remove entry from the tree
    
    void clear_tree();             //clear this object (delete all nodes etc.) // done and tested

    bool contains(const T& entry) const; //true if entry can be found // done and tested
    T& get(const T& entry);              //return a reference to entry // done and tested
    const T& get(const T& entry)const;   //return a reference to entry // done and tested
    T& get_existing(const T& entry);     //return a reference to entry// done and tested
    
    
    Iterator find(const T& key);       //return an iterator to this key.    // done and tested
                                        //     NULL if not there.
    
    Iterator find(const T& key) const;       // const version, where we return an iterator to this key. // done and tested

    Iterator findBound(const T& key);

    
    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const;                    //count the number of elements

    bool empty() const;                  //true if the tree is empty

    
    void print_tree(int level = 0,  // done and tested
                    ostream &outs=cout) const;

    friend ostream& operator<<(ostream& outs,  // done and tested
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();
    string in_order(); // done and tested
    string pre_order(); // done and tested
    string post_order(); // done and tested

    Iterator begin(); // done and tested
    Iterator begin() const; // done and tested
    Iterator end(); // done and tested
    
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

    // a function to see if my copy constructor worked:
    //void check_leaf_link();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const //true if this is a leaf node // done and tested
    {
        return child_count==0;
    }             

    T* find_ptr(const T& entry);         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);   //allows MAXIMUM+1 data elements in        // done and tested
                                         //   the root
    void fix_excess(int i);              //fix excess in child i        // done and tested

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node(); // done and tested

    BPlusTree<T>* get_bounded_node(const T& entry); // done and tested


    BPlusTree<T>* get_smallest_node() const; // done and tested


    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};

template <typename T>
BPlusTree<T>::BPlusTree(bool dups){
    data_count = 0;
    child_count = 0;
    for(int i = 0; i < MAXIMUM+2; i++)
        subset[i] = nullptr;

}

template <typename T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups)
{
    child_count = 0;
    data_count = 0;
    for(int i = 0; i < size; i++)
    {
        this->insert(a[i]);        
    }    
}

template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other){
    copy_tree(other);


}

template <typename T>
BPlusTree<T>::~BPlusTree(){
   if(data_count != 0)
    clear_tree();

}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
   
    if(this == &RHS)
    {
        return *this;
    }

    if(data_count != 0)
    {
        clear_tree();
        
    }
    copy_tree(RHS);

    
    return *this;
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other)  //copy other into this object
{
    BPlusTree<T> *last = new BPlusTree<T>();
    last = nullptr;
    copy_tree(other, last);
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node)
{
    copy_array(data, other.data, data_count, other.data_count);

    for(int i = 0; i < other.child_count; i++)
    {
        subset[i] = new BPlusTree<T>();
        subset[i]->copy_tree(*(other.subset[i]), last_node);
        
    }
    child_count = other.child_count;
    if(is_leaf())
    {
        if(last_node == nullptr)
            last_node = this;
        else
        {
            last_node->next = this;
            last_node = this;
        }
        
    }
    
}

template <typename T>
void BPlusTree<T>::insert(const T& entry)   //insert entry into the tree
{
    loose_insert(entry);

    // GROW TREE
    if(data_count > MAXIMUM)
    {
        //cout << "WE NEED TO GROW THE TREE!" << endl;
        BPlusTree<T> *tempRoot = new BPlusTree<T>();
        //copy_array X 2: data, subset.

        copy_array(tempRoot->data, data, tempRoot->data_count, data_count);
        copy_array(tempRoot->subset, subset, tempRoot->child_count, child_count);

        data_count = 0;
        child_count = 1;
        subset[0] = tempRoot;
        fix_excess(0);

    }
}

template <typename T>
void BPlusTree<T>::remove(const T& entry)   //remove entry from the tree
{

}

template <typename T>
void BPlusTree<T>::clear_tree()             //clear this object (delete all nodes etc.)
{
    if(is_leaf())
    {
        data_count = 0;
        next = nullptr;
        return;
    }
    for(int i = 0; i < child_count; i++)
        subset[i]->clear_tree();
    
    data_count = 0;
    child_count = 0;
    next = nullptr;

}

template <typename T>
bool BPlusTree<T>::contains(const T& entry) const {
    int i = first_ge(data, data_count, entry);

    if (i < data_count && data[i] == entry)
        return true;  

    if (is_leaf())
        return false; 

    return subset[i]->contains(entry);
}


template <typename T>
T& BPlusTree<T>::get(const T& entry)              //return a reference to entry
{
    //cout << "non const get was called: " << endl;
    return get_existing(entry);
}   

template <typename T>
const T& BPlusTree<T>::get(const T& entry)const   //return a reference to entry
{
    Iterator foundItem = find(entry);
    assert(foundItem == end() && "NOT IN TREE");

    T item = *foundItem;
    return item;
}  

template <typename T>
T& BPlusTree<T>::get_existing(const T& entry)     //return a reference to entry
{
    //cout << "calling get_exisiting" << endl;
    //cout << "Current data_count is " << data_count << endl;
    T* foundItem = find_ptr(entry);
    //cout << "CHECK" << endl;

    if(contains(entry))
    {
        //cout << "contains the key! " << endl;
        
        return *foundItem;
    }
    else
    {
        //cout << "we must INSERT in our get existing: " << endl;
        insert(entry);
        foundItem = find_ptr(entry);

        assert(foundItem != nullptr && "NOT IN TREE");

        return *foundItem; 
    
    }
    
   
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& key)       //return an iterator to this key.
{


    Iterator endIt = NULL;

    for(Iterator it = begin(); it != endIt; it++)
    {
        if(*it == key)
        {
            return it;
        }
    }   

    return endIt;
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& key) const       //return an iterator to this key.
{
    Iterator endIt;
    endIt = nullptr;

    endIt.it = nullptr;

    //cout << "const find was called " << endl;

    for(Iterator it = begin(); it != endIt; it++)
    {
        //cout << *it << endl;
        if(*it == key)
            return it;
    }   

    return NULL;
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::findBound(const T& key)
{
    Iterator boundedIterator;
    boundedIterator.it = get_bounded_node(key);
    
    boundedIterator.key_ptr = 0;
    
    


   if(boundedIterator.it->data_count > 1 && boundedIterator.it->data[0] < key)
    {
    for(int i = 0; i < boundedIterator.it->data_count; i++)
        {
          if(boundedIterator.it->data[i] == key)
                break;
            else if(boundedIterator.it->data[i] < key && i != data_count-1)
                (boundedIterator.key_ptr)++;
        }
   }
    
    return boundedIterator;
    //cout << "Our value is: " << boundedIterator.it->data[0] << endl;

}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_bounded_node(const T& entry)
{

    int i = first_ge(data, data_count, entry);
    

    if(child_count == 0)
    {
        return this;
    }
    
    else if(data[i] == entry && child_count != 0 && i != data_count)
    {
        // find rights, left most node
        BPlusTree<T>* tempPointer = subset[i+1]->get_bounded_node(entry);
        return tempPointer;

    }


    else {
        //cout << "calling subset" << endl;
        
        BPlusTree<T>* tempPointer = subset[i]->get_bounded_node(entry);
        
        return tempPointer;
    }
}


template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key)  //return first that goes NOT BEFORE
{                                    // key entry or next if does not
    Iterator foundItem = findBound(key);

  

    if(*foundItem < key)
        foundItem++; // upper bound is everything greater 
   
    
   

    return foundItem; 
}
                        
template <typename T>                
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key)  //return first that goes AFTER key
{                                         //exist or not, the next entry  >entry
    Iterator foundItem = findBound(key);


    Iterator original = foundItem;

    
    if(key == *foundItem || key > *foundItem)
        foundItem++; // upper bound is everything greater 
    
    if(foundItem == nullptr)
    {
        return original;
    }
    //cout << "Final iterator item:" << *foundItem << endl;


    return foundItem; 
}

template <typename T>              
int BPlusTree<T>::size() const                    //count the number of elements
{

    return 0;
}

template <typename T>                
bool BPlusTree<T>::empty() const                  //true if the tree is empty
{
    return true;
}

template <typename T>                
void BPlusTree<T>::print_tree(int level, ostream &outs) const
{
    for(int i = data_count; i > 0; i--)
    {
        if(child_count != 0)
            subset[i]->print_tree(level+1, outs);
        
        if(i == data_count)
        {
            if(child_count == 0 && next != nullptr)
                cout << setw(4 * level) << "" <<  "^" << endl;

            cout << setw(4 * level) << "" <<  "⎴" << endl;

        }
        
     
        cout << setw(4*level) << "" << data[i-1] << endl;

    }
    
    cout  << setw(4*level) << "" << "⎵" << endl;

    if(subset[0] != nullptr)
        subset[0]->print_tree(level+1, outs); 

}

template <typename T>                
bool BPlusTree<T>::is_valid()
{
    return true;
}

template <typename T>                
string BPlusTree<T>::in_order()
{
    string inOrder;
    if(child_count == 0)
    {
        for(int i = 0; i < data_count; i++)
            inOrder += to_string(data[i]) + "|";

        return inOrder;
    }

    for(int i = 0;i < child_count; i++)
    {
        inOrder += subset[i]->in_order();
        if(i < data_count)
            inOrder += to_string(data[i]) + "|";
       


    }
    return inOrder;
 

}

template <typename T>                
string BPlusTree<T>::pre_order()
{
    string preOrder;
    if(child_count == 0)
    {
        for(int i = 0; i < data_count; i++)
        {
            preOrder += to_string(data[i]) + "|";
        }
        return preOrder;
    }

    int i;
    for(i = 0; i < data_count; i++)
    {
        preOrder += to_string(data[i]) + "|";
        preOrder += subset[i]->pre_order();
    }
    preOrder += subset[i]->pre_order();
    return preOrder;

}

template <typename T>                
string BPlusTree<T>::post_order()
{
    string postOrder; 
    if(child_count == 0)
    {
        for(int i = 0; i < data_count; i++)
        {
            postOrder += to_string(data[i]) + "|";
        }
        return postOrder;

    }

    for(int i = 0; i < child_count; i++)
    {
        postOrder += subset[i]->post_order();

        if(i > 0)
            postOrder += to_string(data[i-1]) + "|";


    }
    return postOrder;

}

template <typename T>                
typename BPlusTree<T>::Iterator BPlusTree<T>::begin()
{
    Iterator beginningNode;

    beginningNode.it = get_smallest_node();
    
    return beginningNode;    
}

template <typename T>                
typename BPlusTree<T>::Iterator BPlusTree<T>::begin() const
{
    Iterator beginningNode;

    if(data_count == 0)
    {
     
        return NULL;
    }
    beginningNode.it = get_smallest_node();
    // default constructor already sets the (int) key_ptr to 0


    return beginningNode;    
}

template <typename T>                
typename BPlusTree<T>::Iterator BPlusTree<T>::end()
{
    Iterator endIt;
    endIt = nullptr;
    return endIt; 
}    

template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry) {

    //print_array(data, data_count, 0);
    //cout << endl;
    //cout << "find_ptr was called!" << endl;
    //cout << data[1] << endl;

    int i = first_ge(data, data_count, entry);





 
    if(!(data[i] == entry) && child_count == 0)
    {
        return nullptr;
    }


    else if (data[i] == entry && i != data_count) // avoids hitting a null subset pointer
    {
        if(child_count == 0)
        {
            T *returnPointer = &data[i];
            return returnPointer;
        }
        else
        {
            //cout << "IS THIS BEING CALLED??" << endl;
            T *returnPointer = subset[i+1]->find_ptr(entry);
            return returnPointer;
        }
        
    } 

    else {
        //cout << "calling subset" << endl;
        T* tempPointer = subset[i]->find_ptr(entry);
        return tempPointer;
    }
}


template <typename T>                
void BPlusTree<T>::loose_insert(const T& entry)   //allows MAXIMUM+1 data elements in
{                                         //   the root
    int i = first_ge(data, data_count, entry);

    if(data[i] == entry) // if found
    {
        data[i] = entry; // overwrite
    }
    
    else if(child_count == 0)
    {
        

        insert_item(data, i, data_count, entry);

        //cout << data[MAXIMUM] << endl;
        
        //cout << "MAX: " << MAXIMUM << endl;
        
    
    }

    else
    {
        subset[i]->loose_insert(entry);
        if(subset[i]->data_count > MAXIMUM)
        {
            //cout << i << " is the index of the subset where are excess value is in" << endl;
            //cout << "Calling fix_excess" << endl;
            //cout << "Its data is " << data[i] << endl;
            fix_excess(i);
        }
    }
}

template <typename T>                
void BPlusTree<T>::fix_excess(int i)              //fix excess in child i
{
    BPlusTree<T> *newBTree = new BPlusTree<T>();

    insert_item(subset, i+1, child_count, newBTree);


    split(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
    split(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);

    
    T item;
    detach_item(subset[i]->data, subset[i]->data_count, item);
    insert_item(data, i, data_count, item);

    // BPlus tree  
    if(subset[i]->is_leaf())
    {
        insert_item(subset[i+1]->data, 0, subset[i+1]->data_count, item);
        subset[i+1]->next = subset[i]->next; // new guy points to old neighbor
        subset[i]->next = subset[i+1]; // old fat guy points to the new guy
    }

   
}

template <typename T>                
void BPlusTree<T>::loose_remove(const T& entry)  //allows MINIMUM-1 data elements
{


}

template <typename T>                
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i)  //fix shortage in child i
{
    BPlusTree<T> *temp;
    return temp; 

}

template <typename T>                
BPlusTree<T>* BPlusTree<T>::get_smallest_node()
{
    //cout << "Current data count: " << data_count << endl;

    if(data_count == 0)
        return 0;

    if(child_count != 0)
        return subset[0]->get_smallest_node();

    return this;
    

    
   //return temp; 
}

template <typename T>                
BPlusTree<T>* BPlusTree<T>::get_smallest_node() const
{
    if (data_count == 0)
        return nullptr;

    const BPlusTree<T>* current_node = this;
    while (current_node->child_count > 0) {
        current_node = current_node->subset[0];
    }
    
    // returns it as non const
    return const_cast<BPlusTree<T>*>(current_node);
}

    
   

template <typename T>                
void BPlusTree<T>::get_smallest(T& entry)      //entry := leftmost leaf
{
    
}

template <typename T>                
void BPlusTree<T>::get_biggest(T& entry)       //entry := rightmost leaf
{

}

template <typename T>                
void BPlusTree<T>::remove_biggest(T& entry)    //remove the biggest child of tree->entry
{


}

template <typename T>                
void BPlusTree<T>::transfer_left(int i)        //transfer one element LEFT from child i
{

}

template <typename T>                
void BPlusTree<T>::transfer_right(int i)       //transfer one element RIGHT from child i
{

}

template <typename T>                
BPlusTree<T> * BPlusTree<T>::merge_with_next_subset(int i) //merge subset i with  i+1
{

}

// template <typename T>                
// void BPlusTree<T>::check_leaf_link()
// {
//     if(child_count != 0)
//         subset[0]->check_leaf_link();

//     BPlusTree<T>* temp = this;

//     if(is_leaf())
//     {
//         while(temp != nullptr)
//         {
//             print_array(temp->data, temp->data_count, 0);
//             cout << endl;
//             temp = temp->next;
//         }
//     }    
//     cout << endl;

// }


#endif