//Debug.h

#ifndef Debug_h
#define Debug_h

#include "Arduino.h"
#include "Deposito2.h"

class Deposito;  

class Debug {
  private:
    String _StrMargen, _StrSeparador, _StrDebug, _StrN1, _StrN2, _StrN3, _StrN4, _StrMensaje,
			_StrEstadoControladora, _StrVolDescargas, _StrVolAlmacenamiento, _StrVolVentas1, _StrVolVentas2, _StrVolFugas,
			_StrProxVolDescargas, _StrProxVolAlmacenamiento, _StrProxVolVentas1, _StrProxVolVentas2, _StrProxVolFugas,
			_StrMinDescargas, _StrMaxDescargas, _StrMinAlmacenamiento, _StrMaxAlmacenamiento,
			_StrMinVentas1, _StrMaxVentas1, _StrMinVentas2, _StrMaxVentas2, _StrMinFugas, _StrMaxFugas, _StrVolDescargasNivel1, _StrVolDescargasNivel2, _StrVolDescargasNivel3,
			_StrNumDescargas, _StrVolDescargasAcum, _StrNumVentas1, _StrVolVentas1Acum, _StrNumVentas2, _StrVolVentas2Acum, _StrVolFugasAcum;
		int _TipoDebug;
    unsigned long _InstanteActual;
    unsigned long _IntervaloDebug;
    unsigned long _InstanteAnteriorDebug;
    Deposito *_DepositoDescargas;
    Deposito *_DepositoAlmacenamiento;
		Deposito *_DepositoVentas1;
		Deposito *_DepositoVentas2;
		Deposito *_DepositoFugas;

    const int SENSOR_NIVEL_1;
    const int SENSOR_NIVEL_2;
    const int SENSOR_NIVEL_3;
		

    const int DEBUG_NULO;
    const int DEBUG_MINIMO;
    const int DEBUG_NORMAL;
    const int DEBUG_AVANZADO;
    const int DEBUG_TODO;

    const unsigned long T_DEBUG_RAPIDO;
    const unsigned long T_DEBUG_MEDIO;
    const unsigned long T_DEBUG_LENTO;

    

  public:
    Debug(Deposito *DepositoDescarga, Deposito *DepositoAlmacenamiento, Deposito *DepositoVentas1, Deposito *DepositoVentas2, Deposito *DepositoFugas);
    void CambiarTipoDebug(int NuevoTipo);
    void CambiarIntervaloDebug(unsigned long NuevoIntervalo);
    void EnviarMensajeDebug(String Mensaje, unsigned long InstanteActual, int EstadoControladora);
    
};

#endif