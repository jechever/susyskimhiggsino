#include "SusySkimHiggsino/twoleptonSelector.h"
#include <algorithm>

// includes needed for emulation of L1Topo based triggers
#include "xAODTrigMissingET/TrigMissingETContainer.h"
#include "xAODTrigger/EnergySumRoI.h"
#include "xAODTrigger/JetRoIContainer.h"
#include "xAODJet/JetContainer.h"


// ------------------------------------------------------------------------------------------ //
twoleptonSelector::twoleptonSelector() :
  BaseUser("SusySkimHiggsino", "twoleptonSelector")
{

}
// ------------------------------------------------------------------------------------------ //
void twoleptonSelector::setup(ConfigMgr*& configMgr)
{

  // Make a cutflow stream
  configMgr->cutflow->defineCutFlow("cutFlow", configMgr->treeMaker->getFile("tree"));

  // Use object def from SUSYTools
  configMgr->obj->useSUSYToolsSignalDef(true);

  // Bad muon veto
  configMgr->obj->applyBadMuonVeto(false);

  // Muon cuts
  configMgr->obj->setBaselineMuonPt(3.0);
  configMgr->obj->setBaselineMuonEta(2.50);
  configMgr->obj->setSignalMuonPt(3.0);
  configMgr->obj->setSignalMuonEta(2.50);

  // Electrons
  configMgr->obj->setBaselineElectronEt(4.5);
  configMgr->obj->setBaselineElectronEta(2.47);
  configMgr->obj->setSignalElectronEt(4.5);
  configMgr->obj->setSignalElectronEta(2.47);

  // Jets
  configMgr->obj->setCJetPt(20.0);
  configMgr->obj->setCJetEta(2.80);
  configMgr->obj->setFJetPt(30.0);
  configMgr->obj->setFJetEtaMin(2.80);
  configMgr->obj->setFJetEtaMax(4.50);

  // Turn off OR for baseline muons for fakes selection
  //configMgr->obj->disableORBaselineMuons(true);

  // MET Triggers
  // Defined by run number <start,end>; -1 means ignore that bound (e.g. 2016 runs are open ended)
  configMgr->addTriggerAna("HLT_xe70",276262,284484,"metTrig");
  configMgr->addTriggerAna("HLT_xe90_mht_L1XE50",296939,302872,"metTrig");
  configMgr->addTriggerAna("HLT_xe100_mht_L1XE50",302919,303892,"metTrig");
  configMgr->addTriggerAna("HLT_xe110_mht_L1XE50",303943,-1,"metTrig");
  //
  // ---- Output trees ---- //
  //
  
  // Emulated Triggers
  configMgr->treeMaker->addBoolVariable("HLT_2mu4_j85_xe50_mht_emul", 0);
  configMgr->treeMaker->addBoolVariable("HLT_mu4_j125_xe90_mht_emul", 0);
  configMgr->treeMaker->addFloatVariable("L1_met_Et", 0);
  configMgr->treeMaker->addFloatVariable("L1_jet1Pt", 0);
  configMgr->treeMaker->addFloatVariable("HLT_met_Et", 0);
  configMgr->treeMaker->addFloatVariable("HLT_jet1Pt", 0);
  configMgr->treeMaker->addBoolVariable("DPHI_2J20XE", 0);
  configMgr->treeMaker->addBoolVariable("2dphi10", 0);
  configMgr->treeMaker->addBoolVariable("HLT_2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI_J20s2XE30", 0);
  configMgr->treeMaker->addBoolVariable("HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI_J20s2XE30", 0);
  configMgr->treeMaker->addBoolVariable("HLT_e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI_J20s2XE30", 0);
  configMgr->treeMaker->addBoolVariable("HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI_J20s2XE50", 0);
  configMgr->treeMaker->addBoolVariable("HLT_e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI_J20s2XE50", 0);

  // Global vars
  configMgr->treeMaker->addFloatVariable("mu",0.0);
  configMgr->treeMaker->addIntVariable("nVtx",0.0);

  // Lepton vars
  configMgr->treeMaker->addIntVariable("nLep_base",0);
  configMgr->treeMaker->addIntVariable("nLep_signal",0);

  configMgr->treeMaker->addIntVariable("lep1Flavor",0);
  configMgr->treeMaker->addIntVariable("lep1Charge",0);
  configMgr->treeMaker->addIntVariable("lep1Author",-1);
  configMgr->treeMaker->addFloatVariable("lep1Pt",0.0);
  configMgr->treeMaker->addFloatVariable("lep1Eta",0.0);
  configMgr->treeMaker->addFloatVariable("lep1Phi",0.0);
  configMgr->treeMaker->addFloatVariable("lep1M",0.0);
  configMgr->treeMaker->addFloatVariable("lep1D0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1D0Sig",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1Z0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1Z0SinTheta",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1Topoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1Ptvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1Ptvarcone30",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1CorrTopoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1CorrPtvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep1CorrPtvarcone30",-999.0);
  configMgr->treeMaker->addBoolVariable("lep1PassOR",false);
  configMgr->treeMaker->addIntVariable("lep1Type",-1);
  configMgr->treeMaker->addIntVariable("lep1Origin",-1);
  configMgr->treeMaker->addIntVariable("lep1EgMotherType",-1);
  configMgr->treeMaker->addIntVariable("lep1EgMotherOrigin",-1);
  configMgr->treeMaker->addIntVariable("lep1NPix",-1);
  configMgr->treeMaker->addBoolVariable("lep1PassBL",false);
  configMgr->treeMaker->addBoolVariable("lep1VeryLoose",false);
  configMgr->treeMaker->addBoolVariable("lep1Loose",false);
  configMgr->treeMaker->addBoolVariable("lep1Medium",false);
  configMgr->treeMaker->addBoolVariable("lep1Tight",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoLoose",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoTight",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoGradient",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrLoose",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrTight",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrGradient",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep1IsoCorrFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep1Signal",false);
  configMgr->treeMaker->addBoolVariable("lep1TruthMatched",false);
  configMgr->treeMaker->addIntVariable("lep1TruthCharge",0);
  configMgr->treeMaker->addFloatVariable("lep1TruthPt",0.0);
  configMgr->treeMaker->addFloatVariable("lep1TruthEta",0.0);
  configMgr->treeMaker->addFloatVariable("lep1TruthPhi",0.0);
  configMgr->treeMaker->addFloatVariable("lep1TruthM",0.0);

  configMgr->treeMaker->addIntVariable("lep2Flavor",0);
  configMgr->treeMaker->addIntVariable("lep2Charge",0);
  configMgr->treeMaker->addIntVariable("lep2Author",-1);
  configMgr->treeMaker->addFloatVariable("lep2Pt",0.0);
  configMgr->treeMaker->addFloatVariable("lep2Eta",0.0);
  configMgr->treeMaker->addFloatVariable("lep2Phi",0.0);
  configMgr->treeMaker->addFloatVariable("lep2M",0.0);
  configMgr->treeMaker->addFloatVariable("lep2D0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2D0Sig",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2Z0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2Z0SinTheta",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2Topoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2Ptvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2Ptvarcone30",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2CorrTopoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2CorrPtvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep2CorrPtvarcone30",-999.0);
  configMgr->treeMaker->addBoolVariable("lep2PassOR",false);
  configMgr->treeMaker->addIntVariable("lep2Type",-1);
  configMgr->treeMaker->addIntVariable("lep2Origin",-1);
  configMgr->treeMaker->addIntVariable("lep2EgMotherType",-1);
  configMgr->treeMaker->addIntVariable("lep2EgMotherOrigin",-1);
  configMgr->treeMaker->addIntVariable("lep2NPix",-1);
  configMgr->treeMaker->addBoolVariable("lep2PassBL",false);
  configMgr->treeMaker->addBoolVariable("lep2VeryLoose",false);
  configMgr->treeMaker->addBoolVariable("lep2Loose",false);
  configMgr->treeMaker->addBoolVariable("lep2Medium",false);
  configMgr->treeMaker->addBoolVariable("lep2Tight",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoLoose",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoTight",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoGradient",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrLoose",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrTight",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrGradient",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep2IsoCorrFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep2Signal",false);
  configMgr->treeMaker->addBoolVariable("lep2TruthMatched",false);
  configMgr->treeMaker->addIntVariable("lep2TruthCharge",0);
  configMgr->treeMaker->addFloatVariable("lep2TruthPt",0.0);
  configMgr->treeMaker->addFloatVariable("lep2TruthEta",0.0);
  configMgr->treeMaker->addFloatVariable("lep2TruthPhi",0.0);
  configMgr->treeMaker->addFloatVariable("lep2TruthM",0.0);

  configMgr->treeMaker->addIntVariable("lep3Flavor",0);
  configMgr->treeMaker->addIntVariable("lep3Charge",0);
  configMgr->treeMaker->addIntVariable("lep3Author",-1);
  configMgr->treeMaker->addFloatVariable("lep3Pt",0.0);
  configMgr->treeMaker->addFloatVariable("lep3Eta",0.0);
  configMgr->treeMaker->addFloatVariable("lep3Phi",0.0);
  configMgr->treeMaker->addFloatVariable("lep3M",0.0);
  configMgr->treeMaker->addFloatVariable("lep3D0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3D0Sig",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3Z0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3Z0SinTheta",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3Topoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3Ptvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3Ptvarcone30",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3CorrTopoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3CorrPtvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep3CorrPtvarcone30",-999.0);
  configMgr->treeMaker->addBoolVariable("lep3PassOR",false);
  configMgr->treeMaker->addIntVariable("lep3Type",-1);
  configMgr->treeMaker->addIntVariable("lep3Origin",-1);
  configMgr->treeMaker->addIntVariable("lep3EgMotherType",-1);
  configMgr->treeMaker->addIntVariable("lep3EgMotherOrigin",-1);
  configMgr->treeMaker->addIntVariable("lep3NPix",-1);
  configMgr->treeMaker->addBoolVariable("lep3PassBL",false);
  configMgr->treeMaker->addBoolVariable("lep3VeryLoose",false);
  configMgr->treeMaker->addBoolVariable("lep3Loose",false);
  configMgr->treeMaker->addBoolVariable("lep3Medium",false);
  configMgr->treeMaker->addBoolVariable("lep3Tight",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoLoose",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoTight",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoGradient",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrLoose",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrTight",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrGradient",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep3IsoCorrFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep3Signal",false);
  configMgr->treeMaker->addBoolVariable("lep3TruthMatched",false);
  configMgr->treeMaker->addIntVariable("lep3TruthCharge",0);
  configMgr->treeMaker->addFloatVariable("lep3TruthPt",0.0);
  configMgr->treeMaker->addFloatVariable("lep3TruthEta",0.0);
  configMgr->treeMaker->addFloatVariable("lep3TruthPhi",0.0);
  configMgr->treeMaker->addFloatVariable("lep3TruthM",0.0);

  configMgr->treeMaker->addIntVariable("lep4Flavor",0);
  configMgr->treeMaker->addIntVariable("lep4Charge",0);
  configMgr->treeMaker->addIntVariable("lep4Author",-1);
  configMgr->treeMaker->addFloatVariable("lep4Pt",0.0);
  configMgr->treeMaker->addFloatVariable("lep4Eta",0.0);
  configMgr->treeMaker->addFloatVariable("lep4Phi",0.0);
  configMgr->treeMaker->addFloatVariable("lep4M",0.0);
  configMgr->treeMaker->addFloatVariable("lep4D0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4D0Sig",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4Z0",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4Z0SinTheta",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4Topoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4Ptvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4Ptvarcone30",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4CorrTopoetcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4CorrPtvarcone20",-999.0);
  configMgr->treeMaker->addFloatVariable("lep4CorrPtvarcone30",-999.0);
  configMgr->treeMaker->addBoolVariable("lep4PassOR",false);
  configMgr->treeMaker->addIntVariable("lep4Type",-1);
  configMgr->treeMaker->addIntVariable("lep4Origin",-1);
  configMgr->treeMaker->addIntVariable("lep4EgMotherType",-1);
  configMgr->treeMaker->addIntVariable("lep4EgMotherOrigin",-1);
  configMgr->treeMaker->addIntVariable("lep4NPix",-1);
  configMgr->treeMaker->addBoolVariable("lep4PassBL",false);
  configMgr->treeMaker->addBoolVariable("lep4VeryLoose",false);
  configMgr->treeMaker->addBoolVariable("lep4Loose",false);
  configMgr->treeMaker->addBoolVariable("lep4Medium",false);
  configMgr->treeMaker->addBoolVariable("lep4Tight",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoLoose",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoTight",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoGradient",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrLoose",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrTight",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrGradient",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrGradientLoose",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrLooseTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrFixedCutLoose",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrFixedCutTight",false);
  configMgr->treeMaker->addBoolVariable("lep4IsoCorrFixedCutTightTrackOnly",false);
  configMgr->treeMaker->addBoolVariable("lep4Signal",false);
  configMgr->treeMaker->addBoolVariable("lep4TruthMatched",false);
  configMgr->treeMaker->addIntVariable("lep4TruthCharge",0);
  configMgr->treeMaker->addFloatVariable("lep4TruthPt",0.0);
  configMgr->treeMaker->addFloatVariable("lep4TruthEta",0.0);
  configMgr->treeMaker->addFloatVariable("lep4TruthPhi",0.0);
  configMgr->treeMaker->addFloatVariable("lep4TruthM",0.0);

  // Jets
  configMgr->treeMaker->addIntVariable("nJet30",0.0);
  configMgr->treeMaker->addIntVariable("nJet25",0.0);
  configMgr->treeMaker->addIntVariable("nJet20",0.0);

  configMgr->treeMaker->addIntVariable("nTotalJet",0.0);
  configMgr->treeMaker->addIntVariable("nTotalJet20",0.0);
  configMgr->treeMaker->addIntVariable("nBJet30_MV2c10",0.0);
  configMgr->treeMaker->addIntVariable("nBJet20_MV2c10",0.0);
  configMgr->treeMaker->addIntVariable("nBJet30_MV2c10_FixedCutBEff_60",0.0);
  configMgr->treeMaker->addIntVariable("nBJet30_MV2c10_FixedCutBEff_70",0.0);
  configMgr->treeMaker->addIntVariable("nBJet30_MV2c10_FixedCutBEff_77",0.0);
  configMgr->treeMaker->addIntVariable("nBJet30_MV2c10_FixedCutBEff_85",0.0);
  configMgr->treeMaker->addIntVariable("nBJet20_MV2c10_FixedCutBEff_60",0.0);
  configMgr->treeMaker->addIntVariable("nBJet20_MV2c10_FixedCutBEff_70",0.0);
  configMgr->treeMaker->addIntVariable("nBJet20_MV2c10_FixedCutBEff_77",0.0);
  configMgr->treeMaker->addIntVariable("nBJet20_MV2c10_FixedCutBEff_85",0.0);
  configMgr->treeMaker->addIntVariable("DecayModeTTbar",-1);
  
  configMgr->treeMaker->addVecFloatVariable("jetPt");
  configMgr->treeMaker->addVecFloatVariable("jetEta");
  configMgr->treeMaker->addVecFloatVariable("jetPhi");
  configMgr->treeMaker->addVecFloatVariable("jetM");

  // For L1Calo timing issue check:
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SusyBgForumRun2CheckList#Standard_diagnostic_plots_checks
  // Note that this var is only saved in the detailed skims, which we're only producing for data currently!
  configMgr->treeMaker->addVecFloatVariable("jetTileEnergy");

  configMgr->treeMaker->addFloatVariable("vectorSumJetsPt", 0.);
  configMgr->treeMaker->addFloatVariable("vectorSumJetsEta", 0.);
  configMgr->treeMaker->addFloatVariable("vectorSumJetsPhi", 0.);
  configMgr->treeMaker->addFloatVariable("vectorSumJetsM", 0.);
  configMgr->treeMaker->addFloatVariable("dPhiVectorSumJetsMET", 0.);

  // MET
  configMgr->treeMaker->addFloatVariable("met_Et",0.0);
  configMgr->treeMaker->addFloatVariable("met_Phi",0.0);
  configMgr->treeMaker->addFloatVariable("TST_Et",0.0);
  configMgr->treeMaker->addFloatVariable("TST_Phi",0.0);
  configMgr->treeMaker->addFloatVariable("met_track_Et",0.0);
  configMgr->treeMaker->addFloatVariable("met_track_Phi",0.0);
  configMgr->treeMaker->addFloatVariable("deltaPhi_MET_TST_Phi", 0.0);
  
  // Analysis variables
  configMgr->treeMaker->addFloatVariable("mt",0.0);
  configMgr->treeMaker->addFloatVariable("meffInc30",0.0);
  configMgr->treeMaker->addFloatVariable("Ht30",0.0);
  configMgr->treeMaker->addFloatVariable("hadronicWMass",0.0);
  configMgr->treeMaker->addFloatVariable("hadronicWPt",0.0);
  
  configMgr->treeMaker->addFloatVariable("LepAplanarity",0.0);
  configMgr->treeMaker->addFloatVariable("JetAplanarity",0.0);
  
  configMgr->treeMaker->addFloatVariable("amt2",0.0);
  configMgr->treeMaker->addFloatVariable("amt2b",0.0);
  configMgr->treeMaker->addFloatVariable("amt2bWeight",0.0);
  configMgr->treeMaker->addFloatVariable("mt2tau",0.0);
  configMgr->treeMaker->addFloatVariable("mt2lj",0.0);
  //configMgr->treeMaker->addFloatVariable("topNess",0.0);
  configMgr->treeMaker->addFloatVariable("mbb",-1.0);

  // other analysis variables
  configMgr->treeMaker->addFloatVariable("mt_lep1",0.0);
  configMgr->treeMaker->addFloatVariable("mt_lep2",0.0);
  configMgr->treeMaker->addFloatVariable("mt_lep3",0.0);
  configMgr->treeMaker->addFloatVariable("mt_lep4",0.0);
  configMgr->treeMaker->addFloatVariable("mt_lep1_metTrack",0.0);
  configMgr->treeMaker->addFloatVariable("mt_lep2_metTrack",0.0);
  configMgr->treeMaker->addFloatVariable("mt_lep3_metTrack",0.0);
  configMgr->treeMaker->addFloatVariable("mt_lep4_metTrack",0.0);
  configMgr->treeMaker->addFloatVariable("METOverHT",-1.);
  configMgr->treeMaker->addFloatVariable("METOverJ1pT",-1.);
  configMgr->treeMaker->addFloatVariable("METTrackOverHT",-1.);
  configMgr->treeMaker->addFloatVariable("METTrackOverJ1pT",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ1Met",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ2Met",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ3Met",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ4Met",-1.);
  configMgr->treeMaker->addFloatVariable("minDPhi4JetsMet",-1.);
  configMgr->treeMaker->addFloatVariable("minDPhiAllJetsMet",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ1MetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ2MetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ3MetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("DPhiJ4MetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("minDPhi4JetsMetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("minDPhiAllJetsMetTrack",-1.);

  // 2L Analysis Variables
  configMgr->treeMaker->addFloatVariable("METOverHTLep",-1.);
  configMgr->treeMaker->addFloatVariable("METTrackOverHTLep",-1.);
  configMgr->treeMaker->addFloatVariable("mll",-1.);
  configMgr->treeMaker->addFloatVariable("Rll",-1.);
  configMgr->treeMaker->addFloatVariable("Ptll",-1.);
  configMgr->treeMaker->addFloatVariable("dPhiPllMet",-1.);
  configMgr->treeMaker->addFloatVariable("dPhiPllMetTrack",-1.);

  configMgr->treeMaker->addFloatVariable("METRel",-1.);
  configMgr->treeMaker->addFloatVariable("METTrackRel",-1.);
  configMgr->treeMaker->addFloatVariable("dPhiNearMet",-1.);
  configMgr->treeMaker->addFloatVariable("dPhiNearMetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("dPhiMetAndMetTrack",-1.);

  configMgr->treeMaker->addFloatVariable("MSqTauTau_1",-99.); 
  configMgr->treeMaker->addFloatVariable("MSqTauTau_2",-99999.);
  configMgr->treeMaker->addFloatVariable("MTauTau",-99999.);
  configMgr->treeMaker->addFloatVariable("MSqTauTau_1_metTrack", -99.);
  configMgr->treeMaker->addFloatVariable("MSqTauTau_2_metTrack", -99999.);
  configMgr->treeMaker->addFloatVariable("MTauTau_metTrack", -99999.);
  configMgr->treeMaker->addFloatVariable("RjlOverEl",-1.);
  configMgr->treeMaker->addFloatVariable("LepCosThetaLab",-1.);
  configMgr->treeMaker->addFloatVariable("LepCosThetaCoM",-1.);
  
  // mt2(L1,L2,MET) with various trial invisible particle masses
  configMgr->treeMaker->addFloatVariable("mt2leplsp_0", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_25", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_50", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_75", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_90", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_95", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_100", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_105", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_110", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_115", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_120", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_150", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_175", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_200", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_250", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_300", -1.);

  configMgr->treeMaker->addFloatVariable("mt2leplsp_0_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_25_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_50_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_75_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_90_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_95_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_100_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_105_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_110_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_115_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_120_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_150_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_175_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_200_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_250_metTrack", -1.);
  configMgr->treeMaker->addFloatVariable("mt2leplsp_300_metTrack", -1.);

  // Weights
  configMgr->treeMaker->addDoubleVariable("pileupWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("leptonWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("eventWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("genWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("bTagWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("jvtWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("genWeightUp",1.0);
  configMgr->treeMaker->addDoubleVariable("genWeightDown",1.0);

  // For Wino-Bino reweighting
  configMgr->treeMaker->addDoubleVariable("truthMll",-999);
  configMgr->treeMaker->addDoubleVariable("winoBinoMllWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("winoBinoXsecWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("winoBinoBrFracWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("winoBinoWeight",1.0);

  //NUHM2
  configMgr->treeMaker->addDoubleVariable("NUHM2weight_350m12",1.0);
  configMgr->treeMaker->addDoubleVariable("NUHM2weight_400m12",1.0);
  configMgr->treeMaker->addDoubleVariable("NUHM2weight_500m12",1.0);
  configMgr->treeMaker->addDoubleVariable("NUHM2weight_600m12",1.0);
  configMgr->treeMaker->addDoubleVariable("NUHM2weight_700m12",1.0);
  configMgr->treeMaker->addDoubleVariable("NUHM2weight_800m12",1.0);

  // LHE3 weights for theory systematics
  configMgr->treeMaker->addVecFloatVariable("LHE3Weights");

  // For use in FF estimate (which is a post-processing step, so we
  // only create these branches here and give them dummy values)
  configMgr->treeMaker->addDoubleVariable("FFWeight",1.0);
  configMgr->treeMaker->addIntVariable("nLep_antiID",0);
  configMgr->treeMaker->addBoolVariable("lep1AntiID",false);
  configMgr->treeMaker->addBoolVariable("lep2AntiID",false);
  configMgr->treeMaker->addBoolVariable("lep3AntiID",false);
  configMgr->treeMaker->addBoolVariable("lep4AntiID",false);
  configMgr->treeMaker->addIntVariable("nLep_signalActual",0);
  configMgr->treeMaker->addBoolVariable("lep1SignalActual",false);
  configMgr->treeMaker->addBoolVariable("lep2SignalActual",false);
  configMgr->treeMaker->addBoolVariable("lep3SignalActual",false);
  configMgr->treeMaker->addBoolVariable("lep4SignalActual",false);

  // Write these trigger chains
  // The list doesn't include single lepton triggers - they are already defined by addTriggerAna
  this->triggerChains =  {
    "HLT_mu4",
    "HLT_2mu4",
    "HLT_2mu10",
    "HLT_2mu4_j85_xe50_mht",
    "HLT_mu4_j125_xe90_mht",
    "HLT_xe70",
    "HLT_xe70_mht",
    "HLT_xe70_mht_wEFMu",
    "HLT_xe70_tc_lcw",
    "HLT_xe70_tc_lcw_wEFMu",
    "HLT_xe80_tc_lcw_L1XE50",
    "HLT_xe90_tc_lcw_L1XE50",
    "HLT_xe90_mht_L1XE50",
    "HLT_xe90_tc_lcw_wEFMu_L1XE50",
    "HLT_xe90_mht_wEFMu_L1XE50",
    "HLT_xe100_L1XE50",
    "HLT_xe100_wEFMu_L1XE50",
    "HLT_xe100_tc_lcw_L1XE50",
    "HLT_xe100_mht_L1XE50",
    "HLT_xe100_tc_lcw_wEFMu_L1XE50",
    "HLT_xe100_mht_wEFMu_L1XE50",
    "HLT_xe110_L1XE50",
    "HLT_xe110_tc_em_L1XE50",
    "HLT_xe110_wEFMu_L1XE50",
    "HLT_xe110_tc_em_wEFMu_L1XE50",
    "HLT_xe110_mht_L1XE50", // emulated in SUSYTools
    "HLT_xe90_mht_L1XE40", // emulated in TriggerTools
    "HLT_xe50_mht_L1XE20", //emulated in TriggerTools
    "HLT_j85_L1J40", //emulated in TriggerTools
    "HLT_j125_L1J50", //emulated in TriggerTools
    // single lepton for supporting studies
    "HLT_e26_lhtight_nod0_ivarloose",
    "HLT_e60_lhmedium_nod0",
    "HLT_e60_medium",
    "HLT_e140_lhloose_nod0",
    "HLT_mu26_ivarmedium",
    "HLT_e5_lhvloose_nod0",
    "HLT_2e5_lhvloose_nod0",
    "HLT_e5_lhloose_nod0",
    "HLT_2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30",
    "HLT_2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30",
    "HLT_2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE40_DPHI-J20s2XE30",
    "HLT_2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J40_XE50",
    "HLT_2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J40_XE50",
    "HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30",
    "HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_XE60",
    "HLT_mu4_j80_xe80_pufit_2dphi10_L1MU4_XE60",
    "HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50",
    "HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1XE60",
    "HLT_2e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1XE60",
    "HLT_2e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50",
    "HLT_e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30",
    "HLT_e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50",
    "HLT_e5_lhloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30",
    "HLT_e5_lhmedium_nod0_j50_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50",
    "HLT_e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_XE60",
    "HLT_e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1XE60",
    "HLT_e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1XE60",
    "HLT_e5_lhmedium_nod0_j50_xe90_pufit_2dphi10_L1J40_XE50_DPHI_J20s2XE50",
    "HLT_mu4_j100_xe60mht_L1MU4_J20_XE30_DPHI-J20sXE30",
    "HLT_mu4_xe60mht_L1MU4_J20_XE30_DPHI-J20sXE30",
    "HLT_2mu4_xe40mht_L12MU4_J20_XE30_DPHI-J20sXE30",
    "HLT_e5_lhmedium_nod0_mu4_xe40_mht_L1MU4_J20_XE30_DPHI-J20sXE30",
    "HLT_2e5_lhmedium_nod0_j50_xe80_mht_L1J40_XE50_DPHI-J20sXE50",
  };
  for (const char* chainName : this->triggerChains) {
    configMgr->treeMaker->addBoolVariable(chainName, false);
  }

  // Truth information for ttbar reweighting
  configMgr->treeMaker->addDoubleVariable("ttbarNNLOWeight",1.0);
  configMgr->treeMaker->addDoubleVariable("ttbarNNLOWeightUp",1.0);
  configMgr->treeMaker->addDoubleVariable("ttbarNNLOWeightDown",1.0);
  configMgr->treeMaker->addFloatVariable("truthTopPt",0.0);
  configMgr->treeMaker->addFloatVariable("truthAntiTopPt",0.0);
  configMgr->treeMaker->addFloatVariable("truthTtbarPt",0.0);
  configMgr->treeMaker->addFloatVariable("truthTtbarM",0.0);

  // PDF variables
  configMgr->treeMaker->addFloatVariable("x1", -1.0);
  configMgr->treeMaker->addFloatVariable("x2", -1.0);
  configMgr->treeMaker->addFloatVariable("pdf1", -1.0);
  configMgr->treeMaker->addFloatVariable("pdf2", -1.0);
  configMgr->treeMaker->addFloatVariable("scalePDF", -1.0);
  configMgr->treeMaker->addIntVariable("id1", 0);
  configMgr->treeMaker->addIntVariable("id2", 0);

  // Set lumi:
  // We want to set the lumis proportionally
  // dependent on year. i.e. set 2015+2016 to 36100/80000, and similar
  // for 2017. Then when we add the samples together, the total lumi weight
  // will be 1 pb-1, which we can then scale up
  //
  // 2015 in R21: 3219.56
  // 2016 in R21: 32988.1
  // 2017 in R21: 43813.7
  //
  if(configMgr->obj->evt.evtNumber < 320000){
    // 2015+2016 lumi / total lumi
    configMgr->objectTools->setLumi(0.452475);
  }
  else{
    // 2017 lumi / total lumi
    configMgr->objectTools->setLumi(0.547525);
  }

  // Object class contains the definitions of all physics objects, eg muons, electrons, jets
  // See xAODNtupleMaker::Objects for available methods; configMgr->obj

}
// ------------------------------------------------------------------------------------------ //
bool twoleptonSelector::doAnalysis(ConfigMgr*& configMgr)
{
  /*
    This is the main method, which is called for each event
  */

  // Skims events by imposing any cuts you define in this method below
  if( !passCuts(configMgr) ) return false;

  //Construct emulated soft muon + jet + met triggers
  bool HLT_2mu4_j85_xe50_mht_emul = 0;
  bool HLT_mu4_j125_xe90_mht_emul = 0;

  if(!configMgr->obj->evt.isData()){
    HLT_2mu4_j85_xe50_mht_emul = configMgr->obj->evt.getHLTEvtTrigDec("HLT_2mu4") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_j85_L1J40") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe50_mht_L1XE20");
    HLT_mu4_j125_xe90_mht_emul = configMgr->obj->evt.getHLTEvtTrigDec("HLT_mu4") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_j125_L1J50") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe90_mht_L1XE40");
  }

  bool useBaseline = true; // to calculate our observables using baseline leptons rather than signal leptons
  bool useTrackMET = true; // for calculating only SOME variables using track-based MET
  int nLep_base = configMgr->obj->baseLeptons.size();
 
  // We requested at least two baseline leptons in the skim
  const LeptonVariable* lep1 = configMgr->obj->baseLeptons[0];
  const LeptonVariable* lep2 = configMgr->obj->baseLeptons[1];
  const LeptonVariable* lep3 = nLep_base < 3 ? new LeptonVariable() : (configMgr->obj->baseLeptons[2]->Pt() > 20 ? configMgr->obj->baseLeptons[2] : new LeptonVariable());
  const LeptonVariable* lep4 = nLep_base < 4 ? new LeptonVariable() : (configMgr->obj->baseLeptons[3]->Pt() > 20 ? configMgr->obj->baseLeptons[3] : new LeptonVariable());
;

  // Truth information for ttbar
  int intTTbarDecayMode = -1;
  if( configMgr->obj->truthEvent.TTbarTLVs.size()>0 ){

    // Decay mode
    TruthEvent::TTbarDecayMode decayMode = configMgr->obj->truthEvent.decayMode;
    intTTbarDecayMode = (int)decayMode;

    const TLorentzVector* top     = configMgr->obj->truthEvent.getTTbarTLV("top");
    const TLorentzVector* antitop = configMgr->obj->truthEvent.getTTbarTLV("antitop");

    // Top kinematics
    configMgr->treeMaker->setFloatVariable("truthTopPt", top->Pt() );
    configMgr->treeMaker->setFloatVariable("truthAntiTopPt", antitop->Pt() );
    configMgr->treeMaker->setFloatVariable("truthTtbarPt", (*top+*antitop).Pt() );
    configMgr->treeMaker->setFloatVariable("truthTtbarM", (*top+*antitop).M() );

    float avgPt = (top->Pt() + antitop->Pt() ) / 2;
    
    float NNLOWeight = configMgr->objectTools->getTtbarNNLOWeight( avgPt,configMgr->obj->evt.dsid,true);

    configMgr->treeMaker->setDoubleVariable("ttbarNNLOWeight", NNLOWeight );
    configMgr->treeMaker->setDoubleVariable("ttbarNNLOWeightUp", NNLOWeight );
    configMgr->treeMaker->setDoubleVariable("ttbarNNLOWeightDown", NNLOWeight );
    
  }

  // Hadronic mass and boost
  float wPt = 0.0;
  float wMass = getHadWMass( configMgr->obj, wPt );
  
  // Topness: currently only defined for one lepton! Can think about modifying in the future
  //float topNess = getTopness(configMgr->obj);

  // aMT2 variables
  float trial_invisible_mass = 0; // for the following calculations, set to 0 by default
  float amt2        = getMt2(configMgr->obj, AMT2, trial_invisible_mass, useBaseline);
  float amt2b       = getMt2(configMgr->obj, AMT2B, trial_invisible_mass, useBaseline);
  float amt2bWeight = getMt2(configMgr->obj, AMT2BWEIGHT, trial_invisible_mass, useBaseline);
  float mt2tau      = getMt2(configMgr->obj, MT2TAU, trial_invisible_mass, useBaseline);
  float mt2lj       = getMt2(configMgr->obj, MT2LJ, trial_invisible_mass, useBaseline);

  // Inclusive Meff, all jets with pT>30 GeV
  float meffInc30 = getMeff( configMgr->obj, configMgr->obj->cJets.size(), 30.0 );
  
  // Ht: Scalar sum of jets
  float Ht30 = getHt( configMgr->obj, configMgr->obj->cJets.size(), 30.0 );
  
  // Aplanarity
  float JetAplanarity = getAplanarity( configMgr->obj,configMgr->obj->cJets.size(),0.0, 30.0, useBaseline );
  float LepAplanarity = getAplanarity( configMgr->obj,configMgr->obj->cJets.size(),configMgr->obj->signalLeptons.size(), 30.0, useBaseline );
  
  // Jet multiplicity and kinematics
  int nJet30  = getNJets( configMgr->obj, 30.0 );

  std::vector<float> jetPtVec;
  std::vector<float> jetEtaVec;
  std::vector<float> jetPhiVec;
  std::vector<float> jetMVec;
  std::vector<float> jetTileEnergy;

  TLorentzVector vectorSumJets;

  for(JetVariable* jet : configMgr->obj->cJets){
    jetPtVec.push_back(jet->Pt());
    jetEtaVec.push_back(jet->Eta());
    jetPhiVec.push_back(jet->Phi());
    jetMVec.push_back(jet->M());

    // For background forum checklist
    jetTileEnergy.push_back(jet->tileEnergy);

    // vectorial sum of jet momenta
    TLorentzVector jetTemp;
    jetTemp.SetPtEtaPhiM(jet->Pt(),jet->Eta(),jet->Phi(),jet->M()); 
    vectorSumJets = vectorSumJets + jetTemp;
  }

  float dPhiVectorSumJetsMET = fabs( vectorSumJets.DeltaPhi(configMgr->obj->met) );
  float dPhiVectorSumJetsMETTrack = fabs( vectorSumJets.DeltaPhi(configMgr->obj->met.getTrackTLV()) );

  // B-jets
  int nBJet30 = getNBJets( configMgr->obj, 30.0 );
  int nBJet20 = getNBJets( configMgr->obj, 20.0 );
  
  int nBJet30_FixedCutBEff_60=0;
  int nBJet30_FixedCutBEff_70=0;
  int nBJet30_FixedCutBEff_77=0;
  int nBJet30_FixedCutBEff_85=0;

  int nBJet20_FixedCutBEff_60=0;
  int nBJet20_FixedCutBEff_70=0;
  int nBJet20_FixedCutBEff_77=0;
  int nBJet20_FixedCutBEff_85=0;

  for( auto& jet : configMgr->obj->cJets ){

    // The pT cut is redundant, and possibly the eta cut is built into the
    // mv2c10 definition, but better to keep them both
    if( fabs(jet->Eta()) > 2.5 || jet->Pt()<20.0 ) continue;

    // Different WPs, based on:
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarksRelease21#MV2c10_tagger
    if( jet->mv2c10 >= 0.94 ) nBJet20_FixedCutBEff_60++;
    if( jet->mv2c10 >= 0.83 ) nBJet20_FixedCutBEff_70++;
    if( jet->mv2c10 >= 0.64 ) nBJet20_FixedCutBEff_77++;
    if( jet->mv2c10 >= 0.11 ) nBJet20_FixedCutBEff_85++;

    if(jet->Pt() < 30.0) continue;
    if( jet->mv2c10 >= 0.94 ) nBJet30_FixedCutBEff_60++;
    if( jet->mv2c10 >= 0.83 ) nBJet30_FixedCutBEff_70++;
    if( jet->mv2c10 >= 0.64 ) nBJet30_FixedCutBEff_77++;
    if( jet->mv2c10 >= 0.11 ) nBJet30_FixedCutBEff_85++;

  }
 
  // Leading 2 B-jet invariant mass
  float mbb = -1.0;

  if (configMgr->obj->bJets.size()>=2) {
    mbb = (*(configMgr->obj->bJets[0])+*(configMgr->obj->bJets[1])).M();
  }

  // MET
  float met   = configMgr->obj->met.Et;

  // Track-based MET
  float metTrack   = configMgr->obj->met.Et_track;

  // Other analysis variables
  float mt_lep1  = getMt(configMgr->obj, useBaseline, 0); // leading lepton
  float mt_lep2  = getMt(configMgr->obj, useBaseline, 1); // subleading lepton
  float mt_lep3  = nLep_base < 3 ? -999 : getMt(configMgr->obj, useBaseline, 2); // 3rd lepton
  float mt_lep4  = nLep_base < 4 ? -999 : getMt(configMgr->obj, useBaseline, 3); // 4th lepton

  float mt_lep1_metTrack  = getMt(configMgr->obj, useBaseline, 0, useTrackMET); // leading lepton
  float mt_lep2_metTrack  = getMt(configMgr->obj, useBaseline, 1, useTrackMET); // subleading lepton
  float mt_lep3_metTrack  = nLep_base < 3 ? -999 : getMt(configMgr->obj, useBaseline, 2, useTrackMET); // 3rd lepton
  float mt_lep4_metTrack  = nLep_base < 4 ? -999 : getMt(configMgr->obj, useBaseline, 3, useTrackMET); // 4th lepton

  float METOverHT      = met / Ht30;
  float METTrackOverHT = metTrack / Ht30;

  float METOverJ1pT = configMgr->obj->cJets.size() < 1 ? -999 : met / configMgr->obj->cJets[0]->Pt();
  float METTrackOverJ1pT = configMgr->obj->cJets.size() < 1 ? -999 : metTrack / configMgr->obj->cJets[0]->Pt();

  float HTLep = 0;
  for (LeptonVariable* lep: configMgr->obj->baseLeptons){
    HTLep += lep->Pt();
  }
  float METOverHTLep  = HTLep > 0 ? met / HTLep : -999;
  float METTrackOverHTLep  = HTLep > 0 ? metTrack / HTLep : -999;
  
  float DPhiJ1Met       = getDPhiJNMet(configMgr->obj, 0, 30.0);
  float DPhiJ2Met       = getDPhiJNMet(configMgr->obj, 1, 30.0);
  float DPhiJ3Met       = getDPhiJNMet(configMgr->obj, 2, 30.0);
  float DPhiJ4Met       = getDPhiJNMet(configMgr->obj, 3, 30.0);
  float minDPhi4JetsMet = std::min(DPhiJ1Met, std::min(DPhiJ2Met, std::min(DPhiJ3Met, DPhiJ4Met) ) );

  float DPhiJ1MetTrack       = getDPhiJNMet(configMgr->obj, 0, 30.0, useTrackMET);
  float DPhiJ2MetTrack       = getDPhiJNMet(configMgr->obj, 1, 30.0, useTrackMET);
  float DPhiJ3MetTrack       = getDPhiJNMet(configMgr->obj, 2, 30.0, useTrackMET);
  float DPhiJ4MetTrack       = getDPhiJNMet(configMgr->obj, 3, 30.0, useTrackMET);
  float minDPhi4JetsMetTrack = std::min(DPhiJ1MetTrack, std::min(DPhiJ2MetTrack, std::min(DPhiJ3MetTrack, DPhiJ4MetTrack) ) );

  float minDPhiAllJetsMet = 99999;
  float minDPhiAllJetsMetTrack = 99999;
  for(unsigned int jetIndex = 0; jetIndex < configMgr->obj->cJets.size(); ++jetIndex){
    minDPhiAllJetsMet = std::min(minDPhiAllJetsMet, getDPhiJNMet(configMgr->obj, jetIndex, 30.0));
    minDPhiAllJetsMetTrack = std::min(minDPhiAllJetsMetTrack, getDPhiJNMet(configMgr->obj, jetIndex, 30.0, useTrackMET));
  }

  // 2L Analysis variables
  float mll           = getMll(configMgr->obj, useBaseline);
  float Rll           = getRll(configMgr->obj); 
  float Ptll          = (*lep1 + *lep2).Pt();

  float dPhiPllMet      = (*lep1 + *lep2).DeltaPhi(configMgr->obj->met);
  float dPhiPllMetTrack = (*lep1 + *lep2).DeltaPhi(configMgr->obj->met.getTrackTLV());

  // METRel
  float dPhiNearMet = 99999;
  float dPhiNearMetTrack = 99999;

  // FIXME Ideally just rely on the definition inside of the Observables class,
  // but right now our cJet pT definition differs from what we use for our
  // signal jets, so it won't quite work yet
  for(LeptonVariable* lep : configMgr->obj->baseLeptons){
    float tmp1 = fabs(lep->DeltaPhi(configMgr->obj->met));
    if( tmp1 < dPhiNearMet ) dPhiNearMet = tmp1;

    float tmp2 = fabs(lep->DeltaPhi(configMgr->obj->met.getTrackTLV()));
    if( tmp2 < dPhiNearMetTrack ) dPhiNearMetTrack = tmp2;
  }
  for(JetVariable* jet : configMgr->obj->cJets){
    
    // Only use signal jets
    if( fabs(jet->Eta()) > 2.8 || jet->Pt() < 30.0 ) continue;

    float tmp1 = fabs(jet->DeltaPhi(configMgr->obj->met));
    if( tmp1 < dPhiNearMet ) dPhiNearMet = tmp1;

    float tmp2 = fabs(jet->DeltaPhi(configMgr->obj->met.getTrackTLV()));
    if( tmp2 < dPhiNearMetTrack ) dPhiNearMetTrack = tmp2;
  }

  float METRel = met;
  if(dPhiNearMet < M_PI/2) METRel = met * TMath::Sin(dPhiNearMet);

  float METTrackRel = metTrack;
  if(dPhiNearMetTrack < M_PI/2) METTrackRel = metTrack * TMath::Sin(dPhiNearMetTrack);

  
  // dPhi between our two MET definitions
  float dPhiMetAndMetTrack = configMgr->obj->met.DeltaPhi(configMgr->obj->met.getTrackTLV());

  float RjlOverEl     = getRjlOverEl(configMgr->obj);

  // MTauTau definitions
  float MSqTauTau_1   = getMSqTauTau(configMgr->obj, 1); // version 1 : positive definite
  float MSqTauTau_2   = getMSqTauTau(configMgr->obj, 2); // version 2 : more natural definition
  float MSqTauTau_1_metTrack   = getMSqTauTau(configMgr->obj, 1, useTrackMET); // version 1 : positive definite
  float MSqTauTau_2_metTrack   = getMSqTauTau(configMgr->obj, 2, useTrackMET); // version 2 : more natural definition
  
  // Take signed square root of MSqTauTau_2
  float MTauTau       = -99999.;
  if (MSqTauTau_2 >= 0.) MTauTau =  sqrt( MSqTauTau_2 ); 
  if (MSqTauTau_2 < 0.)  MTauTau = -sqrt( fabs( MSqTauTau_2 ) );

  // Take signed square root of MSqTauTau_2
  float MTauTau_metTrack       = -99999.;
  if (MSqTauTau_2_metTrack >= 0.) MTauTau_metTrack =  sqrt( MSqTauTau_2_metTrack ); 
  if (MSqTauTau_2_metTrack < 0.)  MTauTau_metTrack = -sqrt( fabs( MSqTauTau_2_metTrack ) );
   
  // lepton-beam polar angle
  float LepCosThetaLab  = getLepCosThetaLab(configMgr->obj);
  float LepCosThetaCoM  = getLepCosThetaCoM(configMgr->obj);


  // mt2(L1,L2,MET) with various trial invisible particle masses
  float mt2leplsp_0   = getMt2(configMgr->obj, MT2LL, 0.,   useBaseline);
  float mt2leplsp_25  = getMt2(configMgr->obj, MT2LL, 25.,  useBaseline);
  float mt2leplsp_50  = getMt2(configMgr->obj, MT2LL, 50.,  useBaseline);
  float mt2leplsp_75  = getMt2(configMgr->obj, MT2LL, 75.,  useBaseline);
  float mt2leplsp_90  = getMt2(configMgr->obj, MT2LL, 90.,  useBaseline);
  float mt2leplsp_95  = getMt2(configMgr->obj, MT2LL, 95.,  useBaseline);
  float mt2leplsp_100 = getMt2(configMgr->obj, MT2LL, 100., useBaseline);
  float mt2leplsp_105 = getMt2(configMgr->obj, MT2LL, 105., useBaseline);
  float mt2leplsp_110 = getMt2(configMgr->obj, MT2LL, 110., useBaseline);
  float mt2leplsp_115 = getMt2(configMgr->obj, MT2LL, 115., useBaseline);
  float mt2leplsp_120 = getMt2(configMgr->obj, MT2LL, 120., useBaseline);
  float mt2leplsp_150 = getMt2(configMgr->obj, MT2LL, 150., useBaseline);
  float mt2leplsp_175 = getMt2(configMgr->obj, MT2LL, 175., useBaseline);
  float mt2leplsp_200 = getMt2(configMgr->obj, MT2LL, 200., useBaseline);
  float mt2leplsp_250 = getMt2(configMgr->obj, MT2LL, 250., useBaseline);
  float mt2leplsp_300 = getMt2(configMgr->obj, MT2LL, 300., useBaseline);

  float mt2leplsp_0_metTrack   = getMt2(configMgr->obj, MT2LL, 0.,   useBaseline, useTrackMET);
  float mt2leplsp_25_metTrack  = getMt2(configMgr->obj, MT2LL, 25.,  useBaseline, useTrackMET);
  float mt2leplsp_50_metTrack  = getMt2(configMgr->obj, MT2LL, 50.,  useBaseline, useTrackMET);
  float mt2leplsp_75_metTrack  = getMt2(configMgr->obj, MT2LL, 75.,  useBaseline, useTrackMET);
  float mt2leplsp_90_metTrack  = getMt2(configMgr->obj, MT2LL, 90.,  useBaseline, useTrackMET);
  float mt2leplsp_95_metTrack  = getMt2(configMgr->obj, MT2LL, 95.,  useBaseline, useTrackMET);
  float mt2leplsp_100_metTrack = getMt2(configMgr->obj, MT2LL, 100., useBaseline, useTrackMET);
  float mt2leplsp_105_metTrack = getMt2(configMgr->obj, MT2LL, 105., useBaseline, useTrackMET);
  float mt2leplsp_110_metTrack = getMt2(configMgr->obj, MT2LL, 110., useBaseline, useTrackMET);
  float mt2leplsp_115_metTrack = getMt2(configMgr->obj, MT2LL, 115., useBaseline, useTrackMET);
  float mt2leplsp_120_metTrack = getMt2(configMgr->obj, MT2LL, 120., useBaseline, useTrackMET);
  float mt2leplsp_150_metTrack = getMt2(configMgr->obj, MT2LL, 150., useBaseline, useTrackMET);
  float mt2leplsp_175_metTrack = getMt2(configMgr->obj, MT2LL, 175., useBaseline, useTrackMET);
  float mt2leplsp_200_metTrack = getMt2(configMgr->obj, MT2LL, 200., useBaseline, useTrackMET);
  float mt2leplsp_250_metTrack = getMt2(configMgr->obj, MT2LL, 250., useBaseline, useTrackMET);
  float mt2leplsp_300_metTrack = getMt2(configMgr->obj, MT2LL, 300., useBaseline, useTrackMET);

  //
  // -------- Fill the output tree variables ---------- //
  //

  //Emulated Triggers
  configMgr->treeMaker->setBoolVariable("HLT_2mu4_j85_xe50_mht_emul", HLT_2mu4_j85_xe50_mht_emul);
  configMgr->treeMaker->setBoolVariable("HLT_mu4_j125_xe90_mht_emul", HLT_mu4_j125_xe90_mht_emul);
  emulateL1TopoTriggers(configMgr);

  // Global Vars
  configMgr->treeMaker->setFloatVariable("mu",configMgr->obj->evt.mu);
  configMgr->treeMaker->setIntVariable("nVtx",configMgr->obj->evt.nVtx);

  // Leptons
  configMgr->treeMaker->setIntVariable("nLep_base", nLep_base);
  configMgr->treeMaker->setIntVariable("nLep_signal",configMgr->obj->signalLeptons.size());

  int lep1Flavor = 0;
  int lep1EgMotherType = -1;
  int lep1EgMotherOrigin = -1;
  int lep1NPix = -1;
  bool lep1PassBL = false;
  bool lep1TruthMatched = false;

  int lep2Flavor = 0;
  int lep2EgMotherType = -1;
  int lep2EgMotherOrigin = -1;
  int lep2NPix = -1;
  bool lep2PassBL = false;
  bool lep2TruthMatched = false;

  int lep3Flavor = 0;
  int lep3EgMotherType = -1;
  int lep3EgMotherOrigin = -1;
  int lep3NPix = -1;
  bool lep3PassBL = false;
  bool lep3TruthMatched = false;

  int lep4Flavor = 0;
  int lep4EgMotherType = -1;
  int lep4EgMotherOrigin = -1;
  int lep4NPix = -1;
  bool lep4PassBL = false;
  bool lep4TruthMatched = false;

  if(lep1->isEle()){
    const ElectronVariable* el = dynamic_cast<const ElectronVariable*>(lep1);
    lep1Flavor         = 1;
    lep1PassBL         = el->passBL;
    lep1NPix           = el->nPixHitsPlusDeadSensors;
    lep1TruthMatched   = el->type == 2 || (el->type == 4 && el->egMotherType == 2);
    lep1EgMotherType   = el->egMotherType;
    lep1EgMotherOrigin = el->egMotherOrigin;
  }
  else if(lep1->isMu()){
    lep1Flavor = 2;
    lep1TruthMatched = lep1->type == 6;
  }

  if(lep2->isEle()){
    const ElectronVariable* el = dynamic_cast<const ElectronVariable*>(lep2);
    lep2Flavor         = 1;
    lep2PassBL         = el->passBL;
    lep2NPix           = el->nPixHitsPlusDeadSensors;
    lep2TruthMatched   = el->type == 2 || (el->type == 4 && el->egMotherType == 2);
    lep2EgMotherType   = el->egMotherType;
    lep2EgMotherOrigin = el->egMotherOrigin;
  }
  else if(lep2->isMu()){
    lep2Flavor = 2;
    lep2TruthMatched = lep2->type == 6;
  }

  if(lep3->isEle()){
    const ElectronVariable* el = dynamic_cast<const ElectronVariable*>(lep3);
    lep3Flavor         = 1;
    lep3PassBL         = el->passBL;
    lep3NPix           = el->nPixHitsPlusDeadSensors;
    lep3TruthMatched   = el->type == 2 || (el->type == 4 && el->egMotherType == 2);
    lep3EgMotherType   = el->egMotherType;
    lep3EgMotherOrigin = el->egMotherOrigin;
  }
  else if(lep3->isMu()){
    lep3Flavor = 2;
    lep3TruthMatched = lep3->type == 6;
  }

  if(lep4->isEle()){
    const ElectronVariable* el = dynamic_cast<const ElectronVariable*>(lep4);
    lep4Flavor         = 1;
    lep4PassBL         = el->passBL;
    lep4NPix           = el->nPixHitsPlusDeadSensors;
    lep4TruthMatched   = el->type == 2 || (el->type == 4 && el->egMotherType == 2);
    lep4EgMotherType   = el->egMotherType;
    lep4EgMotherOrigin = el->egMotherOrigin;
  }
  else if(lep4->isMu()){
    lep4Flavor = 2;
    lep4TruthMatched = lep4->type == 6;
  }

  // For data, set these to true so that data can be processed
  // with the same cuts as the MC
  if( !configMgr->obj->evt.isMC ){
    lep1TruthMatched = true;
    lep2TruthMatched = true;
    lep3TruthMatched = true;
    lep4TruthMatched = true;
  }

  configMgr->treeMaker->setIntVariable("lep1Flavor",lep1Flavor);
  configMgr->treeMaker->setIntVariable("lep1Charge",lep1->q);
  configMgr->treeMaker->setIntVariable("lep1Author",lep1->author);
  configMgr->treeMaker->setFloatVariable("lep1Pt",lep1->Pt());
  configMgr->treeMaker->setFloatVariable("lep1Eta",lep1->Eta());;
  configMgr->treeMaker->setFloatVariable("lep1Phi",lep1->Phi());
  configMgr->treeMaker->setFloatVariable("lep1M",lep1->M());
  configMgr->treeMaker->setFloatVariable("lep1D0",lep1->d0);
  configMgr->treeMaker->setFloatVariable("lep1D0Sig",fabs(lep1->d0Err) > 0 ? fabs(lep1->d0 / lep1->d0Err) : 999.0);
  configMgr->treeMaker->setFloatVariable("lep1Z0",lep1->z0);
  configMgr->treeMaker->setFloatVariable("lep1Z0SinTheta",fabs(lep1->z0 * TMath::Sin(lep1->Theta())));
  configMgr->treeMaker->setFloatVariable("lep1Topoetcone20",lep1->topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep1Ptvarcone20",lep1->ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep1Ptvarcone30",lep1->ptvarcone30);
  configMgr->treeMaker->setFloatVariable("lep1CorrTopoetcone20",lep1->corr_topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep1CorrPtvarcone20",lep1->corr_ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep1CorrPtvarcone30",lep1->corr_ptvarcone30);
  configMgr->treeMaker->setBoolVariable("lep1PassOR",lep1->passOR);
  configMgr->treeMaker->setIntVariable("lep1Type",lep1->type);
  configMgr->treeMaker->setIntVariable("lep1Origin",lep1->origin);
  configMgr->treeMaker->setIntVariable("lep1EgMotherType",lep1EgMotherType);
  configMgr->treeMaker->setIntVariable("lep1EgMotherOrigin",lep1EgMotherOrigin);
  configMgr->treeMaker->setIntVariable("lep1NPix",lep1NPix);
  configMgr->treeMaker->setBoolVariable("lep1PassBL",lep1PassBL);
  configMgr->treeMaker->setBoolVariable("lep1VeryLoose",lep1->passesVeryLoose());
  configMgr->treeMaker->setBoolVariable("lep1Loose",lep1->passesLoose());
  configMgr->treeMaker->setBoolVariable("lep1Medium",lep1->passesMedium());
  configMgr->treeMaker->setBoolVariable("lep1Tight",lep1->passesTight());
  configMgr->treeMaker->setBoolVariable("lep1IsoLoose",lep1->IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep1IsoTight",lep1->IsoTight);
  configMgr->treeMaker->setBoolVariable("lep1IsoGradient",lep1->IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep1IsoGradientLoose",lep1->IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep1IsoLooseTrackOnly",lep1->IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep1IsoFixedCutLoose",lep1->IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep1IsoFixedCutTight",lep1->IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep1IsoFixedCutTightTrackOnly",lep1->IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrLoose",lep1->corr_IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrTight",lep1->corr_IsoTight);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrGradient",lep1->corr_IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrGradientLoose",lep1->corr_IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrLooseTrackOnly",lep1->corr_IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrFixedCutLoose",lep1->corr_IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrFixedCutTight",lep1->corr_IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep1IsoCorrFixedCutTightTrackOnly",lep1->corr_IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep1Signal",lep1->signal);
  configMgr->treeMaker->setBoolVariable("lep1TruthMatched",lep1TruthMatched);

  configMgr->treeMaker->setIntVariable("lep1TruthCharge",lep1->truthQ);
  configMgr->treeMaker->setFloatVariable("lep1TruthPt",  lep1->truthTLV.Pt());
  configMgr->treeMaker->setFloatVariable("lep1TruthEta", lep1->truthTLV.Eta());;
  configMgr->treeMaker->setFloatVariable("lep1TruthPhi", lep1->truthTLV.Phi());
  configMgr->treeMaker->setFloatVariable("lep1TruthM",   lep1->truthTLV.M());

  configMgr->treeMaker->setIntVariable("lep2Flavor",lep2Flavor);
  configMgr->treeMaker->setIntVariable("lep2Charge",lep2->q);
  configMgr->treeMaker->setIntVariable("lep2Author",lep2->author);
  configMgr->treeMaker->setFloatVariable("lep2Pt",lep2->Pt());
  configMgr->treeMaker->setFloatVariable("lep2Eta",lep2->Eta());;
  configMgr->treeMaker->setFloatVariable("lep2Phi",lep2->Phi());
  configMgr->treeMaker->setFloatVariable("lep2M",lep2->M());
  configMgr->treeMaker->setFloatVariable("lep2D0",lep2->d0);
  configMgr->treeMaker->setFloatVariable("lep2D0Sig",fabs(lep2->d0Err) > 0 ? fabs(lep2->d0 / lep2->d0Err) : 999.0);
  configMgr->treeMaker->setFloatVariable("lep2Z0",lep2->z0);
  configMgr->treeMaker->setFloatVariable("lep2Z0SinTheta",fabs(lep2->z0 * TMath::Sin(lep2->Theta())));
  configMgr->treeMaker->setFloatVariable("lep2Topoetcone20",lep2->topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep2Ptvarcone20",lep2->ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep2Ptvarcone30",lep2->ptvarcone30);
  configMgr->treeMaker->setFloatVariable("lep2CorrTopoetcone20",lep2->corr_topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep2CorrPtvarcone20",lep2->corr_ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep2CorrPtvarcone30",lep2->corr_ptvarcone30);
  configMgr->treeMaker->setBoolVariable("lep2PassOR",lep2->passOR);
  configMgr->treeMaker->setIntVariable("lep2Type",lep2->type);
  configMgr->treeMaker->setIntVariable("lep2Origin",lep2->origin);
  configMgr->treeMaker->setIntVariable("lep2EgMotherType",lep2EgMotherType);
  configMgr->treeMaker->setIntVariable("lep2EgMotherOrigin",lep2EgMotherOrigin);
  configMgr->treeMaker->setIntVariable("lep2NPix",lep2NPix);
  configMgr->treeMaker->setBoolVariable("lep2PassBL",lep2PassBL);
  configMgr->treeMaker->setBoolVariable("lep2VeryLoose",lep2->passesVeryLoose());
  configMgr->treeMaker->setBoolVariable("lep2Loose",lep2->passesLoose());
  configMgr->treeMaker->setBoolVariable("lep2Medium",lep2->passesMedium());
  configMgr->treeMaker->setBoolVariable("lep2Tight",lep2->passesTight());
  configMgr->treeMaker->setBoolVariable("lep2IsoLoose",lep2->IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep2IsoTight",lep2->IsoTight);
  configMgr->treeMaker->setBoolVariable("lep2IsoGradient",lep2->IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep2IsoGradientLoose",lep2->IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep2IsoLooseTrackOnly",lep2->IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep2IsoFixedCutLoose",lep2->IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep2IsoFixedCutTight",lep2->IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep2IsoFixedCutTightTrackOnly",lep2->IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrLoose",lep2->corr_IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrTight",lep2->corr_IsoTight);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrGradient",lep2->corr_IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrGradientLoose",lep2->corr_IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrLooseTrackOnly",lep2->corr_IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrFixedCutLoose",lep2->corr_IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrFixedCutTight",lep2->corr_IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep2IsoCorrFixedCutTightTrackOnly",lep2->corr_IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep2Signal",lep2->signal);
  configMgr->treeMaker->setBoolVariable("lep2TruthMatched",lep2TruthMatched);

  configMgr->treeMaker->setIntVariable("lep2TruthCharge",lep2->truthQ);
  configMgr->treeMaker->setFloatVariable("lep2TruthPt",  lep2->truthTLV.Pt());
  configMgr->treeMaker->setFloatVariable("lep2TruthEta", lep2->truthTLV.Eta());;
  configMgr->treeMaker->setFloatVariable("lep2TruthPhi", lep2->truthTLV.Phi());
  configMgr->treeMaker->setFloatVariable("lep2TruthM",   lep2->truthTLV.M());

  configMgr->treeMaker->setIntVariable("lep3Flavor",lep3Flavor);
  configMgr->treeMaker->setIntVariable("lep3Charge",lep3->q);
  configMgr->treeMaker->setIntVariable("lep3Author",lep3->author);
  configMgr->treeMaker->setFloatVariable("lep3Pt",lep3->Pt());
  configMgr->treeMaker->setFloatVariable("lep3Eta",lep3->Eta());;
  configMgr->treeMaker->setFloatVariable("lep3Phi",lep3->Phi());
  configMgr->treeMaker->setFloatVariable("lep3M",lep3->M());
  configMgr->treeMaker->setFloatVariable("lep3D0",lep3->d0);
  configMgr->treeMaker->setFloatVariable("lep3D0Sig",fabs(lep3->d0Err) > 0 ? fabs(lep3->d0 / lep3->d0Err) : 999.0);
  configMgr->treeMaker->setFloatVariable("lep3Z0",lep3->z0);
  configMgr->treeMaker->setFloatVariable("lep3Z0SinTheta",fabs(lep3->z0 * TMath::Sin(lep3->Theta())));
  configMgr->treeMaker->setFloatVariable("lep3Topoetcone20",lep3->topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep3Ptvarcone20",lep3->ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep3Ptvarcone30",lep3->ptvarcone30);
  configMgr->treeMaker->setFloatVariable("lep3CorrTopoetcone20",lep3->corr_topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep3CorrPtvarcone20",lep3->corr_ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep3CorrPtvarcone30",lep3->corr_ptvarcone30);
  configMgr->treeMaker->setBoolVariable("lep3PassOR",lep3->passOR);
  configMgr->treeMaker->setIntVariable("lep3Type",lep3->type);
  configMgr->treeMaker->setIntVariable("lep3Origin",lep3->origin);
  configMgr->treeMaker->setIntVariable("lep3EgMotherType",lep3EgMotherType);
  configMgr->treeMaker->setIntVariable("lep3EgMotherOrigin",lep3EgMotherOrigin);
  configMgr->treeMaker->setIntVariable("lep3NPix",lep3NPix);
  configMgr->treeMaker->setBoolVariable("lep3PassBL",lep3PassBL);
  configMgr->treeMaker->setBoolVariable("lep3VeryLoose",lep3->passesVeryLoose());
  configMgr->treeMaker->setBoolVariable("lep3Loose",lep3->passesLoose());
  configMgr->treeMaker->setBoolVariable("lep3Medium",lep3->passesMedium());
  configMgr->treeMaker->setBoolVariable("lep3Tight",lep3->passesTight());
  configMgr->treeMaker->setBoolVariable("lep3IsoLoose",lep3->IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep3IsoTight",lep3->IsoTight);
  configMgr->treeMaker->setBoolVariable("lep3IsoGradient",lep3->IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep3IsoGradientLoose",lep3->IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep3IsoLooseTrackOnly",lep3->IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep3IsoFixedCutLoose",lep3->IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep3IsoFixedCutTight",lep3->IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep3IsoFixedCutTightTrackOnly",lep3->IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrLoose",lep3->corr_IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrTight",lep3->corr_IsoTight);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrGradient",lep3->corr_IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrGradientLoose",lep3->corr_IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrLooseTrackOnly",lep3->corr_IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrFixedCutLoose",lep3->corr_IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrFixedCutTight",lep3->corr_IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep3IsoCorrFixedCutTightTrackOnly",lep3->corr_IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep3Signal",lep3->signal);
  configMgr->treeMaker->setBoolVariable("lep3TruthMatched",lep3TruthMatched);

  configMgr->treeMaker->setIntVariable("lep3TruthCharge",lep3->truthQ);
  configMgr->treeMaker->setFloatVariable("lep3TruthPt",  lep3->truthTLV.Pt());
  configMgr->treeMaker->setFloatVariable("lep3TruthEta", lep3->truthTLV.Eta());;
  configMgr->treeMaker->setFloatVariable("lep3TruthPhi", lep3->truthTLV.Phi());
  configMgr->treeMaker->setFloatVariable("lep3TruthM",   lep3->truthTLV.M());

  configMgr->treeMaker->setIntVariable("lep4Flavor",lep4Flavor);
  configMgr->treeMaker->setIntVariable("lep4Charge",lep4->q);
  configMgr->treeMaker->setIntVariable("lep4Author",lep4->author);
  configMgr->treeMaker->setFloatVariable("lep4Pt",lep4->Pt());
  configMgr->treeMaker->setFloatVariable("lep4Eta",lep4->Eta());;
  configMgr->treeMaker->setFloatVariable("lep4Phi",lep4->Phi());
  configMgr->treeMaker->setFloatVariable("lep4M",lep4->M());
  configMgr->treeMaker->setFloatVariable("lep4D0",lep4->d0);
  configMgr->treeMaker->setFloatVariable("lep4D0Sig",fabs(lep4->d0Err) > 0 ? fabs(lep4->d0 / lep4->d0Err) : 999.0);
  configMgr->treeMaker->setFloatVariable("lep4Z0",lep4->z0);
  configMgr->treeMaker->setFloatVariable("lep4Z0SinTheta",fabs(lep4->z0 * TMath::Sin(lep4->Theta())));
  configMgr->treeMaker->setFloatVariable("lep4Topoetcone20",lep4->topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep4Ptvarcone20",lep4->ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep4Ptvarcone30",lep4->ptvarcone30);
  configMgr->treeMaker->setFloatVariable("lep4CorrTopoetcone20",lep4->corr_topoetcone20);
  configMgr->treeMaker->setFloatVariable("lep4CorrPtvarcone20",lep4->corr_ptvarcone20);
  configMgr->treeMaker->setFloatVariable("lep4CorrPtvarcone30",lep4->corr_ptvarcone30);
  configMgr->treeMaker->setBoolVariable("lep4PassOR",lep4->passOR);
  configMgr->treeMaker->setIntVariable("lep4Type",lep4->type);
  configMgr->treeMaker->setIntVariable("lep4Origin",lep4->origin);
  configMgr->treeMaker->setIntVariable("lep4EgMotherType",lep4EgMotherType);
  configMgr->treeMaker->setIntVariable("lep4EgMotherOrigin",lep4EgMotherOrigin);
  configMgr->treeMaker->setIntVariable("lep4NPix",lep4NPix);
  configMgr->treeMaker->setBoolVariable("lep4PassBL",lep4PassBL);
  configMgr->treeMaker->setBoolVariable("lep4VeryLoose",lep4->passesVeryLoose());
  configMgr->treeMaker->setBoolVariable("lep4Loose",lep4->passesLoose());
  configMgr->treeMaker->setBoolVariable("lep4Medium",lep4->passesMedium());
  configMgr->treeMaker->setBoolVariable("lep4Tight",lep4->passesTight());
  configMgr->treeMaker->setBoolVariable("lep4IsoLoose",lep4->IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep4IsoTight",lep4->IsoTight);
  configMgr->treeMaker->setBoolVariable("lep4IsoGradient",lep4->IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep4IsoGradientLoose",lep4->IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep4IsoLooseTrackOnly",lep4->IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep4IsoFixedCutLoose",lep4->IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep4IsoFixedCutTight",lep4->IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep4IsoFixedCutTightTrackOnly",lep4->IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrLoose",lep4->corr_IsoLoose);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrTight",lep4->corr_IsoTight);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrGradient",lep4->corr_IsoGradient);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrGradientLoose",lep4->corr_IsoGradientLoose);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrLooseTrackOnly",lep4->corr_IsoLooseTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrFixedCutLoose",lep4->corr_IsoFixedCutLoose);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrFixedCutTight",lep4->corr_IsoFixedCutTight);
  configMgr->treeMaker->setBoolVariable("lep4IsoCorrFixedCutTightTrackOnly",lep4->corr_IsoFixedCutTightTrackOnly);
  configMgr->treeMaker->setBoolVariable("lep4Signal",lep4->signal);
  configMgr->treeMaker->setBoolVariable("lep4TruthMatched",lep4TruthMatched);

  configMgr->treeMaker->setIntVariable("lep4TruthCharge",lep4->truthQ);
  configMgr->treeMaker->setFloatVariable("lep4TruthPt",  lep4->truthTLV.Pt());
  configMgr->treeMaker->setFloatVariable("lep4TruthEta", lep4->truthTLV.Eta());;
  configMgr->treeMaker->setFloatVariable("lep4TruthPhi", lep4->truthTLV.Phi());
  configMgr->treeMaker->setFloatVariable("lep4TruthM",   lep4->truthTLV.M());

  // Jets
  configMgr->treeMaker->setIntVariable("nJet30",nJet30);
  configMgr->treeMaker->setIntVariable("nJet25",getNJets( configMgr->obj, 25.0 ));
  configMgr->treeMaker->setIntVariable("nJet20",getNJets( configMgr->obj, 20.0 ));
  configMgr->treeMaker->setIntVariable("nTotalJet",getNJets( configMgr->obj, 25.0 ) + configMgr->obj->fJets.size() );
  configMgr->treeMaker->setIntVariable("nTotalJet20",getNJets( configMgr->obj, 20.0 ) + configMgr->obj->fJets.size() );
  configMgr->treeMaker->setIntVariable("nBJet30_MV2c10",nBJet30);
  configMgr->treeMaker->setIntVariable("nBJet20_MV2c10",nBJet20);
  configMgr->treeMaker->setIntVariable("nBJet30_MV2c10_FixedCutBEff_60",nBJet30_FixedCutBEff_60);
  configMgr->treeMaker->setIntVariable("nBJet30_MV2c10_FixedCutBEff_70",nBJet30_FixedCutBEff_70);
  configMgr->treeMaker->setIntVariable("nBJet30_MV2c10_FixedCutBEff_77",nBJet30_FixedCutBEff_77);
  configMgr->treeMaker->setIntVariable("nBJet30_MV2c10_FixedCutBEff_85",nBJet30_FixedCutBEff_85);
  configMgr->treeMaker->setIntVariable("nBJet20_MV2c10_FixedCutBEff_60",nBJet20_FixedCutBEff_60);
  configMgr->treeMaker->setIntVariable("nBJet20_MV2c10_FixedCutBEff_70",nBJet20_FixedCutBEff_70);
  configMgr->treeMaker->setIntVariable("nBJet20_MV2c10_FixedCutBEff_77",nBJet20_FixedCutBEff_77);
  configMgr->treeMaker->setIntVariable("nBJet20_MV2c10_FixedCutBEff_85",nBJet20_FixedCutBEff_85);
  configMgr->treeMaker->setIntVariable("DecayModeTTbar",intTTbarDecayMode);

  configMgr->treeMaker->setVecFloatVariable("jetPt",jetPtVec);
  configMgr->treeMaker->setVecFloatVariable("jetEta",jetEtaVec);
  configMgr->treeMaker->setVecFloatVariable("jetPhi",jetPhiVec);
  configMgr->treeMaker->setVecFloatVariable("jetM",jetMVec);
  configMgr->treeMaker->setVecFloatVariable("jetTileEnergy",jetTileEnergy);

  configMgr->treeMaker->setFloatVariable("vectorSumJetsPt",  vectorSumJets.Pt());
  configMgr->treeMaker->setFloatVariable("vectorSumJetsEta", vectorSumJets.Eta());
  configMgr->treeMaker->setFloatVariable("vectorSumJetsPhi", vectorSumJets.Phi());
  configMgr->treeMaker->setFloatVariable("vectorSumJetsM",   vectorSumJets.M());
  configMgr->treeMaker->setFloatVariable("dPhiVectorSumJetsMET", dPhiVectorSumJetsMET);

  // MET
  configMgr->treeMaker->setFloatVariable("met_Et",configMgr->obj->met.Et);
  configMgr->treeMaker->setFloatVariable("met_Phi",configMgr->obj->met.phi );
  configMgr->treeMaker->setFloatVariable("TST_Et",configMgr->obj->met.Et_soft );
  configMgr->treeMaker->setFloatVariable("TST_Phi",configMgr->obj->met.phi_soft );
  configMgr->treeMaker->setFloatVariable("met_track_Et",configMgr->obj->met.Et_track);
  configMgr->treeMaker->setFloatVariable("met_track_Phi",configMgr->obj->met.phi_track );
  configMgr->treeMaker->setFloatVariable("deltaPhi_MET_TST_Phi", TVector2::Phi_mpi_pi(configMgr->obj->met.phi_soft-configMgr->obj->met.phi) );

  // Analysis variables
  configMgr->treeMaker->setFloatVariable("meffInc30",meffInc30);
  configMgr->treeMaker->setFloatVariable("Ht30",Ht30);
  configMgr->treeMaker->setFloatVariable("hadronicWMass",wMass);
  configMgr->treeMaker->setFloatVariable("hadronicWPt",wPt);

  configMgr->treeMaker->setFloatVariable("LepAplanarity",LepAplanarity);
  configMgr->treeMaker->setFloatVariable("JetAplanarity",JetAplanarity);

  configMgr->treeMaker->setFloatVariable("amt2",amt2);
  configMgr->treeMaker->setFloatVariable("amt2b",amt2b);
  configMgr->treeMaker->setFloatVariable("amt2bWeight",amt2bWeight);
  configMgr->treeMaker->setFloatVariable("mt2tau",mt2tau);
  configMgr->treeMaker->setFloatVariable("mt2lj",mt2lj);
  //configMgr->treeMaker->setFloatVariable("topNess",topNess);
  configMgr->treeMaker->setFloatVariable("mbb",mbb);

  // other analysis variables
  configMgr->treeMaker->setFloatVariable("mt_lep1",mt_lep1); // mT leading lepton
  configMgr->treeMaker->setFloatVariable("mt_lep2",mt_lep2); // mT subleading lepton
  configMgr->treeMaker->setFloatVariable("mt_lep3",mt_lep3); // mT 3rd lepton
  configMgr->treeMaker->setFloatVariable("mt_lep4",mt_lep4); // mT 4th lepton
  configMgr->treeMaker->setFloatVariable("mt_lep1_metTrack",mt_lep1_metTrack);
  configMgr->treeMaker->setFloatVariable("mt_lep2_metTrack",mt_lep2_metTrack);
  configMgr->treeMaker->setFloatVariable("mt_lep3_metTrack",mt_lep3_metTrack);
  configMgr->treeMaker->setFloatVariable("mt_lep4_metTrack",mt_lep4_metTrack);
  configMgr->treeMaker->setFloatVariable("METOverHT",METOverHT);
  configMgr->treeMaker->setFloatVariable("METOverJ1pT",METOverJ1pT);
  configMgr->treeMaker->setFloatVariable("METTrackOverHT",METTrackOverHT);
  configMgr->treeMaker->setFloatVariable("METTrackOverJ1pT",METTrackOverJ1pT);
  configMgr->treeMaker->setFloatVariable("DPhiJ1Met", DPhiJ1Met);
  configMgr->treeMaker->setFloatVariable("DPhiJ2Met", DPhiJ2Met);
  configMgr->treeMaker->setFloatVariable("DPhiJ3Met", DPhiJ3Met);
  configMgr->treeMaker->setFloatVariable("DPhiJ4Met", DPhiJ4Met);  
  configMgr->treeMaker->setFloatVariable("minDPhi4JetsMet",minDPhi4JetsMet);
  configMgr->treeMaker->setFloatVariable("minDPhiAllJetsMet",minDPhiAllJetsMet);
  configMgr->treeMaker->setFloatVariable("DPhiJ1MetTrack", DPhiJ1MetTrack);
  configMgr->treeMaker->setFloatVariable("DPhiJ2MetTrack", DPhiJ2MetTrack);
  configMgr->treeMaker->setFloatVariable("DPhiJ3MetTrack", DPhiJ3MetTrack);
  configMgr->treeMaker->setFloatVariable("DPhiJ4MetTrack", DPhiJ4MetTrack);  
  configMgr->treeMaker->setFloatVariable("minDPhi4JetsMetTrack",minDPhi4JetsMetTrack);
  configMgr->treeMaker->setFloatVariable("minDPhiAllJetsMetTrack",minDPhiAllJetsMetTrack);
  configMgr->treeMaker->setFloatVariable("METOverHTLep",METOverHTLep);
  configMgr->treeMaker->setFloatVariable("METTrackOverHTLep",METTrackOverHTLep);
  // 2L analysis variables
  configMgr->treeMaker->setFloatVariable("mll", mll);
  configMgr->treeMaker->setFloatVariable("Rll", Rll);
  configMgr->treeMaker->setFloatVariable("Ptll", Ptll);
  configMgr->treeMaker->setFloatVariable("dPhiPllMet", dPhiPllMet);
  configMgr->treeMaker->setFloatVariable("dPhiPllMetTrack", dPhiPllMetTrack);

  configMgr->treeMaker->setFloatVariable("METRel",METRel);
  configMgr->treeMaker->setFloatVariable("METTrackRel",METTrackRel);
  configMgr->treeMaker->setFloatVariable("dPhiNearMet",dPhiNearMet);
  configMgr->treeMaker->setFloatVariable("dPhiNearMetTrack",dPhiNearMetTrack);
  configMgr->treeMaker->setFloatVariable("dPhiMetAndMetTrack", dPhiMetAndMetTrack);

  configMgr->treeMaker->setFloatVariable("MSqTauTau_1", MSqTauTau_1);
  configMgr->treeMaker->setFloatVariable("MSqTauTau_2", MSqTauTau_2);
  configMgr->treeMaker->setFloatVariable("MTauTau", MTauTau);
  configMgr->treeMaker->setFloatVariable("MSqTauTau_1_metTrack", MSqTauTau_1_metTrack);
  configMgr->treeMaker->setFloatVariable("MSqTauTau_2_metTrack", MSqTauTau_2_metTrack);
  configMgr->treeMaker->setFloatVariable("MTauTau_metTrack", MTauTau_metTrack);
  configMgr->treeMaker->setFloatVariable("RjlOverEl", RjlOverEl);
  configMgr->treeMaker->setFloatVariable("LepCosThetaLab", LepCosThetaLab);
  configMgr->treeMaker->setFloatVariable("LepCosThetaCoM", LepCosThetaCoM);

  // mt2(L1,L2,MET) with various trial invisible particle masses
  configMgr->treeMaker->setFloatVariable("mt2leplsp_0",mt2leplsp_0);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_25",mt2leplsp_25);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_50",mt2leplsp_50);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_75",mt2leplsp_75);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_90",mt2leplsp_90);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_95",mt2leplsp_95);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_100",mt2leplsp_100);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_105",mt2leplsp_105);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_110",mt2leplsp_110);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_115",mt2leplsp_115);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_120",mt2leplsp_120);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_150",mt2leplsp_150);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_175",mt2leplsp_175);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_200",mt2leplsp_200);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_250",mt2leplsp_250);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_300",mt2leplsp_300);

  configMgr->treeMaker->setFloatVariable("mt2leplsp_0_metTrack",mt2leplsp_0_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_25_metTrack",mt2leplsp_25_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_50_metTrack",mt2leplsp_50_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_75_metTrack",mt2leplsp_75_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_90_metTrack",mt2leplsp_90_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_95_metTrack",mt2leplsp_95_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_100_metTrack",mt2leplsp_100_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_105_metTrack",mt2leplsp_105_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_110_metTrack",mt2leplsp_110_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_115_metTrack",mt2leplsp_115_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_120_metTrack",mt2leplsp_120_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_150_metTrack",mt2leplsp_150_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_175_metTrack",mt2leplsp_175_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_200_metTrack",mt2leplsp_200_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_250_metTrack",mt2leplsp_250_metTrack);
  configMgr->treeMaker->setFloatVariable("mt2leplsp_300_metTrack",mt2leplsp_300_metTrack);

  // Triggers
  for (const char* chainName : this->triggerChains) {
    configMgr->treeMaker->setBoolVariable(chainName, configMgr->obj->evt.getHLTEvtTrigDec(chainName));
  }

  // Weights
  configMgr->treeMaker->setDoubleVariable("pileupWeight",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::PILEUP));
  configMgr->treeMaker->setDoubleVariable("leptonWeight",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::LEP));
  configMgr->treeMaker->setDoubleVariable("eventWeight",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::EVT));
  configMgr->treeMaker->setDoubleVariable("genWeight",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::GEN));
  configMgr->treeMaker->setDoubleVariable("bTagWeight",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::BTAG));
  configMgr->treeMaker->setDoubleVariable("jvtWeight",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::JVT));
  configMgr->treeMaker->setDoubleVariable("genWeightUp",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::GEN));
  configMgr->treeMaker->setDoubleVariable("genWeightDown",configMgr->objectTools->getWeight(configMgr->obj,"",ObjectTools::WeightType::GEN));

  // Wino-Bino weight, for signal samples
  double truthMll = configMgr->obj->truthEvent.truthMll;
  double winoBinoMllWeight = getWinoBinoMllWeight(configMgr->obj->evt.dsid, truthMll);
  double winoBinoXsecWeight = getWinoBinoXsecWeight(configMgr->obj->evt.dsid);
  double winoBinoBrFracWeight = getWinoBinoBrFracWeight(configMgr->obj->evt.dsid);
  double winoBinoWeight = winoBinoMllWeight*winoBinoXsecWeight*winoBinoBrFracWeight;
  configMgr->treeMaker->setDoubleVariable("truthMll",truthMll);
  configMgr->treeMaker->setDoubleVariable("winoBinoMllWeight",winoBinoMllWeight);
  configMgr->treeMaker->setDoubleVariable("winoBinoXsecWeight",winoBinoXsecWeight);
  configMgr->treeMaker->setDoubleVariable("winoBinoBrFracWeight",winoBinoBrFracWeight);
  configMgr->treeMaker->setDoubleVariable("winoBinoWeight",winoBinoWeight);

  //NUHM2
  double NUHM2weight_350m12 = get_NUHM2_weight(configMgr->obj->evt.dsid, truthMll,350);
  double NUHM2weight_400m12 = get_NUHM2_weight(configMgr->obj->evt.dsid, truthMll,400);
  double NUHM2weight_500m12 = get_NUHM2_weight(configMgr->obj->evt.dsid, truthMll,500);
  double NUHM2weight_600m12 = get_NUHM2_weight(configMgr->obj->evt.dsid, truthMll,600);
  double NUHM2weight_700m12 = get_NUHM2_weight(configMgr->obj->evt.dsid, truthMll,700);
  double NUHM2weight_800m12 = get_NUHM2_weight(configMgr->obj->evt.dsid, truthMll,800);
  configMgr->treeMaker->setDoubleVariable("NUHM2weight_350m12",NUHM2weight_350m12);
  configMgr->treeMaker->setDoubleVariable("NUHM2weight_400m12",NUHM2weight_400m12);
  configMgr->treeMaker->setDoubleVariable("NUHM2weight_500m12",NUHM2weight_500m12);
  configMgr->treeMaker->setDoubleVariable("NUHM2weight_600m12",NUHM2weight_600m12);
  configMgr->treeMaker->setDoubleVariable("NUHM2weight_700m12",NUHM2weight_700m12);
  configMgr->treeMaker->setDoubleVariable("NUHM2weight_800m12",NUHM2weight_800m12);

  // PDF variables (only for nominal trees)
  if (configMgr->treeMaker->getTreeState()=="") {
    configMgr->treeMaker->setFloatVariable("x1", configMgr->obj->evt.x1);
    configMgr->treeMaker->setFloatVariable("x2", configMgr->obj->evt.x2);
    configMgr->treeMaker->setFloatVariable("pdf1", configMgr->obj->evt.pdf1);
    configMgr->treeMaker->setFloatVariable("pdf2", configMgr->obj->evt.pdf2);
    configMgr->treeMaker->setFloatVariable("scalePDF", configMgr->obj->evt.scalePDF);
    configMgr->treeMaker->setIntVariable("id1", configMgr->obj->evt.id1);
    configMgr->treeMaker->setIntVariable("id2", configMgr->obj->evt.id2);
    configMgr->treeMaker->setVecFloatVariable("LHE3Weights",configMgr->obj->evt.LHE3Weights);
  }

  //
  configMgr->doWeightSystematics();

  // Fill the output tree
  configMgr->treeMaker->Fill(configMgr->getSysState(),"tree");

  // We may have new-ed these, so better delete them
  if(nLep_base < 3) delete lep3;
  if(nLep_base < 4) delete lep4;

  return true;

}
// ------------------------------------------------------------------------------------------ //
bool twoleptonSelector::passCuts(ConfigMgr*& configMgr)
{

  /*
   This method is used to apply any cuts you wish before writing
   the output trees
  */
  double weight = 1; // pb-1
  if(!configMgr->obj->evt.isData()){

    // Note that 2015+2016+2017 is not exactly 80 fb-1, but close enough
    weight = 80000; // pb-1
  }
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::GEN);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::EVT);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::LEP);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::JVT);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::PILEUP);

  // Fill cutflow histograms
  configMgr->cutflow->bookCut("cutFlow","allEvents",weight );

  ////////////////////////////////////////
  //// begin ntuple preselection cuts ////
  ////////////////////////////////////////

  // Apply all recommended event cleaning cuts
  if( !configMgr->obj->passEventCleaning( configMgr->cutflow, "cutFlow", weight ) ) return false;

  // At least two baseline leptons
  if( configMgr->obj->baseLeptons.size() < 2 ) return false;
  configMgr->cutflow->bookCut("cutFlow","At least two baseline leptons", weight );

  // MET skimming 
  //if(configMgr->obj->met.Et < 100.0) return false;
  //configMgr->cutflow->bookCut("cutFlow","MET > 100 GeV", weight );

  // Removing for now!
  /*
  // Extra cuts for all trees except the nominal with a loose preselection
  // (used for e.g. producing the fake ntuple)
  if(configMgr->treeMaker->getTreeState() != "LOOSE_NOMINAL"){

    // Tighter MET skimming
    if(configMgr->obj->met.Et < 200.0) return false;
    configMgr->cutflow->bookCut("cutFlow","Syst trees: MET > 200 GeV", weight );

    // At least one signal jet with pT > 100 GeV
    if( getNJets(configMgr->obj, 100.0) < 1 ) return false;
    configMgr->cutflow->bookCut("cutFlow","Syst trees: >= 1 signal jet with pT > 100 GeV", weight );

    // At least two signal leptons
    if( configMgr->obj->signalLeptons.size() < 2 ) return false;
    configMgr->cutflow->bookCut("cutFlow","Syst trees: At least two signal leptons", weight );

    // Require mll < 60 GeV for the leading dilepton pair (baseline leptons)
    if( getMll(configMgr->obj, true) > 60.0 ) return false;
    configMgr->cutflow->bookCut("cutFlow","Syst trees: Dilepton mass below 60 GeV", weight );

  }
  */

  /////////////////////////////////////////
  //// begin cuts for detailed cutflow ////
  /////////////////////////////////////////

  bool doComparisonChecks = false;
  if(doComparisonChecks){

    // Non-sequential, post-cleaning
    if(configMgr->obj->baseElectrons.size() >= 1)
      configMgr->cutflow->bookCut("cutFlow",">= 1 baseline el", weight );
    if(configMgr->obj->signalElectrons.size() >= 1)
      configMgr->cutflow->bookCut("cutFlow",">= 1 signal el", weight );
    if(configMgr->obj->baseMuons.size() >= 1)
      configMgr->cutflow->bookCut("cutFlow",">= 1 baseline mu", weight );
    if(configMgr->obj->signalMuons.size() >= 1)
      configMgr->cutflow->bookCut("cutFlow",">= 1 signal mu", weight );
    if(configMgr->obj->baseElectrons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow",">= 2 baseline el", weight );
    if(configMgr->obj->signalElectrons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow",">= 2 signal el", weight );
    if(configMgr->obj->baseMuons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow",">= 2 baseline mu", weight );
    if(configMgr->obj->signalMuons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow",">= 2 signal mu", weight );
    if(configMgr->obj->baseLeptons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow",">= 2 baseline lep", weight );
    if(configMgr->obj->signalLeptons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow",">= 2 signal lep", weight );
    if(configMgr->obj->baseElectrons.size() == 3)
      configMgr->cutflow->bookCut("cutFlow","== 3 baseline el", weight );
    if(configMgr->obj->signalElectrons.size() == 3)
      configMgr->cutflow->bookCut("cutFlow","== 3 signal el", weight );
    if(configMgr->obj->baseMuons.size() == 3)
      configMgr->cutflow->bookCut("cutFlow","== 3 baseline mu", weight );
    if(configMgr->obj->signalMuons.size() == 3)
      configMgr->cutflow->bookCut("cutFlow","== 3 signal mu", weight );
    if(configMgr->obj->baseLeptons.size() == 3)
      configMgr->cutflow->bookCut("cutFlow","== 3 baseline lep", weight );
    if(configMgr->obj->signalLeptons.size() == 3)
      configMgr->cutflow->bookCut("cutFlow","== 3 signal lep", weight );
    if(configMgr->obj->baselineJets.size() >= 1)
      configMgr->cutflow->bookCut("cutFlow",">= 1 baseline jet", weight );
    if(configMgr->obj->cJets.size() >= 1)
      configMgr->cutflow->bookCut("cutFlow",">= 1 signal jet", weight );
    if(getNBJets(configMgr->obj, 30.0) == 0 )
      configMgr->cutflow->bookCut("cutFlow","zero b-jets", weight );

    //Construct emulated soft muon + jet + met triggers
    bool HLT_2mu4_j85_xe50_mht_emul = 0;
    bool HLT_mu4_j125_xe90_mht_emul = 0;
    if(!configMgr->obj->evt.isData()){
      HLT_2mu4_j85_xe50_mht_emul = configMgr->obj->evt.getHLTEvtTrigDec("HLT_2mu4") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_j85_L1J40") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe50_mht_L1XE20");
      HLT_mu4_j125_xe90_mht_emul = configMgr->obj->evt.getHLTEvtTrigDec("HLT_mu4") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_j125_L1J50") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe90_mht_L1XE40");
    }

    if(HLT_mu4_j125_xe90_mht_emul)
      configMgr->cutflow->bookCut("cutFlow","HLT_mu4_j125_xe90_mht_emul", weight );
    if(HLT_2mu4_j85_xe50_mht_emul)
      configMgr->cutflow->bookCut("cutFlow","HLT_2mu4_j85_xe50_mht_emul", weight );

    if(configMgr->obj->evt.getHLTEvtTrigDec("HLT_mu4_j125_xe90_mht"))
      configMgr->cutflow->bookCut("cutFlow","HLT_mu4_j125_xe90_mht", weight );
    if(configMgr->obj->evt.getHLTEvtTrigDec("HLT_2mu4_j85_xe50_mht"))
      configMgr->cutflow->bookCut("cutFlow","HLT_2mu4_j85_xe50_mht", weight );
    if(configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe110_mht_L1XE50"))
      configMgr->cutflow->bookCut("cutFlow","HLT_xe110_mht_L1XE50", weight );

    // Sequential, post-cleaning
    if(configMgr->obj->baseLeptons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow","SEQ: >= 2 baseline lep", weight );
    else return false;

    if(configMgr->obj->signalLeptons.size() >= 2)
      configMgr->cutflow->bookCut("cutFlow","SEQ: >= 2 signal lep", weight );
    else return false;

    LeptonVariable* lep1 = configMgr->obj->baseLeptons[0];
    LeptonVariable* lep2 = configMgr->obj->baseLeptons[1];

    bool SF = (lep1->isEle() && lep2->isEle()) || (lep1->isMu() && lep2->isMu());
    bool OS = (lep1->q == -lep2->q);

    if(SF && OS)
      configMgr->cutflow->bookCut("cutFlow","SEQ: SFOS", weight );
    else return false;

    if(lep1->Pt() < 50)
      configMgr->cutflow->bookCut("cutFlow","SEQ: lead lep pT < 50 GeV", weight );
    else return false;

    if(lep1->Pt() >20 && lep2->Pt() >20)
      configMgr->cutflow->bookCut("cutFlow","SEQ: 2 lead lep pT >20 GeV", weight );
    else return false;

    if( getNJets(configMgr->obj, 30.0) >= 1 )
      configMgr->cutflow->bookCut("cutFlow","SEQ: >= 1 signal jet", weight );
    else return false;

    JetVariable* jet1 = configMgr->obj->cJets[0];

    if(jet1->Pt() > 150)
      configMgr->cutflow->bookCut("cutFlow","SEQ: lead jet pT > 150 GeV", weight );
    else return false;

    if (getNBJets(configMgr->obj, 30.0) == 0 )
      configMgr->cutflow->bookCut("cutFlow","SEQ: zero b-jets", weight );
    else return false;

    if(configMgr->obj->met.Et > 150.0)
      configMgr->cutflow->bookCut("cutFlow","SEQ: MET > 150 GeV", weight );
    else return false;

    if(configMgr->obj->evt.isData()){
      if(fabs(TVector2::Phi_mpi_pi(configMgr->obj->met.phi - jet1->Phi())) < 2.5)
        configMgr->cutflow->bookCut("cutFlow","SEQ: dphi(jet, MET) < 2.5", weight );
      else return false;
    }
    else{
      if(fabs(TVector2::Phi_mpi_pi(configMgr->obj->met.phi - jet1->Phi())) > 2.5)
        configMgr->cutflow->bookCut("cutFlow","SEQ: dphi(jet, MET) > 2.5", weight );
      else return false;
    }

    if((*lep1+*lep2).M() < 50)
      configMgr->cutflow->bookCut("cutFlow","SEQ: mll < 50 GeV", weight );
    else return false;

    if((*lep1).DeltaR(*lep2) < 1.5)
      configMgr->cutflow->bookCut("cutFlow","SEQ: Rll < 1.5", weight );
    else return false;

    float MSqTauTau_2   = getMSqTauTau(configMgr->obj, 2); // version 2 : more natural definition
    
    // Take signed square root of MSqTauTau_2
    float MTauTau       = -99999.;
    if (MSqTauTau_2 >= 0.) MTauTau =  sqrt( MSqTauTau_2 ); 
    if (MSqTauTau_2 < 0.)  MTauTau = -sqrt( fabs( MSqTauTau_2 ) );

    if(MTauTau < 0)
      configMgr->cutflow->bookCut("cutFlow","SEQ: mtt < 0 GeV", weight );
    else return false;
  }

  return true;

}
// ------------------------------------------------------------------------------------------ //
int twoleptonSelector::getHiggsinoDM(int DSID){
  int deltaM = 0;
  if (DSID==393596 || DSID==394173 || DSID==394177 || DSID==394181 || DSID==394185 || DSID==394189 || DSID==394193 || DSID==393597 || DSID==394174 || DSID==394178 || DSID==394182 || DSID==394186 || DSID==394190 || DSID==394194 || DSID==393598 || DSID==394175 || DSID==394179 || DSID==394183 || DSID==394187 || DSID==394191 || DSID==394195 || DSID==393599 || DSID==394176 || DSID==394180 || DSID==394184 || DSID==394188 || DSID==394192 || DSID==394196){
    deltaM = 2;
  } else if (DSID==393400 || DSID==393407 || DSID==393414 || DSID==393421 || DSID==393428 || DSID==393435 || DSID==393442 || DSID==393449 || DSID==393456 || DSID==393463 || DSID==393470 || DSID==393477 || DSID==393484 || DSID==393491 || DSID==393498 || DSID==393505 || DSID==393512 || DSID==393519 || DSID==393526 || DSID==393533 || DSID==393540 || DSID==393547 || DSID==393554 || DSID==393561 || DSID==393568 || DSID==393575 || DSID==393582 || DSID==393589){
    deltaM = 3;
  } else if (DSID==393401 || DSID==393408 || DSID==393415 || DSID==393422 || DSID==393429 || DSID==393436 || DSID==393443 || DSID==393450 || DSID==393457 || DSID==393464 || DSID==393471 || DSID==393478 || DSID==393485 || DSID==393492 || DSID==393499 || DSID==393506 || DSID==393513 || DSID==393520 || DSID==393527 || DSID==393534 || DSID==393541 || DSID==393548 || DSID==393555 || DSID==393562 || DSID==393569 || DSID==393576 || DSID==393583 || DSID==393590){
    deltaM = 5;
  } else if (DSID==393402 || DSID==393409 || DSID==393416 || DSID==393423 || DSID==393430 || DSID==393437 || DSID==393444 || DSID==393451 || DSID==393458 || DSID==393465 || DSID==393472 || DSID==393479 || DSID==393486 || DSID==393493 || DSID==393500 || DSID==393507 || DSID==393514 || DSID==393521 || DSID==393528 || DSID==393535 || DSID==393542 || DSID==393549 || DSID==393556 || DSID==393563 || DSID==393570 || DSID==393577 || DSID==393584 || DSID==393591){
    deltaM = 10;
  } else if (DSID==393403 || DSID==393410 || DSID==393417 || DSID==393424 || DSID==393431 || DSID==393438 || DSID==393445 || DSID==393452 || DSID==393459 || DSID==393466 || DSID==393473 || DSID==393480 || DSID==393487 || DSID==393494 || DSID==393501 || DSID==393508 || DSID==393515 || DSID==393522 || DSID==393529 || DSID==393536 || DSID==393543 || DSID==393550 || DSID==393557 || DSID==393564 || DSID==393571 || DSID==393578 || DSID==393585 || DSID==393592){
    deltaM = 20;
  } else if (DSID==393404 || DSID==393411 || DSID==393418 || DSID==393425 || DSID==393432 || DSID==393439 || DSID==393446 || DSID==393453 || DSID==393460 || DSID==393467 || DSID==393474 || DSID==393481 || DSID==393488 || DSID==393495 || DSID==393502 || DSID==393509 || DSID==393516 || DSID==393523 || DSID==393530 || DSID==393537 || DSID==393544 || DSID==393551 || DSID==393558 || DSID==393565 || DSID==393572 || DSID==393579 || DSID==393586 || DSID==393593){
    deltaM = 40;
  } else if (DSID==393405 || DSID==393412 || DSID==393419 || DSID==393426 || DSID==393433 || DSID==393440 || DSID==393447 || DSID==393454 || DSID==393461 || DSID==393468 || DSID==393475 || DSID==393482 || DSID==393489 || DSID==393496 || DSID==393503 || DSID==393510 || DSID==393517 || DSID==393524 || DSID==393531 || DSID==393538 || DSID==393545 || DSID==393552 || DSID==393559 || DSID==393566 || DSID==393573 || DSID==393580 || DSID==393587 || DSID==393594){
    deltaM = 60;
  } else if (DSID==393406 || DSID==393413 || DSID==393420 || DSID==393427 || DSID==393434 || DSID==393441 || DSID==393448 || DSID==393455 || DSID==393462 || DSID==393469 || DSID==393476 || DSID==393483 || DSID==393490 || DSID==393497 || DSID==393504 || DSID==393511 || DSID==393518 || DSID==393525 || DSID==393532 || DSID==393539 || DSID==393546 || DSID==393553 || DSID==393560 || DSID==393567 || DSID==393574 || DSID==393581 || DSID==393588 || DSID==393595){
    deltaM = 100;
  } else {
    deltaM = 0;
  }
  return deltaM;
}
// ------------------------------------------------------------------------------------------ //
bool twoleptonSelector::isHiggsinoN2C1p(int DSID){
  bool isN2C1p = false;
  if (DSID==393596 || DSID==393400 || DSID==393401 || DSID==393402 || DSID==393403 || DSID==393404 || DSID==393405 || DSID==393406 || DSID==394173 || DSID==393407 || DSID==393408 || DSID==393409 || DSID==393410 || DSID==393411 || DSID==393412 || DSID==393413 || DSID==394177 || DSID==393414 || DSID==393415 || DSID==393416 || DSID==393417 || DSID==393418 || DSID==393419 || DSID==393420 || DSID==394181 || DSID==393421 || DSID==393422 || DSID==393423 || DSID==393424 || DSID==393425 || DSID==393426 || DSID==393427 || DSID==394185 || DSID==393428 || DSID==393429 || DSID==393430 || DSID==393431 || DSID==393432 || DSID==393433 || DSID==393434 || DSID==394189 || DSID==393435 || DSID==393436 || DSID==393437 || DSID==393438 || DSID==393439 || DSID==393440 || DSID==393441 || DSID==394193 || DSID==393442 || DSID==393443 || DSID==393444 || DSID==393445 || DSID==393446 || DSID==393447 || DSID==393448){
    isN2C1p = true;
  } else {
    isN2C1p = false;
  }
  return isN2C1p;
}
// ------------------------------------------------------------------------------------------ //
bool twoleptonSelector::isHiggsinoN2C1m(int DSID){
  bool isN2C1m = false;
  if (DSID==393597 || DSID==393449 || DSID==393450 || DSID==393451 || DSID==393452 || DSID==393453 || DSID==393454 || DSID==393455 || DSID==394174 || DSID==393456 || DSID==393457 || DSID==393458 || DSID==393459 || DSID==393460 || DSID==393461 || DSID==393462 || DSID==394178 || DSID==393463 || DSID==393464 || DSID==393465 || DSID==393466 || DSID==393467 || DSID==393468 || DSID==393469 || DSID==394182 || DSID==393470 || DSID==393471 || DSID==393472 || DSID==393473 || DSID==393474 || DSID==393475 || DSID==393476 || DSID==394186 || DSID==393477 || DSID==393478 || DSID==393479 || DSID==393480 || DSID==393481 || DSID==393482 || DSID==393483 || DSID==394190 || DSID==393484 || DSID==393485 || DSID==393486 || DSID==393487 || DSID==393488 || DSID==393489 || DSID==393490 || DSID==394194 || DSID==393491 || DSID==393492 || DSID==393493 || DSID==393494 || DSID==393495 || DSID==393496 || DSID==393497){
    isN2C1m = true;
  } else{
    isN2C1m = false;
  }
  return isN2C1m;
}
// ------------------------------------------------------------------------------------------ //
bool twoleptonSelector::isHiggsinoC1C1(int DSID){
  bool isC1C1 = false;
  if (DSID==393599 || DSID==393547 || DSID==393548 || DSID==393549 || DSID==393550 || DSID==393551 || DSID==393552 || DSID==393553 || DSID==394176 || DSID==393554 || DSID==393555 || DSID==393556 || DSID==393557 || DSID==393558 || DSID==393559 || DSID==393560 || DSID==394180 || DSID==393561 || DSID==393562 || DSID==393563 || DSID==393564 || DSID==393565 || DSID==393566 || DSID==393567 || DSID==394184 || DSID==393568 || DSID==393569 || DSID==393570 || DSID==393571 || DSID==393572 || DSID==393573 || DSID==393574 || DSID==394188 || DSID==393575 || DSID==393576 || DSID==393577 || DSID==393578 || DSID==393579 || DSID==393580 || DSID==393581 || DSID==394192 || DSID==393582 || DSID==393583 || DSID==393584 || DSID==393585 || DSID==393586 || DSID==393587 || DSID==393588 || DSID==394196 || DSID==393589 || DSID==393590 || DSID==393591 || DSID==393592 || DSID==393593 || DSID==393594 || DSID==393595){
    isC1C1 = true;
  } else {
    isC1C1 = false; 
  }
  return isC1C1;
}
// ------------------------------------------------------------------------------------------ //
bool twoleptonSelector::isHiggsinoN2N1(int DSID){
  bool isN2N1 = false;
  if (DSID==393598 || DSID==393498 || DSID==393499 || DSID==393500 || DSID==393501 || DSID==393502 || DSID==393503 || DSID==393504 || DSID==394175 || DSID==393505 || DSID==393506 || DSID==393507 || DSID==393508 || DSID==393509 || DSID==393510 || DSID==393511 || DSID==394179 || DSID==393512 || DSID==393513 || DSID==393514 || DSID==393515 || DSID==393516 || DSID==393517 || DSID==393518 || DSID==394183 || DSID==393519 || DSID==393520 || DSID==393521 || DSID==393522 || DSID==393523 || DSID==393524 || DSID==393525 || DSID==394187 || DSID==393526 || DSID==393527 || DSID==393528 || DSID==393529 || DSID==393530 || DSID==393531 || DSID==393532 || DSID==394191 || DSID==393533 || DSID==393534 || DSID==393535 || DSID==393536 || DSID==393537 || DSID==393538 || DSID==393539 || DSID==394195 || DSID==393540 || DSID==393541 || DSID==393542 || DSID==393543 || DSID==393544 || DSID==393545 || DSID==393546){
    isN2N1 = true;
  } else{
    isN2N1 = false;
  }
  return isN2N1;
}
// ------------------------------------------------------------------------------------------ //
double twoleptonSelector::getWinoBinoFuncMllDistr(double x, double *par){
  // See https://its.cern.ch/jira/browse/HIGGSINO-27:
  // taken from https://arxiv.org/abs/0704.2515
  // equation 4, page 9 
  // NB there is a typo in the formula (just in writing it in the the paper, not in the work)
  // the correct formula is in this code

  double m1=par[1];
  double m2=par[2];
  double mu=m2-m1;
  double m=x;
  double M=m1+m2;
  double m_Z=91;
  double norm=par[0]*m;

  double radice=sqrt( pow(m,4) -pow(m,2)*(pow(mu,2) + pow(M,2) ) + pow(mu*M,2) );

  double normalizzazione = pow(  pow(m,2) -pow(m_Z,2),2);
  double var = (norm* radice)/ normalizzazione  ;
  var = var* (-2*pow(m,4)+ pow(m,2)*(2*pow(M,2) - pow(mu,2)) +pow((mu*M),2));
  if(x>fabs(m2) - fabs(m1)) var=1;
  return var; 
}
// ------------------------------------------------------------------------------------------ //
double twoleptonSelector::getWinoBinoMllWeight(int DSID, double mass){
  // See https://its.cern.ch/jira/browse/HIGGSINO-27:

  // Don't calculate a weight unless it is N2C1 Higgsino
  if( !isHiggsinoN2C1p(DSID) && !isHiggsinoN2C1m(DSID) ) {
    return 1.;
  }

  if (getHiggsinoDM(DSID) == 100){ // no reweighting for on-shell Z
    return 1.;
  }

  double winoBinoMllWeight = 1.0;
  std::vector<double> parHvec;
  parHvec = {0.0, 0.0, 0.0};

  if (DSID == 393596 || DSID == 393597) parHvec = {3.00003, 80.0, -82.0};
  else if (DSID == 393400 || DSID == 393449) parHvec = {3.00008, 80.0, -83.0};
  else if (DSID == 393401 || DSID == 393450) parHvec = {3.00031, 80.0, -85.0};
  else if (DSID == 393402 || DSID == 393451) parHvec = {3.00199, 80.0, -90.0};
  else if (DSID == 393403 || DSID == 393452) parHvec = {3.01389, 80.0, -100.0};
  else if (DSID == 393404 || DSID == 393453) parHvec = {3.10528, 80.0, -120.0};
  else if (DSID == 393405 || DSID == 393454) parHvec = {3.40493, 80.0, -140.0};
  else if (DSID == 394173 || DSID == 394174) parHvec = {3.00022, 100.0, -102.0};
  else if (DSID == 393407 || DSID == 393456) parHvec = {3.00049, 100.0, -103.0};
  else if (DSID == 393408 || DSID == 393457) parHvec = {3.00142, 100.0, -105.0};
  else if (DSID == 393409 || DSID == 393458) parHvec = {3.0061, 100.0, -110.0};
  else if (DSID == 393410 || DSID == 393459) parHvec = {3.02817, 100.0, -120.0};
  else if (DSID == 393411 || DSID == 393460) parHvec = {3.15156, 100.0, -140.0};
  else if (DSID == 393412 || DSID == 393461) parHvec = {3.50164, 100.0, -160.0};
  else if (DSID == 394177 || DSID == 394178) parHvec = {3.0004, 150.0, -152.0};
  else if (DSID == 393463 || DSID == 393414) parHvec = {3.00091, 150.0, -153.0};
  else if (DSID == 393415 || DSID == 393464) parHvec = {3.00254, 150.0, -155.0};
  else if (DSID == 393416 || DSID == 393465) parHvec = {3.01033, 150.0, -160.0};
  else if (DSID == 393417 || DSID == 393466) parHvec = {3.0435, 150.0, -170.0};
  else if (DSID == 393418 || DSID == 393467) parHvec = {3.20529, 150.0, -190.0};
  else if (DSID == 393419 || DSID == 393468) parHvec = {3.6223, 150.0, -210.0};
  else if (DSID == 394181 || DSID == 394182) parHvec = {3.00047, 200.0, -202.0};
  else if (DSID == 393421 || DSID == 393470) parHvec = {3.00105, 200.0, -203.0};
  else if (DSID == 393422 || DSID == 393471) parHvec = {3.00293, 200.0, -205.0};
  else if (DSID == 393423 || DSID == 393472) parHvec = {3.01187, 200.0, -210.0};
  else if (DSID == 393424 || DSID == 393473) parHvec = {3.04929, 200.0, -220.0};
  else if (DSID == 393425 || DSID == 393474) parHvec = {3.22707, 200.0, -240.0};
  else if (DSID == 393426 || DSID == 393475) parHvec = {3.67466, 200.0, -260.0};
  else if (DSID == 394185 || DSID == 394186) parHvec = {3.0005, 250.0, -252.0};
  else if (DSID == 393477 || DSID == 393428) parHvec = {3.00112, 250.0, -253.0};
  else if (DSID == 393429 || DSID == 393478) parHvec = {3.00312, 250.0, -255.0};
  else if (DSID == 393430 || DSID == 393479) parHvec = {3.0126, 250.0, -260.0};
  else if (DSID == 393431 || DSID == 393480) parHvec = {3.05208, 250.0, -270.0};
  else if (DSID == 393432 || DSID == 393481) parHvec = {3.23798, 250.0, -290.0};
  else if (DSID == 393433 || DSID == 393482) parHvec = {3.7019, 250.0, -310.0};
  else if (DSID == 394189 || DSID == 394190) parHvec = {3.00051, 300.0, -302.0};
  else if (DSID == 393435 || DSID == 393484) parHvec = {3.00116, 300.0, -303.0};
  else if (DSID == 393436 || DSID == 393485) parHvec = {3.00322, 300.0, -305.0};
  else if (DSID == 393437 || DSID == 393486) parHvec = {3.013, 300.0, -310.0};
  else if (DSID == 393438 || DSID == 393487) parHvec = {3.05363, 300.0, -320.0};
  else if (DSID == 393439 || DSID == 393488) parHvec = {3.24421, 300.0, -340.0};
  else if (DSID == 393440 || DSID == 393489) parHvec = {3.71784, 300.0, -360.0};
  else if (DSID == 394193 || DSID == 394194) parHvec = {3.00053, 400.0, -402.0};
  else if (DSID == 393491 || DSID == 393442) parHvec = {3.0012, 400.0, -403.0};
  else if (DSID == 393443 || DSID == 393492) parHvec = {3.00333, 400.0, -405.0};
  else if (DSID == 393444 || DSID == 393493) parHvec = {3.0134, 400.0, -410.0};
  else if (DSID == 393445 || DSID == 393494) parHvec = {3.05521, 400.0, -420.0};
  else if (DSID == 393446 || DSID == 393495) parHvec = {3.25068, 400.0, -440.0};
  else if (DSID == 393447 || DSID == 393496) parHvec = {3.73475, 400.0, -460.0};
  else return winoBinoMllWeight; //return 1 for other cases

  double parH[3] = {parHvec.at(0), parHvec.at(1), parHvec.at(2)};
  double parS[3] = {1.0, parH[1], -1*parH[2]};

  double hig_w = getWinoBinoFuncMllDistr(mass,parH);
  double stop_w = getWinoBinoFuncMllDistr(mass,parS);
  winoBinoMllWeight = stop_w/hig_w;

  return winoBinoMllWeight;
}

// ------------------------------------------------------------------------------------------ //
double twoleptonSelector::getWinoBinoXsecWeight(int DSID){
  // See https://its.cern.ch/jira/browse/HIGGSINO-27: simply ratio of Higgsino and WinoBino xsec

  double winoBinoXsecWeight = 1.0;

  if (isHiggsinoN2C1p(DSID) || isHiggsinoN2C1m(DSID)){
    if (DSID == 393596) { //N2C1p_82_80
      winoBinoXsecWeight *= 3.89;
    } else if (DSID == 393400) { //N2C1p_83_80
      winoBinoXsecWeight *= 3.85;
    } else if (DSID == 393401) { //N2C1p_85_80
      winoBinoXsecWeight *= 3.79;
    } else if (DSID == 393402) { //N2C1p_90_80
      winoBinoXsecWeight *= 3.64;
    } else if (DSID == 393403) { //N2C1p_100_80
      winoBinoXsecWeight *= 3.36;
    } else if (DSID == 393404) { //N2C1p_120_80
      winoBinoXsecWeight *= 2.98;
    } else if (DSID == 393405) { //N2C1p_140_80
      winoBinoXsecWeight *= 2.73;
    } else if (DSID == 393406) { //N2C1p_180_80
      winoBinoXsecWeight *= 2.42;
    } else if (DSID == 394173) { //N2C1p_102_100
      winoBinoXsecWeight *= 3.97;
    } else if (DSID == 393407) { //N2C1p_103_100
      winoBinoXsecWeight *= 3.93;
    } else if (DSID == 393408) { //N2C1p_105_100
      winoBinoXsecWeight *= 3.87;
    } else if (DSID == 393409) { //N2C1p_110_100
      winoBinoXsecWeight *= 3.72;
    } else if (DSID == 393410) { //N2C1p_120_100
      winoBinoXsecWeight *= 3.48;
    } else if (DSID == 393411) { //N2C1p_140_100
      winoBinoXsecWeight *= 3.12;
    } else if (DSID == 393412) { //N2C1p_160_100
      winoBinoXsecWeight *= 2.87;
    } else if (DSID == 393413) { //N2C1p_200_100
      winoBinoXsecWeight *= 2.54;
    } else if (DSID == 394177) { //N2C1p_152_150
      winoBinoXsecWeight *= 3.99;
    } else if (DSID == 393414) { //N2C1p_153_150
      winoBinoXsecWeight *= 3.96;
    } else if (DSID == 393415) { //N2C1p_155_150
      winoBinoXsecWeight *= 3.92;
    } else if (DSID == 393416) { //N2C1p_160_150
      winoBinoXsecWeight *= 3.81;
    } else if (DSID == 393417) { //N2C1p_170_150
      winoBinoXsecWeight *= 3.63;
    } else if (DSID == 393418) { //N2C1p_190_150
      winoBinoXsecWeight *= 3.33;
    } else if (DSID == 393419) { //N2C1p_210_150
      winoBinoXsecWeight *= 3.10;
    } else if (DSID == 393420) { //N2C1p_250_150
      winoBinoXsecWeight *= 2.76;
    } else if (DSID == 394181) { //N2C1p_202_200
      winoBinoXsecWeight *= 3.99;
    } else if (DSID == 393421) { //N2C1p_203_200
      winoBinoXsecWeight *= 3.97;
    } else if (DSID == 393422) { //N2C1p_205_200
      winoBinoXsecWeight *= 3.94;
    } else if (DSID == 393423) { //N2C1p_210_200
      winoBinoXsecWeight *= 3.86;
    } else if (DSID == 393424) { //N2C1p_220_200
      winoBinoXsecWeight *= 3.71;
    } else if (DSID == 393425) { //N2C1p_240_200
      winoBinoXsecWeight *= 3.45;
    } else if (DSID == 393426) { //N2C1p_260_200
      winoBinoXsecWeight *= 3.24;
    } else if (DSID == 393427) { //N2C1p_300_200
      winoBinoXsecWeight *= 2.91;
    } else if (DSID == 394185) { //N2C1p_252_250
      winoBinoXsecWeight *= 4.00;
    } else if (DSID == 393428) { //N2C1p_253_250
      winoBinoXsecWeight *= 3.98;
    } else if (DSID == 393429) { //N2C1p_255_250
      winoBinoXsecWeight *= 3.95;
    } else if (DSID == 393430) { //N2C1p_260_250
      winoBinoXsecWeight *= 3.88;
    } else if (DSID == 393431) { //N2C1p_270_250
      winoBinoXsecWeight *= 3.75;
    } else if (DSID == 393432) { //N2C1p_290_250
      winoBinoXsecWeight *= 3.53;
    } else if (DSID == 393433) { //N2C1p_310_250
      winoBinoXsecWeight *= 3.33;
    } else if (DSID == 393434) { //N2C1p_350_250
      winoBinoXsecWeight *= 3.03;
    } else if (DSID == 394189) { //N2C1p_302_300
      winoBinoXsecWeight *= 4.00;
    } else if (DSID == 393435) { //N2C1p_303_300
      winoBinoXsecWeight *= 3.98;
    } else if (DSID == 393436) { //N2C1p_305_300
      winoBinoXsecWeight *= 3.96;
    } else if (DSID == 393437) { //N2C1p_310_300
      winoBinoXsecWeight *= 3.90;
    } else if (DSID == 393438) { //N2C1p_320_300
      winoBinoXsecWeight *= 3.79;
    } else if (DSID == 393439) { //N2C1p_340_300
      winoBinoXsecWeight *= 3.58;
    } else if (DSID == 393440) { //N2C1p_360_300
      winoBinoXsecWeight *= 3.41;
    } else if (DSID == 393441) { //N2C1p_400_300
      winoBinoXsecWeight *= 3.11;
    } else if (DSID == 394193) { //N2C1p_402_400
      winoBinoXsecWeight *= 4.00;
    } else if (DSID == 393442) { //N2C1p_403_400
      winoBinoXsecWeight *= 3.99;
    } else if (DSID == 393443) { //N2C1p_405_400
      winoBinoXsecWeight *= 3.97;
    } else if (DSID == 393444) { //N2C1p_410_400
      winoBinoXsecWeight *= 3.92;
    } else if (DSID == 393445) { //N2C1p_420_400
      winoBinoXsecWeight *= 3.83;
    } else if (DSID == 393446) { //N2C1p_440_400
      winoBinoXsecWeight *= 3.66;
    } else if (DSID == 393447) { //N2C1p_460_400
      winoBinoXsecWeight *= 3.50;
    } else if (DSID == 393448) { //N2C1p_500_400
      winoBinoXsecWeight *= 3.23;
    } else if (DSID == 393597) { //N2C1m_82_80
      winoBinoXsecWeight *= 3.98;
    } else if (DSID == 393449) { //N2C1m_83_80
      winoBinoXsecWeight *= 3.94;
    } else if (DSID == 393450) { //N2C1m_85_80
      winoBinoXsecWeight *= 3.86;
    } else if (DSID == 393451) { //N2C1m_90_80
      winoBinoXsecWeight *= 3.69;
    } else if (DSID == 393452) { //N2C1m_100_80
      winoBinoXsecWeight *= 3.34;
    } else if (DSID == 393453) { //N2C1m_120_80
      winoBinoXsecWeight *= 2.94;
    } else if (DSID == 393454) { //N2C1m_140_80
      winoBinoXsecWeight *= 2.68;
    } else if (DSID == 393455) { //N2C1m_180_80
      winoBinoXsecWeight *= 2.35;
    } else if (DSID == 394174) { //N2C1m_102_100
      winoBinoXsecWeight *= 3.98;
    } else if (DSID == 393456) { //N2C1m_103_100
      winoBinoXsecWeight *= 3.94;
    } else if (DSID == 393457) { //N2C1m_105_100
      winoBinoXsecWeight *= 3.88;
    } else if (DSID == 393458) { //N2C1m_110_100
      winoBinoXsecWeight *= 3.72;
    } else if (DSID == 393459) { //N2C1m_120_100
      winoBinoXsecWeight *= 3.46;
    } else if (DSID == 393460) { //N2C1m_140_100
      winoBinoXsecWeight *= 3.08;
    } else if (DSID == 393461) { //N2C1m_160_100
      winoBinoXsecWeight *= 2.82;
    } else if (DSID == 393462) { //N2C1m_200_100
      winoBinoXsecWeight *= 2.47;
    } else if (DSID == 394178) { //N2C1m_152_150
      winoBinoXsecWeight *= 4.00;
    } else if (DSID == 393463) { //N2C1m_153_150
      winoBinoXsecWeight *= 3.97;
    } else if (DSID == 393464) { //N2C1m_155_150
      winoBinoXsecWeight *= 3.93;
    } else if (DSID == 393465) { //N2C1m_160_150
      winoBinoXsecWeight *= 3.81;
    } else if (DSID == 393466) { //N2C1m_170_150
      winoBinoXsecWeight *= 3.62;
    } else if (DSID == 393467) { //N2C1m_190_150
      winoBinoXsecWeight *= 3.30;
    } else if (DSID == 393468) { //N2C1m_210_150
      winoBinoXsecWeight *= 3.05;
    } else if (DSID == 393469) { //N2C1m_250_150
      winoBinoXsecWeight *= 2.69;
    } else if (DSID == 394182) { //N2C1m_202_200
      winoBinoXsecWeight *= 4.01;
    } else if (DSID == 393470) { //N2C1m_203_200
      winoBinoXsecWeight *= 3.99;
    } else if (DSID == 393471) { //N2C1m_205_200
      winoBinoXsecWeight *= 3.95;
    } else if (DSID == 393472) { //N2C1m_210_200
      winoBinoXsecWeight *= 3.86;
    } else if (DSID == 393473) { //N2C1m_220_200
      winoBinoXsecWeight *= 3.70;
    } else if (DSID == 393474) { //N2C1m_240_200
      winoBinoXsecWeight *= 3.42;
    } else if (DSID == 393475) { //N2C1m_260_200
      winoBinoXsecWeight *= 3.20;
    } else if (DSID == 393476) { //N2C1m_300_200
      winoBinoXsecWeight *= 2.85;
    } else if (DSID == 394186) { //N2C1m_252_250
      winoBinoXsecWeight *= 4.01;
    } else if (DSID == 393477) { //N2C1m_253_250
      winoBinoXsecWeight *= 3.99;
    } else if (DSID == 393478) { //N2C1m_255_250
      winoBinoXsecWeight *= 3.96;
    } else if (DSID == 393479) { //N2C1m_260_250
      winoBinoXsecWeight *= 3.89;
    } else if (DSID == 393480) { //N2C1m_270_250
      winoBinoXsecWeight *= 3.75;
    } else if (DSID == 393481) { //N2C1m_290_250
      winoBinoXsecWeight *= 3.50;
    } else if (DSID == 393482) { //N2C1m_310_250
      winoBinoXsecWeight *= 3.30;
    } else if (DSID == 393483) { //N2C1m_350_250
      winoBinoXsecWeight *= 2.97;
    } else if (DSID == 394190) { //N2C1m_302_300
      winoBinoXsecWeight *= 4.01;
    } else if (DSID == 393484) { //N2C1m_303_300
      winoBinoXsecWeight *= 4.00;
    } else if (DSID == 393485) { //N2C1m_305_300
      winoBinoXsecWeight *= 3.97;
    } else if (DSID == 393486) { //N2C1m_310_300
      winoBinoXsecWeight *= 3.90;
    } else if (DSID == 393487) { //N2C1m_320_300
      winoBinoXsecWeight *= 3.78;
    } else if (DSID == 393488) { //N2C1m_340_300
      winoBinoXsecWeight *= 3.56;
    } else if (DSID == 393489) { //N2C1m_360_300
      winoBinoXsecWeight *= 3.37;
    } else if (DSID == 393490) { //N2C1m_400_300
      winoBinoXsecWeight *= 3.05;
    } else if (DSID == 394194) { //N2C1m_402_400
      winoBinoXsecWeight *= 4.01;
    } else if (DSID == 393491) { //N2C1m_403_400
      winoBinoXsecWeight *= 4.00;
    } else if (DSID == 393492) { //N2C1m_405_400
      winoBinoXsecWeight *= 3.98;
    } else if (DSID == 393493) { //N2C1m_410_400
      winoBinoXsecWeight *= 3.92;
    } else if (DSID == 393494) { //N2C1m_420_400
      winoBinoXsecWeight *= 3.82;
    } else if (DSID == 393495) { //N2C1m_440_400
      winoBinoXsecWeight *= 3.63;
    } else if (DSID == 393496) { //N2C1m_460_400
      winoBinoXsecWeight *= 3.47;
    } else if (DSID == 393497) { //N2C1m_500_400
      winoBinoXsecWeight *= 3.18;
    }
  } else if (isHiggsinoC1C1(DSID) || isHiggsinoN2N1(DSID)) {
    winoBinoXsecWeight *= 0.0;
  } else {
    winoBinoXsecWeight *= 1.0;
  }

  return winoBinoXsecWeight;
}
// ------------------------------------------------------------------------------------------ //
double twoleptonSelector::getWinoBinoBrFracWeight(int DSID){
  // See https://its.cern.ch/jira/browse/HIGGSINO-27:
  // unclear if there is a difference here or not

  double winoBinoBrFracWeight = 1.0;

  if (getHiggsinoDM(DSID) == 3 || getHiggsinoDM(DSID) == 2){ //fixme
    winoBinoBrFracWeight *= (0.065005/0.099413);
  } else if (getHiggsinoDM(DSID) == 5){
    winoBinoBrFracWeight *= (0.064803/0.098670);
  } else if (getHiggsinoDM(DSID) == 10){
    winoBinoBrFracWeight *= (0.075414/0.112346);
  } else if (getHiggsinoDM(DSID) == 20){
    winoBinoBrFracWeight *= (0.073124/0.109839);
  } else if (getHiggsinoDM(DSID) == 40){
    winoBinoBrFracWeight *= (0.072993/0.105571);
  } else if (getHiggsinoDM(DSID) == 60){
    winoBinoBrFracWeight *= (0.076728/0.104532);
  } else if (getHiggsinoDM(DSID) == 100){
    winoBinoBrFracWeight *= (0.100974/0.100974);
  } else {
    winoBinoBrFracWeight *= 1.0;
  }

  return winoBinoBrFracWeight;
}
// ------------------------------------------------------------------------------------------ //
double twoleptonSelector::get_NUHM2_N1_mass(int m12){
  // Masses of N1, N2 for NUHM2
  double par_NUHM2_m12_300[2] = {105.631409, 161.307800};
  double par_NUHM2_m12_350[2] = {115.618103, 161.675125};
  double par_NUHM2_m12_400[2] = {122.974541, 161.142700};
  double par_NUHM2_m12_500[2] = {132.279099, 160.295654};
  double par_NUHM2_m12_600[2] = {137.607727, 159.657379};
  double par_NUHM2_m12_700[2] = {140.978836, 159.170868};
  double par_NUHM2_m12_800[2] = {143.288208, 158.783905};
  double mass_N1 = 0;
  if (m12 == 300){
    mass_N1 = par_NUHM2_m12_300[0];
  }else if (m12 == 350) {
    mass_N1 = par_NUHM2_m12_350[0];
  }else if (m12 == 400){
    mass_N1 = par_NUHM2_m12_400[0];
  }else if (m12 == 500){
    mass_N1 = par_NUHM2_m12_500[0];
  }else if (m12 == 600){
    mass_N1 = par_NUHM2_m12_600[0];
  }else if (m12 == 700){
    mass_N1 = par_NUHM2_m12_700[0];
  }else if (m12 == 800){
    mass_N1 = par_NUHM2_m12_800[0];
  }
  return mass_N1;
}
// ------------------------------------------------------------------------------------------ //
double twoleptonSelector::get_NUHM2_N2_mass(int m12){
  // Masses of N1, N2 for NUHM2
  double par_NUHM2_m12_300[2] = {105.631409, 161.307800};
  double par_NUHM2_m12_350[2] = {115.618103, 161.675125};
  double par_NUHM2_m12_400[2] = {122.974541, 161.142700};
  double par_NUHM2_m12_500[2] = {132.279099, 160.295654};
  double par_NUHM2_m12_600[2] = {137.607727, 159.657379};
  double par_NUHM2_m12_700[2] = {140.978836, 159.170868};
  double par_NUHM2_m12_800[2] = {143.288208, 158.783905};
  double mass_N2 = 0;
  if (m12 == 300){
    mass_N2 = par_NUHM2_m12_300[1];
  }else if (m12 == 350){
    mass_N2 = par_NUHM2_m12_350[1];
  }else if (m12 == 400){
    mass_N2 = par_NUHM2_m12_400[1];
  }else if (m12 == 500){
    mass_N2 = par_NUHM2_m12_500[1];
  }else if (m12 == 600){
    mass_N2 = par_NUHM2_m12_600[1];
  }else if (m12 == 700){
    mass_N2 = par_NUHM2_m12_700[1];
  }else if (m12 == 800){
    mass_N2 = par_NUHM2_m12_800[1];
  }
  return mass_N2;
}
// ------------------------------------------------------------------------------------------ //
double twoleptonSelector::get_NUHM2_weight(int DSID, double mass, int m12){
    // Don't calculate a weight unless it is one of the specified DSIDs
    double NUHM2_weight = 1.0;

    double NUHM2_N1_mass = get_NUHM2_N1_mass(m12);
    double NUHM2_N2_mass = get_NUHM2_N2_mass(m12);
    double par_NUHM2[3] = {1., NUHM2_N1_mass, -1. * NUHM2_N2_mass};

    double NUHM2_w = getWinoBinoFuncMllDistr(mass, par_NUHM2);

    // Higgsino_160_100 <---> m12 = 350
    // Higgsino_190_150 <---> m12 = 400, 500, 600
    // Higgsino_170_150 <---> m12 = 700, 800

    // Area ratio:
    // NUHM2 m12=300 / Higgsino 160_100: area_f5/area_f2=0.704021
    // NUHM2 m12=350 / Higgsino 160_100: area_f6/area_f2=0.277169
    // NUHM2 m12=700 / Higgsino 170_150: area_f10/area_f3=0.511249
    // NUHM2 m12=800 / Higgsino 170_150: area_f11/area_f3=0.232064

    //new areas see https://its.cern.ch/jira/browse/HIGGSINO-28?focusedCommentId=1663479&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-1663479
    //NUHM2 m12=400 / Higgsino 190_150: area_f7/area_f4=0.457146
    //NUHM2 m12=500 / Higgsino 190_150: area_f8/area_f4=0.100594
    //NUHM2 m12=600 / Higgsino 190_150: area_f9/area_f4=0.0311175

    double par_Higgsino[3] = {0., 0., 0.};

    // Higgsino_160_100
    if (DSID == 393510 || // N2N1
        DSID == 393412 || // N2C1p
        DSID == 393461) { // N2C1m
      if (m12 == 350) {
        par_Higgsino[0] = 0.277169; // normalization factor in order that the two curves have the same area.
        par_Higgsino[1] = 100.; // N1 mass higgsino
        par_Higgsino[2] = -160.; // N2 mass higgsino
      }
    }
    // Higgsino_170_150
    else if (DSID == 393515 || // N2N1
           DSID == 393417 || // N2C1p
           DSID == 393466) { // N2C1m
      if (m12 == 700) {
        par_Higgsino[0] = 0.511249;
      }
      else if (m12 == 800) {
        par_Higgsino[0] = 0.232064;
      }
      par_Higgsino[1] = 150.;
      par_Higgsino[2] = -170.;
    }
    // Higgsino_190_150
    else if (DSID == 393516 || // N2N1
           DSID == 393418 || // N2C1p
           DSID == 393467) { // N2C1m
      if (m12 == 400) {
        par_Higgsino[0] = 0.457146;
      }
      else if (m12 == 500) {
        par_Higgsino[0] = 0.100594;
      }
      else if (m12 == 600) {
        par_Higgsino[0] = 0.0311175;
      }
      par_Higgsino[1] = 150.;
      par_Higgsino[2] = -190.;
    }

    double Higgsino_w = getWinoBinoFuncMllDistr(mass, par_Higgsino);
    double weight_mass = NUHM2_w / Higgsino_w;

    if(isHiggsinoN2C1m(DSID) || isHiggsinoN2C1p(DSID) || isHiggsinoN2N1(DSID)){
      NUHM2_weight *= weight_mass;
    }

    // weight for the different xsec
    // weight = Xsec_NUHM2 / Xsec_Higgsino
    // For N2N1
    if (DSID == 393510 && m12 == 350)
        NUHM2_weight *= 0.500489103669;
    else if (DSID == 393516 && m12 == 400)
        NUHM2_weight *= 1.40609496035;
    else if (DSID == 393516 && m12 == 500)
        NUHM2_weight *= 1.48420174312;
    else if (DSID == 393516 && m12 == 600)
        NUHM2_weight *= 1.49620688527;
    else if (DSID == 393515 && m12 == 700)
        NUHM2_weight *= 1.18114972806;
    else if (DSID == 393515 && m12 == 800)
        NUHM2_weight *= 1.17312186754;
    // For C1C1
    else if (DSID == 393559 && m12 == 350)
        NUHM2_weight *= 0.809295197893;
    else if (DSID == 393565 && m12 == 400)
        NUHM2_weight *= 1.85763670956;
    else if (DSID == 393565 && m12 == 500)
        NUHM2_weight *= 1.60709633189;
    else if (DSID == 393565 && m12 == 600)
        NUHM2_weight *= 1.48886200733;
    else if (DSID == 393564 && m12 == 700)
        NUHM2_weight *= 1.14281934535;
    else if (DSID == 393564 && m12 == 800)
        NUHM2_weight *= 1.10758453419;
    // For N2C1p
    else if (DSID == 393412 && m12 == 350)
        NUHM2_weight *= 0.75334324027;
    else if (DSID == 393418 && m12 == 400)
        NUHM2_weight *= 1.61810558673;
    else if (DSID == 393418 && m12 == 500)
        NUHM2_weight *= 1.61195100629;
    else if (DSID == 393418 && m12 == 600)
        NUHM2_weight *= 1.60540039476;
    else if (DSID == 393417 && m12 == 700)
        NUHM2_weight *= 1.17032101263;
    else if (DSID == 393417 && m12 == 800)
        NUHM2_weight *= 1.16731420042;
    // For N2C1m
    else if (DSID == 393461 && m12 == 350)
        NUHM2_weight *= 0.751651917122;
    else if (DSID == 393467 && m12 == 400)
        NUHM2_weight *= 1.68915991429;
    else if (DSID == 393467 && m12 == 500)
        NUHM2_weight *= 1.68026588159;
    else if (DSID == 393467 && m12 == 600)
        NUHM2_weight *= 1.66943310114;
    else if (DSID == 393466 && m12 == 700)
        NUHM2_weight *= 1.1917838431;
    else if (DSID == 393466 && m12 == 800)
        NUHM2_weight *= 1.18958896195;

    return NUHM2_weight;

}
// ------------------------------------------------------------------------------------------ //
void twoleptonSelector::emulateL1TopoTriggers(ConfigMgr*& configMgr){

  // try to emulate
  // HLT_2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30
  // HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30
  // HLT_e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30
  // HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50
  // HLT_e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50

  // retrieve all needed containers first
  xAOD::TEvent* m_event = configMgr->objectTools->m_trigTool->GetTEvent();

  //retrieve L1 and HLT met containers
  const xAOD::EnergySumRoI* l1MetObject(0);
  const xAOD::TrigMissingETContainer* pufitCont(0);
  //If containers don't exist, return and do not perform emualation
  if( !m_event->retrieve(l1MetObject, "LVL1EnergySumRoI" ).isSuccess() || !m_event->retrieve(pufitCont, "HLT_xAOD__TrigMissingETContainer_TrigEFMissingET_topocl_PUC" ).isSuccess() ){
    std::cout << "Cannot emulate met triggers. L1 or HLT MET containers do not exits" << std::endl;
    return;
  }

  //retrieve L1 and HLT jet containers
  const xAOD::JetRoIContainer* l1_jet = 0;
  const xAOD::JetContainer* hlt_jet = 0;
  //If containers don't exist, return and do not perform emualation
  if( !m_event->retrieve(l1_jet, "LVL1JetRoIs" ).isSuccess() || !m_event->retrieve(hlt_jet, "HLT_xAOD__JetContainer_a4tcemsubjesFS" ).isSuccess()  ) {
    std::cout << "Cannot emulate met triggers. L1 or HLT jet containers do not exits" << std::endl;
    return ;
  }

  // get L1 MET
  float l1_mex = l1MetObject->exMiss() * 0.001; //exMiss in MeV, trigger threshold is in GeV
  float l1_mey = l1MetObject->eyMiss() * 0.001;
  float l1_met = sqrt(l1_mex*l1_mex + l1_mey*l1_mey);
  float l1_met_phi = atan2(l1_mey, l1_mex);
  if (l1MetObject->energyT()>0) {
    l1_met_phi = atan2(l1_mey, l1_mex);
  }
  else {
    l1_met = -999;
    l1_met_phi = -999;
  }
  TLorentzVector l1met_vector;
  l1met_vector.SetPtEtaPhiE(l1_met,0,-1.*l1_met_phi,l1_met);

  // get HLT MET
  float hlt_mex = pufitCont->front()->ex() * 0.001;
  float hlt_mey = pufitCont->front()->ey() * 0.001;
  float hlt_met = sqrt(hlt_mex*hlt_mex + hlt_mey*hlt_mey);
  float hlt_met_phi = atan2(hlt_mey, hlt_mex);
  TLorentzVector hltmet_vector;
  hltmet_vector.SetPtEtaPhiE(hlt_met,0,-1.*hlt_met_phi,hlt_met);

  // get leading and subleading L1 jet
  TLorentzVector leading_l1jet;
  leading_l1jet.SetPtEtaPhiM(0,0,0,0);
  TLorentzVector sublead_l1jet;
  sublead_l1jet.SetPtEtaPhiM(0,0,0,0);
  TLorentzVector thisjet;
  for (auto p : * l1_jet) {
    //L1 jet with pT > 20 GeV, and |Eta| < 3.1
    thisjet.SetPtEtaPhiM(p->et8x8(),p->eta(),p->phi(),0);
    if(TMath::Abs(p->eta())<3.1){
      if(p->et8x8()>leading_l1jet.Pt()){
        sublead_l1jet=leading_l1jet;
        leading_l1jet = thisjet;
      }
      else if(p->et8x8()>sublead_l1jet.Pt()){
        sublead_l1jet=thisjet;
      }
    }
  }

  // get leading and subleading HLT jet
  const xAOD::Jet* ldg = 0;
  const xAOD::Jet* subldg = 0;
  for (auto j : *hlt_jet){
    // require pT > 20 GeV and |eta| < 3.2 according to
    // https://svnweb.cern.ch/trac/atlasoff/browser/Trigger/TrigHypothesis/TrigHLTJetHypo/trunk/src/TrigEFDPhiMetJetAllTE.cxx
    if (j->pt()<20e3 || TMath::Abs(j->eta() > 3.2 )){
      continue;
    }
    if (!ldg || j->pt() > ldg->pt()) {
      subldg = ldg;
      ldg = j;
    }
    else if (!subldg || j->pt() > subldg->pt()){
      subldg = j;
    }
  }
  TLorentzVector leading_hltjet;
  TLorentzVector sublead_hltjet;
  if (ldg) {
    leading_hltjet = TLorentzVector(ldg->p4());
  }
  if (subldg){
    sublead_hltjet = TLorentzVector(subldg->p4());
  }

  // get L1Topo decision
  float minDPHI_2J20XE=9999;
  if(leading_l1jet.Pt()>20e3 && fabs(leading_l1jet.DeltaPhi(l1met_vector))<minDPHI_2J20XE) minDPHI_2J20XE=fabs(leading_l1jet.DeltaPhi(l1met_vector));
  if(sublead_l1jet.Pt()>20e3 && fabs(sublead_l1jet.DeltaPhi(l1met_vector))<minDPHI_2J20XE) minDPHI_2J20XE=fabs(sublead_l1jet.DeltaPhi(l1met_vector));
  bool DPHI_2J20XE = (minDPHI_2J20XE>=1.0);

  // similar on HLT level
  float min2dphi10=9999;
  if(leading_hltjet.Pt()>20e3 && fabs(leading_hltjet.DeltaPhi(hltmet_vector))<min2dphi10) min2dphi10=fabs(leading_hltjet.DeltaPhi(hltmet_vector));
  if(sublead_hltjet.Pt()>20e3 && fabs(sublead_hltjet.DeltaPhi(hltmet_vector))<min2dphi10) min2dphi10=fabs(sublead_hltjet.DeltaPhi(hltmet_vector));
  bool _2dphi10 = (min2dphi10>=1.0);

  configMgr->treeMaker->setFloatVariable("L1_met_Et", l1_met);
  configMgr->treeMaker->setFloatVariable("L1_jet1Pt", leading_l1jet.Pt()/1e3);
  configMgr->treeMaker->setFloatVariable("HLT_met_Et", hlt_met);
  configMgr->treeMaker->setFloatVariable("HLT_jet1Pt", leading_hltjet.Pt()/1e3);
  configMgr->treeMaker->setBoolVariable("DPHI_2J20XE", DPHI_2J20XE);
  configMgr->treeMaker->setBoolVariable("2dphi10", _2dphi10);

  bool emulatedDecision = false;
  // HLT_2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30
  if (configMgr->obj->evt.getHLTEvtTrigDec("HLT_2mu4") && leading_hltjet.Pt()>20e3 && hlt_met>40 && _2dphi10 && leading_l1jet.Pt()>20e3 && l1_met>30 && DPHI_2J20XE) {
    emulatedDecision = true;
  }
  configMgr->treeMaker->setBoolVariable("HLT_2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI_J20s2XE30", emulatedDecision);
  emulatedDecision = false;

  // HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30
  if (configMgr->obj->evt.getHLTEvtTrigDec("HLT_mu4") && leading_hltjet.Pt()>90e3 && hlt_met>90 && _2dphi10 && leading_l1jet.Pt()>50e3 && l1_met>50 && DPHI_2J20XE) {
    emulatedDecision = true;
  }
  configMgr->treeMaker->setBoolVariable("HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI_J20s2XE30", emulatedDecision);
  emulatedDecision = false;

  // HLT_e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30
  // sadly HLT_e5_lhvloose_nod0 is not in the TriggerList for SUSY16,
  // so we ask for HLT_e5_lhmedium_nod0_mu4_xe40_mht_L1MU4_J20_XE30_DPHI-J20sXE30 instead
  if (configMgr->obj->evt.getHLTEvtTrigDec("HLT_e5_lhmedium_nod0_mu4_xe40_mht_L1MU4_J20_XE30_DPHI-J20sXE30") && configMgr->obj->evt.getHLTEvtTrigDec("HLT_mu4") && leading_hltjet.Pt()>30e3 && hlt_met>40 && _2dphi10 && leading_l1jet.Pt()>30e3 && l1_met>40 && DPHI_2J20XE) {
    emulatedDecision = true;
  }
  configMgr->treeMaker->setBoolVariable("HLT_e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI_J20s2XE30", emulatedDecision);
  emulatedDecision = false;

  // HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50
  // sadly HLT_2e5_lhvloose_nod0 is not in the trigger menu, so we ask for
  // HLT_2e5_lhmedium_nod0_j50_xe80_mht_L1J40_XE50_DPHI-J20sXE50 instead
  if (configMgr->obj->evt.getHLTEvtTrigDec("HLT_2e5_lhmedium_nod0_j50_xe80_mht_L1J40_XE50_DPHI-J20sXE50") && leading_hltjet.Pt()>40e3 && hlt_met>70 && _2dphi10 && leading_l1jet.Pt()>40e3 && l1_met>50 && DPHI_2J20XE) {
    emulatedDecision = true;
  }
  configMgr->treeMaker->setBoolVariable("HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI_J20s2XE50", emulatedDecision);
  emulatedDecision = false;

  // HLT_e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50
  // sadly HLT_e5_lhloose_nod0 is not in the TriggerList for SUSY16
  // no other trigger usable for emulation :/
  if (configMgr->obj->evt.getHLTEvtTrigDec("HLT_e5_lhloose_nod0") && leading_hltjet.Pt()>50e3 && hlt_met>70 && _2dphi10 && leading_l1jet.Pt()>40e3 && l1_met>50 && DPHI_2J20XE) {
    emulatedDecision = true;
  }
  configMgr->treeMaker->setBoolVariable("HLT_e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI_J20s2XE50", emulatedDecision);
  emulatedDecision = false;
}
// ------------------------------------------------------------------------------------------ //
void twoleptonSelector::finalize(ConfigMgr*& configMgr)
{

  /*
   This method is called at the very end of the job. Can be used to merge cutflow histograms 
   for example. See CutFlowTool::mergeCutFlows(...)
  */

}
// ------------------------------------------------------------------------------------------ //
