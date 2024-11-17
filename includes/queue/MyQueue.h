#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"
#include "../tokenizor/token.h"
#include "../node/node.h"

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator()
        {
            _ptr = NULL; //default ctor
        }                          
        Iterator(node<T>* p){
                                    //Point Iterator to where
                _ptr = p; 
        }                            
                                                          //  p is pointing to
        T operator *(){
                return _ptr->_item;
                //dereference operator

        }    

        bool is_null(){

                return _ptr == NULL;
                //true if _ptr is NULL
        }           
       
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {
            return left._ptr != right._ptr;

        }

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {

            return left._ptr == right._ptr;
        }


        Iterator& operator++(){                         //member operator:++it
                                                        // or ++it = new_value
             _ptr = _ptr->_next;  
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){         //friend operator: it++
       
            assert(it._ptr != nullptr);

            Iterator temp = it; 
            it._ptr = it._ptr->_next;  // moves iterator to the next node
            return temp;  // returns the original state of the pointer
            
       
        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue()
    {
        _front = nullptr;
        _rear = nullptr;
        _size = 0;

    }

    Queue(const Queue<T>& copyMe) : _size(copyMe._size), _front(nullptr), _rear(nullptr)
    {
        if (copyMe._front != nullptr) {
            _front = _copy_list(copyMe._front);
        }

    }
    ~Queue()
    {
        _size = 0;
        _clear_list(_front);
        _front = nullptr;
        _rear = nullptr;

    }

    Queue& operator=(const Queue<T>& RHS)
    {
        _clear_list(_front); // clears the original list, then copies over the new list
        _size = 0;

        if (RHS._front != nullptr) {
            _front = _copy_list(RHS._front);
        }
        _size = RHS._size;

        return *this;

    }



    bool empty()
    {
        if(_front == nullptr)
            return true;
        else
            return false;

    }

    T front()
    {
        assert(!empty() && "LIST IS EMPTY!");
        return _front->_item;

    }

    T back()
    {
        assert(!empty() && "LIST IS EMPTY!");
        return _rear->_item;

    }

    void push(T item)
    {
        // push to the back 

        if(_front == nullptr) // we check if there is no nodes in the list 
        {

            _rear = _insert_after(_front, _rear, item); // since the list was empty initially, the front and rear both point to the same thing
            _front = _rear; 
            _size++;

        }
        else if(_front->_next == nullptr) // we check if there is only one node in the list 
        {
            _rear = _insert_after(_front, _rear, item);
            _size++;

          



        }

        else
        {
            _rear = _insert_after(_front, _rear, item);
            _size++;

        }

        

        //_insert_head(_front, item);

    }
    T pop()
    {

        _size--;

        if(_front == _rear)
            _rear = nullptr;

        
        return _delete_node(_front, _front);

    

      

    }

    Iterator begin() const //Iterator to the head node
    {                   
            return Iterator(_front);
    }                                     
    
    
    Iterator end() const //Iterator to NULL
    {
        return Iterator(nullptr);


    }                     

    void print_pointers()
    {
        _print_list(_front);

    }
    
    int size() const 
    { 
        return _size; 
    }
    
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe)
    {
        node<T>* current = printMe._front;

        while (current != nullptr)
        {
            outs << "<-[" << current->_item << "]";
            if (current->_next != nullptr)
            {
            outs << "->";
            }
            current = current->_next;
        }

    return outs;

    }

    bool onlyNode()
    {
        if(_front != nullptr)
        {
            if(_front->_next != nullptr)
                return true;
            else
                cout << "ONLY ONE NODE" << endl;

        }
        else
        {
            cout << "NO NODES" << endl;
            return false;
        }

    }
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

#endif 