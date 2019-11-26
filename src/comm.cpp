#include "comm.h"

uint8_t state=0;
uint8_t ind=0;
uint8_t tmp_var=0;
char comms[32];

uint8_t loadarrayfromstring_hex(uint8_t a[],char s[]);
uint8_t char_to_hex(const char c);

void get_available_commands(void){
  //TODO
}
void process_input(){
  if(uart.rx_available_command()){
    state=1;
    // there is another command to read
    // do the switch case to find what to do

    // get the message
    uart.rx_command(comms);

    if (strncmp(comms,"help",4)==0){
      uart.print("Raey here, commands are:\npulse [n=0] [d=0]\n"
      "laser [on/tog/OFF]\nmeasure\ntemp apd/uc\ngain [g=0]\n"
      "read spi\nstatus\n");
      uart.print("8=D\n");
    }

    else if (strncmp(comms,"pulse",5)==0){
      uint8_t n = stringtoint(comms+nospace_after(comms,0));
      uint8_t d = stringtoint(comms+
        nospace_after(comms,nospace_after(comms,0)));

      if (n==0 && d==0){
        //activate laserpulsing
        util_reg^=(1<<UTIL_PULSE_ENABLE);
      }
      else if (n!=0){
        while(n!=0){
          if (d==0){
            laser_pulse();
          }
          else{
            laser_pulse_n(d);
          }
          n--;
        }
      }
    }

    else if (strncmp(comms,"laser",5)==0){
      if (strncmp(comms+nospace_after(comms,0),"on",2)==0){
        LASER_ON();
      }
      else if(strncmp(comms+nospace_after(comms,0),"tog",3)==0){
        LASER_TOG();
      }
      else{
        LASER_OFF();
      }
    }

    else if (strncmp(comms,"gain",4)==0){
      uint8_t g = stringtoint(comms+nospace_after(comms,0));
      vga_set_gain(g);
    }

    else if (strncmp(comms,"temp",4)==0){
      if (strncmp(comms+nospace_after(comms,0),"apd",3)==0){
        adc_onetime(ADCWhich::DTEMP);
      }
      else if (strncmp(comms+nospace_after(comms,0),"uc",2)==0){
        adc_onetime(ADCWhich::DTEMP);
      }
      else{
        uart.print("Sensor not present\n");
      }
    }

    else if (strncmp(comms,"measure",7)==0){
      uart.print("Maybesure\n");
      // measure(comms+nospace_after(comms,0));
    }

    else if (strncmp(comms,"readspi",7)==0){
      uart.print("spi.pack_size=");
      uart.tx_uint(spi.pack_size());
      uart.print("\n");
      for (uint8_t i=0; i<spi.pack_size();++i){
        uart.tx_hex(spi.buf[i]);
        uart.tx_byte(' ');
      }
      uart.print("\n");
    }

    else if (strncmp(comms,"writespi",8)==0){
      uint8_t startdata=nospace_after(comms,nospace_after(comms,0));
      uint8_t size=loadarrayfromstring_hex(spi.buf,comms+startdata);

      if (strncmp(comms+nospace_after(comms,0),"tdc",3)==0){
        spi.tx(SPIWhich::TDC,size);
      }
      else{
        uart.print("Don't know where to write\n");
      }
    }

    else if (strncmp(comms,"debug",5)==0){
      uart.tx_hex('c');
    }

    else {
      uart.print("Did not understand\n");
    }
  }
}

uint8_t loadarrayfromstring_hex(uint8_t a[],char s[]){
  // Load the string to the array given and return the number of bytes
  // loaded

  uint8_t i=0;
  uint8_t tmp=0;
  while (uint8_t v=char_to_hex(*(s+i))) {
    if (i%2==0){
      tmp=v;
    }
    else {
      a[i/2]=(tmp<<4)+v;
      tmp=0;
    }
    ++i;
  }
  return (i+1)/2;
}
uint8_t char_to_hex(const char c){
  if (c>='0' && c<='9')
    return c-'0';
  else if (c>='a' && c<='f')
    return c-'a'+10;
  else if (c>='A' && c<='F')
    return c-'A'+10;
  else //if (c==0)
    return 0;
}
