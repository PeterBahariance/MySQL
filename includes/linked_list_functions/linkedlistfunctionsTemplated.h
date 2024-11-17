#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H

#include <iostream>
#include "../node/node.h"

using namespace std;

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head);

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head);

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head,  T key);


template <typename T>
node<T>* _insert_head(node<T> *&head,  T insert_this);

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,  node<T> *after_this, T insert_this);

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,  node<T>* before_this,  T insert_this);

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this);

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head);

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src);



//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

template <typename T>
void _print_list(node<T>* head)
{
    while(head != nullptr)
    {
        cout << head->_item << " ";
        head = head->_next;
    }
}

template <typename T>  
void _print_list_backwards(node<T> *head)
{
    while(head->_next != nullptr)
        head = head->_next;

    while(head != nullptr)
    {
         cout << head->_item << " ";
         head = head->_prev;


    }

}

template <typename T>
node<T>* _search_list(node<T>* head,  T key)
{
    while(head != nullptr)
    {
        if(head->_item == key)
        {
            //cout << head->_item;
            return head;
        }

        head = head->_next;

    }
    // if we don't find the key amongst the list, we just return a node that doesn't point to anything
    //cout << "The key was not found in this list.";
    head = nullptr;
    return head;


}

template <typename T>
node<T>* _insert_head(node<T> *&head,  T insert_this)
{
    // cout << "Here is the original list: ";
    // cout << endl;


    node<T> *temp = new node<T>(insert_this, head, nullptr);

    if(head != nullptr)   
        head->_prev = temp;




    
    head = temp;

     
    
    
    

     return head;



}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,  node<T> *after_this, T insert_this)
{
    if(head == nullptr) // if the list is empty, the node that we want to insert after will just become the head node.
    {
        node<T> *insertedNode = new node<T>(insert_this);
        after_this = insertedNode;
    
        // set the value of the item in the passed in node to the passed in item insert_this

        return after_this;  

    }

    else if(head->_next == nullptr)
    {
        node<T> *insertedNode = new node<T>(insert_this);
        head->_next = insertedNode;
        insertedNode->_prev = head; 

        after_this = insertedNode;
        return after_this;

    }

    else
    {
        node<T> *insertedNode = new node<T>(insert_this);
        
        if(after_this == nullptr)
        {
            return insertedNode;
        }
        
        else if(after_this->_next != nullptr)        
        {
            insertedNode->_next = after_this->_next;
        
            after_this->_next->_prev = insertedNode;

            after_this->_next = insertedNode;
            insertedNode->_prev = after_this;

            return insertedNode;
        }
        
        else
        {
            after_this->_next = insertedNode;
            insertedNode->_prev = after_this;

            return insertedNode;
        }
    }   

   
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,  node<T>* before_this,  T insert_this)
{
    node<T>* insertedNode = new node<T>(insert_this);

    if (head == nullptr || head == before_this)
    {
        insertedNode->_next = head;

        if (head != nullptr) {
            head->_prev = insertedNode;
        }

        head = insertedNode;
        return head;
    }

    else
    {
        insertedNode->_prev = before_this->_prev;
        before_this->_prev->_next = insertedNode;

        before_this->_prev = insertedNode;
        insertedNode->_next = before_this;
        //_print_list(head);
        return insertedNode;


    }  

}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this)
{
    node<T> *priorNode = new node<T>();

    assert(prev_to_this->_prev != nullptr && "There was only one node in the list!");
    priorNode = prev_to_this->_prev;
    return priorNode;
    
   

}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this)
{
    node<T> *newNode = new node<T>();
    newNode = head;
    T item = delete_this->_item;

    if(delete_this == head)
    {
        if(head->_next != nullptr)
        {
            head = head->_next;
            delete delete_this;
            return item;


        }
        else
        {
            head = nullptr;
            delete delete_this;
            return item;

        }


    }

    node<T> *temp = new node<T>();
    while(newNode != delete_this)
    {
        temp = newNode; // keeps track of the node before the deleted node
        newNode = newNode->_next;
    }

    if(delete_this->_next != nullptr)
    {
        temp->_next = delete_this->_next;
        delete_this->_next->_prev = temp;
        delete delete_this;
        return item;
    }  
    else{
        delete_this->_prev = nullptr;
        temp->_next = nullptr;
        delete delete_this;

    }  
    
    
    return item; // in this case that the node that we want isn't in the list itself.


}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head)
{
    node<T> *newHead = new node<T>(head->_item); // creates the first node of the list, our new head node.
    
    node<T> *curr = new node<T>();  // creates a new node called curr
    // curr will be used to traverse through the list and link the newly created nodes together. It will be deleted once finished. 
    curr = newHead; // has it point to the same node newHead is pointing to. 

    head = head->_next;
    
    while(head != nullptr)
    {
        node<T> *newNode = new node<T>(head->_item); // sets the newly created node to hold the value of node we are currently copying.
        curr->_next = newNode; // links the previous node to the new node
        newNode->_prev = curr;

        curr = newNode; // moves our curr pointer, so that it now currently points to the most recently created node.
        head = head->_next; // moves to the next node in the list we are copying.
    }
    curr = nullptr; // sets curr so that it doesn't point to anything, so that its ready to be deleted
    delete curr; // deletes the dynamically created curr pointer, since we are no longer using it link the nodes together.
    

    return newHead;

}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src)
{
    dest = _copy_list(src);
    while(dest->_next != nullptr)
        dest = dest->_next;

    return dest;

   

}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head)
{

    while (head != nullptr)
    {
        node<T>* curr = head;
        head = head->_next;
        delete curr;
    }
    
    //head = nullptr; // makes sure that head isn't pointing to anything

}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos)
{
    if(head->_next == nullptr)
    {
        //cout << head->_item;
        return head->_item;
    }
    else
    {
        int tracker = 1;
        while(tracker != pos && head->_next != nullptr)
        {
            head = head->_next;
            tracker++;
        }

        //cout << head->_item;
        return head->_item;


    }

}

#endif 