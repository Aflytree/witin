/*================================================================
*   
*   @file       ：active.h
*   @author     ：afly
*   @date       ：2020.08.26
*   @description：
*
*================================================================*/
#ifndef _ACTIVE_H
#define _ACTIVE_H


#include <witin/node/node.h>
#include <witin/global.h>

namespace witin{
namespace active{

//using namespace witin::node;

class tanhOpNode : public OpNode{
    public:
		tanhOpNode(){}
		tanhOpNode(int id = TANH_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

class sigmoidOpNode : public OpNode{
    public:
		sigmoidOpNode(){}
		sigmoidOpNode(int id = SIGMOID_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

class reluOpNode : public OpNode{
    public:
		reluOpNode(){}
		reluOpNode(int id = RELU_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};


} //namespace active
} //namespace witin

#endif //_ACTIVE_H




