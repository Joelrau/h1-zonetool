#include <std_include.hpp>
#include "comworld.hpp"

namespace zonetool
{
	namespace
	{
#define COPY_VALUE(name) \
		lights[i].name = h1_lights[i].name; \

#define COPY_ARR(name) \
		std::memcpy(&lights[i].name, &h1_lights[i].name, sizeof(lights[i].name)); \

		h2::ComPrimaryLight* convert_primary_lights(ComPrimaryLight* h1_lights, const unsigned int count, 
			utils::memory::allocator& allocator)
		{
			const auto lights = allocator.allocate_array<h2::ComPrimaryLight>(count);

			for (auto i = 0u; i < count; i++)
			{
				lights[i].type = static_cast<h2::GfxLightType>(h1_lights[i].type);
				COPY_VALUE(canUseShadowMap);
				COPY_VALUE(needsDynamicShadows);
				COPY_VALUE(exponent);
				COPY_VALUE(isVolumetric);
				COPY_ARR(color);
				std::memcpy(lights[i].color2, lights[i].color, sizeof(lights[i].color));
				COPY_ARR(dir);
				COPY_ARR(up);
				COPY_ARR(origin);
				COPY_ARR(fadeOffset);
				COPY_VALUE(bulbRadius);
				COPY_ARR(bulbLength);
				COPY_VALUE(radius);
				COPY_VALUE(cosHalfFovOuter);
				COPY_VALUE(cosHalfFovInner);
				COPY_VALUE(cosHalfFovExpanded);
				COPY_VALUE(rotationLimit);
				COPY_VALUE(translationLimit);
				COPY_VALUE(cucRotationOffsetRad);
				COPY_VALUE(cucRotationSpeedRad);
				COPY_ARR(cucScrollVector);
				COPY_ARR(cucScaleVector);
				COPY_ARR(cucTransVector);
				lights[i].defName = h1_lights[i].defName;
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