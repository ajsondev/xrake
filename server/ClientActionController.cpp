#include "ClientActionController.h"

#include <vector>
#include <sstream>

#include "ClassificationBayes.h"
#include "ClassificationSVM.h"
#include "Serialization.h"

#include "third_party/chromium/base/time/time.h"  //TODO: maybe get rid of chromium?
using base::TimeTicks;
using base::TimeDelta;
using namespace std;

vector<string> split(string str, string separator = " ") {
  stringstream ss(str);
  istream_iterator<string> begin(ss);
  istream_iterator<string> end;
  vector<std::string> result(begin, end);
  copy(result.begin(), result.end(), ostream_iterator<string>(cout, separator.c_str()));
  return result;
}

typedef std::vector<int> (*classification_func)(const SampleSet& ss);

void classify(RPCMachine* rpc, int pageId, classification_func decide) {
  SampleSet ss;
  LoadSampleSetFromJson(rpc->GetFeaturesJumbo(pageId), ss);

  auto uids = decide(ss);
  for (auto uid : uids) {
    rpc->MarkNode(uid);
    cout << "MarkNode " << uid << endl;
  }
}


void ClientActionController::RunAction(string& action) {
  TimeTicks time = TimeTicks::HighResNow();
  auto action_vector = split(action);
  string& name = action_vector[0];

  if (name == "InitializeTraining")
    trainingManager.StartSupervising();
  else if (name == "TestClassificationOnPageSVM")
    classify(&rpc, stoi(action_vector.at(1)), SVM_decide);
  else if (name == "TestClassificationOnPageBayes")
    classify(&rpc, stoi(action_vector.at(1)), Bayes_decide);
  else if (name == "PresentTrainingMarks")
    trainingManager.PresentTrainingMarks();
  else if (name == "TestTrainingMarksRecall")
    trainingManager.TestTrainingMarksRecall();
  else if (name == "VisualizeTrainingFeatures")
    trainingManager.VisualizeTrainingFeatures();
  else if (name == "TrainSVMClassifier")
    SVM_train();
  else if (name == "TrainBayesClassifier")
    Bayes_train();
  
  TimeDelta delta = TimeTicks::HighResNow() - time;
  cout << "Time: " << delta.InMilliseconds() << " ms" << endl;
}
