#include "std_include.hpp"
#include "clut.hpp"

namespace zonetool
{
	void IClut::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IClut::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{
	}

	void IClut::load_depending(IZone* zone)
	{
	}

	std::string IClut::name()
	{
		return this->name_;
	}

	std::int32_t IClut::type()
	{
		return ASSET_TYPE_CLUT;
	}

	void IClut::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void IClut::dump(Clut* asset)
	{
		const auto path = "clut\\"s + asset->name + ".clut";

		assetmanager::dumper dumper;
		if (!dumper.open(path))
		{
			return;
		}

		dumper.dump_single(asset);
		dumper.dump_string(asset->name);
		dumper.dump_array(asset->unk, 4 * asset->count0 * asset->count1 * asset->count2);

		dumper.close();
	}
}