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

int	xzoom_mat4_identity(t_mat4x4 dst) {
	memcpy(dst, XZOOM_MAT4_IDENTITY, sizeof(t_mat4x4));
	return (1);
}

int	xzoom_mat4_inverse(t_mat4x4 dst) {
	float	a = dst[0][0],
			b = dst[0][1],
			c = dst[0][2],
			d = dst[0][3],
			e = dst[1][0],
			f = dst[1][1],
			g = dst[1][2],
			h = dst[1][3],
			i = dst[2][0],
			j = dst[2][1],
			k = dst[2][2],
			l = dst[2][3],
			m = dst[3][0],
			n = dst[3][1],
			o = dst[3][2],
			p = dst[3][3],
			c1 = k * p - l * o,
			c2 = c * h - d * g,
			c3 = i * p - l * m,
			c4 = a * h - d * e,
			c5 = j * p - l * n,
			c6 = b * h - d * f,
			c7 = i * n - j * m,
			c8 = a * f - b * e,
			c9 = j * o - k * n,
			c10 = b * g - c * f,
			c11 = i * o - k * m,
			c12 = a * g - c * e,
			idt = 1.0f / (c8 * c1 + c4 * c9 + c10 * c3 + c2 * c7 - c12 * c5 - c6 * c11),
			ndt = -idt;
	
	dst[0][0] = (f * c1  - g * c5  + h * c9)  * idt;
	dst[0][1] = (b * c1  - c * c5  + d * c9)  * ndt;
	dst[0][2] = (n * c2  - o * c6  + p * c10) * idt;
	dst[0][3] = (j * c2  - k * c6  + l * c10) * ndt;
	dst[1][0] = (e * c1  - g * c3  + h * c11) * ndt;
	dst[1][1] = (a * c1  - c * c3  + d * c11) * idt;
	dst[1][2] = (m * c2  - o * c4  + p * c12) * ndt;
	dst[1][3] = (i * c2  - k * c4  + l * c12) * idt;
	dst[2][0] = (e * c5  - f * c3  + h * c7)  * idt;
	dst[2][1] = (a * c5  - b * c3  + d * c7)  * ndt;
	dst[2][2] = (m * c6  - n * c4  + p * c8)  * idt;
	dst[2][3] = (i * c6  - j * c4  + l * c8)  * ndt;
	dst[3][0] = (e * c9  - f * c11 + g * c7)  * ndt;
	dst[3][1] = (a * c9  - b * c11 + c * c7)  * idt;
	dst[3][2] = (m * c10 - n * c12 + o * c8)  * ndt;
	dst[3][3] = (i * c10 - j * c12 + k * c8)  * idt;	
	return (1);
}

int	xzoom_mat4_ortho(t_mat4x4 dst, float top, float bottom, float left, float right) {
	t_mat4x4	_matrix;

	memcpy(_matrix, XZOOM_MAT4_IDENTITY, sizeof(t_mat4x4));
	_matrix[0][0] = 2.0f / (right - left);
	_matrix[1][1] = 2.0f / (top - bottom);
	_matrix[2][2] = 1.0f;
	_matrix[3][0] = -1.0f * (right + left) / (right - left);
	_matrix[3][1] = -1.0f * (top + bottom) / (top - bottom);
	memcpy(dst, _matrix, sizeof(t_mat4x4));
	return (1);
}

int	xzoom_mat4_trans(t_mat4x4 src, t_vec3 vec, t_mat4x4 dst) {
	xzoom_vec4_muladds(src[0], vec[0], dst[3]);
	xzoom_vec4_muladds(src[1], vec[1], dst[3]);
	xzoom_vec4_muladds(src[2], vec[2], dst[3]);
	return (1);	
}

int	xzoom_vec2_scale(t_vec2 src, float s, t_vec2 dst) {
	dst[0] = src[0] * s;
	dst[1] = src[1] * s;
	return (1);
}

int	xzoom_mat4_scale(t_mat4x4 src, t_vec3 vec, t_mat4x4 dst) {
	xzoom_vec4_scale(src[0], vec[0], dst[0]);
	xzoom_vec4_scale(src[1], vec[1], dst[1]);
	xzoom_vec4_scale(src[2], vec[2], dst[2]);
	return (1);	
}

int	xzoom_mat4_mulv(t_mat4x4 src, t_vec4 vec, t_vec4 dst) {
	dst[0] = src[0][0] * vec[0] + src[1][0] * vec[1] + src[2][0] * vec[2] + src[3][0] * vec[3];
	dst[1] = src[0][1] * vec[0] + src[1][1] * vec[1] + src[2][1] * vec[2] + src[3][1] * vec[3];
	dst[2] = src[0][2] * vec[0] + src[1][2] * vec[1] + src[2][2] * vec[2] + src[3][2] * vec[3];
	dst[3] = src[0][3] * vec[0] + src[1][3] * vec[1] + src[2][3] * vec[2] + src[3][3] * vec[3];
	return (1);
}

int	xzoom_vec4_scale(t_vec4 src, float s, t_vec4 dst) {
	dst[0] = src[0] * s;
	dst[1] = src[1] * s;
	dst[2] = src[2] * s;
	dst[3] = src[3] * s;
	return (1);
}

int	xzoom_vec4_muladds(t_vec4 src, float s, t_vec4 dst) {
	dst[0] += src[0] * s;
	dst[1] += src[1] * s;
	dst[2] += src[2] * s;
	dst[3] += src[3] * s;
	return (1);
}
