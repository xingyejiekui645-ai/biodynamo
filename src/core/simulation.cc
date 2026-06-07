#include "simulation.h"
#include <omp.h>
#include <memory>

class SimulationImpl : public Simulation {
public:
    SimulationImpl() : state_(std::make_unique<SimState>()) {}
    
    void Run() override {
        #pragma omp parallel
        {
            #pragma omp single
            {
                while (!state_->IsFinished()) {
                    #pragma omp taskgroup
                    {
                        for (auto& agent : agents_) {
                            #pragma omp task firstprivate(agent)
                            {
                                agent->Update();
                            }
                        }
                    }
                    #pragma omp critical
                    {
                        state_->Advance();
                    }
                }
            }
        }
    }
    
    void AddAgent(std::unique_ptr<Agent> agent) override {
        agents_.push_back(std::move(agent));
    }
    
private:
    std::unique_ptr<SimState> state_;
    std::vector<std::unique_ptr<Agent>> agents_;
};

std::unique_ptr<Simulation> CreateSimulation() {
    return std::make_unique<SimulationImpl>();
}