/*
  ==============================================================================

    MultiGrainTable.h
    Created: 2 Jan 2018 11:44:30am
    Author:  spearse

  ==============================================================================
*/

#pragma once
#include "GrainTable.h"
#include <vector>


class MultiGrainTable{
public:
    typedef std::vector<GrainTable*> MultiGrainTableStorage;
private:
    MultiGrainTableStorage m_tableStorage;
public:
    MultiGrainTable();
    ~MultiGrainTable();
    void create_table();
    GrainTable* get_table(int index);
    GrainTable* get_lastTable();
    
    double get_sample(double tableIndex,double sampleIndex);
    int get_numberTables();
    
    
};


