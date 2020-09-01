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
#include <boost/version.hpp>
#include <dmlc/logging.h>
#include <witin/node/node.h>
#include <witin/graph/graph.h>
#include <witin/op/math.h>
#include <witin/op/nn.h>
#include <witin/session/session.h>
#include <witin/global.h>

using namespace std;
//using namespace witin::node;
using namespace witin::graph;
using namespace witin::math;
using namespace witin::session;
using namespace witin::base;
using namespace witin::nn;

typedef witin::base::OpNode base1OpNode;

int fillTensor(Tensor &t, vector<int> shape){
	
	
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
			data[i * shape[1] + j] = i + j;		
		}
	}

	t.setShape(shape);
	t.setData((void*)data);
	return 0;
}

int main(int argc, char *argv[])
{
    DLOG(INFO)<<"main test";
    std::cout << "Boost version: "  //1.58.0
          << BOOST_VERSION / 100000
          << "."
          << BOOST_VERSION / 100 % 1000
          << "."
          << BOOST_VERSION % 100
          << std::endl;
    auto absNode1 = std::make_shared<AbsOpNode>(1,"abs");
	
	Tensor mv_tensor;

	vector<int> mv_shape;
	mv_shape.push_back(8);
	mv_shape.push_back(18);
	
    fillTensor(mv_tensor, mv_shape);
	mv_tensor.print();
	std::shared_ptr <base1OpNode> mvNode =
                        std::make_shared<mvOpNode>(mv_tensor, MV_OPNODE_ID,"mvOpNode");
    std::shared_ptr <base1OpNode> absNode =
                        std::make_shared<AbsOpNode>(ABS_OPNODE_ID,"abs");
    std::shared_ptr <base1OpNode> logNode =
                        std::make_shared<LogOpNode>(2,"log");
    std::shared_ptr <base1OpNode> logNode1 =
                        std::make_shared<LogOpNode>(2,"log1");
    // fy::graph::FyGraphType* graph =  new fy::graph::FyGraphType();
    witin::graph::WitinGraphType graph;
    EdgeProperty ep1;
    ep1.src = 10;
    ep1.dst = 11;
    
	EdgeProperty ep2;
    ep2.src = 12;
    ep2.dst = 13;
	
	graph.addNode(mvNode);
    graph.addNode(absNode);
    graph.addNode(logNode);
    graph.addNode(logNode1);
    
	graph.addEdge(mvNode, absNode, ep1);
	graph.addEdge(absNode, logNode, ep2);
    EdgeProperty ep3;
    ep3.src = 14;
    ep3.src = 15;
    graph.addEdge(logNode, logNode1, ep3);
    // graph.addEdge(logNode, logNode1, ep);
    //graph.printAllNodes();

    //graph.printAllEdges();
    graph.print();
	
	auto out = graph.outNodes();
	cout <<"out.size() = "<<out.size()<<endl;
	auto in = graph.inNodes();
	cout <<"in.size() = "<<in.size()<<endl;

	vector<int> shape;
	vector<vector<int> > shapes;

	shape.push_back(1);
	shape.push_back(8);
	
	shapes.push_back(shape);
    //Tensor
	Tensor input_tensor;
    fillTensor(input_tensor, shape);
	input_tensor.print();
	
	std::vector<Tensor> input_tensors;
	input_tensors.push_back(input_tensor);

	////Session
	Session ss;
	ss.build(graph,shapes);
	ss.run(graph, input_tensors);

	//free data;


	// auto s = graph.getAllNodes();
    // DLOG(INFO) <<" nodes size: "<<s.size();

    // graph->AddVertex(logNode);
    // DLOG(INFO)<<"add abs node.";

    // EdgeProperty edge;
    // edge.src=0;
    // edge.dst=1;
    // graph->addEdge(baseNode, absNode1, edge);
    // DLOG(INFO)<<"add log node.";
    // graph->addNode(logNode);
    // DLOG(INFO)<<"get all node.";
    // graph->getAllNodes();
    // graph->print();

    return 0;
}




