//-----------------------------------------------------------------
//          Programa hecho por: Konrad Peschka
//          Entidad: Kaiwa Technology
//          Fecha: Febero 2016
//-----------------------------------------------------------------


//--------------defino librerias y variables para Ultrasonidos------------------


#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

#include <NewPing.h>

#define MAX_DISTANCE 100 // Maximum distance (in cm) to ping.

NewPing ultrasonido_frente(A0,A1, MAX_DISTANCE);

#include <Servo.h>

Servo servo_frente; 

#include <PS2X_lib.h> 

PS2X ps2x; // create PS2 Controller Class

//-----------------------------------------------------------------------
//                Definicion Variables del Programa
//------------------------------------------------------------------

//#define debug                  //comentando o descomentando esta linea activo el codigo para debug

int variable_control;

const int distancia_critica = 25;
const int distancia_deteccion=40;
const int distancia_deteccion_lejana=70;

int velocidad=180;
const int velocidad_maxima=180;
const int velocidad_media=100;
const int velocidad_minima=50;
const int velocidad_giro=180;
const int velocidad_atras=180;

const int tiempo_stop=50;
const int tiempo_giro=100;
const int tiempo_atras=100;
const int tiempo_frente=100;
const int tiempo_servo=90;

const int servo_medio=100;
const int servo_der=50;
const int servo_izq=150;

int distancia_der=0;
int distancia_der_anterior=30;
int distancia_centro=0;
int distancia_centro_anterior=30;
int distancia_izq=0;
int distancia_izq_anterior=30;

int aux_matriz=0;

//-----------------------------------------------------
//                  Funcion SETUP
//-------------------------------------------------------

void setup()
{

 servo_frente.attach(9);
 servo_frente.write(100);
 
#ifdef debug
 Serial.begin(9600);
#endif

  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);

  pinMode(10, INPUT);
 
  movimiento_stop();
  
ps2x.config_gamepad(A2,A4,A3,A5, true, true);   //clock, command, attention, data
delay(100);

}

//--------------------------------------------------------
//              Funcion principal del codigo
//------------------------------------------------------


void loop()
{

      lectura_control();
       if(variable_control==0)  //fue pulsado arriba
         {
          movimiento_stop();
          delay(tiempo_stop);
         }
         
       else
       {
          lectura_ultrasonido();
          if((variable_control==11)&&(distancia_centro <= distancia_critica)&&(distancia_centro!=0))
          {
               movimiento_stop();
               delay(tiempo_stop);
          }

          else
          {
          
             if((variable_control==11)&&((distancia_centro >= distancia_critica)||(distancia_centro==0)))  //fue pulsado arriba
               {
                if((distancia_centro > distancia_deteccion_lejana)||(distancia_centro==0))
                velocidad=velocidad_maxima;
                if((distancia_centro >= distancia_deteccion)&&(distancia_centro <= distancia_deteccion_lejana))
                velocidad=velocidad_media;
                if((distancia_centro < distancia_deteccion)&&(distancia_centro!=0))
                velocidad=velocidad_minima;
            
                movimiento_frente();
                delay(tiempo_frente);
              }
               
            if(variable_control==12)  //fue pulsado abajo
             {
             movimiento_atras();
              delay(tiempo_atras);
             }
             
             if(variable_control==13) //fue pulsado izquierda
             {
                movimiento_izquierda();
                delay(tiempo_giro);
             }
              
             if(variable_control==14)  //fue pulsado derecha
             {
                movimiento_derecha();
                delay(tiempo_giro);
             }

          }
       }
}


//---------------------------------------------------------------------------------------
//                    FUNCION LECTURA ULTRASONIDO
//---------------------------------------------------------------------------------------

void lectura_ultrasonido(void)
{
   servo_frente.write(servo_medio);
   delay(tiempo_servo);
   distancia_centro=ultrasonido_frente.ping_cm();

   #ifdef debug
   Serial.print("Centro= ");
   Serial.print(distancia_centro);
   #endif
  
  
}

//---------------------------------------------------------------------------------------
//                    FUNCION MIRA Y LEE PARA LOS COSTADOS
//---------------------------------------------------------------------------------------

void mira_costados(void)
{
   movimiento_stop();
   delay(tiempo_stop);
    
   servo_frente.write(servo_der);
   delay(tiempo_servo);
   distancia_der=ultrasonido_frente.ping_cm();

   servo_frente.write(servo_medio);
   delay(tiempo_servo);
   
   servo_frente.write(servo_izq);
   delay(tiempo_servo);
   distancia_izq=ultrasonido_frente.ping_cm();

   servo_frente.write(servo_medio);
   delay(tiempo_servo);
   distancia_centro=ultrasonido_frente.ping_cm();

   #ifdef debug
   Serial.print("Der= ");
   Serial.print(distancia_der);
   Serial.print("Izq= ");
   Serial.print(distancia_izq);
   #endif

}

//---------------------------------------------------------------------------------------
//                    FUNCION PARA DECIDIR HACIA QUE COSTADO GIRAR
//---------------------------------------------------------------------------------------

void decide_giro(void)
{
  if(distancia_der>distancia_izq)
  {
    movimiento_derecha();
    delay(tiempo_giro);
  }
  else
  {
    if(distancia_izq>distancia_der)
    {
    movimiento_izquierda();
    delay(tiempo_giro);
    }
  }
}
// -------------------------------------------------------------
//               Definimos librerias de movimiento
//--------------------------------------------------------------

void movimiento_stop(void)
{
 motor1.setSpeed(1);  
 motor2.setSpeed(1); 
 motor3.setSpeed(1); 
 motor4.setSpeed(1); 
}

void movimiento_frente(void)
{

 motor1.setSpeed(velocidad);  
 motor2.setSpeed(velocidad); 
 motor3.setSpeed(velocidad); 
 motor4.setSpeed(velocidad); 

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
}

void movimiento_atras(void)
{
 motor1.setSpeed(velocidad_atras);  
 motor2.setSpeed(velocidad_atras); 
 motor3.setSpeed(velocidad_atras); 
 motor4.setSpeed(velocidad_atras); 

 
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
}

void movimiento_derecha(void)
{
 motor1.setSpeed(velocidad_giro);  
 motor2.setSpeed(velocidad_giro); 
 motor3.setSpeed(velocidad_giro); 
 motor4.setSpeed(velocidad_giro); 

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

}

void movimiento_izquierda(void)
{
 motor1.setSpeed(velocidad_giro);  
 motor2.setSpeed(velocidad_giro); 
 motor3.setSpeed(velocidad_giro); 
 motor4.setSpeed(velocidad_giro); 

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);

}


//-------------------------------------------------------------------------------------
void lectura_control(void)
{
         ps2x.read_gamepad(false, 0);          //read controller and set large motor to spin at 'vibrate' speed 
         
//-------------------------------------lectura de los Botones------------------------------------------------
        variable_control=0;

        if(ps2x.ButtonPressed(PSB_START))   
        {
          #ifdef debug
          Serial.println("Start");
          #endif    
          variable_control=1;     
          return;       
        }
         
        if(ps2x.ButtonPressed(PSB_SELECT))
        {
          #ifdef debug
          Serial.println("Select");
          #endif
          variable_control=2;
          return; 
        }
         

        if(ps2x.ButtonPressed(PSB_L3))
        {
          #ifdef debug
          Serial.println("L3");
          #endif
          variable_control=3;
          return; 
        }
         
         
        if(ps2x.ButtonPressed(PSB_R3))
        {
          #ifdef debug
          Serial.println("R3");
          #endif
          variable_control=4;
          return; 
        }
         
         
        if(ps2x.ButtonPressed(PSB_L2))
        {
          #ifdef debug
          Serial.println("L2");
          #endif
          variable_control=5;
          return; 
        }
         
         
        if(ps2x.Button(PSB_R2))
        {
          #ifdef debug
          Serial.println("R2");
          #endif
          variable_control=6;
          return; 
        }
         
         
         if(ps2x.Button(PSB_GREEN))  //triangulo
         {
          #ifdef debug
          Serial.println("Triangulo");
          #endif
           variable_control=7;
           return; 
         }
         
          
         if(ps2x.Button(PSB_RED))   //circulo
         {
          #ifdef debug
          Serial.println("Circulo");
          #endif
          variable_control=8;
          return; 
         }
         
          
         if(ps2x.Button(PSB_BLUE))   //x
         {
          #ifdef debug
          Serial.println("X");
          #endif
          variable_control=9;
          return; 
         }


         if(ps2x.Button(PSB_PINK))   //cuadrado
         {
          #ifdef debug
          Serial.println("Cuadrado");
          #endif
           variable_control=10;
           return; 
         }
        

        if(ps2x.Button(PSB_PAD_UP))       //boton arriba   
        {
          #ifdef debug
          Serial.println("Arriba");
          #endif   
          variable_control=11;  
          return;     
        }
         
         
        if(ps2x.Button(PSB_PAD_DOWN))     //boton abajo  
        {
          #ifdef debug
          Serial.println("Abajo");
          #endif  
          variable_control=12; 
          return;           
        }
         
         
        if(ps2x.Button(PSB_PAD_LEFT))   //boton izquierda     
        {
          #ifdef debug
          Serial.println("Izquierda");
          #endif  
          variable_control=13;
          return;          
        }
         
         
        if(ps2x.Button(PSB_PAD_RIGHT))     //boton derecha   
        {
          #ifdef debug
          Serial.println("Derecha");
          #endif   
          variable_control=14;  
          return;    
        }

        if(ps2x.ButtonPressed(PSB_L1))
        {
          #ifdef debug
          Serial.println("L1");
          #endif
          variable_control=23;
          return; 
        }
         
         
        if(ps2x.ButtonPressed(PSB_R1))
        {
          #ifdef debug
          Serial.println("R1");
          #endif
          variable_control=24;
          return; 
        }


//------------------------------------Lectura de Los analogicos-------------------------------------------------   
   /*
      #ifdef debug

        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC);

      #endif
        
     if(ps2x.Analog(PSS_LY)>(128+margen_joystick))   //me fijo si joystick Izquierdo para abajo
     {
     variable_control=17;
     return; 
     }
     if(ps2x.Analog(PSS_LY)<(128-margen_joystick))   //me fijo si joystick Izquierdo para arriba
     {
     variable_control=18;
     return; 
     }
     if(ps2x.Analog(PSS_LX)>(128+margen_joystick))   //me fijo si joystick Izquierdo para derecha
     {
     variable_control=15;
     return; 
     }
     if(ps2x.Analog(PSS_LX)<(128-margen_joystick))  //me fijo si joystick Izquierdo para izquierda
     {
     variable_control=16;
     return; 
     }
     if(ps2x.Analog(PSS_RY)>(128+margen_joystick))  //me fijo si joystick derecho para abajo
     {
     variable_control=19;
     return; 
     }
     if(ps2x.Analog(PSS_RY)<(128-margen_joystick))  //me fijo si joystick derecho para arriba
     {
     variable_control=20;
     return; 
     }
     if(ps2x.Analog(PSS_RX)>(128+margen_joystick))  //me fijo si joystick derecho para derecha
     {
     variable_control=21;
     return; 
     }
     if(ps2x.Analog(PSS_RX)<(128-margen_joystick))  //me fijo si joystick derecho para izquierda
     {
     variable_control=22;
     return; 
     }

*/
}

