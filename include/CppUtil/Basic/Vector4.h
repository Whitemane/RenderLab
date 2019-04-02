#ifndef _CPPUTIL_BASIC_MATH_VECTOR4_H_
#define _CPPUTIL_BASIC_MATH_VECTOR4_H_

#include <CppUtil/Basic/Val4.h>
#include <CppUtil/Basic/Vector.h>

namespace CppUtil {
	namespace Basic {
		template <typename T>
		class Vector<4, T> : public Val<4, T> {
		public:
			using Val<4, T>::Val;

		public:
			T Length2() const { return x * x + y * y + z * z; }
			T Length() const { return std::sqrt(Length2()); }

		public:
			static T Dot(const Vector & v0, const Vector & v1) {
				return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
			}
			T Dot(const Vector & v) const {
				return Dot(*this, v);
			}

			const Vector Norm() const {
				return *this / Length();
			}

			Vector & NormSelf() {
				(*this) /= Length();
				return *this;
			}

		public:
			const Vector operator+(const Vector &v) const {
				return Vector(x + v.x, y + v.y, z + v.z, w + v.w);
			}

			Vector & operator+=(const Vector &v) {
				x += v.x;
				y += v.y;
				z += v.z;
				w += v.w;
				return *this;
			}

			const Vector operator+(T val)const {
				return Vector(x + val, y + val, z + val, w + val);
			}

			Vector operator+=(T val) {
				x += val;
				y += val;
				z += val;
				w += val;
				return *this;
			}

			const Vector operator-(const Vector &v) const {
				return Vector(x - v.x, y - v.y, z - v.z, w - v.w);
			}

			const Vector operator-() const { return Vector(-x, -y, -z, -w); }

			Vector & operator-=(const Vector &v) {
				x -= v.x;
				y -= v.y;
				z -= v.z;
				w -= v.w;
				return *this;
			}

			const Vector operator-(T val)const {
				return Vector(x - val, y - val, z - val, w - val);
			}

			Vector operator-=(T val) {
				x -= val;
				y -= val;
				z -= val;
				w -= val;
				return *this;
			}

			template <typename U>
			const Vector operator*(U s) const {
				return Vector(s * x, s * y, s * z, s * w);
			}

			template <typename U>
			Vector & operator*=(U s) {
				x *= s;
				y *= s;
				z *= s;
				w *= s;
				return *this;
			}

			template <typename U>
			const Vector operator/(U f) const {
				float inv = (float)1 / f;
				return Vector(x * inv, y * inv, z * inv, w * inv);
			}

			template <typename U>
			Vector & operator/=(U f) {
				float inv = (float)1 / f;
				x *= inv;
				y *= inv;
				z *= inv;
				w *= inv;
				return *this;
			}
		};
	}
}

#endif // !_CPPUTIL_BASIC_MATH_VECTOR4_H_
