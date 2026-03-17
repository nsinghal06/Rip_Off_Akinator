/* category_select.c
 * reads SW3:0 from the 0xFF200040
 * returns: 0/0001 = Food, 1/0010 = Animal, 2/0100 = Country, 3/1000 = Movie, -1/1111 = none/invalid
 */

#define SW_BASE 0xFF200040

int read_category(void) {
    volatile int *SW_ptr = (int *) SW_BASE;
    int sw = *SW_ptr;         // lw from switches data register 
    sw = sw & 0xF;            // mask bits 3:0, ignore SW9:4  

    if (sw == 0x1) return 0;  // 0001 = Food    
    if (sw == 0x2) return 1;  // 0010 = Animal  
    if (sw == 0x4) return 2;  // 0100 = Country 
    if (sw == 0x8) return 3;  // 1000 = Movie   
    return -1;                
}