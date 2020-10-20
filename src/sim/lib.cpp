/*************************************************************************
	> File Name: lib.cpp
	> Author: afly
	> Mail: aifei.zhang@witintech.com
	> Created Time: Wed Oct 14 18:45:00 2020
 ************************************************************************/
#include <witin/sim/lib.h>

namespace witin{
namespace base{

/*
 * basic lib of npu array mac
 */
vector<int16_t> ComputeArrayMac(Tensor* t1, Tensor* t2)
{
	vector<int> shape1 = t1->getShape();
	vector<int> shape2 = t2->getShape();
	vector<int> ret_shape;
	ret_shape.push_back(1);
	ret_shape.push_back(shape2[1]);
	vector<int16_t> data;

	if(shape1[0] != 1)
		LOG(FATAL)<<"shape1[0] should be 0";
	for(int i = 0; i < shape2[1]; i++) //512
	{
		int16_t result_cr = 0;
		for(int j = 0; j <shape1[1];j++) //440
		{
			result_cr += (((char*)t1->getData())[j]) * (((char*)t2->getData())[j * shape2[1] + i]);
			// if(i == 0 && j < 10)
			// {
			// 	DLOG(INFO)<<j<<" DATA1 = " <<(int)(((char*)t1->getData())[j]);
			// 	DLOG(INFO)<<(j*shape2[1] + i)<<" DATA2 = " <<(int)(((char*)t2->getData())[j * shape2[1] + i]);
			// }

			// if(i == 0 && ( (shape1[1] - 10) < j < shape1[1]))
			// {
			// 	if(j > shape1[1] - 10 && j < shape1[1])
			// 	{
			// 		DLOG(INFO)<<"j = "<<j<<" end DATA1 = " <<(int)(((char*)t1->getData())[j]);
			// 		DLOG(INFO)<<"j = "<<j<<" end DATA2 = " <<(int)(((char*)t2->getData())[j * shape2[1] + i]);
			// 	}
			// }
		}
		data.push_back(result_cr);
	}
	return data;
}

/*
 * basic lib of npu bias add
 */
vector<int16_t> ComputeBiasAdd(vector<int16_t> data, vector<int16_t> bias)
{
	vector<int16_t> ret;
	for(size_t i = 0; i < data.size(); i ++)
		ret.push_back(data[i] + bias[i]);
	return ret;
}

/*
 * basic lib of npu relu
 */
vector<char> ComputeRelu(vector<char> t1)
{
	vector<char> ret;
	for(size_t i = 0; i < t1.size(); i ++)
	{
		if(t1[i] < 0)
			ret.push_back(0);
		else
		{
			ret.push_back(t1[i]);
		}
	}
	return ret;
}

/*
 * basic lib of npu sigmoid
 */
vector<char> ComputeSigmoid(vector<char> t1)
{

}

/*
 * basic lib of npu tanh
 */
vector<char> ComputeTanh(vector<char> t1)
{

}

} // namespace base
} // namespace witin
