#include <std_include.hpp>
#include "material.hpp"

#include "techset.hpp"

#define MATERIAL_DUMP_STRING(entry) \
	matdata[#entry] = std::string(asset->entry);

#define MATERIAL_DUMP(entry) \
	matdata[#entry] = asset->entry;

#define MATERIAL_DUMP_INFO(entry) \
	matdata[#entry] = asset->info.entry;

#define MATERIAL_DUMP_CONST_ARRAY(entry,size) \
	ordered_json carr##entry; \
	for (int i = 0; i < size; i++) \
	{ \
		ordered_json cent##entry; \
		std::string name = asset->constantTable[i].name; \
		name.resize(12); \
		cent##entry["name"] = name.data(); \
		cent##entry["nameHash"] = asset->entry[i].nameHash; \
		nlohmann::json centliteral##entry; \
		centliteral##entry[0] = asset->entry[i].literal[0]; \
		centliteral##entry[1] = asset->entry[i].literal[1]; \
		centliteral##entry[2] = asset->entry[i].literal[2]; \
		centliteral##entry[3] = asset->entry[i].literal[3]; \
		cent##entry["literal"] = centliteral##entry; \
		carr##entry[i] = cent##entry; \
	} \
	matdata[#entry] = carr##entry;

namespace zonetool
{
	namespace
	{
		std::string clean_name(const std::string& name)
		{
			auto new_name = name;

			for (auto i = 0u; i < name.size(); i++)
			{
				switch (new_name[i])
				{
				case '*':
					new_name[i] = '_';
					break;
				}
			}

			return new_name;
		}

		std::unordered_map<std::string, std::string> techset_replacements =
		{
			{"m_l_sm_lmpb_dc1dn1ds1dg1dna1_r0c0n0sd0om0_nfwpf", "m_l_sm_lmpb_r0c0n0sd0om0_nfwpf"},
			{"m_l_sm_lmpb_dc1dn1ds1dg1dna1_r0c0n0sd0sr0om0_nfwpf", "m_l_sm_lmpb_r0c0n0sd0sr0om0_nfwpf"},
		};

		std::string get_replaced_techset(const std::string& name)
		{
			if (techset_replacements.find(name) != techset_replacements.end())
			{
				return techset_replacements[name];
			}

			return name;
		}
	}

	MaterialTextureDef* IMaterial::prase_texture_table(json& matdata, ZoneMemory* mem)
	{
		return nullptr;
	}

	Material* IMaterial::parse(std::string name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void IMaterial::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IMaterial::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{

	}

	void IMaterial::load_depending(IZone* zone)
	{

	}

	std::string IMaterial::name()
	{
		return this->name_;
	}

	std::int32_t IMaterial::type()
	{
		return ASSET_TYPE_MATERIAL;
	}

	void IMaterial::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void IMaterial::dump(Material* asset)
	{
		// TODO: maybe add subMaterials?

		if (asset)
		{
			auto c_name = clean_name(asset->name);

			const auto path = "materials\\"s + c_name + ".json"s;
			auto file = filesystem::file(path);
			file.open("wb");

			if (asset && asset->techniqueSet)
			{
				const auto name = get_replaced_techset(asset->techniqueSet->name);

				ITechset::dump_stateinfo(name, asset);
				ITechset::dump_statebits(name, asset->stateBitsEntry);
				ITechset::dump_statebits_map(name, asset->stateBitsTable, asset->stateBitsCount);

				ITechset::dump_constant_buffer_indexes(name, asset->constantBufferIndex);
				ITechset::dump_constant_buffer_def_array(name, asset->constantBufferCount, asset->constantBufferTable);
			}

			ordered_json matdata;

			MATERIAL_DUMP_STRING(name);

			if (asset->techniqueSet)
			{
				const auto name = get_replaced_techset(asset->techniqueSet->name);
				matdata["techniqueSet->name"] = name;
			}

			MATERIAL_DUMP_INFO(gameFlags);
			MATERIAL_DUMP_INFO(sortKey);
			MATERIAL_DUMP_INFO(renderFlags);

			MATERIAL_DUMP_INFO(textureAtlasRowCount);
			MATERIAL_DUMP_INFO(textureAtlasColumnCount);
			MATERIAL_DUMP_INFO(textureAtlasFrameBlend);
			MATERIAL_DUMP_INFO(textureAtlasAsArray);

			MATERIAL_DUMP_INFO(surfaceTypeBits);
			//MATERIAL_DUMP_INFO(hashIndex);

			//MATERIAL_DUMP(stateFlags);
			MATERIAL_DUMP(cameraRegion);
			MATERIAL_DUMP(materialType);
			MATERIAL_DUMP(assetFlags);

			MATERIAL_DUMP_CONST_ARRAY(constantTable, asset->constantCount);

			ordered_json material_images;
			for (auto i = 0; i < asset->textureCount; i++)
			{
				ordered_json image;
				if (asset->textureTable[i].semantic == 11)
				{
					// Haven't tested water yet.
					MessageBoxA(nullptr, asset->textureTable[i].u.water->image->name, "water", 0);

					water_t* waterData = asset->textureTable[i].u.water;

					image["image"] = waterData->image->name;

					ordered_json waterdata;
					waterdata["floatTime"] = waterData->writable.floatTime;
					waterdata["M"] = waterData->M;
					waterdata["N"] = waterData->N;
					waterdata["Lx"] = waterData->Lx;
					waterdata["Lz"] = waterData->Lz;
					waterdata["gravity"] = waterData->gravity;
					waterdata["windvel"] = waterData->windvel;
					waterdata["winddir"][0] = waterData->winddir[0];
					waterdata["winddir"][1] = waterData->winddir[1];
					waterdata["amplitude"] = waterData->amplitude;

					ordered_json waterComplexData;
					ordered_json wTerm;

					for (int a = 0; a < waterData->M * waterData->N; a++)
					{
						ordered_json complexdata;
						ordered_json curWTerm;

						complexdata["real"] = waterData->H0X[a];
						complexdata["imag"] = waterData->H0Y[a];

						curWTerm[a] = waterData->wTerm[a];

						waterComplexData[a] = complexdata;
					}

					waterdata["complex"] = waterComplexData;
					waterdata["wTerm"] = wTerm;

					image["waterinfo"] = waterdata;
				}
				else
				{
					if (asset->textureTable[i].u.image->name)
					{
						image["image"] = asset->textureTable[i].u.image->name;
					}
					else
					{
						image["image"] = "";
					}
				}

				image["semantic"] = asset->textureTable[i].semantic;
				image["samplerState"] = asset->textureTable[i].samplerState;
				image["lastCharacter"] = asset->textureTable[i].nameEnd;
				image["firstCharacter"] = asset->textureTable[i].nameStart;
				image["typeHash"] = asset->textureTable[i].nameHash;

				// add image data to material
				material_images.push_back(image);
			}
			matdata["textureTable"] = material_images;

			auto str = matdata.dump(4);

			matdata.clear();

			file.write(str);

			file.close();
		}
	}
}