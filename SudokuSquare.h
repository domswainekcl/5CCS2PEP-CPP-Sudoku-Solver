#ifndef SUDOKUSQUARE_H
#define SUDOKUSQUARE_H

// Do not add any #include statements to this file

class SudokuSquareSet {
private:
    const static int numberOfBytes = 4;
    unsigned int values;
    int set_size;
public:
    SudokuSquareSet():values(0), set_size(0) {}
    int size() const {
        return set_size;
    }
    bool empty() const {
        return size() == 0;
    }
    void clear(){
        values = 0;
        set_size = 0;
    }
    const unsigned int getValues() const {
        return values;
    }
    const bool operator==(const SudokuSquareSet& other) const{
        return getValues() == other.getValues();
    }
    const bool operator!=(const SudokuSquareSet& other) const{
        return !operator==(other);
    }
    struct SudokuSquareSetIterator {
        int currentBit;
        const SudokuSquareSet* set;
        SudokuSquareSetIterator(int bitNo, const SudokuSquareSet* setIn): currentBit(bitNo), set(setIn){}
        const int getCurrentBit() const {
            return currentBit;
        }
        const SudokuSquareSet* getSetRef() const {
            return set;
        }
        bool operator==(const SudokuSquareSetIterator& other){
            return getSetRef() == other.getSetRef() && getCurrentBit() == other.getCurrentBit();
        }
        bool operator!=(const SudokuSquareSetIterator& other){
            return !operator==(other);
        }
        void operator++(){
            currentBit = set->getFirstSetBitIndex(currentBit+1);
        }
        const int operator*() const{
            return currentBit+1;
        }
    };
    const int getFirstSetBitIndex() const{
        return getFirstSetBitIndex(0);
    }
    const int getFirstSetBitIndex(int startSearch) const{
        for(int i = startSearch; i<numberOfBytes*8; i++){
            if(((values>>i)&1U) == 1) return i;
        }
        return -1;
    }
    const SudokuSquareSetIterator begin() const {
        return {getFirstSetBitIndex(),this};
    }
    const SudokuSquareSetIterator end() const{
        return {-1,this};
    }
    SudokuSquareSetIterator insert(int value){
        values |= 1UL << (value-1);
        set_size++;
        return {value-1,this};
    }
    void erase(int value){
        values &= ~(1UL << (value-1));
        set_size--;
    }
    void erase(SudokuSquareSetIterator iterator){
        values &= ~(1UL << iterator.getCurrentBit());
        set_size--;
    }
    SudokuSquareSetIterator find(int value) const{
        return ((values>>(value-1))&1U) ? SudokuSquareSetIterator(value-1,this) : end();
    }
    const bool contains(int value) const {
        return ((values>>(value-1))&1U);
    }
};

// Do not write any code below this line
static_assert(sizeof(SudokuSquareSet) == sizeof(unsigned int) + sizeof(int), "The SudokuSquareSet class needs to have exactly two 'int' member variables, and no others");
#endif
