# Advanced Embedded FSM & Observer Pattern

Arquitectura profesional para sistemas embebidos utilizando patrones de diseño avanzados, diseñada para ser eficiente en memoria y totalmente desacoplada del hardware.

## 🚀 Características
- **State Pattern (FSM):** Lógica de control encapsulada en clases, eliminando el uso de `switch-case`.
- **Observer Pattern:** Sistema de notificaciones para múltiples suscriptores (Loggers, Actuadores).
- **HAL (Hardware Abstraction Layer):** Interfaz de sensores para intercambiar hardware real por simulado sin cambiar la lógica.
- **Static Memory:** Uso de instancias estáticas para evitar el Heap y garantizar cero fugas de memoria.

## 🛠️ Estructura
- `Interfaces.hpp`: Contratos de interfaces (IState, IObserver, ISensor).
- `System.hpp`: Contexto del sistema y orquestador.
- `ConcreteStates.hpp`: Definición de estados (Normal, Alarma).
- `main.cpp`: Implementación HAL, Loop principal e Inyección de Dependencias.

## 🔧 Compilación
```bash
g++ -std=c++17 main.cpp -o fsm_system
./fsm_system




Diseñado para entornos de alta disponibilidad (ESP32/STM32).