/*
 ==============================================================================
 
 MultiGrainTable.cpp
 Created: 2 Jan 2018 11:44:30am
 Author:  spearse
 
 ==============================================================================
 */

#include "MultiGrainTable.h"




MultiGrainTable::MultiGrainTable(){
    
    create_table();
    get_lastTable()->fill_hann();
    create_table();
    GrainTable* t = get_lastTable();
    t->set_size(44100);
    t->fill_sine(440);
    
    
}

MultiGrainTable::~MultiGrainTable(){
    
    for(int n = 0 ; n < m_tableStorage.size();++n){
        
        delete m_tableStorage[n];
    }
}


void MultiGrainTable::create_table(){
    m_tableStorage.push_back(new GrainTable(512));
}
GrainTable* MultiGrainTable::get_table(int index){
    if(index < 0 )index = 0;
    if(index >= m_tableStorage.size())index = m_tableStorage.size() -1;
    return m_tableStorage[index];
}
GrainTable* MultiGrainTable::get_lastTable(){
    if(m_tableStorage.size() == 0) return nullptr;
    return m_tableStorage[ m_tableStorage.size()-1  ];
}


double MultiGrainTable::get_sample(double tableIndex,double sampleIndex){
    
    double fractpart, intpart;
    fractpart = modf(tableIndex, &intpart);
    int index0(intpart);
    if(index0 < 0)index0= 0;
    //if whole number is being used... don't bother with interpolation
    if(tableIndex == floor(tableIndex)){
        return m_tableStorage[index0]->get_sample(sampleIndex);
    }
    int multiSize = m_tableStorage.size() ;
    int index1(index0 +1);
    while (index1 > multiSize)index1 -= multiSize;
    while (index1 < 0)index1 += multiSize;
    

    
    double index0sample = m_tableStorage[index0]->get_sample(sampleIndex);
    double index1sample = m_tableStorage[index1]->get_sample(sampleIndex);
    
    
    
    
    return (index0sample * (1-fractpart) + index1sample* fractpart  );
    
}

int MultiGrainTable::get_numberTables(){
    return m_tableStorage.size();
}

