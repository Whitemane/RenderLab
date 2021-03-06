#include <CppUtil/Engine/GGX.h>

#include <CppUtil/Engine/SurfCoord.h>

using namespace CppUtil;
using namespace CppUtil::Basic;
using namespace CppUtil::Engine;

float GGX::D(const Normalf & wh) const {
	if (SurfCoord::CosTheta(wh) < 0)
		return 0.f;

	float alpha2 = alpha * alpha;
	float cos2Theta = SurfCoord::Cos2Theta(wh);
	
	float t = (alpha2 - 1) * cos2Theta + 1;

	return alpha2 / (Math::PI * t * t);
}

float GGX::Lambda(const Normalf & w) const {
	auto absTanTheta = std::abs(SurfCoord::TanTheta(w));
	if (std::isinf(absTanTheta))
		return 0.f;

	auto alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);

	return (-1.f + std::sqrt(1.f + alpha2Tan2Theta)) / 2.f;
}

const Normalf GGX::Sample_wh() const {
	// sample
	const float Xi1 = Math::Rand_F();
	const float Xi2 = Math::Rand_F();

	// theta
	const auto cos2Theta = (1 - Xi1) / ((alpha*alpha - 1)*Xi1 + 1);
	const auto cosTheta = sqrt(cos2Theta);
	const auto sinTheta = sqrt(1 - cos2Theta);

	// phi
	const auto phi = 2 * Math::PI * Xi2;

	return SurfCoord::SphericalDirection(sinTheta, cosTheta, phi);
}
