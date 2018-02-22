#include "SusySkimHiggsino/EWK3L_Opt.h"
using namespace std;

// ------------------------------------------------------------------------------------ //
EWK3L_Opt::EWK3L_Opt() :
  BaseUser("SusySkimHiggsino", "EWK3L_Opt")
{}
// ------------------------------------------------------------------------------------ //
void EWK3L_Opt::setup(ConfigMgr*& configMgr)
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
  configMgr->treeMaker->setIntVariable("RunNumber",configMgr->obj->evt.runNumber);
  configMgr->treeMaker->addFloatVariable("weight",-999);
  configMgr->treeMaker->addBoolVariable("trigMatched",false);
  configMgr->treeMaker->addIntVariable("finalState",0);
  configMgr->treeMaker->addIntVariable("nJets",-999);
  configMgr->treeMaker->addIntVariable("nBJets", -999);
  configMgr->treeMaker->addIntVariable("nBaselineLeptons",-999);
  configMgr->treeMaker->addIntVariable("nSignalLeptons",-999);

  configMgr->treeMaker->addFloatVariable("lep1Pt",-999);
  configMgr->treeMaker->addFloatVariable("lep2Pt",-999);
  configMgr->treeMaker->addFloatVariable("lep3Pt",-999);

  configMgr->treeMaker->addFloatVariable("m3l",-999);

  configMgr->treeMaker->addFloatVariable("mll",-99);
  configMgr->treeMaker->addFloatVariable("mll_minmtw",-999);

  configMgr->treeMaker->addFloatVariable("z1LepPt",-999);
  configMgr->treeMaker->addFloatVariable("z1LepEta",-999);
  configMgr->treeMaker->addIntVariable("z1LepFlavor",-999);
  configMgr->treeMaker->addIntVariable("z1LepCharge",-999);
  configMgr->treeMaker->addFloatVariable("z2LepPt",-999);
  configMgr->treeMaker->addFloatVariable("z2LepEta",-999);
  configMgr->treeMaker->addIntVariable("z2LepFlavor",-999);
  configMgr->treeMaker->addIntVariable("z2LepCharge",-999);

  configMgr->treeMaker->addFloatVariable("wLepPt",-999);
  configMgr->treeMaker->addFloatVariable("wLepEta",-999);
  configMgr->treeMaker->addIntVariable("wLepFlavor",-999);
  configMgr->treeMaker->addIntVariable("wLepCharge",-999);
  configMgr->treeMaker->addFloatVariable("wLepMt",-999);
  configMgr->treeMaker->addFloatVariable("minMt",-999);

  configMgr->treeMaker->addBoolVariable("SFOS",false);

  configMgr->treeMaker->addFloatVariable("pT3l",-999);
  configMgr->treeMaker->addFloatVariable("pT3lOverMet",-999);
  configMgr->treeMaker->addFloatVariable("p3lprojOntopWZ",-999);
  configMgr->treeMaker->addFloatVariable("Lt",-999);
  configMgr->treeMaker->addFloatVariable("Ht",-999);
  configMgr->treeMaker->addFloatVariable("met",-999);
  configMgr->treeMaker->addFloatVariable("neutrino_pt",-999);
  configMgr->treeMaker->addFloatVariable("truth_met",-999);
  configMgr->treeMaker->addFloatVariable("met_soft",-999);

  configMgr->treeMaker->addFloatVariable("w_mass",-999);
  configMgr->treeMaker->addFloatVariable("truth_mt", -999);
  configMgr->treeMaker->addFloatVariable("w_mass_trad",-999);
  configMgr->treeMaker->addFloatVariable("truth_mt_trad", -999);

  configMgr->treeMaker->addVecFloatVariable("jetPt");
  configMgr->treeMaker->addVecFloatVariable("jetEta");
  configMgr->treeMaker->addIntVariable("nJetsAbsEtaLt2pt8",-999);
  configMgr->treeMaker->addIntVariable("nBJetsAbsEtaLt2pt8",-999);
  configMgr->treeMaker->addFloatVariable("leadJetPtAbsEtaLt2pt8",-999);
  configMgr->treeMaker->addVecFloatVariable("jetPtAbsEtaLt2pt8");
  configMgr->treeMaker->addVecFloatVariable("jetEtaAbsEtaLt2pt8");

  configMgr->treeMaker->addIntVariable("nJetsAbsEtaLt2pt4",-999);
  configMgr->treeMaker->addIntVariable("nBJetsAbsEtaLt2pt4",-999);
  configMgr->treeMaker->addVecFloatVariable("jetPtAbsEtaLt2pt4");
  configMgr->treeMaker->addVecFloatVariable("jetEtaAbsEtaLt2pt4");

  configMgr->treeMaker->addFloatVariable("pTZ", -999);
  configMgr->treeMaker->addFloatVariable("deltaPhiZMet",-999);
  configMgr->treeMaker->addFloatVariable("mTminSig", -999);

  configMgr->treeMaker->addBoolVariable("trigPassed", false);

  //configMgr->treeMaker->addVecFloatVariable("truthWeights");
}
// ------------------------------------------------------------------------------------ //
void EWK3L_Opt::fillWeights(ConfigMgr*& configMgr)
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
  double gen_weight = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::GEN);

  //get appropriate weight for SUSY 
  int FS = configMgr->obj->evt.FS;
  float xsec = configMgr->obj->evt.xsec;
  int dsid = configMgr->obj->evt.dsid;

  if ((FS==125)||(FS==127)){
    string line;

    string txtfilename = "RawN_subprocess.txt";
    ifstream in(txtfilename.c_str());

    while ( getline(in, line) )
      {
        if ( !line.empty() )
          {
            while ( line[0] == ' ' ) line.erase(0, 1);
          }
        if ( !line.empty() && isdigit(line[0]) )
          {
            stringstream is(line);
            int id;
            float c1p, c1m;
            is >> id >> c1p >> c1m;
            if (id == dsid){
              if (FS==125) m_weight*=xsec*36075/c1p;
              else m_weight*=xsec*36075/c1m;
            }
          }
      }
  }
  else m_weight *= gen_weight ;

  // for V+jet samples, apply extra weight
  m_weight *= configMgr->obj->evt.getSherpaVjetsNjetsWeight();

  return;
}
// ------------------------------------------------------------------------------------ //
bool EWK3L_Opt::passCuts(ConfigMgr*& configMgr)
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
  if(nLeptons_base == 3 && nLeptons_signal == 3){
    configMgr->cutflow->bookCut("cutFlow", "== 3 baseline leptons and == 3 signal leptons", m_weight );
    double m_pileup = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::PILEUP);
    double m_mc = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::EVT);
    double m_lep = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::LEP);
    double m_btag = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::BTAG);
    double m_lumi = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::GEN);
    configMgr->cutflow->bookCut("cutFlow", "lep: == 3 baseline leptons and == 3 signal leptons", m_lep );
    configMgr->cutflow->bookCut("cutFlow", "pileup: == 3 baseline leptons and == 3 signal leptons", m_pileup );
    configMgr->cutflow->bookCut("cutFlow", "lumi: == 3 baseline leptons and == 3 signal leptons", m_lumi );
    configMgr->cutflow->bookCut("cutFlow", "all: == 3 baseline leptons and == 3 signal leptons", m_btag*m_lumi*m_pileup*m_lep );
    configMgr->cutflow->bookCut("cutFlow", "mc: == 3 baseline leptons and == 3 signal leptons", m_mc );
    if (nBTagJets==0){
      configMgr->cutflow->bookCut("cutFlow", "nBJet = 0 ", m_weight );
      if (configMgr->obj->met.Et > 50){
        configMgr->cutflow->bookCut("cutFlow", "met > 50", m_weight );
      }
    }
  }

  if (nLeptons_base == 2 && nLeptons_signal == 2){
    bool SFOS = false;
    LeptonVariable* lep1 = configMgr->obj->signalLeptons[0];
    LeptonVariable* lep2 = configMgr->obj->signalLeptons[1];
    if( (lep1->q*lep2->q)<0   ) SFOS=true;

    float mass = (*lep1+*lep2).M();
    if ((lep1->Pt()>20) && (lep2->Pt()>20)&& SFOS && (mass>40)){
      configMgr->cutflow->bookCut("cutFlow", "Exactly 2 OS baseline+signal leptons, pt>20, mll>40", m_weight );

      double m_pileup = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::PILEUP);
      double m_mc = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::EVT);
      double m_lep = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::LEP);
      double m_btag = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::BTAG);
      double m_lumi = configMgr->objectTools->getWeight(configMgr->obj, "", ObjectTools::WeightType::GEN);

      configMgr->cutflow->bookCut("cutFlow", "lep: Exactly 2 OS baseline+signal leptons, pt>20, mll>40", m_lep );
      configMgr->cutflow->bookCut("cutFlow", "pileup: Exactly 2 OS baseline+signal leptons, pt>20, mll>40", m_pileup );
      configMgr->cutflow->bookCut("cutFlow", "lumi: Exactly 2 OS baseline+signal leptons, pt>20, mll>40", m_lumi );
      configMgr->cutflow->bookCut("cutFlow", "all: Exactly 2 OS baseline+signal leptons, pt>20, mll>40", m_btag*m_lumi*m_pileup*m_lep );
      configMgr->cutflow->bookCut("cutFlow", "mc: Exactly 2 OS baseline+signal leptons, pt>20, mll>40", m_mc );
    }
  }

  return true;

}
// ------------------------------------------------------------------------------------ //
void EWK3L_Opt::fillListOfTriggersPassed(ConfigMgr*& configMgr)
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
      /*
      if (((configMgr->obj->evt.evtNumber==1894010556 && configMgr->obj->evt.runNumber == 309759)||
	   (configMgr->obj->evt.evtNumber==1436971113 && configMgr->obj->evt.runNumber == 307306)||
	   (configMgr->obj->evt.evtNumber==2981718716 && configMgr->obj->evt.runNumber == 299584)||
	   (configMgr->obj->evt.evtNumber==36559227 && configMgr->obj->evt.runNumber == 305727)||
	   (configMgr->obj->evt.evtNumber==127491007 && configMgr->obj->evt.runNumber == 307306)||
	   (configMgr->obj->evt.evtNumber==1084628375 && configMgr->obj->evt.runNumber == 307306)||
	   (configMgr->obj->evt.evtNumber==1928674952 && configMgr->obj->evt.runNumber == 309759))) std::cout << " trigger passed " << trig << std::endl;
      */

    }
  }

  return;
}
// ------------------------------------------------------------------------------------ //
bool EWK3L_Opt::doAnalysis(ConfigMgr*& configMgr)
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

  // The signal selection
  if( !Simple_Selection(configMgr) ) return false;

  // Fill the output tree
  configMgr->treeMaker->Fill(configMgr->getSysState(),"tree");

  return true;

}
// ------------------------------------------------------------------------------------ //
bool EWK3L_Opt::Simple_Selection(ConfigMgr*& configMgr)
{
  
  // No triggers passed!
  bool trigPassed = false;
  if(m_listOfTriggersPassed.size() > 0) trigPassed = true;

  int FS = configMgr->obj->evt.FS;
  float xsec = configMgr->obj->evt.xsec;
  int dsid = configMgr->obj->evt.dsid;

  double mll = 0.0;
  double wLepMt = -999;
  double zLepMt = -999;

  float mll_new = -999;
  double min_mt = -999;
  float mll_minmtw = -999;

  bool SFOS = false;

  unsigned int zlep_1 = 0;
  unsigned int zlep_2 = 0;
  unsigned int wlep_idx = 0;

  unsigned int newzlep_1 = 0;
  unsigned int newzlep_2 = 0;
  unsigned int newwlep_idx = 0;

  float bestMass = 999999.0;

  double mZ = 91.2;
  bool newMll = false;

  int nLeptons_sig   = configMgr->obj->signalLeptons.size();

  if( nLeptons_sig < 3 ) return false;
  
  int nLep_trigMatched[3];
  for (unsigned int i =0; i < m_listOfTriggersPassed.size(); i++){
    nLep_trigMatched[i]=0;
  }

  //determine pair of leptons that form mll closest to zpeak
  for( unsigned int one=0; one<configMgr->obj->signalLeptons.size(); one++ ){
    LeptonVariable* lep1 = configMgr->obj->signalLeptons[one];
    for (unsigned int i =0; i < m_listOfTriggersPassed.size(); i++){
      TString trig =  m_listOfTriggersPassed[i];
      if(lep1->trigMatchResult(trig)){
        nLep_trigMatched[i]++;
	//std::cout << "flavor " << lep1->isEle() << " trigger " << trig << " number trig match " << nLep_trigMatched[i] << std::endl;
      }
    }
    for( unsigned int two=0; two<configMgr->obj->signalLeptons.size(); two++ ){
      LeptonVariable* lep2 = configMgr->obj->signalLeptons[two];

      if( lep1==lep2 ) continue;

      SFOS = false;
      if( lep2->isMu() && lep1->isMu() && (lep1->q*lep2->q)<0   ) SFOS=true;
      if( lep2->isEle() && lep1->isEle() && (lep1->q*lep2->q)<0 ) SFOS=true;

      if( !SFOS ) continue;

      float mass = (*lep1+*lep2).M();

      if( fabs(mass-91.2) < fabs(bestMass-91.2) ){
        if (lep1->Pt() > lep2->Pt()){
          zlep_1 = one;
          zlep_2 = two;
        }
        else{
          zlep_2 = one;
          zlep_1 = two;

        }
        bestMass = mass;
        mll = bestMass;

      }
    }
  }
  LeptonVariable *lep1 = configMgr->obj->signalLeptons[zlep_2];
  LeptonVariable *lep2 = configMgr->obj->signalLeptons[zlep_1];

  if( ((zlep_1+zlep_2) > 0 ) && (bestMass > -999.0)) SFOS = true;

  if( (zlep_1+zlep_2) > 0 ){

    for( unsigned int idx=0; idx<configMgr->obj->signalLeptons.size(); idx++ ){
      if( idx != zlep_1 && idx != zlep_2 ){
        LeptonVariable* wLep = configMgr->obj->signalLeptons[idx];
        wlep_idx = idx;
        wLepMt = getMt(configMgr->obj,false,idx);

      }
    }
  }

  LeptonVariable *wLep = configMgr->obj->signalLeptons[wlep_idx];
  LeptonVariable *z1Lep = configMgr->obj->signalLeptons[zlep_1];
  LeptonVariable *z2Lep = configMgr->obj->signalLeptons[zlep_2];

  int z1LepFlavor = (z1Lep->isEle()==1)? 1:2;
  int z2LepFlavor = (z2Lep->isEle()==1)? 1:2;
  int wLepFlavor = (wLep->isEle()==1)? 1:2;

  double new_z1LepPt, new_z2LepPt, new_wLepPt;
  new_z1LepPt=0;
  new_z2LepPt=0;
  new_wLepPt = 0;

  unsigned int tempzlep_1 = 0;
  unsigned int tempzlep_2 = 0;
  unsigned int tempwlep_idx =0;

  if (SFOS && (z1LepFlavor == wLepFlavor)){
    if (z2Lep->q!=wLep->q){
      LeptonVariable* z_lep = configMgr->obj->signalLeptons[zlep_2];
      LeptonVariable* newz_lep = configMgr->obj->signalLeptons[wlep_idx];
      mll_new = (*z_lep+*newz_lep).M();
      zLepMt = getMt(configMgr->obj,false,zlep_1);
      tempwlep_idx = zlep_1;
      if (newz_lep->Pt() > z_lep->Pt()){
	tempzlep_1 = wlep_idx;
	tempzlep_2 = zlep_2;
      }
      else{
	tempzlep_2 = wlep_idx;
	tempzlep_1 = zlep_2;
      }
    }
    if (z1Lep->q!=wLep->q){
      LeptonVariable* z_lep = configMgr->obj->signalLeptons[zlep_1];
      LeptonVariable* newz_lep = configMgr->obj->signalLeptons[wlep_idx];
      mll_new = (*z_lep+*newz_lep).M();
      zLepMt = getMt(configMgr->obj,false,zlep_2);
      tempwlep_idx =zlep_2;
      if (newz_lep->Pt() > z_lep->Pt()){
        tempzlep_1 = wlep_idx;
        tempzlep_2 = zlep_1;
      }
      else{
	tempzlep_2 = wlep_idx;
        tempzlep_1 = zlep_1;
      }
    }
    if (abs(mll_new-mZ)> 0){
      newMll = true;

      if (wLepMt > 0 && wLepMt <= zLepMt){
        min_mt = wLepMt;
        mll_minmtw = mll;
        newzlep_1 = zlep_1;
        newzlep_2 = zlep_2;
        newwlep_idx = wlep_idx;
      }
      if (zLepMt > 0 && zLepMt < wLepMt){
        min_mt = zLepMt;
        mll_minmtw = mll_new;
        newzlep_1 = tempzlep_1;
        newzlep_2 = tempzlep_2;
        newwlep_idx = tempwlep_idx;

      }
    }
    else{
      min_mt = wLepMt;
      mll_minmtw = mll;
      newzlep_1 = zlep_1;
      newzlep_2 = zlep_2;
      newwlep_idx = wlep_idx;
    }

  }
  else{
    min_mt = wLepMt;
    mll_minmtw = mll;
    newzlep_1 = zlep_1;
    newzlep_2 = zlep_2;
    newwlep_idx = wlep_idx;
  }

  //calculate variables to fill trees
  bool trigMatched=false;
  for (unsigned int i =0; i < m_listOfTriggersPassed.size(); i++){
    trigMatched = trigMatched || (nLep_trigMatched[i]>=2);
  }

  int nLeptons_signal = configMgr->obj->signalLeptons.size();
  int nLeptons_base   = configMgr->obj->baseLeptons.size();

  int nBTagJets       = configMgr->obj->bJets.size();

  lep1 = configMgr->obj->signalLeptons[0];
  lep2 = configMgr->obj->signalLeptons[1];
  LeptonVariable *lep3 = configMgr->obj->signalLeptons[2];

  LeptonVariable *z1_lep = configMgr->obj->signalLeptons[newzlep_1];
  LeptonVariable *z2_lep = configMgr->obj->signalLeptons[newzlep_2];
  LeptonVariable *w_lep = configMgr->obj->signalLeptons[newwlep_idx];

  float m3l = (*lep1+*lep2+*lep3).M();
  float pt3l = (*lep1+*lep2+*lep3).Pt();

  float lep1Pt = configMgr->obj->signalLeptons[0]->Pt();
  float lep2Pt = configMgr->obj->signalLeptons[1]->Pt();
  float lep3Pt = configMgr->obj->signalLeptons[2]->Pt();

  float Lt = lep1Pt+lep2Pt+lep3Pt;

  double met = configMgr->obj->met.Et;
  int nJets = configMgr->obj->cJets.size();

  float pT3lOverMet = pt3l/met;
  float p3lprojOntopWZ = ((*lep1 + *lep2 + *lep3)*(*lep1+ *lep2 + *lep3 + configMgr->obj->met))/ pow((*lep1+ *lep2 + *lep3 + configMgr->obj->met).Mag(),2);

  float pTZ = (*z1_lep + *z2_lep).Pt();
  float deltaPhiZMet = std::abs( (*z1_lep + *z2_lep).DeltaPhi( configMgr->obj->met));

  int z1_flavor = (z1_lep->isEle()==1)? 1:2;
  int z2_flavor = (z2_lep->isEle()==1)? 1:2;
  int w_flavor = (w_lep->isEle()==1)? 1:2;

  int nCentralJets = 0;
  int nCentralBJets = 0;
  int nCentralJetsLt2pt4 = 0;
  int nCentralBJetsLt2pt4 = 0;
  double leadCentralJetPt = -999;

  std::vector<float> jetPt;
  std::vector<float> jetEta;
  std::vector<float> jetPtAbsEtaLt2pt8;
  std::vector<float> jetEtaAbsEtaLt2pt8;
  std::vector<float> jetPtAbsEtaLt2pt4;
  std::vector<float> jetEtaAbsEtaLt2pt4;

  float Ht = -999;

  for(JetVariable* jet : configMgr->obj->cJets){

    if(fabs(jet->Eta()) < 2.8){
      if(jet->Pt() > leadCentralJetPt){
        leadCentralJetPt = jet->Pt();
        jetPtAbsEtaLt2pt8.push_back(jet->Pt());
        jetEtaAbsEtaLt2pt8.push_back(jet->Eta());
      }
      nCentralJets++;
    }

    if(fabs(jet->Eta()) < 2.4){

      jetPtAbsEtaLt2pt4.push_back(jet->Pt());
      jetEtaAbsEtaLt2pt4.push_back(jet->Eta());

      nCentralJetsLt2pt4++;
    }

    jetPt.push_back(jet->Pt());
    jetEta.push_back(jet->Eta());

    Ht = Ht + jet->Pt();
  }

  for(JetVariable* bjet : configMgr->obj->bJets){
    if(fabs(bjet->Eta()) < 2.8){
      nCentralBJets++;
    }
    if(fabs(bjet->Eta()) < 2.4){
      nCentralBJetsLt2pt4++;
    }
  }

  float mTminSig = min_mt/Ht;

  float neutrino_pt = -999;
  float w_mass = -999;
  float truth_mt = -999;
  float w_mass_trad = -999;
  float truth_mt_trad = 0.0;

  LeptonVariable *w_lep_trad = configMgr->obj->signalLeptons[wlep_idx];

  /*if( configMgr->treeMaker->getTreeState()=="" && configMgr->obj->truthEvent.WZtruthTLVs.size()>0 ){

    const TLorentzVector* neutrino  = configMgr->obj->truthEvent.getTruthTLV("neutrino");
    neutrino_pt = neutrino->Pt();
    w_mass = (*w_lep+*neutrino).M();
    w_mass_trad = (*w_lep_trad+*neutrino).M();
    truth_mt_trad = sqrt(2*w_lep_trad->Pt()*neutrino->Pt() - 2*w_lep_trad->Px()*neutrino->Px() - 2*w_lep_trad->Py()*neutrino->Py());
    truth_mt = sqrt(2*w_lep->Pt()*neutrino->Pt() - 2*w_lep->Px()*neutrino->Px() - 2*w_lep->Py()*neutrino->Py());
  }
  */

  //fill tree
  configMgr->treeMaker->setIntVariable("DSID",configMgr->obj->evt.dsid);
  configMgr->treeMaker->setIntVariable("RunNumber",configMgr->obj->evt.runNumber);
  configMgr->treeMaker->setFloatVariable("weight",m_weight);
  configMgr->treeMaker->setIntVariable("finalState", FS);
  configMgr->treeMaker->setBoolVariable("trigMatched", trigMatched);
  configMgr->treeMaker->setBoolVariable("trigPassed",trigPassed);
  configMgr->treeMaker->setIntVariable("nJets",nJets);
  configMgr->treeMaker->setIntVariable("nBJets", nBTagJets);
  configMgr->treeMaker->setIntVariable("nBaselineLeptons",nLeptons_base);
  configMgr->treeMaker->setIntVariable("nSignalLeptons",(configMgr->obj->signalLeptons.size()));
  configMgr->treeMaker->setFloatVariable("m3l",m3l);
  configMgr->treeMaker->setFloatVariable("lep1Pt", lep1Pt);
  configMgr->treeMaker->setFloatVariable("lep2Pt", lep2Pt);
  configMgr->treeMaker->setFloatVariable("lep3Pt", lep3Pt);
  configMgr->treeMaker->setBoolVariable("SFOS", SFOS);
  configMgr->treeMaker->setFloatVariable("mll",mll);
  configMgr->treeMaker->setFloatVariable("mll_minmtw", mll_minmtw);
  configMgr->treeMaker->setFloatVariable("z1LepPt",z1_lep->Pt());
  configMgr->treeMaker->setFloatVariable("z1LepEta",z1_lep->Eta());
  configMgr->treeMaker->setIntVariable("z1LepFlavor",z1_flavor);
  configMgr->treeMaker->setIntVariable("z1LepCharge",z1_lep->q);
  configMgr->treeMaker->setFloatVariable("z1LepPt",z2_lep->Pt());
  configMgr->treeMaker->setFloatVariable("z1LepEta",z2_lep->Eta());
  configMgr->treeMaker->setIntVariable("z1LepFlavor",z2_flavor);
  configMgr->treeMaker->setIntVariable("z1LepCharge",z2_lep->q);
  configMgr->treeMaker->setFloatVariable("z1LepPt",w_lep->Pt());
  configMgr->treeMaker->setFloatVariable("z1LepEta",w_lep->Eta());
  configMgr->treeMaker->setIntVariable("z1LepFlavor",w_flavor);
  configMgr->treeMaker->setIntVariable("z1LepCharge",w_lep->q);
  configMgr->treeMaker->setFloatVariable("wLepMt",wLepMt);
  configMgr->treeMaker->setFloatVariable("minMt", min_mt);
  configMgr->treeMaker->setFloatVariable("met",configMgr->obj->met.Et );
  configMgr->treeMaker->setFloatVariable("truth_met",configMgr->obj->met.Et_truth);
  configMgr->treeMaker->setFloatVariable("neutrino_pt",neutrino_pt);
  configMgr->treeMaker->setFloatVariable("met_soft", configMgr->obj->met.Et_soft);

  configMgr->treeMaker->setFloatVariable("w_mass", w_mass);
  configMgr->treeMaker->setFloatVariable("truth_mt", truth_mt);
  configMgr->treeMaker->setFloatVariable("w_mass_trad", w_mass_trad);
  configMgr->treeMaker->setFloatVariable("truth_mt_trad", truth_mt_trad);

  configMgr->treeMaker->setFloatVariable("pT3l", pt3l);
  configMgr->treeMaker->setFloatVariable("pT3lOverMet",pT3lOverMet);
  configMgr->treeMaker->setFloatVariable("p3lprojOntopWZ",p3lprojOntopWZ);
  configMgr->treeMaker->setFloatVariable("Lt", Lt);

  configMgr->treeMaker->setIntVariable("nJetsAbsEtaLt2pt8",nCentralJets);
  configMgr->treeMaker->setIntVariable("nBJetsAbsEtaLt2pt8",nCentralBJets);
  configMgr->treeMaker->setIntVariable("nJetsAbsEtaLt2pt4",nCentralJetsLt2pt4);
  configMgr->treeMaker->setIntVariable("nBJetsAbsEtaLt2pt4",nCentralBJetsLt2pt4);

  configMgr->treeMaker->setVecFloatVariable("jetPt",jetPt);
  configMgr->treeMaker->setVecFloatVariable("jetEta",jetEta);
  configMgr->treeMaker->setVecFloatVariable("jetPtAbsEtaLt2pt8",jetPtAbsEtaLt2pt8);
  configMgr->treeMaker->setVecFloatVariable("jetEtaAbsEtaLt2pt8",jetEtaAbsEtaLt2pt8);
  configMgr->treeMaker->setVecFloatVariable("jetPtAbsEtaLt2pt4",jetPtAbsEtaLt2pt4);
  configMgr->treeMaker->setVecFloatVariable("jetEtaAbsEtaLt2pt4",jetEtaAbsEtaLt2pt4);

  configMgr->treeMaker->setFloatVariable("pTZ", pTZ);
  configMgr->treeMaker->setFloatVariable("deltaPhiZMet", deltaPhiZMet);
  configMgr->treeMaker->setFloatVariable("mTminSig", mTminSig);
  
  //configMgr->treeMaker->setVecFloatVariable("truthWeights", truthEvtVarWeights);
  
  return true;
}
// ------------------------------------------------------------------------------------ //
void EWK3L_Opt::finalize(ConfigMgr*& configMgr)
{

  /*
   This method is called at the very end of the job. Can be used to merge cutflow histograms 
   for example. See CutFlowTool::mergeCutFlows(...)
  */
}
// ------------------------------------------------------------------------------------ //
