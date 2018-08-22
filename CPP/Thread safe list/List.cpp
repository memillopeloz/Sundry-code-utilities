//***************************************************************************
//* File Name:      List.cpp
//* Description:    This file contains a singly linked, thread safe list
//*                    implementation.
//*
//* Revision History:
//===========================================================================
//*  9/10/2016  (GLN) - Initial creation
//***************************************************************************
 
/** \addtogroup List
 *  @{
 *
 *  \file List.cpp
 *  \brief Source file that contains the implementation of a thread-safe singly linked list.
 */
 
#include "internal/List.h"
 
using namespace TEMS::TCS;
 
/**
 * \fn slist()
 * \brief Constructor
 */
slist::slist() : lock(false), length(0){
    head = tail = new Node(nullptr);
}
 
/**
 * \fn ~slist()
 * \brief Destructor
 */
slist::~slist(){
    auto first = head;
    while( first ) {
        auto unlinked = first;
        first = first->next;
        if(unlinked->value != nullptr) delete unlinked->value;
        delete unlinked;
    }
}
 
/**
 * \brief Function to safely add an element to the list
 *
 * This function is guaranteed to add the new element.
 * Element's content is not checked for errors
 *
 * \param[in] value New element of type TemsMessage.
 */
void slist::pushFront(TemsMessage *value){
    auto p = new Node(value);
    while( lock.exchange(true)){}// acquire exclusivity
    tail->next = p;// publish the new item
    tail = p;
    ++length;
    lock = false;// release exclusivity
}
 
/**
 * \brief Function to safely retrieve an element from the list
 *
 * This function is guaranteed to retrieve first element.
 * The only way for the function to fail is if the list was empty
 *
 * \param[out] result Pointer of type TemsMessage to store retrieved element.
 * \return Returns whether the pop was successful.
 */
bool slist::popBack(TemsMessage **result) {
    while(lock.exchange(true)){}//acquire lock ~~~CritSec~~~
    if( head->next.load() != nullptr) {// if queue is not empty
        Node* popped = head->next;
        head->next.exchange(popped->next);
        if(popped == tail)//if there was one element only
            tail = head;
        *result = popped->value;
        delete popped;// clean up popped node
        --length;
        lock= false;// release lock ~~~CritSecEnd~~~
        return true;// report success
    }
    lock= false;// release lock ~~~CritSecEnd~~~
    return false; // queue was empty
}
 
/**
 * \brief Function to retrieve list length
 *
 * \return Returns list size.
 */
size_t slist::getLength() {
    return this->length;
}
/** @}*/
