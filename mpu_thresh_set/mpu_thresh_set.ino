#include <basicMPU6050.h>
basicMPU6050<> imu;
float defstate=0;
int flag=0;
void setup() {
  imu.setup();
  //imu.setBias();
  Serial.begin(9600);
//  imu.updateBias();
//  Serial.print( imu.ax() );
//  Serial.print( " " );
//  Serial.print( imu.ay() );
//  Serial.print( " " );
//  Serial.print( imu.az() );
//  Serial.print( "    " );
//  delay(1000);
//  imu.updateBias();
//  defstate = imu.ax() * imu.ax() + imu.ay() * imu.ay(); //+ imu.az() * imu.az();
//  float a=sqrt(defstate);
//  a*=100;
//  Serial.print("set kar diya accel ");
//  Serial.println(a);

}
float th=0.13;
void loop() {
  imu.updateBias();
  Serial.print( imu.ax() );
  Serial.print( " " );
  Serial.print( imu.ay() );
  Serial.print( " " );
  Serial.print( imu.az() );
  Serial.print( "    " );
  float magnitude = imu.ax() * imu.ax() + imu.ay() * imu.ay(); //+ imu.az() * imu.az();
  Serial.print(magnitude);
  Serial.print("   ");
  float value = sqrt(magnitude);
  Serial.print(value);
  float x=(abs(value-th))*100;
  int y=x;
  Serial.print(" x= ");
  Serial.print(y);
  if (y>1){
    
  
  Serial.print("   ");
  Serial.print("Magnitude calculated: ");
  Serial.print(y);
  Serial.print("   ");
  }
  Serial.println("   ");
////  if (value < 0)
////    value = -1 * value;
//  value *= 100;
//  if (flag==0)
//  {
//    defstate=value;
//    Serial.print("set kar diya accel ");
//  Serial.println(defstate);
//    flag++;
//  }
//  int s = map(value, defstate-1, defstate+20, 0, 10);
//  Serial.println(s);
  delay(100);
  
}
