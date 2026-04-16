#pragma once
#include "Interfaces.hpp"
#include "System.hpp"
#include <iostream>

class NormalState : public IState {
public:
    const char* getName() override { return "NORMAL"; }
    void execute(SystemContext& ctx) override; // Se define en .cpp
};

class AlarmState : public IState {
public:
    const char* getName() override { return "ALARM"; }
    void execute(SystemContext& ctx) override;
};