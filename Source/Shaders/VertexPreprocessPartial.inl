// Copyright (c) 2021 Sultim Tsyrendashiev
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#if defined(VERTEX_PREPROCESS_PARTIAL_DYNAMIC)

    #define GET_POSITIONS getDynamicVerticesPositions
    #define SET_POSITIONS setDynamicVerticesPositions
    #define GET_NORMALS getDynamicVerticesNormals
    #define SET_NORMALS setDynamicVerticesNormals
    #define GET_TEXCOORDS getDynamicVerticesTexCoords
    #define SET_TANGENTS setDynamicVerticesTangents
    #define INDICES dynamicIndices

#elif defined(VERTEX_PREPROCESS_PARTIAL_STATIC)

    #define GET_POSITIONS getStaticVerticesPositions
    #define SET_POSITIONS setStaticVerticesPositions
    #define GET_NORMALS getStaticVerticesNormals
    #define SET_NORMALS setStaticVerticesNormals
    #define GET_TEXCOORDS getStaticVerticesTexCoords
    #define SET_TANGENTS setStaticVerticesTangents
    #define INDICES staticIndices

#else
    #error
#endif




// translate from local to global geom index
const int geomIndexOffset = globalUniform.instanceGeomInfoOffset[tlasInstanceIndex / 4][tlasInstanceIndex % 4];
const int geomCount = globalUniform.instanceGeomCount[tlasInstanceIndex / 4][tlasInstanceIndex % 4];

for (uint localGeomIndex = gl_LocalInvocationID.x; localGeomIndex < geomCount; localGeomIndex += gl_WorkGroupSize.x)
{
    const ShGeometryInstance inst = geometryInstances[geomIndexOffset + localGeomIndex];

    const bool useIndices = inst.baseIndexIndex != UINT32_MAX;
    // TODO: provide info
    const bool genNormals = true;

    const mat4 model = inst.model;
    const mat3 model3 = mat3(model);

    for (uint l = 0; l < inst.vertexCount; l += 3)
    {
        const uint v = inst.baseVertexIndex + l;

        const uvec3 vertexIndices = uvec3(
            v + 0,
            v + 1,
            v + 2);
        
        //SET_POSITIONS(vertexIndices[0], globalPos[0]);
        //SET_POSITIONS(vertexIndices[1], globalPos[1]);
        //SET_POSITIONS(vertexIndices[2], globalPos[2]);

        if (!useIndices)
        {
            const vec3 localPos[] = 
            {
                GET_POSITIONS(vertexIndices[0]),
                GET_POSITIONS(vertexIndices[1]),
                GET_POSITIONS(vertexIndices[2])
            };

            if (genNormals)
            {
                const vec3 globalPos[] = 
                {
                    (model * vec4(localPos[0], 1.0)).xyz,
                    (model * vec4(localPos[1], 1.0)).xyz,
                    (model * vec4(localPos[2], 1.0)).xyz
                };

                const vec3 globalNormal = normalize(cross(globalPos[1] - globalPos[0], globalPos[2] - globalPos[0]));

                SET_NORMALS(vertexIndices[0], globalNormal);
                SET_NORMALS(vertexIndices[1], globalNormal);
                SET_NORMALS(vertexIndices[2], globalNormal);
            }
            else
            {
                const vec3 localNormal[] = 
                {
                    GET_NORMALS(vertexIndices[0]),
                    GET_NORMALS(vertexIndices[1]),
                    GET_NORMALS(vertexIndices[2])
                };

                const vec3 globalNormal[] = 
                {
                    model3 * localNormal[0],
                    model3 * localNormal[1],
                    model3 * localNormal[2]
                };
                
                SET_NORMALS(vertexIndices[0], globalNormal[0]);
                SET_NORMALS(vertexIndices[1], globalNormal[1]);
                SET_NORMALS(vertexIndices[2], globalNormal[2]);
            }

            const vec2 texCoord[] = 
            {
                GET_TEXCOORDS(vertexIndices[0]),
                GET_TEXCOORDS(vertexIndices[1]),
                GET_TEXCOORDS(vertexIndices[2])
            };

            const uint primitiveId = v / 3;
            SET_TANGENTS(primitiveId, getTangent(localPos, texCoord));
        }
    }

    // if not using indices, baseIndexIndex=UINT32_MAX, so loop will be skipped
    for (uint i = inst.baseIndexIndex; i < inst.baseIndexIndex + inst.indexCount; i += 3)
    {
        const uvec3 vertexIndices = uvec3(
            inst.baseVertexIndex + INDICES[i + 0],
            inst.baseVertexIndex + INDICES[i + 1],
            inst.baseVertexIndex + INDICES[i + 2]);

        const vec3 localPos[] = 
        {
            GET_POSITIONS(vertexIndices[0]),
            GET_POSITIONS(vertexIndices[1]),
            GET_POSITIONS(vertexIndices[2])
        };

        if (genNormals)
        {
            const vec3 globalPos[] = 
            {
                (model * vec4(localPos[0], 1.0)).xyz,
                (model * vec4(localPos[1], 1.0)).xyz,
                (model * vec4(localPos[2], 1.0)).xyz
            };

            const vec3 globalNormal = normalize(cross(globalPos[1] - globalPos[0], globalPos[2] - globalPos[0]));
            
            SET_NORMALS(vertexIndices[0], globalNormal);
            SET_NORMALS(vertexIndices[1], globalNormal);
            SET_NORMALS(vertexIndices[2], globalNormal);
        }
        else
        {
            const vec3 localNormal[] = 
            {
                GET_NORMALS(vertexIndices[0]),
                GET_NORMALS(vertexIndices[1]),
                GET_NORMALS(vertexIndices[2])
            };

            const vec3 globalNormal[] = 
            {
                model3 * localNormal[0],
                model3 * localNormal[1],
                model3 * localNormal[2]
            };
            
            SET_NORMALS(vertexIndices[0], globalNormal[0]);
            SET_NORMALS(vertexIndices[1], globalNormal[1]);
            SET_NORMALS(vertexIndices[2], globalNormal[2]);
        }
        
        const vec2 texCoord[] = 
        {
            GET_TEXCOORDS(vertexIndices[0]),
            GET_TEXCOORDS(vertexIndices[1]),
            GET_TEXCOORDS(vertexIndices[2])
        };

        const uint primitiveId = i / 3;
        SET_TANGENTS(primitiveId, getTangent(localPos, texCoord));
    }
}


#undef GET_POSITIONS
#undef SET_POSITIONS
#undef GET_NORMALS
#undef SET_NORMALS
#undef GET_TEXCOORDS
#undef SET_TANGENTS
#undef INDICES