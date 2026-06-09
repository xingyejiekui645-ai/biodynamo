// ... existing code ...

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
  // ... rest of the function ...
}

// ... existing code ...