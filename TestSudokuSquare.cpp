#include "SudokuSquare.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

#include <iostream>

using std::cout;
using std::endl;


int main() {
    
    int retval = 0;
    
    {
        cout << "Making a SudokuSquareSet with the numbers 1 to 9 in it, size should be 9: ";
        SudokuSquareSet s;
        for (int i = 1; i <= 9; ++i) {
            s.insert(i);
        }
        if (s.size() == 9) {
            cout << "passed\n";
        } else {
            cout << "failed -- size() returned " << s.size() << "\n";
            retval = 1;
        } 
    
        
        {
            cout << "Making a SudokuSquareSet with the numbers 1 to 9 in it, looping over it should get {1,2,3,4,5,6,7,9}: ";
        
            vector<int> gotThese;
            
            for (int val : s) {
                gotThese.push_back(val);
            }
            
            
            vector<int> shouldBe{1,2,3,4,5,6,7,8,9};
            
            if (gotThese == shouldBe) {
                cout << "passed\n";
            } else {
                cout << "failed, got the values {";
                for (auto n : gotThese) {
                    cout << " " << n;
                }
                cout << " }\n";
                retval = 1;
            }
        }
        
        s.erase(1);
        s.erase(4);
        s.erase(7);
        
        
        {

            cout << "Making a SudokuSquareSet with the numbers 1 to 9 in it, erasing 1, 4 and 7, and looping over it should get {2,3,5,6,9}: ";

            vector<int> gotThese;
            
            for (int val : s) {
                gotThese.push_back(val);
            }
            
            
            vector<int> shouldBe{2,3,5,6,8,9};
            
            if (gotThese == shouldBe) {
                cout << "passed\n";
            } else {
                cout << "failed, got the values {";
                for (auto n : gotThese) {
                    cout << " " << n;
                }
                cout << " }\n";
                retval = 1;
            }
        }
        
        {
            cout << "Looking in the SudokuSquareSet containing {2,3,5,6,9} for 2 returns an iterator pointing to 2: ";
            
            auto itr = s.find(2);
            
            if (itr == s.end()) {
                cout << " failed, returned end()\n";
                retval = 1;
            } else if (*itr != 2) {
                cout << " failed, returned a number other than 2\n";
                retval = 1;
            } else {
                cout << " passed\n";
            }
        }
        
        {
            cout << "Looking in the SudokuSquareSet containing {2,3,5,6,9} for 7 returns an iterator that is the same as end(): ";
            
            auto itr = s.find(7);
            
            if (itr != s.end()) {
                cout << " failed, returned something other than end()\n";
                retval = 1;
            } else {
                cout << " passed\n";
            }
        }
        
        
        
    }
    
    return retval;
}
