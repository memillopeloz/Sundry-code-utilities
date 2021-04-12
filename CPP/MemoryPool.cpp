// code source: https://jsdw.me/posts/cpp-memory-pools/
#include<iostream>
#include<stack>

using namespace std;

template<typename Type>
class MemoryPool
{
private:
    // Stack to hold pointers to free memory chunks
    std::stack<Type*> FreeMem;
public:
    MemoryPool() {}

    //Forbid copy constructor
    MemoryPool(const MemoryPool &)=delete;

    //Allow move constructor
    MemoryPool(MemoryPool&& other)
    {
        this->FreeMem(std::move(other.FreeMem));
    }

    //Allocate a chunk of memory as big as Type needs
    void* allocate()
    {
        void* MemAddr;
        if(!FreeMem.empty())
        {
            MemAddr = static_cast<void*>(FreeMem.top());
            FreeMem.pop();
        }
        else
        {
            MemAddr = ::operator new(sizeof(Type));//similar to C's malloc
        }
        return MemAddr;
    }

    //Mark some memory as available (no longer used)
    void deallocate(void* o)
    {
        FreeMem.push(static_cast<Type*>(o));
    }

    //Create instance of Type
    // The create function takes in a variable number of arguments (whatever we would normally pass to the constructor).
    // It then uses the allocate function to provide a chunk of memory to use, but in addition, it forwards the arguments 
    // passed in to the constructor of whatever Type is, and constructs an instance of the object using *placement new*. 
    // It then returns a pointer of Type which points to that instance.
    // NOTE the try/catch block surrounding the placement new; if for some reason our object type throws an error on 
    // construction, we want to free up that memory again to avoid potential leaks.
    template<typename... Args>
    Type* create(Args&& ...args)
    {
        Type* ObjAddr = (Type*)(allocate());
        try
        {
            new (ObjAddr) Type(std::forward<Args>(args)...); //placement new
        }
        catch(...)
        {
            FreeMem.push(ObjAddr); throw;
        }
        return ObjAddr;
    }

    //Remove an instance of Type (add memory to the pool)
    // The remove function simply adds a pointer to the location in memory that is passed in to the free stack, just like 
    // deallocate, but in addition calls the destructor of the object to do any cleaning up that it requires
    void remove(Type* o)
    {
        o->~Type();
        FreeMem.push(o);
    }

    //Delete all of the available memory chunks:
    ~MemoryPool()
    {
        while(!FreeMem.empty())
        {
            ::operator delete(FreeMem.top());
            FreeMem.pop();
        }
    }
};

//TEST SECTION ----------->
struct SomeObject
{
    int val;
    SomeObject(int val_in): val(val_in) {}
};
    
void usePool()
{
    std::stack<SomeObject*> s;
    MemoryPool<SomeObject> p;
    for(unsigned i = 0; i < 100; i++)
    {
        for(unsigned j = 0; j < 1000; j++)
        {
            s.push(p.create(1));
        }
        while(!s.empty())
        {
            p.remove(s.top());
            s.pop();
        }
    }
}

void noPool()
{
    std::stack<SomeObject*> s;
    for(unsigned i = 0; i < 100; i++)
    {
        for(unsigned j = 0; j < 1000; j++)
        {
            s.push(new SomeObject(1));
        }
        while(!s.empty())
        {
            delete s.top();
            s.pop();
        }
    }
}

int main()
{
    MemoryPool<SomeObject> p;

    //create an instance of SomeObject from the pool:
    SomeObject* o = p.create(10);

    //destroy it when done, putting memory back into the pool:
    p.remove(o);
    
    cout<<"SUCCESS"<<endl;

    return 0;
}
//<------------ TEST SECTION
//Use with Valgrind or similar profiler to actually see the difference in instructions executed
//Results from source page:
//usePool(): 7,925,308
//noPool(): 25,636,536
/*int main()
{
    usePool();
    noPool();
    return 0;
}*/
