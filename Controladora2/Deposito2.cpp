#include "Arduino.h"
#include "Deposito2.h"
#include "Debug.h"

#define TANQUE_VACIO 0
#define LLENADO 1
#define TANQUE_LLENO 2
#define VACIADO 3
#define EMERGENCIA 4
#define COMPR_INICIAL 5
#define FIN_EXPERIMENTO 6

#define SENSOR_NIVEL_1 1
#define SENSOR_NIVEL_2 2
#define SENSOR_NIVEL_3 3

#define ALTURA_FUGA_MAX 90
#define DIMENSIONES_FUGA_MAX 1    //Se puede ajustar a 0.5 para que, como máximo, el caudal de fuga sea la mitad

#define ANALOG_READ_MAX 1023
#define PWM_MAX 255
#define t_ciclo 0.003
#define r_max 51
#define v_r_max 512

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
const int calVal_eepromAdress = 0;
unsigned long t = 0;



//Constantes empleadas
const bool DESTINO_DISPENSACION = 0;
const bool ORIGEN_DISPENSACION = 1;

const int IDLE_VACIADO_CONTROLADORA = 0;


const int VOL_PRUEBA_RETORNO = 50;    //Ya que no hay  sensores en retorno





Deposito::Deposito (int PIN_Rele1_Llenado, int PIN_Rele1_Vaciado, int PIN_IN1, int PIN_IN2, int PIN_ENA, int HX711_dout, 
int HX711_sck, float valor_calibracion_basculas, int sensor_0, int sensor_1,  int sensor_2, int sensor_3, float capacidad_maxima, float vol_max_seguridad, float volumen_descarga_max, float volumen_descarga_min, 
float t_prox_descarga_max, float t_prox_descarga_min, bool UsoDeposito, Deposito *deposito_ant, Deposito *deposito_sig, bool es_fugas, bool es_recarga, float vol_seguridad_anterior, float vol_limite_anterior, float vol_min_comienzo_bomba, Debug *DebugDeposito) : LoadCell(HX711_dout, HX711_sck) {   

  _EstadoActualDeposito = TANQUE_VACIO;     
  


  _PIN_Rele1_Llenado = PIN_Rele1_Llenado;
  _PIN_Rele1_Vaciado = PIN_Rele1_Vaciado;
  _PIN_IN1 = PIN_IN1;
  _PIN_IN2 = PIN_IN2;
  _PIN_ENA = PIN_ENA;
  
  _sensor_0 = sensor_0;
  _sensor_1 = sensor_1;
  _sensor_2 = sensor_2;
  _sensor_3 = sensor_3;
  
  _HX711_dout = HX711_dout;
  _HX711_sck = HX711_sck;
  _valor_calibracion_basculas = valor_calibracion_basculas;

  _capacidad_maxima = capacidad_maxima;
  _vol_max_seguridad = vol_max_seguridad;
  _volumen_descarga_max = volumen_descarga_max;
  _volumen_descarga_min = volumen_descarga_min;
  _t_prox_descarga_max = t_prox_descarga_max;
  _t_prox_descarga_min = t_prox_descarga_min;  
  _vol_prox_mov = 0;
  _InstanteProxDisp = 0;          
  GenerarVolumenDisp();      
  GenerarInstanteProxDisp(0);       

  _UsoDeposito = UsoDeposito; 
  _es_fugas = es_fugas;
  _es_recarga = es_recarga; 
  
  _deposito_sig = deposito_sig;
  _deposito_ant = deposito_ant;

  _vol_seguridad_anterior = vol_seguridad_anterior; 
  _vol_limite_anterior = vol_limite_anterior;
  _vol_min_comienzo_bomba = vol_min_comienzo_bomba;

  _DebugDeposito = DebugDeposito;
  

  //  Asignación de pines 
  pinMode(_sensor_0, INPUT);
  pinMode(_sensor_1, INPUT);  
  pinMode(_sensor_2, INPUT); 
  pinMode(_sensor_3, INPUT);   
  
  pinMode(_PIN_Rele1_Llenado, OUTPUT); 
  pinMode(_PIN_Rele1_Vaciado, OUTPUT); 
  
  pinMode(_PIN_IN1, OUTPUT);
  pinMode(_PIN_IN2, OUTPUT);
  pinMode(_PIN_ENA, OUTPUT);  

  pinMode(_HX711_dout, INPUT); 
  pinMode(_HX711_sck, INPUT);


  //  Apagado de relés (active low)
  digitalWrite (_PIN_Rele1_Llenado , HIGH);
  digitalWrite (_PIN_Rele1_Vaciado, HIGH);   
                                                                                                                 
  analogWrite(_PIN_ENA, 0);                  
  digitalWrite(_PIN_IN1, LOW);
  digitalWrite(_PIN_IN2, LOW);    
   
}




//  Método para comprobar y actualizar el estado del panel
bool Deposito::actualizar_estado (unsigned long InstanteActual, unsigned long TiempoTotalPausado, int EmergenciaControladora,  int LecturaPotAltura, int LecturaPotDimensiones, int CierreControladora, int ComprobacionInicialControladora, int EstadoControladora) {          

  _DebugDeposito->EnviarMensajeDebug("Deposito -> Comienza actualizar_estado", InstanteActual, EstadoControladora);

  //Suma de tiempo pausado:
  if(TiempoTotalPausado > 0 && !_PausaSumada){     
    _InstanteProxDisp += TiempoTotalPausado;
    _PausaSumada = true;
  }
  //


  if (CierreControladora == HIGH) {                                                                                    
    _EstadoActualDeposito = FIN_EXPERIMENTO;
  }

  if (ComprobacionInicialControladora == HIGH){   //Al comenzar una ejecución por primera vez, después de finalizar un experimento o después de una emergencia, se consulta el estado del depósito para no ponerlo en VACIO si tiene algo de agua (como por ejemplo después de las cond iniciales) y evitar un posible desbordamiento
    _EstadoActualDeposito = COMPR_INICIAL;

  }


  if (_es_fugas) {                
    _CaudalFuga = generar_caudal(LecturaPotAltura, LecturaPotDimensiones);
  }
    

  if (EmergenciaControladora == HIGH) {                        // Se trata de la seta del panel, por lo que todos los tanques entran en alarma en conjunto
      _EstadoActualDeposito = EMERGENCIA;
  }
  

  if (_volumen >= _vol_max_seguridad) {                                      //  Condición de emergencia
      _EstadoActualDeposito = EMERGENCIA;   
  }

  
  if (_EstadoActualDeposito == COMPR_INICIAL){
    //Inicialización de los atributos con el valor en el que están al encender la Arduino
    InicializarAtributos();
    
    if ( ((_es_recarga) && (digitalRead(_sensor_1) == LOW))   ||   ((!_es_recarga) && (_volumen <= UMBRAL_VACIO)) ) {
      _EstadoActualDeposito = TANQUE_VACIO;

    } else if ( ((_es_recarga) && (digitalRead(_sensor_1) == HIGH))   ||   ((!_es_recarga) && (_volumen > UMBRAL_VACIO)) ) {
      _EstadoActualDeposito = TANQUE_LLENO;
    }
  }



  if (_EstadoActualDeposito == TANQUE_VACIO) {                                                                       
    cerrar_valvula_llenado();                                                                              
    cerrar_valvula_vaciado();                                                                                     
    if ((EstadoControladora != IDLE_VACIADO_CONTROLADORA) && (CierreControladora == 0)  &&  (_peticion_pausa_venta == 0)  &&  (InstanteActual >= _InstanteProxDisp)  &&  ( (_deposito_ant->ready_to_send(_vol_prox_mov) == 1) || (_es_fugas) || (_es_recarga) )    ) {
      _EstadoActualDeposito = LLENADO; 

    } else if (CierreControladora == 1){
      _EstadoActualDeposito = FIN_EXPERIMENTO;
    }

  }


  if ( (_EstadoActualDeposito == LLENADO) && (_peticion_pausa_fuga == 0) ) {          //  Estado que define el llenado del tanque
    abrir_valvula_llenado();                                                                                   
    cerrar_valvula_vaciado();                                                                                             
    if ((CierreControladora == 0) && ((_es_fugas == 0) && (_volumen >= _vol_prox_mov))) {
      _EstadoActualDeposito = TANQUE_LLENO;
      _volumen_total_venta = _volumen_total_venta + _volumen;
      _ventas_totales++;

    } else if ((CierreControladora == 0) && ((_es_fugas) && (_volumen >= 0.9 * _vol_max_seguridad))) {
      _EstadoActualDeposito = TANQUE_LLENO;
      _volumen_total_fuga += _volumen;     

    } else if (CierreControladora == 1){
      _EstadoActualDeposito = FIN_EXPERIMENTO;
    }
  }   
  

  if (_EstadoActualDeposito == TANQUE_LLENO) {                      
    cerrar_valvula_llenado();                                                                            
    cerrar_valvula_vaciado();                                                                                         
    if ((EstadoControladora != IDLE_VACIADO_CONTROLADORA) && (CierreControladora == 0) && (_es_fugas)) {                      
      _EstadoActualDeposito = VACIADO;

    } else if ((EstadoControladora != IDLE_VACIADO_CONTROLADORA) && (CierreControladora == 0)   &&   ((_deposito_sig->ready_to_receive(_vol_prox_mov) == 1)   &&   ((InstanteActual >= (_InstanteProxDisp) ) ||              
    (_UsoDeposito == DESTINO_DISPENSACION)) && (_peticion_pausa_recarga == 0) )) {
      _EstadoActualDeposito = VACIADO;
      _volumen_total_recarga = _volumen_total_recarga + _volumen;
      _recargas_totales++; 

    } else if (CierreControladora == 1){
      _EstadoActualDeposito = FIN_EXPERIMENTO;
    } 
  }
  

  if (_EstadoActualDeposito == VACIADO) {                                                          
    cerrar_valvula_llenado();                                                                                
    abrir_valvula_vaciado();                                                                                              
    if ((CierreControladora == 0) && (_volumen <= UMBRAL_VACIO)) {                                                                   
      _EstadoActualDeposito = TANQUE_VACIO;

      if (_es_fugas == 0){
        GenerarVolumenDisp();                    //Generación de nuevos valores de simulación
        GenerarInstanteProxDisp(InstanteActual);
      }

    }  else if (CierreControladora == 1){
      _EstadoActualDeposito = FIN_EXPERIMENTO;
    }   
  } 




  if (_EstadoActualDeposito == FIN_EXPERIMENTO) {  

    if (_es_recarga || _es_fugas){      //Si no es ventas, también cierra la válvula de llenado
      cerrar_valvula_llenado();   
    }
    
    if (_volumen > UMBRAL_VACIO){  
      abrir_valvula_vaciado();
      _ListoParaReiniciar = 0;
    } else {
      cerrar_valvula_llenado();
      _ListoParaReiniciar = 1;
    }

  }






  if (_EstadoActualDeposito == EMERGENCIA) {                                                                    
    cerrar_valvula_llenado();                                                                          
    cerrar_valvula_vaciado();                                                                                  
    
    return true;
  }
  



  return false;
}





//  Método que abre la válvula de llenado //

void Deposito::abrir_valvula_llenado () {
  if (_volumen < _vol_max_seguridad){         //Mecanismo de seguridad para evitar desbordamientos, pero sin meter el sistema en emergencia (otra medida más de seguridad)
    if (_es_fugas == 0) {
      digitalWrite(_PIN_Rele1_Llenado, LOW);   
    } else {
      digitalWrite(_PIN_IN1, HIGH);
      digitalWrite(_PIN_IN2, LOW);
      analogWrite(_PIN_ENA, _CaudalFuga);                                                                           
    }
  }
}

//  Método que abre la válvula de vaciado //

void Deposito::abrir_valvula_vaciado () {
  if (_volumen > 0){                          //Mecanismo de seguridad para evitar que las bombas trabajen en vacío, pero sin meter el sistema en emergencia
    digitalWrite(_PIN_Rele1_Vaciado, LOW);
  }
    
}





void Deposito::cerrar_valvula_llenado () {
  if (_es_fugas == 0) {

    digitalWrite(_PIN_Rele1_Llenado, HIGH);      

  } else {
    analogWrite(_PIN_ENA, 0);                   
    digitalWrite(_PIN_IN1, LOW);
    digitalWrite(_PIN_IN2, LOW);
  }
}





void Deposito::cerrar_valvula_vaciado () {

  digitalWrite(_PIN_Rele1_Vaciado, HIGH);
}


//  Método que generará valores aleatorios de tiempo a las variables de la simulación //

void Deposito::GenerarInstanteProxDisp (unsigned long InstantePartida) {       //Instante de partida es el momento a partir del cual se cuenta para la próxima dispensación (se invoca como InstanteActual)
  _InstanteProxDisp = InstantePartida + random(_t_prox_descarga_min, _t_prox_descarga_max);
}

//  Método que indica si el tanque está listo para recibir una dispensación

int Deposito::ready_to_receive (float _vol_prox_mov) {
  if(_vol_max_seguridad > (_volumen + _vol_prox_mov)) {     
    return 1;
  } else {
    return 0;
  }
}


//  Método que indica si el tanque anterior está listo para realizar

int Deposito::ready_to_send (float _vol_prox_mov) {
  if (get_volumen() > _vol_prox_mov) {     
    return 1;
  } else {
    return 0;
  }
}



//  Método de conversión de volumen medido por las basculas //

float Deposito::conversion_volumen () {    
  float resultado;
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0;                              //  Aumentar el valor para ralentizar la actividad de impresión en serie
  if (LoadCell.update()) newDataReady = true;                     //  Comprobar si hay nuevos datos / iniciar la siguiente conversión
  if (newDataReady) {                                             //  Obtener un valor suavizado del conjunto de datos
      resultado = LoadCell.getData(); 
  }
  return resultado;
}



int Deposito::generar_caudal(int LecturaPotAltura, int LecturaPotDimensiones) {
  int caudal;
  
  _AlturaFuga = map(LecturaPotAltura, 0, ANALOG_READ_MAX, 0, ALTURA_FUGA_MAX);   //De 0 a 90% de la altura del depósito para que la fuga nunca esté arriba del todo (el nivel del agua no llega hasta ahí casi nunca)
	

  if (_deposito_ant->get_volumen() < (_AlturaFuga * _vol_seguridad_anterior)) {
    int DimensionesFuga = map(LecturaPotDimensiones, 0, ANALOG_READ_MAX, 0, DIMENSIONES_FUGA_MAX);  
    caudal = PWM_MAX * DimensionesFuga;
  } else{
		caudal = 0;
	}

  return caudal;
}




//  Método que realiza la tara de las basculas 

bool Deposito::realizar_tara(bool _tare) {      //De manera predeterminada SÍ se hace tara (el valor predeterminado está en el prototipo de la librería)
  LoadCell.begin();
  float calibrationValue;
  bool tara_realizada;                                          
  calibrationValue = _valor_calibracion_basculas;                    //  Valor de calibración
  #if defined(ESP8266)|| defined(ESP32)
    //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
  #endif
    //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom
  
  unsigned long stabilizingtime = 3000;                             //  La precisión justo después del encendido se puede mejorar agregando unos segundos de tiempo de estabilización
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    //while (1);      //Investigar
    tara_realizada = false;
  
  } else {
    LoadCell.setCalFactor(calibrationValue);                       
    Serial.println("Tara Realizada correctamente");
    tara_realizada = true;
  }
  return tara_realizada;   
}




//  Funciones para detener proceso durante Datalogging  //

bool Deposito::solicitud_pausa_recarga() {
  _peticion_pausa_recarga = 1;
  bool recarga_pausada;
  if (_EstadoActualDeposito != VACIADO) {
    recarga_pausada = 1;
  }
  return recarga_pausada;
}

bool Deposito::solicitud_pausa_venta() {
  _peticion_pausa_venta = 1;    
  bool venta_pausada;
  if (_EstadoActualDeposito != LLENADO) {
    venta_pausada = 1;
  }
  return venta_pausada;
}



bool Deposito::solicitud_pausa_fuga() {    
  bool fuga_pausada;
  _peticion_pausa_fuga = 1;  
  if(  (_peticion_pausa_fuga == 1)&&(_EstadoActualDeposito == LLENADO) ){    
    cerrar_valvula_llenado();                                                                               
    fuga_pausada = 1;                       
  }
  return fuga_pausada;
} 

void Deposito::quitar_pausa() {
  _peticion_pausa_recarga = 0;     
  _peticion_pausa_venta = 0;
  _peticion_pausa_fuga = 0;
  
}

float Deposito::volumen_acumulado_recarga() {
  return _volumen_total_recarga;
}

float Deposito::volumen_acumulado_venta() {
  return _volumen_total_venta;
}

int Deposito::recargas_realizadas() {
  return _recargas_totales;
}

int Deposito::ventas_realizadas() {
  return _ventas_totales;
}

void Deposito::reset_recargas_realizadas(){
  _recargas_totales = 0;
}
  
void Deposito::reset_ventas_realizadas(){
  _ventas_totales = 0;
}




void Deposito::InicializarAtributos(){
  _EstadoActualDeposito = TANQUE_VACIO;

  _peticion_pausa_recarga = 0;
  _peticion_pausa_venta = 0;
  _peticion_pausa_fuga = 0;

  _volumen_total_recarga = 0;
  _volumen_total_venta = 0;
  _ventas_totales = 0;
  _recargas_totales = 0;

  _volumen_total_fuga = 0;

  _ListoParaReiniciar = 0;
}




//Métodos para conocer volumen de cada tanque

float Deposito::recarga_sensor_1_volumen(){
  return _descarga_sensor_1;
}

float Deposito::recarga_sensor_2_volumen(){
  return _descarga_sensor_2;
}

float Deposito::recarga_sensor_3_volumen(){
  return _descarga_sensor_3;
}


float Deposito::get_volumen () {  
  
  if (_deposito_sig->_es_recarga == 1){   //El único que cumple la condición es el de Retorno
    _volumen = VOL_PRUEBA_RETORNO;                      
  
  } else if (_es_recarga == 0) {
    _volumen = conversion_volumen ();     
 
  } else if (digitalRead(_sensor_3) == 1) {            // Si es el de descargas, el volumen actual es el que se asignó por cada nivel durante la inicialización (a través de un setter)
    _volumen = recarga_sensor_3_volumen();            

  } else if (digitalRead(_sensor_2) == 1) {              
    _volumen = recarga_sensor_2_volumen();            
     
  } else if (digitalRead(_sensor_1) == 1) {               
    _volumen = recarga_sensor_1_volumen();            
  
  } else if (digitalRead(_sensor_0) == 1) {
    _volumen = UMBRAL_VACIO;
 
  } else { 
    _volumen = 0;
  }

  return _volumen;
}







//Método para generar valores de próximas dispensaciones


void Deposito::GenerarVolumenDisp () {      
  int numero_sensor;
  if (_es_recarga == 0) {
    _vol_prox_mov = random (_volumen_descarga_min, _volumen_descarga_max); 

  } else {
    numero_sensor = random (1,N_SENSORES_REC);      //N_SENSORES_REC
   
    if (numero_sensor == 3){
      _vol_prox_mov = recarga_sensor_3_volumen();        
    }
    
    if (numero_sensor == 2){
      _vol_prox_mov = recarga_sensor_2_volumen();        
    }
     
    if (numero_sensor == 1) {
      _vol_prox_mov = recarga_sensor_1_volumen();        
    }
  }
}


float Deposito::serial_prox_vol(){
  return _vol_prox_mov;
}

float Deposito::serial_volumen(){
  return _volumen;
}







// Getters

float Deposito::get_VolMaxSeguridad(){
  return _vol_max_seguridad;
}


float Deposito::get_DescargaSensor(int NivelSensor){
  switch (NivelSensor){
    case SENSOR_NIVEL_1:
      return _descarga_sensor_1;
      break;

    case SENSOR_NIVEL_2:
      return _descarga_sensor_2;
      break;

    case SENSOR_NIVEL_3:
      return _descarga_sensor_3;
      break;
  }
}



bool Deposito::get_PausaSumada(){
  return _PausaSumada;

}



bool Deposito::get_ListoParaReiniciar(){
  return _ListoParaReiniciar;

}



float Deposito::get_ProxVolumen(){
  return _vol_prox_mov;

}



float Deposito::get_VolumenAcumulado(){
  return _volumen_total_recarga;

}



unsigned long Deposito::get_InstanteProxDisp(){
  return _InstanteProxDisp;

}



int Deposito::get_RecargasTotales(){
  return _recargas_totales;

}



float Deposito::get_AlturaFuga(){
  return _AlturaFuga;

}



int Deposito::get_CaudalFuga(){
  return _CaudalFuga;

}








//Setters

void Deposito::set_DepositoAnt(Deposito *NuevoDepositoAnt){
  _deposito_ant = NuevoDepositoAnt;
}


void Deposito::set_DepositoSig(Deposito *NuevoDepositoSig){
  _deposito_sig = NuevoDepositoSig;
}


void Deposito::set_DebugDeposito(Debug *NuevoDebugDeposito){
  _DebugDeposito = NuevoDebugDeposito;
}


void Deposito::set_VolumenDescarga(int NivelSensor, float NuevoVolumenDescarga){
  switch (NivelSensor){
    case SENSOR_NIVEL_1:    
      _descarga_sensor_1 = NuevoVolumenDescarga;
      break;

    case SENSOR_NIVEL_2:
      _descarga_sensor_2 = NuevoVolumenDescarga;
      break;

    case SENSOR_NIVEL_3:
      _descarga_sensor_3 = NuevoVolumenDescarga;
      break;
  }
  
}


void Deposito::set_PausaSumada(bool NuevoEstado){
  _PausaSumada = NuevoEstado;

}