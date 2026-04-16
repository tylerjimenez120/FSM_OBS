#pragma once
#include "Interfaces.hpp"
#include <vector>
#include <algorithm>

class SystemContext {
private:
    IState* currentState;
    ISensor* sensor; // El sistema ahora posee un sensor
    std::vector<IObserver*> observers;

public:
    // El constructor recibe el estado inicial y el sensor a usar
    SystemContext(IState* initial, ISensor* s) : currentState(initial), sensor(s) {}

    void setState(IState* newState) { currentState = newState; }
    void addObserver(IObserver* obs) { observers.push_back(obs); }

    void notify(int value, Topic topic) {
        for (auto* obs : observers) {
        // Solo notificamos si el observador está interesado en este tópico
            if (obs->getInterest() == topic) {
                obs->onUpdate(value,topic);
            }
        }
    }

    // Método para que los estados obtengan datos del hardware
    int readSensor() { return sensor->readValue(); }

    //porque el uso de *this --> Es la forma en que el Sistema le dice al Estado: "Aquí me tienes, úsame para controlarme".
    void run() { currentState->execute(*this); }
    const char* getStateName() { return currentState->getName(); }
};

