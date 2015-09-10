#include "ClassificationSVM.h"

#include <iostream>
#include <vector>

#include "third_party/dlib-18.9/dlib/matrix.h"
#include "third_party/dlib-18.9/dlib/algs.h"
#include "third_party/dlib-18.9/dlib/svm.h"
#include "third_party/dlib-18.9/dlib/svm/svm.h"
#include "third_party/dlib-18.9/dlib/svm/multiclass_tools.h"
#include "third_party/dlib-18.9/dlib/svm/svm_multiclass_linear_trainer.h"

//#include "third_party/dlib-18.9/dlib/gui_widgets.h"
//#include "third_party/dlib-18.9/dlib/array2d.h"
//#include "third_party/dlib-18.9/dlib/image_transforms.h"

#include "GlobalConfig.h"
#include "ClassificationMeasures.hpp"

using namespace std;
using namespace dlib;


typedef matrix<double, 3, 1> sample_type;
typedef radial_basis_kernel<sample_type> kernel_type;
//typedef polynomial_kernel<sample_type> kernel_type;
typedef decision_function<kernel_type> dec_funct_type;
typedef normalized_function<dec_funct_type> funct_type;

sample_type toMatrix(Sample s) {
  sample_type m;
  m(0) = s.pq_gram;
  m(1) = s.words_per_child;
  m(2) = s.children_rect_min_variance;

  return m;
}

std::vector<int> SVM_decide(const SampleSet& ss) {
  std::vector<int> uids; //uids of classified lists

  dec_funct_type learned_function;
  deserialize("SVM_func.dat") >> learned_function;

  for (auto s : ss) {
    if (learned_function(toMatrix(s)) > 0) {
      cout << "uid " << s.uid << " pq " << s.pq_gram << " wpc " << s.words_per_child << " mvar " << s.children_rect_min_variance << endl;
      uids.push_back(s.uid);
    }
  }

  return uids;
}

dec_funct_type SVM_measure(svm_c_trainer<kernel_type>& trainer, int ss1_mod, int ss2_mod, SampleSet& ss1, SampleSet& ss2) {
  cout << "----new SVM training----" << endl;
  
  std::vector<sample_type> samples;
  std::vector<double> labels;
  int s1_count=0, s2_count=0;

  for (int i = 0; i < ss1.size(); ++i) {
    if (std::rand() % ss1_mod != 0)
      continue;
    samples.push_back(toMatrix(ss1[i]));
    labels.push_back(+1);
    s1_count++;
  }

  for (int i = 0; i < ss2.size(); ++i) {
    if (std::rand() % ss2_mod == 0) {
      samples.push_back(toMatrix(ss2[i]));
      labels.push_back(-1);
      s2_count++;
    }
  }

  auto decision_fun = trainer.train(samples, labels);

  // Measure
  samples.clear();
  labels.clear();
  int s1_all_count = 0, s2_all_count = 0;

  for (int i = 0; i < ss1.size(); ++i) {
    samples.push_back(toMatrix(ss1[i]));
    labels.push_back(+1);
    s1_all_count++;
  }

  for (int i = 0; i < ss2.size(); ++i) {
    samples.push_back(toMatrix(ss2[i]));
    labels.push_back(-1);
    s2_all_count++;
  }

  cout << "Train " << s1_count << " + " << s2_count << " measures for " << s1_all_count << " + " << s2_all_count << endl;
  randomize_samples(samples, labels);
  cout << "Measure for " << samples.size() << " samples" << endl;
  measure_for_dataset(decision_fun, samples, labels);

  return decision_fun;
}

void SVM_train()
{
  std::vector<sample_type> samples;
  std::vector<double> labels;

  // Get from cache
  SampleSet ss1;
  LoadSampleSet(GlobalConfig::Get().cacheListsFeatures, ss1);
  for (int i=0; i < ss1.size(); ++i) {
    samples.push_back(toMatrix(ss1[i]));
    labels.push_back(+1);
  }
  
  SampleSet ss2;
  LoadSampleSet(GlobalConfig::Get().cacheRandomFeatures, ss2);
  std::srand(12345);
  for (int i = 0; i < ss2.size(); ++i) {
    if (std::rand() % 20 == 0) {
      // 1:20 of 5200 = 260
      samples.push_back(toMatrix(ss2[i]));
      labels.push_back(-1);
    }
  }

  randomize_samples(samples, labels);
  svm_c_trainer<kernel_type> trainer;

  cout << "doing cross validation" << endl;

  double best_gamma10=0, best_C10=0, best_gamma2=0, best_C2=0;
  ComputedMeasures best_cm10, best_cm2;

  matrix<double> params = cartesian_product( 
    logspace(log10(1e-6), log10(100), 10),  // gamma parameter
    logspace(log10(100000), log10(1e-3), 10) // SVM C parameter
  );

  for (long col = 0; col < params.nc(); ++col)
  {
      const double gamma = params(0, col);
      const double C = params(1, col);

      trainer.set_kernel(kernel_type(gamma));
      trainer.set_c(C);

      cout << "gamma: " << gamma << "  C: " << C <<  endl;

      // 2-fold and 10-fold
      auto cm2 = cross_validation(trainer, samples, labels, 2);
      auto cm10 = cross_validation(trainer, samples, labels, 10);

      if (cm2 > best_cm2) {
        best_cm2 = cm2;
        best_gamma2 = gamma;
        best_C2 = C;
      }
      if (cm10 > best_cm10) {
        best_cm10 = cm10;
        best_gamma10 = gamma;
        best_C10 = C;
      }
  }
  
  std::printf("-----\n\nBest measures 2 groups :\n");
  best_cm2.print();
  std::printf("For gamma %lf C %lf\n", best_gamma2, best_C2);

  std::printf("Best measures 10 groups :\n");
  best_cm10.print();
  std::printf("For gamma %lf C %lf\n", best_gamma10, best_C10);

  std::printf("------- end k-fold ------\n");



  trainer.set_kernel(kernel_type(best_gamma10));
  trainer.set_c(best_C10);


  SVM_measure(trainer, 2, 3, ss1, ss2);

  auto best_decision_fun = SVM_measure(trainer, 2, 41, ss1, ss2);
  serialize("SVM_func.dat") << best_decision_fun;

  SVM_measure(trainer, 2, 100000, ss1, ss2);
}
