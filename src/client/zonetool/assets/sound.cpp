#include <std_include.hpp>
#include "sound.hpp"

namespace snd
{
#define SND_MAX_VOLUME_GROUPS 180
	std::unordered_map<std::string, const char*> mapped_volume_mod_groups =
	{
		{"default", "default"},
		// User interface
		{"hud", "hud"},
		{"frontend_sfx", "frontend_sfx"},
		{"frontend_music", "frontend_music"},
		{"sp_hit_alert_npc", "sp_hit_alert_npc"},
		// Music
		{"music", "music"},
		{"music2", "music2"},
		{"music_big", "music_big"},
		{"music_emitter", "music"}, // not found
		// Ambience
		{"amb_quad", "amb_quad"},
		{"amb_dist", "amb_dist"},
		{"amb_damb", "amb_damb"},
		{"amb_damb_big", "amb_damb"}, // not found
		{"amb_emitter", "amb_emitter"},
		{"amb_emitter_int", "amb_emitter_int"},
		{"amb_emitter_ext", "amb_emitter_ext"},
		// Physics
		{"physics", "physics"},
		// Characters
		{"bodyfall", "bodyfall"},
		{"bodyfall_mech", "bodyfall"}, // not found
		{"foley_plr", "foley_plr"},
		{"foley_plr_special", "foley_plr"}, // not found
		{"mp_foley_plr", "foley_plr"}, // not found
		{"foley_npc", "foley_npc"},
		{"foley_mech", "foley_npc"}, // not found
		{"foleymp_npc", "foley_npc"},
		{"wpn_foley_plr", "wpn_foley_plr"},
		{"wpn_foley_npc", "wpn_foley_npc"},
		{"wpn_foley_mech", "wpn_foley_npc"}, // not found
		{"footstep_plr", "footstep_plr"},
		{"footstep_plr_special", "footstep_plr_special"},
		{"footstep_water_plr", "footstep_water_plr"},
		{"footstep_npc", "footstep_npc"},
		{"footstep_mech", "footstep_npc"}, // not found
		{"mp_footstep_plr", "footstep_plr"},
		{"mp_footstep_npc", "footstep_npc"},
		{"melee_plr", "melee_plr"},
		{"melee_npc", "melee_npc"},
		{"melee_hit", "melee_hit"},
		{"plr_internal", "plr_internal"},
		{"deaths_door", "deaths_door"},
		// Dialogue
		{"chatter_friendly", "voices_ally"},
		{"chatter_enemy", "voices_enemy"},
		{"chatter_atlas", "voices_enemy"}, // not found
		{"vox_friendly", "voices_ally"}, // not found
		{"vox_enemy", "voices_enemy"}, // not found
		{"dialog", "voices_dialog"},
		{"dialog_radio", "voices_dialog_radio"},
		{"dialog_critical", "voices_dialog_critical"},
		{"dialog_amb", "voices_pa"}, // maybe
		// Explosions & Destruction
		{"destruct", "destruct"},
		{"explosion", "explosion"},
		{"explosion_grenade", "explosion_grenade"},
		{"explosion_flashbang", "explosion_flashbang"},
		{"explosion_rocket", "explosion_rocket"},
		{"explosion_car", "explosion_car"},
		{"explosion_critical", "explosion_critical"},
		{"explosion_debris", "explosion_debris"},
		{"impact_critical", "default"}, // not found
		// Bullet Impacts & Whizbys
		{"bullet_impact_geo", "bullet_impact_geo"},
		{"bullet_impact_plr", "bullet_impact_plr"},
		{"bullet_impact_npc", "bullet_impact_npc"},
		{"mp_bullet_impact_geo", "mp_bullet_impact_geo"},
		{"mp_bullet_impact_plr", "mp_bullet_impact_plr"},
		{"mp_bullet_impact_npc", "mp_bullet_impact_npc"},
		{"bullet_whizby", "bullet_whizby"},
		{"mp_bullet_whizby", "mp_bullet_whizby"},
		{"bullet_impact_special", "bullet_impact_plr"}, // not found
		// Vehicles
		{"vehicle_plr", "vehicle_plr"},
		{"vehicle_plr_special", "vehicle_plr_special"},
		{"vehicle_npc", "vehicle_npc"},
		{"vehicle_npc_special", "vehicle_npc"}, // not found
		{"vehicle_avm_plr", "vehicle_avm_plr"},
		{"vehicle_avm_npc", "vehicle_avm_npc"},
		{"wpn_vehicle_plr", "wpn_vehicle_plr"},
		{"wpn_vehicle_npc", "wpn_vehicle_npc"},
		{"vehicle", "vehicle_npc"}, // not found
		{"vehicle_aircraft", "vehicle_aircraft"}, // not found
		{"vehicle_pdrone", "vehicle_aircraft"}, // not found
		{"vehicle_drone_swarm", "vehicle_aircraft"}, // not found
		{"vehicle_amb", "vehicle_npc"}, // not found
		// Weapons
		{"grenadebounce", "grenadebounce"},
		{"mp_grenadebounce", "mp_grenadebounce"},
		{"shellcasings", "shellcasings"},
		{"mp_shellcasings", "shellcasings"},
		{"wpn_plr", "wpn_plr"},
		{"wpn_plr_special", "wpn_plr_special"},
		{"wpn_npc_special", "wpn_npc_special"},
		{"mp_wpn_plr", "wpn_plr"},
		{"wpn_npc", "wpn_npc"},
		{"mp_wpn_npc", "mp_wpn_npc"},
		{"wpn_projectile", "wpn_projectile"},
		{"mp_wpn_projectile", "wpn_projectile"},
		// Weapons Detailed
		{"wpn_plr_shot_special", "wpn_plr_shot_special"},
		{"wpn_plr_shot", "wpn_plr_shot"},
		{"wpn_plr_mech", "wpn_plr_mech"},
		{"wpn_plr_sub", "wpn_plr_sub"},
		{"wpn_plr_tail", "wpn_plr_tail"},
		{"wpn_npc_shot", "wpn_npc_shot"},
		{"wpn_npc_mech", "wpn_npc_mech"},
		{"wpn_npc_sub", "wpn_npc_sub"},
		{"wpn_npc_tail", "wpn_npc_tail"},
		{"wpn_npc_dist", "wpn_npc_dist"},
		// Tactical
		{"tactical_plr", "tactical_plr"},
		{"tactical_npc", "tactical_npc"},
		{"tactical_plr_special", "tactical_plr_special"},
		{"tactical_npc_special", "tactical_npc_special"},
		// Attack Dogs
		{"attack_dog", "attack_dog"},
		// Special use
		{"big_moment", "big_moment"},
		{"scripted1", "scripted1"},
		{"scripted2", "scripted2"},
		{"scripted3", "scripted3"},
		{"scripted4", "scripted4"},
		{"scripted5", "scripted5"},
		{"scripted6", "scripted6"},
		{"scripted7", "scripted7"},
		{"scripted8", "scripted8"},
		{"scripted9", "scripted9"},
		{"scripted10", "scripted10"},
		{"fullvolume", "fullvolume"},
		{"mp_pre_event", "mp_pre_event"},
		{"mp_post_event", "default"}, // not found
		{"mp_splash_notify", "mp_splash_notify"},
		{"mp_hit_marker", "mp_hit_marker"},
		// Level specific and MP perks
		{"mp_perk_quiet", "default"}, // not found
		// OLD - not found
		{"mw3_amb_damb", "amb_damb"},
		{"mw3_amb_emitter", "amb_emitter"},
		{"mw3_amb_quad", "amb_quad"},
		{"mw3_bodyfall", "bodyfall"},
		{"mw3_bullet_impact_geo", "bullet_impact_geo"},
		{"mw3_bullet_impact_npc", "bullet_impact_npc"},
		{"mw3_bullet_impact_plr", "bullet_impact_plr"},
		{"mw3_bullet_whizby", "bullet_whizby"},
		{"mw3_chatter_enemy", "voices_enemy"},
		{"mw3_chatter_friendly", "voices_ally"},
		{"mw3_DEPRECATED_default", "default"},
		{"mw3_DEPRECATED_MAX", "default"},
		{"mw3_DEPRECATED_na", "default"},
		{"mw3_destruct", "destruct"},
		{"mw3_dialog", "voices_dialog"},
		{"mw3_dialog_amb", "voices_pa"},
		{"mw3_dialog_critical", "voices_dialog_critical"},
		{"mw3_dialog_radio", "voices_dialog_radio"},
		{"mw3_explosion", "explosion"},
		{"mw3_explosion_car", "explosion_car"},
		{"mw3_explosion_flashbang", "explosion_flashbang"},
		{"mw3_explosion_grenade", "explosion_grenade"},
		{"mw3_explosion_rocket", "explosion_rocket"},
		{"mw3_foley_npc", "foley_npc"},
		{"mw3_footstep_npc", "footstep_npc"},
		{"mw3_frontend_sfx", "frontend_sfx"},
		{"mw3_fullvolume", "fullvolume"},
		{"mw3_grenadebounce", "grenadebounce"},
		{"mw3_melee_npc", "melee_npc"},
		{"mw3_mp_bullet_impact_geo", "mp_bullet_impact_geo"},
		{"mw3_mp_bullet_impact_npc", "mp_bullet_impact_npc"},
		{"mw3_mp_foley_npc", "mp_foley_npc"},
		{"mw3_mp_footstep_npc", "mp_footstep_npc"},
		{"mw3_mp_wpn_npc", "mp_wpn_npc"},
		{"mw3_music", "music"},
		{"mw3_physics", "physics"},
		{"mw3_scripted1", "scripted1"},
		{"mw3_scripted2", "scripted2"},
		{"mw3_scripted3", "scripted3"},
		{"mw3_scripted4", "scripted4"},
		{"mw3_scripted5", "scripted5"},
		{"mw3_shellcasings", "shellcasings"},
		{"mw3_vehicle", "vehicle_plr"},
		{"mw3_vehicle_plr", "vehicle_plr"},
		{"mw3_vehicle_npc", "vehicle_npc"},
		{"mw3_vox_enemy", "voices_enemy"},
		{"mw3_vox_friendly", "voices_ally"},
		{"mw3_wpn_foley_npc", "wpn_foley_npc"},
		{"mw3_wpn_foley_plr", "wpn_foley_plr"},
		{"mw3_footstep_plr", "footstep_plr"},
		{"mw3_wpn_npc", "wpn_npc"},
		{"mw3_wpn_plr", "wpn_plr"},
		{"mw3_wpn_projectile", "wpn_projectile"},
		{"mw3_wpn_vehicle_npc", "wpn_vehicle_npc"},
		{"mw3_wpn_vehicle_plr", "wpn_vehicle_plr"},
	};

	std::array<const char*, SND_MAX_VOLUME_GROUPS> volume_mod_groups =
	{
		"default",
		// User interface
		"hud",
		"frontend_sfx",
		"frontend_music",
		"sp_hit_alert_npc",
		// Music
		"music",
		"music2",
		"music_big",
		"music_emitter",
		// Ambience
		"amb_quad",
		"amb_dist",
		"amb_damb",
		"amb_damb_big",
		"amb_emitter",
		"amb_emitter_int",
		"amb_emitter_ext",
		// Physics
		"physics",
		// Characters
		"bodyfall",
		"bodyfall_mech",
		"foley_plr",
		"foley_plr_special",
		"mp_foley_plr",
		"foley_npc",
		"foley_mech",
		"foleymp_npc",
		"wpn_foley_plr",
		"wpn_foley_npc",
		"wpn_foley_mech",
		"footstep_plr",
		"footstep_plr_special",
		"footstep_water_plr",
		"footstep_npc",
		"footstep_mech",
		"mp_footstep_plr",
		"mp_footstep_npc",
		"melee_plr",
		"melee_npc",
		"melee_hit",
		"plr_internal",
		"deaths_door",
		// Dialogue
		"chatter_friendly",
		"chatter_enemy",
		"chatter_atlas",
		"vox_friendly",
		"vox_enemy",
		"dialog",
		"dialog_radio",
		"dialog_critical",
		"dialog_amb",
		// Explosions & Destruction
		"destruct",
		"explosion",
		"explosion_grenade",
		"explosion_flashbang",
		"explosion_rocket",
		"explosion_car",
		"explosion_critical",
		"explosion_debris",
		"impact_critical",
		// Bullet Impacts & Whizbys
		"bullet_impact_geo",
		"bullet_impact_plr",
		"bullet_impact_npc",
		"mp_bullet_impact_geo",
		"mp_bullet_impact_plr",
		"mp_bullet_impact_npc",
		"bullet_whizby",
		"mp_bullet_whizby",
		"bullet_impact_special",
		// Vehicles
		"vehicle_plr",
		"vehicle_plr_special",
		"vehicle_npc",
		"vehicle_npc_special",
		"vehicle_avm_plr",
		"vehicle_avm_npc",
		"wpn_vehicle_plr",
		"wpn_vehicle_npc",
		"vehicle",
		"vehicle_aircraft",
		"vehicle_pdrone",
		"vehicle_drone_swarm",
		"vehicle_amb",
		// Weapons
		"grenadebounce",
		"mp_grenadebounce",
		"shellcasings",
		"mp_shellcasings",
		"wpn_plr",
		"wpn_plr_special",
		"wpn_npc_special",
		"mp_wpn_plr",
		"wpn_npc",
		"mp_wpn_npc",
		"wpn_projectile",
		"mp_wpn_projectile",
		// Weapons Detailed
		"wpn_plr_shot_special",
		"wpn_plr_shot",
		"wpn_plr_mech",
		"wpn_plr_sub",
		"wpn_plr_tail",
		"wpn_npc_shot",
		"wpn_npc_mech",
		"wpn_npc_sub",
		"wpn_npc_tail",
		"wpn_npc_dist",
		// Tactical
		"tactical_plr",
		"tactical_npc",
		"tactical_plr_special",
		"tactical_npc_special",
		// Attack Dogs
		"attack_dog",
		// Special use
		"big_moment",
		"scripted1",
		"scripted2",
		"scripted3",
		"scripted4",
		"scripted5",
		"scripted6",
		"scripted7",
		"scripted8",
		"scripted9",
		"scripted10",
		"fullvolume",
		"mp_pre_event",
		"mp_post_event",
		"mp_splash_notify",
		"mp_hit_marker",
		// Level specific and MP perks
		"mp_perk_quiet",
		// OLD
		"mw3_amb_damb",
		"mw3_amb_emitter",
		"mw3_amb_quad",
		"mw3_bodyfall",
		"mw3_bullet_impact_geo",
		"mw3_bullet_impact_npc",
		"mw3_bullet_impact_plr",
		"mw3_bullet_whizby",
		"mw3_chatter_enemy",
		"mw3_chatter_friendly",
		"mw3_DEPRECATED_default",
		"mw3_DEPRECATED_MAX",
		"mw3_DEPRECATED_na",
		"mw3_destruct",
		"mw3_dialog",
		"mw3_dialog_amb",
		"mw3_dialog_critical",
		"mw3_dialog_radio",
		"mw3_explosion",
		"mw3_explosion_car",
		"mw3_explosion_flashbang",
		"mw3_explosion_grenade",
		"mw3_explosion_rocket",
		"mw3_foley_npc",
		"mw3_footstep_npc",
		"mw3_frontend_sfx",
		"mw3_fullvolume",
		"mw3_grenadebounce",
		"mw3_melee_npc",
		"mw3_mp_bullet_impact_geo",
		"mw3_mp_bullet_impact_npc",
		"mw3_mp_foley_npc",
		"mw3_mp_footstep_npc",
		"mw3_mp_wpn_npc",
		"mw3_music",
		"mw3_physics",
		"mw3_scripted1",
		"mw3_scripted2",
		"mw3_scripted3",
		"mw3_scripted4",
		"mw3_scripted5",
		"mw3_shellcasings",
		"mw3_vehicle",
		"mw3_vehicle_plr",
		"mw3_vehicle_npc",
		"mw3_vox_enemy",
		"mw3_vox_friendly",
		"mw3_wpn_foley_npc",
		"mw3_wpn_foley_plr",
		"mw3_footstep_plr",
		"mw3_wpn_npc",
		"mw3_wpn_plr",
		"mw3_wpn_projectile",
		"mw3_wpn_vehicle_npc",
		"mw3_wpn_vehicle_plr",
	};

#define SND_MAX_DSP_BUSES 32
	std::unordered_map<std::string, const char*> mapped_dsp_buses =
	{
		{"default", "default"},
		{"weapons", "weapons_plr"},
		{"weapons_dist", "weapons_npc_dist"},
		{"ambience", "default"}, // not found
		{"emitters", "emitters"},
		{"quads", "default"}, // not found
		{"music", "music"},
		{"foley", "foley"},
		{"explosions", "explosions"},
		{"explosions_critical", "explosions_critical"},
		{"tactical", "tactical"},
		{"tactical_special", "tactical"}, // not found
		{"vehicles", "vehicles_land"},
		{"air_vehicles", "vehicles_air"},
		{"int_vehicles_1", "default"}, // not found
		{"int_vehicles_2", "default"}, // not found
		{"voices", "voices"},
		{"voices_critical", "voices_critical"},
		{"voices_unfiltered", "voices_unfiltered"},
		{"animals", "animals"},
		{"physics", "physics"},
		{"battlechatter", "default"}, // not found
		{"interface", "interface"},
		{"scripted", "scripted1"}, // not found
		{"scripted1", "scripted1"},
		{"scripted2", "scripted2"},
		{"scripted3", "scripted3"},
		{"scripted4", "scripted4"},
		{"scripted5", "scripted4"}, // not found
		{"unoccluded", "unoccluded"},
		{"whizbys", "whizbys"},
		{"notimescale", "notimescale"},
	};

	std::array<const char*, SND_MAX_DSP_BUSES> dsp_buses =
	{
		"default",
		"weapons",
		"weapons_dist",
		"ambience",
		"emitters",
		"quads",
		"music",
		"foley",
		"explosions",
		"explosions_critical",
		"tactical",
		"tactical_special",
		"vehicles",
		"air_vehicles",
		"int_vehicles_1",
		"int_vehicles_2",
		"voices",
		"voices_critical",
		"voices_unfiltered",
		"animals",
		"physics",
		"battlechatter",
		"interface",
		"scripted",
		"scripted1",
		"scripted2",
		"scripted3",
		"scripted4",
		"scripted5",
		"unoccluded",
		"whizbys",
		"notimescale",
	};

	const char* get_vol_nod_name(short index)
	{
		return mapped_volume_mod_groups[volume_mod_groups[index]];
	}

	const char* get_dsp_bus_name(char index)
	{
		return mapped_dsp_buses[dsp_buses[index]];
	}

	short get_vol_mod_index_from_name(const char* name)
	{
		for (short i = 0; i < volume_mod_groups.size(); i++)
		{
			const char* vol_mod = volume_mod_groups[i];
			if (!_stricmp(vol_mod, name))
			{
				return i;
			}
		}
		return -1;
	}

	char get_dsp_bus_index_from_name(const char* name)
	{
		for (char i = 0; i < dsp_buses.size(); i++)
		{
			const char* dsp_bus = dsp_buses[i];
			if (!_stricmp(dsp_bus, name))
			{
				return i;
			}
		}
		return -1;
	}
}
using namespace snd;

namespace zonetool
{
#define SOUND_DUMP_SUBASSET(entry) \
	if (asset->entry) sound[#entry] = asset->entry->name;	\
	else sound[#entry] = nullptr;
#define SOUND_DUMP_STRING(entry) \
	if (asset->entry) sound[#entry] = std::string(asset->entry); \
	else sound[#entry] = nullptr;
#define SOUND_DUMP_CHAR(entry) \
	sound[#entry] = (char)asset->entry;
#define SOUND_DUMP_SHORT(entry) \
	sound[#entry] = (short)asset->entry;
#define SOUND_DUMP_INT(entry) \
	sound[#entry] = (int)asset->entry;
#define SOUND_DUMP_FLOAT(entry) \
	sound[#entry] = (float)asset->entry;

#define SOUND_SUBASSET(entry, __type__) \
	if (!snddata[#entry].is_null()) { \
		asset->entry = mem->Alloc<__type__>(); \
		asset->entry->name = mem->StrDup(snddata[#entry].get<std::string>().data()); }
#define SOUND_STRING(entry) \
	if (!snddata[#entry].is_null()) { \
		asset->entry = mem->StrDup(snddata[#entry].get<std::string>().data()); \
	} else { asset->entry = nullptr; }
#define SOUND_CHAR(entry) \
	asset->entry = snddata[#entry].get<char>()
#define SOUND_SHORT(entry) \
	asset->entry = snddata[#entry].get<short>()
#define SOUND_INT(entry) \
	asset->entry = snddata[#entry].get<int>()
#define SOUND_FLOAT(entry) \
	asset->entry = snddata[#entry].get<float>()

	void ISound::json_parse_snd_alias(snd_alias_t* asset, nlohmann::json snddata, ZoneMemory* mem)
	{

	}

	snd_alias_list_t* ISound::json_parse(const std::string& name, ZoneMemory* mem)
	{
		return nullptr;
	}

	snd_alias_list_t* ISound::parse(const std::string& name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void ISound::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void ISound::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{
	}

	void ISound::load_depending(IZone* zone)
	{

	}

	std::string ISound::name()
	{
		return this->name_;
	}

	std::int32_t ISound::type()
	{
		return ASSET_TYPE_SOUND;
	}

	void ISound::write_soundfile(IZone* zone, ZoneBuffer* buf, SoundFile* data)
	{

	}

	void ISound::write_head(IZone* zone, ZoneBuffer* buf, snd_alias_t* dest)
	{

	}

	void ISound::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void ISound::json_dump_snd_alias(ordered_json& sound, snd_alias_t* asset)
	{
		SOUND_DUMP_STRING(aliasName);
		SOUND_DUMP_STRING(secondaryAliasName);
		SOUND_DUMP_STRING(chainAliasName);
		SOUND_DUMP_STRING(subtitle);
		SOUND_DUMP_STRING(mixerGroup);

		// soundfile shit
		if (asset->soundFile)
		{
			sound["soundfile"]["type"] = asset->soundFile->type;
			sound["soundfile"]["exists"] = asset->soundFile->exists;

			auto insert_loaded = [&]()
			{
				sound["soundfile"]["name"] = asset->soundFile->u.loadSnd->name ? asset->soundFile->u.loadSnd->name : "";
			};

			auto insert_streamed = [&]()
			{
				sound["soundfile"]["totalMsec"] = asset->soundFile->u.streamSnd.totalMsec;
				sound["soundfile"]["isLocalized"] = asset->soundFile->u.streamSnd.filename.isLocalized;
				sound["soundfile"]["isStreamed"] = asset->soundFile->u.streamSnd.filename.isStreamed;
				sound["soundfile"]["fileIndex"] = asset->soundFile->u.streamSnd.filename.fileIndex;

				sound["soundfile"]["packed"]["offset"] = 0;
				sound["soundfile"]["packed"]["length"] = 0;
				sound["soundfile"]["raw"]["dir"] = "";
				sound["soundfile"]["raw"]["name"] = "";

				if (asset->soundFile->u.streamSnd.filename.fileIndex)
				{
					sound["soundfile"]["packed"]["offset"] = asset->soundFile->u.streamSnd.filename.info.packed.offset;
					sound["soundfile"]["packed"]["length"] = asset->soundFile->u.streamSnd.filename.info.packed.length;
				}
				else
				{
					sound["soundfile"]["raw"]["dir"] = asset->soundFile->u.streamSnd.filename.info.raw.dir
						? asset->soundFile->u.streamSnd.filename.info.raw.dir
						: "";
					sound["soundfile"]["raw"]["name"] = asset->soundFile->u.streamSnd.filename.info.raw.name
						? asset->soundFile->u.streamSnd.filename.info.raw.name
						: "";
				}
			};

			if (asset->soundFile->type == SAT_LOADED)
			{
				insert_loaded();
			}
			else if (asset->soundFile->type == SAT_STREAMED)
			{
				insert_streamed();
			}
			else if (asset->soundFile->type == SAT_PRIMED)
			{
				insert_loaded();
				insert_streamed();
				//ZONETOOL_FATAL("SAT_PRIMED dumping is not supported yet.");
			}
		}

		SOUND_DUMP_INT(flags);
		SOUND_DUMP_CHAR(priority);
		sound["dspBus"] = get_dsp_bus_name(asset->dspBusIndex); //SOUND_DUMP_CHAR(dspBusIndex);
		sound["volMod"] = get_vol_nod_name(asset->volModIndex); //SOUND_DUMP_SHORT(volModIndex);
		SOUND_DUMP_FLOAT(volMin);
		SOUND_DUMP_FLOAT(volMax);
		SOUND_DUMP_FLOAT(pitchMin);
		SOUND_DUMP_FLOAT(pitchMax);
		SOUND_DUMP_FLOAT(distMin);
		SOUND_DUMP_FLOAT(distMax);
		SOUND_DUMP_FLOAT(velocityMin);
		SOUND_DUMP_FLOAT(probability);
		SOUND_DUMP_INT(sequence);
		SOUND_DUMP_INT(startDelay);

		SOUND_DUMP_CHAR(masterPriority);
		SOUND_DUMP_FLOAT(masterPercentage);
		SOUND_DUMP_FLOAT(slavePercentage);
		SOUND_DUMP_CHAR(playbackPercentage);

		SOUND_DUMP_FLOAT(lfePercentage);
		SOUND_DUMP_FLOAT(centerPercentage);

		SOUND_DUMP_SHORT(poly);
		SOUND_DUMP_SHORT(polyGlobal);
		SOUND_DUMP_CHAR(polyEntityType);
		SOUND_DUMP_CHAR(polyGlobalType);

		SOUND_DUMP_FLOAT(envelopMin);
		SOUND_DUMP_FLOAT(envelopMax);
		SOUND_DUMP_FLOAT(envelopPercentage);

		SOUND_DUMP_FLOAT(reverbWetMixOverride);
		SOUND_DUMP_FLOAT(reverbMultiplier);

		SOUND_DUMP_FLOAT(smartPanDistance2d);
		SOUND_DUMP_FLOAT(smartPanDistance3d);
		SOUND_DUMP_FLOAT(smartPanAttenuation3d);

		SOUND_DUMP_SHORT(stereo3dAngle);
		SOUND_DUMP_FLOAT(stereo3dStart);
		SOUND_DUMP_FLOAT(stereo3dEnd);

		SOUND_DUMP_SUBASSET(sndContext);
		SOUND_DUMP_SUBASSET(sndCurve);
		SOUND_DUMP_SUBASSET(lpfCurve);
		sound["hpfCurve"] = "$defaultHpf";

		SOUND_DUMP_SUBASSET(reverbSendCurve);

		sound["speakerMap"] = nullptr;
		if (asset->speakerMap)
		{
			json speakerMap;
			speakerMap["name"] = asset->speakerMap->name;
			speakerMap["isDefault"] = asset->speakerMap->isDefault;

			json channelMaps;
			for (char x = 0; x < 2; x++)
			{
				for (char y = 0; y < 2; y++)
				{
					json channelMap;
					channelMap["speakerCount"] = asset->speakerMap->channelMaps[x][y].speakerCount;

					json speakers;
					for (int speaker = 0; speaker < asset->speakerMap->channelMaps[x][y].speakerCount; speaker++)
					{
						json jspeaker;

						jspeaker["speaker"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].speaker;
						jspeaker["numLevels"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].numLevels;
						jspeaker["levels0"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].levels[0];
						jspeaker["levels1"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].levels[1];

						speakers[speaker] = jspeaker;
					}

					channelMap["speakers"] = speakers;

					channelMaps[(x & 0x01) << 1 | y & 0x01] = channelMap;
				}
			}

			speakerMap["channelMaps"] = channelMaps;

			sound["speakerMap"] = speakerMap;
		}

		SOUND_DUMP_CHAR(allowDoppler);
		SOUND_DUMP_SUBASSET(dopplerPreset);

		// dump all unknown things too
		sound["unknown"]["pad"][0] = json::binary(std::vector<std::uint8_t>(asset->__pad0, asset->__pad0 + sizeof(asset->__pad0)));
		sound["unknown"]["u1"] = asset->u1;
		sound["unknown"]["u2"] = asset->u2;
	}

	void ISound::json_dump(snd_alias_list_t* asset)
	{
		const auto path = "sounds\\"s + asset->name + ".json"s;

		ordered_json sound;
		ordered_json aliases;
		ordered_json unknownArray;

		SOUND_DUMP_STRING(aliasName);
		SOUND_DUMP_CHAR(count);

		for (unsigned char i = 0; i < asset->count; i++)
		{
			ordered_json alias;
			json_dump_snd_alias(alias, &asset->head[i]);
			sound["head"][i] = alias;
		}

		for (unsigned char i = 0; i < asset->contextListCount; i++)
		{
			sound["contextList"][i] = asset->contextList[i].unk;
		}

		std::string assetstr = sound.dump(4);

		auto file = filesystem::file(path);
		file.open();
		file.write(assetstr.data(), assetstr.size(), 1);
		file.close();
	}

	void ISound::dump(snd_alias_list_t* asset)
	{
		json_dump(asset);
	}
}