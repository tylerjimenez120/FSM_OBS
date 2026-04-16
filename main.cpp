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


// --- OBSERVADORES ACTUALIZADOS ---
class TelemetryLogger : public IObserver {
public:
    Topic getInterest() override { return Topic::SENSOR_DATA; }
    void onUpdate(int data, Topic topic) override {
        std::cout << "[LOG] Sensor: " << data << "°C (Tópico: Telemetría)" << std::endl;
    }
};

class EmergencyBuzzer : public IObserver {
public:
    Topic getInterest() override { return Topic::ALARM_EVENT; }
    void onUpdate(int data, Topic topic) override {
        std::cout << "!!! [BUZZER] BEEP BEEP! Umbral Crítico: " << data << " !!!" << std::endl;
    }
};

// --- INSTANCIAS ESTÁTICAS (SEGURIDAD DE MEMORIA) ---
static NormalState estadoNormal;
static AlarmState estadoAlarma;

// --- LÓGICA DE ESTADOS (FSM) ---
void NormalState::execute(SystemContext& ctx) {
    int valor = ctx.readSensor(); 

    // 1. SIEMPRE notificamos telemetría (el dato debe registrarse sea cual sea)
    ctx.notify(valor, Topic::SENSOR_DATA);

    // 2. Evaluamos si ADEMÁS es una emergencia
    if (valor > 32) {
        std::cout << ">> [SISTEMA] Umbral crítico detectado (" << valor << "). Cambiando a ALARMA..." << std::endl;
        ctx.setState(&estadoAlarma);
        
        // 3. Notificación EXTRA para el canal de pánico
        ctx.notify(valor, Topic::ALARM_EVENT); 
    }
}

void AlarmState::execute(SystemContext& ctx) {
    int valor = ctx.readSensor(); 

    // Telemetría siempre activa
    ctx.notify(valor, Topic::SENSOR_DATA);

    if (valor > 32) {
        // Sigue el peligro: notificamos al canal de pánico
        ctx.notify(valor, Topic::ALARM_EVENT); 
    } else {
        // Bajó la temperatura: no enviamos ALARM_EVENT, solo cambiamos estado
        std::cout << ">> [SISTEMA] Temperatura normalizada. Volviendo a NORMAL..." << std::endl;
        ctx.setState(&estadoNormal);
    }
}

// --- PUNTO DE ENTRADA ---
int main() {
    MockSensor sensor;
    SystemContext machine(&estadoNormal, &sensor);
    
    TelemetryLogger logger;
    EmergencyBuzzer buzzer;
    
    machine.addObserver(&logger);
    machine.addObserver(&buzzer);

    std::cout << "=== FIRMWARE V4.0 (TOPICS) INICIADO ===" << std::endl;

    for(int i = 0; i < 10; i++) {
        std::cout << "\nCiclo: " << i << " [" << machine.getStateName() << "]" << std::endl;
        machine.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    return 0;
}