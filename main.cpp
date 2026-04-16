#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include "System.hpp"
#include "ConcreteStates.hpp"

// --- CAPA DE HARDWARE (HAL) ---
class MockSensor : public ISensor {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
public:
    MockSensor() : gen(std::random_device{}()), dis(20, 40) {}
    int readValue() override { return dis(gen); }
};

// --- CAPA DE COMUNICACIÓN (OBSERVER) ---
class SerialLogger : public IObserver {
public:
    void onUpdate(int data) override {
        std::cout << "[Logger] Dato del Sensor: " << data << "°C" << std::endl;
    }
};

// --- INSTANCIAS ESTÁTICAS (SEGURIDAD DE MEMORIA) ---
static NormalState estadoNormal;
static AlarmState estadoAlarma;

// --- LÓGICA DE ESTADOS (FSM) ---
void NormalState::execute(SystemContext& ctx) {
    int valor = ctx.readSensor(); // Lee del HAL
    ctx.notify(valor);           // Notifica a Observers

    if (valor > 32) {
        std::cout << ">> CAMBIO: Umbral crítico detectado." << std::endl;
        ctx.setState(&estadoAlarma);
    }
}

void AlarmState::execute(SystemContext& ctx) {
    int valor = ctx.readSensor(); 
    std::cout << "!!! MODO ALARMA ACTIVO !!!" << std::endl;
    ctx.notify(valor);

    if (valor <= 32) {
        std::cout << ">> CAMBIO: Sistema estabilizado." << std::endl;
        ctx.setState(&estadoNormal);
    }
}

// --- PUNTO DE ENTRADA ---
int main() {
    // 1. Creamos el hardware (HAL)
    MockSensor sensorSimulado;

    // 2. Iniciamos el sistema inyectando el sensor
    SystemContext machine(&estadoNormal, &sensorSimulado);
    
    // 3. Agregamos suscriptores
    SerialLogger logger;
    machine.addObserver(&logger);

    std::cout << "=== FIRMWARE INDUSTRIAL v1.0 INICIADO ===" << std::endl;

    for(int i = 0; i < 10; i++) {
        std::cout << "\nCiclo: " << i << " | Estado: " << machine.getStateName() << std::endl;
        machine.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }

    return 0;
}