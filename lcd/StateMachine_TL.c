#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "Interrupts.h"
#include "lcd.h"

struct state;
typedef void states(struct state *);

struct state
{
	states *next;

	int Ammo;
	int Healt;
};

states shoot_state, hit_state, reload_state;

void shoot_state(struct state * state)
{
	clearLcd();
	printf("PIEUW \n");
	dispString(1,0,"SHOOT!");

}

void hit_state(struct state * state)			// Software interrupt
{
	clearLcd();
	printf("YES \n");
	dispString(1,0,"HIT!");
}

void reload_state(struct state * state)
{
	clearLcd();
	dispString(0,0,"RELOAD!");
	printf("RELOAD! \n");


}

int main(void)
{
	lcdInit();
	lcdReset();

	init_interrupts(*shoot_state, *reload_state, *hit_state);


	return 0;
}
