#pragma once
#include "../zonetool.hpp"

namespace zonetool
{
	class ISoundContext : public IAsset
	{
	private:
		std::string name_;
		SndContext* asset_ = nullptr;

	public:
		SndContext* parse(const std::string& name, ZoneMemory* mem);

		void init(const std::string& name, ZoneMemory* mem) override;
		void prepare(ZoneBuffer* buf, ZoneMemory* mem) override;
		void load_depending(IZone* zone) override;

		void* pointer() override { return asset_; }
		std::string name() override;
		std::int32_t type() override;
		void write(IZone* zone, ZoneBuffer* buffer) override;

		static void dump(SndContext* asset);
	};
}