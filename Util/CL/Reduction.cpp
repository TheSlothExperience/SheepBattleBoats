#include "Reduction.h"
#include <GL/glx.h>
#include <vector>
#include <QFile>
Reduction* Reduction::m_instance = NULL;

Reduction* Reduction::instance() {
	if(!m_instance) {
		m_instance = new Reduction;
	}
	return m_instance;
}


Reduction::Reduction() {
	if(initContextResources()) {
		std::cout << "OpenCL context initialized" << std::endl;
	}
}

Reduction::~Reduction() {
	std::cout << "Releasing OpenCL resources" << std::endl;
	cleanupContextResources();
}

bool Reduction::initContextResources() {
	//error code
	cl_int clError;

	//get platform ID
	V_RETURN_FALSE_CL(clGetPlatformIDs(1, &clPlatform, NULL), "Failed to get CL platform ID");

	cl_uint numberDevices = 0;
	//get a reference to the first available GPU device
	V_RETURN_FALSE_CL(clGetDeviceIDs(clPlatform, CL_DEVICE_TYPE_GPU, 0, 0, &numberDevices), "No GPU device found.");
	cout << "Found " << numberDevices << " devices" << endl;
	std::vector<cl_device_id> devicesIds(numberDevices);
	V_RETURN_FALSE_CL(clGetDeviceIDs(clPlatform, CL_DEVICE_TYPE_GPU, numberDevices, devicesIds.data(), NULL), "No GPU device found.");

	//Additional attributes to OpenCL context creation
	//which associate an OpenGL context with the OpenCL context
	cl_context_properties props[] = {
		//OpenCL platform
		CL_CONTEXT_PLATFORM, (cl_context_properties)   clPlatform,
		//OpenGL context
		CL_GL_CONTEXT_KHR,   (cl_context_properties)   glXGetCurrentContext(),
		CL_GLX_DISPLAY_KHR , (cl_context_properties) glXGetCurrentDisplay() ,
		0
	};

	for(auto dev : devicesIds) {
		cl_device_id deviceToTry = dev;
		cl_context contextToTry = 0;

		contextToTry = clCreateContext(
			props,
			1, &deviceToTry,
			0, 0,
			&clError);
		if(clError == CL_SUCCESS) {
			clDevice = deviceToTry;
			clContext = contextToTry;
			break;
		}
	}

	char deviceName[1024];
	V_RETURN_FALSE_CL(clGetDeviceInfo(clDevice, CL_DEVICE_NAME, 256, &deviceName, NULL), "Unable to query device name.");
	cout << "Device: " << deviceName << endl;

	//Finally, create the command queue. All the asynchronous commands to the device will be issued
	//from the CPU into this queue. This way the host program can continue the execution until some results
	//from that device are needed.
	clCommandQueue = clCreateCommandQueue(clContext, clDevice, 0, &clError);
	V_RETURN_FALSE_CL(clError, "Failed to create the command queue in the context");


	//Now create and compile the programs
	size_t programSize = 0;

	QFile f(":/shaders/Reduce.cl");
	if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;


	std::string programCodeStr = std::string(f.readAll().data());
	const char *programCode = programCodeStr.c_str();
	programSize = f.size();

	clProgram = clCreateProgramWithSource(clContext, 1, (const char**) &programCode, &programSize, &clError);
	V_RETURN_FALSE_CL(clError, "Failed to create program file");

	clError = clBuildProgram(clProgram, 1, &clDevice, NULL, NULL, NULL);

	if(clError != CL_SUCCESS) {
		PrintBuildLog(clProgram, clDevice);
		return false;
	}

	reduceHorizontalKernel = clCreateKernel(clProgram, "ReduceHorizontal", &clError);
	V_RETURN_FALSE_CL(clError, "Failed to compile kernel: ReduceHorizontal");
	reduceVerticalKernel = clCreateKernel(clProgram, "ReduceVertical", &clError);
	V_RETURN_FALSE_CL(clError, "Failed to compile kernel: ReduceVertical");
	return true;
}

void Reduction::cleanupContextResources() {
	if(clCommandQueue)	clReleaseCommandQueue(clCommandQueue);
	if(clContext)			clReleaseContext(clContext);
}

inline void checkCL(cl_int err, std::string msg = "") {
	if(CL_SUCCESS != err) {
		std::cout << "CL Error: " << err << ", " << errorToString(err) << " " << msg << std::endl;
	}
}

void Reduction::computeSATGLTexture(GLuint src, GLuint dest, int width, int height) {
    cl_int err = 5;
	cl_mem clSrc, clDest;
    clSrc = clCreateFromGLTexture2D(clContext, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, src, &err);
	checkCL(err, "creating src");
	clDest = clCreateFromGLTexture2D(clContext, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, dest, &err);
	checkCL(err, "creating dest");

	glFinish();
	clEnqueueAcquireGLObjects(clCommandQueue, 1,  &clSrc, 0, 0, NULL);
	checkCL(err, "acquiring src");
	clEnqueueAcquireGLObjects(clCommandQueue, 1,  &clDest, 0, 0, NULL);
	checkCL(err, "acquiring dest");

	//Pass the images
	//Horizontal pass
	err = clSetKernelArg(reduceHorizontalKernel, 0, sizeof(clSrc), &clSrc);
	checkCL(err, "passing src");
	err = clSetKernelArg(reduceHorizontalKernel, 1, sizeof(clSrc), &clSrc);
	checkCL(err, "passing dest");
	err = clSetKernelArg(reduceHorizontalKernel, 2, sizeof(int), &width);
	checkCL(err, "passing width");
	err = clSetKernelArg(reduceHorizontalKernel, 3, sizeof(int), &height);
	checkCL(err, "passing height");
	err = clSetKernelArg(reduceHorizontalKernel, 4, width * height * sizeof(cl_float4), NULL);
	checkCL(err, "passing temp");

	clFinish(clCommandQueue);
	clEnqueueReleaseGLObjects(clCommandQueue, 1,  &clSrc, 0, 0, NULL);
	checkCL(err, "Releasing src");
	clEnqueueReleaseGLObjects(clCommandQueue, 1,  &clDest, 0, 0, NULL);
	checkCL(err, "Releasing dest");
	clReleaseMemObject(clSrc);
	clReleaseMemObject(clDest);
}
