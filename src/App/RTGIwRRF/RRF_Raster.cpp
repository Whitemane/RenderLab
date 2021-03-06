#include "RRF_Raster.h"

#include <CppUtil/Qt/RawAPI_Define.h>
#include <CppUtil/Qt/RawAPI_OGLW.h>

#include <CppUtil/Engine/Scene.h>
#include <CppUtil/Engine/SObj.h>
#include <CppUtil/Engine/CmptMaterial.h>
#include <CppUtil/Engine/CmptGeometry.h>
#include <CppUtil/Engine/BSDF_FrostedGlass.h>

#include <CppUtil/OpenGL/CommonDefine.h>

#include <CppUtil/Basic/Image.h>
#include <CppUtil/Basic/EventManager.h>
#include <CppUtil/Basic/LambdaOp.h>
#include <CppUtil/Basic/Math.h>

#include <ROOT_PATH.h>

using namespace App;
using namespace CppUtil::OpenGL;
using namespace Define;
using namespace std;

void RRF_Raster::OGL_Init() {
	RasterBase::OGL_Init();

	scene->GenID();

	InitListeners();
	InitShaders();

	glViewport(0, 0, 512, 512);
}

void RRF_Raster::InitListeners() {
	EventMngr::GetInstance().Reg(static_cast<size_t>(Qt::Key_1), EventMngr::ENUM_EVENT_TYPE::KB_PRESS, 
		LambdaOp_New([=]() {
		for (auto & pair : id2shader) {
			pair.second.SetInt("mode", MODE::DIRECT);
		}
	}));
	EventMngr::GetInstance().Reg(static_cast<size_t>(Qt::Key_2), EventMngr::ENUM_EVENT_TYPE::KB_PRESS,
		LambdaOp_New([=]() {
		for (auto & pair : id2shader) {
			pair.second.SetInt("mode", MODE::INDIRECT);
		}
	}));
	EventMngr::GetInstance().Reg(static_cast<size_t>(Qt::Key_3), EventMngr::ENUM_EVENT_TYPE::KB_PRESS,
		LambdaOp_New([=]() {
		for (auto & pair : id2shader) {
			pair.second.SetInt("mode", MODE::GLOBAL);
		}
	}));

	EventMngr::GetInstance().Reg(static_cast<size_t>(Qt::Key_Up), EventMngr::ENUM_EVENT_TYPE::KB_PRESS,
		LambdaOp_New([&]() {
		interpolateRatio = std::min(1.0f, interpolateRatio + 0.01f);
		float val = Math::Lerp(0.02f, 1.0f, interpolateRatio);
		printf("interpolateRatio: %f\n", val);
		for (auto & pair : id2shader) {
			pair.second.SetFloat("interpolateRatio", val);
		}
	}));

	EventMngr::GetInstance().Reg(static_cast<size_t>(Qt::Key_Down), EventMngr::ENUM_EVENT_TYPE::KB_PRESS,
		LambdaOp_New([&]() {
		interpolateRatio = std::max(0.f, interpolateRatio - 0.01f);
		float val = Math::Lerp(0.02f, 1.0f, interpolateRatio);
		printf("interpolateRatio: %f\n", val);
		for (auto & pair : id2shader) {
			pair.second.SetFloat("interpolateRatio", val);
		}
	}));
}

void RRF_Raster::InitShaders() {
	vector<int> IDs;
	auto visitor = Visitor::New();
	visitor->Reg([&](Ptr<SObj> sobj) {
		auto material = sobj->GetComponent<CmptMaterial>();
		auto geometry = sobj->GetComponent<CmptGeometry>();
		if (!material || !geometry)
			return;

		auto bsdf = CastTo<BSDF_FrostedGlass>(material->material);
		if (bsdf == nullptr)
			return;

		IDs.push_back(scene->GetID(sobj));
	});
	scene->GetRoot()->TraverseAccept(visitor);

	for (auto ID : IDs)
		InitShader(ID);
}

void RRF_Raster::InitShader(int ID) {
	printf("init shader of ID %d ...\n", ID);

	string fsPath = ROOT_PATH + "data/shaders/App/RTGIwRRF/" + to_string(ID) + "_modelKDTree.fs";
	auto shader = Shader(ROOT_PATH + str_Basic_P3N3T2T3_vs, fsPath);
	if (!shader.IsValid())
		return;

	id2shader[ID] = shader;

	shader.SetInt("bsdf.colorTexture", 0);
	shader.SetInt("bsdf.roughnessTexture", 1);
	shader.SetInt("bsdf.aoTexture", 2);
	shader.SetInt("bsdf.normalTexture", 3);

	shader.SetInt("mode", MODE::GLOBAL);

	shader.SetFloat("interpolateRatio", 0.2f);

	RegShader(shader, 4);
	printf("init shader of ID %d success\n", ID);
}

void RRF_Raster::Draw() {
	RasterBase::Draw();
}

void RRF_Raster::Visit(Ptr<SObj> sobj) {
	curID = scene->GetID(sobj);

	RasterBase::Visit(sobj);
}

void RRF_Raster::Visit(Ptr<BSDF_FrostedGlass> bsdf) {
	auto target = id2shader.find(curID);
	if (target == id2shader.end()) {
		printf("ERROR: shader of ID[%d] not found\n", curID);
		return;
	}

	auto & shader = target->second;
	SetCurShader(shader);

	string strBSDF = "bsdf.";
	shader.SetVec3f(strBSDF + "colorFactor", bsdf->colorFactor);
	shader.SetFloat(strBSDF + "roughnessFactor", bsdf->roughnessFactor);

	const int texNum = 4;
	PtrC<Image> imgs[texNum] = { bsdf->colorTexture, bsdf->roughnessTexture, bsdf->aoTexture, bsdf->normalTexture };
	string names[texNum] = { "Color", "Roughness", "AO", "Normal" };

	for (int i = 0; i < texNum; i++) {
		string wholeName = strBSDF + "have" + names[i] + "Texture";
		if (imgs[i] && imgs[i]->IsValid()) {
			shader.SetBool(wholeName, true);
			pOGLW->GetTex(imgs[i]).Use(i);
		}
		else
			shader.SetBool(wholeName, false);
	}

	shader.SetFloat(strBSDF + "ior", bsdf->ior);

	UseLightTex(shader);
}
