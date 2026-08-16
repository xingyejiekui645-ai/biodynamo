// -----------------------------------------------------------------------------
//
// Copyright 2021-2024 The BioDynaMo Project.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// -----------------------------------------------------------------------------

#include "core/execution_context/in_place_exec_ctxt.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <string>
#include <vector>
#include "core/agent/agent.h"
#include "core/agent/agent_uid.h"
#include "core/agent/cell.h"
#include "core/environment/environment.h"
#include "core/execution_context/execution_context.h"
#include "core/param/param.h"
#include "core/resource_manager.h"
#include "core/scheduler.h"
#include "core/simulation.h"
#include "core/util/numa.h"
#include "core/util/thread_info.h"

namespace bdm {

InPlaceExecutionContext::InPlaceExecutionContext()
    : ExecutionContext(),
      tinfo_(ThreadInfo::GetInstance()),
      rm_(nullptr),
      env_(nullptr),
      scheduler_(nullptr),
      param_(nullptr),
      random_(nullptr),
      new_agents_(),
      agents_to_remove_(),
      removed_agents_(),
      diff_(),
      num_new_agents_(0),
      num_removed_agents_(0),
      num_modified_agents_(0) {}

InPlaceExecutionContext::~InPlaceExecutionContext() {}

void InPlaceExecutionContext::Setup(const std::vector<AgentHandle>& agents) {
  // Implementation
}

void InPlaceExecutionContext::TearDown() {
  // Implementation
}

void InPlaceExecutionContext::Execute() {
  // Implementation
}

void InPlaceExecutionContext::AddAgent(Agent* agent) {
  new_agents_.push_back(agent);
}

void InPlaceExecutionContext::RemoveAgent(const AgentUid& uid) {
  agents_to_remove_.push_back(uid);
}

void InPlaceExecutionContext::AddAgentsToRm(
    const std::vector<ExecutionContext*>& all_exec_ctxts) {
  // Skip entirely when no execution context has pending agents
  bool any_new = false;
  for (int i = 0; i < tinfo_->GetMaxThreads(); ++i) {
    auto* ctxt = bdm_static_cast<InPlaceExecutionContext*>(all_exec_ctxts[i]);
    if (!ctxt->new_agents_.empty()) {
      any_new = true;
      break;
    }
  }
  if (!any_new) {
    return;
  }

  // group execution contexts by numa domain
  std::vector<uint64_t> new_agent_per_numa(tinfo_->GetNumaNodes());
  std::vector<uint64_t> thread_offsets(tinfo_->GetMaxThreads());
  // Implementation continues...
}

}  // namespace bdm
