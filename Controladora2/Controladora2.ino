#include "Controladora2.h"
#include "Deposito2.h"
#include "Debug.h"
#include "PanelControl.h"


#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif


//Entradas del panel
PanelControl InterruptorVaciadoTiempo(PIN_INTERRUPTOR_VACTIEMPO, INPUT);
PanelControl InterruptorVaciadoPulsacion(PIN_INTERRUPTOR_VACPULSACION, INPUT);
PanelControl InterruptorRecirculacionTiempo(PIN_INTERRUPTOR_RECIRTIEMPO, INPUT);
PanelControl InterruptorRecirculacionPulsacion(PIN_INTERRUPTOR_RECIRPULSACION, INPUT);
PanelControl InterruptorTara(PIN_INTERRUPTOR_TARA, INPUT);
PanelControl InterruptorCalibracion(PIN_INTERRUPTOR_CALIBRACION, INPUT);
PanelControl InterruptorCondIniciales(PIN_INTERRUPTOR_CONDINICIALES, INPUT);
PanelControl InterruptorCierre(PIN_INTERRUPTOR_CIERRE, INPUT);
PanelControl PulsadorBombeo(PIN_PULSADOR_BOMBEO, INPUT);
PanelControl PulsadorStart(PIN_PULSADOR_START, INPUT);
PanelControl PulsadorStop(PIN_PULSADOR_STOP, INPUT);
PanelControl InterruptorManual(PIN_INTERRUPTOR_MANUAL, INPUT);
PanelControl InterruptorEmergencia(PIN_INTERRUPTOR_EMERGENCIA, INPUT);

PanelControl PotenciometroAltura(PIN_POTENCIOMETRO_ALTURA, INPUT);
PanelControl PotenciometroDimensiones(PIN_POTENCIOMETRO_DIMENSIONES, INPUT);


//Salidas del panel
PanelControl LedInicializacion(PIN_LED_INICIALIZACION, OUTPUT);
PanelControl LedFuncionamientoNormal(PIN_LED_FUNCNORMAL, OUTPUT);
PanelControl LedPausa(PIN_LED_PAUSA, OUTPUT);
PanelControl LedEmergencia(PIN_LED_EMERGENCIA, OUTPUT);

PanelControl LedDescargas(PIN_LED_DESCARGAS, OUTPUT);
PanelControl LedAlmacenamiento(PIN_LED_ALMACENAMIENTO, OUTPUT);
PanelControl LedVentas1(PIN_LED_VENTAS1, OUTPUT);
PanelControl LedVentas2(PIN_LED_VENTAS2, OUTPUT);
PanelControl LedFugas(PIN_LED_FUGAS, OUTPUT);




//  Tanques declarados a partir de la clase deposito  // 

/*
 Deposito (int PIN_Rele1_Llenado, int PIN_Rele1_Vaciado, int PIN_IN1, int PIN_IN2, int PIN_ENA, int HX711_dout, int HX711_sck, float valor_calibracion_basculas, int sensor_0, int sensor_1,  int sensor_2, int sensor_3, 
float capacidad_maxima, float vol_max_seguridad, float volumen_descarga_max, float volumen_descarga_min, float t_prox_descarga_max, float t_prox_descarga_min, 
bool UsoDeposito, Deposito *deposito_ant, Deposito *deposito_sig, bool es_fugas, bool es_recarga, float vol_seguridad_anterior, float vol_limite_anterior, float vol_min_comienzo_bomba, Debug *DebugDeposito);
*/

Deposito DepositoRetorno (PIN_RELE_VACIADO_VENTAS1, PIN_RELE_LLENADO_DESCARGAS , NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, 
volumen_retorno_limite, volumen_retorno_seguridad, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, DESTINO_DISPENSACION, &DepositoRetorno ,&DepositoRetorno, !FUGAS, !DESCARGAS, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO);

Deposito DepositoAlmacenamiento (PIN_RELE_VACIADO_DESCARGAS, PIN_RELE_LLENADO_VENTAS1, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, dout_almacenamiento, sck_almacenamiento, calibracion_almacenamiento, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO,
volumen_almacenamiento_limite, volumen_almacenamiento_seguridad, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, ORIGEN_DISPENSACION, &DepositoAlmacenamiento, &DepositoAlmacenamiento, !FUGAS, !DESCARGAS, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO);

Deposito DepositoDescargas (PIN_RELE_LLENADO_DESCARGAS , PIN_RELE_VACIADO_DESCARGAS, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, PIN_DESCARGAS_SENSOR_NIVEL_0, PIN_DESCARGAS_SENSOR_NIVEL_1, PIN_DESCARGAS_SENSOR_NIVEL_2,
PIN_DESCARGAS_SENSOR_NIVEL_3, volumen_recarga_limite, volumen_recarga_seguridad, volumen_recarga_max, volumen_recarga_min, t_prox_recarga_max, t_prox_recarga_min, ORIGEN_DISPENSACION, &DepositoRetorno, &DepositoAlmacenamiento, !FUGAS, DESCARGAS, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO);

Deposito DepositoVentas1 (PIN_RELE_LLENADO_VENTAS1, PIN_RELE_VACIADO_VENTAS1, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, dout_DepositoVentas1, sck_DepositoVentas1, calibracion_DepositoVentas1, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, volumen_ventas_limite,
volumen_ventas_seguridad, volumen_ventas_max, volumen_ventas_min, t_prox_venta_max, t_prox_venta_min, DESTINO_DISPENSACION, &DepositoAlmacenamiento, &DepositoRetorno, !FUGAS, !DESCARGAS, volumen_almacenamiento_seguridad, volumen_almacenamiento_limite, vol_min_alm_comienzo_bomba, NO_EMPLEADO);

Deposito DepositoVentas2 (PIN_RELE_LLENADO_VENTAS2, PIN_RELE_VACIADO_VENTAS2, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, dout_DepositoVentas2, sck_DepositoVentas2, calibracion_DepositoVentas2, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, volumen_ventas_limite,
volumen_ventas_seguridad, volumen_ventas_max, volumen_ventas_min, t_prox_venta_max, t_prox_venta_min, DESTINO_DISPENSACION, &DepositoAlmacenamiento, &DepositoRetorno, !FUGAS, !DESCARGAS, volumen_almacenamiento_seguridad, volumen_almacenamiento_limite, vol_min_alm_comienzo_bomba, NO_EMPLEADO);

Deposito DepositoFugas (NO_EMPLEADO, PIN_RELE_VACIADO_FUGAS, PIN_IN1_fuga, PIN_IN2_fuga, PIN_ENA_fuga, dout_fugas, sck_fugas, calibracion_fugas, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, volumen_fugas_limite, volumen_fugas_seguridad,
NO_EMPLEADO, NO_EMPLEADO, t_prox_fuga_max, t_prox_fuga_min, DESTINO_DISPENSACION, &DepositoAlmacenamiento, &DepositoRetorno, FUGAS, !DESCARGAS, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO, NO_EMPLEADO);    //Depósito anterior y siguiente corregido





//Objetos para mensajes de depuración
Debug DebugGeneral(&DepositoDescargas, &DepositoAlmacenamiento, &DepositoVentas1, &DepositoVentas2, &DepositoFugas);  //De momento gestiono todos los mensajes a la vez, pero si me interesase sólo un conjunto de depósitos podría instanciar objetos que 
                                                                                                                      //sólo me den información de ellos (aunque habría poner un NO_EMPLEADO en el lugar de los depósitos que no interesan)



//Para la recepción de emergencia del dashboard
String EntradaSerial = ""; 
bool DatosNuevosSerial = false;




void setup () {
  Serial.begin(9600);
  EntradaSerial.reserve(50); //Se reserva memoria para la entrada serial

  //Ahora con todas los depositos declarados, se corrige el puntero a depósito anterior y siguiente
  DepositoRetorno.set_DepositoAnt(&DepositoVentas1);
  DepositoRetorno.set_DepositoSig(&DepositoDescargas);

  DepositoAlmacenamiento.set_DepositoAnt(&DepositoDescargas);
  DepositoAlmacenamiento.set_DepositoSig(&DepositoVentas1);

  DepositoDescargas.set_DebugDeposito(&DebugGeneral);
  DepositoAlmacenamiento.set_DebugDeposito(&DebugGeneral);
  DepositoVentas1.set_DebugDeposito(&DebugGeneral);
  DepositoVentas2.set_DebugDeposito(&DebugGeneral);
  DepositoFugas.set_DebugDeposito(&DebugGeneral);


  //Los relés comienzan apagados (active low)
  digitalWrite (PIN_RELE_LLENADO_DESCARGAS , HIGH);
  digitalWrite (PIN_RELE_VACIADO_DESCARGAS, HIGH);  

  digitalWrite (PIN_RELE_LLENADO_VENTAS1, HIGH); 
  digitalWrite (PIN_RELE_LLENADO_VENTAS2, HIGH);   
 
  digitalWrite (PIN_RELE_VACIADO_VENTAS1, HIGH);
  digitalWrite (PIN_RELE_VACIADO_VENTAS2, HIGH);   
                                                                                                                 
  digitalWrite (PIN_RELE_VACIADO_FUGAS, HIGH);




  randomSeed(analogRead(A4));   //Para los niveles aleatorios de la inicialización y el funcionamiento normal
  DebugGeneral.EnviarMensajeDebug("Saliendo de setup", InstanteActual, EstadoActual);

}






// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





void loop () {
  
  InstanteActual = millis();

  DebugGeneral.EnviarMensajeDebug("Inicio de loop", InstanteActual, EstadoActual);


  ActualizarEntradasPanel();
  ActualizarLecturaSensores();

  






                  //  Proceso de Inicialización  //

/********************************************************************************************************************************/
/**************************************************     Pre-Vaciado    **********************************************************/
/********************************************************************************************************************************/
 //A la espera de pulsar Start para empezar
  if (EstadoActual == IDLE_VACIADO){

    DebugGeneral.EnviarMensajeDebug("Estado: Pre-Vaciado", InstanteActual, EstadoActual);

    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    InicializacionVariables();

    //Se invoca actualizar_estado() con un parámetro de entrada ComprobacionInicialControladora a 1 para que todos los depósitos se metan en el estado ComprobacionInicial
    DepositoDescargas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia, LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, SOLIC_COMPR_INICIAL, EstadoActual);
    DepositoVentas1.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia, LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, SOLIC_COMPR_INICIAL, EstadoActual);
    DepositoVentas2.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia, LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, SOLIC_COMPR_INICIAL, EstadoActual);
    DepositoFugas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia, LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, SOLIC_COMPR_INICIAL, EstadoActual); 
    


    DebugGeneral.EnviarMensajeDebug("Listo para comenzar Proceso de Inicialización", InstanteActual, EstadoActual);

    if ((EstadoInterruptorManual == DIAL_AUTOMATICO) || ((EstadoInterruptorManual == DIAL_MANUAL) && (FlancoPulsadorStart == 1))){
      ActualizarEstadoGeneral(VACIADO);
      Serial.println("Pasando a Vaciado");
    }

  }



/********************************************************************************************************************************/
/****************************************************     Vaciado    ************************************************************/
/********************************************************************************************************************************/
 //Al entrar en esta etapa, no sabemos a qué nivel está cada tanque

  if (EstadoActual == VACIADO){
    Serial.println("Vaciado");

    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    DebugGeneral.EnviarMensajeDebug("Estado: VACIADO", InstanteActual, EstadoActual);

    if (EstadoInterruptorVaciadoTiempo == HIGH) {
      DepositosVaciados = VaciarDepositosTiempo();   //la función hace el vaciado y devuelve un 1 cuando acaba

      if (DepositosVaciados){
        ActualizarEstadoGeneral(RECIRCULACION); 
      }


    } else if (EstadoInterruptorVaciadoPulsacion == HIGH) {
      VaciarDepositosPulsacion();

      if (FlancoPulsadorStop == 1){
        Selector = 0;
        ActualizarEstadoGeneral(RECIRCULACION); 
      }
      

    } else {                                      //BotoneraVaciadoPulsacion == LOW   &&   BotoneraVaciadoTiempo == LOW
      ActualizarEstadoGeneral(RECIRCULACION); 
    }


  }





/********************************************************************************************************************************/
/************************************************     Recirculacion    **********************************************************/
/********************************************************************************************************************************/
//Al entrar en esta etapa, se supone que que todos los tanques están a un nivel muy bajo, e incluso completamente vacíos

  if (EstadoActual == RECIRCULACION) {
    Serial.println("Recirculacion");

    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    DebugGeneral.EnviarMensajeDebug("Estado: RECIRCULACION", InstanteActual, EstadoActual);

    if (EstadoInterruptorRecirculacionTiempo == HIGH) {
		  DepositosRecirculados = RecircularDepositosTiempo(); 

      if (DepositosRecirculados){
        ActualizarEstadoGeneral(TARA);
      }

	  } else if (EstadoInterruptorRecirculacionPulsacion == HIGH) {
		  RecircularDepositosPulsacion();

		  if (FlancoPulsadorStop == 1){
        Selector = 0;
        ActualizarEstadoGeneral(TARA);
		  }
		
    } else {
      ActualizarEstadoGeneral(TARA);
    }


  }






/********************************************************************************************************************************/
/*****************************************************     Tarar    *************************************************************/
/********************************************************************************************************************************/
//Al entrar en esta etapa, se supone que todos los depósitos tienen únicamente el fondillo

  if (EstadoActual == TARA) {    
    Serial.println("Tara");

    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }
    
    DebugGeneral.EnviarMensajeDebug("Estado: TARA", InstanteActual, EstadoActual);
	
    if (EstadoInterruptorTara == HIGH) {
      bool TaraAlmacenamiento = DepositoAlmacenamiento.realizar_tara(); 
      bool TaraVentas1 = DepositoVentas1.realizar_tara();
      bool TaraVentas2 = DepositoVentas2.realizar_tara();
      bool TaraFugas = DepositoFugas.realizar_tara(); 
 
      if (TaraVentas1 && TaraVentas2 && TaraAlmacenamiento && TaraFugas) {
        ActualizarEstadoGeneral(CALIBRACION);
        DebugGeneral.EnviarMensajeDebug("Tara 1 realizada", InstanteActual, EstadoActual);
        DebugGeneral.EnviarMensajeDebug("Pasando a Calibración", InstanteActual, EstadoActual);
      }
 
      

    } else {
      DepositoAlmacenamiento.realizar_tara(false); 
      DepositoVentas1.realizar_tara(false);
      DepositoVentas2.realizar_tara(false);
      DepositoFugas.realizar_tara(false);
      ActualizarEstadoGeneral(CALIBRACION);   
    }
      
  }







/********************************************************************************************************************************/
/*****************************************************     Calibrar    **********************************************************/
/********************************************************************************************************************************/
//Al entrar en esta etapa, como en la anterior no hubo cambios, se supone que todos los tanque tienen únicamente el fondillo


  if (EstadoActual == CALIBRACION) {   
    Serial.println("Calibracion");

    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    DebugGeneral.EnviarMensajeDebug("Estado: CALIBRACION", InstanteActual, EstadoActual);

    if (EstadoInterruptorCalibracion == HIGH) {
      DepositosCalibrados = CalibrarDepositos();
      
      if (DepositosCalibrados){
        ActualizarEstadoGeneral(COND_INICIALES);
      }

    } else {
      ActualizarEstadoGeneral(COND_INICIALES);
    }

  }





/********************************************************************************************************************************/
/*************************************************     Cond. Iniciales    *******************************************************/
/********************************************************************************************************************************/
//Al entrar en esta etapa, todos los tanques tienen únicamente el fondillo, a excepción del depósito principal, que además tiene un volumen de descarga de nivel 2

  if (EstadoActual == COND_INICIALES) {
    Serial.println("Cond-Iniciales");
    
    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    DebugGeneral.EnviarMensajeDebug("Estado: COND_INICIALES", InstanteActual, EstadoActual);

    if (EstadoInterruptorCondIniciales == HIGH) {
      CondInicEstablecidas = EstablecerCondInic();
      
      if (CondInicEstablecidas){
        ActualizarEstadoGeneral(IDLE_FUNC_NORMAL);
      }

    } else {
      ActualizarEstadoGeneral(IDLE_FUNC_NORMAL);
    }

  }




/********************************************************************************************************************************/
/*********************************************     Pre-Funcionamiento Normal    *************************************************/
/********************************************************************************************************************************/

  if (EstadoActual == IDLE_FUNC_NORMAL){
      Serial.println("Pre-Funcionamiento_Normal");
    
    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    DebugGeneral.EnviarMensajeDebug("Listo para comenzar Funcionamiento Normal", InstanteActual, EstadoActual);

    if ((EstadoInterruptorManual == DIAL_AUTOMATICO) || ((EstadoInterruptorManual == DIAL_MANUAL) && (FlancoPulsadorStart == 1))){
      ActualizarEstadoGeneral(FUNC_NORMAL);
    }
  }













//**************************************  FUNCIONAMIENTO NORMAL  *****************************************************************************************
  if (EstadoActual == FUNC_NORMAL) {


    
    if ((EstadoInterruptorEmergencia == LOW) && (FlancoPulsadorStop == HIGH)) {            
      
      if (InstanteComienzoPausa == 0){    //Si InstanteComienzoPausa es 0, quiere decir que o bien aún no ha sucedido una Pausa, o bien ya hemos sumado el tiempo de pausa anterior a TiempoTotalPausado
        InstanteComienzoPausa = InstanteActual;
      }
      ActualizarEstadoGeneral(PAUSA);
    }


      
    emergencia_recarga = DepositoDescargas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);  
    emergencia_disp_1 = DepositoVentas1.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);
    emergencia_disp_2 = DepositoVentas2.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);
    emergencia_fugas = DepositoFugas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);
    
    if ((emergencia_recarga == true) || (emergencia_disp_1 == true) || (emergencia_disp_2 == true) || (emergencia_fugas == true) || (EstadoInterruptorEmergencia == HIGH)) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }


    if(DepositoDescargas.get_PausaSumada() && DepositoVentas1.get_PausaSumada() && DepositoVentas2.get_PausaSumada()){
      TiempoTotalPausado = 0;
    }



    if (((EstadoPulsadorStop == LOW) && (EstadoInterruptorEmergencia == LOW) && (EstadoInterruptorCierre == HIGH)) || InstanteActual >= TiempoSimMax ) {
      ActualizarEstadoGeneral(FIN_EXPERIMENTO);
                                 
    }
    


  }


//*******************************************  PAUSA  *****************************************************************************************
  if (EstadoActual == PAUSA) {

    if (EstadoInterruptorEmergencia == HIGH) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    if (InstanteComienzoPausa == 0){                //Si acabamos de entrar en Pausa desde funcionamiento normal, se registra el instante de comienzo de la pausa.
      InstanteComienzoPausa = InstanteActual;
    }
      
    
    DepositoDescargas.cerrar_valvula_llenado();
    DepositoDescargas.cerrar_valvula_vaciado();
    DepositoVentas1.cerrar_valvula_llenado();
    DepositoVentas1.cerrar_valvula_vaciado();     
    DepositoVentas2.cerrar_valvula_llenado();
    DepositoVentas2.cerrar_valvula_vaciado(); 

    if ((EstadoPulsadorStop == LOW) && (EstadoInterruptorEmergencia == LOW) && (FlancoPulsadorStart == HIGH)) {    
      
      TiempoTotalPausado += InstanteActual - InstanteComienzoPausa;  // Se calcula el tiempo en PAUSA
      InstanteComienzoPausa = 0;                                     // Se resetea el InstanteComienzoPausa para que se pueda asignar al instante correspondiente de sucesivas pausas

      DepositoDescargas.set_PausaSumada(false); 
      DepositoVentas1.set_PausaSumada(false); 
      DepositoVentas2.set_PausaSumada(false); 
      DepositoFugas.set_PausaSumada(false); 

      
      ActualizarEstadoGeneral(FUNC_NORMAL); 
                                      
    }

    if ((EstadoPulsadorStart == LOW) && (EstadoInterruptorEmergencia == LOW) && (EstadoInterruptorCierre == HIGH)) {
      ActualizarEstadoGeneral(FIN_EXPERIMENTO);
                                 
    }


  }






//*******************************************  CIERRE (nuevo)  *****************************************************************************************
  if (EstadoActual == FIN_EXPERIMENTO) {
    
    emergencia_recarga = DepositoDescargas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);  
    emergencia_disp_1 = DepositoVentas1.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);
    emergencia_disp_2 = DepositoVentas2.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);
    emergencia_fugas = DepositoFugas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual); 

    if ((emergencia_recarga == true) || (emergencia_disp_1 == true) || (emergencia_disp_2 == true) || (emergencia_fugas == true) || (EstadoInterruptorEmergencia == HIGH)) {
      ActualizarEstadoGeneral(EMERGENCIA);
    }

    if (DepositoDescargas.get_ListoParaReiniciar() == 1 && DepositoVentas1.get_ListoParaReiniciar() == 1 && DepositoVentas2.get_ListoParaReiniciar() == 1 && DepositoFugas.get_ListoParaReiniciar() == 1){
      ConjuntoListoParaReiniciar = 1;
    }

    if ((EstadoActual != EMERGENCIA) && (EstadoInterruptorEmergencia == LOW) && (EstadoInterruptorCierre == LOW) && (ConjuntoListoParaReiniciar == 1)   &&   ((EstadoInterruptorManual == DIAL_AUTOMATICO) || ((EstadoInterruptorManual == DIAL_MANUAL) && (FlancoPulsadorStart == 1)))) {
      ActualizarEstadoGeneral(IDLE_VACIADO); 
    }

  }













//*******************************************  EMERGENCIA  *****************************************************************************************
  if (EstadoActual == EMERGENCIA) {

    DepositoDescargas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);    //EstadoInterruptorEmergencia es HIGH
    DepositoVentas1.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);
    DepositoVentas2.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual);
    DepositoFugas.actualizar_estado(InstanteActual, TiempoTotalPausado, EstadoInterruptorEmergencia,  LecturaPotenciometroAltura, LecturaPotenciometroDimensiones, EstadoInterruptorCierre, !SOLIC_COMPR_INICIAL, EstadoActual); 

    if ((EstadoInterruptorEmergencia == LOW) && (FlancoPulsadorStart == HIGH)) {
      ActualizarEstadoGeneral(IDLE_VACIADO);
    }
  }







//  Toma de valores de volumen de los depositos de manera periódica, para la detección de fugas//
  if ( (InstanteActual > (T_INTERVALO_DATALOGGER + InstanteDatalogger)) && (EstadoActual == FUNC_NORMAL) ) {   
    String str = "";  
    DepositoDescargas.solicitud_pausa_recarga();
    DepositoVentas1.solicitud_pausa_venta();
    DepositoVentas2.solicitud_pausa_venta();
    DepositoFugas.solicitud_pausa_fuga();
    if (DepositoDescargas.solicitud_pausa_recarga() && DepositoVentas1.solicitud_pausa_venta() && DepositoVentas2.solicitud_pausa_venta() && DepositoFugas.solicitud_pausa_fuga() )  {    
    
      str.concat("Debug - Datalogging:" );      //Para diferenciarlo del resto de mensajes por monitor serial
      str.concat( DepositoAlmacenamiento.get_volumen() );
      str.concat(",");
      str.concat( DepositoDescargas.recargas_realizadas() );
      str.concat(",");
      str.concat( DepositoDescargas.volumen_acumulado_recarga() );
      str.concat(",");
      str.concat( DepositoVentas1.ventas_realizadas() );
      str.concat(",");
      str.concat( DepositoVentas1.volumen_acumulado_venta() );
      str.concat(",");
      str.concat( DepositoVentas2.ventas_realizadas() );
      str.concat(",");
      str.concat( DepositoVentas2.volumen_acumulado_venta() );
      str.concat(",");
      str.concat( DepositoFugas.get_volumen() ); 
      Serial.println(str);
    

      InstanteDatalogger = InstanteActual;                   
      DepositoDescargas.reset_recargas_realizadas();
      DepositoVentas1.reset_ventas_realizadas();
      DepositoVentas2.reset_ventas_realizadas();
      DepositoDescargas.quitar_pausa();
      DepositoVentas1.quitar_pausa();
      DepositoVentas2.quitar_pausa();
      DepositoFugas.quitar_pausa();
    }
  }



  ActualizarSalidasPanel()
  EnviarDatosDashboard();
}




























////////////////////////////////////////////////////////////////////////////////////







bool VaciarDepositosTiempo(){

  DebugGeneral.EnviarMensajeDebug("Vaciando por tiempo", InstanteActual, EstadoActual);

	if (EstadoVaciadoT == VACIADO_ABRIENDO_INFERIORES) {        //"EstadoVaciadoT" se inicializa como "VACIADO_ABRIENDO_INFERIORES"
		DepositoVentas1.abrir_valvula_vaciado();
		DepositoVentas2.abrir_valvula_vaciado();
		DepositoFugas.abrir_valvula_vaciado();

		InstanteInicioMargenVaciado = InstanteActual;
		EstadoVaciadoT = VACIADO_ABRIENDO_SUPERIORES;
		return 0;

	} 


	if ((EstadoVaciadoT == VACIADO_ABRIENDO_SUPERIORES) && (InstanteActual - InstanteInicioMargenVaciado >= T_MARGEN_APERTURA_VACIADO)){
		DepositoDescargas.abrir_valvula_vaciado();
		DepositoAlmacenamiento.abrir_valvula_vaciado();
	
		InstanteInicioVaciado = InstanteActual;
		EstadoVaciadoT = VACIANDO_TODOS;
		return 0;
	}


	if ((EstadoVaciadoT == VACIANDO_TODOS) && (!DescargasSensorNivel0) || ((EstadoVaciadoT == VACIANDO_TODOS) && (InstanteActual - InstanteInicioVaciado >= T_VACIADO_DESCARGAS))){     
		DepositoDescargas.cerrar_valvula_vaciado();
		return 0;
	}


	if ((EstadoVaciadoT == VACIANDO_TODOS) && (InstanteActual - InstanteInicioVaciado >= T_VACIADO_VENTAS)){
		DepositoVentas2.cerrar_valvula_vaciado();
		return 0;
	}

	
	if ((EstadoVaciadoT == VACIANDO_TODOS) && (InstanteActual - InstanteInicioVaciado >= T_VACIADO_VENTAS)){
		DepositoFugas.cerrar_valvula_vaciado();
		return 0;
	}


	if ((EstadoVaciadoT == VACIANDO_TODOS) && (InstanteActual - InstanteInicioVaciado >= T_VACIADO_ALMACENAMIENTO)){
		DepositoAlmacenamiento.cerrar_valvula_vaciado();
		DepositoVentas1.cerrar_valvula_vaciado();
		return 1;
	}
	
}


////////////////////////////////////////////////////////////////////////////////////


void VaciarDepositosPulsacion(){

  Serial.println("Entra a VaciarDepositosPulsacion");
  Serial.println(EstadoActual);

  DebugGeneral.EnviarMensajeDebug("Vaciando por pulsacion", InstanteActual, EstadoActual);

  //Antes de consultar las condiciones se encendería el led correspondiente a selector y se apagaría el resto
	
	if ((EstadoPulsadorBombeo == LOW) && (FlancoPulsadorStart == 0) && (FlancoPulsadorStop == 0)) {        
		DepositoDescargas.cerrar_valvula_llenado();
		DepositoDescargas.cerrar_valvula_vaciado();		
		DepositoVentas1.cerrar_valvula_llenado();
		DepositoVentas1.cerrar_valvula_vaciado();
		DepositoVentas2.cerrar_valvula_llenado();
		DepositoVentas2.cerrar_valvula_vaciado();
		DepositoFugas.cerrar_valvula_llenado();
		DepositoFugas.cerrar_valvula_vaciado();
		
	}


	if ((EstadoPulsadorBombeo == LOW) && (FlancoPulsadorStart == 1)) {
		Selector++;

		if (Selector == 5) {
			Selector = 0;
		}
		
	}


	if ((EstadoPulsadorBombeo == HIGH) && (FlancoPulsadorStart == 0) && (FlancoPulsadorStop == 0)) {
		switch (Selector){
			case 0:
			  DepositoVentas1.abrir_valvula_vaciado();
			  break;

			case 1:
			  DepositoVentas2.abrir_valvula_vaciado();
			  break;

			case 2:
			  DepositoFugas.abrir_valvula_vaciado();
			  break;

			case 3:
			  DepositoAlmacenamiento.abrir_valvula_vaciado();
			  DepositoVentas1.abrir_valvula_vaciado();
		  	break;

			case 4:
        DepositoDescargas.abrir_valvula_vaciado();
        DepositoAlmacenamiento.abrir_valvula_vaciado();
        DepositoVentas1.abrir_valvula_vaciado();
			  break;
		}
		
	}

}


////////////////////////////////////////////////////////////////////////////////////


void ActualizarEstadoGeneral(int SiguienteEstado){   //Las opciones son cada una de las constantes VACIADO, RECIRCULACION, TARA...

  EstadoActual = SiguienteEstado;   
        
}



////////////////////////////////////////////////////////////////////////////////////



bool RecircularDepositosTiempo(){

  DebugGeneral.EnviarMensajeDebug("Recirculando por tiempo", InstanteActual, EstadoActual);

	if (EstadoRecirculacionT == RECIR_ABRIENDO_VALVULAS_1) {      
		DepositoDescargas.abrir_valvula_llenado();
		DepositoDescargas.abrir_valvula_vaciado();		
		DepositoVentas1.abrir_valvula_llenado();
		DepositoVentas1.abrir_valvula_vaciado();
		DepositoVentas2.abrir_valvula_llenado();
		DepositoVentas2.abrir_valvula_vaciado();
		DepositoFugas.abrir_valvula_llenado();
    InstanteInicioRecirculacionRapidas = InstanteActual;       //Marca el instante de apertura de los depósitos que se llenan más rápido (todos menos el de fugas, que se vaciará con el siguiente if)
    EstadoRecirculacionT = RECIR_ABRIENDO_VALVULAS_2;

    return 0;

  } else if ((EstadoRecirculacionT == RECIR_ABRIENDO_VALVULAS_2) && (InstanteActual - InstanteInicioRecirculacionRapidas >= T_MARGEN_RECIRCULACION_AUTO)){
    DepositoFugas.abrir_valvula_vaciado();            
    InstanteInicioRecirculacionTodas = InstanteActual;
    EstadoRecirculacionT = RECIRCULANDO;
  
		return 0;

	} else if ((EstadoRecirculacionT == RECIRCULANDO) && (InstanteActual - InstanteInicioRecirculacionTodas >= T_RECIRCULACION_TOTAL)) {
		DepositoDescargas.cerrar_valvula_llenado();
		DepositoDescargas.cerrar_valvula_vaciado();		
		DepositoVentas1.cerrar_valvula_llenado();
		DepositoVentas1.cerrar_valvula_vaciado();
		DepositoVentas2.cerrar_valvula_llenado();
		DepositoVentas2.cerrar_valvula_vaciado();
		DepositoFugas.cerrar_valvula_llenado();
		DepositoFugas.cerrar_valvula_vaciado();
		return 1;

	}

}


////////////////////////////////////////////////////////////////////////////////////


void RecircularDepositosPulsacion(){

  DebugGeneral.EnviarMensajeDebug("Recirculando por pulsacion", InstanteActual, EstadoActual);
	
	if ((EstadoPulsadorBombeo == LOW) && (FlancoPulsadorStart == 0) && (FlancoPulsadorStop == 0)) {
		DepositoDescargas.cerrar_valvula_llenado();
		DepositoDescargas.cerrar_valvula_vaciado();		
		DepositoVentas1.cerrar_valvula_llenado();
		DepositoVentas1.cerrar_valvula_vaciado();
		DepositoVentas2.cerrar_valvula_llenado();
		DepositoVentas2.cerrar_valvula_vaciado();
		DepositoFugas.cerrar_valvula_llenado();
		DepositoFugas.cerrar_valvula_vaciado();
		
    InstanteIncioBombeoFuga = 0;    //Para luego iniciar la peristáltica antes que la salida de fugas
	}



	if ((EstadoPulsadorBombeo == LOW) && (FlancoPulsadorStart == 1)) {
		Selector++;

		if (Selector == 3) {      
			Selector = 0;
		}
		
	}


	if ((EstadoPulsadorBombeo == HIGH) && (FlancoPulsadorStart == 0) && (FlancoPulsadorStop == 0)) {  
		switch (Selector){                  
			case 0: //Recircular Ventas 1
        DepositoAlmacenamiento.abrir_valvula_llenado();
        DepositoDescargas.abrir_valvula_llenado();

        DepositoVentas1.abrir_valvula_llenado();  
        DepositoVentas1.abrir_valvula_vaciado();
        break;

			case 1: //Recircular Ventas 2
        DepositoVentas2.abrir_valvula_llenado();
        DepositoVentas2.abrir_valvula_vaciado();

        DepositoAlmacenamiento.abrir_valvula_llenado();
        DepositoDescargas.abrir_valvula_llenado();
        break;

			case 2: //Recircular Fugas
        DepositoFugas.abrir_valvula_llenado();  //Se abre la de llenado siempre, pero la de vaciado sólo cuando hayan pasado x segundos con la de llenado abierta, ya que se vacía mucho más rápido de lo que se llena
        
        if (InstanteIncioBombeoFuga == 0){
          InstanteIncioBombeoFuga = InstanteActual;  
        }

        if (InstanteActual - InstanteIncioBombeoFuga >= T_MARGEN_RECIRCULACION_MANUAL){    //Lapso de tiempo para que llegue a por encima de la válvula de salida
          DepositoFugas.abrir_valvula_vaciado();
        }

        break;

		}
		
	}

}







/////////////////////////////////////////////////////////////////////////////////////









bool CalibrarDepositos(){

	bool FinCalibracion = 0;

	if (EstadoCalibracionNivel == CALIBRACION_NIVEL_3){       
		CalibrarNivel3();

	} else if (EstadoCalibracionNivel == CALIBRACION_NIVEL_2) {
		CalibrarNivel2();

	} else if (EstadoCalibracionNivel == CALIBRACION_NIVEL_1) {
		FinCalibracion = CalibrarNivel1();

	}

	return FinCalibracion;   //Sólo devuelve 1 si acaba de pesar el volumen principal de nivel 1 

}


/////////////////////////////////////////////////////////////////////////////////////



bool EstablecerCondInic(){
	bool FinCondIniciales = 0;

		if (EstadoCondInicialesSubetapa == ESTABLECIENDO_MARGENES){
			CondInicialesEstablecerMargenes();

		} else if (EstadoCondInicialesSubetapa == NIVEL_ALMACENAMIENTO){
			CondInicialesEstablecerNivelAlmacenamiento();
						
		} else if (EstadoCondInicialesSubetapa == NIVEL_DESCARGAS){
			FinCondIniciales = CondInicialesEstablecerNivelDescargas();
			
		}
	
		return FinCondIniciales;
}




/////////////////////////////////////////////////////////////////////////////////////
//Funciones calibracion
/////////////////////////////////////////////////////////////////////////////////////

void CalibrarNivel3(){

  DebugGeneral.EnviarMensajeDebug("Calibrando Nivel 3", InstanteActual, EstadoActual);

	if (EstadoCalibracionNivelSubetapa == LLENANDO_DESCARGAS){
		DepositoDescargas.abrir_valvula_llenado();	

		if (DescargasSensorNivel3){
			DepositoDescargas.cerrar_valvula_llenado();
      DepositoDescargas.abrir_valvula_vaciado();
			EstadoCalibracionNivelSubetapa = VACIANDO_DESCARGAS;
		}	

	}	


	if ((EstadoCalibracionNivelSubetapa == VACIANDO_DESCARGAS) && (!DescargasSensorNivel0)){
		DepositoDescargas.cerrar_valvula_vaciado();
    InstanteInicioVaciadoDescargas = InstanteActual;
		EstadoCalibracionNivelSubetapa = PESANDO_ALMACENAMIENTO;
	}


	if ((EstadoCalibracionNivelSubetapa == PESANDO_ALMACENAMIENTO) && ((InstanteActual - InstanteInicioVaciadoDescargas) > T_MARGEN_PESANDO_CALIBRACION)){
		
    DepositoDescargas.set_VolumenDescarga(SENSOR_NIVEL_3, DepositoAlmacenamiento.get_volumen()); //guardar valor de báscula Ppal al nivel correspondiente
    DepositoAlmacenamiento.abrir_valvula_vaciado(); 
    DepositoVentas1.abrir_valvula_vaciado();   
    EstadoCalibracionNivelSubetapa = VACIANDO_ALMACENAMIENTO;
	}


	if ((EstadoCalibracionNivelSubetapa == VACIANDO_ALMACENAMIENTO) && (DepositoAlmacenamiento.get_volumen() <= 0)){
		DepositoAlmacenamiento.cerrar_valvula_vaciado();
    DepositoVentas1.cerrar_valvula_vaciado();
		EstadoCalibracionNivel = CALIBRACION_NIVEL_2;
		EstadoCalibracionNivelSubetapa = LLENANDO_DESCARGAS;   //Con estas dos últimas asignaciones nos aseguramos de que en el siguiente ciclo de
																											//la Arduino, se mete en calibración nivel 3, y lo hace todo exactamente igual que en
																											//esta sección (la única precaución es al final del nivel 2, no vaciar el Principal)
	}

}


/////////////////////////////////////////////////////////////////////////////////////


void CalibrarNivel2(){

  DebugGeneral.EnviarMensajeDebug("Calibrando Nivel 2", InstanteActual, EstadoActual);

	if (EstadoCalibracionNivelSubetapa == LLENANDO_DESCARGAS){
		DepositoDescargas.abrir_valvula_llenado();	

		if (DescargasSensorNivel2){
			DepositoDescargas.cerrar_valvula_llenado();
      DepositoDescargas.abrir_valvula_vaciado();
			EstadoCalibracionNivelSubetapa = VACIANDO_DESCARGAS;
		}	

	}	


	if ((EstadoCalibracionNivelSubetapa == VACIANDO_DESCARGAS) && (!DescargasSensorNivel1)){
		DepositoDescargas.cerrar_valvula_vaciado();
    InstanteInicioVaciadoDescargas = InstanteActual;
		EstadoCalibracionNivelSubetapa = PESANDO_ALMACENAMIENTO;
	}


	if ((EstadoCalibracionNivelSubetapa == PESANDO_ALMACENAMIENTO) && ((InstanteActual - InstanteInicioVaciadoDescargas) > T_MARGEN_PESANDO_CALIBRACION)){
		DepositoDescargas.set_VolumenDescarga(SENSOR_NIVEL_2, DepositoAlmacenamiento.get_volumen()); 
    DepositoAlmacenamiento.abrir_valvula_vaciado(); 
    DepositoVentas1.abrir_valvula_vaciado();  
    EstadoCalibracionNivelSubetapa = VACIANDO_ALMACENAMIENTO;
	}


	if ((EstadoCalibracionNivelSubetapa == VACIANDO_ALMACENAMIENTO) && (DepositoAlmacenamiento.get_volumen() <= 0)){
		DepositoAlmacenamiento.cerrar_valvula_vaciado();
    DepositoVentas1.cerrar_valvula_vaciado();
		EstadoCalibracionNivel = CALIBRACION_NIVEL_1;
		EstadoCalibracionNivelSubetapa = LLENANDO_DESCARGAS;   //Con estas dos últimas asignaciones nos aseguramos de que en el siguiente ciclo de
																											//la Arduino, se mete en calibración nivel 3, y lo hace todo exactamente igual que en
																											//esta sección (la única precaución es al final del nivel 2, no vaciar el Principal)
	}

}


/////////////////////////////////////////////////////////////////////////////////////


bool CalibrarNivel1(){

  DebugGeneral.EnviarMensajeDebug("Calibrando Nivel 1", InstanteActual, EstadoActual);

	if (EstadoCalibracionNivelSubetapa == LLENANDO_DESCARGAS){
		DepositoDescargas.abrir_valvula_llenado();	

		if (DescargasSensorNivel1){
			DepositoDescargas.cerrar_valvula_llenado();
      DepositoDescargas.abrir_valvula_vaciado();
			EstadoCalibracionNivelSubetapa = VACIANDO_DESCARGAS;
		}	

    return 0;
	}	


	if ((EstadoCalibracionNivelSubetapa == VACIANDO_DESCARGAS) && (!DescargasSensorNivel0)){
		DepositoDescargas.cerrar_valvula_vaciado();
    InstanteInicioVaciadoDescargas = InstanteActual;
		EstadoCalibracionNivelSubetapa = PESANDO_ALMACENAMIENTO;
    return 0; 
	}


	if ((EstadoCalibracionNivelSubetapa == PESANDO_ALMACENAMIENTO) && ((InstanteActual - InstanteInicioVaciadoDescargas) > T_MARGEN_PESANDO_CALIBRACION)){
		DepositoDescargas.set_VolumenDescarga(SENSOR_NIVEL_1, DepositoAlmacenamiento.get_volumen()); //guardar valor de báscula Ppal al nivel correspondiente
    //(Nos saltamos toda la parte de vaciar el principal y pasamos directamente a la siguente etapa de la inicializacion)
    return 1; 
	}

}





/////////////////////////////////////////////////////////////////////////////////////
//Funciones cond iniciales
/////////////////////////////////////////////////////////////////////////////////////



void CondInicialesEstablecerMargenes(){

  DebugGeneral.EnviarMensajeDebug("Estableciendo margenes", InstanteActual, EstadoActual);

	if (EstadoMargenes == ABRIENDO_DESCARGAS){
		DepositoDescargas.abrir_valvula_llenado();
    DepositoDescargas.abrir_valvula_vaciado();
		EstadoMargenes = LLENANDO_PRINCIPAL;

	} else if ((EstadoMargenes == LLENANDO_PRINCIPAL) && (DepositoAlmacenamiento.get_volumen() >= VOL_MARGEN_TOTAL)){
		DepositoDescargas.cerrar_valvula_llenado();
    DepositoDescargas.cerrar_valvula_vaciado();

		DepositoVentas1.abrir_valvula_llenado();
    DepositoVentas2.abrir_valvula_llenado();
    DepositoFugas.abrir_valvula_llenado();
		EstadoMargenes = DEPOSITANDO_MARGENES;

	} else if (EstadoMargenes == DEPOSITANDO_MARGENES){
		if (DepositoVentas1.get_volumen() >=  VOL_MARGEN_VENTAS1){
			DepositoVentas1.cerrar_valvula_llenado();
			Ventas1MargenListo = 1;  
		}
		

		if (DepositoVentas2.get_volumen() >=  VOL_MARGEN_VENTAS2){
			DepositoVentas2.cerrar_valvula_llenado();
			Ventas2MargenListo = 1;  
		}


		if (DepositoFugas.get_volumen() >=  VOL_MARGEN_FUGAS){
			DepositoFugas.cerrar_valvula_llenado();
			FugasMargenListo = 1;  
		}


		if (Ventas1MargenListo && Ventas2MargenListo && FugasMargenListo){

      //Aquí hacer otra tara antes de pasar a la siguiente subetapa 
      bool TaraAlmacenamiento = DepositoAlmacenamiento.realizar_tara(); 
      bool TaraVentas1 = DepositoVentas1.realizar_tara();
      bool TaraVentas2 = DepositoVentas2.realizar_tara();
      bool TaraFugas = DepositoFugas.realizar_tara();    
      // Una vez realizadas las taras de los depositos, entramos en funcionamiento normal // 
      if (TaraVentas1 && TaraVentas2 && TaraAlmacenamiento && TaraFugas) {   
        ActualizarEstadoGeneral(CALIBRACION);
        DebugGeneral.EnviarMensajeDebug("Tara 2 realizada", InstanteActual, EstadoActual);
        DebugGeneral.EnviarMensajeDebug("Pasando a vertir nivel aleatorio de Almacenamiento", InstanteActual, EstadoActual);
      }
      
      
			EstadoCondInicialesSubetapa = NIVEL_ALMACENAMIENTO;
		}
		
	}

}


/////////////////////////////////////////////////////////////////////////////////////


void CondInicialesEstablecerNivelAlmacenamiento(){

  DebugGeneral.EnviarMensajeDebug("Estableciendo nivel aleatorio principal", InstanteActual, EstadoActual);

	if (EstadoNivelAlmacenamiento == ASIGNANDO_NIVEL_ALEATORIO) {
		VolumenAleatorioAlmacenamiento = 0.01 * random(25, 75) * DepositoAlmacenamiento.get_VolMaxSeguridad();    
		DepositoDescargas.abrir_valvula_llenado();
	  DepositoDescargas.abrir_valvula_vaciado();
		EstadoNivelAlmacenamiento = DEPOSITANDO_NIVEL_ALEATORIO;     
	
	} else if ((EstadoNivelAlmacenamiento == DEPOSITANDO_NIVEL_ALEATORIO) && (DepositoAlmacenamiento.get_volumen() >= VolumenAleatorioAlmacenamiento)){
		DepositoDescargas.cerrar_valvula_llenado();
	  DepositoDescargas.cerrar_valvula_vaciado();
		EstadoCondInicialesSubetapa = NIVEL_DESCARGAS;
	}

}



/////////////////////////////////////////////////////////////////////////////////////






bool CondInicialesEstablecerNivelDescargas(){

  DebugGeneral.EnviarMensajeDebug("Estableciendo nivel aleatorio en Descargas", InstanteActual, EstadoActual);

	if (EstadoNivelDescargas == ASIGNANDO_NIVEL_ALEATORIO) {
		VolumenAleatorioDescargas = random(2, 4);          
    DepositoDescargas.abrir_valvula_llenado();
		EstadoNivelDescargas = DEPOSITANDO_NIVEL_ALEATORIO;

	} else if (EstadoNivelDescargas == DEPOSITANDO_NIVEL_ALEATORIO){
		switch (VolumenAleatorioDescargas){
      case 1:
        if (DescargasSensorNivel1) {
          DepositoDescargas.cerrar_valvula_llenado();         
					return 1;
        }
        break;

      case 2:
        if (DescargasSensorNivel2) {
          DepositoDescargas.cerrar_valvula_llenado();
					return 1;
        }
        break;

      case 3:
        if (DescargasSensorNivel3) {
          DepositoDescargas.cerrar_valvula_llenado();
					return 1;
        }
        break;
    }
	}
	
}





/////////////////////////////////////////////////////////////////////////////////////



void ActualizarLecturaSensores(){
  DescargasSensorNivel0 = digitalRead(PIN_DESCARGAS_SENSOR_NIVEL_0);
  DescargasSensorNivel1 = digitalRead(PIN_DESCARGAS_SENSOR_NIVEL_1);
  DescargasSensorNivel2 = digitalRead(PIN_DESCARGAS_SENSOR_NIVEL_2);
  DescargasSensorNivel3 = digitalRead(PIN_DESCARGAS_SENSOR_NIVEL_3);

}




/////////////////////////////////////////////////////////////////////////////////////



void ActualizarEntradasPanel(){

	//Comprobación de todos los inputs, ya sea el flanco o el valor actual

  EstadoInterruptorVaciadoTiempo = InterruptorVaciadoTiempo.ComprobarEstado();
  FlancoInterruptorVaciadoTiempo = InterruptorVaciadoTiempo.ComprobarFlanco(InstanteActual);

  EstadoInterruptorVaciadoPulsacion = InterruptorVaciadoPulsacion.ComprobarEstado();
  FlancoInterruptorVaciadoPulsacion = InterruptorVaciadoPulsacion.ComprobarFlanco(InstanteActual);

  EstadoInterruptorRecirculacionTiempo = InterruptorRecirculacionTiempo.ComprobarEstado();
  FlancoInterruptorRecirculacionTiempo = InterruptorRecirculacionTiempo.ComprobarFlanco(InstanteActual);

  EstadoInterruptorRecirculacionPulsacion = InterruptorRecirculacionPulsacion.ComprobarEstado();
  FlancoInterruptorRecirculacionPulsacion = InterruptorRecirculacionPulsacion.ComprobarFlanco(InstanteActual);

  EstadoInterruptorTara = InterruptorTara.ComprobarEstado();
  FlancoInterruptorTara = InterruptorTara.ComprobarFlanco(InstanteActual);

  EstadoInterruptorCalibracion = InterruptorCalibracion.ComprobarEstado();
  FlancoInterruptorCalibracion = InterruptorCalibracion.ComprobarFlanco(InstanteActual);

  EstadoInterruptorCondIniciales = InterruptorCondIniciales.ComprobarEstado();
  FlancoInterruptorCondIniciales = InterruptorCondIniciales.ComprobarFlanco(InstanteActual);

  EstadoInterruptorCierre = InterruptorCierre.ComprobarEstado();
  FlancoInterruptorCierre = InterruptorCierre.ComprobarFlanco(InstanteActual);

  EstadoPulsadorBombeo = PulsadorBombeo.ComprobarEstado();
  FlancoPulsadorBombeo = PulsadorBombeo.ComprobarFlanco(InstanteActual);

  EstadoPulsadorStart = PulsadorStart.ComprobarEstado();
  FlancoPulsadorStart = PulsadorStart.ComprobarFlanco(InstanteActual);

  EstadoPulsadorStop = PulsadorStop.ComprobarEstado();
  FlancoPulsadorStop = PulsadorStop.ComprobarFlanco(InstanteActual);

  EstadoInterruptorManual = InterruptorManual.ComprobarEstado();
  FlancoInterruptorManual = InterruptorManual.ComprobarFlanco(InstanteActual);

  //EstadoInterruptorEmergencia = InterruptorEmergencia.ComprobarEstado();
  FlancoInterruptorEmergencia = InterruptorEmergencia.ComprobarFlanco(InstanteActual);

  LecturaPotenciometroAltura = PotenciometroAltura.LeerValor();

  LecturaPotenciometroDimensiones = PotenciometroDimensiones.LeerValor();

  //Además, para tener en cuenta la emergencia del Dashboard:
  if (InterruptorEmergencia.ComprobarEstado() == 1 || EmergenciaDashboard() == 1){
    EstadoInterruptorEmergencia = 1;
  } else {
    EstadoInterruptorEmergencia = 0;
  }
	
}




/////////////////////////////////////////////////////////////////////////////////////

int EmergenciaDashboard() {
    while (Serial.available()) {
        char caracter = (char)Serial.read(); 
        EntradaSerial += caracter;
        if (caracter == '\n') {
            DatosNuevosSerial = true;
        }
    }
    if (DatosNuevosSerial && EntradaSerial.startsWith("Emergencia_Dash:")) {
        EntradaSerial.trim(); // Elimina espacios innecesarios
        int estado = EntradaSerial.charAt(EntradaSerial.length()-1) - '0'; // Se obtiene el último caracter y conviértelo a int
        EntradaSerial = ""; // Limpia la entrada para la próxima lectura
        DatosNuevosSerial = false;
        return estado; // Devuelve el estado de la emergencia
    }
    return 0; // Devuelve 0 si no hay datos nuevos o si la entrada no empieza con "Emergencia_Dash:"
}






/////////////////////////////////////////////////////////////////////////////////////





void EnviarDatosDashboard() {

                                     //python index
  Serial.print(InstanteActual);                  //0
  Serial.print(",");

  Serial.print(DepositoDescargas.get_volumen()*1000);                  //1
  Serial.print(",");
  Serial.print(DepositoAlmacenamiento.get_volumen()*1000);                  //2
  Serial.print(",");
  Serial.print(DepositoVentas1.get_volumen()*1000);                  //3
  Serial.print(",");
  Serial.print(DepositoVentas2.get_volumen()*1000);                  //4
  Serial.print(",");
  Serial.print(DepositoFugas.get_volumen()*1000);                  //5
  Serial.print(",");

  Serial.print(DepositoDescargas.get_ProxVolumen()*1000);     //6
  Serial.print(",");
  Serial.print(DepositoVentas1.get_ProxVolumen()*1000);            //7
  Serial.print(",");
  Serial.print(DepositoVentas2.get_ProxVolumen()*1000);            //8
  Serial.print(",");

  Serial.print(DepositoDescargas.get_VolumenAcumulado()*1000);     //9
  Serial.print(",");
  Serial.print(DepositoVentas1.get_VolumenAcumulado()*1000);      //10
  Serial.print(",");
  Serial.print(DepositoVentas2.get_VolumenAcumulado()*1000);      //11
  Serial.print(",");
  Serial.print(DepositoFugas.get_VolumenAcumulado()*1000);      //12
  Serial.print(",");

  Serial.print(DepositoDescargas.get_InstanteProxDisp());       //13
  Serial.print(",");
  Serial.print(DepositoVentas1.get_InstanteProxDisp());       //14
  Serial.print(",");
  Serial.print(DepositoVentas2.get_InstanteProxDisp());       //15
  Serial.print(",");

  Serial.print(DepositoDescargas.get_RecargasTotales());          //16
  Serial.print(",");
  Serial.print(DepositoVentas1.get_RecargasTotales());          //17
  Serial.print(",");
  Serial.print(DepositoVentas2.get_RecargasTotales());        //18
  Serial.print(",");

  Serial.print(DepositoFugas.get_AlturaFuga()*1000);          //19
  Serial.print(",");
  Serial.println((DepositoFugas.get_CaudalFuga() / PWM_MAX) * 100);          //20


}






/////////////////////////////////////////////////////////////////////////////////////




void ActualizarSalidasPanel() {

  //Gestión de leds de estados

  if (EstadoActual == IDLE_VACIADO || EstadoActual == VACIADO || EstadoActual == RECIRCULACION || EstadoActual == TARA || EstadoActual == CALIBRACION || EstadoActual == COND_INICIALES || EstadoActual == IDLE_FUNC_NORMAL) {
    LedInicializacion.Encender();
    LedFuncionamientoNormal.Apagar();
    LedPausa.Apagar();
    LedEmergencia.Apagar();

  } else if (EstadoActual == FUNC_NORMAL) {
    LedInicializacion.Apagar();
    LedFuncionamientoNormal.Encender();
    LedPausa.Apagar();
    LedEmergencia.Apagar();

  } else if (EstadoActual == PAUSA || EstadoActual == FIN_EXPERIMENTO) {   
    LedInicializacion.Apagar();
    LedFuncionamientoNormal.Apagar();
    LedPausa.Encender();
    LedEmergencia.Apagar();

  } else if (EstadoActual == EMERGENCIA) {
    LedInicializacion.Apagar();
    LedFuncionamientoNormal.Apagar();
    LedPausa.Apagar();
    LedEmergencia.Parpadear(PARPADEO_RAPIDO, InstanteActual);

  } else {
    LedInicializacion.Apagar();
    LedFuncionamientoNormal.Apagar();
    LedPausa.Apagar();
    LedEmergencia.Apagar();
  }



  //Gestión de leds de depositos    (IMPORTANTE: Los leds de depositos deben estar conectados a pines analógicos, los de estado pueden estar en digitales)


  if (EstadoActual == IDLE_VACIADO || EstadoActual == TARA || EstadoActual == IDLE_FUNC_NORMAL) {
    //Sólo parpadear lento
    LedDescargas.Parpadear(PARPADEO_LENTO, InstanteActual);
    LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);
    LedVentas1.Parpadear(PARPADEO_LENTO, InstanteActual);
    LedVentas2.Parpadear(PARPADEO_LENTO, InstanteActual);
    LedFugas.Parpadear(PARPADEO_LENTO, InstanteActual);



  } else if (EstadoActual == VACIADO) {     

    if (EstadoInterruptorVaciadoTiempo){
      if (EstadoVaciadoT == VACIADO_ABRIENDO_SUPERIORES){
        LedDescargas.Apagar();
        LedAlmacenamiento.Apagar();
        LedVentas1.Encender();
        LedVentas2.Encender();
        LedFugas.Encender();

      } else if (EstadoVaciadoT == VACIANDO_TODOS) {
        LedDescargas.Encender();
        LedAlmacenamiento.Encender();
        LedVentas1.Encender();
        LedVentas2.Encender();
        LedFugas.Encender();
      }

    } else if (EstadoInterruptorVaciadoPulsacion) {
      switch (Selector){
        case 0:
          LedDescargas.Apagar();
          LedAlmacenamiento.Apagar();
          LedVentas2.Apagar();
          LedFugas.Apagar();

          if (EstadoPulsadorBombeo  == HIGH){
            LedVentas1.Encender();
          } else {
            LedVentas1.Parpadear(PARPADEO_LENTO, InstanteActual);
          }

          break;
        
        case 1:
          LedDescargas.Apagar();
          LedAlmacenamiento.Apagar();
          LedVentas1.Apagar();
          LedFugas.Apagar();

          if (EstadoPulsadorBombeo  == HIGH){
            LedVentas2.Encender();
          } else {
            LedVentas2.Parpadear(PARPADEO_LENTO, InstanteActual);
          }

          break;
        
        case 2:
          LedDescargas.Apagar();
          LedAlmacenamiento.Apagar();
          LedVentas1.Apagar();
          LedVentas2.Apagar();
          
          if (EstadoPulsadorBombeo  == HIGH){
            LedFugas.Encender();
          } else {
            LedFugas.Parpadear(PARPADEO_LENTO, InstanteActual);
          }
          
          break;

        case 3:
          LedDescargas.Apagar();
          LedVentas1.Apagar();
          LedVentas2.Apagar();
          LedFugas.Apagar();
          
          if (EstadoPulsadorBombeo  == HIGH){
            LedAlmacenamiento.Encender();
          } else {
            LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);
          }
          
          break;

        case 4:
          LedAlmacenamiento.Apagar();
          LedVentas1.Apagar();
          LedVentas2.Apagar();
          LedFugas.Apagar();
          
          if (EstadoPulsadorBombeo  == HIGH){
            LedDescargas.Encender();
          } else {
            LedDescargas.Parpadear(PARPADEO_LENTO, InstanteActual);
          }
          
          break;

      }
    }



  } else if (EstadoActual == RECIRCULACION) {

    if (EstadoInterruptorRecirculacionTiempo){
      if (EstadoRecirculacionT == RECIR_ABRIENDO_VALVULAS_2){
        LedDescargas.Encender();
        LedAlmacenamiento.Encender();
        LedVentas1.Encender();
        LedVentas2.Encender();
        LedFugas.Parpadear(PARPADEO_LENTO, InstanteActual);

      } else if (EstadoRecirculacionT == RECIRCULANDO) {
        LedDescargas.Encender();
        LedAlmacenamiento.Encender();
        LedVentas1.Encender();
        LedVentas2.Encender();
        LedFugas.Encender();
      }

    } else if (EstadoInterruptorRecirculacionPulsacion) {
      switch (Selector){
        case 0:
          LedVentas2.Apagar();
          LedFugas.Apagar();

          if (EstadoPulsadorBombeo == HIGH){
            LedVentas1.Encender();
            LedDescargas.Encender();
            LedAlmacenamiento.Encender();
          } else {
            LedVentas1.Parpadear(PARPADEO_LENTO, InstanteActual);
            LedDescargas.Parpadear(PARPADEO_LENTO, InstanteActual);
            LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);
          }

          break;
        
        case 1:
          LedVentas1.Apagar();
          LedFugas.Apagar();

          if (EstadoPulsadorBombeo  == HIGH){
            LedVentas2.Encender();
            LedDescargas.Encender();
            LedAlmacenamiento.Encender();
          } else {
            LedVentas2.Parpadear(PARPADEO_LENTO, InstanteActual);
            LedDescargas.Parpadear(PARPADEO_LENTO, InstanteActual);
            LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);
          }

          break;
        
        case 2:
          LedVentas1.Apagar();
          LedVentas2.Apagar();

          if (EstadoPulsadorBombeo  == HIGH){
            LedFugas.Encender();
            LedDescargas.Encender();
            LedAlmacenamiento.Encender();
          } else {
            LedFugas.Parpadear(PARPADEO_LENTO, InstanteActual);
            LedDescargas.Parpadear(PARPADEO_LENTO, InstanteActual);
            LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);
          }
          
          break;

      }
    }



  } else if (EstadoActual == CALIBRACION) {
    
    if (EstadoCalibracionNivelSubetapa == LLENANDO_DESCARGAS) {
      LedDescargas.Encender();
      LedAlmacenamiento.Apagar();
      LedVentas1.Apagar();
      LedVentas2.Apagar();
      LedFugas.Apagar();

    } else if (EstadoCalibracionNivelSubetapa == VACIANDO_DESCARGAS){
      LedDescargas.Parpadear(PARPADEO_LENTO, InstanteActual);
      LedAlmacenamiento.Encender();
      LedVentas1.Apagar();
      LedVentas2.Apagar();
      LedFugas.Apagar();

    } else if (EstadoCalibracionNivelSubetapa == PESANDO_ALMACENAMIENTO) {
      LedDescargas.Apagar();
      LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);
      LedVentas1.Apagar();
      LedVentas2.Apagar();
      LedFugas.Apagar();

    } else if (EstadoCalibracionNivelSubetapa == VACIANDO_ALMACENAMIENTO) {
      LedDescargas.Apagar();
      LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);
      LedVentas1.Parpadear(PARPADEO_LENTO, InstanteActual);
      LedVentas2.Apagar();
      LedFugas.Apagar();

    }


  } else if (EstadoActual == COND_INICIALES) {
    if (EstadoCondInicialesSubetapa == ESTABLECIENDO_MARGENES){
      if (EstadoMargenes == LLENANDO_PRINCIPAL){
        LedDescargas.Apagar();
        LedAlmacenamiento.Encender();
        LedVentas1.Apagar();
        LedVentas2.Apagar();
        LedFugas.Apagar();

      } else if (EstadoMargenes == DEPOSITANDO_MARGENES){
        LedDescargas.Apagar();
        LedAlmacenamiento.Parpadear(PARPADEO_LENTO, InstanteActual);

        if (!Ventas1MargenListo){
          LedVentas1.Encender();
        } else {
          LedVentas1.Apagar();
        }

        if (!Ventas2MargenListo){
          LedVentas2.Encender();
        } else {
          LedVentas2.Apagar();
        }

        if (!FugasMargenListo){
          LedFugas.Encender();
        } else {
          LedFugas.Apagar();
        }

      }

    } else if (EstadoCondInicialesSubetapa == NIVEL_ALMACENAMIENTO) {
      LedDescargas.Parpadear(PARPADEO_LENTO, InstanteActual);
      LedAlmacenamiento.Encender();
      LedVentas1.Apagar();
      LedVentas2.Apagar();
      LedFugas.Apagar();


    } else if (EstadoCondInicialesSubetapa == NIVEL_DESCARGAS) {
      LedDescargas.Encender();
      LedAlmacenamiento.Apagar();
      LedVentas1.Apagar();
      LedVentas2.Apagar();
      LedFugas.Apagar();
    }


  } else if (EstadoActual == FUNC_NORMAL || EstadoActual == PAUSA || EstadoActual == FIN_EXPERIMENTO || EstadoActual == EMERGENCIA) {   
    //Aquí indicar con valores analógicos el nivel de cada depósito con respecto a su máximo:
    //0% brillo:           Depósito entre el  0% y el 10% de capacidad
    //15% brillo:          Depóstio entre el 10% y el 30% de capacidad
    //50% brillo:          Depósito entre el 30% y el 60% de capacidad
    //75% brillo:          Depósito entre el 60% y el 75% de capacidad
    //100% brillo:         Depósito entre el 75% y el 90% de capacidad
    //Parpadeando rápido:  Depósito a más del 90% de capacidad (desbordamiento o desbordamiento inminente)
    
    LedDescargas.EncenderPorNivel(DepositoDescargas.get_volumen(), DepositoDescargas.get_VolMaxSeguridad(), InstanteActual); 
    LedAlmacenamiento.EncenderPorNivel(DepositoAlmacenamiento.get_volumen(), DepositoAlmacenamiento.get_VolMaxSeguridad(), InstanteActual);
    LedVentas1.EncenderPorNivel(DepositoVentas1.get_volumen(), DepositoVentas1.get_VolMaxSeguridad(), InstanteActual);
    LedVentas2.EncenderPorNivel(DepositoVentas2.get_volumen(), DepositoVentas2.get_VolMaxSeguridad(), InstanteActual);
    LedFugas.EncenderPorNivel(DepositoFugas.get_volumen(), DepositoFugas.get_VolMaxSeguridad(), InstanteActual);
    

  } else {    //Para indicar que no se entró en ningún estado de los anteriores, a modo de código de error, se muestra un parpadeo específico
    LedDescargas.Parpadear(PARPADEO_ERROR, InstanteActual);
    LedAlmacenamiento.Parpadear(PARPADEO_ERROR, InstanteActual);
    LedVentas1.Parpadear(PARPADEO_ERROR, InstanteActual);
    LedVentas2.Parpadear(PARPADEO_ERROR, InstanteActual);
    LedFugas.Parpadear(PARPADEO_ERROR, InstanteActual);

  }

}





/////////////////////////////////////////////////////////////////////////////////////





void InicializacionVariables(){

  TiempoTotalPausado = 0;

  InstanteDatalogger = 0;

  emergencia_recarga = 0; 
  emergencia_disp_1 = 0; 
  emergencia_disp_2 = 0; 
  emergencia_fugas = 0;

  ConjuntoListoParaReiniciar = 0;

  EstadoActual = IDLE_VACIADO;

  DepositosVaciados = 0;
  Selector = 0;
  DepositosRecirculados = 0;
  DepositosCalibrados = 0;
  CondInicEstablecidas = 0;

  EstadoVaciadoT = VACIADO_ABRIENDO_INFERIORES;

  InstanteInicioMargenVaciado = 0;
  InstanteInicioVaciado = 0;
  InstanteComienzoPausa = 0;

  EstadoRecirculacionT = RECIR_ABRIENDO_VALVULAS_1;
  InstanteInicioRecirculacionRapidas = 0;
  InstanteInicioRecirculacionTodas = 0;

  InstanteIncioBombeoFuga = 0;

  InstanteInicioVaciadoDescargas = 0;

  EstadoCalibracionNivel = CALIBRACION_NIVEL_3;
  EstadoCondInicialesSubetapa = ESTABLECIENDO_MARGENES;
  EstadoCalibracionNivelSubetapa = LLENANDO_DESCARGAS;
  EstadoMargenes = ABRIENDO_DESCARGAS;
  Ventas1MargenListo = 0;
  Ventas2MargenListo = 0;
  FugasMargenListo = 0;

  EstadoNivelAlmacenamiento = ASIGNANDO_NIVEL_ALEATORIO;

  VolumenAleatorioAlmacenamiento = 0;

  EstadoNivelDescargas = ASIGNANDO_NIVEL_ALEATORIO;

  VolumenAleatorioDescargas = 0;


}
