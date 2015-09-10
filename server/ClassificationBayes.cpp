#include "ClassificationBayes.h"

#include <algorithm>
#include <limits>
#include <iostream>
#include <cstdio>

#define M_PI 3.14159265359

#include "GlobalConfig.h"
#include "ClassificationMeasures.hpp"

using namespace std;
#undef max
#undef min

typedef vector<double> sample_type;
sample_type toBayesSample(Sample s) {
  sample_type out;
  out.push_back(s.pq_gram);
  out.push_back(s.words_per_child);
  out.push_back(s.children_rect_min_variance);

  return out;
}


class BayesClassifier {
public:
  BayesClassifier& train(vector<vector<double>>& samples, vector<int>& labels) {

    for (int i = 0; i < samples.size(); ++i) {
      vector<double>& values = samples[i];
      int label = labels[i];
      for (unsigned int i = 0; i < values.size(); i++) {
        if (sum_x.find(label) == sum_x.end()) {
          vector<double> empty;
          empty.resize(values.size(), 0.0);
          sum_x[label] = empty;
        }
        sum_x[label][i] += values[i];
        multinomial_sums[label] += values[i];
      }

      // DCHECK (values.size() == sum_x[label].size()) 

      data[label].push_back(values);
      n[label]++;
      n_total++;
    }

    for (auto it = sum_x.begin(); it != sum_x.end(); it++){

      priors[it->first] = (double)n[it->first] / n_total;

      // Calculate means
      vector<double> feature_means;
      for (unsigned int i = 0; i < it->second.size(); i++){
        feature_means.push_back(sum_x[it->first][i] / n[it->first]);
      }

      // Calculate variances
      vector<double> feature_variances(feature_means.size());
      for (unsigned int i = 0; i < data[it->first].size(); i++){
        for (unsigned int j = 0; j < data[it->first][i].size(); j++){
          feature_variances[j] += (data[it->first][i][j] - feature_means[j])*(data[it->first][i][j] - feature_means[j]);
        }
      }
      for (unsigned int i = 0; i < feature_variances.size(); i++){
        feature_variances[i] /= data[it->first].size();
      }

      // Calculate multinomial likelihoods
      for (unsigned int i = 0; i < feature_means.size(); i++){
        double mnl = (sum_x[it->first][i] + alpha) / (multinomial_sums[it->first] + (alpha*feature_means.size()));
        multinomial_likelihoods[it->first].push_back(mnl);
      }

      means[it->first] = feature_means;
      variances[it->first] = feature_variances;
    }

    // For dlib compatibility;
    return *this;
  }

  // Classify
  int operator()(vector<double>& sample) {
      vector<double>& values = sample;
      //int label = testing_sample.second;

      int predlabel = 0;
      double maxlikelihood = 0.0;
      double denom = 0.0;
      vector<double> probs;
      for (auto it = priors.begin(); it != priors.end(); it++){
        double numer = priors[it->first];
        for (unsigned int j = 0; j < values.size(); j++){
          switch (decision){
          case 2:
            // Multinomial      
            if (values[j]){
              numer *= pow(multinomial_likelihoods[it->first][j], values[j]);
            }
            break;
          case 3:
            // Bernoulli
            numer *= (pow(means[it->first][j], values[j]) * pow((1.0 - means[it->first][j]), (1.0 - values[j])));
            break;
          default:
            // Gaussian
            numer *= (1 / sqrt(2 * M_PI*variances[it->first][j])*exp((-1 * (values[j] - means[it->first][j])*(values[j] - means[it->first][j])) / (2 * variances[it->first][j])));
            break;
          }
        }
        if (numer > maxlikelihood){
          maxlikelihood = numer;
          predlabel = it->first;
        }
        denom += numer;
        probs.push_back(numer);
      }

      return predlabel;
  }

private:
  // Smoothing factor
  const double alpha = 1.0;
  // Decision rule
  const int decision = 1; /* gaussian, multinomial, bernoulli */ /* Only gaussian used and well tested */
  // Verbose
  const int verbose = 0;

  unsigned int n_total = 0;
  map<int, vector<vector<double> > > data;
  map<int, int> n;
  map<int, double> priors;
  map<int, vector<double> > multinomial_likelihoods;
  map<int, double/*int*/> multinomial_sums;
  map<int, vector<double> > sum_x;
  map<int, vector<double> > means;
  map<int, vector<double> > variances;
};


void bayes_measure(int ss1_mod, int ss2_mod, SampleSet& ss1, SampleSet& ss2) {
  cout << "----new Bayes training----" << endl;

  vector<vector<double>> samples;
  std::vector<int> labels;
  int s1_count = 0, s2_count = 0;

  for (int i = 0; i < ss1.size(); ++i) {
    if (std::rand() % ss1_mod != 0)
      continue;
    samples.push_back(toBayesSample(ss1[i]));
    labels.push_back(+1);
    s1_count++;
  }

  for (int i = 0; i < ss2.size(); ++i) {
    if (std::rand() % ss2_mod == 0) {
      samples.push_back(toBayesSample(ss2[i]));
      labels.push_back(-1);
      s2_count++;
    }
  }

  BayesClassifier c;
  auto decision_fun = c.train(samples, labels);

  // Measure
  samples.clear();
  labels.clear();
  int s1_all_count = 0, s2_all_count = 0;

  for (int i = 0; i < ss1.size(); ++i) {
    samples.push_back(toBayesSample(ss1[i]));
    labels.push_back(+1);
    s1_all_count++;
  }

  for (int i = 0; i < ss2.size(); ++i) {
    samples.push_back(toBayesSample(ss2[i]));
    labels.push_back(-1);
    s2_all_count++;
  }

  cout << "Train " << s1_count << " + " << s2_count << " measures for " << s1_all_count << " + " << s2_all_count << endl;
  cout << "Measure for " << samples.size() << " samples" << endl;
  measure_for_dataset(decision_fun, samples, labels);
}

BayesClassifier bayes_train_internal() {

  vector<vector<double>> samples, testing_samples;
  vector<int> labels, testing_labels;

  SampleSet ss1;
  LoadSampleSet(GlobalConfig::Get().cacheListsFeatures, ss1);
  for (int i = 0; i < ss1.size(); ++i) {
    samples.push_back(toBayesSample(ss1[i]));
    labels.push_back(+1);
  }
  
  SampleSet ss2;
  LoadSampleSet(GlobalConfig::Get().cacheRandomFeatures, ss2);
  for (int i = 0; i < ss2.size(); ++i) {
    if (std::rand() % 20 == 0) {
      // 260 samples of 5200
      samples.push_back(toBayesSample(ss2[i]));
      labels.push_back(-1);
    }
  }

  BayesClassifier c;

  cross_validation(c, samples, labels, 2);
  cout << "-------" << endl;
  cross_validation(c, samples, labels, 10);


  bayes_measure(2, 3, ss1, ss2);
  bayes_measure(2, 41, ss1, ss2);
  bayes_measure(2, 100000, ss1, ss2);

  return c;
}

void Bayes_train()  {
  bayes_train_internal();
}

std::vector<int> Bayes_decide(const SampleSet& ss) {
  std::vector<int> uids; // uids of classified lists
  static BayesClassifier classifiy = bayes_train_internal();

  for (auto s : ss) {
    if (classifiy(toBayesSample(s)) > 0) {
      cout << "uid " << s.uid << " pq " << s.pq_gram << " wpc " << s.words_per_child << " mvar " << s.children_rect_min_variance << endl;
      uids.push_back(s.uid);
    }
  }

  return uids;
}
