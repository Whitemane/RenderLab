#ifndef _ENGINE_MATERIAL_BSDF_METAL_WORKFLOW_H_
#define _ENGINE_MATERIAL_BSDF_METAL_WORKFLOW_H_

#include <CppUtil/Engine/BSDF.h>

#include <CppUtil/Engine/SchlickGGX.h>

namespace CppUtil {
	namespace Basic {
		class Image;
	}

	namespace Engine {

		// Disney
		class BSDF_MetalWorkflow : public BSDF {
		public:
			BSDF_MetalWorkflow(const RGBf & colorFactor = RGBf(1.f), float roughnessFactor = 1.f, float metallicFactor = 1.f)
				: colorFactor(colorFactor), roughnessFactor(roughnessFactor), metallicFactor(metallicFactor),
				albedoTexture(nullptr), metallicTexture(nullptr), aoTexture(nullptr) { }

		public:
			static const Basic::Ptr<BSDF_MetalWorkflow> New(const RGBf & colorFactor = RGBf(1.f), float roughnessFactor = 1.f, float metallicFactor = 1.f) {
				return Basic::New<BSDF_MetalWorkflow>(colorFactor, roughnessFactor, metallicFactor);
			}

		protected:
			virtual ~BSDF_MetalWorkflow() = default;

		public:
			virtual const RGBf F(const Normalf & wo, const Normalf & wi, const Point2 & texcoord) override;

			// probability density function
			virtual float PDF(const Normalf & wo, const Normalf & wi, const Point2 & texcoord) override;

			// PD is probability density
			// return albedo
			virtual const RGBf Sample_f(const Normalf & wo, const Point2 & texcoord, Normalf & wi, float & PD) override;

			virtual void ChangeNormal(const Point2 & texcoord, const Normalf & tangent, Normalf & normal) const override;

		public:
			Basic::PtrC<Basic::Image> GetAlbedoTexture() const { return albedoTexture; }
			Basic::PtrC<Basic::Image> GetMetallicTexture() const { return metallicTexture; }
			Basic::PtrC<Basic::Image> GetRoughnessTexture() const { return roughnessTexture; }
			Basic::PtrC<Basic::Image> GetAOTexture() const { return aoTexture ; }
			Basic::PtrC<Basic::Image> GetNormalTexture() const { return normalTexture; }

			void SetAlbedoTexture(Basic::Ptr<Basic::Image> albedoTexture) {
				this->albedoTexture = albedoTexture;
			}

			void SetMetallicTexture(Basic::Ptr<Basic::Image> metallicTexture) {
				this->metallicTexture = metallicTexture;
			}

			void SetRoughnessTexture(Basic::Ptr<Basic::Image> roughnessTexture) {
				this->roughnessTexture = roughnessTexture;
			}

			void SetAOTexture(Basic::Ptr<Basic::Image> aoTexture) {
				this->aoTexture = aoTexture;
			}

			void SetNormalTexture(Basic::Ptr<Basic::Image> normalTexture) {
				this->normalTexture = normalTexture;
			}

		private:
			// Fresnel
			static const RGBf Fr(const Normalf & w, const Normalf & h, const RGBf & albedo, float metallic);

			const RGBf GetAlbedo(const Point2 & texcoord) const;
			float GetMetallic(const Point2 & texcoord) const;
			float GetRoughness(const Point2 & texcoord) const;
			float GetAO(const Point2 & texcoord) const;

		public:
			SchlickGGX sggx;

			RGBf colorFactor;
			Basic::Ptr<Basic::Image> albedoTexture;

			// 0--1
			float metallicFactor;
			Basic::Ptr<Basic::Image> metallicTexture;

			// 0--1
			float roughnessFactor;
			Basic::Ptr<Basic::Image> roughnessTexture;

			Basic::Ptr<Basic::Image> aoTexture;

			Basic::Ptr<Basic::Image> normalTexture;
		private:
		};
	}
}

#endif//!_ENGINE_MATERIAL_BSDF_METAL_WORKFLOW_H_
