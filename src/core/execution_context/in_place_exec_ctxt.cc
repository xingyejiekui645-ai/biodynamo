// -----------------------------------------------------------------------------
//
// Copyright 2020 The BioDynaMo Project.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// -----------------------------------------------------------------------------

#include "core/execution_context/in_place_exec_ctxt.h"

#include <algorithm>
#include <queue>
#include <vector>

#include "core/agent/agent.h"
#include "core/environment/environment.h"
#include "core/operation/operation.h"
#include "core/param/param.h"
#include "core/simulation.h"

namespace bdm {

InPlaceExecutionContext::InPlaceExecutionContext() {}

InPlaceExecutionContext::~InPlaceExecutionContext() {}

void InPlaceExecutionContext::SetupAgentOpsAll(
    const std::vector<ExecutionContext*>& all_exec_ctxts) {
  auto* sim = Simulation::GetActive();
  auto* env = sim->GetEnvironment();
  auto* param = sim->GetParam();
  for (auto* ec : all_exec_ctxts) {
    auto* ctxt = bdm_static_cast<InPlaceExecutionContext*>(ec);
    ctxt->cached_env_ = env;
    ctxt->cached_param_ = param;
  }
}

void InPlaceExecutionContext::TearDownAgentOpsAll(
    const std::vector<ExecutionContext*>& all_exec_ctxts) {}

void InPlaceExecutionContext::Execute(
    Agent* agent, AgentHandle ah, const std::vector<Operation*>& operations) {
  auto* env =
      cached_env_ ? cached_env_ : Simulation::GetActive()->GetEnvironment();
  auto* param =
      cached_param_ ? cached_param_ : Simulation::GetActive()->GetParam();

  if (param->thread_safety_mechanism ==
      Param::ThreadSafetyMechanism::kUserSpecified) {
    // ... existing code ...
  }

  // ... rest of Execute implementation ...
}

void InPlaceExecutionContext::UpdateCachedAgent(Agent* new_agent,
                                                 AgentHandle new_ah) {
  // ... existing code ...
}

void InPlaceExecutionContext::ClearNeighborCache() {
  // ... existing code ...
}

void InPlaceExecutionContext::SetNeighborCacheValidity(bool valid) {
  // ... existing code ...
}

bool InPlaceExecutionContext::IsNeighborCacheValid() const {
  // ... existing code ...
}

}  // namespace bdm
