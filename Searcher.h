#ifndef SEARCHABLE_H
#define SEARCHABLE_H

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class Searchable {

public:

    virtual ~Searchable() {}
    
    virtual bool isSolution() const = 0;    
    vector<unique_ptr<Searchable> > successors() const = 0;

};

#endif
