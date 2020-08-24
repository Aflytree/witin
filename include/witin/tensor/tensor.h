/*
 * @Author: your name
 * @Date: 2020-08-22 10:19:32
 * @LastEditTime: 2020-08-22 22:38:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/witin/include/fy/tensor.h
 */
#pragma once
#ifndef _TENSOR_H
#define _TENSOR_H

#include <vector>
using namespace std;

namespace witin{
namespace tensor{

class TensorNode{

};

class operation{


};


// template <typename T>
class Tensor{
    //inline const TensorNode* operator ->();
    // typedef T T_
    Tensor(void*ptr);
    inline bool operator ==(const Tensor & other) const;
    inline bool operator !=(const Tensor & other) const;
    inline size_t ndim() const;
    inline void setShape(std::vector<int> s);
    inline std::vector<int> getShape() const;
    inline void *getData() const;
    /*bytes*/
    inline void size() const;

    public:
        operation* op;

    private:
        std::vector<int> shape;
        void* data;
};
}// namespace tensor
}// namespace witin

#endif //TENSOR_H

