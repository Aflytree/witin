/*
 * @Author: your name
 * @Date: 2020-08-22 11:04:29
 * @LastEditTime: 2020-08-23 16:39:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings EditO
 * @FilePath: /tvm/home/afly/work/witin/include/witin/node/node.h
 */
#pragma once
#ifndef _BASE_OP_NODE_H
#define _BASE_OP_NODE_H

#include "../global.h"

namespace witin{
namespace base{

class node{
    public:
        node(){}
        node(int id, const std::string name){
            this->id = id;
            this->name = name;
        }
    private:
        int id;
        std::string name;
};

} // namespace node
} // namespace witin

#endif //NODE_H



