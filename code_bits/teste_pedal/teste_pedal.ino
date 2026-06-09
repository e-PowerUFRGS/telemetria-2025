const int PEDAL1 = A8;
const int PEDAL2 = A9;
const int BOTAO = 3;

//funcao y = ax^c
const double a1 = double(0.0446);
const double a2 = double(0.0241);
const double c1 = double(3.03);
const double c2 = double(3.26);


const float VREF = 5.0;
const float ADC_BIT = 10.0;
const float ADC_STEPS = (1 << int(ADC_BIT)) - 1;

volatile bool registre;
long amostras;
double media1;
double media2;

void fim();

void setup() {
  // put your setup code here, to run once:
  pinMode(PEDAL1, INPUT_PULLUP);
  pinMode(PEDAL2, INPUT_PULLUP);
  pinMode(BOTAO, INPUT);
  attachInterrupt(digitalPinToInterrupt(BOTAO), fim, RISING);
  registre = true;
  Serial.begin(9600);
  amostras = 0;
  media1 = 0;
  media2 = 0;
  delay(4000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(registre == true){
    double val1, val2, volt1, volt2, por1, por2;
    val1 = analogRead(PEDAL1);
    val2 = analogRead(PEDAL2);
    volt1 = (val1/ADC_STEPS) * VREF;
    volt2 = (val2/ADC_STEPS) * VREF;
    if(volt1 >= 4.5 || volt2 >= 4.5){
      Serial.println("FALHA NO PEDAL");
    }
    if(amostras == 0){
      media1 = volt1;
      media2 = volt2;
    }
    else{
      media1 = amostras * media1 + volt1;
      media1 = media1 / (amostras +1);
      media2 = amostras * media2 + volt2;
      media2 = media2 / (amostras + 1);
    }
    //por1 = a1 * pow(volt1, c1);
    //por2 = a2 * pow(volt2, c2);
    //Serial.println(por1, 2);
    //Serial.println(por2, 2);
    Serial.println(amostras);
    amostras++;
  }
  else{
    Serial.println();
    Serial.print(amostras);
    Serial.print(";");
    Serial.print(media1, 4);
    Serial.print(";");
    Serial.print(media2, 4);
    while(true);
  }
}

void fim(){
  registre = false;
}
