/*================================================================
*   @file       :nn.h
*   @author     :afly
*   @date       :2020.08.25
*   @description:
*
*================================================================*/
#ifndef _NN_OP_H
#define _NN_OP_H

#include <witin/node/node.h>
#include <witin/tensor/tensor.h>
#include <witin/global.h>

namespace witin{
namespace nn{

//using namespace witin::node;
//using namespace witin::base;

class mvOpNode : public OpNode{
    public:
		mvOpNode(){}
        mvOpNode(Tensor t, int id = MV_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
			this->t = t;
            this->id = id;
            this->name = name;
        }
		
		int getTensor(Tensor &in){
			in = t;
			return 0;
		}

    private :
        int id;
        std::string name;
		Tensor t;
};

} //namespace nn
} //namespace witin


#endif //_NN_OP_H



