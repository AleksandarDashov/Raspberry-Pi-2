#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BCM2708_BASE (0x3F000000)
#define GPIO_BASE (BCM2708_BASE + 0x200000) //0x3F200000
#define BLOCK_SIZE (4096)
#define MAXTIMINGS 85

struct bcm2835_peripheral
{
    int mem_fd;// "/dev/mem"s file descriptor
    void *map; //pointer to mapped area
    volatile unsigned int *addr;//address of mapped area
};
/*
 *Mapping memory areas with mmap function will provide our program a direct access to device memory
 *Parameters: structrure for the addresses of the specified mapped memory areas
 */

int map_peripheral(struct bcm2835_peripheral *p)
{
    //open the /dev/mem folder with read/write
    if((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
    {
        printf("Failed to open /dem/mem, did you sudo?\n");
        return -1;
    } 
    /*Creates a new mapping in the virtual address space of the calling process
     *Returns a pointer to the mapped area
     */
    p->map = mmap(NULL,BLOCK_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,p->mem_fd, GPIO_BASE);

    if(p->map == MAP_FAILED)
    {
        printf("mmap failed, MAP_FAILED\n");
        return -1;
    }
    close(p->mem_fd);
    p->addr = (volatile unsigned int *)p->map;

    return 0;
}
void unmap_peripheral(struct bcm2835_peripheral *p)
{
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

struct bcm2835_peripheral gpio;
//setting the pin as input
void input_GPIO(int gpio_numb)
{
	// int register_index = gpio_numb/10;
    *(gpio.addr + ((gpio_numb)/10)) &= ~(7<<(((gpio_numb)%10)*3));
}
//setting the pin as output
void output_GPIO(int gpio_numb)
{
    input_GPIO(gpio_numb);
    *(gpio.addr + ((gpio_numb)/10)) |=  (1<<(((gpio_numb)%10)*3));
}
//setting the pin to HIGH
void high_GPIO(int gpio_numb)
{
    *(gpio.addr + 7) = 1 << gpio_numb;
}
//setting the pin to LOW
void low_GPIO(int gpio_numb)
{
	*(gpio.addr + 10) = 1 << gpio_numb;
}
//reading the state of the pin, 1 if HIGH, 0 if LOW
int read_GPIO(int gpio_numb)
{
	if(  *(gpio.addr + 13) & (1 << gpio_numb) )
	{
		return 1;
	}
	else
		return 0;
}


//this function is going to delay time in microseconds
void sleepMicro(unsigned int time)
{
	struct timeval now;
	struct timeval period;
	struct timeval end;

	gettimeofday(&now, NULL);
	period.tv_sec = time / 1000000;
	period.tv_usec = time % 1000000;
	timeradd(&now, &period, &end);
	while(timercmp(&now, &end, <))
	{
		gettimeofday(&now, NULL);
	}
}

uint8_t dht11_data[5]={0,0,0,0,0};//changed
void read_DHT11()
{
    //TODO make the function to return value to the parameteres, reference
    //TODO how to convert char floats to floats
    uint8_t laststate = 1;
    uint8_t counter = 0;
    uint8_t j = 0;
    uint8_t i = 0;
    float farenheit;

    /* From the DHT11 datasheet we read */
    dht11_data[0] = 0;  //The 8bit humidity integer data
    dht11_data[1] = 0;  //The 8bit humidity decimal data
    dht11_data[2] = 0;  //The 8bit temperature integer data
    dht11_data[3] = 0;  //The 8bit temperature decimal data
    dht11_data[4] = 0;  //The 8bit parity bit

    /* Sending initialization signal  */
    output_GPIO(4);/* set the pin as output */
    low_GPIO(4); /* pull the pin LOW  */
    sleepMicro(18 * 1000);/* delay for 18 milliseconds which are 18000 microseconds */
    high_GPIO(4); /* pull the pin HIGH */
    sleepMicro(40);/* delay for 40 microseconds */
    input_GPIO(4);/* set the pin as input so we can read output from the sensor */
    for(i = 0;i < MAXTIMINGS;i++)
    {
        counter = 0;
        while(read_GPIO(4) == laststate)
        {
            counter++;
            sleepMicro(1); //delay for 1 micro second;
            if(counter == 255)
            {
                break;
            }
        }
    laststate = read_GPIO(4);
    if(counter == 255)
    {
        break;
    }
    // top 3 transistions are ignored
    if( (i >= 4) && (i % 2 == 0) )
    {
        dht11_data[j/8] <<= 1;
        if(counter > 16)
        {
            dht11_data[j/8] |= 1;
        }
        j++;
    }
  }
    // Check if the data is correct acording to datasheet
    if( (j >= 40) && (dht11_data[4] == ((dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) & 0xFF)))
    {
        farenheit = dht11_data[2]*9./5.+32;
        printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",dht11_data[0],dht11_data[1],dht11_data[2],dht11_data[3],farenheit);
    }
    else
    {
        printf("Data not correct!\n");
    }
}

int main()//changed
{
    printf("DHT11 on a Raspberry Pi 2\n");
	map_peripheral(&gpio);
    while(1)
    {
        read_DHT11();
        sleep(1);
    }

    return 0;
}
