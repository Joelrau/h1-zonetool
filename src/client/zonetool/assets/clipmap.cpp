#include <std_include.hpp>
#include "clipmap.hpp"

namespace zonetool
{
	void IClipMap::add_script_string(scr_string_t* ptr, std::string str)
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

	std::string IClipMap::get_script_string(scr_string_t* ptr)
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

	void IClipMap::parse_info(ClipInfo* info, assetmanager::reader& read, ZoneMemory* mem)
	{

	}

	clipMap_t* IClipMap::parse(const std::string& name, ZoneMemory* mem)
	{
		return nullptr;
	}

	void IClipMap::init(const std::string& name, ZoneMemory* mem)
	{

	}

	void IClipMap::prepare(ZoneBuffer* buf, ZoneMemory* mem)
	{

	}

	void IClipMap::load_depending(IZone* zone)
	{

	}

	std::string IClipMap::name()
	{
		return this->name_;
	}

	std::int32_t IClipMap::type()
	{
		return ASSET_TYPE_COL_MAP_MP;
	}

	void IClipMap::write_info(IZone* zone, ZoneBuffer* buf, ClipInfo* data, ClipInfo* dest)
	{

	}

	void IClipMap::write(IZone* zone, ZoneBuffer* buf)
	{

	}

	void IClipMap::dump_info(ClipInfo* info, assetmanager::dumper& write)
	{
		write.dump_array(info->planes, info->planeCount);
		write.dump_array(info->materials, info->numMaterials);
		for (unsigned int i = 0; i < info->numMaterials; i++)
		{
			write.dump_string(info->materials[i].name);
		}

		write.dump_array(info->bCollisionTree.leafbrushNodes, info->bCollisionTree.leafbrushNodesCount);
		for (unsigned int i = 0; i < info->bCollisionTree.leafbrushNodesCount; i++)
		{
			if (info->bCollisionTree.leafbrushNodes[i].leafBrushCount > 0)
			{
				write.dump_array(info->bCollisionTree.leafbrushNodes[i].data.leaf.brushes, info->bCollisionTree.leafbrushNodes[i].leafBrushCount);
			}
		}
		write.dump_array(info->bCollisionTree.leafbrushes, info->bCollisionTree.numLeafBrushes);

		write.dump_array(info->pCollisionTree.aabbTrees, info->pCollisionTree.aabbTreeCount);

		write.dump_array(info->sCollisionTree.smodelNodes, info->sCollisionTree.smodelNodeCount);

		write.dump_array(info->bCollisionData.brushSides, info->bCollisionData.numBrushSides);
		write.dump_array(info->bCollisionData.brushEdges, info->bCollisionData.numBrushEdges);
		write.dump_array(info->bCollisionData.brushes, info->bCollisionData.numBrushes);
		for (unsigned int i = 0; i < info->bCollisionData.numBrushes; i++)
		{
			if (info->bCollisionData.brushes[i].sides)
			{
				write.dump_array(info->bCollisionData.brushes[i].sides, 1);
			}
			if (info->bCollisionData.brushes[i].baseAdjacentSide)
			{
				write.dump_array(info->bCollisionData.brushes[i].baseAdjacentSide, 1);
			}
		}
		write.dump_array(info->bCollisionData.brushBounds, info->bCollisionData.numBrushes);
		write.dump_array(info->bCollisionData.brushContents, info->bCollisionData.numBrushes);

		write.dump_array(info->pCollisionData.verts, info->pCollisionData.vertCount);
		write.dump_array(info->pCollisionData.triIndices, info->pCollisionData.triCount * 3);
		write.dump_array(info->pCollisionData.triEdgeIsWalkable, ((info->pCollisionData.triCount + 2 * info->pCollisionData.triCount + 31) >> 3) & 0x1FFFFFFC);
		write.dump_array(info->pCollisionData.borders, info->pCollisionData.borderCount);
		write.dump_array(info->pCollisionData.partitions, info->pCollisionData.partitionCount);
		for (int i = 0; i < info->pCollisionData.partitionCount; i++)
		{
			if (info->pCollisionData.partitions[i].borders)
			{
				write.dump_array(info->pCollisionData.partitions[i].borders, 1);
			}
		}

		write.dump_array(info->sCollisionData.staticModelList, info->sCollisionData.numStaticModels);
		for (unsigned int i = 0; i < info->sCollisionData.numStaticModels; i++)
		{
			if (info->sCollisionData.staticModelList[i].xmodel)
			{
				write.dump_asset(info->sCollisionData.staticModelList[i].xmodel);
			}
		}
	}

	static_assert(sizeof(clipMap_t) == sizeof(h2::clipMap_t));

	void IClipMap::dump(clipMap_t* asset)
	{
		const auto path = asset->name + ".colmap"s;

		assetmanager::dumper write;
		if (!write.open(path))
		{
			return;
		}

		write.dump_single(asset);
		write.dump_string(asset->name);

		dump_info(&asset->info, write);

		write.dump_single(asset->pInfo);
		//dump_info(asset->pInfo, write);

		write.dump_array(asset->nodes, asset->numNodes);
		for (unsigned int i = 0; i < asset->numNodes; i++)
		{
			if (asset->nodes[i].plane)
			{
				write.dump_array(asset->nodes[i].plane, 1);
			}
		}

		write.dump_array(asset->leafs, asset->numLeafs);
		write.dump_array(asset->cmodels, asset->numSubModels);
		for (unsigned int i = 0; i < asset->numSubModels; i++)
		{
			if (asset->cmodels[i].info)
			{
				write.dump_single(asset->cmodels[i].info);
				//dump_info(asset->cmodels[i].info, write);
			}
		}

		write.dump_asset(asset->mapEnts);

		for (int i = 0; i < 2; i++)
		{
			write.dump_int(asset->dynEntCount[i]);

			if (asset->dynEntCount[i] <= 0)
			{
				continue;
			}

			write.dump_array(asset->dynEntDefList[i], asset->dynEntCount[i]);
			write.dump_array(asset->dynEntPoseList[i], asset->dynEntCount[i]);
			write.dump_array(asset->dynEntClientList[i], asset->dynEntCount[i]);
			write.dump_array(asset->dynEntCollList[i], asset->dynEntCount[i]);
			for (int j = 0; j < asset->dynEntCount[i]; j++)
			{
				write.dump_asset(asset->dynEntDefList[i][j].baseModel);
				write.dump_asset(asset->dynEntDefList[i][j].destroyFx);
				write.dump_asset(asset->dynEntDefList[i][j].sound);
				write.dump_asset(asset->dynEntDefList[i][j].physPreset);
				write.dump_single(asset->dynEntDefList[i][j].hinge);
				write.dump_single(asset->dynEntDefList[i][j].linkTo);
				
				//write.dump_single(asset->dynEntClientList[i][j].hinge);
				//write.dump_asset(asset->dynEntClientList[i][j].activeModel);
			}
		}

		for (unsigned int i = 0; i < asset->dynEntAnchorCount; i++)
		{
			write.dump_string(SL_ConvertToString(asset->dynEntAnchorNames[i]));
		}

		write.dump_array(asset->scriptableMapEnts.instances, asset->scriptableMapEnts.instanceCount);
		if (asset->scriptableMapEnts.instances)
		{
			for (unsigned int i = 0; i < asset->scriptableMapEnts.instanceCount; i++)
			{
				if (asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_asset(asset->scriptableMapEnts.instances[i].def);
				}
				if (asset->scriptableMapEnts.instances[i].eventConstantsBuf && asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].eventConstantsBuf,
						asset->scriptableMapEnts.instances[i].def->eventConstantsSize);
				}
				if (asset->scriptableMapEnts.instances[i].targetData)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].targetData,
						asset->scriptableMapEnts.instances[i].targetDataCount);
				}
				auto str = SL_ConvertToString(asset->scriptableMapEnts.instances[i].targetname);
				write.dump_string(str ? str : "");
				if (asset->scriptableMapEnts.instances[i].currentModel)
				{
					write.dump_asset(asset->scriptableMapEnts.instances[i].currentModel);
				}
				if (asset->scriptableMapEnts.instances[i].partStates && asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].partStates,
						asset->scriptableMapEnts.instances[i].def->partCount);
				}
				if (asset->scriptableMapEnts.instances[i].eventStreamBuf && asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].eventStreamBuf,
						asset->scriptableMapEnts.instances[i].def->eventStreamSize);
				}
			}
		}

		write.dump_array(asset->scriptableMapEnts.animEntries, asset->scriptableMapEnts.animEntryCount);
		if (asset->scriptableMapEnts.animEntries)
		{
			for (unsigned int i = 0; i < asset->scriptableMapEnts.animEntryCount; i++)
			{
				if (asset->scriptableMapEnts.animEntries[i].animName)
				{
					write.dump_string(asset->scriptableMapEnts.animEntries[i].animName);
				}
			}
		}

		write.dump_array(asset->grappleData.sphereTreeData.sphereTree, asset->grappleData.sphereTreeData.sphereTreeCount);
		if (asset->grappleData.sphereTreeData.sphereTree)
		{
			for (int i = 0; i < asset->grappleData.sphereTreeData.sphereTreeCount; i++)
			{
				write.dump_array(asset->grappleData.sphereTreeData.sphereTree[i].unk, asset->grappleData.sphereTreeData.sphereTree[i].unk_count);
			}
		}

		write.dump_array(asset->grappleData.sphereTreeData.sphereTreeObj, asset->grappleData.sphereTreeData.sphereTreeObjCount);

		write.dump_array(asset->grappleData.magnet, asset->grappleData.magnetCount);

		write.close();

		// dump stages
		if (asset->stages)
		{
			assetmanager::dumper stageDumper;

			if (stageDumper.open(asset->name + ".ents.stages"s))
			{
				stageDumper.dump_int(asset->stageCount);
				if (asset->stageCount)
				{
					stageDumper.dump_array(asset->stages, asset->stageCount);

					for (int i = 0; i < asset->stageCount; i++)
					{
						stageDumper.dump_string(asset->stages[i].name);
					}
				}
			}

			stageDumper.close();
		}
	}
}