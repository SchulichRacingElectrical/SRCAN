// Variables we need from inspvax: latitude, longitude, height, north vel, east vel, up vel,roll, pitch, azimuth
//Variables we need from rawimu: z accel,  neg y accel, x accel, z gyro, neg y gyro, x gyro


void setup() {
  Serial.begin(115200);
  //Serial.Serial.println("log com1 inspvax ontime 0.5 \r");
  //Serial.Serial.println("log com1 rawimu ontime 0.5 \r");
}
double Zaccel, negYaccel, Xaccel, Zgyro, negYgyro,Xgyro, latitude, longitude, height, northvel, eastvel, upvel,roll, pitch, azimuth;
void loop() {
  int commas =0;
  int semicolon =0;
  if (Serial.available() > 0) {
    printstuff();
     String data = Serial.readString();
     if (data[0] == '#' && data[1] == 'R'){
        for (int i =0; i< data.length(); i++){
          if (data[i] == ';')
            semicolon = 1;
          if (data[i] == ',' && semicolon)
            commas++;
            
          if (commas == 3)
            Zaccel = (data.substring(i+1,findindex(data,i))).toDouble(); 
          else if (commas == 4)
            negYaccel = (data.substring(i+1,findindex(data,i))).toDouble();
          else if (commas == 5)
            Xaccel = (data.substring(i+1,findindex(data,i))).toDouble();
          else if (commas == 6)
            Zgyro = (data.substring(i+1,findindex(data,i))).toDouble();
          else if (commas == 7)
            negYgyro = (data.substring(i+1,findindex(data,i))).toDouble();
          else if (commas == 8)
            Xgyro = (data.substring(i+1,findlastindex(data,i))).toDouble();
        }
        printstuff();
     }
     else if (data[0] == '#' && data[1] == 'I'){
      for(int i =0; i <data.length(); i++){
       if (data[i] == ';')
        semicolon = 1;
       if (data[i] == ',' && semicolon)
        commas++;

       if (commas == 2)
        latitude = (data.substring(i+1,findindex(data,i))).toDouble(); 
       else if (commas == 3)
        longitude = (data.substring(i+1,findindex(data,i))).toDouble();
       else if (commas == 4)
        height = (data.substring(i+1,findindex(data,i))).toDouble();
       else if (commas == 6)
        northvel = (data.substring(i+1,findindex(data,i))).toDouble();
       else if (commas == 7)
        eastvel = (data.substring(i+1,findindex(data,i))).toDouble();
       else if (commas == 8)
        upvel = (data.substring(i+1,findlastindex(data,i))).toDouble();
       else if (commas == 9)
        roll = (data.substring(i+1,findindex(data,i))).toDouble(); 
       else if (commas == 10)
        pitch = (data.substring(i+1,findindex(data,i))).toDouble();
       else if (commas == 11)
        azimuth = (data.substring(i+1,findindex(data,i))).toDouble();
      }
      printstuff();
     }
  }   

}

void printstuff(){
  Serial.println("Zaccel:"+(String) Zaccel);
Serial.println("negYaccel:"+(String)negYaccel);
Serial.println("Xaccel:"+(String) Xaccel);
Serial.println("Zgyro:"+(String) Zgyro);
Serial.println("negYgyro:"+(String) negYgyro);
Serial.println("Xgyro:"+(String) Xgyro);
Serial.println("latitude:"+(String) latitude);
Serial.println("longitude:"+(String) longitude);
Serial.println("height:"+(String)height);
Serial.println("northvel:"+(String) northvel);
Serial.println("eastvel:"+(String) eastvel);
Serial.println("upvel:"+(String) upvel);
Serial.println("roll:"+(String) roll);
Serial.println("pitch:"+(String) pitch);
Serial.println("Azimuth:"+(String) azimuth);

}
int findlastindex(String data, int i){
  while (data[i] != '*'){
    i++;
  }
  return i;
}
int findindex(String data, int i){
  while( data[i] != ','){
        i++;
  }
  return i;
}
  
