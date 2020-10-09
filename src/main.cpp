/*
 * @Author: your name
 * @Date: 2020-08-22 12:21:00
 * @LastEditTime: 2020-08-22 22:36:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/witin/src/main.cpp
 */
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <math.h>
#include <boost/version.hpp>
#include <dmlc/logging.h>
//#include <witin/node/node.h>
#include <witin/graph/graph.h>
#include <witin/tensor/tensor.h>
#include <witin/op/nn.h>
#include <witin/session/session.h>

using namespace std;
using namespace witin::base;

typedef witin::base::OpNode base1OpNode;

int fillTensor1(Tensor *t, vector<int> shape){


	char * data;
	int size = 1;
	for(size_t i = 0; i < shape.size();i++)
	{
		size *= shape[i];
	}
	data = (char*)malloc(size);

	for(int i = 0; i < shape[0]; i++)
	{
		for(int j = 0; j < shape[1]; j++)
		{
			data[i * shape[1] + j] = ((i + j) % 127);
			//DLOG(INFO)<<"data:"<<(int)data[i*shape[1] + j];
		}
	}

	//for(auto kv : shape)
	//{
	//	DLOG(INFO)<<"kv = "<<kv;
	//}
	t->setShape(shape);
	t->setData((void*)data);
	free(data);
	return 0;
}

int main(int argc, char *argv[])
{
    //DLOG(INFO)<<"main test";
    DLOG(INFO) << "Boost version: "  //1.58.0
          << BOOST_VERSION / 100000
          << "."
          << BOOST_VERSION / 100 % 1000
          << "."
          << BOOST_VERSION % 100;

    DLOG(INFO) << "JSONCPP version: "  //1.9.3
          << JSONCPP_VERSION_STRING;
	Tensor *mv_tensor1 = (Tensor * )malloc(sizeof(Tensor));
	Tensor *mv_tensor2 = (Tensor * )malloc(sizeof(Tensor));
	Tensor *mv_tensor3 = (Tensor * )malloc(sizeof(Tensor));
	Tensor *mv_tensor4 = (Tensor * )malloc(sizeof(Tensor));

	mv_tensor1->tensor_type = CONST_TYPE;
	mv_tensor2->tensor_type = CONST_TYPE;
	mv_tensor3->tensor_type = CONST_TYPE;
	mv_tensor4->tensor_type = CONST_TYPE;

	vector<int> mv_shape1;
	vector<int> mv_shape2;
	vector<int> mv_shape3;
	vector<int> mv_shape4;
	mv_shape1.push_back(1);
	mv_shape1.push_back(38);
	mv_shape1.push_back(128);
    mv_shape2.push_back(128);
	mv_shape2.push_back(256);
    mv_shape3.push_back(256);
	mv_shape3.push_back(512);
    mv_shape4.push_back(512);
	mv_shape4.push_back(18);

	fillTensor1(mv_tensor1, mv_shape1);
	fillTensor1(mv_tensor2, mv_shape2);
	fillTensor1(mv_tensor3, mv_shape3);
	fillTensor1(mv_tensor4, mv_shape4);
	//mv_tensor1->print();

	vector<int> shape1 = {1, 38};
	vector<int> shape2 = {1, 128};
	vector<int> shape3 = {1, 256};
	vector<int> shape4 = {1, 512};

	vector<vector<int> > mv_input_shape1;
	vector<vector<int> > mv_input_shape2;
	vector<vector<int> > mv_input_shape3;
	vector<vector<int> > mv_input_shape4;

	mv_input_shape1.push_back(shape1);
	mv_input_shape2.push_back(shape2);
	mv_input_shape3.push_back(shape3);
	mv_input_shape4.push_back(shape4);

	std::shared_ptr <base1OpNode> mvNode1 =
                        std::make_shared<mvOpNode>(mv_input_shape1, mv_tensor1, MV_OPNODE_ID, "mvOpNode1");
    std::shared_ptr <base1OpNode> mvNode2 =
						std::make_shared<mvOpNode>(mv_input_shape2, mv_tensor2, MV_OPNODE_ID, "mvOpNode2");
    std::shared_ptr <base1OpNode> mvNode3 =
                        std::make_shared<mvOpNode>(mv_input_shape3, mv_tensor3, MV_OPNODE_ID, "mvOpNode3");
    std::shared_ptr <base1OpNode> mvNode4 =
                        std::make_shared<mvOpNode>(mv_input_shape4, mv_tensor4, MV_OPNODE_ID,"mvOpNode4");
    // fy::graph::FyGraphType* graph =  new fy::graph::FyGraphType();
	witin::base::WitinGraphType graph;
	graph.addNode(mvNode1);
    graph.addNode(mvNode2);
    graph.addNode(mvNode3);
    graph.addNode(mvNode4);

	graph.addEdge(mvNode1, mvNode2, EdgeProperty(0, 0));
	graph.addEdge(mvNode2, mvNode3, EdgeProperty(0, 0));
    graph.addEdge(mvNode3, mvNode4, EdgeProperty(0, 0));
    //graph.printAllNodes();
    //graph.printAllEdges();
    graph.print();

	auto out = graph.outNodes();
	DLOG(INFO) <<"GRAPH output node size = "<<out.size();

	auto in = graph.inNodes();
	DLOG(INFO) <<"GRAPH input node size = "<<in.size();

	vector<int> input_shape;
	vector<vector<int> > shapes;

	input_shape.push_back(1);
	input_shape.push_back(38);

	//shapes.push_back(shape);
    //Tensor
	Tensor *input_tensor = (Tensor * )malloc(sizeof(Tensor));
	input_tensor->tensor_type = CONST_TYPE;
    //fillTensor1(input_tensor, input_shape);
    //
	//input_tensor->print();

	std::vector<Tensor*> input_tensors;
	input_tensors.push_back(input_tensor);

	////Session
	Session ss;
	ss.build(graph,shapes);
	ss.run(graph, input_tensors);

	////free data;


	//// auto s = graph.getAllNodes();
    //// DLOG(INFO) <<" nodes size: "<<s.size();

    //// graph->AddVertex(logNode);
    //// DLOG(INFO)<<"add abs node.";

    //// EdgeProperty edge;
    //// edge.src=0;
    //// edge.dst=1;
    //// graph->addEdge(baseNode, absNode1, edge);
    //// DLOG(INFO)<<"add log node.";
    //// graph->addNode(logNode);
    //// DLOG(INFO)<<"get all node.";
    //// graph->getAllNodes();
    //// graph->print();

    return 0;
}




