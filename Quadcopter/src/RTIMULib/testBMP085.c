int main(int argc, char **argv)
{
    unsigned int temperature, pressure;

	bmp085_Calibration();
	temperature = bmp085_GetTemperature(bmp085_ReadUT());
	pressure = bmp085_GetPressure(bmp085_ReadUP());

	printf("Temperature\t%0.1f%cC\n", ((double)temperature)/10,0x00B0);
	printf("Pressure\t%0.2fhPa\n", ((double)pressure)/100);
	return 0;
}
