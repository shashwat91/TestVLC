const int ledPin = 11;

int counter = 0;
const int packetLength = 10;
const int silentZone = 6;
int packet[packetLength];

bool ledStatus = false;

void initializeTimer()
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  int freq = 1500;
  int value = 16000000 / 256 /  freq / 2 ; // timer is 8 bits -> 256 values

  OCR1A = value;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)
{
  if(counter < packetLength)
  {
    if(packet[counter] == 1 && ledStatus == false)
      ledStatus = true;
    
    if(packet[counter] == 0 && ledStatus == true)
      ledStatus = false;

  }
  else if(counter < packetLength+silentZone)
  {
    ledStatus = false;
  }

  digitalWrite(ledPin,ledStatus);
  counter++;
  if(counter == packetLength+silentZone)
    counter = 0;
}

void setPacket1()
{
  //StartBIT
  packet[0] = 1;
  //Data
  packet[1] = 0;
  packet[2] = 1;
  packet[3] = 0;
  packet[4] = 1;
  packet[5] = 0;
  packet[6] = 1;
  packet[7] = 0;
  packet[8] = 1;
  //Parity -- even
  packet[9] = 1;
}
void setup()
{
    pinMode(ledPin, OUTPUT);
    setPacket1();
    delay(500);
    initializeTimer();
}

void loop(){}
