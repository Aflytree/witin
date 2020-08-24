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
#include <witin/session/session.h>

using namespace std;
using namespace witin::node;
using namespace witin::graph;
using namespace witin::math;
using namespace witin::session;
using namespace witin::tensor;

typedef witin::node::OpNode base1OpNode;

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
    auto absNode1 = std::make_shared<AbsNode>(1,"abs");
	
    std::shared_ptr <base1OpNode> absNode =
                        std::make_shared<AbsNode>(1,"abs");
    std::shared_ptr <base1OpNode> logNode =
                        std::make_shared<LogNode>(2,"log");
    std::shared_ptr <base1OpNode> logNode1 =
                        std::make_shared<LogNode>(2,"log1");
    // fy::graph::FyGraphType* graph =  new fy::graph::FyGraphType();
    witin::graph::WitinGraphType graph;
    EdgeProperty ep;
    ep.src = 10;
    ep.dst = 11;
    graph.addNode(absNode);
    graph.addNode(logNode);
    // graph.addNode(logNode1);
    graph.addEdge(absNode, logNode, ep);
    // graph.addEdge(logNode, logNode1, ep);
    graph.printAllNodes();

    // graph.printAllNodes();
    graph.printAllEdges();
    graph.print();
	
	vector<int> shape;
	char * data;
	
	int size = 1;
	for(int i = 1; i < 4; i++){
		size *= i;
		shape.push_back(i);
	}
	char aa[size];
	for(int j = 0;j < size; j++){
		aa[j] = j;
	}

	data = (char*)malloc(size);
	memcpy(data, aa, size * sizeof(char));
    //Tensor
	Tensor input_tensor;
	input_tensor.setShape(shape);
	input_tensor.setData((void*)data);
	input_tensor.print();
	std::vector<Tensor> input_tensors;
	input_tensors.push_back(input_tensor);

	////Session
	Session ss;
	ss.build(graph);
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




