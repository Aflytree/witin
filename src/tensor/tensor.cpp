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
		//shape = s;
		cout<<"size = "<<s.size()<<endl;
		cout<<"shape.size = "<<shape.size()<<endl;
		for(auto kv : s){
			cout<<"setShape = "<<kv<<endl;
			shape.push_back(kv);
			cout<<"shape.size = "<<shape.size()<<endl;
			cout<<"setShape = "<<kv<<endl;
		}
    }
    
	std::vector<int> Tensor::getShape() const {
		return shape;
    }
	
	int Tensor::setData(void * data_in) {
		int size = 1;
		for(auto sp : shape){
			size *= sp;
		}
		__data = (char*)malloc(size);
		memcpy(__data, data_in, size);
	}
	
    void* Tensor::getData() const {
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
		cout<<"=========================="<<endl;
		cout<<"Tensor shape is:"<<endl;
		for(auto sp : getShape()){
			cout<<sp<<" ";
		}
		cout<<endl;
		cout<<"Tensor type:"<<tensor_type<<endl;	
		if(tensor_type == CONST_TYPE)
		{
			cout<<"Data is:"<<endl;
			int size = 0;
			getSize(size);
			for(int i = 0;i < size; i++){
				cout<<(int)((char*)__data)[i]<<" ";
				if(i % 10 == 0 && i != 0)
					cout<<endl;
			}
		}
		cout<<endl;
		cout<<"=========================="<<endl;
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
		cout<<"size1 = "<<size<<endl;
		data = (char*)malloc(size);
		
		if(random)
		{
			for(int i = 0; i < shape[0]; i++)
			{
				for(int j = 0; j < shape[1]; j++)
				{
					data[i * shape[1] + j] = (char)(i + j);
					//cout<<"data:"<<(int)(char)(i+j)<<endl;
					//cout<<"data:"<<(int)data[i*shape[1] + j]<<endl;
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
