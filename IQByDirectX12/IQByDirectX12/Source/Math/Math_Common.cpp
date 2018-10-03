#include <cfloat>
#include <cmath>
#include <algorithm>
#include "Math.h"

bool Math::IsEqual(float v1, float v2)
{
	return (fabsf(v1 - v2) <= FLT_EPSILON);
}

bool Math::IsEqual(double v1, double v2)
{
	return (abs(v1 - v2) <= DBL_EPSILON);
}

bool Math::IsEqual(float v1, double v2)
{
	return (abs(static_cast<double>(v1) - v2) <= DBL_EPSILON);
}

bool Math::IsEqual(double v1, float v2)
{
	return (abs(v1 - static_cast<double>(v2)) <= DBL_EPSILON);
}

bool Math::IsZero(float value)
{
	return IsEqual(value, 0.0f);
}

bool Math::IsZero(double value)
{
	return IsEqual(value, 0.0);
}

float Math::Clamp(float value, float minValue, float maxValue)
{
	return std::min(maxValue, std::max(minValue, value));
}

double Math::Clamp(double value, double minValue, double maxValue)
{
	return std::min(maxValue, std::max(minValue, value));
}
