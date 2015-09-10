#pragma once

#include "RemoteProcedures.h"
#include "Serialization.h"

std::vector<int> SVM_decide(const SampleSet& ss);
void SVM_train();
