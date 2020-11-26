#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include <L3G.h>
#include <ArduinoJson.h>

Servo motor_esc1;
Servo motor_esc2;
Servo motor_esc3;
L3G gyro;
SoftwareSerial BT(10,11);


//Límites de actuación del conjunto motor controlador, determinados por excursión manual.
const int lim_inf = 1120; // Limite inferior PWM.
const int lim_sup = 1800; // Límite superior PWM.

int rango_pwm = 0;
int pwm_motor = 0;
int potencia_motor_1 = 0;
int potencia_motor_2 = 0;
int potencia_motor_3 = 0;
int potencia_motor_4 = 0;

int PotenciaMotor ( int pwm_motor );
void setup() 
{
  //Inicializa comunicación serial a 9600 baudios.
  Serial.begin(9600);
  /*BT.begin(9600);

  while ( !Serial ) {
    ;
  }

  Serial.println("Configuración.");*/
  
  rango_pwm = lim_sup - lim_inf;
  Serial.print("Rango de valores a ingresar es desde 0 a ");
  Serial.println(rango_pwm);

  //Asigna la variable motor_esc de tipo Servo al pin 9.
  motor_esc1.attach(3);
  motor_esc2.attach(4);
  motor_esc3.attach(5);
  
  //Permite correcta inicialización del controlador de motor ESC.
  motor_esc1.writeMicroseconds(1000); 
  motor_esc2.writeMicroseconds(1000); 
  motor_esc3.writeMicroseconds(1000); 
  delay(4000);


  /*Wire.begin();

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();*/
  
  /*BT.write("AT+NAME=ESCLAVO");
  delay(100);
  BT.write("AT+PSWD=1234");
  delay(100);
  BT.write("AT+ROLE");
  delay(100);*/
  
}

void loop() 
{
  AjustarMotorPotencia ();
  /*if(Serial.available() > 0)
  {
    pwm_motor = Serial.parseInt();
    if (Serial.read() == '\n') // Fin de línea.
    {
      //Restringe la variable pwm_motor a valores desde 0 a rango_pwm.
      pwm_motor = constrain(pwm_motor, 0, rango_pwm);
      
      pwm_motor = pwm_motor + lim_inf;

      //Pregunta por los valores límite y asigna de acuerdo a ello el mínimo o máximo valor posible.
      //Esto es para evitar la respuesta anómala del motor alrededor de los valores límite, específicamente en puntos
      //sobre el límite superior y en puntos bajo el límite inferior.
      if(pwm_motor == lim_inf)
        pwm_motor = 1000;
      if(pwm_motor == lim_sup)
        pwm_motor = 2000;
        
      motor_esc1.writeMicroseconds(pwm_motor-10);
      motor_esc2.writeMicroseconds(pwm_motor);
      motor_esc3.writeMicroseconds(pwm_motor+10);
      Serial.print("Valor enviado al controlador de motor = ");
      Serial.println(pwm_motor);
    }
    //Asegura que la próxima lectura y asignación de valor ocurra después de 20 milisegundos, que es el periodo de la señal PWM.
    delay(20);
  } */



  if ( BT.available() ) {
    StaticJsonDocument<200> doc;
    // char json =  BT.read();
    char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
     
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
  }

  /*if ( BT.available() ) {
     Serial.write(BT.read());
  }
  if( Serial.available() ) {
    BT.write(Serial.read());
  }*/
  

  
}

void AjustarMotorPotencia () {
  gyro.read();
  int x = (int)gyro.g.x;
  if ( x != 0 ) {
    if ( x > 0 ) {
      VariarPotenciaMotor(1 , 10);
      VariarPotenciaMotor(2 , 10);
    }
    else if ( x < 0 ) {
      VariarPotenciaMotor(3 , -10);
      VariarPotenciaMotor(4 , -10);
    }
  }

  /*Serial.print("G ");
  Serial.print("X: ");
  Serial.print((int)gyro.g.x);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);

  delay(100);*/
}
void VariarPotenciaMotor (int Motor , int Value ) {
    switch (Motor) {
      case 1:
        potencia_motor_1 =  PotenciaMotor ( potencia_motor_1 + Value );
        motor_esc1.writeMicroseconds(  potencia_motor_1 );
      break;
      case 2:
        potencia_motor_2 =  PotenciaMotor ( potencia_motor_2 + Value );
        motor_esc1.writeMicroseconds(  potencia_motor_2 );
      break;
      case 3:
        potencia_motor_3 =  PotenciaMotor ( potencia_motor_3 + Value );
        motor_esc1.writeMicroseconds(  potencia_motor_3 );
      break;
      case 4:
        potencia_motor_4 =  PotenciaMotor ( potencia_motor_4 + Value );
        motor_esc1.writeMicroseconds(  potencia_motor_4 );
      break;
    }
  
}

int PotenciaMotor ( int pwm_motor ) {
  return constrain(pwm_motor, 0, rango_pwm) + lim_inf;
}
