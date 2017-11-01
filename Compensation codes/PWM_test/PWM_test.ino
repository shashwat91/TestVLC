const int ledPin1 = 5;
const int pwmWidth = 3;
const int data_bits = 8;
const int preamble_bits = 4;
const int preamble[preamble_bits] = {0,0,0,0};

byte data1 = 19;

int counter1 = 0;
int one_counter = 0;
const int packet1Size = 37;
bool led1Status = false;
byte packet1[packet1Size];

void initializeTimer()
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  int freq = 1000;
  int value = 16000000 / 256 /  freq / 2 ; // timer is 8 bits -> 256 values

  OCR1A = value;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)
{
 if(counter1 < packet1Size)
 { 
    if(packet1[counter1] == 1 && led1Status == false)
    {
      led1Status = true;
      digitalWrite(ledPin1,led1Status);
    }
    
    if(packet1[counter1] == 0 && led1Status == true)
    {
      led1Status = false;
      digitalWrite(ledPin1,led1Status);
    }
 }

  counter1++;
  if(counter1 == packet1Size)
  {
     counter1 = 0;
     data1++;
     if(data1 == 255)
      data1 =1;
     makePacket1();
  }
}

byte parity(byte x)
{
   byte y;
   y = x ^ (x >> 1);
   y = y ^ (y >> 2);
   y = y ^ (y >> 4);
   y = y ^ (y >> 8);
   y = y ^ (y >>16);
   return y & 1;
}

void compensate()
{
  /*int j = ((data_bits + 1) * pwmWidth) + preamble_bits;
  for(; j < packet1Size; ++j)// Adding compensation bits
  {
    if(one_counter < 17)
    {
      packet1[j] = 1;
      one_counter++;
    }
    else
      packet1[j] = 0;
  }*/
  
}
void makePacket1()
{
  int i,j=0;
  one_counter=0;
  for(i=0;i<data_bits;i++)
  {
    while(j<preamble_bits)
    {
      packet1[j] = preamble[j];
      j++;
    }
    
    byte tmp = (data1) & (0x80>>i);
    if(tmp == 0)
    {
      packet1[j++] = 1; //part 1 of data
      packet1[j++] = 0; //part 2 of data
      packet1[j++] = 0; //part 3 of data
      one_counter++;
    }
    else
    {
      packet1[j++] = 1; //part 1 of data
      packet1[j++] = 1; //part 2 of data
      packet1[j++] = 0; //part 3 of data
      one_counter+=2;
    }
  }
  
  if(parity(data1))
  {
    packet1[j++] = 1; //part 1 of parity
    packet1[j++] = 1; //part 2 of parity
    packet1[j++] = 0; //part 3 of parity
    one_counter+=2;
  }
  else
  {
    packet1[j++] = 1; //part 1 of parity
    packet1[j++] = 0; //part 2 of parity
    packet1[j++] = 0; //part 3 of parity
    one_counter++;
  }
  compensate();
//  printPacket();
}

void printPacket()
{
  Serial.print(data1,HEX);
  Serial.print("::");
  int i=0;
  while(i<packet1Size)
  {
    Serial.print(packet1[i]);
    i++;
  }
  Serial.println();
}

void setup()
{
  makePacket1();
  pinMode(ledPin1, OUTPUT);
  Serial.begin(115200);
  delay(5000);
  printPacket();
  initializeTimer();
}

void loop(){}

