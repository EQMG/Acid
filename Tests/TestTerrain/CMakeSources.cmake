set(TESTTERRAIN_HEADERS_
        "Configs/ConfigManager.hpp"
        "Scenes/Scene1.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/ManagerUis.hpp"
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "MainWorld.hpp"
        "Uis/OverlayDebug.hpp"
        "Terrains/LodBehaviour.hpp"
        "Terrains/MaterialTerrain.hpp"
        "Terrains/MeshTerrain.hpp"
        "Voxels/MaterialVoxel.hpp"
        "Voxels/VoxelBlock.hpp"
        "Voxels/VoxelChunk.hpp"
        "Waters/MaterialWater.hpp"
        "Waters/MeshWater.hpp"
        )

set(TESTTERRAIN_SOURCES_
        "Configs/ConfigManager.cpp"
        "Scenes/Scene1.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/ManagerUis.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "MainWorld.cpp"
        "Uis/OverlayDebug.cpp"
        "Terrains/LodBehaviour.cpp"
        "Terrains/MaterialTerrain.cpp"
        "Terrains/MeshTerrain.cpp"
        "Voxels/MaterialVoxel.cpp"
        "Voxels/VoxelBlock.cpp"
        "Voxels/VoxelChunk.cpp"
        "Waters/MaterialWater.cpp"
        "Waters/MeshWater.cpp"
        )

source_group("Header Files" FILES ${TESTTERRAIN_HEADERS_})
source_group("Source Files" FILES ${TESTTERRAIN_SOURCES_})

set(TESTTERRAIN_SOURCES
        ${TESTTERRAIN_HEADERS_}
        ${TESTTERRAIN_SOURCES_}
        )