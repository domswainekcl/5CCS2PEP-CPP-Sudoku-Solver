#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"
#include <memory>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

// START

struct SearchableComparator {
    bool operator()(const unique_ptr<Searchable>& a, const unique_ptr<Searchable>& b) const {
        return a->heuristicValue() > b->heuristicValue();
    }
};

class BestFirstSearch {

protected:
    int nodes = 0;
    priority_queue<unique_ptr<Searchable>, vector<unique_ptr<Searchable>>, SearchableComparator > q;
public:

    explicit BestFirstSearch(unique_ptr<Searchable> && startFrom) {
        q.push(move(startFrom));
    }
    
    int getNodesExpanded() const {
        return nodes;
    }
    
    Searchable * solve() {

        while (!q.empty()) {

            if (q.top()->isSolution()) {
                return q.top().get();
            }

            ++nodes;
            vector<unique_ptr<Searchable>> successors = q.top()->successors();
            q.pop();

            for (auto & successor : successors) {
                q.push(move(successor));
            }

        } return nullptr;

    }
};


// END

#endif
