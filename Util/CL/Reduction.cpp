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

	reduceHorizontalTransposeKernel = clCreateKernel(clProgram, "ReduceHorizontal", &clError);
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

void Reduction::computeSATGLTexture(GLuint src, GLuint temp, GLuint dest, int width, int height) {
    cl_int err = 5;
    cl_mem clSrc, clTemp, clDest;
    clSrc = clCreateFromGLTexture2D(clContext, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, src, &err);
	checkCL(err, "creating src");
	clTemp = clCreateFromGLTexture2D(clContext, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, temp, &err);
	checkCL(err, "creating temp");
	clDest = clCreateFromGLTexture2D(clContext, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, dest, &err);
	checkCL(err, "creating dest");

	glFinish();
	clEnqueueAcquireGLObjects(clCommandQueue, 1,  &clSrc, 0, 0, NULL);
	checkCL(err, "acquiring src");
	clEnqueueAcquireGLObjects(clCommandQueue, 1,  &clTemp, 0, 0, NULL);
	checkCL(err, "acquiring temp");
	clEnqueueAcquireGLObjects(clCommandQueue, 1,  &clDest, 0, 0, NULL);
	checkCL(err, "acquiring dest");

	//Horizontal pass
	{
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 0, sizeof(clSrc), &clSrc);
		checkCL(err, "passing src");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 1, sizeof(clSrc), &clTemp);
		checkCL(err, "passing dest");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 2, sizeof(int), &width);
		checkCL(err, "passing width");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 3, sizeof(int), &height);
		checkCL(err, "passing height");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 4, width * sizeof(cl_float4), NULL);
		checkCL(err, "passing temp");

		size_t global_work_size[] = {std::min(width / 2, 512), 1};
		size_t local_work_size[] = {std::min(width / 2, 512), 1};
		for(int i = 0; i < height; i++) {
			err = clSetKernelArg(reduceHorizontalTransposeKernel, 5, sizeof(int), &i);
			checkCL(err, "passing row");
			err = clEnqueueNDRangeKernel(clCommandQueue, reduceHorizontalTransposeKernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
			checkCL(err, "starting the kernel " + i);
		}
	}

	//Vertical pass
	{
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 0, sizeof(clSrc), &clTemp);
		checkCL(err, "passing src");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 1, sizeof(clSrc), &clDest);
		checkCL(err, "passing dest");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 2, sizeof(int), &height);
		checkCL(err, "passing width");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 3, sizeof(int), &width);
		checkCL(err, "passing height");
		err = clSetKernelArg(reduceHorizontalTransposeKernel, 4, height * sizeof(cl_float4), NULL);
		checkCL(err, "passing temp");

		size_t global_work_sizeV[] = {std::min(height / 2, 512), 1};
		size_t local_work_sizeV[] = {std::min(height /2, 512), 1};
		for(int i = 0; i < width; i++) {
			err = clSetKernelArg(reduceHorizontalTransposeKernel, 5, sizeof(int), &i);
			checkCL(err, "passing row");
			err = clEnqueueNDRangeKernel(clCommandQueue, reduceHorizontalTransposeKernel, 1, NULL, global_work_sizeV, local_work_sizeV, 0, NULL, NULL);
			checkCL(err, "starting the kernel " + i);
		}
	}



	clFinish(clCommandQueue);
	clEnqueueReleaseGLObjects(clCommandQueue, 1,  &clSrc, 0, 0, NULL);
	checkCL(err, "Releasing src");
	clEnqueueReleaseGLObjects(clCommandQueue, 1,  &clTemp, 0, 0, NULL);
	checkCL(err, "Releasing temp");
	clEnqueueReleaseGLObjects(clCommandQueue, 1,  &clDest, 0, 0, NULL);
	checkCL(err, "Releasing dest");
	clReleaseMemObject(clSrc);
	clReleaseMemObject(clTemp);
	clReleaseMemObject(clDest);
}
