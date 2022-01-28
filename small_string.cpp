#include <iostream>
#include <cstring>

/*

    Custom String class Implementation with small string optimization.  

    The implementation requires that for small enough strings whose size is 
    less than the template parameter, the string is not dynamically allocated. 

    For strings of size greater than the template parameter, the string is
    allocated as usual dynamically. 

    Static and Dynamic char arrays are used for this purpose.

    All assignment operators (copy and move), copy constructors (copy and move) and
    allocators from raw strings are customized to suit this implementation.

    For out of bounds access, the string throws an exception.

*/

template <std::size_t MAX_OPT_SIZE>
class MyString;

template <std::size_t MAX_OPT_SIZE>
std::ostream &operator<<(std::ostream &os, const MyString<MAX_OPT_SIZE> &s);

template <std::size_t MAX_OPT_SIZE>
class MyString
{
public:
    MyString();
    MyString(const char *str);
    MyString(const MyString &other);
    MyString(MyString &&other) noexcept;
    ~MyString();
    // Assigns value to the string
    MyString &operator=(const MyString &other);
    MyString<MAX_OPT_SIZE> &operator=(MyString<MAX_OPT_SIZE> &&other) noexcept;
    MyString &operator=(const char *str);
    // Return a non-modifiable character array version of the string
    const char *CStr() const;
    // Accesses the specified character
    const char &operator[](std::size_t pos) const;
    char &operator[](std::size_t pos);
    // Return the number of characters
    std::size_t size() const;

private:
    struct longer
    {
        char *data;
    };

    struct shorter
    {
        char data[MAX_OPT_SIZE];
    };

    union repr
    {
        longer lval;
        shorter sval;
    };

    size_t sz{0};
    repr item;
    bool version{false};

    friend std::ostream &operator<<<MAX_OPT_SIZE>(std::ostream &os, const MyString<MAX_OPT_SIZE> &s);
};

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE>::~MyString()
{

    if (version && item.lval.data)
    {
        delete[] item.lval.data;
    }
}

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE>::MyString() : sz{0} {};

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE>::MyString(const char *str) : sz{std::strlen(str)}
{

    if (sz >= MAX_OPT_SIZE)
    {
        version = true;
    }
    else
    {
        version = false;
    }

    if (sz > 0)
    {

        if (!version)
        {
            strcpy(item.sval.data, str);
        }
        else
        {
            item.lval.data = new char[sz + 1];
            strcpy(item.lval.data, str);
        }
    }
}

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE>::MyString(const MyString &other) : sz{other.sz}, version{other.version}
{

    if (sz > 0)
    {
        if (version)
        {
            item.lval.data = new char[sz + 1];
            strcpy(item.lval.data, other.item.lval.data);
        }
        else
        {
            strcpy(item.sval.data, other.item.sval.data);
        }
    }
}

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE>::MyString(MyString &&other) noexcept : sz{other.sz}, version{other.version}
{

    if (sz > 0)
    {
        if (version)
        {

            item.lval.data = other.item.lval.data;
            other.item.lval.data = nullptr;
            other.sz = 0;
        }
        else
        {

            strcpy(item.sval.data, other.item.sval.data);
            other.sz = 0;
            memset(&other.item.sval.data, 0, sizeof(sz));
        }
    }
}

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE> &MyString<MAX_OPT_SIZE>::operator=(const MyString<MAX_OPT_SIZE> &other)
{

    sz = other.sz;
    bool prevVersion{version};
    version = other.version;

    if (prevVersion && item.lval.data)
    {
        delete[] item.lval.data;
    }

    if (sz > 0)
    {
        if (version)
        {
            item.lval.data = new char[sz + 1];
            strcpy(item.lval.data, other.item.lval.data);
        }
        else
        {
            strcpy(item.sval.data, other.item.sval.data);
        }
    }

    return *this;
}

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE> &MyString<MAX_OPT_SIZE>::operator=(MyString<MAX_OPT_SIZE> &&other) noexcept
{

    sz = other.sz;
    bool prevVersion{version};
    version = other.version;

    if (sz > 0)
    {

        if (version)
        {

            item.lval.data = other.item.lval.data;
            other.item.lval.data = nullptr;
            other.sz = 0;
        }
        else
        {

            strcpy(item.sval.data, other.item.sval.data);
            other.sz = 0;
            memset(&other.item.sval.data, 0, sizeof(sz));
        }
    }

    return *this;
}

template <std::size_t MAX_OPT_SIZE>
MyString<MAX_OPT_SIZE> &MyString<MAX_OPT_SIZE>::operator=(const char *str)
{

    sz = std::strlen(str);
    bool prevVersion{version};

    if (prevVersion)
    {
        delete[] item.lval.data;
    }

    if (sz >= MAX_OPT_SIZE)
    {
        version = true;
    }
    else
    {
        version = false;
    }

    if (sz > 0)
    {
        if (!version)
        {
            strcpy(item.sval.data, str);
        }
        else
        {
            item.lval.data = new char[sz + 1];
            strcpy(item.lval.data, str);
        }
    }

    return *this;
};

template <std::size_t MAX_OPT_SIZE>
std::size_t MyString<MAX_OPT_SIZE>::size() const
{
    return sz;
}

template <std::size_t MAX_OPT_SIZE>
const char &MyString<MAX_OPT_SIZE>::operator[](std::size_t pos) const
{

    if (pos < sz)
    {

        if (version)
        {
            return item.lval.data[pos];
        }
        else
        {
            return item.sval.data[pos];
        }
    }
    else
    {
        throw std::out_of_range("Cannot access out of bounds index");
    }
}

template <std::size_t MAX_OPT_SIZE>
char &MyString<MAX_OPT_SIZE>::operator[](std::size_t pos)
{

    if (pos < sz)
    {

        if (version)
        {
            return item.lval.data[pos];
        }
        else
        {
            return item.sval.data[pos];
        }
    }
    else
    {
        throw std::out_of_range("Cannot access out of bounds index");
    }
}

template <std::size_t MAX_OPT_SIZE>
const char *MyString<MAX_OPT_SIZE>::CStr() const
{

    if (sz == 0)
        return nullptr;

    if (version)
    {
        return item.lval.data;
    }
    else
    {
        return item.sval.data;
    }
};

template <std::size_t MAX_OPT_SIZE>
std::ostream &operator<<(std::ostream &os, const MyString<MAX_OPT_SIZE> &s)
{

    if (s.sz > 0)
    {
        if (s.version)
        {
            os << s.item.lval.data;
        }
        else
        {
            os << s.item.sval.data;
        }
    }

    return os;
}

int main()
{

    MyString<2> val("abc");
    MyString<2> val1(std::move(val));
    std::cout << val1.size() << "\t" << val1.CStr() << "\t" << val << "\n";

    MyString<2> val2 = val1;
    MyString<2> val3 = std::move(val2);
    std::cout << val3.size() << "\t" << val3.CStr() << "\t" << val2 << "\n";

    MyString<25> val4 = "abcd";
    MyString<25> val5(std::move(val4));
    MyString<25> val6(std::move(val4));
    std::cout << val5.size() << "\t" << val5.CStr() << "\t" << val4 << "\t" << val6.size() << "\n";

    MyString<25> val7 = "abcdef";
    MyString<25> val8 = (std::move(val7));
    MyString<25> val9 = (std::move(val7));
    std::cout << val8.size() << "\t" << val8.CStr() << "\t" << val7 << "\t" << val9.size() << "\n";

    try
    {
        std::cout << val8[6];
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "Exception caught successfully";
    }

    return 0;
}