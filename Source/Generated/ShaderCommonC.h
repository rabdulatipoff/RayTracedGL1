// This file was generated by GenerateShaderCommon.py

#pragma once

namespace RTGL1
{

#include <stdint.h>

#define MAX_STATIC_VERTEX_COUNT (1048576)
#define MAX_DYNAMIC_VERTEX_COUNT (2097152)
#define MAX_VERTEX_COLLECTOR_INDEX_COUNT (4194304)
#define MAX_GEOMETRY_PRIMITIVE_COUNT_POW (18)
#define MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT_POW (14)
#define MAX_GEOMETRY_PRIMITIVE_COUNT (1 << MAX_GEOMETRY_PRIMITIVE_COUNT_POW)
#define MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT (1 << MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT_POW)
#define MAX_TOP_LEVEL_INSTANCE_COUNT (32)
#define BINDING_VERTEX_BUFFER_STATIC (0)
#define BINDING_VERTEX_BUFFER_DYNAMIC (1)
#define BINDING_INDEX_BUFFER_STATIC (2)
#define BINDING_INDEX_BUFFER_DYNAMIC (3)
#define BINDING_GEOMETRY_INSTANCES_STATIC (4)
#define BINDING_GEOMETRY_INSTANCES_DYNAMIC (5)
#define BINDING_GLOBAL_UNIFORM (0)
#define BINDING_ACCELERATION_STRUCTURE (0)
#define BINDING_TEXTURES (0)
#define BINDING_BLUE_NOISE (0)
#define BINDING_LUM_HISTOGRAM (0)
#define BINDING_LIGHT_SOURCES_SPHERICAL (0)
#define BINDING_LIGHT_SOURCES_DIRECTIONAL (1)
#define INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC (1 << 0)
#define INSTANCE_CUSTOM_INDEX_FLAG_FIRST_PERSON (1 << 1)
#define INSTANCE_CUSTOM_INDEX_FLAG_FIRST_PERSON_VIEWER (1 << 2)
#define INSTANCE_MASK_ALL (0xFF)
#define INSTANCE_MASK_WORLD (1 << 0)
#define INSTANCE_MASK_FIRST_PERSON (1 << 1)
#define INSTANCE_MASK_FIRST_PERSON_VIEWER (1 << 2)
#define INSTANCE_MASK_BLENDED (1 << 3)
#define INSTANCE_MASK_EMPTY_4 (1 << 4)
#define INSTANCE_MASK_EMPTY_5 (1 << 5)
#define INSTANCE_MASK_EMPTY_6 (1 << 6)
#define INSTANCE_MASK_EMPTY_7 (1 << 7)
#define PAYLOAD_INDEX_DEFAULT (0)
#define PAYLOAD_INDEX_SHADOW (1)
#define SBT_INDEX_RAYGEN_PRIMARY (0)
#define SBT_INDEX_RAYGEN_DIRECT (1)
#define SBT_INDEX_RAYGEN_INDIRECT (2)
#define SBT_INDEX_MISS_DEFAULT (0)
#define SBT_INDEX_MISS_SHADOW (1)
#define SBT_INDEX_HITGROUP_FULLY_OPAQUE (0)
#define SBT_INDEX_HITGROUP_ALPHA_TESTED (1)
#define SBT_INDEX_HITGROUP_BLEND_ADDITIVE (2)
#define SBT_INDEX_HITGROUP_BLEND_UNDER (3)
#define MATERIAL_ALBEDO_ALPHA_INDEX (0)
#define MATERIAL_NORMAL_METALLIC_INDEX (1)
#define MATERIAL_EMISSION_ROUGHNESS_INDEX (2)
#define MATERIAL_NO_TEXTURE (0)
#define BLUE_NOISE_TEXTURE_COUNT (64)
#define BLUE_NOISE_TEXTURE_SIZE (64)
#define BLUE_NOISE_TEXTURE_SIZE_POW (6)
#define COMPUTE_COMPOSE_GROUP_SIZE_X (16)
#define COMPUTE_COMPOSE_GROUP_SIZE_Y (16)
#define COMPUTE_LUM_HISTOGRAM_GROUP_SIZE_X (16)
#define COMPUTE_LUM_HISTOGRAM_GROUP_SIZE_Y (16)
#define COMPUTE_LUM_HISTOGRAM_BIN_COUNT (256)

struct ShVertexBufferStatic
{
    float positions[3145728];
    float normals[3145728];
    float texCoords[2097152];
    float texCoordsLayer1[2097152];
    float texCoordsLayer2[2097152];
};

struct ShVertexBufferDynamic
{
    float positions[6291456];
    float normals[6291456];
    float texCoords[4194304];
};

struct ShGlobalUniform
{
    float view[16];
    float invView[16];
    float viewPrev[16];
    float projection[16];
    float invProjection[16];
    float projectionPrev[16];
    uint32_t positionsStride;
    uint32_t normalsStride;
    uint32_t texCoordsStride;
    float renderWidth;
    float renderHeight;
    uint32_t frameId;
    float timeDelta;
    float minLogLuminance;
    float maxLogLuminance;
    float luminanceWhitePoint;
    uint32_t stopEyeAdaptation;
    uint32_t lightSourceCountSpherical;
    uint32_t lightSourceCountDirectional;
    float _pad1;
    float _pad2;
    float _pad3;
    int32_t instanceGeomInfoOffset[128];
};

struct ShGeometryInstance
{
    float model[16];
    uint32_t materials[3][4];
    float color[4];
    uint32_t baseVertexIndex;
    uint32_t baseIndexIndex;
    uint32_t primitiveCount;
    float defaultRoughness;
    float defaultMetallicity;
    float defaultEmission;
    uint32_t __pad0;
    uint32_t __pad1;
};

struct ShTonemapping
{
    uint32_t histogram[256];
    float avgLuminance;
};

struct ShLightSpherical
{
    float position[3];
    float radius;
    float color[3];
    uint32_t __pad0;
};

struct ShLightDirectional
{
    float direction[3];
    float tanAngularRadius;
    float color[3];
    uint32_t __pad0;
};

}