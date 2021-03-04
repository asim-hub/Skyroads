#pragma once

#include <include/glm.h>

namespace Transform3D
{
	inline glm::mat4 Translatee(float translateX, float translateY, float translateZ)
	{
		return glm::mat4(
			1.f,		0.f,		0.f,		0.f,
			0.f,		1.f,		0.f,		0.f,
			0.f,		0.f,		1.f,		0.f,
			translateX, translateY, translateZ, 1.f
		);
	}

	inline glm::mat4 Scalee(float scaleX, float scaleY, float scaleZ)
	{
		return glm::mat4(
			scaleX, 0.f,	0.f,	0.f,
			0.f,	scaleY, 0.f,	0.f,
			0.f,	0.f,	scaleZ, 0.f,
			0.f,	0.f,	0.f,	1.f
		);
	}

	inline glm::mat4 RotateeOZ(float radians)
	{
		GLfloat c = cos(radians);
		GLfloat s = sin(radians);

		return glm::mat4(
			c,		s,		0.f,	0.f,
			-s,		c,		0.f,	0.f,
			0.f,	0.f,	1.f,	0.f,
			0.f,	0.f,	0.f,	1.f
		);
	}

	inline glm::mat4 RotateeOY(float radians)
	{
		GLfloat c = cos(radians);
		GLfloat s = sin(radians);

		return glm::mat4(
			c,		0.f,	-s,		0.f,
			0.f,	1.f,	0.f,	0.f,
			s,		0.f,	c,		0.f,
			0.f,	0.f,	0.f,	1.f
		);
	}

	inline glm::mat4 RotateeOX(float radians)
	{
		GLfloat c = cos(radians);
		GLfloat s = sin(radians);

		return glm::mat4(
			1.f, 0.f, 0.f, 0.f,
			0.f, c, s, 0.f,
			0.f, -s, c, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
	}
}
