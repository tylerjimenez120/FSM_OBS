# Advanced Embedded FSM & Observer Pattern

Arquitectura profesional para sistemas embebidos utilizando patrones de diseño avanzados, diseñada para ser eficiente en memoria y totalmente desacoplada del hardware.

## 🚀 Características
- **State Pattern (FSM):** Lógica de control encapsulada en clases, eliminando el uso de `switch-case` engorrosos.
- **Observer Pattern:** Sistema de notificaciones orientado a eventos para múltiples suscriptores (Loggers, Actuadores).
- **Filtrado por Tópicos:** Los observadores solo reciben datos de su interés (ej. Telemetría vs. Alarmas).
- **HAL (Hardware Abstraction Layer):** Interfaz de sensores para intercambiar hardware real por simulado (Mocking) sin cambiar la lógica de negocio.
- **Static Memory:** Uso de instancias estáticas para evitar el uso de memoria dinámica (Heap) y garantizar cero fragmentación.

## 🔄 Flujo del Sistema

El sistema sigue un ciclo de ejecución determinista basado en el "latido" del microcontrolador:

1.  **Inicialización (`main`)**: Se instancian el `MockSensor` y el `SystemContext`. Se registran los observadores (`TelemetryLogger` y `EmergencyBuzzer`).
2.  **Ciclo Principal (`Loop`)**: Entrada al bucle infinito de control.
3.  **Ejecución (`run`)**: El contexto transfiere el control a la lógica del estado actual.
4.  **Lectura de HAL (`readSensor`)**: El estado solicita datos al hardware a través del contexto.
5.  **Lógica de Transición (`execute`)**: Se evalúa el valor. Si cruza el umbral (> 32°C), se dispara un `setState`.
6.  **Notificación Selectiva (`notify`)**: El estado publica el evento etiquetado con un `Topic` específico.
7.  **Filtrado Multicanal**: 
    - El dato se envía **siempre** al tópico `SENSOR_DATA` para registro histórico (Telemetría).
    - Si existe una condición crítica activa, se envía **adicionalmente** al tópico `ALARM_EVENT` para activar actuadores (Buzzer).

## 🛠️ Estructura del Proyecto
- `Interfaces.hpp`: Definición de contratos (interfaces puras) para `IState`, `IObserver` e `ISensor`.
- `System.hpp`: El "Contexto" que mantiene el estado actual y la lista de suscriptores.
- `ConcreteStates.hpp`: Implementación de los estados `NormalState` y `AlarmState`.
- `main.cpp`: Punto de entrada, configuración de inyección de dependencias y simulación del sensor.

## 🔧 Compilación y Uso
Para probar el sistema en un entorno local:

```bash
g++ -std=c++17 main.cpp -o fsm_system
./fsm_system


=== FIRMWARE INDUSTRIAL v1.0 ===
[NORMAL] Telemetría: 34°C
>> [SISTEMA] Umbral crítico detectado (34). Cambiando a ALARMA...
!!! [BUZZER] BEEP BEEP! Alerta Crítica: 34 !!!

[ALARM] Telemetría: 20°C
>> [SISTEMA] Temperatura normalizada. Volviendo a NORMAL...



Diseñado para entornos de alta disponibilidad (ESP32/STM32).

AUTOR: TYLER JMZ





