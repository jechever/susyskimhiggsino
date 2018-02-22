#ifndef SusySkimHiggsino_EWK3L_Opt_H
#define SusySkimHiggsino_EWK3L_Opt_H

#include "SusySkimMaker/BaseUser.h"

class EWK3L_Opt : public BaseUser
{

 public:
  EWK3L_Opt();
  ~EWK3L_Opt() {};

  void setup(ConfigMgr*& configMgr);
  bool doAnalysis(ConfigMgr*& configMgr);
  void fillWeights(ConfigMgr*& configMgr);
  bool passCuts(ConfigMgr*& configMgr);
  void fillListOfTriggersPassed(ConfigMgr*& configMgr);
  bool Simple_Selection(ConfigMgr*& configMgr);
  void finalize(ConfigMgr*& configMgr);

 protected:
  TH1F* m_cutFlow = 0;
  double m_weight = 1;

  std::vector<TString> m_listOfTriggersPassed = {};

};

static const BaseUser* EWK3L_Opt_instance __attribute__((used)) = new EWK3L_Opt();

#endif
