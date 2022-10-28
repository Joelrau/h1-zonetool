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

			lights->type = static_cast<h2::GfxLightType>(primary_lights->type);
			lights->canUseShadowMap = primary_lights->canUseShadowMap;
			lights->needsDynamicShadows = primary_lights->needsDynamicShadows;
			lights->exponent = primary_lights->exponent;
			lights->isVolumetric = primary_lights->isVolumetric;
			std::memcpy(&lights->color, &primary_lights->color, sizeof(h2::ComPrimaryLight) -
				(offsetof(h2::ComPrimaryLight, __pad0) + sizeof(lights->__pad0)));

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