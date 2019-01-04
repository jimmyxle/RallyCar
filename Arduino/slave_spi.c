#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBBR F_CPU/16/BAUD-1

int main(void)
{
  USART_init(MYUBBR);
  SPI_SLAVE_init();
  LED_init();
  USART_println( "Initiated." );

  unsigned int greet = 49;
  unsigned long result;
  
  //USART_println("Sending: ");
  USART_println( greet );
while(1)
  {
    LED_on();
    USART_println( "Loop started. " );

   
    _delay_ms(100);

    PORTD &= ~(1 << 4);
    result = SPI_tranceiver(greet);
    USART_printnum( result );
    
    if(result == '9')
        PORTD &= ~(1<<5);

    result = SPI_tranceiver(greet);
    USART_printnum( result );

    result = SPI_tranceiver(greet);
    USART_printnum( result );
    
    USART_println( "Loop ended." );
    PORTD &= ~(1<<6);

    _delay_ms(100);

  }

  return 0;
}


void USART_init(unsigned int ubrr)
{
  //shift out first 8 bits to clear it out ?
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)(ubrr);
  #if USE_2X
    UCSR0A |= (_BV(U2X0);
  #else
    UCSR0A &= ~(_BV(U2X0));
  #endif
    
  UCSR0B = ((1 << RXEN0) | (1 << TXEN0)); //enable rx and tx
  UCSR0C = ((1 << USBS0) | (3 << UCSZ00));  //set frame to be 8 bits and two stop bits 
  
}
void USART_transmit(unsigned char data)
{
  while(!(UCSR0A & ( 1<<UDRE0)));
  UDR0 = data;
}
uint8_t USART_receive(void)
{
  unsigned char stat, resh, resl;
  
  while(!(UCSR0A&(1<<RXC0)));
  stat = UCSR0A;
  resh = UCSR0B;
  resl = UDR0;
  if(stat & (1<FE0)|(1<<DOR0)|(1<<UPE0))
  return -1;
  resh = (resh >> 1) & 0x01;
  return ((resh << 8 ) | resl);
}

void USART_println(const char *s)
{
    while(*s)
    {
      USART_transmit(*s);
      s++;
    }
    USART_transmit('\n');
}

void USART_printnum(const char *s)
{
  USART_transmit(*s); 

   USART_transmit('\n');
}

void USART_flush(void)
{
  unsigned char dummy;
  while( UCSR0A & ( 1<<RXC0))
  dummy = UDR0;
}
void SPI_SLAVE_init(void)
{
  DDRB = (1<<4);
  DDRB &= ~((1<<3)|(1<<5)|(1<<2));
  SPCR = (1<<SPE);
}


/* spi usart */

uint8_t SPI_tranceiver(uint8_t data)
{
  SPDR = data;
  while(!(SPSR&(1<<SPIF)));
  return SPDR;
}

uint8_t SPI_receiver()
{
  //SPDR = data;
  while(!(SPSR&(1<<SPIF)));
  return SPDR;
}
void LED_init()
{
  DDRD |= (1<<4)|(1<<5)|(1<<6);
}

void LED_on()
{
    PORTD |= (1<<4)|(1<<5)|(1<<6);
}
