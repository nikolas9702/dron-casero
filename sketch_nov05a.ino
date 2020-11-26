#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include <L3G.h>

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
  // motor_esc1.writeMicroseconds(1020);
  /*motor_esc2.writeMicroseconds(1020);
  motor_esc3.writeMicroseconds(1020);*/
  if(Serial.available() > 0)
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
        
      motor_esc1.writeMicroseconds(pwm_motor);
      ////motor_esc2.writeMicroseconds(pwm_motor);
      ///motor_esc3.writeMicroseconds(pwm_motor);
      Serial.print("Valor enviado al controlador de motor = ");
      Serial.println(pwm_motor);
    }
    //Asegura que la próxima lectura y asignación de valor ocurra después de 20 milisegundos, que es el periodo de la señal PWM.
    delay(20);
  } 

  /*gyro.read();

  Serial.print("G ");
  Serial.print("X: ");
  Serial.print((int)gyro.g.x);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);

  delay(100);*/


  /*if ( BT.available() ) {
     Serial.write(BT.read());
  }
  if( Serial.available() ) {
    BT.write(Serial.read());
  }*/
  

  
}
