#pragma once
#include <string>

struct Vec2;
struct Vec3;
struct Vec4;

struct Mat44
{
	enum { Ix, Iy, Iz, Iw, Jx, Jy, Jz, Jw, Kx, Ky, Kz, Kw, Tx, Ty, Tz, Tw};
	float m_values[16] = {};

	Mat44();
	explicit Mat44(const Vec2& iBasis2D, const Vec2& jBasis2D, const Vec2& translation2D);
	explicit Mat44(const Vec3& iBasis3D, const Vec3& jBasis3D, const Vec3& zBasis3D, const Vec3& translation3D);
	explicit Mat44(const Vec4& iBasis4D, const Vec4& jBasis4D, const Vec4& zBasis4D, const Vec4& translation4D);
	explicit Mat44(const float* sixteenValuesBasisMajor);

	static Mat44 const CreateTranslation2D(const Vec2& translation2D);
	static Mat44 const CreateTranslation3D(const Vec3& translation3D);
	static Mat44 const CreateUniformScale2D(float uniformScaleXY);
	static Mat44 const CreateUniformScale3D(float uniformScaleXYZ);
	static Mat44 const CreateNonUniformScale2D(const Vec2& nonUniformScaleXY);
	static Mat44 const CreateNonUniformScale3D(const Vec3& nonUniformScaleXYZ);
	static Mat44 const CreateZRotationDegrees(float rotationDegreesAboutZ);
	static Mat44 const CreateYRotationDegrees(float rotationDegreesAboutY);
	static Mat44 const CreateXRotationDegrees(float rotationDegreesAboutX);
	static Mat44 const CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	static Mat44 const CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar);

	float* GetAsFloatArray();
	const float* GetAsFloatArray() const;
	const Vec2 GetIBasis2D() const;
	const Vec2 GetJBasis2D() const;
	const Vec2 GetTranslation2D() const;
	const Vec3 GetIBasis3D() const;
	const Vec3 GetJBasis3D() const;
	const Vec3 GetKBasis3D() const;
	const Vec3 GetTranslation3D() const;
	const Vec4 GetIBasis4D() const;
	const Vec4 GetJBasis4D() const;
	const Vec4 GetKBasis4D() const;
	const Vec4 GetTranslation4D() const;

	void SetTranslation2D(const Vec2& translationXY);
	void SetTranslation3D(const Vec3& translationXYZ);
	void SetIJ2D(const Vec2& iBasis2D, const Vec2& jBasis2D);
	void SetIJT2D(const Vec2& iBasis2D, const Vec2& jBasis2D, const Vec2& translationXY);
	void SetIJK3D(const Vec3& iBasis3D, const Vec3& jBasis3D, const Vec3& kBasis3D);
	void SetIJKT3D(const Vec3& iBasis3D, const Vec3& jBasis3D, const Vec3& kBasis3D, const Vec3& translationXYZ);
	void SetIJKT4D(const Vec4& iBasis4D, const Vec4& jBasis4D, const Vec4& kBasis4D, const Vec4& translation4D);
	void SetFromText(const char* text);
	
	const Vec2 TransformVectorQuantity2D(const Vec2& vectorQuantityXY) const;
	const Vec3 TransformVectorQuantity3D(const Vec3& vectorQuantityXYZ) const;
	const Vec2 TransformPosition2D(const Vec2& positionXY) const;
	const Vec3 TransformPosition3D(const Vec3& position3D) const;
	const Vec4 TransformHomogeneous3D(const Vec4& homogeneousPoint3D) const;

	void Append(const Mat44& appendThis);
	void AppendZRotation(float degreesRotationAboutZ);
	void AppendYRotation(float degreesRotationAboutY);
	void AppendXRotation(float degreesRotationAboutX);
	void AppendTranslation2D(const Vec2& translationXY);
	void AppendTranslation3D(const Vec3& translationXYZ);
	void AppendScaleUniform2D(float uniformScaleXY);
	void AppendScaleUniform3D(float uniformScaleXYZ);
	void AppendScaleNonUniform2D(const Vec2& nonUniformScaleXY);
	void AppendScaleNonUniform3D(const Vec3& nonUniformScaleXYZ);
	void Transpose();
	const Mat44 GetOrthonormalInverse() const;
	void Orthonormalize_XFwd_YLeft_ZUp();

private:
	void SetBasisFromString(Vec3& basis, const std::string& text);
};