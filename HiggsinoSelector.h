#ifndef SusySkimHiggsino_HiggsinoSelector_H
#define SusySkimHiggsino_HiggsinoSelector_H

//RootCore
#include "SusySkimMaker/BaseUser.h"

class HiggsinoSelector : public BaseUser
{

 public:
  HiggsinoSelector();
  ~HiggsinoSelector() {};

  void setup(ConfigMgr*& configMgr);
  bool doAnalysis(ConfigMgr*& configMgr);
  bool passCuts(ConfigMgr*& configMgr);
  void finalize(ConfigMgr*& configMgr);

  //ugly helper functions for all the reweighting
  int getHiggsinoDM(int DSID);
  bool isHiggsinoN2C1p(int DSID);
  bool isHiggsinoN2C1m(int DSID);
  bool isHiggsinoC1C1(int DSID);
  bool isHiggsinoN2N1(int DSID);

  // For Wino-Bino reweighting
  double getWinoBinoFuncMllDistr(double x, double *par);
  double getWinoBinoMllWeight(int DSID, double mass);
  double getWinoBinoXsecWeight(int DSID);
  double getWinoBinoBrFracWeight(int DSID);

  //For NUHM2 reweighting
  double get_NUHM2_weight(int DSID, double mass, int m12);
  double get_NUHM2_N2_mass(int m12);
  double get_NUHM2_N1_mass(int m12);

  std::vector<const char*> triggerChains;

  // emulates the 2017 triggers using L1Topo
  void emulateL1TopoTriggers(ConfigMgr*& configMgr);

};

static const BaseUser* HiggsinoSelector_instance __attribute__((used)) = new HiggsinoSelector();

#endif
