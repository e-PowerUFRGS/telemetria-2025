#include <mcp_can.h>
#include <SPI.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SD.h>


#define TYPE_FLOAT sizeof(float)
#define TYPE_INT sizeof(int)
#define TYPE_STRING -1 

//definicoes para o gps
#define GPS_SERIAL Serial1
#define GPS_BAUD 9600
#define POT_PIN A1


//tipos de dados que guardam parametros dos sensores
TinyGPS gps;

union Values{
  int i;
  float f;
  const char *s;
};

struct ValuesToWrite { 
  Values data;
  int type;  // sizeof(int) ou sizeof(float)
};

File myFile;
const int chipSelect = 53;  // Pino CS no Arduino Mega

//coleta os dados de latitude e longitude do gps
void gps_data();

/*
  Função para abrir o SD e configurá-lo, retorna true caso tenha sido feito com sucesso ou false caso não tenha funcionado
*/
static bool initSD(int chipSelect);

/*
  Função para escrever os cabeçalhos quando o arquivo for recem criado
*/
void writeCSVHeaderToSD(const char* filename, const char* headers[], int numColumns);
/*
  Função que escreve uma única linha de dados no formato .csv
*/
void writeCSVLineToSD(const char* filename, ValuesToWrite* list, int vector_size);


/* Set the delay between fresh samples */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 1000;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(54, 0x28, &Wire);


void setup() {
  Serial.begin(115200);

  GPS_SERIAL.begin(GPS_BAUD);
  
  uint8_t status, test_result, error;
  Serial.println("Orientation Sensor Test"); Serial.println("");
  /* Initialise the sensor */
  if (!bno.begin(OPERATION_MODE_NDOF))
  {
    /* There was a problem detecting the BNO055 ... check your connections*/ 
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  bno.getSystemStatus(&status, &test_result, &error);
  Serial.println(status);
  Serial.println(test_result);
  Serial.println(error);
  delay(1000);
  /*
  pinMode(POT_PIN, INPUT);
  while(!initSD(chipSelect)){
    Serial.println("Erro ao acessar SD!");
  }*/
}

//guardam os valores coletados pelos sensores em um loop
float latitude = 1, longitude = 2;

void loop() {
   //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
  

  //printEvent(&orientationData);
  //printEvent(&angVelocityData);
  //printEvent(&linearAccelData);
  //printEvent(&magnetometerData);
  //printEvent(&accelerometerData);
  //printEvent(&gravityData);

  int8_t boardTemp = bno.getTemp();
  Serial.println();
  Serial.print(F("temperature: "));
  Serial.println(boardTemp);

  /*uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  Serial.println();
  Serial.print("Calibration: Sys=");
  Serial.print(system);
  Serial.print(" Gyro=");
  Serial.print(gyro);
  Serial.print(" Accel=");
  Serial.print(accel);
  Serial.print(" Mag=");
  Serial.println(mag);*/

  gps_data();
  Serial.print(analogRead(POT_PIN));
  Serial.print("; ");
  Serial.print(linearAccelData.acceleration.x);
  Serial.print("; ");
  Serial.print(linearAccelData.acceleration.y);
  Serial.print("; ");
  Serial.print(linearAccelData.acceleration.z);
  Serial.print("; ");
  Serial.print(latitude);
  Serial.print("; ");
  Serial.print(longitude);
  Serial.println(" ");
  Serial.println("--");

  ValuesToWrite dados[] = {
      {.data = {.i = analogRead(POT_PIN)}, .type = TYPE_INT},
      {.data = {.f = linearAccelData.acceleration.x}, .type = TYPE_FLOAT},
      {.data = {.f = linearAccelData.acceleration.y}, .type = TYPE_FLOAT},
      {.data = {.f = linearAccelData.acceleration.z}, .type = TYPE_FLOAT}
  };
  
  //writeCSVLineToSD("pasta/teste.txt", dados, 4);

  delay(BNO055_SAMPLERATE_DELAY_MS);
}

void gps_data(){
  while(GPS_SERIAL.available()){
    if(gps.encode(GPS_SERIAL.read())){
      unsigned long age;
      gps.f_get_position(&latitude, &longitude, &age);
    }
  }
}

void printEvent(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    Serial.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    Serial.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    Serial.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    Serial.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    Serial.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    Serial.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    Serial.print("Unk:");
  }

  Serial.print("\tx= ");
  Serial.print(x);
  Serial.print(" |\ty= ");
  Serial.print(y);
  Serial.print(" |\tz= ");
  Serial.println(z);
}

void writeCSVLineToSD(const char* filename, ValuesToWrite* list, int vector_size) {
  File f = SD.open(filename, FILE_WRITE);
  if (f) {
    for (int i = 0; i < vector_size; i++) {
      if (list[i].type == TYPE_INT) {
        f.print(list[i].data.i);
      } else if (list[i].type == TYPE_FLOAT) {
        f.print(list[i].data.f, 2);
      } else if (list[i].type == TYPE_STRING) {
        f.print(list[i].data.s);

      }

      if (i < vector_size - 1)
        f.print(", ");
    }

    f.println();
    f.close();
  } else {
    Serial.println("Erro ao abrir o arquivo.");
  }
}

static bool initSD(int chipSelect) {
  Serial.println("Inicializando cartão SD... ");
  if (!SD.begin(chipSelect)) {
    return false;
  }
  return true;
}
