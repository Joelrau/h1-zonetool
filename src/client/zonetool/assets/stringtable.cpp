#include <std_include.hpp>
#include "stringtable.hpp"

#include "zonetool/utils/csv.hpp"

namespace zonetool
{
	int StringTable_Hash(const std::string& string)
	{
		int hash = 0;
		const char* data = string.data();

		while (*data != 0)
		{
			hash = tolower(*data) + (31 * hash);
			data++;
		}

		return hash;
	}

	StringTable* StringTable_Parse(std::string name, ZoneMemory* mem)
	{
		auto table = csv::parser(filesystem::get_file_path(name) + name);
		auto stringtable = mem->Alloc<StringTable>();

		stringtable->name = mem->StrDup(name);
		stringtable->rowCount = static_cast<int>(table.get_num_rows());
		stringtable->columnCount = static_cast<int>(table.get_max_columns());
		stringtable->values = mem->Alloc<StringTableCell>(stringtable->rowCount * stringtable->columnCount);

		auto rows = table.get_rows();
		for (int row = 0; row < table.get_num_rows(); row++)
		{
			for (int col = 0; col < rows[row]->num_fields; col++)
			{
				int entry = (row * stringtable->columnCount) + col;
				stringtable->values[entry].string = mem->StrDup(rows[row]->fields[col]);
				stringtable->values[entry].hash = StringTable_Hash(stringtable->values[entry].string);
			}
		}

		return stringtable;
	}

	void IStringTable::init(const std::string& name, ZoneMemory* mem)
	{
		this->name_ = name;

		if (this->referenced())
		{
			this->asset_ = mem->Alloc<typename std::remove_reference<decltype(*this->asset_)>::type>();
			this->asset_->name = mem->StrDup(name);
			return;
		}

		this->asset_ = DB_FindXAssetHeader_Safe(static_cast<XAssetType>(this->type()), this->name().data()).stringTable;
		if (filesystem::file(name).exists())
		{
			ZONETOOL_INFO("Parsing stringtable \"%s\"...", name.data());
			this->asset_ = StringTable_Parse(name, mem);
		}
	}

	void IStringTable::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{
	}

	void IStringTable::load_depending(IZone* zone)
	{
	}

	std::string IStringTable::name()
	{
		return this->name_;
	}

	std::int32_t IStringTable::type()
	{
		return ASSET_TYPE_STRINGTABLE;
	}

	void IStringTable::write(IZone* zone, ZoneBuffer* buf)
	{
		auto data = this->asset_;
		auto dest = buf->write(data);

		buf->push_stream(3);

		dest->name = buf->write_str(this->name());

		if (data->values)
		{
			buf->align(3);
			const auto destStrings = buf->write(data->values, data->columnCount * data->rowCount);

			if (data->columnCount * data->rowCount > 0)
			{
				for (int i = 0; i < data->columnCount * data->rowCount; i++)
				{
					if (data->values[i].string)
					{
						destStrings[i].string = buf->write_str(data->values[i].string);
					}
				}
			}

			ZoneBuffer::clear_pointer(&dest->values);
		}

		buf->pop_stream();
	}

	void IStringTable::dump(StringTable* asset)
	{
		auto file = filesystem::file(asset->name);
		file.open("wb");

		if (!file.get_fp())
		{
			return;
		}

		for (auto row = 0; row < asset->rowCount; row++)
		{
			for (auto column = 0; column < asset->columnCount; column++)
			{
				const auto index = (row * asset->columnCount) + column;
				const auto string_value = asset->values[index].string;
				const auto last_char = (column == asset->columnCount - 1) ? "\r\n" : ",";

				if (string_value == nullptr)
				{
					std::fprintf(file.get_fp(), last_char);
				}
				else
				{
					std::string str = string_value;
					if (str.contains(',') || str.contains('\n') || str.contains('\t'))
					{
						str.insert(str.begin(), '"');
						str.insert(str.end(), '"');
						str = std::regex_replace(str, std::regex("\n"), "\\n");
						str = std::regex_replace(str, std::regex("\t"), "\\t");
					}
					std::fprintf(file.get_fp(), "%s%s", str.data(), last_char);
				}
			}
		}
	}
}