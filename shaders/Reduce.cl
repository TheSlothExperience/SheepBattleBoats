

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
__kernel void ReduceVertical(image2d_t src, image2d_t dest)
{
	// TODO
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
__kernel void ReduceHorizontal(__read_only image2d_t src, __write_only image2d_t dest, const int width, const int height, __local float4 *temp, int row)
{
	const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

	const int ID = get_global_id(0);
	const int2 uv = {2*ID, row};

	int offset = 1;
	temp[2*ID] = read_imagef(src, sampler, uv);
	temp[2*ID + 1] = read_imagef(src, sampler, uv + (int2)(1, 0));

	//Upsweep pass
	for(int d = width>>1; d > 0; d >>= 1) {
		barrier(CLK_LOCAL_MEM_FENCE);
		if(ID < d) {
			int ai = offset * (2 * ID + 1) - 1;
			int bi = offset * (2 * ID + 2) - 1;
			temp[bi] += temp[ai];
		}
		offset *= 2;
	}

	//Clear the last element
	if(ID == 0) {temp[width - 1] = 0;}

	//Downsweep down the tree nodes
	for(int d = 1; d < width; d *= 2) {
		offset >>= 1;
		barrier(CLK_LOCAL_MEM_FENCE);
		if(ID < d) {
			int ai = offset * (2 * ID + 1) - 1;
			int bi = offset * (2 * ID + 2) - 1;

			float4 t = temp[ai];
			temp[ai] = temp[bi];
			temp[bi] += t;
		}
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	//Write to output array. We want inclusive scan, so sum the initial array
	write_imagef(dest, uv, temp[2*ID] + read_imagef(src, sampler, uv));
	write_imagef(dest, uv + (int2)(1, 0), temp[2*ID + 1] + read_imagef(src, sampler, uv + (int2)(1, 0)));
}
