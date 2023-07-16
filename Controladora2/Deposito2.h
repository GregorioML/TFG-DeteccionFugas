#ifndef Deposito2_h
#define Deposito2_h


#include "Arduino.h"
#include "Debug.h"

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// VARIABLES TANQUE RECARGA //
#define UMBRAL_VACIO 0.1                 //  Volumen cuando sensor_0 está activado, g = ml
#define VOL_REC_1 5                //  Volumen cuando sensor_1 está activado, g = ml
#define VOL_REC_2 20                  //  Volumen cuando sensor_2 está activado, g = ml
#define VOL_REC_3 30               //  Volumen cuando sensor_3 está activado, g = ml

#define N_SENSORES_REC 3                //  Numero de sensores utilizados en el Tanque de Recarga
#define RESOLUCION_REC 500              //  Resolucion entre cada sensor  

#define T_VAL_CONMUT 2000               //  Tiempo para conmutar las valvulas, tanto para abrir como cerrar

class Debug;

class Deposito { 
  
  private:
  
  //Atributos estáticos//
  bool _UsoDeposito;                  //  Variable que nos indica si, a la hora de simular, nos interesa si el deposito se recarga o descarga agua en el siguiente//  
  bool _es_fugas ;                           //  Parámetro que nos indicará si el tanque es el de fugas (1)//
  bool _es_recarga ;                         //  Parámetro que nos indicará si el tanque es el de recarga (1)//

  bool _peticion_pausa_recarga;
  bool _peticion_pausa_venta;
  bool _peticion_pausa_fuga;

  Deposito *_deposito_sig;                   //  Puntero que apunta al deposito siguiente con objetivo de obtener el permiso para realizar una descarga//
  Deposito *_deposito_ant;                   //  Puntero que apunta al deposito siguiente con objetivo de obtener el permiso para realizar una descarga//


  int _PIN_Rele1_Llenado;
  int _PIN_Rele1_Vaciado;
  int _PIN_IN1;
  int _PIN_IN2;
  int _PIN_ENA;
  
  int _sensor_0;
  int _sensor_1;
  int _sensor_2;
  int _sensor_3;

  HX711_ADC LoadCell;                       //Declaracion de la bascula  
  int _HX711_dout; 
  int _HX711_sck;
  float _valor_calibracion_basculas;

  //  Atributos referidos al tanque anterior  //
  float _vol_seguridad_anterior; 
  float _vol_limite_anterior;
  float _vol_min_comienzo_bomba;


  // Para depuracion
  Debug *_DebugDeposito;


 /*
  //  Tomamos el tiempo de inicio para establecer el tiempo de CONMUTADO de las valvulas //
  long t_inicio_valv_llenado_abierta = 0;
  long t_inicio_valv_llenado_cerrada = 0;
  long t_inicio_valv_vaciado_abierta = 0;
  long t_inicio_valv_vaciado_cerrada = 0;
  */
  
  /*  
  //  Condicion para no repetir conmutado de las valvulas //
  bool abrir_valv_llenado_activada_antes = 0;
  bool cerrar_valv_llenado_activada_antes = 0;
  bool abrir_valv_vaciado_activada_antes = 0;
  bool cerrar_valv_vaciado_activada_antes = 0;
  */

  //  Atributos del depósito //
  float _capacidad_maxima;                   
  float _vol_max_seguridad;                  //  Límite imaginario antes de alcanzar la capacidad máxima

  //  Atributos de la simulación //
  float _volumen_descarga_max;               
  float _volumen_descarga_min;               
  unsigned long _t_prox_descarga_max;                
  unsigned long _t_prox_descarga_min;                
  float _vol_prox_mov;                       
  unsigned long _InstanteProxDisp;                  
   
  //Atributos de estado//
  int _EstadoActualDeposito;                       
  float _volumen;                             
  float _CaudalFuga;    

  float _AlturaFuga;                      
  
  float _volumen_total_recarga = 0;
  float _volumen_total_venta = 0;
  int _ventas_totales = 0;
  int _recargas_totales = 0;

  float _volumen_total_fuga = 0;     
  

  float _descarga_sensor_3 = 30;         //Inicializar a un valor comprobado empíricamente, para el caso en el que se decida saltar la calibración de la inicialización
  float _descarga_sensor_2 = 20;
  float _descarga_sensor_1 = 5;

  bool _vacio_almacenamiento; 

  bool _llenar_sensor_1;
  bool _vaciar_sensor_1;
  
  bool _llenar_sensor_2;
  bool _vaciar_sensor_2;

  bool _llenar_sensor_3;
  bool _vaciar_sensor_3;





  bool _PausaSumada;
  bool _ListoParaReiniciar;


  //Constantes empleadas
  const bool DESTINO_DISPENSACION;
  const bool ORIGEN_DISPENSACION;

  const int IDLE_VACIADO_CONTROLADORA;





  public:

  //Constructor de la clase deposito//

  Deposito (int PIN_Rele1_Llenado, int PIN_Rele1_Vaciado, int PIN_IN1, int PIN_IN2, int PIN_ENA, int HX711_dout, int HX711_sck, float valor_calibracion_basculas, int sensor_0, int sensor_1,  int sensor_2, int sensor_3, 
float capacidad_maxima, float vol_max_seguridad, float volumen_descarga_max, float volumen_descarga_min, float t_prox_descarga_max, float t_prox_descarga_min, 
bool UsoDeposito, Deposito *deposito_ant, Deposito *deposito_sig, bool es_fugas, bool es_recarga, float vol_seguridad_anterior, float vol_limite_anterior, float vol_min_comienzo_bomba, Debug *DebugDeposito);


  //  Método que comprueba el estado del deposito//
  bool actualizar_estado (unsigned long InstanteActual, unsigned long TiempoTotalPausado, int EmergenciaControladora, int LecturaPotAltura, int LecturaPotDimensiones, int CierreControladora, int ComprobacionInicialControladora , int EstadoControladora); 


  //  Método que abre la válvula de llenado //
  void abrir_valvula_llenado ();


  //  Método que abre la válvula de vaciado //
  void abrir_valvula_vaciado ();


  //Método que cierra la válvula de llenado //
  void cerrar_valvula_llenado ();


  //  Método que cierra la válvula de vaciado //
  void cerrar_valvula_vaciado ();


  //  Método que genera valores aleatorios de volumen para la simulación
  void GenerarVolumenDisp();   

  
  //  Método que genera valores aleatorios de tiempo para la simulación
  void GenerarInstanteProxDisp(unsigned long InstantePartida);


 //  Método que indica si el tanque está listo para recibir una dispensación
  int ready_to_receive (float _vol_prox_mov);


  //  Método que indica si el tanque anterior está listo para realizar una dispensación  
  int ready_to_send (float _vol_prox_mov);


  //  Método que nos permite saber el volumen del tanque en todo momento  //
  float get_volumen ();

  //  Método que convierte la señal recibida por los sensores a volumen //
  float conversion_volumen ();


  //  Método genera un caudal para las fugas  //
  int generar_caudal(int LecturaPotAltura, int LecturaPotDimensiones);


  //  Método que realiza el tarado inicial  //
  bool realizar_tara (bool _tare = true);


  //  Método que realiza la solicitud neceraria para el Datalogger  dependiendo del Deposito  //
  bool solicitud_pausa_recarga ();
  bool solicitud_pausa_venta ();
  bool solicitud_pausa_fuga ();

  void quitar_pausa();


  //  Método que nos permite saber el volumen acumulado del tanque de recarga  //
  float volumen_acumulado_recarga();

  
  //  Método que nos permite saber el volumen acumulado del deposito de ventas  //
  float volumen_acumulado_venta();
  
  //  Método que nos permite saber las recargas realizadas  //
  int recargas_realizadas();
  
  //  Método que nos permite saber las ventas realizadas  //
  int ventas_realizadas();

  //  Método que resetea las recargas realizadas  //
  void reset_recargas_realizadas();
  
  //  Método que resetea las ventas realizadas  //
  void reset_ventas_realizadas();

  void InicializarAtributos();



  
  float recarga_sensor_1_volumen();    
  float recarga_sensor_2_volumen();
  float recarga_sensor_3_volumen();

  float serial_prox_vol();
  float serial_volumen();


  //getters para acceder a atributos desde el código principal
  float get_VolMaxSeguridad();
  float get_DescargaSensor(int NivelSensor);
  bool get_PausaSumada();
  bool get_ListoParaReiniciar();
  
  float get_ProxVolumen();
  float get_VolumenAcumulado();
  unsigned long get_InstanteProxDisp();
  int get_RecargasTotales();
  float get_AlturaFuga();
  int get_CaudalFuga();


  //Setters para reasignar valores a atributos
  void set_DepositoAnt(Deposito *NuevoDepositoAnt);   //Para designar correctamente los depósitos anterior y siguiente, una vez han sido instanciados todos los objetos Deposito
  void set_DepositoSig(Deposito *NuevoDepositoSig);

  void set_DebugDeposito(Debug *NuevoDebugDeposito);  //Para asignar un objeto Debug a cada depósito (necesario porque los parámetros de entrada de Debug son punteros a todos los Depositos)

  void set_VolumenDescarga(int NivelSensor, float NuevoVolumenDescarga);

  void set_PausaSumada(bool NuevoEstado);

};



#endif
