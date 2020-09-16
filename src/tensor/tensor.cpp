/*
 * @Author: your name
 * @Date: 2020-08-22 10:28:57
 * @LastEditTime: 2020-08-22 22:16:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/witin/src/tensor.cpp
 */
#include <witin/tensor/tensor.h>
#include <witin/global.h>

namespace witin{
namespace base{

    //Tensor::Tensor(void* ptr){
    Tensor::Tensor(){
    }
    
	bool Tensor::operator ==(const Tensor & other) const {
		 //shape == and data ==
    }

    bool Tensor::operator !=(const Tensor & other) const {
		//shape != or data !=
    }
    
	//size_t Tensor::ndim() const{
	//	return shape.size();
    //}
    
	void Tensor::setShape(std::vector<int> s) {
		//why???	
		shape.resize(0);
		DLOG(INFO)<<"tensor shape size:"<<shape.size();
		for(auto kv : shape)
		{
			DLOG(INFO)<<"1tensor shape:"<<kv;
		}
		for(auto kv : s)
		{
			shape.push_back(kv);
		}
		DLOG(INFO)<<"tensor shape size:"<<shape.size();
    }
    
	std::vector<int> Tensor::getShape() const {
		return shape;
    }
	
	int Tensor::setData(void * data_in) {
		int size = 1;
		for(auto sp : shape){
			size *= sp;
		}
		DLOG(INFO)<<"size:"<<size;
		
		__data = (char*)malloc(size * sizeof(char));
		//int a = 0;	
		//memcpy(__data, &a, size);
		if(__data == NULL){
			DLOG(INFO)<<"tensor setData malloc error : return NULL ";
		}
		
		memcpy(__data, data_in, size);
	}
	
    void* Tensor::getData() const {
		return (void*)__data;
	}
    
	int Tensor::getSize(int &s) const {
		int size = 1;
		for(auto sp : shape){
			size *= sp;
		}
		s = size;
		return 0;
    }

	void Tensor::print() const
	{
		std::vector<int> vt = shape;
		DLOG(INFO)<<"==========================";
		DLOG(INFO)<<"Tensor shape is:";
		for(auto sp : getShape()){
			DLOG(INFO)<<sp<<" ";
		}
		DLOG(INFO);
		DLOG(INFO)<<"Tensor type:"<<tensor_type;	
		if(tensor_type == CONST_TYPE)
		{
			DLOG(INFO)<<"Data is:";
			int size = 0;
			getSize(size);
			for(int i = 0;i < size; i++){
				DLOG(INFO)<<(int)((char*)__data)[i]<<" ";
				if(i % 10 == 0 && i != 0)
					DLOG(INFO);
			}
		}
		DLOG(INFO);
		DLOG(INFO)<<"==========================";
	}

	int fillTensor(Tensor &t, vector<int> shape, void * value
					,bool random = 1)
	{
		char * data;
		int size = 1;
		for(size_t i = 0; i < shape.size();i++)
		{
			size *= shape[i];
		}
		DLOG(INFO)<<"size1 = "<<size;
		data = (char*)malloc(size);
		
		if(random)
		{
			for(int i = 0; i < shape[0]; i++)
			{
				for(int j = 0; j < shape[1]; j++)
				{
					data[i * shape[1] + j] = (char)(i + j);
					//DLOG(INFO)<<"data:"<<(int)(char)(i+j);
					//DLOG(INFO)<<"data:"<<(int)data[i*shape[1] + j];
				}
			}
		}
		t.setShape(shape);
		t.setData((void*)data);
		free(data);
		return 0;
	}




}// namespace tensor

}// namespace witin
