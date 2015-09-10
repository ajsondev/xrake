#pragma once

#include <algorithm>    // std::random_shuffle
#include <cstdlib>      // std::rand, std::srand

struct ComputedMeasures {
  double dokladnosc = 0;
  double czulosc = 0;
  double precyzja = 0;
  double Fb = 0;

  void print() {
    cout << "Dokladnosc: " << dokladnosc << " czulosc: " << czulosc << " precyzja: " << precyzja << " Fb: " << Fb << endl;
  }

  bool operator>(const ComputedMeasures& other) const {
    return Fb > other.Fb;
  }

  void add(ComputedMeasures& cm) {
    dokladnosc += cm.dokladnosc;
    czulosc += cm.czulosc;
    precyzja += cm.precyzja;
    Fb += cm.Fb;
    add_count++;
  }

  void normalize() {
    if (!add_count)
      return;
    dokladnosc /= add_count;
    czulosc /= add_count;
    precyzja /= add_count;
    Fb /= add_count;
  }

private:
  unsigned int add_count = 0;
};

struct ResultCounters {
  int p = 0;
  int n = 0;
  int tp = 0;
  int tn = 0;
  int fp = 0;

  //int fn=0;

  void add(int recognized_class, int true_class) {
    if (true_class == 1) {
      p++;
      if (recognized_class == 1) {
        tp++;
      }
      else {
        ; // fn++;
      }
    }
    if (true_class == -1) {
      n++;
      if (recognized_class == -1) {
        tn++;
      }
      else {
        fp++;
      }
    }
  }

  ComputedMeasures measures() {
    ComputedMeasures cm;
    double b = 2;

    //std::printf("(p %i n %i tp %i tn %i fp %i)\n", p, n, tp, tn, fp);

    cm.dokladnosc = (tp + tn) / (double)(p + n);
    cm.czulosc = (tp) / (double)(p);
    cm.precyzja = (tp) / (double)(tp + fp);
    if (tp == 0 && fp == 0)
      cm.precyzja = 1;
    cm.Fb = ((1 + b*b)*cm.precyzja*cm.czulosc) / (b*b*cm.precyzja + cm.czulosc);

    return cm;
  }

};

namespace {
  int random_fun(int i) { return std::rand() % i; }
}  // namespace

template<typename classifier_type, typename sample_type, typename label_type>
ComputedMeasures cross_validation(classifier_type& classifier, std::vector<sample_type>& samples, std::vector<label_type>& labels, unsigned int groups) {
  std::vector<unsigned int> indices;
  indices.reserve(samples.size());
  for (int i = 0; i < samples.size(); ++i)
    indices.push_back(i);
  std::random_shuffle(indices.begin(), indices.end(), random_fun);

  unsigned int step = (static_cast<unsigned int>(indices.size())) / groups;
  ComputedMeasures cm;
  if (!step)
    return cm;

  for (int i = 0; i < groups; ++i) {
    // Train.
    std::vector<sample_type> training_samples;
    std::vector<label_type> training_labels;
    for (int k = i*step; k < (i + 1)*step; ++k) {
      training_samples.push_back(samples[indices[k]]);
      training_labels.push_back(labels[indices[k]]);
    }
    auto decision_fun = classifier.train(training_samples, training_labels);

    // Evaluate.
    for (int j = 0; j < groups; ++j) {
      if (i == j)
        continue;

        ResultCounters rc;
        for (int k = j*step; k < (j + 1)*step; ++k) {
          //cout << indices[k] << " in group " << j << " trained on " << i << endl;
          auto& sample = samples[indices[k]];
          auto label = labels[indices[k]];
          auto predicted = decision_fun(sample);
          // Normalize for SVM.
          predicted = predicted > 0 ? 1 : -1;
          rc.add(predicted, label);
        }

        cm.add(rc.measures());
    }
  }
  cm.normalize();
  cout << "Cross validation, " << groups << " groups, total " << samples.size() << " samples, metrics:" << endl;
  cm.print();
  return cm;
}



template<typename decision_fun_type, typename sample_type, typename label_type>
ComputedMeasures measure_for_dataset(decision_fun_type& decision_fun, std::vector<sample_type>& samples, std::vector<label_type>& labels) {
  ResultCounters rc;

  // Evaluate.
  for (int i = 0; i < samples.size(); ++i) {
      auto& sample = samples[i];
      auto label = labels[i];
      auto predicted = decision_fun(sample);
      // Normalize for SVM.
      predicted = predicted > 0 ? 1 : -1;
      rc.add(predicted, label);
  }

  rc.measures().print();
  return rc.measures();
}