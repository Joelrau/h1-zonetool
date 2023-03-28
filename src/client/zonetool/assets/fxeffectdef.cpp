#include <std_include.hpp>
#include "fxeffectdef.hpp"

namespace zonetool
{
	namespace
	{
#define COPY_VALUE(name) \
		static_assert(sizeof(asset->elemDefs[i].name) == sizeof(h1_asset->elemDefs[i].name)); \
		asset->elemDefs[i].name = h1_asset->elemDefs[i].name;

#define COPY_VALUE_CAST(name) \
		static_assert(sizeof(asset->elemDefs[i].name) == sizeof(h1_asset->elemDefs[i].name)); \
		asset->elemDefs[i].name = *reinterpret_cast<decltype(asset->elemDefs[i].name)*>(&h1_asset->elemDefs[i].name);

#define COPY_ARR(name) \
		static_assert(sizeof(asset->elemDefs[i].name) == sizeof(h1_asset->elemDefs[i].name)); \
		std::memcpy(&asset->elemDefs[i].name, &h1_asset->elemDefs[i].name, sizeof(asset->elemDefs[i].name));

#define REINTERPRET_CAST_SAFE(name) \
		static_assert(sizeof(*asset->elemDefs[i].name) == sizeof(*h1_asset->elemDefs[i].name)); \
		asset->elemDefs[i].name = reinterpret_cast<decltype(asset->elemDefs[i].name)>(h1_asset->elemDefs[i].name);

		h2::FxEffectDef* convert_to_h2(FxEffectDef* h1_asset, utils::memory::allocator& allocator)
		{
			const auto asset = allocator.allocate<h2::FxEffectDef>();

			std::memcpy(asset, h1_asset, sizeof(FxEffectDef));
			const auto elem_count = asset->elemDefCountLooping + asset->elemDefCountOneShot + asset->elemDefCountEmission;
			asset->elemDefs = allocator.allocate_array<h2::FxElemDef>(elem_count);

			for (auto i = 0; i < elem_count; i++)
			{
				COPY_VALUE(flags);
				COPY_VALUE(flags2);
				COPY_VALUE_CAST(spawn);
				COPY_VALUE_CAST(spawnRange);
				COPY_VALUE_CAST(fadeInRange);
				COPY_VALUE_CAST(fadeOutRange);
				COPY_VALUE(spawnFrustumCullRadius);
				COPY_VALUE_CAST(spawnDelayMsec);
				COPY_VALUE_CAST(lifeSpanMsec);
				COPY_ARR(spawnOrigin);
				COPY_VALUE_CAST(spawnOffsetRadius);
				COPY_VALUE_CAST(spawnOffsetHeight);
				COPY_ARR(spawnAngles);
				COPY_ARR(angularVelocity);
				COPY_VALUE_CAST(initialRotation);
				COPY_VALUE_CAST(gravity);
				COPY_VALUE_CAST(reflectionFactor);
				COPY_VALUE_CAST(atlas);
				COPY_VALUE_CAST(elemType);
				COPY_VALUE(elemLitType);
				COPY_VALUE(visualCount);
				COPY_VALUE(velIntervalCount);
				COPY_VALUE(visStateIntervalCount);

				if ((((asset->elemDefs[i].flags & 0x30) - 16) & 0xFFFFFFEF) != 0)
				{
					std::memset(&asset->elemDefs[i].spawnOffsetRadius, 0, sizeof(FxFloatRange));
				}

				const auto vel_count = asset->elemDefs[i].velIntervalCount + 1;
				asset->elemDefs[i].velSamples = allocator.allocate_array<h2::FxElemVelStateSample>(vel_count);
				for (auto o = 0; o < vel_count; o++)
				{
					COPY_ARR(velSamples[o].local.velocity.base);
					COPY_ARR(velSamples[o].local.velocity.amplitude);
					std::memcpy(&asset->elemDefs[i].velSamples[o].local.velocity.unk_vec, 
						asset->elemDefs[i].velSamples[o].local.velocity.amplitude, 
						sizeof(float[3])
					);
					COPY_ARR(velSamples[o].local.totalDelta.base);
					COPY_ARR(velSamples[o].local.totalDelta.amplitude);
					std::memcpy(&asset->elemDefs[i].velSamples[o].local.totalDelta.unk_vec,
						asset->elemDefs[i].velSamples[o].local.totalDelta.amplitude,
						sizeof(float[3])
					);

					COPY_ARR(velSamples[o].world.velocity.base);
					COPY_ARR(velSamples[o].world.velocity.amplitude);
					std::memcpy(&asset->elemDefs[i].velSamples[o].world.totalDelta.unk_vec,
						asset->elemDefs[i].velSamples[o].world.totalDelta.amplitude,
						sizeof(float[3])
					);
					COPY_ARR(velSamples[o].world.totalDelta.base);
					COPY_ARR(velSamples[o].world.totalDelta.amplitude);
					std::memcpy(&asset->elemDefs[i].velSamples[o].world.totalDelta.unk_vec,
						asset->elemDefs[i].velSamples[o].world.totalDelta.amplitude,
						sizeof(float[3])
					);
				}

				REINTERPRET_CAST_SAFE(visSamples);
				REINTERPRET_CAST_SAFE(visuals.markArray);
				COPY_VALUE(collBounds);
				REINTERPRET_CAST_SAFE(effectOnImpact.name);
				REINTERPRET_CAST_SAFE(effectOnDeath.name);
				REINTERPRET_CAST_SAFE(effectEmitted.name);
				COPY_VALUE_CAST(emitDist);
				COPY_VALUE_CAST(emitDistVariance);
				REINTERPRET_CAST_SAFE(extended.unknownDef);

				if (asset->elemDefs[i].extended.trailDef)
				{
					if (asset->elemDefs[i].elemType == FX_ELEM_TYPE_TRAIL)//
					{
						asset->elemDefs[i].extended.trailDef = allocator.allocate<h2::FxTrailDef>();
						COPY_VALUE(extended.trailDef->scrollTimeMsec);
						COPY_VALUE(extended.trailDef->repeatDist);
						COPY_VALUE(extended.trailDef->invSplitDist);
						COPY_VALUE(extended.trailDef->vertCount);
						REINTERPRET_CAST_SAFE(extended.trailDef->verts);
						COPY_VALUE(extended.trailDef->indCount);
						REINTERPRET_CAST_SAFE(extended.trailDef->inds);
					}
					else if (asset->elemDefs[i].elemType == FX_ELEM_TYPE_DECAL)
					{
						asset->elemDefs[i].extended.decalDef = allocator.allocate<h2::FxDecalDef>();
					}
				}

				COPY_VALUE(sortOrder);
				COPY_VALUE(lightingFrac);
				COPY_VALUE(useItemClip);
				COPY_VALUE(fadeInfo);
				COPY_VALUE(randomSeed);
			}

			return asset;
		}
	}

	void parse_visuals(assetmanager::reader* read, FxElemDef* def, FxElemVisuals* vis)
	{

	}

	FxEffectDef* IFxEffectDef::parse(const std::string& name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void IFxEffectDef::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IFxEffectDef::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{
	}

	void IFxEffectDef::load_depending(IZone* zone)
	{

	}

	std::string IFxEffectDef::name()
	{
		return this->name_;
	}

	std::int32_t IFxEffectDef::type()
	{
		return ASSET_TYPE_FX;
	}

	void write_fx_elem_visuals(IZone* zone, ZoneBuffer* buf, FxElemDef* def, FxElemVisuals* dest)
	{

	}

	void write_fx_elem_def_visuals(IZone* zone, ZoneBuffer* buf, FxElemDef* def, FxElemDefVisuals* dest)
	{

	}

	void write_fx_elem_def(IZone* zone, ZoneBuffer* buf, FxElemDef* dest)
	{

	}

	void IFxEffectDef::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void dump_visuals(assetmanager::dumper* dump, h2::FxElemDef* def, h2::FxElemVisuals* vis)
	{
		switch (def->elemType)
		{
		case FX_ELEM_TYPE_MODEL:
			dump->dump_asset(vis->model);
			break;
		case FX_ELEM_TYPE_RUNNER:
			dump->dump_asset(vis->effectDef.handle);
			break;
		case FX_ELEM_TYPE_SOUND:
		case FX_ELEM_TYPE_VECTORFIELD:
			dump->dump_string(vis->soundName);
			break;
		case FX_ELEM_TYPE_PARTICLE_SIM_ANIMATION:
			dump->dump_asset(vis->particleSimAnimation);
			break;
		default:
			if (def->elemType - 12 <= 1u)
			{
				if (def->elemType == FX_ELEM_TYPE_SPOT_LIGHT)
				{
					dump->dump_asset(vis->lightDef);
				}
			}
			else
			{
				dump->dump_asset(vis->material);
			}
			break;
		}
	}

	void IFxEffectDef::dump(FxEffectDef* h1_asset)
	{
		assetmanager::dumper dump;

		utils::memory::allocator allocator;
		const auto asset = convert_to_h2(h1_asset, allocator);
		const auto path = "effects\\"s + asset->name + ".fxe"s;
		if (!dump.open(path))
		{
			return;
		}

		dump.dump_single(asset);
		dump.dump_string(asset->name);
		dump.dump_array(asset->elemDefs,
			asset->elemDefCountLooping + asset->elemDefCountOneShot + asset->elemDefCountEmission);

		// dump elemDefs
		for (auto i = 0; i < asset->elemDefCountLooping + asset->elemDefCountOneShot + asset->elemDefCountEmission; i++)
		{
			auto def = &asset->elemDefs[i];

			// dump elem samples
			dump.dump_array(def->velSamples, def->velIntervalCount + 1);
			dump.dump_array(def->visSamples, def->visStateIntervalCount + 1);

			// dump visuals
			if (def->elemType == FX_ELEM_TYPE_DECAL)
			{
				if (def->visuals.markArray)
				{
					dump.dump_array(def->visuals.markArray, def->visualCount);

					for (unsigned char a = 0; a < def->visualCount; a++)
					{
						if (def->visuals.markArray[a].materials[0])
						{
							dump.dump_asset(def->visuals.markArray[a].materials[0]);
						}
						if (def->visuals.markArray[a].materials[1])
						{
							dump.dump_asset(def->visuals.markArray[a].materials[1]);
						}
						if (def->visuals.markArray[a].materials[2])
						{
							dump.dump_asset(def->visuals.markArray[a].materials[2]);
						}
					}
				}
			}
			else if (def->visualCount > 1)
			{
				if (def->visuals.markArray)
				{
					dump.dump_array(def->visuals.array, def->visualCount);
					for (unsigned char vis = 0; vis < def->visualCount; vis++)
					{
						dump_visuals(&dump, def, &def->visuals.array[vis]);
					}
				}
			}
			else
			{
				dump_visuals(&dump, def, &def->visuals.instance);
			}

			// dump reference FX defs
			dump.dump_asset(def->effectOnImpact.handle);
			dump.dump_asset(def->effectOnDeath.handle);
			dump.dump_asset(def->effectEmitted.handle);

			// dump extended FX data
			if (def->extended.trailDef)
			{
				if (def->elemType == FX_ELEM_TYPE_TRAIL)
				{
					dump.dump_single(def->extended.trailDef);

					if (def->extended.trailDef->verts)
					{
						dump.dump_array(def->extended.trailDef->verts, def->extended.trailDef->vertCount);
					}

					if (def->extended.trailDef->inds)
					{
						dump.dump_array(def->extended.trailDef->inds, def->extended.trailDef->indCount);
					}
				}
				else if (def->elemType == FX_ELEM_TYPE_SPARK_FOUNTAIN)
				{
					dump.dump_single(def->extended.sparkFountainDef);
				}
				else if (def->elemType == FX_ELEM_TYPE_SPOT_LIGHT)
				{
					dump.dump_single(def->extended.spotLightDef);
				}
				else if (def->elemType == FX_ELEM_TYPE_OMNI_LIGHT)
				{
					dump.dump_single(def->extended.omniLightDef);
				}
				else if (def->elemType == FX_ELEM_TYPE_FLARE)
				{
					dump.dump_single(def->extended.flareDef);

					if (def->extended.flareDef->intensityX)
					{
						dump.dump_array(def->extended.flareDef->intensityX, def->extended.flareDef->intensityXIntervalCount + 1);
					}

					if (def->extended.flareDef->intensityY)
					{
						dump.dump_array(def->extended.flareDef->intensityY, def->extended.flareDef->intensityYIntervalCount + 1);
					}

					if (def->extended.flareDef->srcCosIntensity)
					{
						dump.dump_array(def->extended.flareDef->srcCosIntensity, def->extended.flareDef->srcCosIntensityIntervalCount + 1);
					}

					if (def->extended.flareDef->srcCosScale)
					{
						dump.dump_array(def->extended.flareDef->srcCosScale, def->extended.flareDef->srcCosScaleIntervalCount + 1);
					}
				}
				else if (def->elemType == FX_ELEM_TYPE_DECAL)
				{
					dump.dump_single(def->extended.decalDef);
				}
				else
				{
					dump.dump_single(def->extended.unknownDef);
				}
			}
		}

		dump.close();
	}
}