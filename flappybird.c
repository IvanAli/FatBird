#include <font5x7.h>
#include "arial14.h"
#include "arial_bold14.h"
#include "courier7x14.h"
#include "courier_bold14.h"
#include "script19.h"
#include <glcd.h>
#include <delay.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include "pipe.h"
#include "flappy.h"

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

const int WIDTH = 128;
const int HEIGHT = 64;
const int PIPE_N = 8;
const int PIPE_V = 35;
const int PIPE_H = 40;

int i, j;
int ok;
int bird_x, bird_y, bird_dx, bird_dy, bird_ddx, bird_ddy;
int ret;
int btn;
int pipe_x[PIPE_N], pipe_y[PIPE_N];
char pipe_seen[PIPE_N];
int pipes_dx;
int score;
char score_str[20];

void adc_init() {
    ADMUX |= 1 << REFS0;
    ADCSRA |= 1 << ADEN | 1 << ADPS2 | 1 << ADPS0;
}
 
int read_adc() {
    ADMUX |= 1 << 2;
    ADCSRA |= 1 << ADSC;
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= 1 << ADIF;
    ret = ADCL;
    ret = ret + ADCH * 256;
    return ret;
}

void update_bird() {
	int ok = 0;
	btn = read_adc();
    
    if (btn > 200) {
        bird_dy = -5;
 		ok = 1;
    }
	bird_x += bird_dx;
    bird_y += bird_dy;
    bird_dx += bird_ddx;
    bird_dy += bird_ddy;
    if (bird_x >= WIDTH) bird_x -= WIDTH;
    if (!ok && read_adc() > 200) bird_dy = -5;
    // if (bird_y >= HEIGHT) bird_y -= HEIGHT;
}

void init_bird() {
	bird_x = WIDTH >> 1;
	bird_x -= 8;
	bird_y = 20;	
    bird_ddy = 1;
}

void init_pipes() {
	pipes_dx = -5;
	for (i = 0; i < PIPE_N; i++) {
		if (i > 0) pipe_x[i] = pipe_x[i - 1] + PIPE_H;
		else pipe_x[i] = 200;
		pipe_y[i] = rand() % (HEIGHT - 50) + 10;
	}
}

void init_score() {
	score = 0;
}

void init() {
	init_bird();
    init_pipes();
    init_score();
}

void update_pipes() {
	for (i = 0; i < PIPE_N; i++) {
		pipe_x[i] += pipes_dx;
		if (pipe_x[i] < 0) {
			for (j = i; j + 1 < PIPE_N; j++) pipe_x[j] = pipe_x[j + 1], pipe_y[j] = pipe_y[j + 1], pipe_seen[j] = pipe_seen[j + 1];
			pipe_x[PIPE_N - 1] = pipe_x[PIPE_N - 2] + PIPE_H;
			pipe_y[PIPE_N - 1] = rand() % (HEIGHT - 30) + 10;
			pipe_seen[PIPE_N - 1] = 0;
		}
	}
}
/*
void draw_bird() {
	glcd_putpixel(bird_x, bird_y, 2);
}*/

void draw_bird() {
	glcd_putpixel(bird_x, bird_y, 2);
	glcd_putpixel(bird_x + 1, bird_y - 1,2);
    glcd_putpixel(bird_x + 2, bird_y - 1,2);
    glcd_putpixel(bird_x + 3, bird_y - 1,2);
    glcd_putpixel(bird_x + 3, bird_y - 2,2);
    glcd_putpixel(bird_x + 4, bird_y - 1,2);  
    glcd_putpixel(bird_x + 4, bird_y - 3,2);
    glcd_putpixel(bird_x + 5, bird_y - 3,2);
    glcd_putpixel(bird_x + 6, bird_y - 4,2);
    glcd_putpixel(bird_x + 7, bird_y - 4,2);
    glcd_putpixel(bird_x + 8, bird_y - 4,2);
    glcd_putpixel(bird_x + 9, bird_y - 4,2);
    glcd_putpixel(bird_x + 10, bird_y - 4,2);
    glcd_putpixel(bird_x + 11, bird_y - 4,2);
    glcd_putpixel(bird_x + 12, bird_y - 4,2);
    glcd_putpixel(bird_x + 11, bird_y - 3,2);
    glcd_putpixel(bird_x + 13, bird_y - 3,2); 
    glcd_putpixel(bird_x + 14, bird_y - 2,2);
    glcd_putpixel(bird_x + 10, bird_y - 2,2);
    glcd_putpixel(bird_x + 10, bird_y - 1,2);
    glcd_putpixel(bird_x + 10, bird_y, 2);
    glcd_putpixel(bird_x + 13, bird_y - 1, 2);
    glcd_putpixel(bird_x + 13, bird_y, 2);
    glcd_putpixel(bird_x + 15, bird_y - 1, 2);  
    glcd_putpixel(bird_x + 15, bird_y, 2);
    glcd_putpixel(bird_x + 15, bird_y + 1, 2);
    glcd_putpixel(bird_x + 11, bird_y + 1, 2);
    glcd_putpixel(bird_x + 11, bird_y + 2, 2);
    glcd_putpixel(bird_x + 12, bird_y + 2, 2);
    glcd_putpixel(bird_x + 13, bird_y + 2, 2);
    glcd_putpixel(bird_x + 14, bird_y + 2, 2);
    glcd_putpixel(bird_x + 15, bird_y + 2, 2);
    glcd_putpixel(bird_x + 16, bird_y + 2, 2);
    glcd_putpixel(bird_x + 10, bird_y + 3, 2);
    glcd_putpixel(bird_x + 17, bird_y + 3, 2);
    glcd_putpixel(bird_x + 9, bird_y + 4, 2);
    glcd_putpixel(bird_x + 11, bird_y + 4, 2);
    glcd_putpixel(bird_x + 12, bird_y + 4, 2);
    glcd_putpixel(bird_x + 13, bird_y + 4, 2);
    glcd_putpixel(bird_x + 14, bird_y + 4, 2);
    glcd_putpixel(bird_x + 15, bird_y + 4, 2);
    glcd_putpixel(bird_x + 16, bird_y + 4, 2);
    glcd_putpixel(bird_x + 10, bird_y + 5, 2);
    glcd_putpixel(bird_x + 16, bird_y + 5, 2);    
    glcd_putpixel(bird_x + 11, bird_y + 6, 2);
    glcd_putpixel(bird_x + 12, bird_y + 6, 2);
    glcd_putpixel(bird_x + 13, bird_y + 6, 2);
    glcd_putpixel(bird_x + 14, bird_y + 6, 2);
    glcd_putpixel(bird_x + 15, bird_y + 6, 2);
    glcd_putpixel(bird_x + 5, bird_y + 7, 2);
    glcd_putpixel(bird_x + 6, bird_y + 7, 2);
    glcd_putpixel(bird_x + 7, bird_y + 7, 2);
    glcd_putpixel(bird_x + 8, bird_y + 7, 2);
    glcd_putpixel(bird_x + 9, bird_y + 7, 2);
    glcd_putpixel(bird_x + 10, bird_y + 7, 2);
    glcd_putpixel(bird_x + 4, bird_y + 6, 2);
    glcd_putpixel(bird_x + 3, bird_y + 6, 2);
    glcd_putpixel(bird_x + 2, bird_y + 5, 2);
    glcd_putpixel(bird_x + 2, bird_y + 4, 2);
    glcd_putpixel(bird_x + 3, bird_y + 4, 2);
    glcd_putpixel(bird_x + 4, bird_y + 4, 2);
    glcd_putpixel(bird_x + 1, bird_y + 3, 2);
    glcd_putpixel(bird_x + 5, bird_y + 3, 2);
    glcd_putpixel(bird_x, bird_y + 2, 2);
    glcd_putpixel(bird_x + 6, bird_y + 2, 2);
    glcd_putpixel(bird_x + 6, bird_y + 1, 2);
    glcd_putpixel(bird_x, bird_y + 1, 2);
    glcd_putpixel(bird_x + 5, bird_y, 2);
    
}

void draw_pipes() {
	for (i = 0; i < PIPE_N; i++) {
		if (0 <= pipe_x[i] && pipe_x[i] <= WIDTH) {
			/*
			for (j = 0; j < pipe_y[i]; j++) {
				glcd_putpixel(pipe_x[i], j, 2);
			}
			for (j = pipe_y[i] + PIPE_V; j < HEIGHT; j++) {
				glcd_putpixel(pipe_x[i], j, 2);
			}
			*/
			// upper pipe
			glcd_line(pipe_x[i] - 5, 0, pipe_x[i] - 5, pipe_y[i] - 5);
			glcd_line(pipe_x[i] + 5, 0, pipe_x[i] + 5, pipe_y[i] - 5);
			glcd_line(pipe_x[i] - 6, pipe_y[i] - 5, pipe_x[i] - 6, pipe_y[i]);
			glcd_line(pipe_x[i] + 6, pipe_y[i] - 5, pipe_x[i] + 6, pipe_y[i]);
			glcd_line(pipe_x[i] - 6, pipe_y[i] - 5, pipe_x[i] + 6, pipe_y[i] - 5);
			glcd_line(pipe_x[i] - 6, pipe_y[i], pipe_x[i] + 6, pipe_y[i]);
			//lower pipe
			glcd_line(pipe_x[i] - 5, HEIGHT, pipe_x[i] - 5, pipe_y[i] + 5 + PIPE_V);
			glcd_line(pipe_x[i] + 5, HEIGHT, pipe_x[i] + 5, pipe_y[i] + 5 + PIPE_V);
			glcd_line(pipe_x[i] - 6, pipe_y[i] + 5 + PIPE_V, pipe_x[i] - 6, pipe_y[i] + PIPE_V);
			glcd_line(pipe_x[i] + 6, pipe_y[i] + 5 + PIPE_V, pipe_x[i] + 6, pipe_y[i] + PIPE_V);
			glcd_line(pipe_x[i] - 6, pipe_y[i] + 5 + PIPE_V, pipe_x[i] + 6, pipe_y[i] + 5 + PIPE_V);
			glcd_line(pipe_x[i] - 6, pipe_y[i] + PIPE_V, pipe_x[i] + 6, pipe_y[i] + PIPE_V);
			
			// glcd_line(pipe_x[i], pipe_y[i] + PIPE_V, pipe_x[i], HEIGHT);
		}
	}
}

void update_score() {
	for (i = 0; i < PIPE_N; i++) if (pipe_x[i] < bird_x && !pipe_seen[i]) score++, pipe_seen[i] = 1;
}

void update() {
    update_bird();
    update_pipes();
    update_score();
}

void draw_score() {
	sprintf(score_str, "Score: %d", score);
	glcd_outtextxy(74, 0, score_str);
//	glcd_outtextxyf(90, 0, "Score: ");
}

void draw() {
	draw_bird();
	draw_pipes();
	draw_score();
    // lcd_gotoxy(bird_x, bird_y);
    // lcd_putchar('.');
    
}

int check() {
	if (bird_y + 4 >= HEIGHT) return 1;
	for (i = 0; i < PIPE_N; i++) {
		/*if (pipe_x[i] - 3 >= bird_x - 2 && pipe_x[i] <= bird_x + 2) {
			if (bird_y <= pipe_y[i] || bird_y >= pipe_y[i] + PIPE_V) return 1;
		}*/
		if (bird_x + 16 >= pipe_x[i] - 9 && bird_x - 16 <= pipe_x[i] + 9) {
			if (bird_y - 1 <= pipe_y[i] || bird_y + 1 >= pipe_y[i] + PIPE_V) return 1;
		}
	}
	return 0;
}


void main() {
	GLCDINIT_t g_init;
	srand(8492);
    // ADC initialization
// ADC Clock frequency: 250.000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: Free Running
// Digital input buffers on ADC0: Off, ADC1: Off, ADC2: Off, ADC3: Off
// ADC4: On, ADC5: Off                                        
    
    /*
    DIDR0=(1<<ADC5D) | (0<<ADC4D) | (1<<ADC3D) | (1<<ADC2D) | (1<<ADC1D) | (1<<ADC0D);
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (0<<ADSC) | (1<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
    ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
        
    #asm("sei")
     */
    adc_init();
     
    g_init.font=font5x7;
    /* No need for reading data from external memory */
    g_init.readxmem = NULL;

    /* No need for reading data from external memory */ 
    g_init.writexmem = NULL;

    /* Initialize the LCD controller and graphics */
    glcd_init(&g_init); 

    //glcd_outtextxyf(0, 20, "Standard 5x7");

//    glcd_setfont(arial14);
//    glcd_outtextxyf(0, 30, "Arial 14\n");
    /*
    bird = (Bird*) malloc(sizeof(Bird));
    bird->x = WIDTH >> 1;
    bird->y = 0;
    bird->dx = 1;
    bird->dy = 0;
    bird->ddx = 0;
    bird->ddy = 0;
     */
    init();
    
    ok = 1;
    // glcd_putimagef(0, 0, imagen3, GLCD_PUTCOPY);
    // delay_ms(1000);
    glcd_outtextxyf(WIDTH / 2 - 25, HEIGHT / 2 - 10, "Fat Bird");
    glcd_outtextxyf(WIDTH / 2 - 50, HEIGHT / 2 + 10, "Presiona el boton");
    while (true) {
    	btn = read_adc();
    	if (btn > 20) break;
    }
    while (true) {
        // delay_ms(10);
        // lcd_clear();
        glcd_clear();
        if (!ok) {
        	glcd_outtextxyf(WIDTH / 2 - 25, HEIGHT / 2 - 20, "Game over");
        	glcd_outtextxy(WIDTH / 2 - 30, HEIGHT / 2 - 4, score_str);
			glcd_outtextxyf(WIDTH / 2 - 35, HEIGHT / 2 + 4, "Reintentar?");
        	while (true) {
        		btn = read_adc();
        		if (btn > 20) {
        			ok = 1;
        			break;
        		}
        	}
        	if (ok) {
        		init();
        		continue;
        	}
        }
    
        update();
        draw();
        if (check()) ok = 0;
        // btn = read_adc();
                
    }
}
