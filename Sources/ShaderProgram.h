#pragma once

#include "pch.h"

#include <Kore/Graphics4/Graphics.h>
#include <Kore/Graphics4/PipelineState.h>

using namespace Kore;

class ShaderProgram {
public:
	ShaderProgram(const char* vsFile, const char* fsFile, Graphics4::VertexStructure& structure, bool depthWrite)
	{
		// Load and link the shaders
		FileReader vs("shader.vert");
		FileReader fs("shader.frag");
		vertexShader = new Graphics4::Shader(vs.readAll(), vs.size(), Graphics4::VertexShader);
		fragmentShader = new Graphics4::Shader(fs.readAll(), fs.size(), Graphics4::FragmentShader);
		
		pipeline = new Graphics4::PipelineState;
		pipeline->inputLayout[0] = &structure;
		pipeline->inputLayout[1] = nullptr;
		pipeline->vertexShader = vertexShader;
		pipeline->fragmentShader = fragmentShader;
		pipeline->depthMode = Graphics4::ZCompareLess;
		pipeline->depthWrite = depthWrite;
		pipeline->blendSource = Graphics4::BlendingOperation::SourceAlpha;
		pipeline->blendDestination = Graphics4::BlendingOperation::InverseSourceAlpha;
		pipeline->compile();
		
		pvLocation = pipeline->getConstantLocation("PV");
		mLocation = pipeline->getConstantLocation("M");
		tintLocation = pipeline->getConstantLocation("tint");
	}
	
	// Update this program from the scene parameters
	virtual void setPipeline()
	{
		// Important: We need to set the program before we set a uniform
		Graphics4::setPipeline(pipeline);
	}
	
	virtual void setModelMatrix(const mat4 &M) {
		Graphics4::setMatrix(mLocation, M);
	}
	
	virtual void setProjectionViewMatrix(const mat4 &PV) {
		Graphics4::setMatrix(pvLocation, PV);
	}
	
	virtual void setTint(const vec4 &tint) {
		Graphics4::setFloat4(tintLocation, tint);
	}
	
protected:
	Graphics4::Shader* vertexShader;
	Graphics4::Shader* fragmentShader;
	Graphics4::PipelineState* pipeline;
	
	// Uniform locations - add more as you see fit
	Graphics4::ConstantLocation pvLocation;
	Graphics4::ConstantLocation mLocation;
	Graphics4::ConstantLocation tintLocation;
};
