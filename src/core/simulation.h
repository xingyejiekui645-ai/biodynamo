#ifndef BDM_SIMULATION_H_
#define BDM_SIMULATION_H_

#include <memory>
#include <vector>
#include <atomic>
#include <string>
#include <unordered_map>

#include "core/agent/agent.h"
#include "core/environment/environment.h"
#include "core/param/param.h"
#include "core/scheduler/scheduler.h"
#include "core/ode_solver/ode_solver.h"

namespace bdm {

class Simulation {
 public:
  Simulation();
  ~Simulation();

  // No copy or move
  Simulation(const Simulation&) = delete;
  Simulation& operator=(const Simulation&) = delete;
  Simulation(Simulation&&) = delete;
  Simulation& operator=(Simulation&&) = delete;

  static Simulation* GetActive();

  void Initialize(int argc, const char** argv);
  void Finalize();
  void Run();
  void Step();

  // Accessors
  Param* GetParam() { return param_.get(); }
  Environment* GetEnvironment() { return environment_.get(); }
  Scheduler* GetScheduler() { return scheduler_.get(); }
  OdeSolver* GetOdeSolver() { return ode_solver_.get(); }
  const std::vector<Agent*>& GetAgents() const { return agents_; }

  // Agent management
  void AddAgent(Agent* agent);
  void RemoveAgent(Agent* agent);

 private:
  void InitializeAgents();
  void UpdateEnvironment();
  void RunScheduler();
  void SolveOde();

  static std::atomic<Simulation*> active_simulation_;
  static std::mutex initialization_mutex_;

  std::unique_ptr<Param> param_;
  std::unique_ptr<Environment> environment_;
  std::unique_ptr<Scheduler> scheduler_;
  std::unique_ptr<OdeSolver> ode_solver_;
  std::vector<Agent*> agents_;
  std::atomic<bool> initialized_{false};
  std::atomic<bool> finalized_{false};
};

}  // namespace bdm

#endif  // BDM_SIMULATION_H_
