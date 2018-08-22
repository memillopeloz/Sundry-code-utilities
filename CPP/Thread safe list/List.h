//***************************************************************************
//* File Name:      List.h
//* Description:    This file contains a singly linked, thread safe list
//*                    definition.
//*
//* Revision History:
//===========================================================================
//*  9/10/2016  (GLN) - Initial creation
//***************************************************************************
 
/** \addtogroup List
 *  @{
 *
 *  \file List.h
 *  \brief Header file that contains the specification of a thread-safe singly linked list.
 */
 
#include <memory>
#include <atomic>
#include<iostream>
 
#include "messages/TemsMessageApi.h"
 
using namespace std;
using namespace TEMS::Message;
 
namespace TEMS
{
    namespace TCS
    {
        class slist //FIFO queue structured list
        {
            /**
             * \struct Node List.h
             * \brief Node element that composes List
             *
             * Struct contains element value and a pointer to the next Node
             */
            struct Node {
                TemsMessage* value; /**< Node content of type TemsMessage* */
                atomic<Node*> next; /**< Atomic pointer to next Node */
                Node(TemsMessage* val) {
                    next.store(nullptr);
                    value = val;
                }
            };
            Node *head, *tail;/**< Head is a dummy placeholder node, tail points to last element */
            atomic<bool> lock;/**< Atomic lock for safe concurrent access */
            size_t length;      /**< List length */
 
            slist(slist&) =delete;
            void operator=(slist&) =delete;
        public:
            slist();
 
            ~slist();
 
            void pushFront(TemsMessage *value);
 
            bool popBack(TemsMessage **result);
 
            size_t getLength();
        };
    }
}
/** @}*/
