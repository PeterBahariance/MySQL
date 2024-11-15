#ifndef MYSTACK_H
#define MYSTACK_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"
#include "../node/node.h"

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator()
        {
            _ptr = NULL;
        }            //default ctor
        
        Iterator(node<ITEM_TYPE>* p){

             _ptr = p;                   //Point Iterator to where p is pointing to

        }      
                                           
        ITEM_TYPE operator *(){
                return _ptr->_item;     //dereference operator
        }    
       
       
        bool is_null()
        {
            return _ptr == NULL;  //true if _ptr is NULL
        }           
        
        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        {
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right
        {
                return left._ptr == right._ptr;

        }


        Iterator& operator++(){                        //member operator:++it
                _ptr = _ptr->_next;                    // or ++it = new_value (prefix)
        }

        friend Iterator operator++(Iterator& it,       // postfix
                                   int unused){        //friend operator: it++
            assert(it._ptr != nullptr);

            Iterator temp = it; 
            it._ptr = it._ptr->_next;  // moves iterator to the next node
            return temp;  // returns the original state of the pointer
            
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack()
    {
        _top = nullptr;
        _size = 0;

    }
    
    // the big  three
    Stack(const Stack<ITEM_TYPE>& copyMe) : _size(copyMe._size), _top(nullptr)
    {   
        if (copyMe._top != nullptr) {
            _top = _copy_list(copyMe._top);
        }
        
    }

    ~Stack()
    {
        _size = 0;
        _clear_list(_top);
        _top = nullptr;
    }

    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS)
    {
        _clear_list(_top); // clears the original list, then copies over the new list
        _size = 0;

        if (RHS._top != nullptr) {
            _top = _copy_list(RHS._top);
        }
        _size = RHS._size;

        return *this;


    }


    ITEM_TYPE top()
    {
        assert(!empty());
        return _top->_item;
        
    }

    bool empty()
    {
        if(_top == nullptr)
            return true;
        else
            return false;
          
    }

    void push(ITEM_TYPE item)
    {
        _size++;
        _insert_head(_top, item);
    }

    ITEM_TYPE pop()
    {
        assert(!empty() && "The stack is empty, we cant pop!");
        _size--;
        return _delete_node(_top, _top);

      

    
        
    }

    template<typename T>
    friend ostream& operator<<(ostream& outs, const Stack<T>& printMe)
    {
        node<T>* current = printMe._top;

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
    
    Iterator begin() const //Iterator to the head node
    {
        return Iterator(_top);
    }                   
    
    
    Iterator end() const //Iterator to NULL
    {
        return Iterator(nullptr);
    }                    

    
    int size() const 
    { 
        return _size; 
    }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};







#endif 
