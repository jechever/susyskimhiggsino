#ifndef SusySkimHiggsino_CutflowHiggsinos_h
#define SusySkimHiggsino_CutflowHiggsinos_h

//RootCore
#include "SusySkimMaker/BaseUser.h"

class CutflowHiggsinos : public BaseUser
{

 public:
  CutflowHiggsinos();
  ~CutflowHiggsinos() {};

  void setup(ConfigMgr*& configMgr);
  bool doAnalysis(ConfigMgr*& configMgr);
  bool passCuts(ConfigMgr*& configMgr);
  void finalize(ConfigMgr*& configMgr);


};

static const BaseUser* CutflowHiggsinos_instance __attribute__((used)) = new CutflowHiggsinos();

#endif
