#include <std_include.hpp>
#include "techset.hpp"

#include "vertexshader.hpp"
#include "vertexdecl.hpp"
#include "hullshader.hpp"
#include "domainshader.hpp"
#include "pixelshader.hpp"

namespace zonetool
{
	namespace
	{
		h2::MaterialTechniqueSet* convert_to_h2(MaterialTechniqueSet* h1_asset, utils::memory::allocator& allocator)
		{
			const auto asset = allocator.allocate<h2::MaterialTechniqueSet>();
			std::memcpy(asset, h1_asset, sizeof(h1_asset));
			
			std::vector<int> debug_tech_indexes = {59, 119, 179, 239};
			auto current_debug_tech = 0;
			auto h1_tecniques_index = 0;
			for (auto i = 0u; i < TECHNIQUES_COUNT; i++)
			{
				asset->techniques[i] = reinterpret_cast<h2::MaterialTechnique*>(h1_asset->techniques[h1_tecniques_index]);
				if (current_debug_tech < debug_tech_indexes.size() && h1_tecniques_index == debug_tech_indexes[current_debug_tech])
				{
					asset->techniques[i + 1] = asset->techniques[i];
					asset->techniques[i + 2] = asset->techniques[i];
					asset->techniques[i + 3] = asset->techniques[i];
					++current_debug_tech;
					i += 3;
				}

				++h1_tecniques_index;
			}

			return asset;
		}

		unsigned char* convert_to_h2(unsigned char* h1_array, utils::memory::allocator& allocator)
		{
			const auto array = allocator.allocate_array<unsigned char>(TECHNIQUES_COUNT);

			std::vector<int> debug_tech_indexes = {59, 119, 179, 239};
			auto current_debug_tech = 0;
			auto h1_tecniques_index = 0;
			for (auto i = 0u; i < TECHNIQUES_COUNT; i++)
			{
				array[i] = h1_array[h1_tecniques_index];
				if (current_debug_tech < debug_tech_indexes.size() && h1_tecniques_index == debug_tech_indexes[current_debug_tech])
				{
					array[i + 1] = array[i];
					array[i + 2] = array[i];
					array[i + 3] = array[i];
					++current_debug_tech;
					i += 3;
				}

				++h1_tecniques_index;
			}

			return array;
		}
	}

	std::unordered_map<std::string, std::uintptr_t> ITechset::vertexdecl_pointers;

	std::uintptr_t ITechset::get_vertexdecl_pointer(std::string vertexdecl)
	{
		if (vertexdecl_pointers.find(vertexdecl) != vertexdecl_pointers.end())
		{
			return vertexdecl_pointers[vertexdecl];
		}
		return 0;
	}

	void ITechset::add_vertexdecl_pointer(std::string name, std::uintptr_t pointer)
	{

	}

	MaterialTechnique* parse_technique(const std::string& name, ZoneMemory* mem, std::uint32_t index)
	{
		return nullptr;
	}

	MaterialTechniqueSet* ITechset::parse(const std::string& name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void ITechset::parse_constant_buffer_indexes(const std::string& techset, unsigned char* indexes, ZoneMemory* mem)
	{

	}

	void ITechset::parse_constant_buffer_def_array(const std::string& techset, MaterialConstantBufferDef** def_ptr, unsigned char* count, ZoneMemory* mem)
	{

	}

	void ITechset::parse_stateinfo(const std::string& techset, Material* mat, ZoneMemory* mem)
	{

	}

	void ITechset::parse_statebits(const std::string& techset, unsigned char* statebits, ZoneMemory* mem)
	{

	}

	void ITechset::parse_statebitsmap(const std::string& techset, GfxStateBits** map, unsigned char* count,
		std::vector<std::array<std::uint64_t, 10>>* dssb,
		std::vector<std::array<std::uint32_t, 3>>* bsb,
		ZoneMemory* mem)
	{

	}

	void ITechset::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void ITechset::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{

	}

	void ITechset::load_depending(IZone* zone)
	{

	}

	std::string ITechset::name()
	{
		return this->name_;
	}

	std::int32_t ITechset::type()
	{
		return ASSET_TYPE_TECHNIQUE_SET;
	}

	void ITechset::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void ITechset::dump_constant_buffer_indexes(const std::string& techset, unsigned char* h1_cbi)
	{
		utils::memory::allocator allocator;
		const auto path = "techsets\\constantbuffer\\"s + techset + ".cbi";
		auto file = filesystem::file(path);
		file.open("wb");
		auto fp = file.get_fp();
		const auto cbi = convert_to_h2(h1_cbi, allocator);

		if (fp)
		{
			fwrite(cbi, TECHNIQUES_COUNT, 1, fp);
			file.close();
		}
	}

	void ITechset::dump_constant_buffer_def_array(const std::string& techset, unsigned char count, MaterialConstantBufferDef* def)
	{
		const auto path = "techsets\\constantbuffer\\"s + techset + ".cbt";
		assetmanager::dumper dump;
		if (!dump.open(path))
		{
			return;
		}

		dump.dump_int(count);
		dump.dump_array(def, count);
		for (int i = 0; i < count; i++)
		{
			if (def[i].vsData)
			{
				dump.dump_array(def[i].vsData, def[i].vsDataSize);
			}
			if (def[i].hsData)
			{
				dump.dump_array(def[i].hsData, def[i].hsDataSize);
			}
			if (def[i].dsData)
			{
				dump.dump_array(def[i].dsData, def[i].dsDataSize);
			}
			if (def[i].psData)
			{
				dump.dump_array(def[i].psData, def[i].psDataSize);
			}
			if (def[i].vsOffsetData)
			{
				dump.dump_array(def[i].vsOffsetData, def[i].vsOffsetDataSize);
			}
			if (def[i].hsOffsetData)
			{
				dump.dump_array(def[i].hsOffsetData, def[i].hsOffsetDataSize);
			}
			if (def[i].dsOffsetData)
			{
				dump.dump_array(def[i].dsOffsetData, def[i].dsOffsetDataSize);
			}
			if (def[i].psOffsetData)
			{
				dump.dump_array(def[i].psOffsetData, def[i].psOffsetDataSize);
			}
		}

		dump.close();
	}

	void ITechset::dump_stateinfo(const std::string& techset, Material* mat)
	{
		const auto path = "techsets\\state\\"s + techset + ".stateinfo";

		ordered_json json_data = {};

		json_data["stateFlags"] = mat->stateFlags;

		auto file = filesystem::file(path);
		file.open("wb");
		auto fp = file.get_fp();
		if (fp)
		{
			const auto json_dump = json_data.dump(4);
			file.write(json_dump.data(), json_dump.size(), 1);
			file.close();
		}
	}

	void ITechset::dump_statebits(const std::string& techset, unsigned char* h1_statebits)
	{
		utils::memory::allocator allocator;
		const auto path = "techsets\\state\\"s + techset + ".statebits";
		auto file = filesystem::file(path);
		file.open("wb");
		auto fp = file.get_fp();
		const auto statebits = convert_to_h2(h1_statebits, allocator);

		if (fp)
		{
			fwrite(statebits, TECHNIQUES_COUNT, 1, fp);
			file.close();
		}
	}

	void ITechset::dump_statebits_map(const std::string& techset, GfxStateBits* map, unsigned char count)
	{
		const auto path = "techsets\\state\\"s + techset + ".statebitsmap";

		ordered_json json_data = {};
		for (unsigned char i = 0; i < count; i++)
		{
			XGfxGlobals* varXGfxGlobals = GetXGfxGlobalsForZone(map[i].zone);
			ordered_json entry;
			entry["loadBits"][0] = map[i].loadBits[0];
			entry["loadBits"][1] = map[i].loadBits[1];
			entry["loadBits"][2] = map[i].loadBits[2];
			entry["loadBits"][3] = map[i].loadBits[3];
			entry["loadBits"][4] = map[i].loadBits[4];
			entry["loadBits"][5] = map[i].loadBits[5];
			for (int j = 0; j < 11; j++)
			{
				entry["depthStencilStateBits"][j] = varXGfxGlobals ? varXGfxGlobals->depthStencilStateBits[map[i].depthStencilState[j]] : 0;
			}
			for (int j = 0; j < 3; j++)
			{
				entry["blendStateBits"][j] = varXGfxGlobals ? varXGfxGlobals->blendStateBits[map[i].blendState][j] : 0;
			}
			entry["rasterizerState"] = map[i].rasterizerState;
			json_data[i] = entry;
		}

		auto file = filesystem::file(path);
		file.open("wb");
		auto fp = file.get_fp();
		if (fp)
		{
			const auto json_dump = json_data.dump(4);
			file.write(json_dump.data(), json_dump.size(), 1);
			file.close();
		}
	}

	void ITechset::dump_technique(MaterialTechnique* asset)
	{
		const auto path = "techsets\\"s + asset->hdr.name + ".technique";

		assetmanager::dumper dumper;
		if (!dumper.open(path))
		{
			return;
		}

		dumper.dump_single(&asset->hdr);
		dumper.dump_array(asset->passArray, asset->hdr.passCount);

		dumper.dump_string(asset->hdr.name);
		for (unsigned short i = 0; i < asset->hdr.passCount; i++)
		{
			if (asset->passArray[i].vertexShader)
			{
				dumper.dump_asset(asset->passArray[i].vertexShader);
				//IVertexShader::dump(asset->passArray[i].vertexShader);
			}

			if (asset->passArray[i].vertexDecl)
			{
				dumper.dump_asset(asset->passArray[i].vertexDecl);
				IVertexDecl::dump(reinterpret_cast<MaterialVertexDeclaration*>(asset->passArray[i].vertexDecl));
			}

			if (asset->passArray[i].hullShader)
			{
				dumper.dump_asset(asset->passArray[i].hullShader);
				//IHullShader::dump(asset->passArray[i].hullShader);
			}

			if (asset->passArray[i].domainShader)
			{
				dumper.dump_asset(asset->passArray[i].domainShader);
				//IDomainShader::dump(asset->passArray[i].domainShader);
			}

			if (asset->passArray[i].pixelShader)
			{
				dumper.dump_asset(asset->passArray[i].pixelShader);
				//IPixelShader::dump(asset->passArray[i].pixelShader);
			}

			if (asset->passArray[i].args)
			{
				dumper.dump_array(asset->passArray[i].args, asset->passArray[i].perObjArgCount + asset->passArray[i].perPrimArgCount + asset->passArray[i].stableArgCount);
				for (auto arg = 0; arg < asset->passArray[i].perObjArgCount + asset->passArray[i].perPrimArgCount + asset->passArray[i].stableArgCount; arg++)
				{
					if (asset->passArray[i].args[arg].type == 4)
					{
						dumper.dump_array(const_cast<float*>(asset->passArray[i].args[arg].u.literalConst), 4);
					}
				}
			}
		}

		dumper.close();
	}

	void yeet(h2::MaterialTechniqueSet* asset)
	{
		const auto path = "techsets\\"s + asset->name + ".techset.txt";

		filesystem::file file(path);
		file.open("wb");
		auto fp = file.get_fp();

		for (auto i = 0u; i < TECHNIQUES_COUNT; i++)
		{
			if (asset->techniques[i])
			{
				fprintf(fp, "%i: %s\n", i, asset->techniques[i]->hdr.name);
			}
			else
			{
				fprintf(fp, "%i: nullptr\n", i);
			}
		}

		file.close();
	}

	void ITechset::dump(MaterialTechniqueSet* h1_asset)
	{
		utils::memory::allocator allocator;
		const auto asset = convert_to_h2(h1_asset, allocator);
		const auto path = "techsets\\"s + asset->name + ".techset";

		yeet(asset);

		assetmanager::dumper dumper;
		if (!dumper.open(path))
		{
			return;
		}

		dumper.dump_single(asset);
		dumper.dump_string(asset->name);

		for (auto i = 0u; i < TECHNIQUES_COUNT; i++)
		{
			if (asset->techniques[i])
			{
				dumper.dump_string(asset->techniques[i]->hdr.name);
				ITechset::dump_technique(reinterpret_cast<MaterialTechnique*>(asset->techniques[i]));
			}
		}

		dumper.close();
	}
}