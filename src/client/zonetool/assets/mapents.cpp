#include <std_include.hpp>
#include "mapents.hpp"

#include <xsk/gsc/types.hpp>
#include <xsk/resolver.hpp>

namespace zonetool
{
	namespace
	{
		std::string replace_mapents_keys(const std::string& data)
		{
			std::string buffer{};
			const auto lines = utils::string::split(data, '\n');

			for (const auto& line : lines)
			{
				const auto _0 = gsl::finally([&]
				{
					buffer.append("\n");
				});

				if (line.starts_with("{") || line.starts_with("}"))
				{
					buffer.append(line);
					continue;
				}

				const auto first_space = line.find_first_of(' ');
				if (first_space == std::string::npos)
				{
					continue;
				}

				const auto id = static_cast<unsigned int>(std::atoi(line.substr(0, first_space).data()));
				if (id == 0) // 0 "key" "value"
				{
					buffer.append(line);
					continue;
				}

				const auto token = xsk::gsc::h1::resolver::token_name(static_cast<std::uint16_t>(id));
				const auto key = "\"" + token + "\"";

				const auto new_line = key + line.substr(first_space);
				buffer.append(new_line);
			}

			return buffer;
		}
	}

	void IMapEnts::add_script_string(scr_string_t* ptr, std::string str)
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

	std::string IMapEnts::get_script_string(scr_string_t* ptr)
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

	void IMapEnts::parse_splineList(ZoneMemory* mem, std::string name, SplineRecordList* splineList)
	{

	}

	void IMapEnts::parse_spawnList(ZoneMemory* mem, std::string name, SpawnPointRecordList* spawnList)
	{

	}

	void IMapEnts::parse_clientBlendTriggers(ZoneMemory* mem, std::string name, ClientTriggerBlend* clientTriggerBlend)
	{

	}

	void IMapEnts::parse_clientTriggers(ZoneMemory* mem, std::string name, ClientTriggers* clientTrigger)
	{

	}

	void IMapEnts::parse_triggers(ZoneMemory* mem, std::string name, MapTriggers* trigger)
	{

	}

	void IMapEnts::parse_entityStrings(ZoneMemory* mem, std::string name, char** entityStrings, int* numEntityChars)
	{

	}

	MapEnts* IMapEnts::parse(std::string name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void IMapEnts::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IMapEnts::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{

	}

	void IMapEnts::load_depending(IZone* zone)
	{
	}

	std::string IMapEnts::name()
	{
		return this->name_;
	}

	std::int32_t IMapEnts::type()
	{
		return ASSET_TYPE_MAP_ENTS;
	}

	void IMapEnts::write_triggers(ZoneBuffer* buf, MapTriggers* dest)
	{

	}

	void IMapEnts::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void IMapEnts::dump_splineList(const std::string& name, SplineRecordList* splineList)
	{
		assetmanager::dumper dumper;
		const auto path = name + ".ents.splineList"s;
		if (dumper.open(path))
		{
			dumper.dump_short(splineList->splineCount);
			dumper.dump_array(splineList->splines, splineList->splineCount);
			for (unsigned short i = 0; i < splineList->splineCount; i++)
			{
				dumper.dump_array(splineList->splines[i].splinePoints, splineList->splines[i].splinePointCount);
				for (unsigned short j = 0; j < splineList->splines[i].splinePointCount; j++)
				{
					dumper.dump_array(splineList->splines[i].splinePoints[j].splineNodeLabel, 64);
					dumper.dump_array(splineList->splines[i].splinePoints[j].positionCubic, 48);
					dumper.dump_array(splineList->splines[i].splinePoints[j].tangentQuadratic, 36);
				}
			}
		}
	}

	void IMapEnts::dump_spawnList(const std::string& name, SpawnPointRecordList* spawnList)
	{
		const auto path = name + ".ents.spawnList"s;
		auto file = filesystem::file(path);
		file.open("wb");

		if (!file.get_fp())
		{
			return;
		}

		ordered_json data;

		for (unsigned short i = 0; i < spawnList->spawnsCount; i++)
		{
			data[i]["name"] = SL_ConvertToString(spawnList->spawns[i].name);
			data[i]["target"] = SL_ConvertToString(spawnList->spawns[i].target);
			data[i]["script_noteworthy"] = SL_ConvertToString(spawnList->spawns[i].script_noteworthy);
			data[i]["unknown"] = SL_ConvertToString(spawnList->spawns[i].unknown);
			for (auto j = 0; j < 3; j++)
			{
				data[i]["origin"][j] = spawnList->spawns[i].origin[j];
				data[i]["angles"][j] = spawnList->spawns[i].angles[j];
			}
		}

		const auto json = data.dump(4);
		file.write(json.data(), json.size(), 1);

		file.close();
	}

	void IMapEnts::dump_clientBlendTriggers(const std::string& name, ClientTriggerBlend* clientTriggerBlend)
	{
		assetmanager::dumper dumper;
		const auto path = name + ".ents.clientBlendTriggers"s;
		if (dumper.open(path))
		{
			dumper.dump_short(clientTriggerBlend->numClientTriggerBlendNodes);
			dumper.dump_array(clientTriggerBlend->blendNodes, clientTriggerBlend->numClientTriggerBlendNodes);
		}
	}

	void IMapEnts::dump_clientTriggers(const std::string& name, ClientTriggers* clientTrigger)
	{
		assetmanager::dumper dumper;
		const auto path = name + ".ents.clientTriggers";
		if (dumper.open(path))
		{
			dumper.dump_int(clientTrigger->trigger.count);
			dumper.dump_array<TriggerModel>(clientTrigger->trigger.models, clientTrigger->trigger.count);

			dumper.dump_int(clientTrigger->trigger.hullCount);
			dumper.dump_array<TriggerHull>(clientTrigger->trigger.hulls, clientTrigger->trigger.hullCount);

			dumper.dump_int(clientTrigger->trigger.slabCount);
			dumper.dump_array<TriggerSlab>(clientTrigger->trigger.slabs, clientTrigger->trigger.slabCount);

			dumper.dump_array(clientTrigger->clientTriggerAabbTree, clientTrigger->numClientTriggerNodes);

			dumper.dump_int(clientTrigger->triggerStringLength);
			dumper.dump_array(clientTrigger->triggerString, clientTrigger->triggerStringLength);

			dumper.dump_array(clientTrigger->visionSetTriggers, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->blendLookup, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->unk1, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->triggerType, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->origins, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->scriptDelay, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->audioTriggers, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->unk2, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->unk3, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->unk4, clientTrigger->trigger.count);
			dumper.dump_array(clientTrigger->unk5, clientTrigger->trigger.count);

			dumper.close();
		}
	}

	void IMapEnts::dump_triggers(const std::string& name, MapTriggers* trigger)
	{
		assetmanager::dumper dumper;
		const auto path = name + ".ents.triggers"s;
		if (dumper.open(path))
		{
			dumper.dump_int(trigger->count);
			dumper.dump_array<TriggerModel>(trigger->models, trigger->count);

			dumper.dump_int(trigger->hullCount);
			dumper.dump_array<TriggerHull>(trigger->hulls, trigger->hullCount);

			dumper.dump_int(trigger->slabCount);
			dumper.dump_array<TriggerSlab>(trigger->slabs, trigger->slabCount);

			dumper.close();
		}
	}

	void IMapEnts::dump_entityStrings(const std::string& name, char* entityString, int numEntityChars)
	{
		const auto path = name + ".ents"s;
		auto file = filesystem::file(path);
		file.open("wb");
		if (file.get_fp())
		{
			const auto converted = replace_mapents_keys({entityString, static_cast<size_t>(numEntityChars)});
			file.write(converted.data(), converted.size(), 1);
			file.close();
		}
	}

	void IMapEnts::dump(MapEnts* asset)
	{
		dump_entityStrings(asset->name, asset->entityString, asset->numEntityChars);
		dump_triggers(asset->name, &asset->trigger);
		dump_clientTriggers(asset->name, &asset->clientTrigger);
		dump_clientBlendTriggers(asset->name, &asset->clientTriggerBlend);
		dump_spawnList(asset->name, &asset->spawnList);
		dump_splineList(asset->name, &asset->splineList);
	}
}
