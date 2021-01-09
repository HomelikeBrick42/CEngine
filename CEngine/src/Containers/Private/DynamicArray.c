#include "Containers/DynamicArray.h"
#include "System/Logger.h"

#include <stdlib.h>
#include <memory.h>

void* _DynamicArrayCreate(u64 length, u64 stride)
{
	u64 headerSize = DYNAMIC_ARRAY_FIELD_LENGTH * sizeof(u64);
	u64 arraySize = length * stride;
	u64* newArray = (u64*)malloc(headerSize + arraySize);
	newArray[DYNAMIC_ARRAY_CAPACITY] = length;
	newArray[DYNAMIC_ARRAY_LENGTH] = 0;
	newArray[DYNAMIC_ARRAY_STRIDE] = stride;
	return newArray + DYNAMIC_ARRAY_FIELD_LENGTH;
}

void _DynamicArrayDestroy(void* array)
{
	free((u64*)array - DYNAMIC_ARRAY_FIELD_LENGTH);
}

u64 _DynamicArrayFieldGet(void* array, DynamicArrayField field)
{
	u64* header = (u64*)array - DYNAMIC_ARRAY_FIELD_LENGTH;
	return header[field];
}

void _DynamicArrayFieldSet(void* array, DynamicArrayField field, u64 value)
{
	u64* header = (u64*)array - DYNAMIC_ARRAY_FIELD_LENGTH;
	header[field] = value;
}

void* _DynamicArrayResize(void* array)
{
	u64 length = DynamicArrayGetLength(array);
	u64 stride = DynamicArrayGetStride(array);
	void* newArray = _DynamicArrayCreate((u64)(DYNAMIC_ARAY_RESIZE_FACTOR * DynamicArrayGetCapacity(array)), stride);
	memcpy(newArray, array, length * stride);
	_DynamicArrayFieldSet(newArray, DYNAMIC_ARRAY_LENGTH, length);
	_DynamicArrayDestroy(array);
	return newArray;
}

void* _DynamicArrayPush(void* array, void* value_ptr)
{
	u64 length = DynamicArrayGetLength(array);
	u64 stride = DynamicArrayGetStride(array);
	if (length >= DynamicArrayGetCapacity(array))
		array = _DynamicArrayResize(array);
	u64 addr = (u64)array;
	addr += length * stride;
	memcpy((void*)addr, value_ptr, stride);
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length + 1);
	return array;
}

void _DynamicArrayPop(void* array, void* dest)
{
	u64 length = DynamicArrayGetLength(array);
	u64 stride = DynamicArrayGetStride(array);
	u64 addr = (u64)array;
	addr += (length - 1) * stride;
	memcpy(dest, (void*)addr, stride);
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length - 1);
}

void* _DynamicArrayInsert(void* array, u64 index, void* value_ptr)
{
	u64 length = DynamicArrayGetLength(array);
	u64 stride = DynamicArrayGetStride(array);
	if (length >= DynamicArrayGetCapacity(array))
		array = _DynamicArrayResize(array);
	u64 addr = (u64)array;
	if (index != length - 1)
	{
		memcpy(
			(void*)(addr + (index + 1) * stride),
			(void*)(addr + index * stride),
			stride * (length - index)
		);
	}
	memcpy((void*)(addr + index * stride), value_ptr, stride);
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length + 1);
	return array;
}

void _DynamicArrayPopAt(void* array, u64 index, void* dest)
{
	u64 length = DynamicArrayGetLength(array);
	u64 stride = DynamicArrayGetStride(array);
	if (length >= DynamicArrayGetCapacity(array))
		LOG_ERROR("Index outside the bounds of this array! Length: %d, Index: %d", length, index);
	u64 addr = (u64)array;
	if (dest)
		memcpy(dest, (void*)(addr + index * stride), stride);
	if (index != length - 1)
	{
		memcpy(
			(void*)(addr + index * stride),
			(void*)(addr + (index + 1) * stride),
			stride * (length - index)
		);
	}
	_DynamicArrayFieldSet(array, DYNAMIC_ARRAY_LENGTH, length - 1);
}
