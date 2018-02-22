#include "SusySkimHiggsino/CompressedSelector.h"
#include <algorithm>

// ------------------------------------------------------------------------------------------ //
CompressedSelector::CompressedSelector() :
  BaseUser("SusySkimHiggsino", "CompressedSelector")
{

}
// ------------------------------------------------------------------------------------------ //
void CompressedSelector::setup(ConfigMgr*& configMgr)
{

  // Make a cutflow stream
  configMgr->cutflow->defineCutFlow("cutFlow", configMgr->treeMaker->getFile("tree"));

  // Use object def from SUSYTools
  configMgr->obj->useSUSYToolsSignalDef(true);

  // Bad muon veto
  configMgr->obj->applyBadMuonVeto(true);

  // Muon cuts
  configMgr->obj->setBaselineMuonPt(4.0);
  configMgr->obj->setBaselineMuonEta(2.50);
  configMgr->obj->setSignalMuonPt(4.0);
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
  /*
  configMgr->addTriggerAna("HLT_xe70",276262,284484,"metTrig");
  configMgr->addTriggerAna("HLT_xe90_mht_L1XE50",296939,302872,"metTrig");
  configMgr->addTriggerAna("HLT_xe100_mht_L1XE50",302919,303892,"metTrig");
  configMgr->addTriggerAna("HLT_xe110_mht_L1XE50",303943,-1,"metTrig");

  // Single lepton triggers
  // Stores the trigger decision and corresponding SFs ( and their systematics )
  configMgr->addTriggerAna("HLT_mu20_iloose_L1MU15_OR_HLT_mu40",276262,284484,"singleMuonTrig");
  configMgr->addTriggerAna("HLT_mu26_ivarmedium_OR_HLT_mu50",297730,-1,"singleMuonTrig");
  configMgr->addTriggerAna("SINGLE_E_2015_e24_lhmedium_L1EM20VH_OR_e60_lhmedium_OR_e120_lhloose_2016_e26_lhtight_nod0_ivarloose_OR_e60_lhmedium_nod0_OR_e140_lhloose_nod0",276262,-1, "singleElectronTrig");

  //
  // ---- Output trees ---- //
  //
  
  // Emulated Triggers
  configMgr->treeMaker->addBoolVariable("HLT_2mu4_j85_xe50_mht_emul", 0);
  configMgr->treeMaker->addBoolVariable("HLT_mu4_j125_xe90_mht_emul", 0);
  */
  // Global vars
  configMgr->treeMaker->addFloatVariable("mu",0.0);
  configMgr->treeMaker->addIntVariable("nVtx",0.0);

  // Lepton vars
  configMgr->treeMaker->addIntVariable("nLep_base",0);
  configMgr->treeMaker->addIntVariable("nLep_signal",0);
  configMgr->treeMaker->addIntVariable("nEle_signal",0);
  configMgr->treeMaker->addIntVariable("nMu_signal",0);

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
  /*
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
  */
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

  // 3L analysis variables
  configMgr->treeMaker->addBoolVariable("L3_SFOS", 0.0);
  configMgr->treeMaker->addFloatVariable("L3_mt",0.0);
  configMgr->treeMaker->addFloatVariable("L3_mtMin",0.0);
  configMgr->treeMaker->addFloatVariable("L3_mt_minMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_mt_maxMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_mll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_mll_mtMin",0.0);
  configMgr->treeMaker->addFloatVariable("L3_minMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_maxMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_m3l",0.0);
  configMgr->treeMaker->addFloatVariable("L3_pT3l",0.0);
  configMgr->treeMaker->addFloatVariable("L3_pT3lOverMet",0.0);
  configMgr->treeMaker->addFloatVariable("L3_pT3lOverMetTrack",0.0);
  configMgr->treeMaker->addFloatVariable("L3_Ht3l",0.0);
  configMgr->treeMaker->addFloatVariable("L3_METOverHTLep",-1.);
  configMgr->treeMaker->addFloatVariable("L3_METTrackOverHTLep",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZlep",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZlep_mtMin",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZlep_minMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZlep_maxMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZWlep",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZWlep_mtMin",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZWlep_minMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_RZWlep_maxMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMet",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMet_mtMin",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMet_minMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMet_maxMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMetTrack_mtMin",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMetTrack_minMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiWLepMetTrack_maxMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMet",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMet_mtMin",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMet_minMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMet_maxMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMetTrack_mtMin",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMetTrack_minMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhiZMetTrack_maxMll",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhi3lMet",-1.);
  configMgr->treeMaker->addFloatVariable("L3_dPhi3lMetTrack",-1.);
  configMgr->treeMaker->addFloatVariable("L3_ptZ",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptZ_mtMin",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptZ_minMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptZ_maxMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptW",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptW_mtMin",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptW_minMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptW_maxMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptWMetTrack",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptWMetTrack_mtMin",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptWMetTrack_minMll",0.0);
  configMgr->treeMaker->addFloatVariable("L3_ptWMetTrack_maxMll",0.0);

  //RJ translation variables (only using MET, not track MET)
  configMgr->treeMaker->addFloatVariable("L3_pTISR",0.0);
  configMgr->treeMaker->addFloatVariable("L3_deltaPhiISRMet",0.0);
  configMgr->treeMaker->addFloatVariable("L3_RISR",0.0);
  configMgr->treeMaker->addFloatVariable("L3_pTCM",0.0);
  configMgr->treeMaker->addFloatVariable("L3_HT31",0.0);
  configMgr->treeMaker->addFloatVariable("L3_H31",0.0);

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
    //"HLT_xe110_mht_L1XE50", // emulated in SUSYTools
    //"HLT_xe90_mht_L1XE40", // emulated in TriggerTools
    //"HLT_xe50_mht_L1XE20", //emulated in TriggerTools
    //"HLT_j85_L1J40", //emulated in TriggerTools
    //"HLT_j125_L1J50", //emulated in TriggerTools
    //lepton triggers    
    //2017 triggers
    "HLT_e26_lhtight_nod0_ivarloose_L1EM22VHIM",
    "HLT_e28_lhtight_nod0_ivarloose",
    "HLT_e28_lhtight_nod0_ivarloose_L1EM24VHIM",
    "HLT_e60_lhmedium_nod0_L1EM24VHI",
    "HLT_mu60",
    "HLT_mu60_0eta105_msonly",
    "HLT_2e17_lhvloose_nod0_L12EM15VHI",
    "HLT_2e24_lhvloose_nod0",
    "HLT_e26_lhmedium_nod0_mu8noL1",
    "HLT_mu22_mu8noL1_calotag_0eta010",
    "HLT_mu24_mu8noL1",
    "HLT_mu24_mu8noL1_calotag_0eta010",
    "HLT_mu26_mu10noL1",
    //2016 triggers
    "HLT_e24_lhtight_nod0_ivarloose",
    "HLT_e26_lhtight_nod0_ivarloose",
    "HLT_e60_lhmedium_nod0",
    "HLT_e60_medium",
    "HLT_mu24_iloose",
    "HLT_mu24_iloose_L1MU15",
    "HLT_mu26_ivarmedium",
    "HLT_mu24_ivarloose",
    "HLT_mu24_ivarloose_L1MU15",
    "HLT_mu24_imedium",
    "HLT_mu26_imedium",
    "HLT_mu50",
    "HLT_2e15_lhvloose_nod0_L12EM13VH",
    "HLT_2e17_lhvloose_nod0",
    "HLT_e17_lhloose_nod0_mu14",
    "HLT_e7_lhmedium_nod0_mu24",
    "HLT_e24_lhmedium_nod0_L1EM20VHI_mu8noL1",
    "HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1",
    "HLT_2mu10_nomucomb",
    "HLT_2mu14",
    "HLT_2mu14_nomucomb",
    "HLT_mu20_mu8noL1",
    "HLT_mu20_nomucomb_mu6noL1_nscan03",
    "HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb",
    "HLT_mu22_mu8noL1",
    //2015 triggers
    "HLT_e24_lhmedium_L1EM20VH",
    "HLT_e60_lhmedium",
    "HLT_e120_lhloose",
    "HLT_mu20_iloose_L1MU15",
    "HLT_mu40",
    "HLT_2e12_lhloose_L12EM10VH",
    "HLT_e17_lhloose_mu14",
    "HLT_e24_lhmedium_L1EM20VHI_mu8noL1",
    "HLT_e7_lhmedium_mu24",
    "HLT_2mu10",
    "HLT_mu18_mu8noL1"
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

  // Set lumi
  configMgr->objectTools->setLumi(1.0);

  // Object class contains the definitions of all physics objects, eg muons, electrons, jets
  // See xAODNtupleMaker::Objects for available methods; configMgr->obj

}
// ------------------------------------------------------------------------------------------ //
bool CompressedSelector::doAnalysis(ConfigMgr*& configMgr)
{
  /*
    This is the main method, which is called for each event
  */

  // Skims events by imposing any cuts you define in this method below
  if( !passCuts(configMgr) ) return false;

  bool useBaseline = true; // to calculate our observables using baseline leptons rather than signal leptons
  bool useTrackMET = true; // for calculating only SOME variables using track-based MET
  int nLep_base = configMgr->obj->baseLeptons.size();
 
  // We requested at least two baseline leptons in the skim
  const LeptonVariable* lep1 = configMgr->obj->baseLeptons[0];
  const LeptonVariable* lep2 = configMgr->obj->baseLeptons[1];
  const LeptonVariable* lep3 = configMgr->obj->baseLeptons[2];
  //const LeptonVariable* lep4 = nLep_base < 4 ? new LeptonVariable() : configMgr->obj->baseLeptons[3];

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
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarksRelease20#MV2c10_tagger_added_on_11th_May
    if( jet->mv2c10 >= 0.934906  ) nBJet20_FixedCutBEff_60++;
    if( jet->mv2c10 >= 0.8244273 ) nBJet20_FixedCutBEff_70++;
    if( jet->mv2c10 >= 0.645925  ) nBJet20_FixedCutBEff_77++;
    if( jet->mv2c10 >= 0.1758475 ) nBJet20_FixedCutBEff_85++;

    if(jet->Pt() < 30.0) continue;
    if( jet->mv2c10 >= 0.934906  ) nBJet30_FixedCutBEff_60++;
    if( jet->mv2c10 >= 0.8244273 ) nBJet30_FixedCutBEff_70++;
    if( jet->mv2c10 >= 0.645925  ) nBJet30_FixedCutBEff_77++;
    if( jet->mv2c10 >= 0.1758475 ) nBJet30_FixedCutBEff_85++;

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
   
  // lepton-beam polar angle
  float LepCosThetaLab  = getLepCosThetaLab(configMgr->obj);
  float LepCosThetaCoM  = getLepCosThetaCoM(configMgr->obj);

  //3L analysis
  // traditional assignment
  float mt = -999.;
  float mll_3Lep = 99999.; 
  int leadZindex = -1;
  int subZindex = -1;
  int Windex = -1;
  
  //minMt assignment
  float minMt = 99999.;
  float mll_minMt = -999.;
  int leadZindex_minMt = -1;
  int subZindex_minMt = -1;
  int Windex_minMt = -1;

  //minMll assignment
  float minMll = 99999.;
  float mt_minMll = -999.;
  int leadZindex_minMll = -1;
  int subZindex_minMll = -1;
  int Windex_minMll = -1;
  
  //maxMll assignment
  float maxMll = -999.;
  float mt_maxMll = -999.;
  int leadZindex_maxMll = -1;
  int subZindex_maxMll = -1;
  int Windex_maxMll = -1;

  float ZMass = 91.1876;

  int nb_SFOS = 0;

  if (nLep_base != 3) return false;

  // lepton assignment
  for(int ilep=0; ilep<nLep_base-1; ilep++){
    for(int jlep=ilep+1; jlep<nLep_base; jlep++){
      int klep = nLep_base - ilep - jlep;

      LeptonVariable* tmp_lep1 = configMgr->obj->baseLeptons[ilep];
      LeptonVariable* tmp_lep2 = configMgr->obj->baseLeptons[jlep];

      bool SF = (tmp_lep1->isEle() && tmp_lep2->isEle()) || (tmp_lep1->isMu() && tmp_lep2->isMu());
      bool OS = (tmp_lep1->q == -tmp_lep2->q);      

      if(SF && OS){
        nb_SFOS++;

        float tmp_mll = (*tmp_lep1+*tmp_lep2).M();
        float tmp_mt = getMt(configMgr->obj, useBaseline, klep);

	if (fabs(tmp_mll-ZMass) < fabs(mll_3Lep-ZMass)){ //traditional assignment
	  mt = tmp_mt;
          mll_3Lep = tmp_mll;
          leadZindex = ilep;
          subZindex = jlep;
          Windex = klep;
        }

        if (tmp_mt < minMt){ //minMt assignment
          minMt = tmp_mt;
          mll_minMt = tmp_mll;
          leadZindex_minMt = ilep;
          subZindex_minMt = jlep;
          Windex_minMt = klep;
        }

	if (tmp_mll < minMll){ //minMll assignment
	  mt_minMll = tmp_mt;
          minMll = tmp_mll;
          leadZindex_minMll = ilep;
          subZindex_minMll = jlep;
          Windex_minMll = klep;
	}

	if (tmp_mll > maxMll){ //maxMll assignment
          mt_maxMll = tmp_mt;
          maxMll = tmp_mll;
          leadZindex_maxMll = ilep;
          subZindex_maxMll = jlep;
          Windex_maxMll = klep;
	}
      }
    }
  }

  if (nb_SFOS == 0) return false;   

  // Various assignment leptons
  LeptonVariable* z1Lep = configMgr->obj->baseLeptons[leadZindex];
  LeptonVariable* z2Lep = configMgr->obj->baseLeptons[subZindex];
  LeptonVariable* wLep = configMgr->obj->baseLeptons[Windex];

  LeptonVariable* z1Lep_minMt = configMgr->obj->baseLeptons[leadZindex_minMt];
  LeptonVariable* z2Lep_minMt = configMgr->obj->baseLeptons[subZindex_minMt];
  LeptonVariable* wLep_minMt = configMgr->obj->baseLeptons[Windex_minMt];

  LeptonVariable* z1Lep_minMll = configMgr->obj->baseLeptons[leadZindex_minMll];
  LeptonVariable* z2Lep_minMll = configMgr->obj->baseLeptons[subZindex_minMll];
  LeptonVariable* wLep_minMll = configMgr->obj->baseLeptons[Windex_minMll];

  LeptonVariable* z1Lep_maxMll = configMgr->obj->baseLeptons[leadZindex_maxMll];
  LeptonVariable* z2Lep_maxMll = configMgr->obj->baseLeptons[subZindex_maxMll];
  LeptonVariable* wLep_maxMll = configMgr->obj->baseLeptons[Windex_maxMll];

  bool L3_SFOS = nb_SFOS > 0? true : false;
  float L3_mt = mt;
  float L3_mtMin = minMt;
  float L3_mt_minMll = mt_minMll;
  float L3_mt_maxMll = mt_maxMll;
  float L3_mll = mll_3Lep;
  float L3_mll_mtMin = mll_minMt;
  float L3_minMll = minMll;
  float L3_maxMll = maxMll;
  float L3_m3l = (*lep1+*lep2+*lep3).M();
  float L3_pT3l = (*lep1+*lep2+*lep3).Pt();
  float L3_pT3lOverMet = L3_pT3l/met;
  float L3_pT3lOverMetTrack = L3_pT3l/metTrack;
  float L3_Ht3l = lep1->Pt()+lep2->Pt()+lep3->Pt();
  float L3_METOverHTLep = L3_Ht3l > 0 ? met / L3_Ht3l : -999;
  float L3_METTrackOverHTLep = L3_Ht3l > 0 ? metTrack / L3_Ht3l : -999;
  float L3_RZlep = z1Lep->DeltaR(*z2Lep);
  float L3_RZlep_mtMin = z1Lep_minMt->DeltaR(*z2Lep_minMt);
  float L3_RZlep_minMll = z1Lep_minMll->DeltaR(*z2Lep_minMll);
  float L3_RZlep_maxMll = z1Lep_maxMll->DeltaR(*z2Lep_maxMll);
  float L3_RZWlep = (*z1Lep + *z2Lep).DeltaR(*wLep);
  float L3_RZWlep_mtMin = (*z1Lep_minMt + *z2Lep_minMt).DeltaR(*wLep_minMt);
  float L3_RZWlep_minMll = (*z1Lep_minMll + *z2Lep_minMll).DeltaR(*wLep_minMll);
  float L3_RZWlep_maxMll = (*z1Lep_maxMll + *z2Lep_maxMll).DeltaR(*wLep_maxMll);
  float L3_dPhiWLepMet = wLep->DeltaPhi( configMgr->obj->met);
  float L3_dPhiWLepMet_mtMin = wLep_minMt->DeltaPhi( configMgr->obj->met);
  float L3_dPhiWLepMet_minMll = wLep_minMll->DeltaPhi( configMgr->obj->met);
  float L3_dPhiWLepMet_maxMll = wLep_maxMll->DeltaPhi( configMgr->obj->met);
  float L3_dPhiWLepMetTrack = wLep->DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhiWLepMetTrack_mtMin = wLep_minMt->DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhiWLepMetTrack_minMll = wLep_minMll->DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhiWLepMetTrack_maxMll = wLep_maxMll->DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhiZMet = (*z1Lep + *z2Lep).DeltaPhi( configMgr->obj->met);
  float L3_dPhiZMet_mtMin = (*z1Lep_minMt + *z2Lep_minMt).DeltaPhi( configMgr->obj->met);
  float L3_dPhiZMet_minMll = (*z1Lep_minMll + *z2Lep_minMll).DeltaPhi( configMgr->obj->met);
  float L3_dPhiZMet_maxMll = (*z1Lep_maxMll + *z2Lep_maxMll).DeltaPhi( configMgr->obj->met);
  float L3_dPhiZMetTrack = (*z1Lep + *z2Lep).DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhiZMetTrack_mtMin = (*z1Lep_minMt + *z2Lep_minMt).DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhiZMetTrack_minMll = (*z1Lep_minMll + *z2Lep_minMll).DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhiZMetTrack_maxMll = (*z1Lep_maxMll + *z2Lep_maxMll).DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_dPhi3lMet = (*lep1 + *lep2 + *lep3).DeltaPhi( configMgr->obj->met);
  float L3_dPhi3lMetTrack = (*lep1 + *lep2 + *lep3).DeltaPhi( configMgr->obj->met.getTrackTLV());
  float L3_ptZ = (*z1Lep + *z2Lep).Pt();
  float L3_ptZ_mtMin = (*z1Lep_minMt + *z2Lep_minMt).Pt();
  float L3_ptZ_minMll = (*z1Lep_minMll + *z2Lep_minMll).Pt();
  float L3_ptZ_maxMll = (*z1Lep_maxMll + *z2Lep_maxMll).Pt();
  float L3_ptW = (*wLep + configMgr->obj->met).Pt();
  float L3_ptW_mtMin = (*wLep_minMt + configMgr->obj->met).Pt();
  float L3_ptW_minMll = (*wLep_minMll + configMgr->obj->met).Pt();
  float L3_ptW_maxMll = (*wLep_maxMll + configMgr->obj->met).Pt();
  float L3_ptWMetTrack = (*wLep + configMgr->obj->met.getTrackTLV()).Pt();
  float L3_ptWMetTrack_mtMin = (*wLep_minMt + configMgr->obj->met.getTrackTLV()).Pt();
  float L3_ptWMetTrack_minMll = (*wLep_minMll + configMgr->obj->met.getTrackTLV()).Pt();
  float L3_ptWMetTrack_maxMll = (*wLep_maxMll + configMgr->obj->met.getTrackTLV()).Pt();

  //RJ-like variables (only using MET, not track MET)
  //Variables in lab frame
  TLorentzVector ISR;
  ISR.SetPtEtaPhiM(0,0,0,0);
  for(JetVariable* jet : configMgr->obj->cJets){
    if(fabs(jet->Eta()) < 2.4){
      ISR = ISR + *jet;
    }
  }
  float L3_pTISR = ISR.Pt();
  float L3_deltaPhiISRMet = ISR.DeltaPhi(configMgr->obj->met);
  float L3_RISR = fabs(configMgr->obj->met.px*ISR.Px()+configMgr->obj->met.py*ISR.Py())/fabs(L3_pTISR*L3_pTISR);
  float L3_pTCM = (*lep1+ *lep2 + *lep3 + configMgr->obj->met + ISR).Pt();
  float L3_HT31 = lep1->Pt()+ lep2->Pt() + lep3->Pt() + configMgr->obj->met.Et;
  
  //calculating Z-boost using mInv = MInv = 0 to PP frame
  float long_inv = (*lep1 + *lep2 + *lep3).Pz()*sqrt(configMgr->obj->met.Et*configMgr->obj->met.Et)/sqrt((*lep1 + *lep2 + *lep3).Pt()*(*lep1 + *lep2 + *lep3).Pt()+(*lep1 + *lep2 + *lep3).M()*(*lep1 + *lep2 + *lep3).M());
  float p_I = sqrt((configMgr->obj->met.px*configMgr->obj->met.px)+(configMgr->obj->met.py*configMgr->obj->met.py)+long_inv*long_inv);

  float bx = ((*lep1 + *lep2 + *lep3).Px() + configMgr->obj->met.px)/ ((*lep1 + *lep2 + *lep3).E() + sqrt(p_I*p_I));
  float by = ((*lep1 + *lep2 + *lep3).Py() + configMgr->obj->met.py)/ ((*lep1 + *lep2 + *lep3).E() + sqrt(p_I*p_I));
  float bz = ((*lep1 + *lep2 + *lep3).Pz() + long_inv)/( (*lep1 + *lep2 + *lep3).E() + sqrt(p_I*p_I) );

  TLorentzVector lep1_tlv, lep2_tlv, lep3_tlv, met_tlv;

  lep1_tlv.SetPtEtaPhiM(lep1->Pt(),lep1->Eta(),lep1->Phi(),lep1->M());
  lep2_tlv.SetPtEtaPhiM(lep2->Pt(),lep2->Eta(),lep2->Phi(),lep2->M());
  lep3_tlv.SetPtEtaPhiM(lep3->Pt(),lep3->Eta(),lep3->Phi(),lep3->M());
  met_tlv.SetXYZM(configMgr->obj->met.px,configMgr->obj->met.py,long_inv,0);

  lep1_tlv.Boost(-bx,-by,-bz);
  lep2_tlv.Boost(-bx,-by,-bz);
  lep3_tlv.Boost(-bx,-by,-bz);
  met_tlv.Boost(-bx,-by,-bz);

  float L3_H31 = lep1_tlv.P()+ lep2_tlv.P() + lep3_tlv.P() + met_tlv.P();
  
  //
  // -------- Fill the output tree variables ---------- //
  //

  //Emulated Triggers
  //configMgr->treeMaker->setBoolVariable("HLT_2mu4_j85_xe50_mht_emul", HLT_2mu4_j85_xe50_mht_emul);
  //configMgr->treeMaker->setBoolVariable("HLT_mu4_j125_xe90_mht_emul", HLT_mu4_j125_xe90_mht_emul);

  // Global Vars
  configMgr->treeMaker->setFloatVariable("mu",configMgr->obj->evt.mu);
  configMgr->treeMaker->setIntVariable("nVtx",configMgr->obj->evt.nVtx);

  // Leptons
  configMgr->treeMaker->setIntVariable("nLep_base", nLep_base);
  configMgr->treeMaker->setIntVariable("nLep_signal",configMgr->obj->signalLeptons.size());
  configMgr->treeMaker->setIntVariable("nEle_signal",configMgr->obj->signalElectrons.size());
  configMgr->treeMaker->setIntVariable("nMu_signal",configMgr->obj->signalMuons.size());

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
  /*
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
  */
  // For data, set these to true so that data can be processed
  // with the same cuts as the MC
  if( !configMgr->obj->evt.isMC ){
    lep1TruthMatched = true;
    lep2TruthMatched = true;
    lep3TruthMatched = true;
    //lep4TruthMatched = true;
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
  /*
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
  */
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

  configMgr->treeMaker->setFloatVariable("mbb",mbb);

  // other analysis variables
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

  configMgr->treeMaker->setFloatVariable("LepCosThetaLab", LepCosThetaLab);
  configMgr->treeMaker->setFloatVariable("LepCosThetaCoM", LepCosThetaCoM);

  //3L Analysis Variables
  configMgr->treeMaker->setBoolVariable("L3_SFOS",L3_SFOS);
  configMgr->treeMaker->setFloatVariable("L3_mt",L3_mt);
  configMgr->treeMaker->setFloatVariable("L3_mtMin",L3_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_mt_minMll",L3_mt_minMll);
  configMgr->treeMaker->setFloatVariable("L3_mt_maxMll",L3_mt_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_mll",L3_mll);
  configMgr->treeMaker->setFloatVariable("L3_mll_mtMin",L3_mll_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_minMll",L3_minMll);
  configMgr->treeMaker->setFloatVariable("L3_maxMll",L3_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_m3l",L3_m3l);
  configMgr->treeMaker->setFloatVariable("L3_pT3l",L3_pT3l);
  configMgr->treeMaker->setFloatVariable("L3_pT3lOverMet",L3_pT3lOverMet);
  configMgr->treeMaker->setFloatVariable("L3_pT3lOverMetTrack",L3_pT3lOverMetTrack);
  configMgr->treeMaker->setFloatVariable("L3_Ht3l",L3_Ht3l);
  configMgr->treeMaker->setFloatVariable("L3_METOverHTLep",L3_METOverHTLep);
  configMgr->treeMaker->setFloatVariable("L3_METTrackOverHTLep",L3_METTrackOverHTLep);
  configMgr->treeMaker->setFloatVariable("L3_RZlep",L3_RZlep);
  configMgr->treeMaker->setFloatVariable("L3_RZlep_mtMin",L3_RZlep_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_RZlep_minMll",L3_RZlep_minMll);
  configMgr->treeMaker->setFloatVariable("L3_RZlep_maxMll",L3_RZlep_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_RZWlep",L3_RZWlep);
  configMgr->treeMaker->setFloatVariable("L3_RZWlep_mtMin",L3_RZWlep_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_RZWlep_minMll",L3_RZWlep_minMll);
  configMgr->treeMaker->setFloatVariable("L3_RZWlep_maxMll",L3_RZWlep_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMet",L3_dPhiWLepMet);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMet_mtMin",L3_dPhiWLepMet_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMet_minMll",L3_dPhiWLepMet_minMll);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMet_maxMll",L3_dPhiWLepMet_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMetTrack",L3_dPhiWLepMetTrack);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMetTrack_mtMin",-1.);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMetTrack_minMll",-1.);
  configMgr->treeMaker->setFloatVariable("L3_dPhiWLepMetTrack_maxMll",-1.);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMet",L3_dPhiZMet);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMet_mtMin",L3_dPhiZMet_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMet_minMll",L3_dPhiZMet_minMll);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMet_maxMll",L3_dPhiZMet_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMetTrack",L3_dPhiZMetTrack);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMetTrack_mtMin",L3_dPhiZMetTrack_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMetTrack_minMll",L3_dPhiZMetTrack_minMll);
  configMgr->treeMaker->setFloatVariable("L3_dPhiZMetTrack_maxMll",L3_dPhiZMetTrack_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_dPhi3lMet",L3_dPhi3lMet);
  configMgr->treeMaker->setFloatVariable("L3_dPhi3lMetTrack",L3_dPhi3lMetTrack);
  configMgr->treeMaker->setFloatVariable("L3_ptZ",L3_ptZ); 
  configMgr->treeMaker->setFloatVariable("L3_ptZ_mtMin",L3_ptZ_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_ptZ_minMll",L3_ptZ_minMll);
  configMgr->treeMaker->setFloatVariable("L3_ptZ_maxMll",L3_ptZ_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_ptW",L3_ptW);
  configMgr->treeMaker->setFloatVariable("L3_ptW_mtMin",L3_ptW_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_ptW_minMll",L3_ptW_minMll);
  configMgr->treeMaker->setFloatVariable("L3_ptW_maxMll",L3_ptW_maxMll);
  configMgr->treeMaker->setFloatVariable("L3_ptWMetTrack",L3_ptWMetTrack);
  configMgr->treeMaker->setFloatVariable("L3_ptWMetTrack_mtMin",L3_ptWMetTrack_mtMin);
  configMgr->treeMaker->setFloatVariable("L3_ptWMetTrack_minMll",L3_ptWMetTrack_minMll);
  configMgr->treeMaker->setFloatVariable("L3_ptWMetTrack_maxMll",L3_ptWMetTrack_maxMll);

  //RJ translation variables (only using MET, not track MET)
  configMgr->treeMaker->setFloatVariable("L3_pTISR",L3_pTISR);
  configMgr->treeMaker->setFloatVariable("L3_deltaPhiISRMet",L3_deltaPhiISRMet);
  configMgr->treeMaker->setFloatVariable("L3_RISR",L3_RISR);
  configMgr->treeMaker->setFloatVariable("L3_pTCM",L3_pTCM);
  configMgr->treeMaker->setFloatVariable("L3_HT31",L3_HT31);
  configMgr->treeMaker->setFloatVariable("L3_H31",L3_H31);

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
  //double truthMll = configMgr->obj->truthEvent.truthMll;
  //configMgr->treeMaker->setDoubleVariable("truthMll",truthMll);
 
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
  //if(nLep_base < 4) delete lep4;

  return true;

}
// ------------------------------------------------------------------------------------------ //
bool CompressedSelector::passCuts(ConfigMgr*& configMgr)
{

  /*
   This method is used to apply any cuts you wish before writing
   the output trees
  */
  double weight = 1; // pb-1
  if(!configMgr->obj->evt.isData()){

    // Note that Rel20.7 2015+2016 is actually 36097.56 pb-1, but this is close enough
    weight = 36100; // pb-1
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
  if( configMgr->obj->baseLeptons.size() < 3 ) return false;
  configMgr->cutflow->bookCut("cutFlow","At least two baseline leptons", weight );

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

    /*
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
    */
    // Sequential, post-cleaning
    if(configMgr->obj->baseLeptons.size() >= 3)
      configMgr->cutflow->bookCut("cutFlow","SEQ: >= 2 baseline lep", weight );
    else return false;

    if(configMgr->obj->signalLeptons.size() >= 3)
      configMgr->cutflow->bookCut("cutFlow","SEQ: >= 2 signal lep", weight );
    else return false;

    /*
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
    */
  }
  return true;

}
// ------------------------------------------------------------------------------------------ //
int CompressedSelector::getHiggsinoDM(int DSID){
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
bool CompressedSelector::isHiggsinoN2C1p(int DSID){
  bool isN2C1p = false;
  if (DSID==393596 || DSID==393400 || DSID==393401 || DSID==393402 || DSID==393403 || DSID==393404 || DSID==393405 || DSID==393406 || DSID==394173 || DSID==393407 || DSID==393408 || DSID==393409 || DSID==393410 || DSID==393411 || DSID==393412 || DSID==393413 || DSID==394177 || DSID==393414 || DSID==393415 || DSID==393416 || DSID==393417 || DSID==393418 || DSID==393419 || DSID==393420 || DSID==394181 || DSID==393421 || DSID==393422 || DSID==393423 || DSID==393424 || DSID==393425 || DSID==393426 || DSID==393427 || DSID==394185 || DSID==393428 || DSID==393429 || DSID==393430 || DSID==393431 || DSID==393432 || DSID==393433 || DSID==393434 || DSID==394189 || DSID==393435 || DSID==393436 || DSID==393437 || DSID==393438 || DSID==393439 || DSID==393440 || DSID==393441 || DSID==394193 || DSID==393442 || DSID==393443 || DSID==393444 || DSID==393445 || DSID==393446 || DSID==393447 || DSID==393448){
    isN2C1p = true;
  } else {
    isN2C1p = false;
  }
  return isN2C1p;
}
// ------------------------------------------------------------------------------------------ //
bool CompressedSelector::isHiggsinoN2C1m(int DSID){
  bool isN2C1m = false;
  if (DSID==393597 || DSID==393449 || DSID==393450 || DSID==393451 || DSID==393452 || DSID==393453 || DSID==393454 || DSID==393455 || DSID==394174 || DSID==393456 || DSID==393457 || DSID==393458 || DSID==393459 || DSID==393460 || DSID==393461 || DSID==393462 || DSID==394178 || DSID==393463 || DSID==393464 || DSID==393465 || DSID==393466 || DSID==393467 || DSID==393468 || DSID==393469 || DSID==394182 || DSID==393470 || DSID==393471 || DSID==393472 || DSID==393473 || DSID==393474 || DSID==393475 || DSID==393476 || DSID==394186 || DSID==393477 || DSID==393478 || DSID==393479 || DSID==393480 || DSID==393481 || DSID==393482 || DSID==393483 || DSID==394190 || DSID==393484 || DSID==393485 || DSID==393486 || DSID==393487 || DSID==393488 || DSID==393489 || DSID==393490 || DSID==394194 || DSID==393491 || DSID==393492 || DSID==393493 || DSID==393494 || DSID==393495 || DSID==393496 || DSID==393497){
    isN2C1m = true;
  } else{
    isN2C1m = false;
  }
  return isN2C1m;
}
// ------------------------------------------------------------------------------------------ //
bool CompressedSelector::isHiggsinoC1C1(int DSID){
  bool isC1C1 = false;
  if (DSID==393599 || DSID==393547 || DSID==393548 || DSID==393549 || DSID==393550 || DSID==393551 || DSID==393552 || DSID==393553 || DSID==394176 || DSID==393554 || DSID==393555 || DSID==393556 || DSID==393557 || DSID==393558 || DSID==393559 || DSID==393560 || DSID==394180 || DSID==393561 || DSID==393562 || DSID==393563 || DSID==393564 || DSID==393565 || DSID==393566 || DSID==393567 || DSID==394184 || DSID==393568 || DSID==393569 || DSID==393570 || DSID==393571 || DSID==393572 || DSID==393573 || DSID==393574 || DSID==394188 || DSID==393575 || DSID==393576 || DSID==393577 || DSID==393578 || DSID==393579 || DSID==393580 || DSID==393581 || DSID==394192 || DSID==393582 || DSID==393583 || DSID==393584 || DSID==393585 || DSID==393586 || DSID==393587 || DSID==393588 || DSID==394196 || DSID==393589 || DSID==393590 || DSID==393591 || DSID==393592 || DSID==393593 || DSID==393594 || DSID==393595){
    isC1C1 = true;
  } else {
    isC1C1 = false; 
  }
  return isC1C1;
}
// ------------------------------------------------------------------------------------------ //
bool CompressedSelector::isHiggsinoN2N1(int DSID){
  bool isN2N1 = false;
  if (DSID==393598 || DSID==393498 || DSID==393499 || DSID==393500 || DSID==393501 || DSID==393502 || DSID==393503 || DSID==393504 || DSID==394175 || DSID==393505 || DSID==393506 || DSID==393507 || DSID==393508 || DSID==393509 || DSID==393510 || DSID==393511 || DSID==394179 || DSID==393512 || DSID==393513 || DSID==393514 || DSID==393515 || DSID==393516 || DSID==393517 || DSID==393518 || DSID==394183 || DSID==393519 || DSID==393520 || DSID==393521 || DSID==393522 || DSID==393523 || DSID==393524 || DSID==393525 || DSID==394187 || DSID==393526 || DSID==393527 || DSID==393528 || DSID==393529 || DSID==393530 || DSID==393531 || DSID==393532 || DSID==394191 || DSID==393533 || DSID==393534 || DSID==393535 || DSID==393536 || DSID==393537 || DSID==393538 || DSID==393539 || DSID==394195 || DSID==393540 || DSID==393541 || DSID==393542 || DSID==393543 || DSID==393544 || DSID==393545 || DSID==393546){
    isN2N1 = true;
  } else{
    isN2N1 = false;
  }
  return isN2N1;
}
// ------------------------------------------------------------------------------------------ //
void CompressedSelector::finalize(ConfigMgr*& configMgr)
{

  /*
   This method is called at the very end of the job. Can be used to merge cutflow histograms 
   for example. See CutFlowTool::mergeCutFlows(...)
  */

}
// ------------------------------------------------------------------------------------------ //
