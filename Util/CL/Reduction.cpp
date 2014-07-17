#include "Reduction.h"

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
	cleanupContextResources();
}

bool Reduction::initContextResources() {
	//error code
	cl_int clError;

	//get platform ID
	V_RETURN_FALSE_CL(clGetPlatformIDs(1, &clPlatform, NULL), "Failed to get CL platform ID");

	//get a reference to the first available GPU device
	V_RETURN_FALSE_CL(clGetDeviceIDs(clPlatform, CL_DEVICE_TYPE_GPU, 1, &clDevice, NULL), "No GPU device found.");

	char deviceName[256];
	V_RETURN_FALSE_CL(clGetDeviceInfo(clDevice, CL_DEVICE_NAME, 256, &deviceName, NULL), "Unable to query device name.");
	cout << "Device: " << deviceName << endl;

	//Create a new OpenCL context on the selected device
	clContext = clCreateContext(0, 1, &clDevice, NULL, NULL, &clError);
	V_RETURN_FALSE_CL(clError, "Failed to create OpenCL context.");

	//Finally, create the command queue. All the asynchronous commands to the device will be issued
	//from the CPU into this queue. This way the host program can continue the execution until some results
	//from that device are needed.
	clCommandQueue = clCreateCommandQueue(clContext, clDevice, 0, &clError);
	V_RETURN_FALSE_CL(clError, "Failed to create the command queue in the context");

	return true;
}

void Reduction::cleanupContextResources() {
	if(clCommandQueue)	clReleaseCommandQueue(clCommandQueue);
	if(clContext)			clReleaseContext(clContext);
}
