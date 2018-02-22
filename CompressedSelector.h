#ifndef SusySkimHiggsino_CompressedSelector_H
#define SusySkimHiggsino_CompressedSelector_H

//RootCore
#include "SusySkimMaker/BaseUser.h"

class CompressedSelector : public BaseUser
{

 public:
  CompressedSelector();
  ~CompressedSelector() {};

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

  std::vector<const char*> triggerChains;

};

static const BaseUser* CompressedSelector_instance __attribute__((used)) = new CompressedSelector();

#endif
