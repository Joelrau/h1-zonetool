#pragma once
#include "../zonetool.hpp"

namespace zonetool
{
	class ILoadedSound : public IAsset
	{
	private:
		std::string name_;
		LoadedSound* asset_ = nullptr;

	public:
		LoadedSound* parse_flac(const std::string& name, ZoneMemory* mem);
		LoadedSound* parse_wav(const std::string& name, ZoneMemory* mem);
		LoadedSound* parse(const std::string& name, ZoneMemory* mem);

		void init(const std::string& name, ZoneMemory* mem) override;
		void prepare(ZoneBuffer* buf, ZoneMemory* mem) override;
		void load_depending(IZone* zone) override;

		void* pointer() override { return asset_; }
		std::string name() override;
		std::int32_t type() override;
		void write(IZone* zone, ZoneBuffer* buffer) override;

		static void dump_data(LoadedSound* asset, char* data, size_t size, const std::string& extension = "");
		static void dump(LoadedSound* asset);
	};
}