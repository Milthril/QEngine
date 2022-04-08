#ifndef Converter_h__
#define Converter_h__


#include "qvectornd.h"
#include "assimp\vector3.h"
#include "assimp\matrix4x4.h"
#include "QMatrix4x4"

inline QVector3D converter(const aiVector3D& aiVec3) {
	return QVector3D(aiVec3.x, aiVec3.y, aiVec3.z);
}

inline QMatrix4x4 converter(const aiMatrix4x4& aiMat4) {
	QMatrix4x4 mat4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat4(i, j) = aiMat4[i][j];
		}
	}
	return mat4;
}

#endif // Converter_h__
