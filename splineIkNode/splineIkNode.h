#ifndef splineIkNode_H
#define splineIkNode_H

#include <maya/MTypeId.h>
#include <maya/MPxNode.h>

class splineIkNode : public MPxNode
{
public:

	static void*		creator();

	virtual MStatus		compute(const MPlug& plug, MDataBlock& data);
	static MStatus		initialize();
	splineIkNode();

public:

	static MTypeId		typeId;
	static MObject		curveInitLength;
	static MObject		curveCurrentLength;
	static MObject		stretch;
	static MObject		global;
	static MObject		globalX;
	static MObject		globalY;
	static MObject		globalZ;
	static MObject		scale;
	static MObject		scaleX;
	static MObject		scaleY;
	static MObject		scaleZ;

};

#endif
