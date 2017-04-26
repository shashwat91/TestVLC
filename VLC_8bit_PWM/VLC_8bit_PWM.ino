const int ledPin = 11;
const int ledPin2 = 10;
const int dataLength = 10;
const int pwmWidth = 3;
const int silentZone = 2 * pwmWidth;

int counter = 0;
int dataArray[dataLength];
int *packet;
int packetSize;
bool ledStatus = false;

void initializeTimer()
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  int freq = 3000;
  int value = 16000000 / 256 /  freq / 2 ; // timer is 8 bits -> 256 values

  OCR1A = value;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)
{
  if(counter < packetSize)
  {
    if(packet[counter] == 1 && ledStatus == false)
    {
      ledStatus = true;
      digitalWrite(ledPin,ledStatus);
    }
    
    if(packet[counter] == 0 && ledStatus == true)
    {
      ledStatus = false;
      digitalWrite(ledPin,ledStatus);
    }
  }
  else if(counter < packetSize+silentZone)
  {
    ledStatus = false;
    digitalWrite(ledPin,ledStatus);
  }
  
  counter++;
  if(counter == packetSize+silentZone)
    counter = 0;
}

void setData1()
{
  //StartBIT
  dataArray[0] = 1;
  //Data
  dataArray[1] = 0;
  dataArray[2] = 1;
  dataArray[3] = 0;
  dataArray[4] = 1;
  dataArray[5] = 0;
  dataArray[6] = 1;
  dataArray[7] = 0;
  dataArray[8] = 1;
  //Parity -- even
  dataArray[9] = 0;
}

void setPacket1()
{
  packetSize = dataLength * pwmWidth;
  packet = (int*) malloc(packetSize * sizeof(int));
  int i=0,j=0;
  while(i<dataLength)
  {
    if(dataArray[i] == 1)
    {
      packet[j++] = 1; //part 1 of data
      packet[j++] = 1; //part 2 of data
      packet[j++] = 0; //part 3 of data
      i++;
    }
    else
    {
      packet[j++] = 1; //part 1 of data
      packet[j++] = 0; //part 2 of data
      packet[j++] = 0; //part 3 of data
      i++;
    }
  }
}
void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    setData1();
    setPacket1();
    delay(500);
    digitalWrite(ledPin2,LOW);
    initializeTimer();
}

void loop(){}

