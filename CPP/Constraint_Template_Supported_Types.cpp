#include <iostream>
#include <type_traits>

template<class T,
    std::enable_if_t<
        std::is_floating_point<T>::value ||
        std::is_integral<T>::value
    , bool> = true
>
int test(T param)
{
    //This function supports floating point and integral types only
    //https://en.cppreference.com/w/cpp/types/is_integral
    //https://en.cppreference.com/w/cpp/types/is_floating_point
    return 0;
}

int main()
{
    double t = 0.0;
    unsigned long t2 = 0;
    char* t3 = "Unsupported Type"
    
    std::cout << "Hello, Good Sir! \n";
  
    test(t);
    test(t2);
    //test(t3);//Uncommenting this would make compilation fail
    
    return 0;
}

