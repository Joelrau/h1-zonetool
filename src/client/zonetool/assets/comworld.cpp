#include <std_include.hpp>
#include "comworld.hpp"

namespace zonetool
{
	namespace
	{
		h2::ComPrimaryLight* convert_primary_lights(ComPrimaryLight* primary_lights, const unsigned int count, 
			utils::memory::allocator& allocator)
		{
			const auto lights = allocator.allocate_array<h2::ComPrimaryLight>(count);

			for (auto i = 0; i < count; i++)
			{
				lights[i].type = static_cast<h2::GfxLightType>(primary_lights[i].type);
				lights[i].canUseShadowMap = primary_lights[i].canUseShadowMap;
				lights[i].needsDynamicShadows = primary_lights[i].needsDynamicShadows;
				lights[i].exponent = primary_lights[i].exponent;
				lights[i].isVolumetric = primary_lights[i].isVolumetric;
				std::memcpy(&lights[i].color, &primary_lights[i].color, sizeof(h2::ComPrimaryLight) -
					(offsetof(h2::ComPrimaryLight, __pad0) + sizeof(lights[i].__pad0)));
				lights[i].defName = primary_lights[i].defName;
			}

			return lights;
		}
	}

	ComWorld* IComWorld::parse(const std::string& name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void IComWorld::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IComWorld::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{
	}

	void IComWorld::load_depending(IZone* zone)
	{

	}

	std::string IComWorld::name()
	{
		return this->name_;
	}

	std::int32_t IComWorld::type()
	{
		return ASSET_TYPE_COM_MAP;
	}

	void IComWorld::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	static_assert(sizeof(ComWorld) == sizeof(h2::ComWorld));

	void IComWorld::dump(ComWorld* h1_asset)
	{
		const auto asset = reinterpret_cast<h2::ComWorld*>(h1_asset);
		const auto path = asset->name + ".commap"s;

		assetmanager::dumper write;
		if (!write.open(path))
		{
			return;
		}

		utils::memory::allocator allocator;
		asset->primaryLights = convert_primary_lights(h1_asset->primaryLights,
			asset->primaryLightCount, allocator);

		write.dump_single(asset);
		write.dump_string(asset->name);

		write.dump_array(asset->primaryLights, asset->primaryLightCount);
		for (unsigned int i = 0; i < asset->primaryLightCount; i++)
		{
			write.dump_string(asset->primaryLights[i].defName);
		}
		write.dump_array(asset->primaryLightEnvs, asset->primaryLightEnvCount);

		write.close();
	}
}