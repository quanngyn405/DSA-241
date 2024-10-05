/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "ann/xtensor_lib.h"
#include "list/listheader.h"

using namespace std;

template<typename DType, typename LType>
class DataLabel{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label) : data(data), label(label){}
    xt::xarray<DType> getData() const{ return data; }
    xt::xarray<LType> getLabel() const{ return label; }
};

template<typename DType, typename LType>
class Batch{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    Batch() : data(), label(xt::xarray<LType>()) {}
    Batch(xt::xarray<DType> data,  xt::xarray<LType> label) : data(data), label(label){}
    virtual ~Batch(){}
    xt::xarray<DType>& getData(){return data; }
    xt::xarray<LType>& getLabel(){return label; }

    friend ostream& operator<<(std::ostream& os, Batch<DType, LType> b) {
        os << b.getData() << " " << b.getLabel() << endl;  
        return os;
    }

    bool operator==(const Batch& other) const {
        if(data.size() != other.data.size() || label.size() != other.label.size()){
            return false;
        }
        for(auto it1 = data.begin(), it2 = other.data.begin(); it1 != data.end(); ++it1, ++it2){
            if(*it1 != *it2){
                return false;
            }
        }
        for(auto it1 = label.begin(), it2 = other.label.begin(); it1 != label.end(); ++it1, ++it2){
            if(*it1 != *it2){
                return false;
            }
        }
        return true;
    }
};


template<typename DType, typename LType>
class Dataset{
private:
public:
    Dataset(){};
    virtual ~Dataset(){};
    
    virtual int len() = 0;
    virtual DataLabel<DType, LType> getitem(int index)=0;
    virtual xt::svector<unsigned long> get_data_shape()=0;
    virtual xt::svector<unsigned long> get_label_shape()=0;
    
};

//////////////////////////////////////////////////////////////////////
template<typename DType, typename LType>
class TensorDataset: public Dataset<DType, LType>{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
    
public:
    /* TensorDataset: 
     * need to initialize:
     * 1. data, label;
     * 2. data_shape, label_shape
    */
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label) : data(data), label(label) {
        /* TODO: your code is here for the initialization
         */
        data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
        label_shape = xt::svector<unsigned long>(label.shape().begin(), label.shape().end());
        
        // data_shape = data.shape();
        // label_shape = label.shape();
    }
    /* len():
     *  return the size of dimension 0
    */
    int len(){
        /* TODO: your code is here to return the dataset's length
         */
        return data.shape(0); 
    }
    
    /* getitem:
     * return the data item (of type: DataLabel) that is specified by index
     */
    DataLabel<DType, LType> getitem(int index){
        /* TODO: your code is here
         */
        if(idx < 0 || idx >= this->len()) {
            throw std::out_of_range("Index is out of range!");
        }

        if(label.dimension() == 0) {
            return DataLabel<DType, LType>(xt::view(data, idx), label);
        }
        return DataLabel<DType, LType>(xt::view(data, idx), xt::view(label, idx));
    }
    
    xt::svector<unsigned long> get_data_shape(){
        /* TODO: your code is here to return data_shape
         */
        return data_shape;
    }
    xt::svector<unsigned long> get_label_shape(){
        /* TODO: your code is here to return label_shape
         */
        return label_shape;
    }
};
#endif /* DATASET_H */