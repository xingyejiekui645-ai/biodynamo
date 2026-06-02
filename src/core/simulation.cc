#include "core/simulation/simulation.h"

#include <omp.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "core/agent/agent.h"
#include "core/environment/environment.h"
#include "core/param/param.h"
#include "core/scheduler/scheduler.h"
#include "core/ode_solver/ode_solver.h"

namespace bdm {

std::atomic<Simulation*> Simulation::active_simulation_{nullptr};
std::mutex Simulation::initialization_mutex_;

Simulation::Simulation()
    : param_(std::make_unique<Param>()),
      environment_(std::make_unique<Environment>()),
      scheduler_(std::make_unique<Scheduler>()),
      ode_solver_(std::make_unique<OdeSolver>()) {}

Simulation::~Simulation() {
  Finalize();
}

Simulation* Simulation::GetActive() {
  return active_simulation_.load(std::memory_order_acquire);
}

void Simulation::Initialize(int argc, const char** argv) {
  std::lock_guard<std::mutex> lock(initialization_mutex_);
  if (initialized_.load(std::memory_order_acquire)) {
    return;
  }

  // Set active simulation
  active_simulation_.store(this, std::memory_order_release);

  // Initialize components
  param_->Initialize(argc, argv);
  environment_->Initialize(param_.get());
  scheduler_->Initialize(param_.get());
  ode_solver_->Initialize(param_.get());

  initialized_.store(true, std::memory_order_release);
}

void Simulation::Finalize() {
  if (finalized_.load(std::memory_order_acquire)) {
    return;
  }

  // Clear active simulation if it's this one
  Simulation* expected = this;
  active_simulation_.compare_exchange_strong(expected, nullptr,
                                              std::memory_order_acq_rel);

  // Finalize components
  ode_solver_->Finalize();
  scheduler_->Finalize();
  environment_->Finalize();
  param_->Finalize();

  // Clear agents
  for (auto* agent : agents_) {
    delete agent;
  }
  agents_.clear();

  finalized_.store(true, std::memory_order_release);
}

void Simulation::Run() {
  if (!initialized_.load(std::memory_order_acquire)) {
    throw std::runtime_error("Simulation not initialized");
  }

  InitializeAgents();

  while (!scheduler_->IsFinished()) {
    Step();
  }
}

void Simulation::Step() {
  // Update environment (thread-safe via internal locks)
  UpdateEnvironment();

  // Run scheduler (parallel operations)
  RunScheduler();

  // Solve ODE for each agent (parallel)
  SolveOde();

  // Advance time
  scheduler_->AdvanceTime();
}

void Simulation::InitializeAgents() {
  // Agents are added externally; here we could set up initial conditions
}

void Simulation::UpdateEnvironment() {
  environment_->Update(agents_);
}

void Simulation::RunScheduler() {
  scheduler_->Run(agents_);
}

void Simulation::SolveOde() {
  const auto& agents = agents_;
  #pragma omp parallel for
  for (size_t i = 0; i < agents.size(); ++i) {
    Agent* agent = agents[i];
    if (agent->IsActive()) {
      ode_solver_->Solve(agent, param_->GetTimeStep());
    }
  }
}

void Simulation::AddAgent(Agent* agent) {
  #pragma omp critical
  {
    agents_.push_back(agent);
  }
}

void Simulation::RemoveAgent(Agent* agent) {
  #pragma omp critical
  {
    auto it = std::find(agents_.begin(), agents_.end(), agent);
    if (it != agents_.end()) {
      agents_.erase(it);
      delete agent;
    }
  }
}

}  // namespace bdm
