#include <iostream>
#include <thread>
#include "internal/List.h"
#include "internal/TCSConfig.h"
using namespace std;
 
#define POPt 990
#define PUSHt 1000
 
std::thread addThreads[PUSHt];
std::thread popThreads[POPt];
 
int main() {
    auto* lista = new slist();
    cout << "creating threads" << endl;
    int i = 0;
 
    for (i = 0; i<PUSHt; i++)
        addThreads[i] = thread([lista, i]() -> void {string temp = "t" + std::to_string(i+45); lista->push_front(temp);});
    for (i = 0; i<POPt; i++)
        popThreads[i] = thread([lista]() ->
                void {
                    string temp = "";
                    if(lista->pop_back(temp))
                        cout<<"popped: "<<temp<<endl;
                    else
                        cout<<"pop failed"<<endl;
                });
 
 
    cout<<"joining threads"<<endl;
    for (i = 0; i<PUSHt; i++)
    {
        addThreads[i].join();
        if(i<POPt)
            popThreads[i].join();
    }
 
    cout<<"printing list"<<endl;
    lista->print();
 
    delete lista;
    return 0;
}
