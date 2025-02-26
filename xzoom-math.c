#include "xzoom.h"

#include <string.h>

#ifndef XZOOM_MAT4_IDENTITY
# define XZOOM_MAT4_IDENTITY	(	\
	(float [4][4]) {				\
		{ 1.0f, 0.0f, 0.0f, 0.0f }, \
		{ 0.0f, 1.0f, 0.0f, 0.0f }, \
		{ 0.0f, 0.0f, 1.0f, 0.0f }, \
		{ 0.0f, 0.0f, 0.0f, 1.0f }	\
	}								\
)
#endif

int	xzoom_mat4_identity(t_mat4x4 dest) {
	memcpy(dest, XZOOM_MAT4_IDENTITY, sizeof(t_mat4x4));
	return (1);
}

int	xzoom_mat4_ortho(t_mat4x4 dest, float top, float bottom, float left, float right) {
	t_mat4x4	_matrix;

	memcpy(_matrix, XZOOM_MAT4_IDENTITY, sizeof(t_mat4x4));
	_matrix[0][0] = 2.0f / (right - left);
	_matrix[1][1] = 2.0f / (top - bottom);
	_matrix[2][2] = 1.0f;
	_matrix[3][0] = -1.0f * (right + left) / (right - left);
	_matrix[3][1] = -1.0f * (top + bottom) / (top - bottom);
	memcpy(dest, _matrix, sizeof(t_mat4x4));
	return (1);
}
