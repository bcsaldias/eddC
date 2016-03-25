int current_bit = 0;
unsigned char bit_buffer;

FILE *f;

void write_bit (int bit)
{
  if (bit)
    bit_buffer |= (1<<current_bit);

  current_bit++;
  if (current_bit == 8)
  {
    fwrite (&bit_buffer, 1, 1, f);
    current_bit = 0;
    bit_buffer = 0;
  }
}

void flush_bits (void)
{
  while (current_bit) 
    WriteBit (0);
}

void write_cod(char* cod){
	int index = 0;
	while(cod[i]){
		printf("%c\n", cod[i]);
	}
	printf("final\n");
}