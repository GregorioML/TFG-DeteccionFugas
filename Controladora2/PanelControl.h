//PanelControl.h

#ifndef PanelControl_h
#define PanelControl_h

#include "Arduino.h"




class PanelControl {
	private:
		int _Pin;
		int _Tipo;
		int _EstadoActual;
		int _EstadoAnterior;
		int _Flanco;
    unsigned long _InstanteActual;
		unsigned long _TiempoRebote;
    unsigned long _InstanteAnteriorRebote;

    int _LecturaActual;

    int _TipoParpadeo;
    bool _ParpadeoEncendido;
    unsigned long _InstanteUltimoCambioParpadeo;
    unsigned long _TiempoEncendidoParpadeo;
    unsigned long _TiempoApagadoParpadeo;
    float _RatioLlenado;



    //const  
    const int ERROR_ENTRADA_PANEL;

    const int TIEMPO_REBOTE_PRED;

    const int PARPADEO_LENTO ;
    const int PARPADEO_RAPIDO;
    const int PARPADEO_ERROR;

    const unsigned long TIEMPO_PARPADEO_RAPIDO;
    const unsigned long TIEMPO_ON_PARPADEO_LENTO;
    const unsigned long TIEMPO_OFF_PARPADEO_LENTO;
    const unsigned long TIEMPO_PARPADEO_ERROR;

    const float LED_UMBRAL_INF_INTERVALO1;
    const float LED_UMBRAL_INF_INTERVALO2;
    const float LED_UMBRAL_INF_INTERVALO3;
    const float LED_UMBRAL_INF_INTERVALO4;
    const float LED_UMBRAL_INF_INTERVALO5;
		


	public:
		PanelControl(int Pin, int Tipo);
		int ComprobarEstado();  //Int porque se devuelvo un 2 en caso de error
		int ComprobarFlanco(unsigned long InstanteActual);
    int LeerValor();
		void Encender();
		void Apagar();
    void Parpadear(int TipoParpadeo, unsigned long InstanteActual);
    void EncenderPorNivel(float VolumenActual, float VolumenMax, unsigned long InstanteActual);

};





#endif