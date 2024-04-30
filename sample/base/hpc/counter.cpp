#include "counter.h"

namespace jipu
{
namespace hpc
{

const char* counterName(Counter counter)
{
    switch (counter)
    {
    case Counter::GPUActiveCy:
        return "GPUActiveCy";
    case Counter::GPUIRQActiveCy:
        return "GPUIRQActiveCy";
    case Counter::FragQueueJob:
        return "FragQueueJob";
    case Counter::FragQueueTask:
        return "FragQueueTask";
    case Counter::FragQueueActiveCy:
        return "FragQueueActiveCy";
    case Counter::NonFragQueueJob:
        return "NonFragQueueJob";
    case Counter::NonFragQueueTask:
        return "NonFragQueueTask";
    case Counter::NonFragQueueActiveCy:
        return "NonFragQueueActiveCy";
    case Counter::ResQueueJob:
        return "ResQueueJob";
    case Counter::ResQueueTask:
        return "ResQueueTask";
    case Counter::ResQueueActiveCy:
        return "ResQueueActiveCy";
    case Counter::ExtBusWrBt:
        return "ExtBusWrBt";
    case Counter::ExtBusRdBt:
        return "ExtBusRdBt";
    case Counter::ExtBusRdStallCy:
        return "ExtBusRdStallCy";
    case Counter::ExtBusWrStallCy:
        return "ExtBusWrStallCy";
    case Counter::FragActiveCy:
        return "FragActiveCy";
    case Counter::FragRdPrim:
        return "FragRdPrim";
    case Counter::FragThread:
        return "FragThread";
    case Counter::FragHelpThread:
        return "FragHelpThread";
    case Counter::FragRastQd:
        return "FragRastQd";
    case Counter::FragEZSTestQd:
        return "FragEZSTestQd";
    case Counter::FragEZSKillQd:
        return "FragEZSKillQd";
    case Counter::FragLZSTestTd:
        return "FragLZSTestTd";
    case Counter::FragLZSKillTd:
        return "FragLZSKillTd";
    case Counter::FragTile:
        return "FragTile";
    case Counter::FragTileKill:
        return "FragTileKill";
    case Counter::NonFragActiveCy:
        return "NonFragActiveCy";
    case Counter::NonFragThread:
        return "NonFragThread";
    case Counter::CoreActiveCy:
        return "CoreActiveCy";
    case Counter::EngInstr:
        return "EngInstr";
    case Counter::LSIssueCy:
        return "LSIssueCy";
    case Counter::TexInstr:
        return "TexInstr";
    case Counter::TexFiltIssueCy:
        return "TexFiltIssueCy";
    case Counter::LSRdHitCy:
        return "LSRdHitCy";
    case Counter::LSWrHitCy:
        return "LSWrHitCy";
    case Counter::GeomTrianglePrim:
        return "GeomTrianglePrim";
    case Counter::GeomPointPrim:
        return "GeomPointPrim";
    case Counter::GeomLinePrim:
        return "GeomLinePrim";
    case Counter::GeomFrontFacePrim:
        return "GeomFrontFacePrim";
    case Counter::GeomBackFacePrim:
        return "GeomBackFacePrim";
    case Counter::GeomVisiblePrim:
        return "GeomVisiblePrim";
    case Counter::GeomFaceXYPlaneCullPrim:
        return "GeomFaceXYPlaneCullPrim";
    case Counter::GeomZPlaneCullPrim:
        return "GeomZPlaneCullPrim";
    case Counter::TilerActiveCy:
        return "TilerActiveCy";
    case Counter::GPUIRQUtil:
        return "GPUIRQUtil";
    case Counter::FragQueueUtil:
        return "FragQueueUtil";
    case Counter::NonFragQueueUtil:
        return "NonFragQueueUtil";
    case Counter::ExtBusRdBy:
        return "ExtBusRdBy";
    case Counter::ExtBusWrBy:
        return "ExtBusWrBy";
    case Counter::ExtBusRdStallRate:
        return "ExtBusRdStallRate";
    case Counter::ExtBusWrStallRate:
        return "ExtBusWrStallRate";
    case Counter::TilerUtil:
        return "TilerUtil";
    case Counter::GeomTotalPrim:
        return "GeomTotalPrim";
    case Counter::GeomVisibleRate:
        return "GeomVisibleRate";
    case Counter::GeomTotalCullPrim:
        return "GeomTotalCullPrim";
    case Counter::GeomFaceXYPlaneCullRate:
        return "GeomFaceXYPlaneCullRate";
    case Counter::GeomZPlaneCullRate:
        return "GeomZPlaneCullRate";
    case Counter::NonFragUtil:
        return "NonFragUtil";
    case Counter::NonFragThroughputCy:
        return "NonFragThroughputCy";
    case Counter::FragUtil:
        return "FragUtil";
    case Counter::FragThroughputCy:
        return "FragThroughputCy";
    case Counter::FragHelpTdRate:
        return "FragHelpTdRate";
    case Counter::FragEZSTestRate:
        return "FragEZSTestRate";
    case Counter::FragEZSKillRate:
        return "FragEZSKillRate";
    case Counter::FragLZSTestRate:
        return "FragLZSTestRate";
    case Counter::FragLZSKillRate:
        return "FragLZSKillRate";
    case Counter::FragOverdraw:
        return "FragOverdraw";
    case Counter::FragTileKillRate:
        return "FragTileKillRate";
    case Counter::CoreUtil:
        return "CoreUtil";
    case Counter::ALUUtil:
        return "ALUUtil";
    case Counter::TexSample:
        return "TexSample";
    case Counter::TexCPI:
        return "TexCPI";
    case Counter::TexUtil:
        return "TexUtil";
    case Counter::TexIssueCy:
        return "TexIssueCy";
    case Counter::LSUtil:
        return "LSUtil";
    case Counter::GPUPix:
        return "GPUPix";
    case Counter::GPUCyPerPix:
        return "GPUCyPerPix";
    case Counter::FragQueueWaitRdCy:
        return "FragQueueWaitRdCy";
    case Counter::FragQueueWaitIssueCy:
        return "FragQueueWaitIssueCy";
    case Counter::FragQueueWaitDepCy:
        return "FragQueueWaitDepCy";
    case Counter::FragQueueWaitFinishCy:
        return "FragQueueWaitFinishCy";
    case Counter::NonFragQueueWaitRdCy:
        return "NonFragQueueWaitRdCy";
    case Counter::NonFragQueueWaitIssueCy:
        return "NonFragQueueWaitIssueCy";
    case Counter::NonFragQueueWaitDepCy:
        return "NonFragQueueWaitDepCy";
    case Counter::NonFragQueueWaitFinishCy:
        return "NonFragQueueWaitFinishCy";
    case Counter::ResQueueWaitRdCy:
        return "ResQueueWaitRdCy";
    case Counter::ResQueueWaitIssueCy:
        return "ResQueueWaitIssueCy";
    case Counter::ResQueueWaitDepCy:
        return "ResQueueWaitDepCy";
    case Counter::ResQueueWaitFinishCy:
        return "ResQueueWaitFinishCy";
    case Counter::MMUL2Hit:
        return "MMUL2Hit";
    case Counter::MMUL2Rd:
        return "MMUL2Rd";
    case Counter::MMULookup:
        return "MMULookup";
    case Counter::L2CacheLookup:
        return "L2CacheLookup";
    case Counter::L2CacheRdLookup:
        return "L2CacheRdLookup";
    case Counter::L2CacheWrLookup:
        return "L2CacheWrLookup";
    case Counter::FragFPKActiveCy:
        return "FragFPKActiveCy";
    case Counter::LSRdCy:
        return "LSRdCy";
    case Counter::LSWrCy:
        return "LSWrCy";
    case Counter::LSAtomic:
        return "LSAtomic";
    case Counter::TilerPosCacheHit:
        return "TilerPosCacheHit";
    case Counter::TilerPosCacheMiss:
        return "TilerPosCacheMiss";
    case Counter::FragFPKBUtil:
        return "FragFPKBUtil";
    case Counter::FragQueueWaitFlushCy:
        return "FragQueueWaitFlushCy";
    case Counter::NonFragQueueWaitFlushCy:
        return "NonFragQueueWaitFlushCy";
    case Counter::ResQueueWaitFlushCy:
        return "ResQueueWaitFlushCy";
    case Counter::L2CacheFlush:
        return "L2CacheFlush";
    case Counter::GeomSampleCullPrim:
        return "GeomSampleCullPrim";
    case Counter::TilerRdBt:
        return "TilerRdBt";
    case Counter::TilerWrBt:
        return "TilerWrBt";
    case Counter::GeomPosShadTask:
        return "GeomPosShadTask";
    case Counter::TilerPosShadStallCy:
        return "TilerPosShadStallCy";
    case Counter::TilerPosShadFIFOFullCy:
        return "TilerPosShadFIFOFullCy";
    case Counter::TilerVarCacheHit:
        return "TilerVarCacheHit";
    case Counter::TilerVarCacheMiss:
        return "TilerVarCacheMiss";
    case Counter::GeomVarShadTask:
        return "GeomVarShadTask";
    case Counter::TilerVarShadStallCy:
        return "TilerVarShadStallCy";
    case Counter::FragRastPrim:
        return "FragRastPrim";
    case Counter::FragWarp:
        return "FragWarp";
    case Counter::FragPartWarp:
        return "FragPartWarp";
    case Counter::FragEZSUpdateQd:
        return "FragEZSUpdateQd";
    case Counter::FragLZSTestQd:
        return "FragLZSTestQd";
    case Counter::FragLZSKillQd:
        return "FragLZSKillQd";
    case Counter::FragOpaqueQd:
        return "FragOpaqueQd";
    case Counter::NonFragTask:
        return "NonFragTask";
    case Counter::NonFragWarp:
        return "NonFragWarp";
    case Counter::EngActiveCy:
        return "EngActiveCy";
    case Counter::EngDivergedInstr:
        return "EngDivergedInstr";
    case Counter::EngStarveCy:
        return "EngStarveCy";
    case Counter::TexQuads:
        return "TexQuads";
    case Counter::TexQuadPass:
        return "TexQuadPass";
    case Counter::TexQuadPassDescMiss:
        return "TexQuadPassDescMiss";
    case Counter::TexQuadPassMip:
        return "TexQuadPassMip";
    case Counter::TexQuadPassTri:
        return "TexQuadPassTri";
    case Counter::TexCacheFetch:
        return "TexCacheFetch";
    case Counter::TexCacheCompressFetch:
        return "TexCacheCompressFetch";
    case Counter::TexCacheLookup:
        return "TexCacheLookup";
    case Counter::LSFullRd:
        return "LSFullRd";
    case Counter::LSPartRd:
        return "LSPartRd";
    case Counter::LSFullWr:
        return "LSFullWr";
    case Counter::LSPartWr:
        return "LSPartWr";
    case Counter::VarInstr:
        return "VarInstr";
    case Counter::Var32IssueSlot:
        return "Var32IssueSlot";
    case Counter::Var16IssueSlot:
        return "Var16IssueSlot";
    case Counter::AttrInstr:
        return "AttrInstr";
    case Counter::SCBusFFEL2RdBt:
        return "SCBusFFEL2RdBt";
    case Counter::SCBusFFEExtRdBt:
        return "SCBusFFEExtRdBt";
    case Counter::SCBusLSL2RdBt:
        return "SCBusLSL2RdBt";
    case Counter::SCBusLSExtRdBt:
        return "SCBusLSExtRdBt";
    case Counter::SCBusTexL2RdBt:
        return "SCBusTexL2RdBt";
    case Counter::SCBusTexExtRdBt:
        return "SCBusTexExtRdBt";
    case Counter::SCBusOtherL2RdBt:
        return "SCBusOtherL2RdBt";
    case Counter::SCBusLSWBWrBt:
        return "SCBusLSWBWrBt";
    case Counter::SCBusTileWrBt:
        return "SCBusTileWrBt";
    case Counter::SCBusLSOtherWrBt:
        return "SCBusLSOtherWrBt";
    case Counter::MMUL3Rd:
        return "MMUL3Rd";
    case Counter::MMUL3Hit:
        return "MMUL3Hit";
    case Counter::MMUS2Lookup:
        return "MMUS2Lookup";
    case Counter::MMUS2L3Rd:
        return "MMUS2L3Rd";
    case Counter::MMUS2L2Rd:
        return "MMUS2L2Rd";
    case Counter::MMUS2L3Hit:
        return "MMUS2L3Hit";
    case Counter::MMUS2L2Hit:
        return "MMUS2L2Hit";
    case Counter::L2CacheRd:
        return "L2CacheRd";
    case Counter::L2CacheRdStallCy:
        return "L2CacheRdStallCy";
    case Counter::L2CacheWr:
        return "L2CacheWr";
    case Counter::L2CacheWrStallCy:
        return "L2CacheWrStallCy";
    case Counter::L2CacheSnp:
        return "L2CacheSnp";
    case Counter::L2CacheSnpStallCy:
        return "L2CacheSnpStallCy";
    case Counter::L2CacheL1Rd:
        return "L2CacheL1Rd";
    case Counter::L2CacheL1RdStallCy:
        return "L2CacheL1RdStallCy";
    case Counter::L2CacheL1Wr:
        return "L2CacheL1Wr";
    case Counter::L2CacheSnpLookup:
        return "L2CacheSnpLookup";
    case Counter::ExtBusRd:
        return "ExtBusRd";
    case Counter::ExtBusRdNoSnoop:
        return "ExtBusRdNoSnoop";
    case Counter::ExtBusRdUnique:
        return "ExtBusRdUnique";
    case Counter::ExtBusRdOTQ1:
        return "ExtBusRdOTQ1";
    case Counter::ExtBusRdOTQ2:
        return "ExtBusRdOTQ2";
    case Counter::ExtBusRdOTQ3:
        return "ExtBusRdOTQ3";
    case Counter::ExtBusRdLat0:
        return "ExtBusRdLat0";
    case Counter::ExtBusRdLat128:
        return "ExtBusRdLat128";
    case Counter::ExtBusRdLat192:
        return "ExtBusRdLat192";
    case Counter::ExtBusRdLat256:
        return "ExtBusRdLat256";
    case Counter::ExtBusRdLat320:
        return "ExtBusRdLat320";
    case Counter::ExtBusWr:
        return "ExtBusWr";
    case Counter::ExtBusWrNoSnoopFull:
        return "ExtBusWrNoSnoopFull";
    case Counter::ExtBusWrNoSnoopPart:
        return "ExtBusWrNoSnoopPart";
    case Counter::ExtBusWrSnoopFull:
        return "ExtBusWrSnoopFull";
    case Counter::ExtBusWrSnoopPart:
        return "ExtBusWrSnoopPart";
    case Counter::ExtBusWrOTQ1:
        return "ExtBusWrOTQ1";
    case Counter::ExtBusWrOTQ2:
        return "ExtBusWrOTQ2";
    case Counter::ExtBusWrOTQ3:
        return "ExtBusWrOTQ3";
    case Counter::L2CacheIncSnp:
        return "L2CacheIncSnp";
    case Counter::L2CacheIncSnpStallCy:
        return "L2CacheIncSnpStallCy";
    case Counter::L2CacheRdMissRate:
        return "L2CacheRdMissRate";
    case Counter::L2CacheWrMissRate:
        return "L2CacheWrMissRate";
    case Counter::ExtBusRdLat384:
        return "ExtBusRdLat384";
    case Counter::ExtBusRdOTQ4:
        return "ExtBusRdOTQ4";
    case Counter::ExtBusWrOTQ4:
        return "ExtBusWrOTQ4";
    case Counter::GeomSampleCullRate:
        return "GeomSampleCullRate";
    case Counter::GeomPosShadThread:
        return "GeomPosShadThread";
    case Counter::GeomPosShadThreadPerPrim:
        return "GeomPosShadThreadPerPrim";
    case Counter::TilerPosCacheHitRate:
        return "TilerPosCacheHitRate";
    case Counter::GeomVarShadThread:
        return "GeomVarShadThread";
    case Counter::GeomVarShadThreadPerPrim:
        return "GeomVarShadThreadPerPrim";
    case Counter::TilerVarCacheHitRate:
        return "TilerVarCacheHitRate";
    case Counter::FragOpaqueQdRate:
        return "FragOpaqueQdRate";
    case Counter::FragTransparentQd:
        return "FragTransparentQd";
    case Counter::FragShadedQd:
        return "FragShadedQd";
    case Counter::FragPartWarpRate:
        return "FragPartWarpRate";
    case Counter::FragEZSUpdateRate:
        return "FragEZSUpdateRate";
    case Counter::FragFPKKillQd:
        return "FragFPKKillQd";
    case Counter::FragFPKKillRate:
        return "FragFPKKillRate";
    case Counter::EngDivergedInstrRate:
        return "EngDivergedInstrRate";
    case Counter::TexCacheUtil:
        return "TexCacheUtil";
    case Counter::TexMipInstrRate:
        return "TexMipInstrRate";
    case Counter::TexCacheCompressFetchRate:
        return "TexCacheCompressFetchRate";
    case Counter::TexTriInstrRate:
        return "TexTriInstrRate";
    case Counter::Var32IssueCy:
        return "Var32IssueCy";
    case Counter::Var16IssueCy:
        return "Var16IssueCy";
    case Counter::VarIssueCy:
        return "VarIssueCy";
    case Counter::VarUtil:
        return "VarUtil";
    case Counter::SCBusFFEL2RdBy:
        return "SCBusFFEL2RdBy";
    case Counter::SCBusFFEExtRdBy:
        return "SCBusFFEExtRdBy";
    case Counter::SCBusLSL2RdBy:
        return "SCBusLSL2RdBy";
    case Counter::SCBusLSL2RdByPerRd:
        return "SCBusLSL2RdByPerRd";
    case Counter::SCBusLSExtRdBy:
        return "SCBusLSExtRdBy";
    case Counter::SCBusLSExtRdByPerRd:
        return "SCBusLSExtRdByPerRd";
    case Counter::SCBusTexL2RdBy:
        return "SCBusTexL2RdBy";
    case Counter::SCBusTexL2RdByPerRd:
        return "SCBusTexL2RdByPerRd";
    case Counter::SCBusTexExtRdBy:
        return "SCBusTexExtRdBy";
    case Counter::SCBusTexExtRdByPerRd:
        return "SCBusTexExtRdByPerRd";
    case Counter::SCBusLSWrBt:
        return "SCBusLSWrBt";
    case Counter::SCBusLSWrBy:
        return "SCBusLSWrBy";
    case Counter::SCBusLSWrByPerWr:
        return "SCBusLSWrByPerWr";
    case Counter::SCBusTileWrBy:
        return "SCBusTileWrBy";
    case Counter::CoreAllRegsWarp:
        return "CoreAllRegsWarp";
    case Counter::CoreFullQdWarp:
        return "CoreFullQdWarp";
    case Counter::CoreAllRegsWarpRate:
        return "CoreAllRegsWarpRate";
    case Counter::CoreFullQdWarpRate:
        return "CoreFullQdWarpRate";
    case Counter::TexMipInstr:
        return "TexMipInstr";
    case Counter::TexCompressInstr:
        return "TexCompressInstr";
    case Counter::Tex3DInstr:
        return "Tex3DInstr";
    case Counter::TexTriInstr:
        return "TexTriInstr";
    case Counter::TexCoordStallCy:
        return "TexCoordStallCy";
    case Counter::TexDataStallCy:
        return "TexDataStallCy";
    case Counter::TexPartDataStallCy:
        return "TexPartDataStallCy";
    case Counter::SCBusOtherWrBt:
        return "SCBusOtherWrBt";
    case Counter::TexCompressInstrRate:
        return "TexCompressInstrRate";
    case Counter::Tex3DInstrRate:
        return "Tex3DInstrRate";
    case Counter::SCBusOtherWrBy:
        return "SCBusOtherWrBy";
    case Counter::FragRastPartQd:
        return "FragRastPartQd";
    case Counter::EngFMAInstr:
        return "EngFMAInstr";
    case Counter::EngCVTInstr:
        return "EngCVTInstr";
    case Counter::EngSFUInstr:
        return "EngSFUInstr";
    case Counter::EngICacheMiss:
        return "EngICacheMiss";
    case Counter::EngSWBlendInstr:
        return "EngSWBlendInstr";
    case Counter::TexInBt:
        return "TexInBt";
    case Counter::TexDescStallCy:
        return "TexDescStallCy";
    case Counter::TexDataFetchStallCy:
        return "TexDataFetchStallCy";
    case Counter::TexFiltStallCy:
        return "TexFiltStallCy";
    case Counter::TexFullBiFiltCy:
        return "TexFullBiFiltCy";
    case Counter::TexFullTriFiltCy:
        return "TexFullTriFiltCy";
    case Counter::TexOutMsg:
        return "TexOutMsg";
    case Counter::TexOutBt:
        return "TexOutBt";
    case Counter::FragRastPartQdRate:
        return "FragRastPartQdRate";
    case Counter::EngFMAPipeUtil:
        return "EngFMAPipeUtil";
    case Counter::EngCVTPipeUtil:
        return "EngCVTPipeUtil";
    case Counter::EngSFUPipeUtil:
        return "EngSFUPipeUtil";
    case Counter::EngArithInstr:
        return "EngArithInstr";
    case Counter::EngSWBlendRate:
        return "EngSWBlendRate";
    case Counter::TexInBusUtil:
        return "TexInBusUtil";
    case Counter::TexOutBusUtil:
        return "TexOutBusUtil";
    case Counter::TexFiltFullRate:
        return "TexFiltFullRate";
    case Counter::AnyActiveCy:
        return "AnyActiveCy";
    case Counter::AnyUtil:
        return "AnyUtil";
    case Counter::CSFMCUActiveCy:
        return "CSFMCUActiveCy";
    case Counter::GPUIterActiveCy:
        return "GPUIterActiveCy";
    case Counter::GPUIRQ:
        return "GPUIRQ";
    case Counter::L2CacheFlushCy:
        return "L2CacheFlushCy";
    case Counter::VertIterQueuedCy:
        return "VertIterQueuedCy";
    case Counter::VertIterJob:
        return "VertIterJob";
    case Counter::VertIterTask:
        return "VertIterTask";
    case Counter::VertIterTotalActiveCy:
        return "VertIterTotalActiveCy";
    case Counter::VertIterIRQActiveCy:
        return "VertIterIRQActiveCy";
    case Counter::VertIterAssignStallCy:
        return "VertIterAssignStallCy";
    case Counter::TilerIterDrainStallCy:
        return "TilerIterDrainStallCy";
    case Counter::CompIterQueuedCy:
        return "CompIterQueuedCy";
    case Counter::CompIterJob:
        return "CompIterJob";
    case Counter::CompIterTask:
        return "CompIterTask";
    case Counter::CompIterTotalActiveCy:
        return "CompIterTotalActiveCy";
    case Counter::CompIterIRQActiveCy:
        return "CompIterIRQActiveCy";
    case Counter::CompIterAssignStallCy:
        return "CompIterAssignStallCy";
    case Counter::CompIterDrainStallCy:
        return "CompIterDrainStallCy";
    case Counter::FragIterQueuedCy:
        return "FragIterQueuedCy";
    case Counter::FragIterJob:
        return "FragIterJob";
    case Counter::FragIterTask:
        return "FragIterTask";
    case Counter::FragIterTotalActiveCy:
        return "FragIterTotalActiveCy";
    case Counter::FragIterIRQActiveCy:
        return "FragIterIRQActiveCy";
    case Counter::FragIterAssignStallCy:
        return "FragIterAssignStallCy";
    case Counter::CSFCEUActiveCy:
        return "CSFCEUActiveCy";
    case Counter::CSFLSUActiveCy:
        return "CSFLSUActiveCy";
    case Counter::CSFCS0ActiveCy:
        return "CSFCS0ActiveCy";
    case Counter::CS0WaitStallCy:
        return "CS0WaitStallCy";
    case Counter::CSFCS1ActiveCy:
        return "CSFCS1ActiveCy";
    case Counter::CS1WaitStallCy:
        return "CS1WaitStallCy";
    case Counter::CSFCS2ActiveCy:
        return "CSFCS2ActiveCy";
    case Counter::CS2WaitStallCy:
        return "CS2WaitStallCy";
    case Counter::CSFCS3ActiveCy:
        return "CSFCS3ActiveCy";
    case Counter::CS3WaitStallCy:
        return "CS3WaitStallCy";
    case Counter::L2CacheEvict:
        return "L2CacheEvict";
    case Counter::L2CacheCleanUnique:
        return "L2CacheCleanUnique";
    case Counter::FragIterActiveCy:
        return "FragIterActiveCy";
    case Counter::FragIterUtil:
        return "FragIterUtil";
    case Counter::VertIterActiveCy:
        return "VertIterActiveCy";
    case Counter::VertIterUtil:
        return "VertIterUtil";
    case Counter::CompIterActiveCy:
        return "CompIterActiveCy";
    case Counter::CompIterUtil:
        return "CompIterUtil";
    case Counter::CSFMCUUtil:
        return "CSFMCUUtil";
    case Counter::CSFLSUUtil:
        return "CSFLSUUtil";
    case Counter::CSFCEUUtil:
        return "CSFCEUUtil";
    case Counter::EngNarrowInstr:
        return "EngNarrowInstr";
    case Counter::FragRastCoarseQd:
        return "FragRastCoarseQd";
    case Counter::RTUTri:
        return "RTUTri";
    case Counter::RTUBox:
        return "RTUBox";
    case Counter::RTUTriBin1:
        return "RTUTriBin1";
    case Counter::RTUTriBin5:
        return "RTUTriBin5";
    case Counter::RTUTriBin9:
        return "RTUTriBin9";
    case Counter::RTUTriBin13:
        return "RTUTriBin13";
    case Counter::RTUBoxBin1:
        return "RTUBoxBin1";
    case Counter::RTUBoxBin5:
        return "RTUBoxBin5";
    case Counter::RTUBoxBin9:
        return "RTUBoxBin9";
    case Counter::RTUBoxBin13:
        return "RTUBoxBin13";
    case Counter::RTUOpaqueHit:
        return "RTUOpaqueHit";
    case Counter::RTUNonOpaqueHit:
        return "RTUNonOpaqueHit";
    case Counter::RTUFirstHitTerm:
        return "RTUFirstHitTerm";
    case Counter::RTUMiss:
        return "RTUMiss";
    case Counter::RTURay:
        return "RTURay";
    case Counter::RTUBoxIssueCy:
        return "RTUBoxIssueCy";
    case Counter::RTUTriIssueCy:
        return "RTUTriIssueCy";
    case Counter::GeomFaceCullPrim:
        return "GeomFaceCullPrim";
    case Counter::GeomPlaneCullPrim:
        return "GeomPlaneCullPrim";
    case Counter::GeomFaceCullRate:
        return "GeomFaceCullRate";
    case Counter::GeomPlaneCullRate:
        return "GeomPlaneCullRate";
    case Counter::FragShadRate:
        return "FragShadRate";
    case Counter::CoreFragWarpOcc:
        return "CoreFragWarpOcc";
    case Counter::EngNarrowInstrRate:
        return "EngNarrowInstrRate";
    case Counter::RTUUtil:
        return "RTUUtil";
    case Counter::BinningIterQueuedCy:
        return "BinningIterQueuedCy";
    case Counter::BinningIterJob:
        return "BinningIterJob";
    case Counter::BinningIterTask:
        return "BinningIterTask";
    case Counter::BinningIterIRQActiveCy:
        return "BinningIterIRQActiveCy";
    case Counter::BinningIterAssignStallCy:
        return "BinningIterAssignStallCy";
    case Counter::MainIterQueuedCy:
        return "MainIterQueuedCy";
    case Counter::MainIterJob:
        return "MainIterJob";
    case Counter::MainIterTask:
        return "MainIterTask";
    case Counter::MainIterIRQActiveCy:
        return "MainIterIRQActiveCy";
    case Counter::MainIterAssignStallCy:
        return "MainIterAssignStallCy";
    case Counter::TexL1CacheLoadCy:
        return "TexL1CacheLoadCy";
    case Counter::TexCacheSimpleLoadCy:
        return "TexCacheSimpleLoadCy";
    case Counter::TexL1CacheOutputCy:
        return "TexL1CacheOutputCy";
    case Counter::TexL1CacheLookupCy:
        return "TexL1CacheLookupCy";
    case Counter::TexOutSingleMsg:
        return "TexOutSingleMsg";
    case Counter::TexCacheLookupCy:
        return "TexCacheLookupCy";
    case Counter::TexCacheComplexLoadCy:
        return "TexCacheComplexLoadCy";
    case Counter::TexIndexCy:
        return "TexIndexCy";
    case Counter::TexClkStarvedCy:
        return "TexClkStarvedCy";
    case Counter::TexClkActiveCy:
        return "TexClkActiveCy";
    case Counter::GeomScissorCullPrim:
        return "GeomScissorCullPrim";
    case Counter::GeomVisibleDVSPrim:
        return "GeomVisibleDVSPrim";
    case Counter::MainIterActiveCy:
        return "MainIterActiveCy";
    case Counter::MainIterUtil:
        return "MainIterUtil";
    case Counter::BinningIterActiveCy:
        return "BinningIterActiveCy";
    case Counter::BinningIterUtil:
        return "BinningIterUtil";
    case Counter::GeomScissorCullRate:
        return "GeomScissorCullRate";
    }
}

} // namespace hpc
} // namespace jipu