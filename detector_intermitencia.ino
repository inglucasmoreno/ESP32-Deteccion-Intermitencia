/* 
    ESP32 - Detector de intermitencia
    Autor: Ing. Lucas Omar Moreno
    Año: 2020
*/

// FLAGS
bool lectura_habilitada = true;

// Variables: Retardo Milli()
unsigned long periodo = 500;
unsigned long TiempoAhora = 0;

// Variables: Seteo de tiempo para verificacion de intermitencia
int MULTI_DESBORDE = 2;              // n x 5 - Intervalo de consulta de intermitencia
int contador_multi_desborde = 0;

// Variables: Timer
hw_timer_t * timer = NULL;

// Variable: Cuenta de pulsos de intermitencia
int contador_pulsos_intermitencia = 0;
int PULSOS_INTERMITENCIA = 8;


// Funcion: Cuenta de pulso de intermitencia
void isr(){  
  if(lectura_habilitada){
    Serial.println("Pulso detectado");
    contador_pulsos_intermitencia += 1;
    Serial.println(contador_pulsos_intermitencia);
    lectura_habilitada = false;     
  }  
}

// Funcion: Verificacion de intermitencia
void IRAM_ATTR onTimer() {
  contador_multi_desborde += 1;
  if(contador_multi_desborde >= MULTI_DESBORDE){
    Serial.println("Se verifica intermitencia");
    if(contador_pulsos_intermitencia >= PULSOS_INTERMITENCIA){
      Serial.println("EL SEMAFORO ESTA INTERMITENTE!!");   
    }else{
      Serial.println(" - SEMAFORO FUNCIONANDO NORMALMENTE - ");       
    }
    contador_pulsos_intermitencia = 0;
    contador_multi_desborde = 0;
  }
}

// Funcion: Configuracion de timer
void config_timer() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 5000000, true); // 5 Segundos
  timerAlarmEnable(timer);  
}

// Arduino: Main
void setup() {
  Serial.begin(115200);
  pinMode(27, INPUT); // Interrupcion
  attachInterrupt(27, isr, FALLING);
  config_timer();    
}

// Arduino: Loop
void loop() {
  TiempoAhora = millis();
  while(millis() < TiempoAhora+periodo && lectura_habilitada == false){
    lectura_habilitada = false;
  };
  lectura_habilitada = true; 
}
