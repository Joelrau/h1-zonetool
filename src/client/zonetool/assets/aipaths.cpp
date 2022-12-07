#include "std_include.hpp"
#include "aipaths.hpp"

namespace zonetool
{
	void IAIPaths::add_script_string(scr_string_t* ptr, std::string str)
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

	std::string IAIPaths::get_script_string(scr_string_t* ptr)
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

	PathData* IAIPaths::parse(const std::string& name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void IAIPaths::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IAIPaths::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{

	}

	void IAIPaths::load_depending(IZone* zone)
	{
	}

	std::string IAIPaths::name()
	{
		return this->name_;
	}

	std::int32_t IAIPaths::type()
	{
		return ASSET_TYPE_AIPATHS;
	}

	void IAIPaths::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void dump_node_tree_children(assetmanager::dumper& dumper, pathnode_tree_t* node_tree);

	void dump_node_tree(assetmanager::dumper& dumper, pathnode_tree_t* node_tree)
	{
		if (node_tree->axis < 0)
		{
			dumper.dump_array(node_tree->u.s.nodes, node_tree->u.s.nodeCount);
		}
		else
		{
			for (auto i = 0; i < 2; i++)
			{
				dump_node_tree_children(dumper, node_tree->u.child[i]);
			}
		}
	}

	void dump_node_tree_children(assetmanager::dumper& dumper, pathnode_tree_t* node_tree)
	{
		dumper.dump_single(node_tree);

		if (node_tree->axis < 0)
		{
			dumper.dump_array(node_tree->u.s.nodes, node_tree->u.s.nodeCount);
		}
		else
		{
			for (auto i = 0; i < 2; i++)
			{
				dump_node_tree_children(dumper, node_tree->u.child[i]);
			}
		}
	}

	void IAIPaths::dump(PathData* asset)
	{
		const auto path = asset->name + ".aipaths"s;

		assetmanager::dumper dumper;
		if (!dumper.open(path))
		{
			return;
		}

		dumper.dump_single(asset);
		dumper.dump_string(asset->name);

		dumper.dump_array(asset->nodes, asset->nodeCount);
		for (auto i = 0u; i < asset->nodeCount; i++)
		{
			dumper.dump_string(SL_ConvertToString(asset->nodes[i].constant.targetname));
			dumper.dump_string(SL_ConvertToString(asset->nodes[i].constant.script_linkName));
			dumper.dump_string(SL_ConvertToString(asset->nodes[i].constant.script_noteworthy));
			dumper.dump_string(SL_ConvertToString(asset->nodes[i].constant.target));
			dumper.dump_string(SL_ConvertToString(asset->nodes[i].constant.animscript));

			if (!asset->parentIndexResolved)
			{
				dumper.dump_string(SL_ConvertToString(asset->nodes[i].constant.parent.name));
			}

			dumper.dump_array(asset->nodes[i].constant.Links, asset->nodes[i].constant.totalLinkCount);
			dumper.dump_string(SL_ConvertToString(asset->nodes[i].constant.unk));
		}

		dumper.dump_array(asset->pathVis, asset->visBytes);

		dumper.dump_array(asset->nodeTree, asset->nodeTreeCount);
		for (auto i = 0; i < asset->nodeTreeCount; i++)
		{
			dump_node_tree(dumper, &asset->nodeTree[i]);
		}

		dumper.dump_array(asset->dynamicNodeGroups, asset->dynamicNodeGroupCount);
		for (auto i = 0; i < asset->dynamicNodeGroupCount; i++)
		{
			dumper.dump_array(asset->dynamicNodeGroups[i].nodeTree, 
				asset->dynamicNodeGroups[i].nodeTreeCount);
			for (auto o = 0; o < asset->dynamicNodeGroups[i].nodeTreeCount; o++)
			{
				dump_node_tree(dumper, &asset->dynamicNodeGroups[i].nodeTree[o]);
			}
		}

		dumper.dump_array(asset->pathExposure, asset->exposureBytes);
		dumper.dump_array(asset->pathNoPeekVis, asset->noPeekVisBytes);
		dumper.dump_array(asset->pathZones, asset->zonesBytes);
		dumper.dump_array(asset->pathDynStates, asset->dynStatesBytes);

		dumper.close();
	}
}