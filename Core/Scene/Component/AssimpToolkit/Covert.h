#ifndef Covert_h__
#define Covert_h__

#include "qvectornd.h"
#include "assimp\vector3.h"
#include "assimp\matrix4x4.h"
#include "QMatrix4x4"

inline QVector3D converter(const aiVector3D& aiVec3) {
	return QVector3D(aiVec3.x, aiVec3.y, aiVec3.z);
}

inline QMatrix4x4 converter(const aiMatrix4x4& aiMat4) {
	QMatrix4x4 mat4;
	memcpy(mat4.data(), &aiMat4, sizeof(float) * 16);
	return mat4;
}

#endif // Covert_h__

