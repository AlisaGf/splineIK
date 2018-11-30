//
// Copyright (C) 
// 
// File: splineIkNodeCmd.cpp
//
// MEL Command: splineIkNode
//
// Author: Maya Plug-in Wizard 2.0
//

// Includes everything needed to register a simple MEL command with Maya.
// 
#include "splineIkNode.h"

#include <maya/MObject.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <cMath>

//add variables
MTypeId splineIkNode::typeId(0x80005);
MObject splineIkNode::curveInitLength;
MObject splineIkNode::curveCurrentLength;
MObject splineIkNode::stretch;
MObject splineIkNode::global;
MObject splineIkNode::globalX;
MObject splineIkNode::globalY;
MObject splineIkNode::globalZ;
MObject splineIkNode::scale;
MObject splineIkNode::scaleX;
MObject splineIkNode::scaleY;
MObject splineIkNode::scaleZ;


//Class
splineIkNode::splineIkNode()
{

}

void* splineIkNode::creator()
{
	return new splineIkNode();
}

MStatus splineIkNode::initialize()
{
	MFnNumericAttribute numFn;

	curveInitLength = numFn.create("curveInitLength", "cil", MFnNumericData::kDouble);
	numFn.setKeyable(true);
	numFn.setStorable(true);
	numFn.setWritable(true);
	addAttribute(curveInitLength);

	curveCurrentLength = numFn.create("curveCurrentLength", "ccl", MFnNumericData::kDouble);
	numFn.setKeyable(true);
	numFn.setStorable(true);
	numFn.setWritable(true);
	addAttribute(curveCurrentLength);

	stretch = numFn.create("stretch", "stretch", MFnNumericData::kDouble);
	numFn.setKeyable(true);
	numFn.setStorable(true);
	numFn.setWritable(true);
	numFn.setMin(0.0);
	numFn.setMax(1.0);
	addAttribute(stretch);

	globalX = numFn.create("globalX", "gx", MFnNumericData::kDouble, 1);
	numFn.setKeyable(true);
	numFn.setStorable(true);
	numFn.setWritable(true);
	addAttribute(globalX);

	globalY = numFn.create("globalY", "gy", MFnNumericData::kDouble, 1);
	numFn.setKeyable(true);
	numFn.setStorable(true);
	numFn.setWritable(true);
	addAttribute(globalY);

	globalZ = numFn.create("globalZ", "gz", MFnNumericData::kDouble, 1);
	numFn.setKeyable(true);
	numFn.setStorable(true);
	numFn.setWritable(true);
	addAttribute(globalZ);

	global = numFn.create("global", "g", globalX, globalY, globalZ);
	numFn.setKeyable(true);
	numFn.setStorable(true);
	numFn.setWritable(true);
	addAttribute(global);

	scaleX = numFn.create("scaleX", "sx", MFnNumericData::kDouble);
	numFn.setKeyable(false);
	numFn.setStorable(false);
	numFn.setWritable(false);
	addAttribute(scaleX);

	scaleY = numFn.create("scaleY", "sy", MFnNumericData::kDouble);
	numFn.setKeyable(false);
	numFn.setStorable(false);
	numFn.setWritable(false);
	addAttribute(scaleY);

	scaleZ = numFn.create("scaleZ", "sz", MFnNumericData::kDouble);
	numFn.setKeyable(false);
	numFn.setStorable(false);
	numFn.setWritable(false);
	addAttribute(scaleZ);

	scale = numFn.create("scale", "s", scaleX, scaleY, scaleZ);
	numFn.setKeyable(false);
	numFn.setStorable(false);
	numFn.setWritable(false);
	addAttribute(scale);

	// connect inputs and outputs
	attributeAffects(curveInitLength, scale);
	attributeAffects(curveCurrentLength, scale);
	attributeAffects(stretch, scale);
	attributeAffects(global, scale);
	attributeAffects(globalX, scale);
	attributeAffects(globalY, scale);
	attributeAffects(globalZ, scale);

	attributeAffects(curveInitLength, scaleX);
	attributeAffects(curveCurrentLength, scaleX);
	attributeAffects(stretch, scaleX);
	attributeAffects(global, scaleX);
	attributeAffects(globalX, scaleX);
	attributeAffects(globalY, scaleX);
	attributeAffects(globalZ, scaleX);

	attributeAffects(curveInitLength, scaleY);
	attributeAffects(curveCurrentLength, scaleY);
	attributeAffects(stretch, scaleY);
	attributeAffects(global, scaleY);
	attributeAffects(globalX, scaleY);
	attributeAffects(globalY, scaleY);
	attributeAffects(globalZ, scaleY);

	attributeAffects(curveInitLength, scaleZ);
	attributeAffects(curveCurrentLength, scaleZ);
	attributeAffects(stretch, scaleZ);
	attributeAffects(global, scaleZ);
	attributeAffects(globalX, scaleZ);
	attributeAffects(globalY, scaleZ);
	attributeAffects(globalZ, scaleZ);


	return MS::kSuccess;
}

MStatus splineIkNode::compute(const MPlug& plug, MDataBlock& dataBlock)
{
	if ((plug == scale) || (plug == scaleX) || (plug == scaleY) || (plug == scaleZ))
	{
		//Get initial length input
		double curveInitLengthV = dataBlock.inputValue(curveInitLength).asDouble();
		double curveCurrentLengthV = dataBlock.inputValue(curveCurrentLength).asDouble();
		double stretchV = dataBlock.inputValue(stretch).asDouble();

		//double globalV = dataBlock.inputValue(global).asDouble();
		double globalXV = dataBlock.inputValue(globalX).asDouble();
		double globalYV = dataBlock.inputValue(globalY).asDouble();
		double globalZV = dataBlock.inputValue(globalZ).asDouble();

		double outScaleXV;
		double outScaleYV;
		double outScaleZV;

		//Compute delta
		double n = curveCurrentLengthV / curveInitLengthV;
		outScaleXV = (((curveCurrentLengthV / globalXV - curveInitLengthV)*stretchV + curveInitLengthV) * n) / curveCurrentLengthV / globalXV;

		outScaleYV = 1 / (sqrt((((curveCurrentLengthV / globalYV - curveInitLengthV)*stretchV + curveInitLengthV) * n) / curveCurrentLengthV) / globalYV);
		outScaleZV = 1 / (sqrt((((curveCurrentLengthV / globalZV - curveInitLengthV)*stretchV + curveInitLengthV) * n) / curveCurrentLengthV) / globalZV);

		//set the output values
		dataBlock.outputValue(scale).set(outScaleXV, outScaleYV, outScaleZV);
		dataBlock.outputValue(scale).setClean();

	}

	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MStatus stat;

	MFnPlugin fnPlugin(obj, "Alisa Gafarova", "1.0", "Any");

	stat = fnPlugin.registerNode("splineIkNode", splineIkNode::typeId, splineIkNode::creator, splineIkNode::initialize, MPxNode::kDependNode);

	if (stat != MS::kSuccess)
		stat.perror("Could not register stretchy Ik node");

	return stat;

}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin fnPlugin;
	fnPlugin.deregisterNode((0x80005));

	return MS::kSuccess;
}

