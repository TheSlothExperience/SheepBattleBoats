#define GL_GLEXT_PROTOTYPES

#include "heightmapnode.h"


HeightMapNode::HeightMapNode(HeightMap *p, std::string name)
	: SceneGraph(p, name)
{
	this->heightMap = p;
}

HeightMapNode::~HeightMapNode() {
	
}

void HeightMapNode::drawGrid(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc) {
    
	MVStack.push(MVStack.top());  
	MVStack.top().translate(this->translation);
    
	//Convert the quat to a matrix, may be a performance leak.
	QMatrix4x4 tempRot;
	tempRot.rotate(this->rotation.normalized());
	MVStack.top() *= tempRot;
    
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, MVStack.top().constData());
    
	this->heightMap->drawGrid();
    
	MVStack.pop();
}
