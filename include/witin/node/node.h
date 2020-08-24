/*
 * @Author: your name
 * @Date: 2020-08-22 11:04:29
 * @LastEditTime: 2020-08-22 16:39:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings EditO
 * @FilePath: /tvm/home/afly/work/witin/include/fy/node.h
 */

#pragma once
#ifndef ___NODE_H
#define ___NODE_H

#include <witin/node/baseNode.h>
#include <witin/global.h>

using namespace std;

namespace witin{
namespace node{


class OpNode : public node{
    public:
        OpNode(){}
        OpNode(int id, std::string name)
        : node(id, name)
        {
            this->id = id;
            this->name = name;
        }
        std::string getName(){
            return name;
        }
    private:
        int id;
        std::string name;
};

} // namespace node
} // namespace witin

#endif //NODE_H



