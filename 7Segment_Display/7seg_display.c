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

#define BCM2708_PERI_BASE (0x3F000000)//decimal 1056964608
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)//decimal 1059061760
#define BLOCK_SIZE (4096)
#define MAXTIMINGS 85


struct bcm2835_peripheral
{
    int mem_fd;
    void *map;
    volatile unsigned int *addr;//address of mapped area
};
struct bcm2835_peripheral gpio;

/*
 *Mapping memory areas with mmap function will provide our program a direct access to device memory
 *Parameters: structrure for the addresses of the specified mapped memory areas
 *
 */

int map_peripheral(struct bcm2835_peripheral *p)
{
    //open the /dev/mem folder with read/write
    if((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
    {
        printf("Failed to open /dem/mem, did you sudo?\n");
        return -1;
    }
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


//setting the pin as input
void input_GPIO(int gpio_numb)
{
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
//setting all GPIOs to LOW state
void low_all_GPIO()
{
    *(gpio.addr + 10) = 1 << 4;
    *(gpio.addr + 10) = 1 << 17;
    *(gpio.addr + 10) = 1 << 27;
    *(gpio.addr + 10) = 1 << 22;
    *(gpio.addr + 10) = 1 << 5;
    *(gpio.addr + 10) = 1 << 6;
    *(gpio.addr + 10) = 1 << 13;
    *(gpio.addr + 10) = 1 << 19;
    *(gpio.addr + 10) = 1 << 26;
    *(gpio.addr + 10) = 1 << 18;
    *(gpio.addr + 10) = 1 << 23;
    *(gpio.addr + 10) = 1 << 24;
    *(gpio.addr + 10) = 1 << 25;
    *(gpio.addr + 10) = 1 << 12;
    *(gpio.addr + 10) = 1 << 16;
    *(gpio.addr + 10) = 1 << 20;
    *(gpio.addr + 10) = 1 << 21;
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

void set_GPIO_alt(int gpio_numb, int alt_funtion)
{
    int register_index = gpio_numb / 10;
    int bit = (gpio_numb % 10) * 3;

    unsigned old_value = gpio.addr[register_index];
    unsigned mask = 0b111 << bit;
    printf("Changing function of GPIO: %d from %x to %x\n", gpio_numb, (old_value >> bit) & 0b111, alt_funtion);
    gpio.addr[register_index] = (old_value & ~mask) | ((alt_funtion << bit) & mask);

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
//function to display the number in the parameter
//simply pulls the pin high of the segments we wish to turn off
void display_numb(int numb)
{
    if(numb == 0)
    {
        high_GPIO(13);
    }
    else if(numb == 1)
    {
        high_GPIO(4);
        high_GPIO(22);
        high_GPIO(5);
        high_GPIO(6);
        high_GPIO(13);
    }
    else if(numb == 2)
    {
        high_GPIO(6);
        high_GPIO(27);
    }
    else if(numb == 3)
    {
        high_GPIO(6);
        high_GPIO(5);
    }
    else if(numb == 4)
    {
        high_GPIO(5);
        high_GPIO(22);
        high_GPIO(4);
    }
    else if(numb == 5)
    {
        high_GPIO(17);
        high_GPIO(5);
    }
    else if(numb == 6)
    {
        high_GPIO(17);
    }
    else if(numb == 7)
    {
        high_GPIO(22);
        high_GPIO(5);
        high_GPIO(6);
        high_GPIO(13);
    }
    else if(numb == 9)
    {
        high_GPIO(5);
    }
    sleepMicro(4000);//small delay for multiplexing. 400000 is a good value to demonstrate multiplexing  
    low_all_GPIO();//pull all pins LOW, so it can be ready for the next number
}
//function to display more than 1 number at a time, multiplexing method used
void segment_display(int numb)
{
    //just to figure out how many digits the number has
    int numb_clone = numb;
    int numb_digits = 0;
    int index = 0;
    int array[4];
    while(numb_clone != 0)
    {
        array[index] = numb_clone % 10;
        numb_clone/=10;
        numb_digits++;
        index++;
    }
    //special case for the zero, since we can't divide by 0
    if(numb == 0)
    {
        array[0] = 0;
        numb_digits = 1;
    }
    //if the we have a single digit we pull the Anode pin high for the rightmost part of the display
    if(numb_digits == 1)
    {
        int count = 0;
        while(1)
        {
            high_GPIO(25);
            display_numb(array[0]);
            if(count == 255)
            {
                low_all_GPIO();
                break;
            }
                count++;
        }
    }
    else if(numb_digits == 2)
    {
        int count = 0;
        //the multiplexing method itself
        while(1)
        {
            high_GPIO(25);
            display_numb(array[0]);
            low_GPIO(25);
            high_GPIO(24);
            display_numb(array[1]);
            low_GPIO(24);
            if(count == 255)
            {
             low_all_GPIO();
             break;
            }
            count++;
        }
    }
    else if(numb_digits == 3)
    {
        int count = 0;
        while(1)
        {
            high_GPIO(25);
            display_numb(array[0]);
            low_GPIO(25);
            high_GPIO(24);
            display_numb(array[1]);
            low_GPIO(24);
            high_GPIO(23);
            display_numb(array[2]);
            low_GPIO(23);
            if(count == 255)
            {
                low_all_GPIO();
                break;
            }
            count++;
        }

    }
    else if(numb_digits == 4)
    {
        int count = 0;
        while(1)
        {
            high_GPIO(25);
            display_numb(array[0]);
            low_GPIO(25);
            high_GPIO(24);
            display_numb(array[1]);
            low_GPIO(24);
            high_GPIO(23);
            display_numb(array[2]);
            low_GPIO(23);
            high_GPIO(18);
            display_numb(array[3]);
            low_GPIO(18);
            if(count == 255)
            {
                low_all_GPIO();
                break;
            }
            count++;
        }

    }
}
int main(void)
{
    printf("7 segment display on a Raspberry Pi 2\n");
    map_peripheral(&gpio);

    output_GPIO(4);
    output_GPIO(17);
    output_GPIO(27);
    output_GPIO(22);
    output_GPIO(5);
    output_GPIO(6);
    output_GPIO(13);
    output_GPIO(18);
    output_GPIO(23);
    output_GPIO(24);
    output_GPIO(25);
    output_GPIO(12);
    segment_display(1234);

    unmap_peripheral(&gpio);
    return 0;
}
