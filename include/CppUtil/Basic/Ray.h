#ifndef _CPPUTIL_BASIC_MATH_RAY_H_
#define _CPPUTIL_BASIC_MATH_RAY_H_

#include <CppUtil/Basic/Point3.h>
#include <CppUtil/Basic/Vector3.h>

namespace CppUtil {
	namespace Basic {
		class Ray {
		public:
			Ray(const Point3 & o, const Vec3 & d) : o(o), d(d) { }
			
		public:
			bool HasNaN() const { return o.HasNaN() || d.HasNaN(); }
			Point3 operator()(float t) const { return o + t * d; }

			friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
				os << "[o=" << r.o << ", d=" << r.d << "]";
				return os;
			}

		public:
			Point3 o;
			Vec3 d;
		};
	}
}

#endif // !_CPPUTIL_BASIC_MATH_RAY_H_
