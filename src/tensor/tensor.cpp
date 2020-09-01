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
		for(auto kv : s){
			shape.push_back(kv);
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
		
		cout<<"Data is:"<<endl;
		int size = 0;
		getSize(size);
		for(int i = 0;i < size; i++){
			cout<<(int)((char*)__data)[i]<<" ";
			if(i % 10 == 0 && i != 0)
				cout<<endl;
		}
		cout<<endl;
		cout<<"=========================="<<endl;
	}


}// namespace tensor

}// namespace witin
