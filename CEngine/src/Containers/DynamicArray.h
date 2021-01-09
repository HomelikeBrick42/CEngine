#pragma once

#include "System/Defines.h"

typedef enum DynamicArrayField
{
	DYNAMIC_ARRAY_CAPACITY,
	DYNAMIC_ARRAY_LENGTH,
	DYNAMIC_ARRAY_STRIDE,
	DYNAMIC_ARRAY_FIELD_LENGTH
} DynamicArrayField;

void* _DynamicArrayCreate(u64 length, u64 stride);
void _DynamicArrayDestroy(void* array);

u64 _DynamicArrayFieldGet(void* array, DynamicArrayField field);
void _DynamicArrayFieldSet(void* array, DynamicArrayField field, u64 value);

void* _DynamicArrayResize(void* array);

void* _DynamicArrayPush(void* array, void* value_ptr);
void _DynamicArrayPop(void* array, void* dest);

void* _DynamicArrayInsert(void* array, u64 index, void* value_ptr);
void _DynamicArrayPopAt(void* array, u64 index, void* dest);

#define DYNAMIC_ARAY_DEFAULT_CAPACITY 1
#define DYNAMIC_ARAY_RESIZE_FACTOR 2

#define DynamicArrayCreate(type) \
	_DynamicArrayCreate(DYNAMIC_ARAY_DEFAULT_CAPACITY, sizeof(type))

#define DynamicArrayReserve(type, capasity) \
	_DynamicArrayCreate(capasity, sizeof(type))

#define DynamicArrayDestroy(array) \
	_DynamicArrayDestroy(array)

#define DynamicArrayPush(array, value) \
	array = _DynamicArrayPush(array, &value)

#define DynamicArrayPushLiteral(array, value, type) \
	{ \
		type temp = value; \
		array = _DynamicArrayPush(array, &temp); \
	}

#define DynamicArrayPop(array, value_ptr) \
	_DynamicArrayPop(array, value_ptr)

#define DynamicArrayInsert(array, index, value) \
	array = _DynamicArrayInsert(array, index, &value)

#define DynamicArrayInsertLiteral(array, index, value, type) \
	{ \
		type temp = value; \
		array = _DynamicArrayInsert(array, index, &temp); \
	}

#define DynamicArrayPopAt(array, index, value_ptr) \
	_DynamicArrayPopAt(array, index, value_ptr)

#define DynamicArrayGetCapacity(array) \
	_DynamicArrayFieldGet(array, DYNAMIC_ARRAY_CAPACITY)

#define DynamicArrayGetLength(array) \
	_DynamicArrayFieldGet(array, DYNAMIC_ARRAY_LENGTH)

#define DynamicArrayGetStride(array) \
	_DynamicArrayFieldGet(array, DYNAMIC_ARRAY_STRIDE)
