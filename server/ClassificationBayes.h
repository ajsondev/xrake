#pragma once

#include "RemoteProcedures.h"
#include "Serialization.h"

#include <vector>

class BayesClassifier;

std::vector<int> Bayes_decide(const SampleSet& ss);
void Bayes_train();
