#include "SusySkimHiggsino/EWK3L_Fakes.h"


// ------------------------------------------------------------------------------------ //
EWK3L_Fakes::EWK3L_Fakes() :
  BaseUser("SusySkimHiggsino", "EWK3L_Fakes")
{}
// ------------------------------------------------------------------------------------ //
void EWK3L_Fakes::setup(ConfigMgr*& configMgr)
{
  // Make a cutflow stream
  configMgr->cutflow->defineCutFlow("cutFlow",configMgr->treeMaker->getFile("tree"));

  //
  // Object class contains the definitions of all physics objects, eg muons, electrons, jets
  // See xAODNtupleMaker::Objects for available methods; configMgr->obj
  //

  // Use signal object def from SUSYTools
  configMgr->obj->useSUSYToolsSignalDef(true);

  // Baseline lepton kinematic cuts:
  // Note: set the defaults to -1 in order to just use the SUSYTool cuts
  configMgr->obj->setBaselineElectronEt(-1);
  configMgr->obj->setBaselineElectronEta(-1);
  configMgr->obj->setBaselineMuonPt(-1);
  configMgr->obj->setBaselineMuonEta(-1);

  // Jets
  configMgr->obj->setCJetPt(20.0);
  configMgr->obj->setCJetEta(4.5);

  // Forward Jets
  // I don't think we need to categorize the forward jets
  // in any special way, so let's just make it so this container
  // is never filled
  configMgr->obj->setFJetPt(999999999);
  configMgr->obj->setFJetEtaMin(-1);
  configMgr->obj->setFJetEtaMax(-1);

  // Turn on the bad and cosmic muon vetos
  configMgr->obj->applyBadMuonVeto(true);
  configMgr->obj->applyCosmicMuonVeto(true);

  // Set lumi
  configMgr->objectTools->setLumi(36075);

  // Turn off OR for baseline muons for fakes selection
  //configMgr->obj->disableORBaselineMuons(true);

  // Variables to be added to the output tree
  configMgr->treeMaker->addIntVariable("DSID",-999);
  configMgr->treeMaker->addFloatVariable("weight",-999);

  configMgr->treeMaker->addFloatVariable("metEt",-999);
  configMgr->treeMaker->addFloatVariable("metPhi",-999);

  configMgr->treeMaker->addFloatVariable("leadJetPt",-999);
  configMgr->treeMaker->addIntVariable("nJets",-999);
  configMgr->treeMaker->addIntVariable("nBJets",-999);

  configMgr->treeMaker->addVecFloatVariable("lepPt");
  configMgr->treeMaker->addVecFloatVariable("lepEta");
  configMgr->treeMaker->addVecFloatVariable("lepPhi");
  configMgr->treeMaker->addVecFloatVariable("lepD0Sig");
  configMgr->treeMaker->addVecFloatVariable("lepZ0SinTheta");
  configMgr->treeMaker->addVecIntVariable("lepFlavor");
  configMgr->treeMaker->addVecIntVariable("lepCharge");
  configMgr->treeMaker->addVecIntVariable("lepPassOR");
  configMgr->treeMaker->addVecIntVariable("lepTruthType");
  configMgr->treeMaker->addVecIntVariable("lepTruthOrigin");
  configMgr->treeMaker->addVecIntVariable("lepPassBL");
  configMgr->treeMaker->addVecIntVariable("lepNPix");
  configMgr->treeMaker->addVecIntVariable("lepVeryLoose");
  configMgr->treeMaker->addVecIntVariable("lepLoose");
  configMgr->treeMaker->addVecIntVariable("lepMedium");
  configMgr->treeMaker->addVecIntVariable("lepTight");
  configMgr->treeMaker->addVecIntVariable("lepIsoLoose");
  configMgr->treeMaker->addVecIntVariable("lepIsoTight");
  configMgr->treeMaker->addVecIntVariable("lepIsoGradient");
  configMgr->treeMaker->addVecIntVariable("lepIsoGradientLoose");
  configMgr->treeMaker->addVecIntVariable("lepIsoLooseTrackOnly");
  configMgr->treeMaker->addVecIntVariable("lepSignal");
  configMgr->treeMaker->addVecIntVariable("lepBaseline");
  configMgr->treeMaker->addVecIntVariable("lepMatchesTrigger");
}
// ------------------------------------------------------------------------------------ //
void EWK3L_Fakes::fillWeights(ConfigMgr*& configMgr)
{
  // Reset the weight
  m_weight = 1;

  // Pileup weight
  m_weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::PILEUP);

  // MC event weight
  m_weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::EVT);

  // Lepton reco, ID, isolation SFs
  m_weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::LEP);

  // Btagging SFs
  m_weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::BTAG);

  // FIXME Trigger SFs: need to properly handle multileg triggers!! Keep this off for now!
  //m_weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::TRIG);

  // to additionally apply xs * lumi weight
  m_weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::GEN);

  // for V+jet samples, apply extra weight
  m_weight *= configMgr->obj->evt.getSherpaVjetsNjetsWeight();

  return;
}
// ------------------------------------------------------------------------------------ //
bool EWK3L_Fakes::passCuts(ConfigMgr*& configMgr)
{

  /*
   This method is used to apply any cuts you wish before writing
   the output trees
  */

  // Fill cutflow histograms
  configMgr->cutflow->bookCut("cutFlow","allEvents",m_weight );

  // Apply all recommended event cleaning cuts
  if( !configMgr->obj->passEventCleaning( configMgr->cutflow, "cutFlow", m_weight ) ) return false;


  // Note! All cuts that follow are ONLY for the cutflow, and events are not discarded
  // which fail the following cuts
  int nLeptons_signal = configMgr->obj->signalLeptons.size();
  int nLeptons_base   = configMgr->obj->baseLeptons.size();

  int nAllJets        = configMgr->obj->aJets.size(); // after OR, no eta reqs
  //int nBaselineJets   = configMgr->obj->baselineJets.size(); // before OR, but must be central
  int nCentralJets    = configMgr->obj->cJets.size(); // after OR, must be "central" (though, we use |eta| < 4.5), and passes signal reqs
  int nBTagJets       = configMgr->obj->bJets.size(); // same as nCentralJets but with a b-tag

  int nElectrons_base = configMgr->obj->baseElectrons.size();
  int nMuons_base = configMgr->obj->baseMuons.size();

  int nElectrons_signal = configMgr->obj->signalElectrons.size();
  int nMuons_signal = configMgr->obj->signalMuons.size();

  // nElectrons
  if(nElectrons_base >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 baseline electron", m_weight );
  if(nElectrons_signal >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 signal electron", m_weight );
  if(nElectrons_base >= 2)
    configMgr->cutflow->bookCut("cutFlow", ">= 2 baseline electron", m_weight );
  if(nElectrons_signal >= 2)
    configMgr->cutflow->bookCut("cutFlow", ">= 2 signal electron", m_weight );
  if(nElectrons_base == 3)
    configMgr->cutflow->bookCut("cutFlow", "== 3 baseline electron", m_weight );
  if(nElectrons_signal == 3)
    configMgr->cutflow->bookCut("cutFlow", "== 3 signal electron", m_weight );

  // nMuons
  if(nMuons_base >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 baseline muon", m_weight );
  if(nMuons_signal >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 signal muon", m_weight );
  if(nMuons_base >= 2)
    configMgr->cutflow->bookCut("cutFlow", ">= 2 baseline muon", m_weight );
  if(nMuons_signal >= 2)
    configMgr->cutflow->bookCut("cutFlow", ">= 2 signal muon", m_weight );
  if(nMuons_base == 3)
    configMgr->cutflow->bookCut("cutFlow", "== 3 baseline muon", m_weight );
  if(nMuons_signal == 3)
    configMgr->cutflow->bookCut("cutFlow", "== 3 signal muon", m_weight );

  // nJets
  if(nAllJets >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 All jet", m_weight );
  if(nCentralJets >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 Central jet", m_weight );
  if(nBTagJets >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 BTag jet", m_weight );

  // nLeptons
  if(nLeptons_base >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 baseline lepton", m_weight );
  if(nLeptons_signal >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 signal lepton", m_weight );
  if(nLeptons_base >= 2)
    configMgr->cutflow->bookCut("cutFlow", ">= 2 baseline lepton", m_weight );
  if(nLeptons_signal >= 2)
    configMgr->cutflow->bookCut("cutFlow", ">= 2 signal lepton", m_weight );
  if(nLeptons_base == 3)
    configMgr->cutflow->bookCut("cutFlow", "== 3 baseline lepton", m_weight );
  if(nLeptons_signal == 3)
    configMgr->cutflow->bookCut("cutFlow", "== 3 signal lepton", m_weight );
  if(nLeptons_base == 3 && nLeptons_signal == 3)
    configMgr->cutflow->bookCut("cutFlow", "== 3 baseline leptons and == 3 signal leptons", m_weight );

  return true;

}
// ------------------------------------------------------------------------------------ //
void EWK3L_Fakes::fillListOfTriggersPassed(ConfigMgr*& configMgr)
{

  // Also think about how to handle when multiple triggers passed!!!
  std::vector<TString> listOfPossibleTriggers2015 = {"HLT_2e12_lhloose_L12EM10VH", "HLT_mu18_mu8noL1", "HLT_e17_lhloose_mu14"};
  std::vector<TString> listOfPossibleTriggers2016 = {"HLT_2e17_lhvloose_nod0", "HLT_mu22_mu8noL1", "HLT_e17_lhloose_nod0_mu14"};

  std::vector<TString> listOfPossibleTriggers = {};

  // Now look at runNumber to determine the year
  // (randomRunNumber when using MC)
  int runNumber = configMgr->obj->evt.runNumber;
  if(configMgr->obj->evt.isMC){
    runNumber = configMgr->obj->evt.randomRunNumber;
  }

  //std::cout << "Verify: runNumber is " << runNumber << std::endl;

  // Basically a copy of the SUSYTools::TreatAsYear() function
  if(runNumber < 290000){
    listOfPossibleTriggers = listOfPossibleTriggers2015;
  }
  else{
    listOfPossibleTriggers = listOfPossibleTriggers2016;
  }

  // Check all possible triggers, and put them into m_listOfTriggersPassed if they pass
  for(TString trig : listOfPossibleTriggers){
    if(configMgr->obj->evt.getHLTEvtTrigDec(trig.Data())){
      m_listOfTriggersPassed.push_back(trig);
    }
  }

  return;
}
// ------------------------------------------------------------------------------------ //
bool EWK3L_Fakes::doAnalysis(ConfigMgr*& configMgr)
{

  /*
    This is the main method, which is called for each event
  */

  // Do this at the beginning of each loop, to reset the values!!
  fillWeights(configMgr);
  m_listOfTriggersPassed.clear();

  // Fill in list of passed triggers
  fillListOfTriggersPassed(configMgr);

  // Skims events by imposing any cuts you define in this method below
  if( !passCuts(configMgr) ) return false;

  // The fake factor selection
  if( !Simple_FakeFactor_Selection(configMgr) ) return false;

  // Fill the output tree
  configMgr->treeMaker->Fill(configMgr->getSysState(),"tree");

  return true;

}
// ------------------------------------------------------------------------------------ //
bool EWK3L_Fakes::Simple_FakeFactor_Selection(ConfigMgr*& configMgr)
{

  // No triggers passed!
  if(m_listOfTriggersPassed.size() == 0) return false; 

  // Create vectors for each branch of the output ntuple
  std::vector<float> lepPt;
  std::vector<float> lepEta;
  std::vector<float> lepPhi;
  std::vector<float> lepD0Sig;
  std::vector<float> lepZ0SinTheta;
  std::vector<int> lepFlavor;
  std::vector<int> lepCharge;
  std::vector<int> lepPassOR;
  std::vector<int> lepTruthType;
  std::vector<int> lepTruthOrigin;
  std::vector<int> lepPassBL;
  std::vector<int> lepNPix;
  std::vector<int> lepVeryLoose;
  std::vector<int> lepLoose;
  std::vector<int> lepMedium;
  std::vector<int> lepTight;
  std::vector<int> lepIsoLoose;
  std::vector<int> lepIsoTight;
  std::vector<int> lepIsoGradient;
  std::vector<int> lepIsoGradientLoose;
  std::vector<int> lepIsoLooseTrackOnly;
  std::vector<int> lepSignal;
  std::vector<int> lepBaseline;
  std::vector<int> lepMatchesTrigger;

  // For fake factor implementation
  ElectronVector electrons = configMgr->obj->baseElectrons;
  MuonVector muons = configMgr->obj->baseMuons;

  if(electrons.size() + muons.size() < 3) return false;

  // For studying the ttbar event in the SR with a large weight

  /*
  bool foundLead = false;
  bool foundSubLead = false;
  bool foundSubSubLead = false;

  ElectronVariable* leadLep = 0;
  MuonVariable* subleadLep = 0;
  MuonVariable* subsubleadLep = 0;

  for(ElectronVariable* el : electrons){
    if(170.26 < el->Pt() && el->Pt() < 170.27){
      foundLead = true;
      leadLep = el;
    }
  }
  for(MuonVariable* mu : muons ){
    if(40.183 < mu->Pt() && mu->Pt() < 40.184){
      foundSubLead = true;
      subleadLep = mu;
    }
    if(13.42 < mu->Pt() && mu->Pt() < 13.43){
      foundSubSubLead = true;
      subsubleadLep = mu;
    }
  }

  if( !(foundLead && foundSubLead && foundSubSubLead) ) return false;
  else{
    std::cout << "Found the event!" << std::endl;

    std::cout << "lead lep (el) type " << leadLep->type << " origin " << leadLep->origin << std::endl;
    std::cout << "sublead lep (mu) type " << subleadLep->type << " origin " << subleadLep->origin << std::endl;
    std::cout << "subsublead lep (mu) type " << subsubleadLep->type << " origin " << subsubleadLep->origin << std::endl;

    std::cout << "PILEUP " << configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::PILEUP) << std::endl;
    std::cout << "EVT " << configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::EVT) << std::endl;
    std::cout << "LEP " << configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::LEP) << std::endl;
    std::cout << "BTAG " << configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::BTAG) << std::endl;
    std::cout << "TRIG " << configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::TRIG) << std::endl;
    std::cout << "GEN " << configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::GEN) << std::endl;
    std::cout << "SherpaVjets " << configMgr->obj->evt.getSherpaVjetsNjetsWeight() << std::endl;
  }
  */

  for(ElectronVariable* el : electrons){
    int flavor = 1;

    double d0 = el->d0;
    double d0Err = el->d0Err;
    double d0sig = fabs(d0Err) > 0 ? fabs(d0 / d0Err) : 999.0;

    double z0sinTheta = fabs(el->z0 * TMath::Sin(el->Theta()));

    bool isBaseline = true;
    bool isSignal = isBaseline && el->signal;

    // Check if the lepton matches to any of the passed triggers.
    // Be careful with this if we ever expand our list of triggers,
    // since we technically want to be sure all of our leptons fire
    // the same trigger!
    bool isTriggerMatched = false;
    for(TString trig : m_listOfTriggersPassed){
      if(el->trigMatchResult(trig)) isTriggerMatched = true;
    }

    // Now fill things in!
    lepPt.push_back(el->Pt());
    lepEta.push_back(el->Eta());
    lepPhi.push_back(el->Phi());
    lepD0Sig.push_back(d0sig);
    lepZ0SinTheta.push_back(z0sinTheta);
    lepFlavor.push_back(flavor);
    lepCharge.push_back(el->q);
    lepPassOR.push_back(el->passOR);
    lepTruthType.push_back(el->type);
    lepTruthOrigin.push_back(el->origin);
    lepPassBL.push_back(el->passBL);
    lepNPix.push_back(el->nPixHitsPlusDeadSensors);
    lepVeryLoose.push_back(el->veryloosellh);
    lepLoose.push_back(el->loosellh);
    lepMedium.push_back(el->mediumllh);
    lepTight.push_back(el->tightllh);
    lepIsoLoose.push_back(el->IsoLoose);
    lepIsoTight.push_back(el->IsoTight);
    lepIsoGradient.push_back(el->IsoGradient);
    lepIsoGradientLoose.push_back(el->IsoGradientLoose);
    lepIsoLooseTrackOnly.push_back(el->IsoLooseTrackOnly);
    lepSignal.push_back(isSignal);
    lepBaseline.push_back(isBaseline);
    lepMatchesTrigger.push_back(isTriggerMatched);
  }

  for(MuonVariable* mu : muons){
    int flavor = 2;

    double d0 = mu->d0;
    double d0Err = mu->d0Err;
    double d0sig = fabs(d0Err) > 0 ? fabs(d0 / d0Err) : 999.0;

    double z0sinTheta = fabs(mu->z0 * TMath::Sin(mu->Theta()));

    bool isBaseline = mu->passOR; // since it is from the baseline container, but OR was ignored until now
    bool isSignal = mu->signal && isBaseline;

    // Check if the lepton matches to any of the passed triggers.
    // Be careful with this if we ever expand our list of triggers,
    // since we technically want to be sure all of our leptons fire
    // the same trigger!
    bool isTriggerMatched = false;
    for(TString trig : m_listOfTriggersPassed){
      if(mu->trigMatchResult(trig)) isTriggerMatched = true;
    }

    // Now fill things in!
    lepPt.push_back(mu->Pt());
    lepEta.push_back(mu->Eta());
    lepPhi.push_back(mu->Phi());
    lepD0Sig.push_back(d0sig);
    lepZ0SinTheta.push_back(z0sinTheta);
    lepFlavor.push_back(flavor);
    lepCharge.push_back(mu->q);
    lepPassOR.push_back(mu->passOR);
    lepTruthType.push_back(mu->type);
    lepTruthOrigin.push_back(mu->origin);
    lepPassBL.push_back(-1);
    lepNPix.push_back(-1);
    lepVeryLoose.push_back(mu->passesVeryLoose());
    lepLoose.push_back(mu->passesLoose());
    lepMedium.push_back(mu->passesMedium());
    lepTight.push_back(mu->passesTight());
    lepIsoLoose.push_back(mu->IsoLoose);
    lepIsoTight.push_back(mu->IsoTight);
    lepIsoGradient.push_back(mu->IsoGradient);
    lepIsoGradientLoose.push_back(mu->IsoGradientLoose);
    lepIsoLooseTrackOnly.push_back(mu->IsoLooseTrackOnly);
    lepSignal.push_back(isSignal);
    lepBaseline.push_back(isBaseline);
    lepMatchesTrigger.push_back(isTriggerMatched);
  }

  int nCentralJets = 0;
  int nBJets = 0;
  double leadJetPt = -999;
  for(JetVariable* jet : configMgr->obj->cJets){
    if(fabs(jet->Eta()) < 2.4){

      nCentralJets++;

      if(jet->bjet) nBJets++;

      if(jet->Pt() > leadJetPt){
        leadJetPt = jet->Pt();
      }

    }
  }



  configMgr->treeMaker->setIntVariable("DSID",configMgr->obj->evt.dsid);
  configMgr->treeMaker->setIntVariable("RunNumber",configMgr->obj->evt.runNumber);

  configMgr->treeMaker->setFloatVariable("weight",m_weight);

  configMgr->treeMaker->setFloatVariable("metEt",configMgr->obj->met.Et );
  configMgr->treeMaker->setFloatVariable("metPhi",configMgr->obj->met.phi );

  configMgr->treeMaker->setFloatVariable("leadJetPt",leadJetPt);

  configMgr->treeMaker->setIntVariable("nJets",nCentralJets);
  configMgr->treeMaker->setIntVariable("nBJets",nBJets);

  configMgr->treeMaker->setVecFloatVariable("lepPt",lepPt);
  configMgr->treeMaker->setVecFloatVariable("lepEta",lepEta);
  configMgr->treeMaker->setVecFloatVariable("lepPhi",lepPhi);
  configMgr->treeMaker->setVecFloatVariable("lepD0Sig",lepD0Sig);
  configMgr->treeMaker->setVecFloatVariable("lepZ0SinTheta",lepZ0SinTheta);
  configMgr->treeMaker->setVecIntVariable("lepFlavor",lepFlavor);
  configMgr->treeMaker->setVecIntVariable("lepCharge",lepCharge);
  configMgr->treeMaker->setVecIntVariable("lepPassOR",lepPassOR);
  configMgr->treeMaker->setVecIntVariable("lepTruthType",lepTruthType);
  configMgr->treeMaker->setVecIntVariable("lepTruthOrigin",lepTruthOrigin);
  configMgr->treeMaker->setVecIntVariable("lepPassBL",lepPassBL);
  configMgr->treeMaker->setVecIntVariable("lepNPix",lepNPix);
  configMgr->treeMaker->setVecIntVariable("lepVeryLoose",lepVeryLoose);
  configMgr->treeMaker->setVecIntVariable("lepLoose",lepLoose);
  configMgr->treeMaker->setVecIntVariable("lepMedium",lepMedium);
  configMgr->treeMaker->setVecIntVariable("lepTight",lepTight);
  configMgr->treeMaker->setVecIntVariable("lepIsoLoose",lepIsoLoose);
  configMgr->treeMaker->setVecIntVariable("lepIsoTight",lepIsoTight);
  configMgr->treeMaker->setVecIntVariable("lepIsoGradient",lepIsoGradient);
  configMgr->treeMaker->setVecIntVariable("lepIsoGradientLoose",lepIsoGradientLoose);
  configMgr->treeMaker->setVecIntVariable("lepIsoLooseTrackOnly",lepIsoLooseTrackOnly);
  configMgr->treeMaker->setVecIntVariable("lepSignal",lepSignal);
  configMgr->treeMaker->setVecIntVariable("lepBaseline",lepBaseline);
  configMgr->treeMaker->setVecIntVariable("lepMatchesTrigger",lepMatchesTrigger);

  return true;
}
// ------------------------------------------------------------------------------------ //
void EWK3L_Fakes::finalize(ConfigMgr*& configMgr)
{

  /*
   This method is called at the very end of the job. Can be used to merge cutflow histograms 
   for example. See CutFlowTool::mergeCutFlows(...)
  */
}
// ------------------------------------------------------------------------------------ //
