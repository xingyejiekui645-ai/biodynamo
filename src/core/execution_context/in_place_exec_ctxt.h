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

#ifndef IN_PLACE_EXEC_CTXT_H_
#define IN_PLACE_EXEC_CTXT_H_

#include <vector>

#include "core/agent/agent_handle.h"
#include "core/environment/environment.h"
#include "core/execution_context/execution_context.h"
#include "core/param/param.h"

namespace bdm {

class Environment;
struct Param;

namespace in_place_exec_ctxt_detail {
class InPlaceExecutionContext_NeighborCacheValidity_Test;
}

class InPlaceExecutionContext : public ExecutionContext {
 public:
  InPlaceExecutionContext();
  virtual ~InPlaceExecutionContext();

  void SetupAgentOpsAll(const std::vector<ExecutionContext*>& all_exec_ctxts) override;
  void TearDownAgentOpsAll(const std::vector<ExecutionContext*>& all_exec_ctxts) override;

  void Execute(Agent* agent, AgentHandle ah,
               const std::vector<Operation*>& operations) override;

  void UpdateCachedAgent(Agent* new_agent, AgentHandle new_ah) override;

  void ClearNeighborCache();

  void SetNeighborCacheValidity(bool valid);

 private:
  /// Cache the value of Param::cache_neighbors
  bool cache_neighbors_ = false;

  /// Cached pointers refreshed once per agent-ops phase in SetupAgentOpsAll,
  /// so Execute avoids per-agent Simulation::GetActive() lookups.
  Environment* cached_env_ = nullptr;
  const Param* cached_param_ = nullptr;

  /// Check whether or not the neighbors in `neighbor_cache_` were queried with
  /// the same squared radius (`cached_squared_search_radius_`) as currently
  /// being queried with (`query_squared_radius_`)
  bool IsNeighborCacheValid() const;

  friend class in_place_exec_ctxt_detail::InPlaceExecutionContext_NeighborCacheValidity_Test;
};

}  // namespace bdm

#endif  // IN_PLACE_EXEC_CTXT_H_
