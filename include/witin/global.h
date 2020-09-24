/*
 * @Author: your name
 * @Date: 2020-08-22 11:04:29
 * @LastEditTime: 2020-08-27 16:39:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings EditO
 * @FilePath: /tvm/home/afly/work/witin/include/global.h
 */

#pragma once

#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <string>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <utility>
#include <deque>
#include <map>
//#include <json/json.h>
#include </usr/local/include/json/json.h>
#include <dmlc/logging.h>

using namespace std;

enum BASE_OPNODE_ID
{
	MV_OPNODE_ID = 0,
	POOL_OPNODE_ID,
	TANH_OPNODE_ID,
	SIGMOID_OPNODE_ID,
	RELU_OPNODE_ID,
	LOG_OPNODE_ID ,
	ABS_OPNODE_ID ,
	MAX_OPNODE_ID ,
	MIN_OPNODE_ID ,
	INVERT_OPNODE_ID ,
	ADD_OPNODE_ID,
	SHIFT_OPNODE_ID
};

enum OTHER_OPNODE_ID{
	CONST_NODE_ID=100
};

#define WITIN_DEBUG 0

#endif
