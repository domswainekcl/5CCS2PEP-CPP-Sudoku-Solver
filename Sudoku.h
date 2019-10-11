#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include "SudokuSquare.h"
#include <cmath>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>
using namespace std;

// START

class GridCell{
private:
    int x;
    int y;
    int dimension;
    int block;
    SudokuSquareSet values;
public:
    GridCell(int xCoord, int yCoord, int dimensionIn): x(xCoord), y(yCoord), dimension(dimensionIn), block(calcBlock(x,y,dimension)), values(){
        setDefaultCellValues();
    }
    int getValue() const{
        return (values.size() >= 2) ? (-1) : *values.begin();
    }
    const string printValue() const {
        return (values.size() >= 2) ? " " : to_string(*values.begin());
    }
    void setSingleValue(int value){
        values.clear();
        values.insert(value);
    }
    void setDefaultCellValues(){
        values.clear();
        for(int i = 0; i<dimension;i++){
            values.insert(i+1);
        }
    }
    const int getX() const {
        return x;
    }
    const int getY() const {
        return y;
    }
    const double getBlock() const {
        return block;
    }
    static const int calcBlock(int xIn, int yIn, int dimIn) {
        return (int)(floor(yIn/sqrt(dimIn))*sqrt(dimIn)+floor(xIn/sqrt(dimIn)));
    }
    const bool dependent(const GridCell* other) const {
        return ( other->getX() == getX() || other->getY() == getY() || other->getBlock() == getBlock() )
                    && !(other->getX() == getX() && other->getY() == getY());
    }
    bool removeValue(int value){
        values.erase(value);
        return !values.empty();
    }
    const SudokuSquareSet getValuesSet() const{
        return values;
    }
    const bool isSet() const{
        return getNumValues() == 1;
    }
    const int getNumValues() const{
        return values.size();
    }
};

class Sudoku : public Searchable {
private:
    vector<GridCell> cells;
    int dimension;
public:
    explicit Sudoku(int dimensionIn): cells(), dimension(dimensionIn){
        auto numCells = (unsigned long) pow(dimension,2);
        cells.reserve(numCells);
        for(int i=0; i<numCells; i++){
            cells.emplace_back(GridCell(i%dimension, (int)floor(i/dimension), dimension));
        }
    }
    GridCell* getGridCell(int i){
        return &cells[i];
    }
    GridCell* getGridCell(int x, int y) {
        return getGridCell(y * dimension + x);
    }
    const GridCell* getGridCell(int x, int y) const {
        return getGridCell(y * dimension + x);
    }
    const GridCell* getGridCell(int i) const {
        return &cells[i];
    }
    int getSquare(int row, int col) const{
        return getGridCell(row, col)->getValue();
    }
    bool setSquare(int row, int col, int value){
        GridCell* cellToUpdate = getGridCell(row, col);
        cellToUpdate->setSingleValue(value);
        return cascadeUpdate(cellToUpdate);
    }
    bool cascadeUpdate(const GridCell* updatedCell){
        int updatedCellValue = updatedCell->getValue();
        if(updatedCellValue != -1){
            vector<GridCell*> row, col, bck;
            row.reserve(dimension); col.reserve(dimension); bck.reserve(dimension);

            for(int i = 0; i<cells.size(); i++){
                GridCell* loopCell = getGridCell(i);
                if(updatedCell->dependent(loopCell)) {
                    if (loopCell->getValuesSet().contains(updatedCellValue)) {
                        if (!loopCell->removeValue(updatedCellValue)) return false;
                        if (!cascadeUpdate(loopCell)) return false;
                    }
                    if (updatedCell->getX() == loopCell->getX()) row.push_back(loopCell);
                    if (updatedCell->getY() == loopCell->getY()) col.push_back(loopCell);
                    if (updatedCell->getBlock() == loopCell->getBlock()) bck.push_back(loopCell);
                }
            }
            if(!betterSetSquare(row)) return false;
            if(!betterSetSquare(col)) return false;
            if(!betterSetSquare(bck)) return false;
        }
        return true;
    }
    bool betterSetSquare(const vector<GridCell*> seg) {
        for(GridCell* cell: seg){
            int len = cell->getNumValues();
            if(len == 2 || len == 3 || len == 4 ){
                vector<GridCell*> nonMatchingCells(seg.size());
                auto it = copy_if (seg.begin(), seg.end(), nonMatchingCells.begin(), [cell](GridCell* c){
                    return (c->getValuesSet()!=cell->getValuesSet());
                });
                nonMatchingCells.resize(distance(nonMatchingCells.begin(),it));
                if(nonMatchingCells.size() < dimension-len){
                    for(auto innerLoopCell: nonMatchingCells){
                        int count = 0;
                        for(int valueToRemove: cell->getValuesSet()){
                            if(innerLoopCell->getValuesSet().contains(valueToRemove)){
                                if (!innerLoopCell->removeValue(valueToRemove)) return false;
                                count++;
                            }
                        }
                        if (count >=1 && !cascadeUpdate(innerLoopCell)) return false;
                    }
                }
            }
        }
        return true;
    }
    bool isSolution() const override {
        return all_of(cells.begin(), cells.end(), [](GridCell c){return c.isSet();});
    }
    void write(ostream & o) const override {
        o << endl;
        for(int i = 0; i<cells.size(); i++){
            o << getGridCell(i)->printValue();
            if(i%dimension == dimension-1) o << "\n";
        }
        o << endl;
    }
    const GridCell* getFirstUnsetCell() const{
        for(int i = 0; i<cells.size(); i++){
            const GridCell* loopCell = &cells[i];
            if(!(loopCell->isSet())){return loopCell;}
        }
        return nullptr;
    }
    vector<unique_ptr<Searchable> > successors() const override {
        vector<unique_ptr<Searchable>> successorsList;
        const GridCell* hasUnsetValue = getFirstUnsetCell();
        int xCoord = hasUnsetValue->getX();
        int yCoord = hasUnsetValue->getY();
        for(int possibleValue: hasUnsetValue->getValuesSet()){
            auto sudokuCopy = new Sudoku(*this);
            if(sudokuCopy->setSquare(xCoord,yCoord,possibleValue)){
                successorsList.emplace_back(unique_ptr<Sudoku>(sudokuCopy));
                //sudokuCopy->write(cout);
            }
            else delete sudokuCopy;
        }
        if(successorsList.size() == 1){
            const Sudoku output = dynamic_cast<Sudoku&>(*successorsList[0].get());
            if(!output.isSolution()) return output.successors();
        }
        return successorsList;
    }
    int heuristicValue() const override {
        return (int)count_if(cells.begin(), cells.end(), [](GridCell c){return !c.isSet();});
    }
};

// END

#endif