//Debug.cpp

#include "Arduino.h"
#include "Debug.h"
#include "Deposito2.h"    


Debug::Debug(Deposito *DepositoDescargas, Deposito *DepositoAlmacenamiento, Deposito *DepositoVentas1, Deposito *DepositoVentas2, Deposito *DepositoFugas) {     //Ahora uso punteros a los tanques en lugar para prescindir de actualizar al principio de cada ciclo
  //Asigno los punteros a los objetos miembros de la clase
	_DepositoDescargas = DepositoDescargas;    
  _DepositoAlmacenamiento = DepositoAlmacenamiento;
	_DepositoVentas1 = DepositoVentas1;
	_DepositoVentas2 = DepositoVentas2;
	_DepositoFugas = DepositoFugas;
  //

  _TipoDebug = DEBUG_MINIMO;
  _IntervaloDebug = T_DEBUG_RAPIDO;

  //Strings sin variables numéricas
	_StrMargen = "\n\n\n";    //Márgenes y separadores para facilitar la lectura en el monitor serial - se pueden comentar si no hace 
  _StrSeparador = "---";
  _StrDebug = "Debug-";
  _StrN1 = "Nivel 1 (Min): ";
  _StrN2 = "Nivel 2 (Normal): ";
  _StrN3 = "Nivel 3 (Avanzado): ";
	_StrN4 = "Nivel 4 (Todo): ";
  //Los strings conformados a partir de la concatenación de datos variables no pueden estar en el constructor, ya que los datos variables no se actualizarían

  const int SENSOR_NIVEL_1 = 1;
  const int SENSOR_NIVEL_2 = 2;
  const int SENSOR_NIVEL_3 = 3;

  const int DEBUG_NULO = 0;
  const int DEBUG_MINIMO = 1;
  const int DEBUG_NORMAL = 2;
  const int DEBUG_AVANZADO = 3;
  const int DEBUG_TODO = 4;

  const unsigned long T_DEBUG_RAPIDO = 1000;
  const unsigned long T_DEBUG_MEDIO = 5000;
  const unsigned long T_DEBUG_LENTO = 10000;

}




void Debug::CambiarTipoDebug(int NuevoTipo) {
  _TipoDebug = NuevoTipo;
}



void Debug::CambiarIntervaloDebug(unsigned long NuevoIntervalo) {
  _IntervaloDebug = NuevoIntervalo;
}



void Debug::EnviarMensajeDebug(String Mensaje, unsigned long InstanteActual, int EstadoControladora) {       //Se sale de la función después de mostrar los mensajes correspondientes, para no escribir los mismos mensajes varias veces
  //Las strings de datos numéricos deben estar en esta función, ya que de lo contrario no mostraría la información actualizada
  
  _InstanteActual = InstanteActual;
  
  if (_TipoDebug == DEBUG_NULO){
    return;
  } 

  _StrMensaje = _StrDebug + _StrN1 + Mensaje;

  //Serial.print(_StrMargen);         
  Serial.println(_StrMensaje);
  //Serial.println(_StrSeparador);

  if (_TipoDebug == DEBUG_MINIMO){
    return;
  } 


  if (InstanteActual >= (_InstanteAnteriorDebug + _IntervaloDebug)){

    _InstanteAnteriorDebug = InstanteActual;

    _StrEstadoControladora = _StrDebug + _StrN2 + "Estado actual de la controladora = " + EstadoControladora;   
    _StrVolDescargas = _StrDebug + _StrN2 + "Volumen actual de Descargas = " + _DepositoDescargas->get_volumen();       
    _StrVolAlmacenamiento = _StrDebug + _StrN2 + "Volumen actual de Almacenamiento = " + _DepositoAlmacenamiento->get_volumen();
    _StrVolVentas1 = _StrDebug + _StrN2 + "Volumen actual de Ventas1 = " + _DepositoVentas1->get_volumen();
    _StrVolVentas2 = _StrDebug + _StrN2 + "Volumen actual de Ventas2 = " + _DepositoVentas2->get_volumen();
    _StrVolFugas = _StrDebug + _StrN2 + "Volumen actual de Fugas = " + _DepositoFugas->get_volumen();
    _StrProxVolDescargas = _StrDebug + _StrN2 + "Proximo Volumen de Descargas = " + _DepositoDescargas->serial_prox_vol();   
    _StrProxVolAlmacenamiento = _StrDebug + _StrN2 + "Proximo Volumen de Almacenamiento = " + _DepositoAlmacenamiento->serial_prox_vol();
    _StrProxVolVentas1 = _StrDebug + _StrN2 + "Proximo Volumen de Ventas1 = " + _DepositoVentas1->serial_prox_vol();
    _StrProxVolVentas2 = _StrDebug + _StrN2 + "Proximo Volumen de ventas2 = " + _DepositoVentas2->serial_prox_vol();
    _StrProxVolFugas = _StrDebug + _StrN2 + "Proximo Volumen de Fugas = " + _DepositoFugas->serial_prox_vol();
    
    Serial.println(_StrEstadoControladora);
    Serial.println(_StrVolDescargas);
    Serial.println(_StrVolAlmacenamiento);
    Serial.println(_StrVolVentas1);
    Serial.println(_StrVolVentas2);
    Serial.println(_StrVolFugas);
    Serial.println(_StrProxVolDescargas);
    Serial.println(_StrProxVolAlmacenamiento);
    Serial.println(_StrProxVolVentas1);
    Serial.println(_StrProxVolVentas2);
    Serial.println(_StrProxVolFugas);
    //Serial.println(_StrSeparador);

    if (_TipoDebug == DEBUG_NORMAL){
      return;
    } 


    
    _StrMaxAlmacenamiento = _StrDebug + _StrN3 + "Nivel maximo de Almacenamiento = " + _DepositoAlmacenamiento->get_VolMaxSeguridad();
    _StrMaxVentas1 = _StrDebug + _StrN3 + "Nivel maximo de Ventas1 = " + _DepositoVentas1->get_VolMaxSeguridad();
    _StrMaxVentas2 = _StrDebug + _StrN3 + "Nivel maximo de Ventas2 = " + _DepositoVentas2->get_VolMaxSeguridad();
    _StrMaxFugas = _StrDebug + _StrN3 + "Nivel maximo de Fugas = " + _DepositoFugas->get_VolMaxSeguridad();
    _StrVolDescargasNivel1 = _StrDebug + _StrN3 + "Volumen del nivel 1 de Descargas = " + _DepositoDescargas->get_DescargaSensor(SENSOR_NIVEL_1);
    _StrVolDescargasNivel2 = _StrDebug + _StrN3 + "Volumen del nivel 2 de Descargas = " + _DepositoDescargas->get_DescargaSensor(SENSOR_NIVEL_2);
    _StrVolDescargasNivel3 = _StrDebug + _StrN3 + "Volumen del nivel 3 de Descargas = " + _DepositoDescargas->get_DescargaSensor(SENSOR_NIVEL_3);


    Serial.println(_StrMaxAlmacenamiento);
    Serial.println(_StrMaxVentas1);
    Serial.println(_StrMaxVentas2);
    Serial.println(_StrMaxFugas);
    Serial.println(_StrVolDescargasNivel1);
    Serial.println(_StrVolDescargasNivel2);
    Serial.println(_StrVolDescargasNivel3);
    //Serial.println(_StrSeparador);




    if (_TipoDebug == DEBUG_AVANZADO){
      return;
    } 

    _StrNumDescargas = _StrDebug + _StrN4 + "Cantidad de descargas realizadas" + _DepositoDescargas->recargas_realizadas();
    _StrVolDescargasAcum = _StrDebug + _StrN4 + "Volumen acumulado de descargas" + _DepositoDescargas->volumen_acumulado_recarga();
    _StrNumVentas1 = _StrDebug + _StrN4 + "Cantidad de ventas realizadas en Ventas1" + _DepositoVentas1->ventas_realizadas();
    _StrVolVentas1Acum = _StrDebug + _StrN4 + "Volumen acumulado de ventas en Ventas1" + _DepositoVentas1->volumen_acumulado_venta();
    _StrNumVentas2 = _StrDebug + _StrN4 + "Cantidad de ventas realizadas en Ventas2" + _DepositoVentas2->ventas_realizadas();
    _StrVolVentas2Acum = _StrDebug + _StrN4 + "Volumen acumulado de ventas en Ventas2" + _DepositoVentas2->volumen_acumulado_venta();
    _StrVolFugasAcum = _StrDebug + _StrN4 + "Volumen acumulado de fugas" + _DepositoFugas->get_volumen();

    Serial.println(_StrNumDescargas);
    Serial.println(_StrVolDescargasAcum);
    Serial.println(_StrNumVentas1);
    Serial.println(_StrVolVentas1Acum);
    Serial.println(_StrNumVentas2);
    Serial.println(_StrVolVentas2Acum);
    Serial.println(_StrVolFugasAcum);
    //Serial.println(_StrSeparador);


    if (_TipoDebug == DEBUG_TODO){          //Prescindible porque ya es el fin de la función de todos modos
      return;
    }

  } else {

    return;

  }






}