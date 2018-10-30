#include <cfloat>
#include <cmath>
#include <algorithm>
#include "XMMath.h"


bool XMMath::IsEqual(float v1, float v2)
{
	return (fabsf(v1 - v2) <= FLT_EPSILON);
}

bool XMMath::IsEqual(double v1, double v2)
{
	return (abs(v1 - v2) <= DBL_EPSILON);
}

bool XMMath::IsEqual(float v1, double v2)
{
	return (abs(static_cast<double>(v1) - v2) <= DBL_EPSILON);
}

bool XMMath::IsEqual(double v1, float v2)
{
	return (abs(v1 - static_cast<double>(v2)) <= DBL_EPSILON);
}

bool XMMath::IsZero(float value)
{
	return IsEqual(value, 0.0f);
}

bool XMMath::IsZero(double value)
{
	return IsEqual(value, 0.0);
}

float XMMath::Clamp(float value, float minValue, float maxValue)
{
	return std::min(maxValue, std::max(minValue, value));
}

double XMMath::Clamp(double value, double minValue, double maxValue)
{
	return std::min(maxValue, std::max(minValue, value));
}
