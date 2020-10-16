/*
 * @Author: your name
 * @Date: 2020-08-22 11:04:29
 * @LastEditTime: 2020-08-22 16:39:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings EditO
 * @FilePath: /tvm/home/afly/work/witin/include/witin/sim/lib.h
 */

#pragma once
#ifndef _COMPUTE_LIB_H
#define _COMPUTE_LIB_H

#include <witin/global.h>
#include <witin/tensor/tensor.h>

namespace witin{
namespace base{

    vector<int16_t> ComputeArrayMac(Tensor *t1, Tensor *t2);
    vector<int16_t> ComputeBiasAdd(vector<int16_t> data, vector<int16_t> bias);
    vector<char> ComputeRelu(vector<char> t1);
    vector<char> ComputeSigmoid(vector<char> t1);
    vector<char> ComputeTanh(vector<char> t1);

} // namespace base
} // namespace witin

#endif //_COMPUTE_LIB_H



