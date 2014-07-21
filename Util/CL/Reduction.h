#ifndef REDUCTION_H
#define REDUCTION_H

// All OpenCL headers
#if defined (__APPLE__) || defined(MACOSX)
    #include <OpenCL/opencl.h>
#else
    #include <CL/opencl.h>
    #include <CL/cl_gl.h>
    #include <CL/cl_gl_ext.h>
    #include <GL/gl.h>
#endif
#include "Common.h"
#include <QObject>

class Reduction : public QObject
{
	Q_OBJECT
public:
	static Reduction* instance();
	~Reduction();

	void computeSATGLTexture(GLuint src, GLuint temp, GLuint dest, int width, int height);

private:
	Reduction();
	static Reduction* m_instance;

	cl_context			clContext;
	cl_command_queue	clCommandQueue;
	cl_platform_id		clPlatform;
	cl_device_id		clDevice;

	cl_program          clProgram;
	cl_kernel           reduceHorizontalTransposeKernel;
	cl_kernel           reduceVerticalKernel;

	bool initContextResources();
	void cleanupContextResources();
};

#endif
