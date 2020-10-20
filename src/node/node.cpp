/*
 * @Author: your name
 * @Date: 2020-08-22 11:43:43
 * @LastEditTime: 2020-08-22 22:16:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/witin/src/node.cpp
 */
#include <witin/node/node.h>

namespace witin{
namespace base{


    void OpNode::updateOutputTensors(Tensor* ts, Tensor* array_result_tensor)
    {
        Tensor *tensor = new Tensor(ts->getShape(), CONST_TYPE);
        fillTensorPtr(tensor,
		                        ts->getShape(),
		                        ts->getData());
        // DLOG(INFO)<<"bottom_tensors size = "<<bottom_tensors.size();
        bottom_tensors.push_back(tensor);
        // tensor->print();

        Tensor *tensor1 = new Tensor(array_result_tensor->getShape(), CONST_TYPE);
        fillTensorPtr(tensor1,
		                        array_result_tensor->getShape(),
		                        array_result_tensor->getData());
        // tensor1->print();
        // DLOG(INFO)<<"array_output_tensors size = "<<array_output_tensors.size();
        array_output_tensors.push_back(tensor1);
    }

     void OpNode::updateInputTensors(Tensor* ts)
     {

        Tensor *tensor = new Tensor(ts->getShape(), CONST_TYPE);
        fillTensorPtr(tensor,
		                        ts->getShape(),
		                        ts->getData());
        // DLOG(INFO)<<"top_tensors size = "<<top_tensors.size();
        top_tensors.push_back(tensor);

        Tensor *tensor1 = new Tensor(ts->getShape(), CONST_TYPE);
        fillTensorPtr(tensor1,
		                        ts->getShape(),
		                        ts->getData());

        array_input_tensors.push_back(tensor1);

     }

}// namespace node
}// namespace witin