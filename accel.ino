 #include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

float t_new, t_old = 0.0f, v_new, v_old = 0.0f, x_new, x_old = 0.0f, delta_t;
float a_off;
float max_static_Acc ,min_static_Acc ;
float Distance;
float a_fix,a_array[10];
int i = 0;
char dbgmsg[200];
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");
  //Serial.println(F_CPU);
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
   caccel.setRange(ADXL345_RANGE_16_G);
   //accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  //accel.setRange(ADXL345_RANGE_2_G);
  
  /* Display some basic information on this sensor */
  //displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  //displayDataRate();
  //displayRange();
  Serial.println("");

  Serial.println("Please hold the accelerometer still for a couple seconds!");
  delay(3000);

  sensors_event_t event; 
  accel.getEvent(&event);
  a_off = event.acceleration.x;

  pinMode(3,OUTPUT);
  digitalWrite(3,LOW);
  pinMode(2,INPUT);
  digitalWrite(2,HIGH);
}

void loop(void) 
{

  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);

  t_new = micros()*1e-3;
  delta_t = t_new-t_old;

  a_fix = event.acceleration.x-a_off; // Corrected acceleration

  a_fix = (a_fix > 0.35) ? a_fix : 0.0f;
 /* for(i=0;i<10;i++)
  {
    sensors_event_t event; 
    accel.getEvent(&event);
    a_fix = event.acceleration.x-a_off; // Corrected acceleration
    a_array[i]=a_fix;
  }
  a_fix = 0;
  for(i=0;i<10;i++)
  {
   a_fix += a_array[i];
  }
  a_fix /= 10;
  a_fix = (a_fix > 0.25) ? a_fix : 0.0f;
  */
  v_new = delta_t*a_fix;

  if(digitalRead(2)==0)
  {
    x_new = x_old + delta_t*v_new;
  }
  
  if(max_static_Acc<a_fix)
  {
    max_static_Acc=a_fix;
  }
    if(min_static_Acc>a_fix)
  {
    min_static_Acc=a_fix;
  }
  
  Distance = map(x_new,0,45000,0,22);
  //if(digitalRead(2)==1)
  //{
    //Serial.print("SW: ");Serial.print(digitalRead(2));
    //Serial.print(" T: "); Serial.print(delta_t);// Serial.print(" s, ");
    Serial.print(" X: "); Serial.print(x_new); //Serial.print(" m, ");
    Serial.print(" D: "); Serial.print(Distance); //Serial.print(" m, ");

    //Serial.print(" V: "); Serial.print(v_new); //Serial.print(" m/s, ");
    Serial.print(" A: "); Serial.print(a_fix); //Serial.print(" m/s^2, ");
    //Serial.print(" MaxA: "); Serial.print(max_static_Acc); //Serial.print(" m/s^2, ");
    //Serial.print(" MinA: "); Serial.print(min_static_Acc); //Serial.println(" m/s^2, ");  
    //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
    //delay(500);

    Serial.println("");
  //}

  
  if(Serial.available())
   {
    if(Serial.read()=='c')
      {
      x_new = 0;
      }
    }
  t_old = t_new;
  x_old = x_new;
  v_old = v_new;

  
}
