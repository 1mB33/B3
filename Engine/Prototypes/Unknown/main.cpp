#include <iostream>

using namespace std;

// HPP // -------------------------------------------------------------------------------------------------------------
template <class T>
class Unknown
{
    static inline int index = 0;

  public:
    Unknown();

    static int GetIndex()
    {
        return index;
    }
};
// --------------------------------------------------------------------------------------------------------------------

// CPP // -------------------------------------------------------------------------------------------------------------
static inline int counter = 0;
// --------------------------------------------------------------------------------------------------------------------

// HPP // -------------------------------------------------------------------------------------------------------------
int IncreaseAndGetCounter()
// --------------------------------------------------------------------------------------------------------------------
// CPP // -------------------------------------------------------------------------------------------------------------
{
    return ++counter;
}
// --------------------------------------------------------------------------------------------------------------------

// HPP // -------------------------------------------------------------------------------------------------------------
template <class T>
Unknown<T>::Unknown()
{
    if ( this->index == 0 )
        index = IncreaseAndGetCounter();
}

class Other : public Unknown<Other>
{
};
// --------------------------------------------------------------------------------------------------------------------

int main()
{
    unordered_map<int, void *> mapp = {};

    Unknown<double> a = {};
    Other           b = {};
    Other           c = {};

    mapp[ Unknown<double>::GetIndex() ] = &a;
    mapp[ Other::GetIndex() ]           = &b;
    mapp[ Other::GetIndex() ]           = &c;

    cout << Unknown<double>::GetIndex() << endl;
    cout << Other::GetIndex() << endl;

    cout << "&a: " << &a << endl;
    cout << "&b: " << &b << endl;
    cout << "&c: " << &c << endl;

    cout << "mapp[ Unknown<double>::GetIndex() ]: " << mapp[ Unknown<double>::GetIndex() ] << endl;
    cout << "mapp[ Other::GetIndex() ]" << mapp[ Other::GetIndex() ] << endl;
    cout << "mapp[ Other::GetIndex() ]" << mapp[ Other::GetIndex() ] << endl;

    for ( auto v : mapp )
    {
        cout << v.first << " " << v.second << endl;
    }
}
