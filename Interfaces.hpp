#pragma once

// Interfaz Estado
class SystemContext; // Forward declaration

enum class Topic {
    SENSOR_DATA,
    SYSTEM_STATUS,
    ALARM_EVENT
};


// Interfaz para el Sensor (HAL)
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual int readValue() = 0; // Método que todo sensor debe tener
};


// Interfaz Observer
class IObserver {
public:
    //el compilador genera el codigode limpieza.
    virtual ~IObserver() = default;
    //código que se ejecuta "por suscripción" cuando algo importante sucede.
    //=0 boliga al que hereda a implementarla -> sino no va compilar
    virtual void onUpdate(int data,Topic topic) = 0;
    virtual Topic getInterest() = 0; // Filtro de interés
};

/*
contiene datos reales -> lista de obs,estado,sensores
el coordinador
*/

class IState {
public:
    virtual ~IState() = default;
    virtual void execute(SystemContext& ctx) = 0;
    //recibe ctx para poder controlar al sistema y saltar a otros estados cuando sea necesario.

    virtual const char* getName() = 0;
};




/*
=0
Convierte al método en una función virtual pura, y a la clase en una clase abstracta.

El Contexto (ctx) es el dueño del estado actual, pero el Estado (State) es el que tiene la inteligencia para decidir el cambio.

Por qué el Estado necesita al Contexto (ctx):
    
    Para ejecutar el cambio: El estado detecta una condición (ej: temperatura > 50°C) y llama a ctx.setState(new AlarmState()). Sin la referencia al contexto, el estado estaría "aislado" y no podría influir en el sistema.

    Para usar las herramientas del sistema: Si el estado quiere avisar a los observadores, usa ctx.notify().
*/
