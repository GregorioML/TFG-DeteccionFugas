//  DECLARACION DE VARIABLES  //

//Las variables no inicializadas a un valor, se inicializan automáticamente a 0

#ifndef Controladora2_h
#define Controladora2_h



#include "Arduino.h"


const int NO_EMPLEADO = 0;
const int DESTINO_DISPENSACION = 0;
const int ORIGEN_DISPENSACION = 1;
const bool DESCARGAS = 1;
const bool FUGAS = 1;

const bool SOLIC_COMPR_INICIAL = 1;



//  Tiempos utilizados  //

unsigned long InstanteActual;
unsigned long TiempoTotalPausado = 0;
const unsigned long TiempoSimMax = 3600000;     //1 hora en ms



unsigned long InstanteDatalogger = 0;                               //  Instante en el que se realizó el Datalogging
#define T_INTERVALO_DATALOGGER 10000                         






//  Sensores 

#define PIN_DESCARGAS_SENSOR_NIVEL_0 8                                    
#define PIN_DESCARGAS_SENSOR_NIVEL_1 9                                       
#define PIN_DESCARGAS_SENSOR_NIVEL_2 10                                 
#define PIN_DESCARGAS_SENSOR_NIVEL_3 11                                 

#define dout_almacenamiento 27                               
#define sck_almacenamiento 26                                    
float calibracion_almacenamiento = 13000;                    

#define dout_DepositoVentas1 35                               
#define sck_DepositoVentas1 34
float calibracion_DepositoVentas1 = 264000;                  

#define dout_DepositoVentas2 36                                 
#define sck_DepositoVentas2 37
float calibracion_DepositoVentas2 = 268000;                     

#define dout_fugas 39                                         
#define sck_fugas 38
float calibracion_fugas = 310000;                               



//  Actuadores

const int PIN_RELE_VACIADO_DESCARGAS = 28;        // RELÉ 8
const int PIN_RELE_LLENADO_VENTAS1 = 29;          // RELÉ 7
const int PIN_RELE_LLENADO_VENTAS2 = 30;          // RELÉ 6
const int PIN_RELE_VACIADO_FUGAS = 50;            // RELÉ 3
const int PIN_RELE_VACIADO_VENTAS1 = 48;          // RELÉ 5
const int PIN_RELE_VACIADO_VENTAS2 = 49;          // RELÉ 4
const int PIN_RELE_LLENADO_DESCARGAS = 52;        // RELÉ 1

#define PIN_ENA_fuga 7                                        
#define PIN_IN1_fuga 44                                                                
#define PIN_IN2_fuga 45    

#define PWM_MAX 255



//  Volúmenes

float volumen_recarga_limite = 33;                         
float volumen_recarga_seguridad = 30;                      

float volumen_almacenamiento_seguridad = 100;               
float volumen_almacenamiento_limite = 125;                   
float vol_min_alm_comienzo_bomba = 2;                   

float volumen_ventas_limite = 2.2;                            
float volumen_ventas_seguridad = 2;                        

float volumen_fugas_limite = 2.2;                            
float volumen_fugas_seguridad = 2;                         

float volumen_retorno_limite = 150;
float volumen_retorno_seguridad = 125;



//  Distribuciones aleatorias de dispensaciones

#define volumen_recarga_min 5                              
#define volumen_recarga_max 25 
#define volumen_ventas_min 0.5                               
#define volumen_ventas_max 1.6                             
#define t_prox_recarga_min 30000                             
#define t_prox_recarga_max 50000                              
#define t_prox_venta_min 10000                              
#define t_prox_venta_max 20000
#define t_prox_fuga_min 60000
#define t_prox_fuga_max 120000



//  Variables de simulación //
bool emergencia_recarga; 
bool emergencia_disp_1;
bool emergencia_disp_2;
bool emergencia_fugas;

bool ConjuntoListoParaReiniciar;




//  PANEL DE CONTROL  //

//Pines de entradas del panel
const int PIN_INTERRUPTOR_VACTIEMPO = 16;
const int PIN_INTERRUPTOR_VACPULSACION = 17;
const int PIN_INTERRUPTOR_RECIRTIEMPO = 18;
const int PIN_INTERRUPTOR_RECIRPULSACION = 19;
const int PIN_INTERRUPTOR_TARA = 20;
const int PIN_INTERRUPTOR_CALIBRACION = 21;
const int PIN_INTERRUPTOR_CONDINICIALES = 22;
const int PIN_INTERRUPTOR_CIERRE = 32;
const int PIN_PULSADOR_BOMBEO = 25;
const int PIN_PULSADOR_START = 31;
const int PIN_PULSADOR_STOP = 33;
const int PIN_INTERRUPTOR_MANUAL = 24;
const int PIN_INTERRUPTOR_EMERGENCIA = 23;
const int PIN_POTENCIOMETRO_ALTURA = A0;
const int PIN_POTENCIOMETRO_DIMENSIONES = A1;

//Pines de salidas del panel
const int PIN_LED_INICIALIZACION = 14;
const int PIN_LED_FUNCNORMAL = 15;
const int PIN_LED_PAUSA = 41;
const int PIN_LED_EMERGENCIA = 40;
const int PIN_LED_DESCARGAS = 6;
const int PIN_LED_ALMACENAMIENTO = 5;
const int PIN_LED_VENTAS1 = 4;
const int PIN_LED_VENTAS2 = 3;
const int PIN_LED_FUGAS = 2;


//Señales correspondientes a las entradas del panel
int EstadoInterruptorVaciadoTiempo;
int FlancoInterruptorVaciadoTiempo;
int EstadoInterruptorVaciadoPulsacion;
int FlancoInterruptorVaciadoPulsacion;
int EstadoInterruptorRecirculacionTiempo;
int FlancoInterruptorRecirculacionTiempo;
int EstadoInterruptorRecirculacionPulsacion;
int FlancoInterruptorRecirculacionPulsacion;
int EstadoInterruptorTara;
int FlancoInterruptorTara;
int EstadoInterruptorCalibracion;
int FlancoInterruptorCalibracion;
int EstadoInterruptorCondIniciales;
int FlancoInterruptorCondIniciales;
int EstadoInterruptorCierre;
int FlancoInterruptorCierre;
int EstadoPulsadorBombeo;
int FlancoPulsadorBombeo;
int EstadoPulsadorStart;
int FlancoPulsadorStart;
int EstadoPulsadorStop;
int FlancoPulsadorStop;
int EstadoInterruptorManual;
int FlancoInterruptorManual;
int EstadoInterruptorEmergencia;
int FlancoInterruptorEmergencia;
int LecturaPotenciometroAltura;
int LecturaPotenciometroDimensiones;

const int DIAL_AUTOMATICO = 0;
const int DIAL_MANUAL = 1;

const int PARPADEO_LENTO = 0;
const int PARPADEO_RAPIDO = 1;
const int PARPADEO_ERROR = 2;



// INICIALIZACIÓN y FUNCIONAMIENTO NORMAL //


//  ActualizarLecturaSensores()  //
bool DescargasSensorNivel0;
bool DescargasSensorNivel1;
bool DescargasSensorNivel2;
bool DescargasSensorNivel3;




//Variables de estado
const int IDLE_VACIADO = 0;
const int VACIADO = 1;
const int RECIRCULACION = 2;
const int TARA = 3;
const int CALIBRACION = 4;
const int COND_INICIALES = 5;
const int IDLE_FUNC_NORMAL = 6;
const int FUNC_NORMAL = 7;
const int EMERGENCIA = 29;
const int PAUSA = 30;
const int FIN_EXPERIMENTO = 31;      //o CIERRE

int EstadoActual = IDLE_VACIADO;


//Vaciado
bool DepositosVaciados;
int Selector;

//Recirculacion
bool DepositosRecirculados;

//Calibrar
bool DepositosCalibrados;

//Cond Iniciales
bool CondInicEstablecidas;






//  VaciarDepositosTiempo()  //
const int VACIADO_ABRIENDO_INFERIORES = 8;
const int VACIADO_ABRIENDO_SUPERIORES = 9;
const int VACIANDO_TODOS = 10;

int EstadoVaciadoT = VACIADO_ABRIENDO_INFERIORES;


unsigned long InstanteInicioMargenVaciado;
unsigned long InstanteInicioVaciado;
unsigned long InstanteComienzoPausa;
const unsigned long T_MARGEN_APERTURA_VACIADO = 5000;
const unsigned long T_VACIADO_DESCARGAS = 20000;
const unsigned long T_VACIADO_VENTAS = 15000;
const unsigned long T_VACIADO_ALMACENAMIENTO = 30000;






//  RecircularDepositosTiempo ()  //
const int RECIR_ABRIENDO_VALVULAS_1 = 11;
const int RECIR_ABRIENDO_VALVULAS_2 = 12;
const int RECIRCULANDO = 13;

int EstadoRecirculacionT = RECIR_ABRIENDO_VALVULAS_1;


unsigned long InstanteInicioRecirculacionRapidas;
unsigned long InstanteInicioRecirculacionTodas;
const unsigned long T_MARGEN_RECIRCULACION_AUTO = 10000;
const unsigned long T_RECIRCULACION_TOTAL = 10000;






//  RecircularDepositosPulsacion()  //
unsigned long InstanteIncioBombeoFuga;

const unsigned long T_MARGEN_RECIRCULACION_MANUAL = 10000;






//  CalibrarDepositos()  //
const int CALIBRACION_NIVEL_3 = 14;
const int CALIBRACION_NIVEL_2 = 15;
const int CALIBRACION_NIVEL_1 = 16;

int EstadoCalibracionNivel = CALIBRACION_NIVEL_3;







//  EstablecerCondInic()  //
const int ESTABLECIENDO_MARGENES = 17;
const int NIVEL_ALMACENAMIENTO = 18;
const int NIVEL_DESCARGAS = 19;

int EstadoCondInicialesSubetapa = ESTABLECIENDO_MARGENES;






//  CalibrarNivelX()  //
const int LLENANDO_DESCARGAS = 20;
const int VACIANDO_DESCARGAS = 21;
const int PESANDO_ALMACENAMIENTO = 22;
const int VACIANDO_ALMACENAMIENTO = 23;

int EstadoCalibracionNivelSubetapa = LLENANDO_DESCARGAS;

const int SENSOR_NIVEL_1 = 1;
const int SENSOR_NIVEL_2 = 2;
const int SENSOR_NIVEL_3 = 3;


unsigned long InstanteInicioVaciadoDescargas;
const unsigned long T_MARGEN_PESANDO_CALIBRACION = 5000;






//  CondInicialesEstablecerMargenes()  //
const int ABRIENDO_DESCARGAS  = 24;
const int LLENANDO_PRINCIPAL = 25;
const int DEPOSITANDO_MARGENES = 26;

int EstadoMargenes = ABRIENDO_DESCARGAS;


const float VOL_MARGEN_TOTAL = 10;          //Antes 3
const float VOL_MARGEN_VENTAS1 = 0.3;       //Antes 0.3
const float VOL_MARGEN_VENTAS2 = 0.3;       //Antes 0.3
const float VOL_MARGEN_FUGAS = 0.1;         //Antes 0.3

bool Ventas1MargenListo;
bool Ventas2MargenListo;
bool FugasMargenListo;





//  CondInicialesEstablecerNivelAlmacenamiento()  //
const int ASIGNANDO_NIVEL_ALEATORIO = 27;
const int DEPOSITANDO_NIVEL_ALEATORIO = 28;

int EstadoNivelAlmacenamiento = ASIGNANDO_NIVEL_ALEATORIO;


int VolumenAleatorioAlmacenamiento;




//  CondInicialesEstablecerNivelDescargas ()  //
int EstadoNivelDescargas = ASIGNANDO_NIVEL_ALEATORIO;


int VolumenAleatorioDescargas;





#endif
