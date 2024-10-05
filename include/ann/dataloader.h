/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
#include "list/listheader.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader {
public:
    
private:
    Dataset<DType, LType>* dataset_ptr;
    int batch_size;
    int batch_num;
    bool shuffle;
    bool drop_last;
    /*TODO: add more member variables to support the iteration*/
    xt::xarray<unsigned long> id;
    XArrayList<Batch<DType, LType>> batch;
public:
    DataLoader(Dataset<DType, LType>* dataset_ptr, int batch_size,
             bool shuffle = true, bool drop_last = false) {
        this->dataset_ptr = dataset_ptr;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;

        if (this->shuffle) {
            id = xt::arange<int>(dataset_ptr->len());
            xt::random::shuffle(id);
        } else {
            id = xt::arange<int>(dataset_ptr->len());
        }
        batch = XArrayList<Batch<DType, LType>>(nullptr, nullptr, batch_num);
        this->batch_num = dataset_ptr->len()/batch_size;

        if(drop_last) {
            for(int i = 0; i < batch_num; ++i) {
                int start = i * batch_size;

                xt::svector<unsigned long> ds = dataset_ptr->get_data_shape();
                xt::svector<unsigned long> ls = dataset_ptr->get_label_shape();

                xt::svector<unsigned long> ds_1(ds.begin(), ds.end());
                xt::svector<unsigned long> ls_1(ls.begin(), ls.end());

                ds_1[0] = batch_size;
                ls_1[0] = batch_size;

                xt::xarray<DType> batch_data(ds_1); 
                xt::xarray<LType> batch_label(ls_1);

                for(int j = 0; j < batch_size; ++j) {
                    DataLabel a = dataset_ptr->getitem(id[start + j]);
                    xt::view(batch_data, j) = a.getData();
                    if(batch_label.dimension() != 0) {
                        xt::view(batch_label, j) = a.getLabel();
                    } else {
                        batch_label = a.getLabel();
                    }
                }
                batch.add(Batch<DType, LType>(batch_data, batch_label));         
            }
        } else {
            int remained = max(0, this->dataset_ptr->len() - batch_num * batch_size);
            if(remained == 0){
                for (int i = 0; i < batch_num; ++i) {
                    int start = i * batch_size;

                    xt::svector<unsigned long> ds = dataset_ptr->get_data_shape();
                    xt::svector<unsigned long> ls = dataset_ptr->get_label_shape();

                    xt::svector<unsigned long> ds_1(ds.begin(), ds.end());
                    xt::svector<unsigned long> ls_1(ls.begin(), ls.end());

                    ds_1[0] = batch_size;
                    ls_1[0] = batch_size;

                    xt::xarray<DType> batch_data(ds_1);
                    xt::xarray<LType> batch_label(ls_1);

                    for (int j = 0; j < batch_size; ++j) {
                        DataLabel a = dataset_ptr->getitem(id[start + j]);
                        xt::view(batch_data, j) = a.getData();
                        if(batch_label.dimension() != 0) {
                            xt::view(batch_label, j) = a.getLabel();
                        }else {
                            batch_label = a.getLabel();
                        }
                    }
                    batch.add(Batch<DType, LType>(batch_data, batch_label));
                }
            } else {
                if(this->dataset_ptr->len() < batch_size)
                    batch_num = max(1, batch_num);
                if(batch_num == 1) {
                    int start = 0;

                    xt::svector<unsigned long> ds = dataset_ptr->get_data_shape();
                    xt::svector<unsigned long> ls = dataset_ptr->get_label_shape();

                    xt::svector<unsigned long> ds_1(ds.begin(), ds.end());
                    xt::svector<unsigned long> ls_1(ls.begin(), ls.end());

                    int s = this->dataset_ptr->len() < batch_size ? remained : remained + batch_size;

                    ds_1[0] = s;
                    ls_1[0] = s;

                    xt::xarray<DType> batch_data(ds_1);
                    xt::xarray<LType> batch_label(ls_1);
            
                    for (int j = 0; j < s; ++j) {
                        DataLabel a = dataset_ptr->getitem(id[start + j]);
                        xt::view(batch_data, j) = a.getData();
                        if(batch_label.dimension() != 0 ){
                            xt::view(batch_label, j) = a.getLabel();
                        } else {
                            batch_label = a.getLabel();
                        }
                    }
                    batch.add(Batch<DType, LType>(batch_data, batch_label));   
                } else {
                    for (int i = 0; i < batch_num; ++i) {
                        int start = i * batch_size;

                        xt::svector<unsigned long> ds = dataset_ptr->get_data_shape();
                        xt::svector<unsigned long> ls = dataset_ptr->get_label_shape();

                        xt::svector<unsigned long> ds_1(ds.begin(), ds.end());
                        xt::svector<unsigned long> ls_1(ls.begin(), ls.end());
                        
                        if(i == batch_num - 1) {
                            ds_1[0] = batch_size + remained;
                            ls_1[0] = batch_size + remained;
                        } else {
                            ds_1[0] = batch_size;
                            ls_1[0] = batch_size;
                        }
                        xt::xarray<DType> batch_data(ds_1);
                        xt::xarray<LType> batch_label(ls_1);
                        if(i != batch_num - 1){
                            for (int j = 0; j < batch_size; ++j) {
                                DataLabel a = dataset_ptr->getitem(id[start + j]);
                                xt::view(batch_data, j) = a.getData();
                                if(batch_label.dimension() != 0) {
                                    xt::view(batch_label, j) = a.getLabel();
                                } else {
                                    batch_label = a.getLabel();
                                }
                            }
                            batch.add(Batch<DType, LType>(batch_data, batch_label));
                        } else {
                            for (int j = 0; j < (batch_size + remained); ++j) {
                                DataLabel a = dataset_ptr->getitem(id[start + j]);
                                xt::view(batch_data, j) = a.getData();
                                if(batch_label.dimension() != 0) {
                                    xt::view(batch_label, j) = a.getLabel();
                                } else {
                                    batch_label = a.getLabel();
                                } 
                            }
                            batch.add(Batch<DType, LType>(batch_data, batch_label));
                        }
                    }
                }
            }
        }
    }
    virtual ~DataLoader(){}
    
    class Iterator {
    private:
        int cursor;
        DataLoader<DType, LType>* data_ld;

    public:
        Iterator(int cursor = 0, DataLoader* data_ld = nullptr) : cursor(cursor), data_ld(data_ld) {}

        Iterator& operator=(const Iterator& it) {
            if(*this == &it) return *this;
            cursor = it.cursor;
            data_ld = it.data_ld;
            return *this;
        }

        Iterator& operator++() {
            this->cursor++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }   

        bool operator!=(const Iterator& it) const {
            return it.cursor != this->cursor;
        }

        Batch<DType, LType> operator*() const {
            return this->data_ld->batch.get(cursor);
        }

  };

    Iterator begin() {
        return Iterator(0, this);
    }

    Iterator end() {
        return Iterator(batch_num, this);
    }

};
#endif /* DATALOADER_H */

