#include "SusySkimHiggsino/CutflowHiggsinos.h"


// ------------------------------------------------------------------------------------------ //
CutflowHiggsinos::CutflowHiggsinos() : BaseUser("SusySkimHiggsino","CutflowHiggsinos")
{

}
// ------------------------------------------------------------------------------------------ //
void CutflowHiggsinos::setup(ConfigMgr*& configMgr)
{

  // Object class contains the definitions of all physics objects, eg muons, electrons, jets
  // See SusySkimMaker::Objects for available methods; configMgr->obj
  // Define any variables you want to write out here. An example is given below
  // Make a cutflow stream
  configMgr->cutflow->defineCutFlow("cutFlow",configMgr->treeMaker->getFile("tree"));
  configMgr->cutflow->defineCutFlow("cutFlow_sq",configMgr->treeMaker->getFile("tree"));

  // Use object def from SUSYTools
  configMgr->obj->useSUSYToolsSignalDef(true);

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
  configMgr->obj->setFJetPt(20.0);
  configMgr->obj->setFJetEtaMin(2.80);
  configMgr->obj->setFJetEtaMax(4.50);

  
}
// ------------------------------------------------------------------------------------------ //
bool CutflowHiggsinos::doAnalysis(ConfigMgr*& configMgr)
{

  /*
    This is the main method, which is called for each event
  */

  // Skims events by imposing any cuts you define in this method below
  if( !passCuts(configMgr) ) return false;

  // Fill output trees, build observables, what ever you like here.
  // You need to define the variable above in setup(...), before filling here
  configMgr->treeMaker->setFloatVariable("met",configMgr->obj->met.Et );

  // Fill the output tree
  configMgr->treeMaker->Fill(configMgr->getSysState(),"tree");

  return true;

}
// ------------------------------------------------------------------------------------------ //
bool CutflowHiggsinos::passCuts(ConfigMgr*& configMgr)
{

  /*
   This method is used to apply any cuts you wish before writing
   the output trees
  */
  
  // Reset the weight
  double weight = 1;
  //double weight = configMgr->objectTools->getWeight(configMgr->obj);
  float lumi = 36100; // pb^{-1}
  configMgr->objectTools->setLumi(36100);
  
  // Get event details
  int dsid = configMgr->obj->evt.dsid;
  int  FS = configMgr->obj->evt.FS;

  bool isMC = configMgr->obj->evt.isMC;
  bool isHiggsino = ( dsid == 393408 ) ? true : false;
  bool isWZ       = ( dsid == 363491 ) ? true : false;
  bool isSlepton  = ( dsid == 393162 ) ? true : false;

  /*
  // Hard code genWeight for now - extract numbers from SUSYTools
  float N2C1p_xsec      = 3.013579; // pb
  float N2C1p_filtEffic = 2.750886e-02;
  int   N2C1p_sumWgt    = 1.3462103515625e+04;//weighted__AOD
  
  float WZ_xsec         = 4.5877; // pb
  float WZ_filtEffic    = 1.;
  int   WZ_sumWgt       = 5.417303e+06; //weighted__AOD
  
  float SlepLH_xsec     = 0.203233; // pb
  float SlepRH_xsec     = 0.0729029; // pb
  float Slep_filtEffic  = 2.6752E-01;
  int   SelLH_sumWgt    = 6918.0356; // SUSYWeight_ID_201_sumOfEventWeights
  int   SmuLH_sumWgt    = 7047.8823; // SUSYWeight_ID_216_sumOfEventWeights
  int   StaLH_sumWgt    = 7018.4482; // SUSYWeight_ID_206_sumOfEventWeights
  int   SelRH_sumWgt    = 2691.2048; // SUSYWeight_ID_202_sumOfEventWeights 
  int   SmuRH_sumWgt    = 2583.3491; // SUSYWeight_ID_217_sumOfEventWeights
  int   StaRH_sumWgt    = 2592.8759; // SUSYWeight_ID_207_sumOfEventWeights
  
  // Manually calculate genWeight = (xs * filtEffic * lumi) / (N sum of weights)
  float genWeight = 1.;
  if (isHiggsino) genWeight = (N2C1p_xsec * N2C1p_filtEffic * lumi) / N2C1p_sumWgt;
  if (isWZ)       genWeight = (WZ_xsec * WZ_filtEffic * lumi) / WZ_sumWgt;
  if (FS == 201)  genWeight = (SlepLH_xsec * Slep_filtEffic * lumi) / SelLH_sumWgt;
  if (FS == 216)  genWeight = (SlepLH_xsec * Slep_filtEffic * lumi) / SmuLH_sumWgt;
  if (FS == 206)  genWeight = (SlepLH_xsec * Slep_filtEffic * lumi) / StaLH_sumWgt;
  if (FS == 202)  genWeight = (SlepRH_xsec * Slep_filtEffic * lumi) / SelLH_sumWgt;
  if (FS == 217)  genWeight = (SlepRH_xsec * Slep_filtEffic * lumi) / SmuLH_sumWgt;
  if (FS == 207)  genWeight = (SlepRH_xsec * Slep_filtEffic * lumi) / StaLH_sumWgt;
  */

  // Do as inclusive strong 1L do: eventWeight, leptonWeight, jvtWeight, pileupWeight
  //weight *= genWeight;
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::GEN);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::EVT);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::LEP);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::JVT);
  weight *= configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::PILEUP);
  
  // Fill cutflow histograms
  configMgr->cutflow->bookCut("cutFlow","allEvents",weight );    // non-sequential cutflow
  configMgr->cutflow->bookCut("cutFlow_sq","allEvents",weight ); // sequential cutflow

  // Apply all recommended event cleaning cuts
  if( !configMgr->obj->passEventCleaning( configMgr->cutflow, "cutFlow", weight ) ) return false;
  if( !configMgr->obj->passEventCleaning( configMgr->cutflow, "cutFlow_sq", weight ) ) return false;

  // for now set m_weight to the default weight
  double m_weight = weight;

  // ------------------------------------------------------
  // Calculate variables
  // ------------------------------------------------------
  
  // Multiplicity
  int nLeptons_base     = configMgr->obj->baseLeptons.size();   
  int nLeptons_signal   = configMgr->obj->signalLeptons.size();   

  int nElectrons_base   = configMgr->obj->baseElectrons.size();  
  int nMuons_base       = configMgr->obj->baseMuons.size();    

  int nElectrons_signal = configMgr->obj->signalElectrons.size();  
  int nMuons_signal     = configMgr->obj->signalMuons.size();   
  
  int nTotalJet = getNJets( configMgr->obj, 25.0 ) + configMgr->obj->fJets.size(); 
  int nJet20    = getNJets( configMgr->obj, 20.0 );
  int nJet25    = getNJets( configMgr->obj, 25.0 );
  int nJet30    = getNJets( configMgr->obj, 30.0 );
  int nBJet30 = getNBJets( configMgr->obj, 30.0 );

  // Object properties and kinematics
  float met_Et = configMgr->obj->met.Et;

  int lep1Charge = nLeptons_base >= 1 ? configMgr->obj->baseLeptons[0]->q    : 0;
  int lep2Charge = nLeptons_base >= 2 ? configMgr->obj->baseLeptons[1]->q    : 0;
  int lep1Flavor = ( nLeptons_base >= 1 && configMgr->obj->baseLeptons[0]->isMu() ) ? 2 : 1;
  int lep2Flavor = ( nLeptons_base >= 2 && configMgr->obj->baseLeptons[1]->isMu() ) ? 2 : 1;

  float lep1Pt   = nLeptons_base >= 1 ? configMgr->obj->baseLeptons[0]->Pt() : -99;
  float lep2Pt   = nLeptons_base >= 2 ? configMgr->obj->baseLeptons[0]->Pt() : -99;

  bool is_SFOS = (lep1Charge != lep2Charge) && (lep1Flavor == lep2Flavor);

  float jet1Pt = configMgr->obj->cJets.size()>=1 ? configMgr->obj->cJets[0]->Pt() : -99.;

  float DPhiJ1Met     = getDPhiJ1Met(configMgr->obj);
  float MSqTauTau_1   = getMSqTauTau(configMgr->obj, 1); // version 1 : positive definite
  float MSqTauTau_2   = getMSqTauTau(configMgr->obj, 2); // version 2 : more natural definition
  bool useBaseline    = true;
  float mll           = getMll(configMgr->obj, useBaseline);
  float Rll           = getRll(configMgr->obj); 

  // Triggers
  // data only triggers for now
  bool HLT_2mu4_j85_xe50_mht  = configMgr->obj->evt.getHLTEvtTrigDec("HLT_2mu4_j85_xe50_mht");
  bool HLT_mu4_j125_xe90_mht  = configMgr->obj->evt.getHLTEvtTrigDec("HLT_mu4_j125_xe90_mht");

  // following are emulated in TriggerTools
  bool HLT_mu4  = configMgr->obj->evt.getHLTEvtTrigDec("HLT_mu4");
  bool HLT_2mu4 = configMgr->obj->evt.getHLTEvtTrigDec("HLT_2mu4");

  bool HLT_j85_L1J40  = configMgr->obj->evt.getHLTEvtTrigDec("HLT_j85_L1J40");
  bool HLT_j125_L1J50 = configMgr->obj->evt.getHLTEvtTrigDec("HLT_j125_L1J50");  

  bool HLT_xe90_mht_L1XE40  = configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe90_mht_L1XE40");  
  bool HLT_xe50_mht_L1XE20  = configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe50_mht_L1XE20");

  // emulated in SUSYTools
  bool HLT_xe110_mht_L1XE50 = configMgr->obj->evt.getHLTEvtTrigDec("HLT_xe110_mht_L1XE50");

  // combine the emulated triggers
  bool emul_mu4_j125_xe90 = HLT_mu4 && HLT_j125_L1J50 && HLT_xe90_mht_L1XE40;
  bool emul_2mu4_j85_xe50 = HLT_2mu4 && HLT_j85_L1J40 && HLT_xe50_mht_L1XE20;

  // ------------------------------------------------------
  // Non-sequential cuts 
  // i.e. events not discarded after each cut
  // ------------------------------------------------------
  /*
  NON-SEQUENTIAL (after cleaning)
  >=1 baseline electron
  >=1 signal electron
  >=1 baseline muon
  >=1 signal muon
  >=2 baseline electron
  >=2 signal electron
  >=2 baseline muon
  >=2 signal muon
  >=2 baseline leptons
  >=2 signal leptons
  exactly 3 baseline electrons
  exactly 3 signal electrons
  exactly 3 baseline muon
  exactly 3 signal muon
  exactly 3 baseline leptons
  exactly 3 signal leptons
  >=1 baseline jet
  >=1 signal jet
  zero b-jet jets
  pass mu4_j125_xe90
  pass 2mu4_j85_xe50
  pass met110_mht
  */

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

  // nJets
  if(nTotalJet >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 baseline jet", m_weight );
  if(nJet30 >= 1)
    configMgr->cutflow->bookCut("cutFlow", ">= 1 signal jet", m_weight );
  if(nBJet30 == 0)
    configMgr->cutflow->bookCut("cutFlow", "== 0 b-jets", m_weight );

  // triggers
  if(HLT_mu4_j125_xe90_mht) 
    configMgr->cutflow->bookCut("cutFlow", "Pass data HLT_mu4_j125_xe90_mht", m_weight );
  if(HLT_2mu4_j85_xe50_mht) 
    configMgr->cutflow->bookCut("cutFlow", "Pass data HLT_2mu4_j85_xe50_mht", m_weight );
  if(emul_mu4_j125_xe90) 
    configMgr->cutflow->bookCut("cutFlow", "Pass emul_mu4_j125_xe90", m_weight );
  if(emul_2mu4_j85_xe50) 
    configMgr->cutflow->bookCut("cutFlow", "Pass emul_2mu4_j85_xe50", m_weight );
  if(HLT_xe110_mht_L1XE50)   
    configMgr->cutflow->bookCut("cutFlow", "Pass HLT_xe110_mht_L1XE50", m_weight );

  // ------------------------------------------------------
  // Sequential cuts 
  // i.e. events discarded after each cut
  // ------------------------------------------------------

  /*
  SEQUENTIAL (after cleaning)
  >=2 baseline leptons
  >=2 signal leptons
  SFOS
  leading lepton pT<50 GeV
  >=1 signal jet
  leading jet pT>150 GeV
  b-jet veto
  MET>150 GeV
  Dphi(jet,MET)>2.5
  mll<50 GeV
  Rll<1.5
  mtt<0
  */
 
  if( nLeptons_base < 2 ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", ">= 2 baseline leptons", m_weight );
  
  if( nLeptons_signal < 2 ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", ">= 2 signal leptons", m_weight );

  if( !is_SFOS ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", "SFOS", m_weight );

  if( lep1Pt > 50. ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", "Leading lepton pT < 50 GeV", m_weight );

  if( nJet30 < 1 ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", ">= 1 signal jet", m_weight );

  if( jet1Pt < 150 ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", "Leading jet pT > 150 GeV", m_weight );
  
  if( nBJet30 != 0 ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", "b-jet veto", m_weight );

  if( met_Et < 150 ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", "MET > 150 GeV", m_weight );

  // IMPORTANT BLINDING | data: DPhiJ1Met < 2.5, mc: DPhiJ1Met > 2.5
  if( isMC ) {
    if( DPhiJ1Met < 2.5 ) return false;
    configMgr->cutflow->bookCut("cutFlow_sq", "DPhiJ1Met > 2.5", m_weight );
  }
  else {
    if( DPhiJ1Met > 2.5 ) return false;
    configMgr->cutflow->bookCut("cutFlow_sq", "DPhiJ1Met < 2.5", m_weight );
  }

  if( mll > 50 ) return false;
  configMgr->cutflow->bookCut("cutFlow_sq", "mll < 50 GeV", m_weight );

  // slepton: R>1.5, else R<1.5
  if(isSlepton) {
    if( Rll < 1.5 ) return false;
    configMgr->cutflow->bookCut("cutFlow_sq", "Rll > 1.5", m_weight );
  }
  else {
    if( Rll > 1.5 ) return false;
    configMgr->cutflow->bookCut("cutFlow_sq", "Rll < 1.5", m_weight );
  }

  // Use the MSqTauTau_2 definition of m^2(tautau) for now
  if( MSqTauTau_2 > 0 ) return false;
    configMgr->cutflow->bookCut("cutFlow_sq", "mtt < 0", m_weight );


  return true;

}
// ------------------------------------------------------------------------------------------ //
void CutflowHiggsinos::finalize(ConfigMgr*& configMgr)
{

  /*
   This method is called at the very end of the job. Can be used to merge cutflow histograms 
   for example. See CutFlowTool::mergeCutFlows(...)
  */

}
// ------------------------------------------------------------------------------------------ //
