#include "std_include.hpp"
#include "xmodel.hpp"

#include <utils/memory.hpp>

namespace zonetool
{
	namespace
	{
		h2::XModel* convert_to_h2(XModel* h1_asset, utils::memory::allocator& allocator)
		{
#define COPY_VALUE(name) \
		static_assert(sizeof(asset->name) == sizeof(h1_asset->name)); \
		asset->name = h1_asset->name;

#define COPY_ARR(name) \
		static_assert(sizeof(asset->name) == sizeof(h1_asset->name)); \
		std::memcpy(&asset->name, &h1_asset->name, sizeof(asset->name));

#define REINTERPRET_CAST_SAFE(name) \
		static_assert(sizeof(*asset->name) == sizeof(*h1_asset->name)); \
		asset->name = reinterpret_cast<decltype(asset->name)>(h1_asset->name);

			const auto asset = allocator.allocate<h2::XModel>();

			COPY_VALUE(name);
			COPY_VALUE(numBones);
			COPY_VALUE(numRootBones);
			COPY_VALUE(numsurfs);
			COPY_VALUE(lodRampType);
			COPY_VALUE(numBonePhysics);
			COPY_VALUE(numCompositeModels);
			asset->unk_float = -1.0f;
			COPY_VALUE(scale);
			COPY_ARR(noScalePartBits);
			REINTERPRET_CAST_SAFE(boneNames);
			REINTERPRET_CAST_SAFE(parentList);
			REINTERPRET_CAST_SAFE(tagAngles);
			REINTERPRET_CAST_SAFE(tagPositions);
			REINTERPRET_CAST_SAFE(partClassification);
			REINTERPRET_CAST_SAFE(baseMat);
			REINTERPRET_CAST_SAFE(reactiveMotionTweaks);

			asset->materialHandles = allocator.allocate_array<h2::Material*>(h1_asset->numsurfs);
			for (auto i = 0; i < h1_asset->numsurfs; i++)
			{
				const auto material = allocator.allocate<h2::Material>();
				material->name = h1_asset->materialHandles[i]->name;
				asset->materialHandles[i] = material;
			}

			for (auto i = 0; i < h1_asset->numLods; i++)
			{
				COPY_VALUE(lodInfo[i].dist);
				COPY_VALUE(lodInfo[i].numsurfs);
				COPY_VALUE(lodInfo[i].surfIndex);
				COPY_ARR(lodInfo[i].partBits);
				REINTERPRET_CAST_SAFE(lodInfo[i].modelSurfs);
				REINTERPRET_CAST_SAFE(lodInfo[i].surfs);
			}

			COPY_VALUE(numLods);
			COPY_VALUE(collLod);
			COPY_VALUE(flags);
			COPY_VALUE(numCollSurfs);
			REINTERPRET_CAST_SAFE(collSurfs);
			REINTERPRET_CAST_SAFE(boneInfo);
			COPY_VALUE(contents);
			COPY_VALUE(radius);
			COPY_VALUE(bounds);
			REINTERPRET_CAST_SAFE(invHighMipRadius);
			REINTERPRET_CAST_SAFE(mdaoVolumes);
			COPY_VALUE(mdaoVolumeCount);
			COPY_VALUE(targetCount);
			COPY_VALUE(numberOfWeights);
			COPY_VALUE(numberOfWeightMaps);
			REINTERPRET_CAST_SAFE(weightNames);
			REINTERPRET_CAST_SAFE(blendShapeWeightMap);
			REINTERPRET_CAST_SAFE(physPreset);
			REINTERPRET_CAST_SAFE(physCollmap);
			COPY_VALUE(quantization);
			COPY_VALUE(memUsage);
			REINTERPRET_CAST_SAFE(skeletonScript);

			asset->compositeModels = allocator.allocate_array<h2::XModel*>(h1_asset->numCompositeModels);
			for (auto i = 0; i < h1_asset->numCompositeModels; i++)
			{
				const auto model = allocator.allocate<h2::XModel>();
				model->name = h1_asset->compositeModels[i]->name;
				asset->compositeModels[i] = model;
			}

			REINTERPRET_CAST_SAFE(bonePhysics);

			return asset;
		}
	}

	void IXModel::add_script_string(scr_string_t* ptr, std::string str)
	{
		for (std::uint32_t i = 0; i < this->script_strings.size(); i++)
		{
			if (this->script_strings[i].first == ptr)
			{
				return;
			}
		}
		this->script_strings.push_back(std::pair<scr_string_t*, std::string>(ptr, str));
	}

	std::string IXModel::get_script_string(scr_string_t* ptr)
	{
		for (std::uint32_t i = 0; i < this->script_strings.size(); i++)
		{
			if (this->script_strings[i].first == ptr)
			{
				return this->script_strings[i].second;
			}
		}
		return "";
	}

	XModel* IXModel::parse(std::string name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void IXModel::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IXModel::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{

	}

	void IXModel::load_depending(IZone* zone)
	{

	}

	std::string IXModel::name()
	{
		return this->name_;
	}

	std::int32_t IXModel::type()
	{
		return ASSET_TYPE_XMODEL;
	}

	void IXModel::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void IXModel::dump(XModel* h1_asset)
	{
		utils::memory::allocator allocator;

		const auto asset = convert_to_h2(h1_asset, allocator);
		const auto path = "xmodel\\"s + asset->name + ".xmodel_export";

		assetmanager::dumper dump;
		if (!dump.open(path))
		{
			return;
		}

		// asset
		dump.dump_single(asset);
		dump.dump_string(asset->name);

		// tags
		dump.dump_array(asset->boneNames, asset->numBones);
		for (unsigned char i = 0; i < asset->numBones; i++)
		{
			dump.dump_string(SL_ConvertToString(asset->boneNames[i]));
		}

		// basic info
		dump.dump_array(asset->parentList, asset->numBones - asset->numRootBones);
		dump.dump_array(asset->tagAngles, asset->numBones - asset->numRootBones);
		dump.dump_array(asset->tagPositions, asset->numBones - asset->numRootBones);
		dump.dump_array(asset->partClassification, asset->numBones);
		dump.dump_array(asset->baseMat, asset->numBones);
		//dump.dump_array(asset->reactiveMotionParts, asset->numReactiveMotionParts);
		dump.dump_single(asset->reactiveMotionTweaks);
		dump.dump_array(asset->collSurfs, asset->numCollSurfs);
		dump.dump_array(asset->boneInfo, asset->numBones);
		dump.dump_array(asset->invHighMipRadius, asset->numsurfs);

		// surfaces
		dump.dump_array(asset->materialHandles, asset->numsurfs);
		for (unsigned char i = 0; i < asset->numsurfs; i++)
		{
			dump.dump_asset(asset->materialHandles[i]);
		}

		// lods
		for (auto i = 0; i < 6; i++)
		{
			dump.dump_asset(asset->lodInfo[i].modelSurfs);
			dump.dump_array(asset->lodInfo[i].reactiveMotionParts, asset->lodInfo[i].numReactiveMotionParts);
		}

		// physics subassets
		dump.dump_asset(asset->physPreset);
		dump.dump_asset(asset->physCollmap);

		// weights
		dump.dump_array(asset->weightNames, asset->numberOfWeights);
		for (unsigned short i = 0; i < asset->numberOfWeights; i++)
		{
			dump.dump_string(SL_ConvertToString(asset->weightNames[i]));
		}

		// blendshapeweights
		dump.dump_array(asset->blendShapeWeightMap, asset->numberOfWeightMaps);

		// mdao
		dump.dump_array(asset->mdaoVolumes, asset->mdaoVolumeCount);
		for (auto i = 0; i < asset->mdaoVolumeCount; i++)
		{
			dump.dump_asset(asset->mdaoVolumes[i].volumeData);
		}

		// extra models
		dump.dump_array(asset->compositeModels, asset->numCompositeModels);
		for (char i = 0; i < asset->numCompositeModels; i++)
		{
			dump.dump_asset(asset->compositeModels[i]);
		}

		// skeletonscript subasset
		dump.dump_asset(asset->skeletonScript);

		// bone physics
		dump.dump_array(asset->bonePhysics, asset->numBonePhysics);
		for (char i = 0; i < asset->numBonePhysics; i++)
		{
			dump.dump_asset(asset->bonePhysics[i].physPreset);
			dump.dump_asset(asset->bonePhysics[i].physContraint);
			dump.dump_asset(asset->bonePhysics[i].physCollmap);
		}

		dump.close();
	}
}