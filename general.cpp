
#include "general.hpp"


#include <sstream>

using std::cout;
using std::cerr;
using std::endl;
using std::ostringstream;


#include <cstdlib>







string Printable::to_string() const
{
    return "OBJECT()";
}



ostream& operator<<(ostream& os, const Printable& obj)
{
    os << obj.to_string();
    return os;
}



void warn (string msg="warning")
{
    cerr << "warn: " << msg << endl;
}

void die (string msg="spontaneous death")
{
    cerr << "died: " << msg << endl;
    exit(1);
}



