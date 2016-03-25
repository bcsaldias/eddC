int current_bit = 0;
unsigned char bit_buffer = 0;

void write_bit(int bit, FILE *f)
{
	bit_buffer = (bit_buffer<<1);
  if (bit)
    bit_buffer |= 0x01;

  current_bit++;
  if (current_bit == 8)
  {
    fwrite (&bit_buffer, 1, 1, f);
    current_bit = 0;
    bit_buffer = 0;
  }
}

void flush_bits(FILE* f)
{
  while (current_bit) 
    write_bit(0, f);
}

void write_cod(char* cod, FILE *f){
	int i = 0;
	while(cod[i]){
		if(cod[i]=='1'){
			write_bit(1, f);
			//printf("Escribiendo 1\n");	
		}
		else{
			//printf("Escribiendo 0\n");
			write_bit(0, f);
		}
		i = i +1;		
	}
}

void write_length_sep(char* sep, FILE* f){
	int largo = 0;
	while(sep[largo]){
		largo = largo + 1;	
	}
	unsigned char byte1 = ((unsigned char*)(&largo))[0];
	unsigned char byte2 = ((unsigned char*)(&largo))[1];
	unsigned char byte3 = ((unsigned char*)(&largo))[2];
	fwrite(&byte3,sizeof(unsigned char), 1, f);
	fwrite(&byte2,sizeof(unsigned char), 1, f);
	fwrite(&byte1,sizeof(unsigned char), 1, f);
}

void write_color(char* char_color, FILE* f){
    int r, g, b;
    sscanf(char_color, "%d %d %d", &r, &g, &b);

    unsigned char n;
    unsigned char arr[8];

    n = ((unsigned char*)(&r))[0];   

    for(int i=0; i<8; i++) {
	    if (n & 1)
    		arr[7-i] = '1';
	    else
	    	arr[7-i] = '0';
	    n >>= 1;
	}
	write_cod(arr, f);

	n = ((unsigned char*)(&g))[0];   

    for(int i=0; i<8; i++) {
	    if (n & 1)
    		arr[7-i] = '1';
	    else
	    	arr[7-i] = '0';
	    n >>= 1;
	}
	write_cod(arr, f);

	n = ((unsigned char*)(&b))[0];   

    for(int i=0; i<8; i++) {
	    if (n & 1)
    		arr[7-i] = '1';
	    else
	    	arr[7-i] = '0';
	    n >>= 1;
	}
	write_cod(arr, f);

	//sprintf(rojo, "%s", "0");

	//printf("Rojo: %d\n", r);
	//printf("Rojo binario: %s\n", arr);
    //write_cod(rgb,f);
}
