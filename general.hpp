
#pragma once

#include <string>
#include <iostream>
using std::string;

using std::ostream;

class Printable
{
public:
    virtual string to_string() const;
    friend ostream& operator<<(ostream&, const Printable&);
};


void warn (string msg);
void die (string msg);


