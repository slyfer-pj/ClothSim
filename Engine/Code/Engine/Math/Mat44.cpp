#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"

Mat44::Mat44()
{
	m_values[Ix] = 1.f;	m_values[Jx] = 0.f;	m_values[Kx] = 0.f;	m_values[Tx] = 0.f;
	m_values[Iy] = 0.f;	m_values[Jy] = 1.f;	m_values[Ky] = 0.f;	m_values[Ty] = 0.f;
	m_values[Iz] = 0.f;	m_values[Jz] = 0.f;	m_values[Kz] = 1.f;	m_values[Tz] = 0.f;
	m_values[Iw] = 0.f;	m_values[Jw] = 0.f;	m_values[Kw] = 0.f;	m_values[Tw] = 1.f;
}

Mat44::Mat44(const Vec2& iBasis2D, const Vec2& jBasis2D, const Vec2& translation2D)
	:Mat44()
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;

	m_values[Tx] = translation2D.x;
	m_values[Ty] = translation2D.y;
}

Mat44::Mat44(const Vec3& iBasis3D, const Vec3& jBasis3D, const Vec3& kBasis3D, const Vec3& translation3D)
	:Mat44()
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	
	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;

	m_values[Tx] = translation3D.x;
	m_values[Ty] = translation3D.y;
	m_values[Tz] = translation3D.z;
}

Mat44::Mat44(const Vec4& iBasis4D, const Vec4& jBasis4D, const Vec4& kBasis4D, const Vec4& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

Mat44::Mat44(const float* sixteenValuesBasisMajor)
{
	for (int i = 0; i < 16; i++)
	{
		m_values[i] = *(sixteenValuesBasisMajor + i);
	}
}

Mat44 const Mat44::CreateTranslation2D(const Vec2& translation2D)
{
	Mat44 mat;
	mat.m_values[Tx] = translation2D.x;
	mat.m_values[Ty] = translation2D.y;

	return mat;
}

Mat44 const Mat44::CreateTranslation3D(const Vec3& translation3D)
{
	Mat44 mat;
	mat.m_values[Tx] = translation3D.x;
	mat.m_values[Ty] = translation3D.y;
	mat.m_values[Tz] = translation3D.z;

	return mat;
}

Mat44 const Mat44::CreateUniformScale2D(float uniformScaleXY)
{
	Mat44 mat;
	mat.m_values[Ix] = uniformScaleXY;
	mat.m_values[Jy] = uniformScaleXY;

	return mat;
}

Mat44 const Mat44::CreateUniformScale3D(float uniformScaleXYZ)
{
	Mat44 mat;
	mat.m_values[Ix] = uniformScaleXYZ;
	mat.m_values[Jy] = uniformScaleXYZ;
	mat.m_values[Kz] = uniformScaleXYZ;

	return mat;
}

Mat44 const Mat44::CreateNonUniformScale2D(const Vec2& nonUniformScaleXY)
{
	Mat44 mat;
	mat.m_values[Ix] = nonUniformScaleXY.x;
	mat.m_values[Jy] = nonUniformScaleXY.y;

	return mat;
}

Mat44 const Mat44::CreateNonUniformScale3D(const Vec3& nonUniformScaleXYZ)
{
	Mat44 mat;
	mat.m_values[Ix] = nonUniformScaleXYZ.x;
	mat.m_values[Jy] = nonUniformScaleXYZ.y;
	mat.m_values[Kz] = nonUniformScaleXYZ.z;

	return mat;
}

Mat44 const Mat44::CreateZRotationDegrees(float rotationDegreesAboutZ)
{
	Mat44 mat;
	mat.m_values[Ix] = CosDegrees(rotationDegreesAboutZ);
	mat.m_values[Iy] = SinDegrees(rotationDegreesAboutZ);

	mat.m_values[Jx] = -SinDegrees(rotationDegreesAboutZ);
	mat.m_values[Jy] = CosDegrees(rotationDegreesAboutZ);

	return mat;
}

Mat44 const Mat44::CreateYRotationDegrees(float rotationDegreesAboutY)
{
	Mat44 mat;
	mat.m_values[Ix] = CosDegrees(rotationDegreesAboutY);
	mat.m_values[Iz] = -SinDegrees(rotationDegreesAboutY);

	mat.m_values[Kx] = SinDegrees(rotationDegreesAboutY);
	mat.m_values[Kz] = CosDegrees(rotationDegreesAboutY);

	return mat;
}

Mat44 const Mat44::CreateXRotationDegrees(float rotationDegreesAboutX)
{
	Mat44 mat;
	mat.m_values[Jy] = CosDegrees(rotationDegreesAboutX);
	mat.m_values[Jz] = SinDegrees(rotationDegreesAboutX);

	mat.m_values[Ky] = -SinDegrees(rotationDegreesAboutX);
	mat.m_values[Kz] = CosDegrees(rotationDegreesAboutX);

	return mat;
}

Mat44 const Mat44::CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float orthoIx = 2.f / (right - left);
	float orthoJy = 2.f / (top - bottom);
	float orthoKz = 1.f / (zFar - zNear);
	float orthoTx = (left + right) / (left - right);
	float orthoTy = (bottom + top) / (bottom - top);
	float orthoTz = zNear / (zNear - zFar);

	Vec4 iBasis4D(orthoIx, 0.f, 0.f, 0.f);
	Vec4 jBasis4D(0.f, orthoJy, 0.f, 0.f);
	Vec4 kBasis4D(0.f, 0.f, orthoKz, 0.f);
	Vec4 translation4D(orthoTx, orthoTy, orthoTz, 1.f);

	Mat44 orthoProjectionMat(iBasis4D, jBasis4D, kBasis4D, translation4D);
	return orthoProjectionMat;
}

Mat44 const Mat44::CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	float halfFovYDegrees = fovYDegrees * 0.5f;
	float Sy = CosDegrees(halfFovYDegrees) / SinDegrees(halfFovYDegrees);
	float Sx = Sy / aspect;
	float Sz = zFar / (zFar - zNear);
	float Tz = (zNear * zFar) / (zNear - zFar);

	Vec4 iBasis4D(Sx, 0.f, 0.f, 0.f);
	Vec4 jBasis4D(0.f, Sy, 0.f, 0.f);
	Vec4 kBasis4D(0.f, 0.f, Sz, 1.f);
	Vec4 translation4D(0.f, 0.f, Tz, 0.f);

	Mat44 perspectiveProjectionMat(iBasis4D, jBasis4D, kBasis4D, translation4D);
	return perspectiveProjectionMat;
}

float* Mat44::GetAsFloatArray()
{
	return m_values;
}

const float* Mat44::GetAsFloatArray() const
{
	return m_values;
}

const Vec2 Mat44::GetIBasis2D() const
{
	return Vec2(m_values[Ix], m_values[Iy]);
}

const Vec2 Mat44::GetJBasis2D() const
{
	return Vec2(m_values[Jx], m_values[Jy]);
}

const Vec2 Mat44::GetTranslation2D() const
{
	return Vec2(m_values[Tx], m_values[Ty]);
}

const Vec3 Mat44::GetIBasis3D() const
{
	return Vec3(m_values[Ix], m_values[Iy], m_values[Iz]);
}

const Vec3 Mat44::GetJBasis3D() const
{
	return Vec3(m_values[Jx], m_values[Jy], m_values[Jz]);
}

const Vec3 Mat44::GetKBasis3D() const
{
	return Vec3(m_values[Kx], m_values[Ky], m_values[Kz]);
}

const Vec3 Mat44::GetTranslation3D() const
{
	return Vec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}

const Vec4 Mat44::GetIBasis4D() const
{
	return Vec4(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
}

const Vec4 Mat44::GetJBasis4D() const
{
	return Vec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}

const Vec4 Mat44::GetKBasis4D() const
{
	return Vec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}

const Vec4 Mat44::GetTranslation4D() const
{
	return Vec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}

void Mat44::SetTranslation2D(const Vec2& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetTranslation3D(const Vec3& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJ2D(const Vec2& iBasis2D, const Vec2& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;
}

void Mat44::SetIJT2D(const Vec2& iBasis2D, const Vec2& jBasis2D, const Vec2& translationXY)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;

	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJK3D(const Vec3& iBasis3D, const Vec3& jBasis3D, const Vec3& kBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;
}

void Mat44::SetIJKT3D(const Vec3& iBasis3D, const Vec3& jBasis3D, const Vec3& kBasis3D, const Vec3& translationXYZ)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;

	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJKT4D(const Vec4& iBasis4D, const Vec4& jBasis4D, const Vec4& kBasis4D, const Vec4& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

void Mat44::SetFromText(const char* text)
{
	Vec3 iBasis, jBasis, kBasis;
	Strings basis = SplitStringOnDelimiter(text, ',');
	SetBasisFromString(iBasis, basis[0]);
	SetBasisFromString(jBasis, basis[1]);
	SetBasisFromString(kBasis, basis[2]);
	SetIJK3D(iBasis, jBasis, kBasis);
}

const Vec2 Mat44::TransformVectorQuantity2D(const Vec2& vectorQuantityXY) const
{
	Vec2 transformedVector;
	transformedVector.x = (m_values[Ix] * vectorQuantityXY.x) + (m_values[Jx] * vectorQuantityXY.y) + (m_values[Kx] * 0.f) + (m_values[Tx] * 0.f);
	transformedVector.y = (m_values[Iy] * vectorQuantityXY.x) + (m_values[Jy] * vectorQuantityXY.y) + (m_values[Ky] * 0.f) + (m_values[Ty] * 0.f);
	return transformedVector;
}

const Vec3 Mat44::TransformVectorQuantity3D(const Vec3& vectorQuantityXYZ) const
{
	Vec3 transformedVector;
	transformedVector.x = (m_values[Ix] * vectorQuantityXYZ.x) + (m_values[Jx] * vectorQuantityXYZ.y) + (m_values[Kx] * vectorQuantityXYZ.z) + (m_values[Tx] * 0.f);
	transformedVector.y = (m_values[Iy] * vectorQuantityXYZ.x) + (m_values[Jy] * vectorQuantityXYZ.y) + (m_values[Ky] * vectorQuantityXYZ.z) + (m_values[Ty] * 0.f);
	transformedVector.z = (m_values[Iz] * vectorQuantityXYZ.x) + (m_values[Jz] * vectorQuantityXYZ.y) + (m_values[Kz] * vectorQuantityXYZ.z) + (m_values[Tz] * 0.f);
	return transformedVector;
}

const Vec2 Mat44::TransformPosition2D(const Vec2& positionXY) const
{
	Vec2 transformedVector;
	transformedVector.x = (m_values[Ix] * positionXY.x) + (m_values[Jx] * positionXY.y) + (m_values[Kx] * 0.f) + (m_values[Tx] * 1.f);
	transformedVector.y = (m_values[Iy] * positionXY.x) + (m_values[Jy] * positionXY.y) + (m_values[Ky] * 0.f) + (m_values[Ty] * 1.f);
	return transformedVector;
}

const Vec3 Mat44::TransformPosition3D(const Vec3& position3D) const
{
	Vec3 transformedVector;
	transformedVector.x = (m_values[Ix] * position3D.x) + (m_values[Jx] * position3D.y) + (m_values[Kx] * position3D.z) + (m_values[Tx] * 1.f);
	transformedVector.y = (m_values[Iy] * position3D.x) + (m_values[Jy] * position3D.y) + (m_values[Ky] * position3D.z) + (m_values[Ty] * 1.f);
	transformedVector.z = (m_values[Iz] * position3D.x) + (m_values[Jz] * position3D.y) + (m_values[Kz] * position3D.z) + (m_values[Tz] * 1.f);
	return transformedVector;
}

const Vec4 Mat44::TransformHomogeneous3D(const Vec4& homogeneousPoint3D) const
{
	Vec4 transformedVector;
	transformedVector.x = (m_values[Ix] * homogeneousPoint3D.x) + (m_values[Jx] * homogeneousPoint3D.y) + (m_values[Kx] * homogeneousPoint3D.z) + (m_values[Tx] * homogeneousPoint3D.w);
	transformedVector.y = (m_values[Iy] * homogeneousPoint3D.x) + (m_values[Jy] * homogeneousPoint3D.y) + (m_values[Ky] * homogeneousPoint3D.z) + (m_values[Ty] * homogeneousPoint3D.w);
	transformedVector.z = (m_values[Iz] * homogeneousPoint3D.x) + (m_values[Jz] * homogeneousPoint3D.y) + (m_values[Kz] * homogeneousPoint3D.z) + (m_values[Tz] * homogeneousPoint3D.w);
	transformedVector.w = (m_values[Iw] * homogeneousPoint3D.x) + (m_values[Jw] * homogeneousPoint3D.y) + (m_values[Kw] * homogeneousPoint3D.z) + (m_values[Tw] * homogeneousPoint3D.w);
	return transformedVector;
}

void Mat44::Append(const Mat44& appendThis)
{
	Mat44 copyOfThis = *this;
	const float* left = &copyOfThis.m_values[0];
	const float* right = &appendThis.m_values[0];

	m_values[Ix] = (left[Ix] * right[Ix]) + (left[Jx] * right[Iy]) + (left[Kx] * right[Iz]) + (left[Tx] * right[Iw]);
	m_values[Iy] = (left[Iy] * right[Ix]) + (left[Jy] * right[Iy]) + (left[Ky] * right[Iz]) + (left[Ty] * right[Iw]);
	m_values[Iz] = (left[Iz] * right[Ix]) + (left[Jz] * right[Iy]) + (left[Kz] * right[Iz]) + (left[Tz] * right[Iw]);
	m_values[Iw] = (left[Iw] * right[Ix]) + (left[Jw] * right[Iy]) + (left[Kw] * right[Iz]) + (left[Tw] * right[Iw]);

	m_values[Jx] = (left[Ix] * right[Jx]) + (left[Jx] * right[Jy]) + (left[Kx] * right[Jz]) + (left[Tx] * right[Jw]);
	m_values[Jy] = (left[Iy] * right[Jx]) + (left[Jy] * right[Jy]) + (left[Ky] * right[Jz]) + (left[Ty] * right[Jw]);
	m_values[Jz] = (left[Iz] * right[Jx]) + (left[Jz] * right[Jy]) + (left[Kz] * right[Jz]) + (left[Tz] * right[Jw]);
	m_values[Jw] = (left[Iw] * right[Jx]) + (left[Jw] * right[Jy]) + (left[Kw] * right[Jz]) + (left[Tw] * right[Jw]);

	m_values[Kx] = (left[Ix] * right[Kx]) + (left[Jx] * right[Ky]) + (left[Kx] * right[Kz]) + (left[Tx] * right[Kw]);
	m_values[Ky] = (left[Iy] * right[Kx]) + (left[Jy] * right[Ky]) + (left[Ky] * right[Kz]) + (left[Ty] * right[Kw]);
	m_values[Kz] = (left[Iz] * right[Kx]) + (left[Jz] * right[Ky]) + (left[Kz] * right[Kz]) + (left[Tz] * right[Kw]);
	m_values[Kw] = (left[Iw] * right[Kx]) + (left[Jw] * right[Ky]) + (left[Kw] * right[Kz]) + (left[Tw] * right[Kw]);

	m_values[Tx] = (left[Ix] * right[Tx]) + (left[Jx] * right[Ty]) + (left[Kx] * right[Tz]) + (left[Tx] * right[Tw]);
	m_values[Ty] = (left[Iy] * right[Tx]) + (left[Jy] * right[Ty]) + (left[Ky] * right[Tz]) + (left[Ty] * right[Tw]);
	m_values[Tz] = (left[Iz] * right[Tx]) + (left[Jz] * right[Ty]) + (left[Kz] * right[Tz]) + (left[Tz] * right[Tw]);
	m_values[Tw] = (left[Iw] * right[Tx]) + (left[Jw] * right[Ty]) + (left[Kw] * right[Tz]) + (left[Tw] * right[Tw]);
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	Mat44 rotZMat = Mat44::CreateZRotationDegrees(degreesRotationAboutZ);
	Append(rotZMat);
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	Mat44 rotYMat = Mat44::CreateYRotationDegrees(degreesRotationAboutY);
	Append(rotYMat);
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	Mat44 rotXMat = Mat44::CreateXRotationDegrees(degreesRotationAboutX);
	Append(rotXMat);
}

void Mat44::AppendTranslation2D(const Vec2& translationXY)
{
	Mat44 translate2DMat = Mat44::CreateTranslation2D(translationXY);
	Append(translate2DMat);
}

void Mat44::AppendTranslation3D(const Vec3& translationXYZ)
{
	Mat44 translate3DMat = Mat44::CreateTranslation3D(translationXYZ);
	Append(translate3DMat);
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	Mat44 scale2DMat = Mat44::CreateUniformScale2D(uniformScaleXY);
	Append(scale2DMat);
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	Mat44 scale3DMat = Mat44::CreateUniformScale3D(uniformScaleXYZ);
	Append(scale3DMat);
}

void Mat44::AppendScaleNonUniform2D(const Vec2& nonUniformScaleXY)
{
	Mat44 nonUniformScale2DMat = Mat44::CreateNonUniformScale2D(nonUniformScaleXY);
	Append(nonUniformScale2DMat);
}

void Mat44::AppendScaleNonUniform3D(const Vec3& nonUniformScaleXYZ)
{
	Mat44 nonUniformScale3DMat = Mat44::CreateNonUniformScale3D(nonUniformScaleXYZ);
	Append(nonUniformScale3DMat);
}

void Mat44::Transpose()
{
	Vec4 transposedIBasis4D = Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]);
	Vec4 transposedJBasis4D = Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]);
	Vec4 transposedKBasis4D = Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]);
	Vec4 transposedTranslation4D = Vec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]);
	Mat44 transposed(transposedIBasis4D, transposedJBasis4D, transposedKBasis4D, transposedTranslation4D);
	*this = transposed;
}

const Mat44 Mat44::GetOrthonormalInverse() const
{
	//extract translation part from the current matrix
	Vec3 translation3D = GetTranslation3D();

	//extract rotation part from the current matrix
	Mat44 rotationMat(GetIBasis3D(), GetJBasis3D(), GetKBasis3D(), Vec3());

	//invert translation and make a mat with only translation
	Mat44 invertedTranslationMat;
	invertedTranslationMat.SetTranslation3D(-translation3D);

	//transpose the rotation part of the current matrix
	rotationMat.Transpose();

	//multiply transposed matrix to the inverted translation matrix
	rotationMat.Append(invertedTranslationMat);

	//we now have the inverse matrix
	return rotationMat;
}

void Mat44::Orthonormalize_XFwd_YLeft_ZUp()
{
	Vec4 iBasis4D = GetIBasis4D();
	iBasis4D.Normalize();

	Vec4 kBasis4D = GetKBasis4D();
	kBasis4D = kBasis4D - (iBasis4D * DotProduct4D(kBasis4D, iBasis4D));
	kBasis4D.Normalize();

	Vec4 jBasis4D = GetJBasis4D();
	jBasis4D = jBasis4D - (iBasis4D * DotProduct4D(jBasis4D, iBasis4D));
	jBasis4D = jBasis4D - (kBasis4D * DotProduct4D(jBasis4D, kBasis4D));
	jBasis4D.Normalize();

	SetIJKT4D(iBasis4D, jBasis4D, kBasis4D, GetTranslation4D());
}

void Mat44::SetBasisFromString(Vec3& basis, const std::string& text)
{
	char basisChar = text.size() > 1 ? text[1] : text[0];
	float signMultiplier = text.size() > 1 ? -1.f : 1.f;

	switch (basisChar)
	{
	case 'i':
	{
		basis = Vec3(1.f, 0.f, 0.f);
		break;
	}
	case 'j':
	{
		basis = Vec3(0.f, 1.f, 0.f);
		break;
	}
	case 'k':
	{
		basis = Vec3(0.f, 0.f, 1.f);
		break;
	}
	}

	basis = basis * signMultiplier;
}

